/** 
 * @file
 * @brief lteaccess 라이브러리의 Open API들을 정의한 헤더 파일
 * @date 2020-10-28
 * @author gyun
 */


#ifndef V2X_SW_LTEACCESS_API_H
#define V2X_SW_LTEACCESS_API_H

#ifdef __cplusplus
extern "C" {
#endif


// 라이브러리 헤더 파일
#include "lteaccess-api-params.h"


/**
 * @brief 라이브러리 및 칩 디바이스를 초기화한다(상세 내용 API 매뉴얼 참조).
 * @param[in] 라이브러리 로그메시지 출력 레벨
 * @retval 0: 성공
 * @retval 음수(-LalResultCode): 실패
 */
int LAL_Init(LalLogLevel log_level);


/**
 * @brief 라이브러리 및 칩 디바이스를 종료한다(상세 내용 API 매뉴얼 참조).
 */
void LAL_Close(void);


/**
 * @brief 특정 인터페이스를 특정 채널에 접속한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx 인터페이스 식별번호
 * @param[in] chan 채널번호
 * @retval 0: 성공
 * @retval 음수(-LalResultCode): 실패
 */
int LAL_AccessChannel(unsigned int if_idx, LalChannelNumber chan_num);


/**
 * @brief 특정 인터페이스가 접속 중인 채널을 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx 인터페이스 식별번호
 * @param[out] chan_num 채널번호가 저장될 변수의 포인터
 * @retval 0: 성공
 * @retval 음수(-LalResultCode): 실패
 */
int LAL_GetCurrentChannel(unsigned int if_idx, LalChannelNumber *chan_num);


/**
 * @brief 특정 인터페이스의 MAC 주소를 설정한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx 인터페이스 식별번호
 * @param[in] addr 설정할 MAC 주소 (3바이트 길이)
 * @retval 0: 성공
 * @retval 음수(-LalResultCode): 실패
 */
int LAL_SetIfMACAddress(unsigned int if_idx, int addr);


/**
 * @brief 특정 인터페이스의 MAC 주소를 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx 인터페이스 식별번호
 * @param[in] addr MAC 주소가 저장될 변수의 포인터
 * @retval 0: 성공
 * @retval 음수(-LalResultCode): 실패
 */
int LAL_GetIfMACAddress(unsigned int if_idx, int *addr);


/**
 * @brief 송신플로우를 등록한다.
 * @param[in] flow_params 등록할 송신플로우 정보
 * @retval 0: 성공
 * @retval 음수(-LalResultCode): 실패
 */
int LAL_RegisterTransmitFlow(struct LalTxFlowParams *flow_params);


/**
 * @brief MSDU를 전송한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx 인터페이스 식별번호
 * @param[in] msdu 전송할 MSDU
 * @param[in] msdu_size MSDU 사이즈
 * @param[in] tx_params 송신 파라미터
 * @retval 0: 성공
 * @retval 음수(-LalResultCode): 실패
 */
int LAL_TransmitMSDU(
  unsigned int if_idx,
  const uint8_t *msdu,
  LalMSDUSize msdu_size,
  const struct LalMSDUTxParams *tx_params);


/**
 * @brief MSDU 수신을 위한 콜백함수를 등록한다(상세 내용 API 매뉴얼 참조).
 * @param[in] ProcessRxMSDUCallback 콜백함수 포인터
 */
void LAL_RegisterCallbackRxMSDU(void (*ProcessRxMSDUCallback)(const uint8_t *msdu, size_t msdu_size));


#ifdef __cplusplus
}
#endif

#endif //V2X_SW_LTEACCESS_API_H
