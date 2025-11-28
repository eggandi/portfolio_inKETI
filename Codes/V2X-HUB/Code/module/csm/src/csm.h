/**
 * @file
 * @brief CSM(C-ITS Service Module) 구현
 * @date 2024-11-19
 * @author user
 */


#ifndef V2X_HUB_CSM_H
#define V2X_HUB_CSM_H
#ifdef __cplusplus
extern "C" {
#endif

// 시스템 헤더파일
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <limits.h>

// 공용 헤더파일
#include "kvh_common.h"
#include "sudo_queue.h"
#include "ext_data_fmt/kvh_ext_data_fmt.h"
#include "internal_data_fmt/kvh_internal_data_fmt_uds.h"

// 공용 라이브러리 헤더파일
#include "kvhcits.h"
#include "kvhlog.h"
#include "kvhrsdpr.h"
#include "kvhuds.h"
#include "kvhq.h"
#include "kvhredis.h"

/// C-ITS 서비스 데이터 전송 주기 (마이크로초 단위)
#define CSM_CITS_SVC_DATA_TX_INTERNVAL (1000000)
#define CSMREDIS_PROTOBUF_DATAPOINTER_HANDLER_MAX (3) ///< Protobuf 데이터 포인터 핸들러 최대 개수

typedef void * CSMRedis_ProtobufHandler; ///< CSM Protobuf 핸들러

enum eCSMRedisProtobufCITSMsgType{
	kCSMRedisProtobufCITSMsgType_WSA = 135,	 ///< WSA 데이터 모델 타입
	kCSMRedisProtobufCITSMsgType_BSM = 32,	 ///< BSM 데이터 모델 타입
	kCSMRedisProtobufCITSMsgType_BSM_KSR = 82050,	 ///< BSM 데이터 모델 타입
	kCSMRedisProtobufCITSMsgType_PVD = 82051,	 ///< PVD 데이터 모델 타입
	kCSMRedisProtobufCITSMsgType_RSA = 82053,	 ///< RSA 데이터 모델 타입
	kCSMRedisProtobufCITSMsgType_TIM = 82054,	 ///< TIM 데이터 모델 타입
	kCSMRedisProtobufCITSMsgType_SPAT = 82055,	 ///< SPAT 데이터 모델 타입
	kCSMRedisProtobufCITSMsgType_MAP = 82056,	 ///< MAP 데이터 모델 타입
	kCSMRedisProtobufCITSMsgType_RTCM = 82057,	 ///< RTCM 데이터 모델 타입
};
typedef long CSMRedisProtobufCITSMsgType; ///< @ref eCSMRedisProtobufCITSMsgType

enum eCSMRedisProtobufCITSMsgFrom{
	kCSMRedisProtobufCITSMsgFrom_V2X = 1,	 ///< WSA 데이터 모델 타입
	kCSMRedisProtobufCITSMsgFrom_CSM = 2,	 ///< BSM 데이터 모델 타입
};
typedef long CSMRedisProtobufCITSMsgFrom; ///< @ref eCSMRedisProtobufCITSMsgFrom

typedef struct {
	bool isfulfill[CSMREDIS_PROTOBUF_DATAPOINTER_HANDLER_MAX];
	CSMRedis_ProtobufHandler h[CSMREDIS_PROTOBUF_DATAPOINTER_HANDLER_MAX]; ///< Protobuf 핸들러 (데이터 포인터)
} CSMRedisProtobufHandlerInfo;


/**
 * @brief 모듈 통합관리정보(Management Information Base)
 */
typedef struct
{
  char *logfile; ///< 로그 저장 파일경로
  KVHLOGLevel log_lv; ///< 로그 저장 레벨 (기본값 적용 또는 프로그램 실행 인자로 입력된다)

  /// SMEM과의 I/F 관리정보
  /// SMEM -> CSM 단방향 데이터 전달만 수행된다.
  struct {
    KVHUDSINFO uds; ///< UDS 통신기능 관리정보 (SMEM과의 UDS 통신 시 사용 - 클라이언트로 동작)
  } smem_if;

  /// DGM과의 I/F 관리정보
  /// DGM으로 UDS 데이터를 전송하는 주체는 단일 쓰레드(C-ITS 요약정보 전송 타이머 쓰레드)이므로 송신큐가 필요하지 않다.
  struct {
    KVHUDSINFO uds; ///< UDS 통신기능 관리정보 (DGM과의 UDS 통신 시 사용 - 클라이언트로 동작)
    /// 송신 관리정보
    struct {
      int timerfd; ///< 데이터전송 타이머 파일 디스크립터
      pthread_t th; ///< 데이터전송 쓰레드 식별자
      volatile bool th_running; ///< 쓰레드가 동작 중인지 여부
    } tx;
  } dgm_if;

  struct {
    KVHCITSHANDLE h; ///< C-ITS 라이브러리 핸들
  } cits;
	
	struct {
    KVHREDISHANDLE h_psub; ///< Redis 기능 핸들 (PSUBSCRIBE 용)
		KVHREDISHANDLE h_pub; ///< Redis 기능 핸들 (PUBLISH 용)
		KVHREDISHANDLE h_cmd; ///< Redis 기능 핸들 (GET 용)
		CSMRedisProtobufHandlerInfo protobuf_info; ///< Protobuf 핸들러 정보
		pthread_t th_pub; ///< Redis PUBLISH 쓰레드 식별자
		volatile bool th_pub_running; ///< Redis PUBLISH 쓰레드가 동작 중인지
  } redis_if;

  KVHRSDPRHANDLE rsdpr_h; ///< RSDPR 핸들

} CSM_MIB;

extern CSM_MIB g_csm_mib;


// csm_dgm_if.c
int CSM_InitDGMIF(CSM_MIB *mib);
int CSM_PushTxQueueToDGM(CSM_MIB *mib, uint8_t *data, size_t len);

// csm_input_params.c
int CSM_ParsingInputParameters(int argc, char *argv[], CSM_MIB *mib);

// csm_smem_if.c
int CSM_InitSMEMIF(CSM_MIB *mib);

// csm_redis_if.c
int CSM_InitRedisIF(CSM_MIB *mib);

// csm_redis_pb.cc
int CSMRedis_InitProtobufLibrary();
int CSMRedis_ShutdownProtobufLibrary();
CSMRedis_ProtobufHandler CSMRedis_CreateProtobufHandler();
void CSMRedis_DestroyProtobuf(CSMRedis_ProtobufHandler h);
size_t CSMRedis_SetProtobufCITSMsgToHandler(CSMRedis_ProtobufHandler h, CSMRedisProtobufCITSMsgFrom msg_from, CSMRedisProtobufCITSMsgType msg_type, char *cits_msg, size_t msg_size);
size_t CSMRedis_ProcessProtobufSerializeToString(CSMRedis_ProtobufHandler h, char **out_buffer);
void CSMRedis_ClearProtobufHandler(CSMRedis_ProtobufHandler h);


#ifdef __cplusplus
}
#endif

#endif //V2X_HUB_CSM_H
