/**
 * @file
 * @brief DMSDCM(DMS Data Collection Module) 기능 구현
 * @date 2024-12-13
 * @author user
 */

#ifndef KVH_DMSDCM_H
#define KVH_DMSDCM_H


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
#include "internal_data_fmt/kvh_internal_data_fmt_uds.h"
#include "internal_data_fmt/kvh_internal_data_fmt_dms.h" // c Type 구조체


// 공용 라이브러리 헤더파일
#include "kvhlog.h"
#include "kvhuds.h"


/// DMS I/F 기본 설정
#define DMSDCM_DEFAULT_IF_DEV_NAME "/dev/ttyFTDI1"
#define DMSDCM_DEFAULT_IF_BAUDRATE (115200)
#define DMSDCM_BASIC_TX_MSG_LEN (46) ///< MDSM-7 Basic transmission 메시지 길이 (per MDSM-7 RS-232 Protocol Document)

/// 모본 MDSM-7 데이터 프로토콜 필드값
#define DMSDCM_MDSM7_BASIC_DATA_STX_1 (0x5B)
#define DMSDCM_MDSM7_BASIC_DATA_HDR_1 (0x79)
#define DMSDCM_MDSM7_BASIC_DATA_HDR_2 (0x41)


/**
 * @brief 모본 DMS의 Basic 데이터 형식 (MDSM-7 RS-232 Protocol Document v3.07 참조)
 */
typedef struct
{
  uint8_t stx_1;
  uint8_t hdr_1;
  uint8_t hdr_2;
  uint8_t len;
  uint8_t speed;
  uint8_t dummy_carinfo[3];
  uint8_t dsm_event;
  // 그 외 필드는 생략
} __attribute__((packed)) DMSDCM_MDSM7BasicData;


/**
 * @brief 모듈 통합관리정보(Management Information Base)
 */
typedef struct
{
  char *logfile; ///< 로그 저장 파일경로
  KVHLOGLevel log_lv; ///< 로그 저장 레벨 (기본값 적용 또는 프로그램 실행 인자로 입력된다)

	/// Redis I/F 관리정보
	struct {
		char server_addr_str[INET_ADDRSTRLEN]; ///< Redis 서버 주소 문자열
		uint16_t server_port; ///< Redis 서버 포트번호
		KVHREDISHANDLE h_cmd; ///< Redis 기능 핸들 (PUBLISH 용)
		KVHREDISKey key_ivdct_protobuf;
		KVHREDISKey key_ivdct_raw;
		KVHREDISKey key_ivdct_publish;
  } redis_if;

  /// DMS와의 I/F 관리정보
  struct
  {
    char dev_name[KVH_MAXLINE+1]; ///< 시리얼포트 디바이스명 (예: /dev/ttyUSB1)
    long baudrate; ///< 시리얼포트 baudrate
    int sock; ///< 시리얼포트 소켓
    pthread_t rx_th; ///< 시리얼포트 데이터 수신 쓰레드
    volatile bool rx_th_running; ///< 수신 쓰레드 동작 여부
  } dms_if;

  struct {
    unsigned int dms_rx; ///< DMS로부터의 운전자상태데이터 수신 누적횟수
    unsigned int dgm_tx; ///< DGM으로의 운전자상태데이터 송신 누적횟수
  } io_cnt; ///< 입출력 누적횟수

} DMSDCM_MIB;

extern DMSDCM_MIB g_dmsdcm_mib;

// dmsdcm_dgm_if.c
int DMSDCM_InitDGMIF(DMSDCM_MIB *mib);
int DMSDCM_TransmitDriverStatusToDGM(DMSDCM_MIB *mib, KVHDMSDriverStatus status);

// dmsdcm_dms_if.c
int DMSDCM_InitDMSIF(DMSDCM_MIB *mib);


// dmsdcm_input_params.c
int DMSDCM_ParsingInputParameters(int argc, char *argv[], DMSDCM_MIB *mib);


#endif //KVH_DMSDCM_H
