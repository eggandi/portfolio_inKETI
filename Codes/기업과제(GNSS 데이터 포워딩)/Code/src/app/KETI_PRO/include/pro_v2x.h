#include <stdio.h>     // 사용된 함수: printf, fprintf, perror, snprintf
#include <stdlib.h>    // 사용된 함수: malloc, free, exit, strdup
#include <stdint.h>    // 사용된 타입: uint8_t, uint16_t 등
#include <string.h>    // 사용된 함수: memset, memcpy, strlen, strdup
#include <sys/stat.h>  // stat(), mkdir() (Linux/macOS)
#include <sys/types.h> // stat(), mkdir()
#include <unistd.h>    // 사용된 함수: access, mkdir
#include <stdbool.h>   // 사용된 타입: bool, true, false
#include <fcntl.h>     // 사용된 함수: open, O_RDWR, O_NOCTTY, O_SYNC
#include <inttypes.h>  // 사용된 매크로: PRIu64
#include "ltev2x-hal/ltev2x-hal.h"

#ifndef _D_HEADER_PRO_V2X
#define _D_HEADER_PRO_V2X

struct __attribute__((__packed__)) pro_waytis_header_t
{
  uint32_t Seconds;
  uint32_t Nanoseconds;
  uint8_t Source_Address[6];
  uint32_t Sequence_Number;
  uint8_t Destination_Address[6];
  uint8_t Priority;
  uint8_t Transceiver_ID;
  uint8_t Channel_Busy_Ratio[2];
  uint8_t Channel_Number;
  uint8_t RSSI_A[2];
  uint8_t RSSI_B[2];
};

/**
 * @brief 어플리케이션 동작 유형
 */
enum eOperationType
{
  kOperationType_rx_only, ///< 수신동작만 수행
  kOperationType_trx, ///< 송수신동작 수행
  kOperationType_max = kOperationType_trx
};
typedef unsigned int Operation; ///< @ref eOperationType


/**
 * @brief 로그메시지 출력 레벨
 */
enum eDbgMsgLevel
{
  kDbgMsgLevel_None, ///< 미출력
  kDbgMsgLevel_Err, ///< 에러
  kDbgMsgLevel_Init, ///< 에러
  kDbgMsgLevel_Event, ///< 이벤트 출력
  kDbgMsgLevel_Dump, ///< 메시지 hexdump 출력
  kDbgMsgLevel_max = kDbgMsgLevel_Dump
};
typedef unsigned int DbgMsgLevel; ///< @ref eDbgMsgLevel

/**
 * @brief 어플리케이션 관리정보
 */
struct pro_config_v2x_t
{
  Operation op; ///< 어플리케이션 동작 유형
  char dev_name[256]; ///< LTE-V2X 통신 디바이스 이름
  LTEV2XHALTxFlowType tx_type; ///< 송신 유형
  unsigned int psid; ///< 송신 또는 수신하고자 하는 PSID
  DbgMsgLevel dbg; ///< 디버그 메시지 출력 레벨
  unsigned int lib_dbg; ///< V2X 라이브러리 디버그 메시지 출력 레벨
  unsigned int rx_chan_num; ///< V2X 수신 채널 번호 (uper 파일 링크생성할 때 사용)
  unsigned int rx_power; ///< V2X 수신 세기 (uper 파일 링크생성할 때 사용)
  unsigned int rx_bandwidth; ///< V2X 수신 세기 (uper 파일 링크생성할 때 사용)
  unsigned int tx_chan_num; ///< WSM 송신 채널번호 (실제 전송에는 사용되지 않고 WSM 확장 헤더에 수납되는 용도로만 사용)
  unsigned int tx_datarate; ///< WSM 송신 데이터레이트 (실제 전송에는 사용되지 않고 WSM 확장 헤더에 수납되는 용도로만 사용)
  LTEV2XHALPower tx_power; ///< WSM 송신 파워 (실제 전송에도 사용되며, WSM 확장 헤더에도 수납된다)
  LTEV2XHALPriority tx_priority; ///< WSM 송신에 사용되는 우선순위
  LTEV2XHALMSDUSize tx_wsm_body_len; ///< 송신 WSM body 의 길이
  unsigned int tx_interval; ///< 송신 주기 (usec 단위)

  bool tx_running;
};

#endif //?_D_HEADER_PRO_V2X
extern int g_sockfd;
extern struct sockaddr_in g_v2x_addr;
extern int PRO_V2X_Init(void);