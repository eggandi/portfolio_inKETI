
#ifndef _V2XHO_UTIL_GNSS_H_
#define _V2XHO_UTIL_GNSS_H_

struct v2xho_gnss_state_t
{
  bool unavailable; ///< 미장착 또는 Unavailable
  bool is_healthy; ///< 2D_FIX 이상이면 set
  bool is_monitored; ///< GNSS 위성이 관측되면 set
  bool base_station_type; ///< 0으로 설정(=OBU) (API에 의해 설정될 수 있다)
  bool pdop_under_5; ///< PDOP 값이 5 미만인지 여부
  bool num_sv_under_5; ///< 관측되는 위성의 개수가 5개 미만인지 여부
  bool use_dgps; ///< DGPS 사용 여부 (API에 의해 설정될 수 있다)
  bool use_rtk; ///< RTK 사용 여부 (API에 의해 설정될 수 있다)
};

struct v2xho_gnss_data_t
{
	pthread_mutex_t mtx;
	uint32_t sec;
	uint32_t min;
	struct timespec ts; ///< 데이터 수집 시점
	struct v2xho_gnss_state_t status; ///< GNSS 상태 정보
	struct gps_fix_t fix;
	double lat_raw; ///< GNSS 입력 위도 (1도 단위)
	double lon_raw; ///< GNSS 입력 경도 (1도 단위)
	int lat; ///< 위도
	int lon; ///< 경도
	int elev; ///< 고도
	uint32_t speed;
	uint32_t heading;
	double pdop; ///< PDOP(Position Dilution of Precision)
	int num_sv; ///< 관측되는 위성 수
	struct timedelta_t pps;
	struct gps_policy_t policy;
};

#endif