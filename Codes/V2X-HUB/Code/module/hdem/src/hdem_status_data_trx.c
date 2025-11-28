/**
* @file
* @brief HMI로의 상태데이터 패킷 전송 기능 구현
* @date 2024-11-16
* @author user
*/


// 시스템 헤더파일
#include <sys/timerfd.h>

// 모듈 헤더파일
#include "hdem.h"


/**
 * @brief HMI로의 상태데이터 패킷 전송 타이머를 초기화한다.
 * @param[in] mib 모듈 관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int HDEM_InitStatusDataPktTxToHMITimer(HDEM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize status data pkt tx to HMI timer\n");

  /*
   * 타이머를 생성한다.
   */
  mib->hmi_if.status_data_trx.tx.timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
  if (mib->hmi_if.status_data_trx.tx.timerfd < 0) {
    ERR("Fail to initialize status data pkt tx to HMI timer - timerfd_create() failed - %m\n");
    return -1;
  }

  /*
   * 타이머 주기를 설정한다.
   */
  struct itimerspec ts;
  long usec = HDEM_STATUS_DATA_PKT_TX_INTERVAL_MIN;
  ts.it_value.tv_sec = (time_t)(usec / 1000000);
  ts.it_value.tv_nsec = (long)((usec % 1000000) * 1000);
  ts.it_interval.tv_sec = (time_t)(usec / 1000000);
  ts.it_interval.tv_nsec = (long)((usec % 1000000) * 1000);
  int ret = timerfd_settime(mib->hmi_if.status_data_trx.tx.timerfd, TFD_TIMER_ABSTIME, &ts, NULL);
  if (ret < 0) {
    ERR("Fail to initialize status data pkt tx to HMI timer - timerfd_settime() failed - %m\n");
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize status data pkt tx to HMI timer\n");
  return 0;
}


/**
 * @brief HMI로 전송할 상태데이터 JSON 패킷을 만든다
 * @param[in] mib 모듈 통합관리정보
 * @param[in] pkt_type 패킷 데이터 유형
 * @param[in] data 패킷에 수납할 페이로드
 * @param[in] len 페이로드의 길이
 * @param[out] pktlen 생성된 패킷의 길이가 저장될 변수 포인터
 * @retval 생성된 JSON 패킷 바이트열 (실패 시 NULL)
 */
static uint8_t *HDEM_ConstructStatusDataJSONPktToHMI(
  HDEM_MIB *mib,
  KVHJSONPktDataType pkt_type,
  uint8_t *data,
  KVHIFDataLen len,
  KVHIFDataLen *pkt_len)
{
  *pkt_len = (KVHIFDataLen)(sizeof(KVHJSONPktHeader) + len + sizeof(KVHJSONPktTail));
  uint8_t *pkt = (uint8_t *)malloc(*pkt_len);
  if (pkt) {
    KVHJSONPktHeader *h = (KVHJSONPktHeader *)pkt;
    h->stx = htonl(KVHJSONPKT_STX);
    h->seq = htonl(mib->hmi_if.status_data_trx.tx.pkt_seq++);
    h->data_type = htonl(pkt_type);
    h->payload_len = htonl(len);
    uint8_t *payload = pkt + sizeof(KVHJSONPktHeader);
    memcpy(payload, data, len);
    KVHJSONPktTail *t = (KVHJSONPktTail *)(pkt + sizeof(KVHJSONPktHeader) + len);
    t->etx = htonl(KVHJSONPKT_ETX);
  }
  return pkt;
}


/**
 * @brief 상태데이터가 포함된 JSON 패킷을 HMI로 전송한다.
 * @param[in] mib 모듈 관리정보
 * @param[in] pkt 전송할 패킷
 * @param[in] len 전송할 패킷의 길이
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int HDEM_TransmitStatusDataJSONPktToHMI(HDEM_MIB *mib, uint8_t *pkt, KVHIFDataLen len)
{
  KVHJSONPktHeader *pkt_hdr = (KVHJSONPktHeader *)pkt;
  LOG(kKVHLOGLevel_Event, "Transmit %s(%d) status data JSON pkt to HMI(len: %d)\n",
      KVH_GetJSONPktDataTypeStr(ntohl(pkt_hdr->data_type)), ntohl(pkt_hdr->data_type), len);
  DUMP(kKVHLOGLevel_Dump, "Status data JSON pkt to HMI", pkt, len);
  STR(kKVHLOGLevel_Dump, "Status data JSON to HMI", (char *)(pkt + sizeof(KVHJSONPktHeader)));

  if (len > KVH_IF_DATA_MAX_LEN) {
    ERR("Fail to transmit status data JSON pkt to HMI - too long: %u > %u\n", len, KVH_IF_DATA_MAX_LEN);
    return -1;
  }

  if (KVHUDP_Send(mib->hmi_if.status_data_trx.udp.h, pkt, len) < 0) {
    ERR("Fail to transmit status data JSON pkt to HMI(len: %d) - KVHUDP_Send()\n", len);
    return -1;
  }

  LOG(kKVHLOGLevel_Event, "Success to transmit status data JSON pkt to HMI\n");
  return 0;
}


/**
 * @brief LF ADS 상태정보 패킷을 HMI로 전송한다.
 * @param[in] mib 모듈 관리정보
 */
static void HDEM_TransmitStatusDataPktToHMI_LFADSData(HDEM_MIB *mib)
{
  mib->trx_cnt.hmi.lf_ads_status_tx++;
  LOG(kKVHLOGLevel_InOut, "Transmit LF ADS status data pkt to HMI - cnt: %u\n", mib->trx_cnt.hmi.lf_ads_status_tx);

  /*
   * RDPR에서 LF ADS 데이터를 읽어온다.
   */
  KVHLFADSData data;
  int ret = KVHRSDPR_ReadLFADSData(mib->hmi_if.status_data_trx.rsdpr, &data);
  if (ret < 0) {
    ERR("Fail to transmit LF ADS status data pkt to HMI - KVHRSDPR_ReadLFADSData()\n");
    return;
  }

  /*
   * LF ADS 데이터를 json 형태로 인코딩한다.
   */
  char *json_str;
  ret = KVHJSON_EncodeLFADSData(&data, &json_str);
  if ((ret < 0) || !json_str) {
    ERR("Fail to transmit LF ADS status data pkt to HMI - KVHJSON_EncodeLFADSData()\n");
    return;
  }

  /*
   * json 문자열을 패킷에 수납한다.
   */
  KVHIFDataLen pkt_len;
  uint8_t *pkt = HDEM_ConstructStatusDataJSONPktToHMI(mib, kKVHJSONPktDataType_ADS, (uint8_t *)json_str, strlen(json_str), &pkt_len);
  if (pkt) {
    HDEM_TransmitStatusDataJSONPktToHMI(mib, pkt, pkt_len);
    free(pkt);
  }
  free(json_str);
}


/**
 * @brief HF ADS 상태정보 패킷을 HMI로 전송한다.
 * @param[in] mib 모듈 관리정보
 */
static void HDEM_TransmitStatusDataPktToHMI_HFADSData(HDEM_MIB *mib)
{
  mib->trx_cnt.hmi.hf_ads_status_tx++;
  KVHLOGLevel lv = ((mib->trx_cnt.hmi.hf_ads_status_tx % 10) == 1) ? kKVHLOGLevel_InOut : kKVHLOGLevel_Event;
  LOG(lv, "Transmit HF ADS status data pkt to HMI - cnt: %u\n", mib->trx_cnt.hmi.hf_ads_status_tx);

  KVHHFADSData data;

  /*
   * RDPR에서 HF ADS 데이터를 읽어온다.
   */
  int ret = KVHRSDPR_ReadHFADSData(mib->hmi_if.status_data_trx.rsdpr, &data);
  if (ret < 0) {
    ERR("Fail to transmit HF ADS status data pkt to HMI - KVHRSDPR_ReadHFADSData()\n");
    return;
  }

  /*
   * HF ADS 데이터를 json 형태로 인코딩한다.
   */
  char *json_str;
  ret = KVHJSON_EncodeHFADSData(&data, &json_str);
  if ((ret < 0) || !json_str) {
    ERR("Fail to transmit HF ADS status data pkt to HMI - KVHJSON_EncodeHFADSData()\n");
    return;
  }

  /*
   * json 문자열을 패킷에 수납한다.
   */
  KVHIFDataLen pkt_len;
  uint8_t *pkt = HDEM_ConstructStatusDataJSONPktToHMI(mib, kKVHJSONPktDataType_ADS, (uint8_t *)json_str, strlen(json_str), &pkt_len);
  if (pkt) {
    HDEM_TransmitStatusDataJSONPktToHMI(mib, pkt, pkt_len);
    free(pkt);
  }
  free(json_str);
}


/**
 * @brief HF Hub 상태정보 패킷을 HMI로 전송한다.
 * @param[in] mib 모듈 관리정보
 */
static void HDEM_TransmitStatusDataPktToHMI_HFHubData(HDEM_MIB *mib)
{
  mib->trx_cnt.hmi.hf_ads_status_tx++;
  KVHLOGLevel lv = ((mib->trx_cnt.hmi.hf_ads_status_tx % 10) == 1) ? kKVHLOGLevel_InOut : kKVHLOGLevel_Event;
  LOG(lv, "Transmit HF Hub status data pkt to HMI - cnt: %u\n", mib->trx_cnt.hmi.hf_ads_status_tx);

  KVHHFHubData data;

  /*
   * RDPR에서 HF Hub 데이터를 읽어온다.
   */
  int ret = KVHRSDPR_ReadHFHubData(mib->hmi_if.status_data_trx.rsdpr, &data);
  if (ret < 0) {
    ERR("Fail to transmit HF Hub status data pkt to HMI - KVHRSDPR_ReadHFHubData()\n");
    return;
  }

  /*
   * HF Hub 데이터를 json 형태로 인코딩한다.
   */
  char *json_str;
  ret = KVHJSON_EncodeHFHubData(&data, &json_str);
  if ((ret < 0) || !json_str) {
    ERR("Fail to transmit HF Hub status data pkt to HMI - KVHJSON_EncodeHFHubData()\n");
    return;
  }

  /*
   * json 문자열을 패킷에 수납한다.
   */
  KVHIFDataLen pkt_len;
  uint8_t *pkt = HDEM_ConstructStatusDataJSONPktToHMI(mib, kKVHJSONPktDataType_HUB, (uint8_t *)json_str, strlen(json_str), &pkt_len);
  if (pkt) {
    HDEM_TransmitStatusDataJSONPktToHMI(mib, pkt, pkt_len);
    free(pkt);
  }
  free(json_str);
}

#if 0
/**
 * @brief LF Hub 상태정보 패킷을 HMI로 전송한다.
 * @param[in] mib 모듈 관리정보
 */
static void HDEM_TransmitStatusDataPktToHMI_LFHubData(HDEM_MIB *mib)
{
  LOG(kKVHLOGLevel_Event, "Transmit LF Hub status data pkt to HMI\n");

  KVHLFHubData data;

  /*
   * RDPR에서 LF Hub 데이터를 읽어온다.
   */
  int ret = KVHRSDPR_ReadLFHubData(mib->hmi_if.status_data_trx.rsdpr, &data);
  if (ret < 0) {
    ERR("Fail to transmit HF Hub status data pkt to HMI - KVHRSDPR_ReadHFHubData()\n");
    return;
  }

  /*
   * LF Hub 데이터를 json 형태로 인코딩한다.
   */
  char *json_str;
  ret = KVHJSON_EncodeLFHubData(&data, &json_str);
  if ((ret < 0) || !json_str) {
    ERR("Fail to transmit LF Hub status data pkt to HMI - KVHJSON_EncodeLFHubData()\n");
    return;
  }

  /*
   * json 문자열을 패킷에 수납한다.
   */
  KVHIFDataLen pkt_len;
  uint8_t *pkt = HDEM_ConstructStatusDataJSONPktToHMI(mib, kKVHJSONPktDataType_HUB, (uint8_t *)json_str, strlen(json_str), &pkt_len);
  if (pkt) {
    HDEM_TransmitStatusDataJSONPktToHMI(mib, pkt, pkt_len);
    free(pkt);
  }
  free(json_str);
}
#endif


/**
 * @brief DMS 상태정보 패킷을 HMI로 전송한다.
 * @param[in] mib 모듈 관리정보
 */
static void HDEM_TransmitStatusDataPktToHMI_DMS(HDEM_MIB *mib)
{
  mib->trx_cnt.hmi.dms_status_tx++;
  LOG(kKVHLOGLevel_InOut, "Transmit DMS status data pkt to HMI - cnt: %u\n", mib->trx_cnt.hmi.dms_status_tx);

  KVHDMSData data;

  /*
   * RDPR에서 DMS 데이터를 읽어온다.
   */
  int ret = KVHRSDPR_ReadDMSData(mib->hmi_if.status_data_trx.rsdpr, &data);
  if (ret < 0) {
    ERR("Fail to transmit DMS status data pkt to HMI - KVHRSDPR_ReadDMSData()\n");
    return;
  }

  /*
   * DMS 데이터를 json 형태로 인코딩한다.
   */
  char *json_str;
  ret = KVHJSON_EncodeDMSData(&data, &json_str);
  if ((ret < 0) || !json_str) {
    ERR("Fail to transmit DMS status data pkt to HMI - KVHJSON_EncodeDMSData()\n");
    return;
  }

  /*
   * json 문자열을 패킷에 수납한다.
   */
  KVHIFDataLen pkt_len;
  uint8_t *pkt = HDEM_ConstructStatusDataJSONPktToHMI(mib, kKVHJSONPktDataType_DMS, (uint8_t *)json_str, strlen(json_str), &pkt_len);
  if (pkt) {
    if (HDEM_TransmitStatusDataJSONPktToHMI(mib, pkt, pkt_len) == 0) {
      LOG(kKVHLOGLevel_Event, "Success to transmit DMS status data pkt to HMI\n");
    }
    free(pkt);
  }
  free(json_str);
}


/**
 * @brief 데이터 갱신 주기에 맞춰 HMI로 상태데이터 패킷을 전송하는 쓰레드
 * @param[in] arg 모듈 관리정보
 * @return NULL(프로그램 종료되기 전까지는 리턴되지 않음)
 */
static void * HDEM_StatusDataPktTxToHMIThread(void *arg)
{
  LOG(kKVHLOGLevel_Init, "Start status data pkt tx to HMI thread\n");

  HDEM_MIB *mib = (HDEM_MIB *)arg;
  mib->hmi_if.status_data_trx.tx.th_running = true;

  long max_interval = HDEM_STATUS_DATA_PKT_TX_INTERVAL_MAX;
  long min_interval = HDEM_STATUS_DATA_PKT_TX_INTERVAL_MIN;
  long max_cnt = max_interval / HDEM_STATUS_DATA_PKT_TX_INTERVAL_MIN;

  /*
   * 타이머 주기마다 깨어나서, HMI로 데이터를 송신한다.
   */
  uint64_t exp;
  long cnt = 0;
  while (1)
  {
    // 타이머 만기까지 대기
    read(mib->hmi_if.status_data_trx.tx.timerfd, &exp, sizeof(uint64_t));

    // 쓰레드 종료
    if (mib->hmi_if.status_data_trx.tx.th_running == false) {
      break;
    }
    cnt++;

    if ((cnt % (KVH_HF_ADS_DATA_UPDATE_INTERVAL/min_interval)) == 0) {
      HDEM_TransmitStatusDataPktToHMI_HFADSData(mib);
    }
    if ((cnt % (KVH_LF_ADS_DATA_UPDATE_INTERVAL/min_interval)) == 0) {
      HDEM_TransmitStatusDataPktToHMI_LFADSData(mib);
    }
    if ((cnt % (KVH_HF_HUB_DATA_UPDATE_INTERVAL/min_interval)) == 0) {
      HDEM_TransmitStatusDataPktToHMI_HFHubData(mib);
    }
#if 0
    if ((cnt % (KVH_LF_HUB_DATA_UPDATE_INTERVAL/min_interval)) == 0) {
      HDEM_TransmitStatusDataPktToHMI_LFHubData(mib);
    }
#endif
    if ((cnt % (KVH_DMS_DATA_UPDATE_INTERVAL/min_interval)) == 0) {
      HDEM_TransmitStatusDataPktToHMI_DMS(mib);
    }
    if (cnt == max_cnt) {
      cnt = 0;
    }
  }

  LOG(kKVHLOGLevel_Event, "Exit status data pkt tx to HMI thread\n");
  return NULL;
}


/**
 * @brief HMI로의 상태데이터 패킷 송신 기능을 초기화한다.
 * @param[in] mib 모듈 관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 *
 * @note 각 데이터 주기에 맞춰 데이터를 전송하는 기능이다.
 */
int HDEM_InitStatusDataPktTxToHMIFunction(HDEM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize status data pkt tx to HMI function\n");

  /*
   * 패킷전송 타이머를 초기화한다.
   */
  if (HDEM_InitStatusDataPktTxToHMITimer(mib) < 0) {
    return -1;
  }

  /*
   * 패킷전송 쓰레드를 생성한다.
   */
  if (pthread_create(&(mib->hmi_if.status_data_trx.tx.th), NULL, HDEM_StatusDataPktTxToHMIThread, mib) != 0) {
    ERR("Fail to initialize status data pkt tx to HMI function - pthread_create()\n");
    return -1;
  }
  struct timespec req = { .tv_sec = 0, .tv_nsec = 10000000 }, rem; // 쓰레드가 생성될 때까지 대기한다.
  while (mib->hmi_if.status_data_trx.tx.th_running == false) {
    nanosleep(&req, &rem);
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize status data pkt tx to HMI function\n");
  return 0;
}
