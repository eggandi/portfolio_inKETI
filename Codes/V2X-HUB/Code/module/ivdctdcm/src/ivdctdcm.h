/**
 * @file
 * @brief IVDCT() 구현
 * @date 2025-06-11
 * @author donggyu
 */


#ifndef V2X_HUB_IVDCTDCM_H
#define V2X_HUB_IVDCTDCM_H

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

// 공용 헤더파일
#include "kvh_common.h"
#include "sudo_queue.h"
#include "ext_data_fmt/kvh_ext_data_fmt.h"
#include "internal_data_fmt/kvh_internal_data_fmt_uds.h"
#include "internal_data_fmt/kvh_internal_data_fmt_ivdct.h" // c Type 구조체

// 공용 라이브러리 헤더파일
#include "kvhlog.h"
#include "kvhuds.h"
#include "kvhredis.h"


/// IVDCT I/F 기본 설정
#define IVDCTDCM_DEFAULT_IF_DEV_NAME "/dev/ttyFTDI2"
#define IVDCTDCM_DEFAULT_IF_BAUDRATE (921600)
#define IVDCTDCM_BASIC_SERIAL_TX_MSG_LEN_MAX 50
#define IVDCTDCM_BASIC_TX_MSG_LEN (IVDCTDCM_BASIC_SERIAL_TX_MSG_LEN_MAX)

#define IVDCTDCMPOROBUFPKT_STX (0xABCD)
#define IVDCTDCMPOROBUFPKT_ETX (0xDCBA)

#define IVDCTDCM_REDIS_PUBLISH_KEY "IVDCT" ///< IVDCT Redis Publish Key

typedef struct 
{
	uint8_t stx[2];
	uint8_t payload_len[2];
	uint8_t payload[];
} __attribute__((packed)) IVDCTDCM_IvdctBasicData;


/**
 * @brief 모듈 통합관리정보(Management Information Base)
 */
typedef struct
{
  char *logfile; ///< 로그 저장 파일경로
  KVHLOGLevel log_lv; ///< 로그 저장 레벨 (기본값 적용 또는 프로그램 실행 인자로 입력된다)

  /// DGM과의 I/F 관리정보
  /// UDS로 데이터를 전송하는 주체는 단일 쓰레드이므로 송신큐가 필요하지 않다.
  struct {
		bool uds_init; ///< UDS 초기화 여부
    KVHUDSINFO uds; ///< UDS 통신기능 관리정보 (DGM과의 UDS 통신 시 사용 - 클라이언트로 동작)
  } dgm_if;

  /// IVDCT와의 I/F 관리정보
  struct
  {
    char dev_name[KVH_MAXLINE+1]; ///< 시리얼포트 디바이스명 (예: /dev/ttyUSB1)
    long baudrate; ///< 시리얼포트 baudrate
    int sock; ///< 시리얼포트 소켓
    pthread_t rx_th; ///< 시리얼포트 데이터 수신 쓰레드
    volatile bool rx_th_running; ///< 수신 쓰레드 동작 여부
  } ivdct_if;

	struct {
		char server_addr_str[INET_ADDRSTRLEN]; ///< Redis 서버 주소 문자열
		uint16_t server_port; ///< Redis 서버 포트번호
		KVHREDISHANDLE h_cmd; ///< Redis 기능 핸들 (PUBLISH 용)
		KVHREDISKey key_ivdct_protobuf;
		KVHREDISKey key_ivdct_raw;
		KVHREDISKey key_ivdct_publish;
  } redis_if;

  struct {
    unsigned int ivdctdcm_rx; ///< IVDCT로부터의 운전자상태데이터 수신 누적횟수
    unsigned int dgm_tx; ///< DGM으로의 운전자상태데이터 송신 누적횟수
  } io_cnt; ///< 입출력 누적횟수
} IVDCTDCM_MIB;

extern IVDCTDCM_MIB g_ivdctdcm_mib;

/* IVDCTDCM Protobuf 핸들러 타입 정의 */
typedef void* IVDCTDCM_PBHandler;

// ivdctdcm_input_params.c
int IVDCTDCM_ParsingInputParameters(int argc, char *argv[], IVDCTDCM_MIB *mib);

// ivdctdcm_dgm_if.c
int IVDCTDCM_InitDGMIF(IVDCTDCM_MIB *mib);
int IVDCTDCM_TransmitIvdctDataToDGM(IVDCTDCM_MIB *mib, KVHIVDCTData *ivdct_data);

// ivdctdcm_ivdct_if.c
int IVDCTDCM_InitIVDCTIF(IVDCTDCM_MIB *mib);

// ivdctdcm_pb.cc
IVDCTDCM_PBHandler IVDCTDCM_CreatePB();
void IVDCTDCM_TestSetPBData(IVDCTDCM_PBHandler hypot);
int IVDCTDCM_ProcessPBParseFromArray(IVDCTDCM_PBHandler h, const uint8_t *serialized_data, size_t data_size);
size_t IVDCTDCM_ProcessPBSerializeToString(IVDCTDCM_PBHandler h, char **out_buffer);
int IVDCTDCM_GetPBHandleData(IVDCTDCM_PBHandler h, KVHIVDCTData** out_data);

// ivdctdcm_redis_if.c
int IVDCTDCM_ProcessRedisSETtoKey(IVDCTDCM_MIB *mib, KVHREDISKey *key, uint8_t *data, size_t data_size);
int IVDCTDCM_ProcessRedisPUBLISHToKey(IVDCTDCM_MIB *mib, KVHREDISKey *key,  uint8_t *data, size_t data_size);
int IVDCTDCM_InitRedisIF(IVDCTDCM_MIB *mib);


#ifdef __cplusplus
}
#endif

#endif // V2X_HUB_IVDCTDCM_H