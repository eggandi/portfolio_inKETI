/**
* @file
* @brief 클라우드로의 상태데이터 업로드 기능 구현
* @date 2024-12-01
* @author user
*/


// 시스템 헤더파일
#include <sys/timerfd.h>

// 모듈 헤더파일
#include "udmem_kadap.h"
#include "cJSON/cJSON.h"


/**
 * @brief 클라우드로의 상태데이터 업로드 타이머를 초기화한다.
 * @param[in] mib 모듈 관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int UDMEM_KADAP_InitStatusDataCloudUploadTimer(UDMEM_KADAP_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize status data cloud upload timer\n");

  /*
   * 타이머를 생성한다.
   */
  mib->cloud_if.status_data_upload.tx.timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
  if (mib->cloud_if.status_data_upload.tx.timerfd < 0) {
    ERR("Fail to initialize status data cloud upload timer - timerfd_create() failed - %m\n");
    return -1;
  }

  /*
   * 타이머 주기를 설정한다.
   */
  struct itimerspec ts;
  long usec = mib->cloud_if.status_data_upload.interval;
  ts.it_value.tv_sec = (time_t)(usec / 1000000);
  ts.it_value.tv_nsec = (long)((usec % 1000000) * 1000);
  ts.it_interval.tv_sec = (time_t)(usec / 1000000);
  ts.it_interval.tv_nsec = (long)((usec % 1000000) * 1000);
  int ret = timerfd_settime(mib->cloud_if.status_data_upload.tx.timerfd, TFD_TIMER_ABSTIME, &ts, NULL);
  if (ret < 0) {
    ERR("Fail to initialize status data cloud upload timer - timerfd_settime() failed - %m\n");
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize status data cloud upload timer\n");
  return 0;
}


/**
 * @brief 업로드할 상태데이터의 유효성을 체크한다. (업로드하기에 충분한지)
 * @param[in] lf_hub LF Hub 상태데이터
 * @param[in] hf_hub HF Hub 상태데이터
 * @return 업로드할 상태데이터가 유효한지 여부
 *
 * @note 필수데이터가 없으면 업로드하는 의미가 없으므로, 필수데이터가 모두 있는지 확인한다.
 */
static bool UDMEM_KADAP_CheckUploadStatusDataValidity(KVHLFHubData *lf_hub, KVHHFHubData *hf_hub)
{
  LOG(kKVHLOGLevel_Event, "Check upload status data validity\n");

  bool valid = true;

  /*
   * LF Hub 데이터에 시스템정보가 없으면 carid를 알 수 없다.
   */
  if (!lf_hub->present.system) {
    ERR("Check upload status data validity - no Hub system data\n");
    valid = false;
  }

  /*
   * 필수 GNSS 데이터가 있어야 한다.
   */
  if ((hf_hub->present.gnss == false) ||
      (KVH_IsValidTimestamp(hf_hub->gnss.timestamp) == false) ||
      (KVH_IsValidGNSSFix(hf_hub->gnss.fix) == false) ||
      (KVH_IsValidGNSSLatitude(hf_hub->gnss.lat) == false) ||
      (KVH_IsValidGNSSLongitude(hf_hub->gnss.lon) == false) ||
      (KVH_IsValidGNSSElevation(hf_hub->gnss.elev) == false) ||
      (KVH_IsValidGNSSSpeed(hf_hub->gnss.speed) == false) ||
      (KVH_IsValidGNSSHeading(hf_hub->gnss.heading) == false)) {
    ERR("Check upload status data validity - Invalid Hub gnss data\n");
    valid = false;
  }

  /*
   * 3D-fix가 된 상태여야 한다.
   */
  if (hf_hub->gnss.fix < kKVHGNSSFix_3DFix) {
    ERR("Check upload status data validity - Not 3D-fix, current: %d\n", hf_hub->gnss.fix);
    valid = false;
  }

  return valid;
}


/**
 * @brief "carid" 데이터를 json 오브젝트에 추가한다.
 * @param[in] carid carid 값
 * @param[out] obj json 오브젝트
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int UDMEM_KADAP_AddJSON_Carid(char *carid, cJSON *obj)
{
  LOG(kKVHLOGLevel_Event, "Add carid to json object\n");

  if (cJSON_AddStringToObject(obj, "carid", carid) == NULL) {
    ERR("Fail to add carid to json object - cJSON_AddStringToObject(carid)\n");
    return -1;
  }

  LOG(kKVHLOGLevel_Event, "Success to add carid to json object\n");
  return 0;
}


/**
 * @brief "timestamp" 관련 데이터를 json 오브젝트에 추가한다.
 * @param[in] ts timestamp 값
 * @param[out] obj json 오브젝트
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int UDMEM_KADAP_AddJSON_Timestamp(KVHTimestamp ts, cJSON *obj)
{
  LOG(kKVHLOGLevel_Event, "Add timestamp to json object\n");

  /*
   * timestamp 데이터를 추가한다.
   */
  if (cJSON_AddNumberToObject(obj, "timestamp", ts) == NULL) {
    ERR("Fail to add timestamp to json object - cJSON_AddNumberToObject(timestamp)\n");
    return -1;
  }

#if 1
  /*
   * date_time 데이터를 추가한다.
   */
  time_t sec = (time_t)(ts / 1000ULL);
  int msec = (int)(ts % 1000ULL);
  struct tm *tm_now = localtime(&sec);
  char date_time[UDMEM_KADAP_DATE_TIME_STR_LEN+1];
  memset(date_time, 0, sizeof(date_time));
  sprintf(date_time, "%04d-%02d-%02dT%02d:%02d:%02d.%03d",
          tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday,
          tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, msec);
  if (cJSON_AddStringToObject(obj, "date_time", date_time) == NULL) {
    ERR("Fail to add timestamp to json object - cJSON_AddStringToObject(date_time)\n");
    return -1;
  }
#else
  /*
   * year, month, day, hour, min, sec, msec 데이터를 추가한다.
   */
  time_t sec = (time_t)(ts / 1000ULL);
  int msec = (int)(ts % 1000ULL);
  struct tm *tm_now = localtime(&sec);
  char buf[5];
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%04d", tm_now->tm_year+1900);
  if (cJSON_AddStringToObject(obj, "year", buf) == NULL) {
    ERR("Fail to add timestamp to json object - cJSON_AddStringToObject(year)\n");
    return -1;
  }
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%02d", tm_now->tm_mon+1);
  if (cJSON_AddStringToObject(obj, "month", buf) == NULL) {
    ERR("Fail to add timestamp to json object - cJSON_AddStringToObject(month)\n");
    return -1;
  }
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%02d", tm_now->tm_mday);
  if (cJSON_AddStringToObject(obj, "day", buf) == NULL) {
    ERR("Fail to add timestamp to json object - cJSON_AddStringToObject(day)\n");
    return -1;
  }
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%02d", tm_now->tm_hour);
  if (cJSON_AddStringToObject(obj, "hour", buf) == NULL) {
    ERR("Fail to add timestamp to json object - cJSON_AddStringToObject(hour)\n");
    return -1;
  }
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%02d", tm_now->tm_min);
  if (cJSON_AddStringToObject(obj, "min", buf) == NULL) {
    ERR("Fail to add timestamp to json object - cJSON_AddStringToObject(min)\n");
    return -1;
  }
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%02d", tm_now->tm_sec);
  if (cJSON_AddStringToObject(obj, "sec", buf) == NULL) {
    ERR("Fail to add timestamp to json object - cJSON_AddStringToObject(sec)\n");
    return -1;
  }
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%03d", msec);
  if (cJSON_AddStringToObject(obj, "msec", buf) == NULL) {
    ERR("Fail to add timestamp to json object - cJSON_AddStringToObject(msec)\n");
    return -1;
  }
#endif

  LOG(kKVHLOGLevel_Event, "Success to add timestamp to json object\n");
  return 0;
}


/**
 * @brief "GNSS" 데이터를 json 오브젝트에 추가한다.
 * @param[in] gnss GNSS 데이터
 * @param[out] obj json 오브젝트
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int UDMEM_KADAP_AddJSON_GNSS(KVHGNSSData *gnss, cJSON *obj)
{
  LOG(kKVHLOGLevel_Event, "Add GNSS data to json object\n");

#if 1
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  KVHTimestamp timestamp = KVH_ConvertTimespecToTimestamp(&ts);
  if (UDMEM_KADAP_AddJSON_Timestamp(timestamp, obj) < 0) {
    return -1;
  }
#else
  if (UDMEM_KADAP_AddJSON_Timestamp(gnss->timestamp, obj) < 0) {
    return -1;
  }
#endif

  if (cJSON_AddNumberToObject(obj, "lat", gnss->lat) == NULL) {
    ERR("Fail to add GNSS data to json object - cJSON_AddNumberToObject(lat)\n");
    return -1;
  }
  if (cJSON_AddNumberToObject(obj, "lon", gnss->lon) == NULL) {
    ERR("Fail to add GNSS data to json object - cJSON_AddNumberToObject(lon)\n");
    return -1;
  }
  if (cJSON_AddNumberToObject(obj, "elev", gnss->elev) == NULL) {
    ERR("Fail to add GNSS data to json object - cJSON_AddNumberToObject(elev)\n");
    return -1;
  }
  if (cJSON_AddNumberToObject(obj, "speed", gnss->speed) == NULL) {
    ERR("Fail to add GNSS data to json object - cJSON_AddNumberToObject(speed)\n");
    return -1;
  }
  if (cJSON_AddNumberToObject(obj, "heading", gnss->heading) == NULL) {
    ERR("Fail to add GNSS data to json object - cJSON_AddNumberToObject(heading)\n");
    return -1;
  }

  KVHAcceleration accel = kKVHAcceleration_Unknown;
  if (KVH_IsValidGNSSAcceleration(gnss->accel_lon)) {
    accel = gnss->accel_lon;
  }
  if (cJSON_AddNumberToObject(obj, "accel_lon", accel) == NULL) {
    ERR("Fail to add GNSS data to json object - cJSON_AddNumberToObject(accel_lon)\n");
    return -1;
  }

  accel = kKVHAcceleration_Unknown;
  if (KVH_IsValidGNSSAcceleration(gnss->accel_lat)) {
    accel = gnss->accel_lat;
  }
  if (cJSON_AddNumberToObject(obj, "accel_lat", accel) == NULL) {
    ERR("Fail to add GNSS data to json object - cJSON_AddNumberToObject(aceel_lat)\n");
    return -1;
  }

  KVHVerticalAcceleration accel_vert = kKVHVerticalAcceleration_Unknown;
  if (KVH_IsValidGNSSVerticalAcceleration(gnss->accel_vert)) {
    accel_vert = gnss->accel_vert;
  }
  if (cJSON_AddNumberToObject(obj, "accel_vert", accel_vert) == NULL) {
    ERR("Fail to add GNSS data to json object - cJSON_AddNumberToObject(accel_vert)\n");
    return -1;
  }

  KVHYawrate yawrate = kKVHYawrate_Unknown;
  if (KVH_IsValidGNSSYawrate(gnss->yawrate)) {
    yawrate = gnss->yawrate;
  }
  if (cJSON_AddNumberToObject(obj, "yawrate", yawrate) == NULL) {
    ERR("Fail to add GNSS data to json object - cJSON_AddNumberToObject(yawrate)\n");
    return -1;
  }

  KVHGNSSFix fix = kKVHGNSSFix_Unknown;
  if (KVH_IsValidGNSSFix(gnss->fix)) {
    fix = gnss->fix;
  }
  if (cJSON_AddNumberToObject(obj, "fix", fix) == NULL) {
    ERR("Fail to add GNSS data to json object - cJSON_AddNumberToObject(fix)\n");
    return -1;
  }

  KVHNetRTKFix net_rtk_fix = kKVHNetRTKFix_Unknown;
  if (KVH_IsValidNetRTKFix(gnss->net_rtk_fix)) {
    net_rtk_fix = gnss->net_rtk_fix;
  }
  if (cJSON_AddNumberToObject(obj, "net_rtk_fix", net_rtk_fix) == NULL) {
    ERR("Fail to add GNSS data to json object - cJSON_AddNumberToObject(net_rtk_fix)\n");
    return -1;
  }

  LOG(kKVHLOGLevel_Event, "Success to add GNSS data to json object\n");
  return 0;
}


/**
 * @brief "ADS" 관련 데이터를 json 오브젝트에 추가한다.
 * @param[in] lf_ads LF ADS 데이터
 * @param[in] hf_ads HF ADS 데이터
 * @param[out] obj json 오브젝트
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int UDMEM_KADAP_AddJSON_ADS(KVHLFADSData *lf_ads, KVHHFADSData *hf_ads, cJSON *obj)
{
  LOG(kKVHLOGLevel_Event, "Add ADS data to json object\n");

  KVHDrvMode drv_mode = kKVHDrvMode_Unknown;
  if ((lf_ads->present.system) &&
      KVH_IsValidDrvMode(lf_ads->system.supported.driving_mode)) {
    drv_mode = lf_ads->system.supported.driving_mode;
  }
  if (cJSON_AddNumberToObject(obj, "supported_driving_mode", drv_mode) == NULL) {
    ERR("Fail to add ADS data to json object - cJSON_AddNumberToObject(supported_driving_mode)\n");
    return -1;
  }

  drv_mode = kKVHDrvMode_Unknown;
  if ((hf_ads->present.ads) &&
      KVH_IsValidDrvMode(hf_ads->ads.driving_mode.current_mode)) {
    drv_mode = hf_ads->ads.driving_mode.current_mode;
  }
  if (cJSON_AddNumberToObject(obj, "current_driving_mode", drv_mode) == NULL) {
    ERR("Fail to add ADS data to json object - cJSON_AddNumberToObject(current_driving_mode)\n");
    return -1;
  }

  KVHVehicleLength len = kKVHVehicleLength_Unknown;
  if ((lf_ads->present.vehicle) &&
      KVH_IsValidVehicleLength(lf_ads->vehicle.length)) {
    len = lf_ads->vehicle.length;
  }
  if (cJSON_AddNumberToObject(obj, "length", len) == NULL) {
    ERR("Fail to add ADS data to json object - cJSON_AddNumberToObject(length)\n");
    return -1;
  }

  KVHVehicleWidth width = kKVHVehicleWidth_Unknown;
  if ((lf_ads->present.vehicle) &&
      KVH_IsValidVehicleWidth(lf_ads->vehicle.width)) {
    width = lf_ads->vehicle.width;
  }
  if (cJSON_AddNumberToObject(obj, "width", width) == NULL) {
    ERR("Fail to add ADS data to json object - cJSON_AddNumberToObject(width)\n");
    return -1;
  }

  KVHVehicleHeight height = kKVHVehicleHeight_Unknown;
  if ((lf_ads->present.vehicle) &&
      KVH_IsValidVehicleHeight(lf_ads->vehicle.height)) {
    height = lf_ads->vehicle.height;
  }
  if (cJSON_AddNumberToObject(obj, "height", height) == NULL) {
    ERR("Fail to add ADS data to json object - cJSON_AddNumberToObject(height)\n");
    return -1;
  }

  KVHWeight weight = kKVHWeight_Unknown;
  if ((lf_ads->present.vehicle) &&
      KVH_IsValidWeight(lf_ads->vehicle.weight)) {
    weight = lf_ads->vehicle.weight;
  }
  if (cJSON_AddNumberToObject(obj, "weight", weight) == NULL) {
    ERR("Fail to add ADS data to json object - cJSON_AddNumberToObject(weight)\n");
    return -1;
  }

  LOG(kKVHLOGLevel_Event, "Success to add ADS data to json object\n");
  return 0;
}


/**
 * @brief "DMS" 관련 데이터를 json 오브젝트에 추가한다.
 * @param[in] dms DMS 데이터
 * @param[out] obj json 오브젝트
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int UDMEM_KADAP_AddJSON_DMS(KVHDMSData *dms, cJSON *obj)
{
  LOG(kKVHLOGLevel_Event, "Add DMS data to json object\n");

  if (cJSON_AddNumberToObject(obj, "driver_status", dms->status) == NULL) {
    ERR("Fail to add DMS data to json object - cJSON_AddNumberToObject(driver_status)\n");
    return -1;
  }

  LOG(kKVHLOGLevel_Event, "Success to add DMS data to json object\n");
  return 0;
}


/**
 * @brief 업로드 컨텐츠 내 json 문자열을 만든다.
 * @param[in] mib 모듈 관리정보
 * @param[in] lf_ads LF ADS 상태데이터
 * @param[in] hf_ads HF ADS 상태데이터
 * @param[in] lf_hub LF Hub 상태데이터
 * @param[in] hf_hub HF Hub 상태데이터
 * @param[in] dms DMS 상태데이터
 * @return 업로드 컨텐츠에 포함되는 json 문자열
 *
 * NOTE:: 클라우드로 업로드하는 데이터 중 GNSS 정보는 ADS의 위치정보가 아닌 V2X Hub의 위치정보를 전송한다. (비자율차도 있을 수 있으므로)
 */
static char * UDMEM_KADAP_ConstructJSONStr(
  KVHLFADSData *lf_ads,
  KVHHFADSData *hf_ads,
  KVHLFHubData *lf_hub,
  KVHHFHubData *hf_hub,
  KVHDMSData *dms)
{
  LOG(kKVHLOGLevel_Event, "Construct upload contents json string\n");
  char *json_str = NULL;

  /*
   * json 오브젝트를 생성한다.
   */
  cJSON *obj = cJSON_CreateObject();
  if (!obj) {
    return NULL;
  }

  /*
   * carid 데이터를 json 오브젝트에 추가한다.
   */
  if (UDMEM_KADAP_AddJSON_Carid(lf_hub->system.dev_id, obj) < 0) {
    goto out;
  }

  /*
   * gnss 데이터를 json 오브젝트에 추가한다.
   */
  if (UDMEM_KADAP_AddJSON_GNSS(&(hf_hub->gnss), obj) < 0) {
    goto out;
  }

  /*
   * ADS 데이터를 json 오브젝트에 추가한다.
   */
  if (UDMEM_KADAP_AddJSON_ADS(lf_ads, hf_ads, obj) < 0) {
    goto out;
  }

  /*
   * DMS 데이터를 json 오브젝트에 추가한다.
   */
  if (UDMEM_KADAP_AddJSON_DMS(dms, obj) < 0) {
    goto out;
  }

  /*
   * json 문자열을 생성한다.
   */
  json_str = cJSON_Print(obj);
  LOG(kKVHLOGLevel_Event, "Success to construct upload contents json string\n");

out:
  cJSON_Delete(obj);
  return json_str;
}


/**
 * @brief 상태데이터를 포함하는 업로드 컨텐츠를 만든다.
 * @param[in] lf_ads LF ADS 상태데이터
 * @param[in] hf_ads HF ADS 상태데이터
 * @param[in] lf_hub LF Hub 상태데이터
 * @param[in] hf_hub HF Hub 상태데이터
 * @param[in] dms DMS 상태데이터
 * @return 업로드 컨텐츠 (HTTP request body에 수납될 데이터)
 */
static char * UDMEM_KADAP_ConstructUploadContents(
  KVHLFADSData *lf_ads,
  KVHHFADSData *hf_ads,
  KVHLFHubData *lf_hub,
  KVHHFHubData *hf_hub,
  KVHDMSData *dms)
{
  LOG(kKVHLOGLevel_Event, "Construct upload contents\n");

  char *json_str = UDMEM_KADAP_ConstructJSONStr(lf_ads, hf_ads, lf_hub, hf_hub, dms);
  if (json_str) {
    size_t prefix_len = strlen(g_udmem_kadap_cloud_upload_contents_prefix);
    size_t suffix_len = strlen(g_udmem_kadap_cloud_upload_contents_suffix);
    size_t json_str_len = strlen(json_str);
    size_t contents_len = prefix_len + suffix_len + json_str_len;
    char *contents = malloc(contents_len + 1);
    if (contents) {
      *(contents + contents_len) = '\0';
      memcpy(contents, g_udmem_kadap_cloud_upload_contents_prefix, prefix_len);
      memcpy(contents + prefix_len, json_str, json_str_len);
      memcpy(contents + prefix_len + json_str_len, g_udmem_kadap_cloud_upload_contents_suffix, suffix_len);
      LOG(kKVHLOGLevel_Event, "Success to construct upload contents\n", contents);
      free(json_str);
      return contents;
    }
    free(json_str);
  }
  LOG(kKVHLOGLevel_Event, "Fail to construct upload contents\n");
  return NULL;
}


/**
 * @brief RSDPR에서 상태데이터를 읽어온다.
 * @param[in] mib 모듈 관리정보
 * @param[in] contents HTTP request body에 수납될 컨텐츠
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int UDMEM_KADAP_UploadContents(UDMEM_KADAP_MIB *mib, const char *contents)
{
  LOG(kKVHLOGLevel_Event, "Upload contents\n");

  /*
   * 클라우드 URL을 생성한다.
   */
  char url[KVH_MAXLINE];
  memset(url, 0, sizeof(url));
  snprintf(url, sizeof(url), "http://%s:%u/", mib->cloud_if.status_data_upload.server_ip, mib->cloud_if.status_data_upload.server_port);

  /*
   * HTTP post method로 클라우드 서버에 업로드한다..
   */
  if (UDMEM_KADAP_HTTP_POST(url, contents) < 0) {
    return -1;
  }

  LOG(kKVHLOGLevel_Event, "Success to upload contents\n");
  return 0;
}


/**
 * @brief 데이터 갱신 주기에 맞춰 클라우드로 상태데이터를 업로드하는 쓰레드
 * @param[in] arg 모듈 관리정보
 * @return NULL(프로그램 종료되기 전까지는 리턴되지 않음)
 */
static void * UDMEM_KADAP_StatusDataCloudUploadThread(void *arg)
{
  LOG(kKVHLOGLevel_Init, "Start status data cloud upload thread\n");

  UDMEM_KADAP_MIB *mib = (UDMEM_KADAP_MIB *)arg;
  mib->cloud_if.status_data_upload.tx.th_running = true;

  /*
   * 타이머 주기마다 깨어나서, 데이터를 클라우드로 업로드한다.
   */
  uint64_t exp;
  KVHLFADSData lf_ads;
  KVHHFADSData hf_ads;
  KVHLFHubData lf_hub;
  KVHHFHubData hf_hub;
  KVHDMSData dms;
  char *contents;
  while (1)
  {
    // 타이머 만기까지 대기
    read(mib->cloud_if.status_data_upload.tx.timerfd, &exp, sizeof(uint64_t));

    // 쓰레드 종료
    if (mib->cloud_if.status_data_upload.tx.th_running == false) {
      break;
    }

    // 업로드할 상태 데이터를 읽어 온다.
    if (UDMEM_KADAP_ReadRSDPR(mib, &lf_ads, &hf_ads, &lf_hub, &hf_hub, &dms) == 0) {

      // 상태데이터의 유효성을 확인한다. (업로드하기에 충분한지)
      if (UDMEM_KADAP_CheckUploadStatusDataValidity(&lf_hub, &hf_hub)) {

        // 업로드할 상태데이터 컨텐츠를 구성한다.
        contents = UDMEM_KADAP_ConstructUploadContents(&lf_ads, &hf_ads, &lf_hub, &hf_hub, &dms);
        if (contents) {
          // 상태데이터 컨텐츠를 업로드한다.
          UDMEM_KADAP_UploadContents(mib, contents);
          free(contents);
        }
      }
    }
  }

  LOG(kKVHLOGLevel_Event, "Exit status data cloud upload thread\n");
  return NULL;
}


/**
 * @brief 클라우드로의 상태데이터 업로드 기능을 초기화한다.
 * @param[in] mib 모듈 관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 *
 * @note 각 데이터 주기에 맞춰 데이터를 전송하는 기능이다.
 */
int UDMEM_KADAP_InitStatusDataCloudUploadFunction(UDMEM_KADAP_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize status data cloud upload function\n");

  /*
   * 상태데이터 업로드 타이머를 초기화한다.
   */
  if (UDMEM_KADAP_InitStatusDataCloudUploadTimer(mib) < 0) {
    return -1;
  }

  /*
   * 상태에디어 업로드 쓰레드를 생성한다.
   */
  if (pthread_create(&(mib->cloud_if.status_data_upload.tx.th), NULL, UDMEM_KADAP_StatusDataCloudUploadThread, mib) != 0) {
    ERR("Fail to initialize status data cloud upload function - pthread_create()\n");
    return -1;
  }
  struct timespec req = { .tv_sec = 0, .tv_nsec = 10000000 }, rem; // 쓰레드가 생성될 때까지 대기한다.
  while(mib->cloud_if.status_data_upload.tx.th_running == false) {
    nanosleep(&req, &rem);
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize status data cloud upload function\n");
  return 0;
}
