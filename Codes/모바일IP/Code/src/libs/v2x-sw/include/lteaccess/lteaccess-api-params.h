/** 
 * @file
 * @brief lteaccess 라이브러리의 Open API 파라미터들을 정의한 헤더 파일
 * @date 2020-10-28
 * @author gyun
 */


#ifndef V2X_SW_LTEACCESS_API_PARAMS_H
#define V2X_SW_LTEACCESS_API_PARAMS_H


// 라이브러리 헤더 파일
#include "lteaccess-types.h"


/**
 * @brief MSDU 송신정보 파라미터
 *
 * LAL_TransmitMSDU() API를 통해 어플리케이션에서 라이브러리로 전달된다.
 */
struct LalMSDUTxParams
{
  LalPSID psid; ///< 송신 PSID
  LalChannelNumber chan_num; ///< 송신채널번호. (현재 실시간 채널변경 기능은 지원되지 않으므로 무시된다)
  LalPower tx_power; ///< 송신파워(dBm 단위) - 무시된다. LAL_RegisterTransmitFlow()를 통해 등록된 송신파워가 적용된다.
  LalPriority priority; ///< 송신 우선순위
};


/**
 * @brief 송신 Flow 파라미터. 주기적으로 또는 이벤트성으로 전송되는 메시지에 대한 정보를 등록하는데 사용된다.
 */
struct LalTxFlowParams
{
  LalPSID psid; ///< 송신 PSID
  LalTxFlowType type; ///< 송신 유형
  LalPower power; ///< 송신 파워
  LalPriority priority; ///< 송신 우선순위
  LalMCSindex mcs_index; ///< 송신 MCS index
  LalTxFlowInterval interval; ///< 송신 주기 (송신유형이 SPS인 경우에만 사용된다)
  LalMSDUSize max_payload_size; ///< 송신 페이로드 최대 길이 (상위계층 페이로드 대상 - 예: WSM) (송신유형이 SPS인 경우에만 사용된다)
};


#endif //V2X_SW_LTEACCESS_API_PARAMS_H
