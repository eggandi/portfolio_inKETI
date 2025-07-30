/**
 * @addtogroup cvcoctci_lib cvcoctci library API
 * @{
 *
 * @file
 * @brief cvcoctci 라이브러리의 Open API 들을 정의한 헤더 파일
 * @date 2019-09-14
 * @author gyun
 */

#ifndef V2X_SW_CVCOCTCI_API_H
#define V2X_SW_CVCOCTCI_API_H

// 시스템 헤더 파일
#include <stdint.h>

// 라이브러리 헤더 파일
#include "cvcoctci-api-params.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief libcvcoctci 를 초기화한다.
 * @param[in] log_level 로그출력레벨
 */
void Cvcoctci_Init(CvcoctciLogLevel log_level);


/**
 * @brief TCI 메시지를 OER 디코딩하고 파싱한다.
 * @param[in] msg 파싱할 TCI 메시지가 저장된 배열 포인터를 전달한다. (NULL 불가)
 * @param[in] msg_size 파싱할 TCI 메시지의 길이를 전달한다. (0 불가)
 * @param[out] params 파싱된 정보가 저장/반환될 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf TCI 메시지에 수납되어 있는 PDU 가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    outbuf 배열의 크기는 되도록 MAX_PDU_SIZE 를 사용하라.
 * @param[in] outbuf_size outbuf 의 크기를 전달한다.
 * @retval 양수: TCI 메시지 내에 PDU 가 수납되어 있는 경우 PDU 의 길이
 * @retval 0: TCI 메시지 내에 PDU 가 수납되어 있지 않은 경우 0
 * @retval 음수(-CvcoctciResultCode): 실패
 *
 * 호출자는 msg 인자에 TCI 메시지를 전달하며, msg_size 인자에 TCI 메시지의 길이를 명시한다. \n
 * API 는 params 인자에 파싱된 정보를 저장/반환한다. \n
 * API 는 outbuf 에 TCI 메시지 내 포함된 PDU 를 저장/반환한다. (TCI 메시지에 PDU 가 없는 경우 반환되지 않는다) \n
 * 호출자는 outbuf_size 인자에 outbuf 의 길이를 전달한다. \n
 * API 는 PDU 가 파싱된 경우 PDU 의 길이를 반환하고, PDU 가 없는 경우에는 0, 실패 시에는 음수를 반환한다.
 */
int Cvcoctci_DecodeAndParseTciMessage(
  const uint8_t *msg,
  size_t msg_size,
  struct CvcoctciParseParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 80211 Response 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct80211Response(
  const struct CvcoctciResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16093 Response 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct16093Response(
  const struct CvcoctciResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16094 Response 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct16094Response(
  const struct CvcoctciResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 29451 Response 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct29451Response(
  const struct CvcoctciResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI SutControl Response 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_ConstructSutControlResponse(
  const struct CvcoctciResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 80211 Indication 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct80211Indication(
  const struct CvcoctciIndicationConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16093 Indication 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct16093Indication(
  const struct CvcoctciIndicationConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16094 Indication 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct16094Indication(
  const struct CvcoctciIndicationConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 29451 Indication 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct29451Indication(
  const struct CvcoctciIndicationConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16093 ResponseInfo 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct16093ResponseInfo(
  const struct CvcoctciResponseInfoConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16094 ResponseInfo 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct16094ResponseInfo(
  const struct CvcoctciResponseInfoConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 29451 ResponseInfo 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct29451ResponseInfo(
  const struct CvcoctciResponseInfoConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI SutControl ResponseInfo 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_ConstructSutControlResponseInfo(
  const struct CvcoctciResponseInfoConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 80211 Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct80211Exception(
  const struct CvcoctciExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16093 Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct16093Exception(
  const struct CvcoctciExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16094 Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct16094Exception(
  const struct CvcoctciExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 29451 Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_Construct29451Exception(
  const struct CvcoctciExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI SutControl Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-CvcoctciResultCode): 실패
 */
int Cvcoctci_ConstructSutControlException(
  const struct CvcoctciExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 메시지의 파싱된 결과(struct CvcoctciParseParams)의 내용을 화면에 출력한다.
 * @param[in] log_level 출력 로그레벨
 * @param[in] desc 설명 문자열
 * @param[in] params 출력할 파싱 결과 구조체 포인터
 */
void Cvcoctci_PrintCvcoctciParseParams(
  CvcoctciLogLevel log_level, const char *desc, const struct CvcoctciParseParams *params);


/**
 * @brief TCI SetInitialState 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 값
 */
void Cvcoctci_PrintSetInitialState(bool data);


/**
 * @brief TCI SetWsmTxInfo 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintSetWsmTxInfo(const struct CvcoctciSetWsmTxInfo *data);


/**
 * @brief TCI StartWsmTx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 * @param[in] pdu_size PDU 크기
 */
void Cvcoctci_PrintStartWsmTx(const struct CvcoctciStartWsmTx *data, size_t pdu_size);


/**
 * @brief TCI StopWsmTx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintStopWsmTx(const struct CvcoctciStopWsmTx *data);


/**
 * @brief TCI StartWsmRx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintStartWsmRx(const struct CvcoctciStartWsmRx *data);


/**
 * @brief TCI StopWsmRx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintStopWsmRx(const struct CvcoctciStopWsmRx *data);


/**
 * @brief TCI AddTxProfile 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintAddTxProfile(const struct CvcoctciAddTxProfile *data);


/**
 * @brief TCI DelTxProfile 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintDelTxProfile(const struct CvcoctciDelTxProfile *data);


/**
 * @brief TCI IPv6TxRecord(StartIPv6Ping,StopIPv6Ping,StartIPv6Tx,StopIPv6Tx) 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintIPv6TxRecord(const struct CvcoctciIPv6TxRecord *data);


/**
 * @brief TCI IPv6RxRecord(StartIPv6Rx, StopIPv6Rx) 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintIPv6RxRecord(const struct CvcoctciIPv6RxRecord *data);


/**
 * @brief TCI StartWsaTxPeriodic 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintStartWsaTxPeriodic(const struct CvcoctciStartWsaTxPeriodic *data);


/**
 * @brief TCI StopWsaTxPeriodic 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintStopWsaTxPeriodic(const struct CvcoctciStopWsaTxPeriodic *data);


/**
 * @brief TCI AddWsaProviderService 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintAddWsaProviderService(const struct CvcoctciAddWsaProviderService *data);


/**
 * @brief TCI ChangeWsaProviderService 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintChangeWsaProviderService(const struct CvcoctciChangeWsaProviderService *data);


/**
 * @brief TCI DelWsaProviderService 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintDelWsaProviderService(const struct CvcoctciDelWsaProviderService *data);


/**
 * @brief TCI AddUserService 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintAddUserService(const struct CvcoctciAddUserService *data);


/**
 * @brief TCI DelUserService 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintDelUserService(const struct CvcoctciDelUserService *data);


/**
 * @brief TCI GetIPv6InterfaceInfo 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintGetIPv6InterfaceInfo(const struct CvcoctciGetIPv6InterfaceInfo *data);


/**
 * @brief TCI SetIPv6Address 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci_PrintSetIPv6Address(const struct CvcoctciSetIPv6Address *data);


/**
 * @brief TCI Restart 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintRestart(bool data);


/**
 * @brief TCI RequestSutAvailability 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintRequestSutAvailability(bool data);


/**
 * @brief TCI RequestSutInfo 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintRequestSutInfo(bool data);


/**
 * @brief TCI SetTestId 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetTestId(const struct CvcoctciSetTestId *data);


/**
 * @brief TCI EnableGpsInput 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintEnableGpsInput(bool data);


/**
 * @brief TCI SetLatitude 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetLatitude(CvcoctciSetLatitude data);


/**
 * @brief TCI SetLongitude 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetLongitude(CvcoctciSetLongitude data);


/**
 * @brief SetElevation 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetElevation(CvcoctciSetElevation data);


/**
 * @brief SetVehicleEventFlags 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetVehicleEventFlags(const struct CvcoctciSetVehicleEventFlags *data);


/**
 * @brief SetPositionalAccuracy 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetPositionalAccuracy(const struct CvcoctciSetPositionalAccuracy *data);


/**
 * @brief SetSpeed 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetSpeed(CvcoctciSetSpeed data);


/**
 * @brief SetHeading 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetHeading(CvcoctciSetHeading data);


/**
 * @brief SetGpsTime 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetGpsTime(CvcoctciSetGpsTime data);


/**
 * @brief SetAccelerationSet4Way 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetAccelerationSet4Way(const struct CvcoctciSetAccelerationSet4Way *data);


/**
 * @brief SetTransmissionState 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetTransmissionState(CvcoctciSetTransmissionState data);


/**
 * @brief SetSteeringWheelAngle 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetSteeringWheelAngle(CvcoctciSetSteeringWheelAngle data);


/**
 * @brief SetBrakeSystemStatus 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetBrakeSystemStatus(const struct CvcoctciSetBrakeSystemStatus *data);


/**
 * @brief SetVehicleSize 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetVehicleSize(const struct CvcoctciSetVehicleSize *data);


/**
 * @brief SetExteriorLights 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci_PrintSetExteriorLights(const struct CvcoctciSetExteriorLights *data);


#ifdef __cplusplus
}
#endif

#endif //V2X_SW_CVCOCTCI_API_H

/**
 * @}
 */
