#include "relay-internal-system.h"
#include "relay-extern-defines.h"

#ifndef _D_HEADER_RELAY_INNO_V2X
#define _D_HEADER_RELAY_INNO_V2X

#ifndef _D_HEADER_RELAY_INNO_V2X_TX
#include "relay_v2x_tx.h"
#endif
#ifndef _D_HEADER_RELAY_INNO_V2X_RX
#include "relay_v2x_rx.h"
#endif
#ifndef _D_HEADER_RELAY_INNO_V2X_DOT2
#include "relay_v2x_dot2.h"
#endif
#ifndef _D_HEADER_RELAY_INNO_V2X_J2735
#include "relay_v2x_j2735.h"
#endif

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
struct relay_inno_config_v2x_t
{
  Operation op; ///< 어플리케이션 동작 유형
  char dev_name[128]; ///< LTE-V2X 통신 디바이스 이름
  unsigned int tx_type; ///< 송신 유형
  unsigned int psid; ///< 송신 또는 수신하고자 하는 PSID
  DbgMsgLevel dbg; ///< 디버그 메시지 출력 레벨
  unsigned int lib_dbg; ///< V2X 라이브러리 디버그 메시지 출력 레벨
  unsigned int chan_num; ///< WSM 송신 채널번호 (실제 전송에는 사용되지 않고 WSM 확장 헤더에 수납되는 용도로만 사용)
  unsigned int tx_datarate; ///< WSM 송신 데이터레이트 (실제 전송에는 사용되지 않고 WSM 확장 헤더에 수납되는 용도로만 사용)
  int tx_power; ///< WSM 송신 파워 (실제 전송에도 사용되며, WSM 확장 헤더에도 수납된다)
  unsigned int  tx_priority; ///< WSM 송신에 사용되는 우선순위
  unsigned int tx_interval; ///< 송신 주기 (usec 단위)
	bool tx_running;
	struct relay_inno_config_v2x_rx_t rx;
	struct relay_inno_config_v2x_dot2_t dot2;
	struct relay_inno_config_v2x_j2735_t j2735;
};

#endif //?_D_HEADER_RELAY_INNO_V2X

extern bool RELAY_INNO_V2X_Psid_Filter(unsigned int psid);
EXTERN_API int RELAY_INNO_V2X_Init(void);