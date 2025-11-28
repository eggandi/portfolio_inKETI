/**
 * @file
 * @brief VHMM의 GNSS 정보 관련 기능 구현
 * @date 2024-11-14
 * @author user
 */


// 의존 헤더파일
#include "gps.h"

// 모듈 헤더파일
#include "vhmm.h"


/**
 * @brief 최신 GNSS 정보를 업데이트한다. (gpsd로부터 가져와서 MIB에 저장한다)
 * @param[in] mib 모듈 관리정보
 */
int VHMM_GetGNSSData(VHMM_MIB *mib, VHMMHighFREQData *gnss_data)
{
  struct gps_data_t *from = &(mib->gnss.gps_data);
	int ret = gps_read(from, NULL, 0);

	if (ret < 0) {
		ERR("Fail to gps_read() - %s, ret : %d\n", strerror(errno), ret);
		return - 2;
	} else if (ret == 0) {
		ERR("Fail to gps_read() - no data avvailable\n");
		return - 1;
	}

  gnss_data->timestamp = KVH_GetCurrentTimestamp();
  gnss_data->lat = KVH_ConvertGNSSLatitude(from->fix.latitude);
  gnss_data->lon = KVH_ConvertGNSSLongitude(from->fix.longitude);
  gnss_data->elev = KVH_ConvertGNSSElevation(from->fix.altHAE);
  gnss_data->smajor_axis_acc = KVH_ConvertGNSSSemiMajorAxisAccuracy(from->gst.smajor_deviation);
  gnss_data->sminor_axis_acc = KVH_ConvertGNSSSemiMinorAxisAccuracy(from->gst.sminor_deviation);
  gnss_data->smajor_axis_ori = KVH_ConvertGNSSSemiMajorAxisOrientation(from->gst.smajor_orientation);
  gnss_data->speed = KVH_ConvertGNSSSpeed(from->fix.speed);
  gnss_data->heading = KVH_ConvertGNSSHeading(from->fix.track);
  gnss_data->accel_lon = KVH_ConvertGNSSAcceleration(from->attitude.acc_x);
  gnss_data->accel_lat = KVH_ConvertGNSSAcceleration(from->attitude.acc_y);
  gnss_data->accel_vert = KVH_ConvertGNSSVerticalAcceleration(from->attitude.acc_z);
#if defined(_GPSD_3_22_)
  gnss_data->yawrate = kKVHYawrate_Unknown;
#elif defined(_GPSD_3_23_1_)
  gnss_data->yawrate = KVH_ConvertGNSSYawRate(from->attitude.gyro_z);
#endif
  gnss_data->fix = KVH_ConvertGNSSFix(from->fix.mode);
  gnss_data->net_rtk_fix = KVH_ConvertGNSSNetRTKFix(from->fix.status);
	
	return 0;
}


/**
 * @brief GNSS 데이터 수집 기능을 초기화한다.
 * @param[in] mib 모듈 관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int VHMM_InitGNSSDataCollectFunction(VHMM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize GNSS data collect function\n");

  /*
   * gpsd 인터페이스를 초기화한다.
   */
  int ret = gps_open("localhost", "2947", &(mib->gnss.gps_data));
  if (ret) {
    ERR("Fail to initialize GNSS data collect function - gps_open() - ret: %d(%s), errno: %s\n",
        ret, gps_errstr(ret), strerror(errno));
    return -1;
  }
  (void)gps_stream(&(mib->gnss.gps_data), WATCH_ENABLE | WATCH_NEWSTYLE, NULL);


  LOG(kKVHLOGLevel_Init, "Success to initialize GNSS data collect function\n");
  return 0;
}

