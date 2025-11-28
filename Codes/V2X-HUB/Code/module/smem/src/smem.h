/**
 * @file
 * @brief SMEM(Sidelink Message Exchange Module) 기능 구현
 * @date 2024-11-19
 * @author user
 */

#ifndef KVH_SMEM_H
#define KVH_SMEM_H


// 시스템 헤더파일
#include <stdio.h>     // 사용된 함수: printf, fprintf, perror, snprintf
#include <stdlib.h>    // 사용된 함수: malloc, free, exit, strdup
#include <stdint.h>    // 사용된 타입: uint8_t, uint16_t 등
#include <string.h>    // 사용된 함수: memset, memcpy, strlen, strdup
#include <sys/stat.h>  // stat(), mkdir() (Linux/macOS)
#include <unistd.h>    // 사용된 함수: access, mkdir
#include <dirent.h>    // 사용된 함수: opendir, readdir, closedir
#include <assert.h>    // 사용된 매크로: assert
#include <stdbool.h>   // 사용된 타입: bool, true, false
#include <fcntl.h>     // 사용된 함수: open, O_RDWR, O_NOCTTY, O_SYNC
#include <arpa/inet.h> // 사용된 함수: inet_pton, htons
#include <sys/socket.h> // 사용된 함수: socket, connect
#include <netinet/in.h> // 사용된 타입: sockaddr_in
#include <pthread.h>   // 사용된 타입: pthread_t, pthread_create, pthread_join
#include <sys/timerfd.h> // 사용된 함수: timerfd_create, timerfd_settime
#include <signal.h>    // 사용된 함수: sigaction, SIGINT, SIGTERM
#include <time.h>      // 사용된 타입: struct timespec, clock_gettime
#include <ifaddrs.h> // 사용된 함수: getifaddrs, freeifaddrs
#include <getopt.h> // 사용된 함수: getopt_long, optarg, optind, 타입: option
#include <stdarg.h> // 사용된 함수: va_list, va_start, va_end, vsnprintf, 타입: va_list
#include <malloc.h> // 사용된 함수: va_list, va_start, va_end, vsnprintf, 타입: va_list

// 의존 헤더파일
#include "dot2/dot2.h"
#include "dot3/dot3.h"
#include "wlanaccess/wlanaccess.h"

// 공용 헤더파일
#include "kvh_common.h"
#include "internal_data_fmt/kvh_internal_data_fmt_uds.h"

// 공용 라이브러리 헤더파일
#include "kvhlog.h"
#include "kvhuds.h"
#include "kvhredis.h"

// LTE-V2X 관련 헤더파일 TODO:통합예정
#include "ltev2x_if/ltev2x.h"


#define SMEM_DEFAULT_SLIB_LOG_LEVEL (1) ///< sidelink 라이브러리 기본 로그 레벨
#define SMEM_PVD_MSG_PSID (82057) ///< PVD 메시지 PSID
#define SMEM_BSM_MSG_PSID (32) ///< BSM 메시지 PSID
#define SMEM_RSA_MSG_PSID (82053) ///< RSA 메시지 PSID
#define SMEM_TIM_MSG_PSID (82054) ///< TIM 메시지 PSID
#define SMEM_SPAT_MSG_PSID (82055) ///< SPAT 메시지 PSID
#define SMEM_MAP_MSG_PSID (82056) ///< MAP 메시지 PSID


/**
 * @brief 모듈 통합관리정보(Management Information Base)
 */
typedef struct
{
  char *logfile; ///< 로그 저장 파일경로
  KVHLOGLevel log_lv; ///< 로그 저장 레벨 (기본값 적용 또는 프로그램 실행 인자로 입력된다)
  long slib_log_lv; ///< Sidelink 라이브러리 로그 출력 레벨

  /// DGM과의 I/F 관리정보
  /// SMEM -> DGM 단방향 데이터 전달만 수행된다.
  /// DGM으로 UDS 데이터를 전송하는 주체는 단일 쓰레드(Sidelink 메시지 수신 쓰레드)이므로 송신큐가 필요하지 않다.
  struct {
    KVHUDSINFO uds; ///< UDS 통신기능 관리정보 (클라이언트로 동작)
  } dgm_if;

  /// CSM과의 I/F 관리정보
  /// SMEM -> CSM 단방향 데이터 전달만 수행된다.
  /// CSM으로 UDS 데이터를 전송하는 주체는 단일 쓰레드(Sidelink 메시지 수신 쓰레드)이므로 송신큐가 필요하지 않다.
  struct {
    KVHUDSINFO uds; ///< UDS 통신기능 관리정보 (클라이언트로 동작)
  } csm_if;

	struct {
		char server_addr_str[INET_ADDRSTRLEN]; ///< Redis 서버 주소 문자열
		uint16_t server_port; ///< Redis 서버 포트번호
		KVHREDISHANDLE h; ///< Redis 기능 핸들
	} redis_if;

  struct {
    unsigned int v2x_rx; ///< V2X 수신 누적횟수
    unsigned int adem_tx; /// ADEM으로의 전송 누적횟수
    unsigned int csm_tx; /// CSM으로의 전송 누적횟수
  } cnt; ///< 각종 이벤트 누적횟수
	
	//20250903 Dong 추가
	int sidelink_type;
	LTEV2X_MIB v2x;
	
} SMEM_MIB;

extern SMEM_MIB g_smem_mib;

// csm_if/smem_csm_if.c
int SMEM_InitCSMIF(SMEM_MIB *mib);
int SMEM_TransmitUDSDataToCSM(SMEM_MIB *mib, uint8_t *data, KVHIFDataLen len);

// dgm_if/smem_dgm_if.c
int SMEM_InitDGMIF(SMEM_MIB *mib);
int SMEM_TransmitUDSDataToDGM(SMEM_MIB *mib, uint8_t *data, KVHIFDataLen len);

// redis_if/smem_redis_if.c
int SMEM_InitRedisIF(SMEM_MIB *mib);
int SMEM_ProcessRedisSETData(KVHREDISKeySUBDOMAIN subdomain, KVHREDISKeyDataModelType datamodeltype, KVHREDISKeyDataModelSubType subdatatype, const uint8_t *data, size_t data_size);
int SMEM_ProcessRedisSETKSR1600(uint32_t psid, uint8_t *data, size_t data_size);

// sidelink_if/smem_sidelink_if.c
int SMEM_InitSidelinkIF(SMEM_MIB *mib);

// smem_input_params.c
int SMEM_ParsingInputParameters(int argc, char *argv[], SMEM_MIB *mib);



#endif //KVH_SMEM_H
