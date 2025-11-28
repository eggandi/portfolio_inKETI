/**
 * @file
 * @brief VHMM() 구현
 * @date 2025-06-11
 * @author donggyu
 */


#ifndef V2X_HUB_VHMM_H
#define V2X_HUB_VHMM_H

#ifdef __cplusplus
extern "C" {
#endif
// 시스템 헤더파일
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/timerfd.h>

#include "gps.h"
// 공용 헤더파일
#include "kvh_common.h"
#include "internal_data_fmt/kvh_internal_data_fmt_vhmm.h" // c Type 구조체
#include "internal_data_fmt/kvh_internal_data_fmt_gnss.h"

// 공용 라이브러리 헤더파일
#include "kvhlog.h"
#include "kvhredis.h"


#define VHMM_GNSS_REDIS_PUBLISH_KEY "VH_HF"
#define VHMM_STATES_REDIS_PUBLISH_KEY "VH_LF"


typedef KVHGNSSData VHMMHighFREQData;	///< VHMM 상태 데이터 타입 정의(고빈도 데이터)
typedef KVHVHMMLowFREQData VHMMLowFREQData;	///< VHMM 상태 데이터 타입 정의(저빈도 데이터)

/* VHMM Protobuf 핸들러 타입 정의 */
typedef void* VHMM_PBHandler;

/**
 * @brief 모듈 통합관리정보(Management Information Base)
 */
typedef struct
{
  char *logfile; ///< 로그 저장 파일경로
  KVHLOGLevel log_lv; ///< 로그 저장 레벨 (기본값 적용 또는 프로그램 실행 인자로 입력된다)

  struct {
    struct gps_data_t gps_data; ///< gpsd I/F 데이터
		KVHREDISKey key_vhmm_gnss_publish;
		KVHREDISKey key_vhmm_gnss_protobuf;
		KVHREDISKey key_vhmm_gnss_raw;
  } gnss; ///< gnss 수집데이터 관리정보

  struct {
    VHMMLowFREQData data; ///< GNSS 데이터 (gpsd로부터 수신하여 저장된)
		KVHREDISKey key_vhmm_states_publish;
		KVHREDISKey key_vhmm_states_protobuf;
		KVHREDISKey key_vhmm_states_raw;
  } states; ///< gnss 수집데이터 관리정보

  struct {
    pthread_t th; ///< 쓰레드 식별자
    volatile bool th_running; ///< 쓰레드가 동작 중인지 여부
    VHMMHighFREQData data; ///< GNSS 데이터 (gpsd로부터 수신하여 저장된)
		KVHREDISKey key_vhmm_global_sequence ;
  } global_sequence; ///< gnss 수집데이터 관리정보

	
	struct {
		char server_addr_str[INET_ADDRSTRLEN]; ///< Redis 서버 주소 문자열
		uint16_t server_port; ///< Redis 서버 포트번호
		KVHREDISHANDLE h_cmd; ///< Redis 기능 핸들 (PUBLISH 용)
		KVHREDISHANDLE h_psub; ///< Redis 기능 핸들 (PSUBSCRIBE 용), 각종 상태정보 수집
		pthread_t th_pub; ///< Redis PUBLISH 쓰레드 식별자
	  volatile bool th_pub_running; ///< Redis PUBLISH 쓰레드가 동작 중인지
  } redis_if;

	struct {
		VHMM_PBHandler vhmm_gnss_pb_handler; ///< VHMM GNSS Protobuf 핸들러
		VHMM_PBHandler vhmm_states_pb_handler; ///< VHMM States Protobuf 핸들러
	} protobuf_if;

} VHMM_MIB;

extern VHMM_MIB g_vhmm_mib;



// vhmm_input_params.c
int VHMM_ParsingInputParameters(int argc, char *argv[], VHMM_MIB *mib);


// vhmm_pb.cc
VHMM_PBHandler VHMM_CreatePBHandlerGNSS();
int VHMM_SetPBHandlerDataFromGNSS(VHMM_PBHandler h, VHMMHighFREQData *data);
size_t VHMM_ProcessPBSerializeToStringGNSS(VHMM_PBHandler h, char **out_buffer);
VHMM_PBHandler VHMM_CreatePBHandlerStates();
int KVHPROTOBUF_Init();
int KVHPROTOBUF_Shutdown();
void VHMM_DestroyPBHandlerGNSS(VHMM_PBHandler h);

// vhmm_redis_if.c
int VHMM_ProcessRedisSETtoKey(VHMM_MIB *mib, KVHREDISKey *key, uint8_t *data, size_t data_size);
int VHMM_InitRedisIF(VHMM_MIB *mib);

// vhmm_gnss.c
int VHMM_GetGNSSData(VHMM_MIB *mib, VHMMHighFREQData *gnss_data);
int VHMM_InitGNSSDataCollectFunction(VHMM_MIB *mib);


// vhmm_states.c
#ifdef __cplusplus
}
#endif

#endif // V2X_HUB_VHMM_H