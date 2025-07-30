/**
 * @file
 * @brief cvcoctci 라이브러리의 Open API 들을 정의한 헤더 파일
 * @date 2019-09-14
 * @author gyun
 */

#ifndef V2X_SW_CVCOCTCI3_API_H
#define V2X_SW_CVCOCTCI3_API_H

// 시스템 헤더 파일
#include <stdint.h>

// 라이브러리 헤더 파일
#include "cvcoctci3-api-params.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief libcvcoctci 를 초기화한다.
 * @param[in] log_level 로그출력레벨
 */
void Cvcoctci3_Init(Cvcoctci3LogLevel log_level);


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
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 *
 * 호출자는 msg 인자에 TCI 메시지를 전달하며, msg_size 인자에 TCI 메시지의 길이를 명시한다. \n
 * API 는 params 인자에 파싱된 정보를 저장/반환한다. \n
 * API 는 outbuf 에 TCI 메시지 내 포함된 PDU 를 저장/반환한다. (TCI 메시지에 PDU 가 없는 경우 반환되지 않는다) \n
 * 호출자는 outbuf_size 인자에 outbuf 의 길이를 전달한다. \n
 * API 는 PDU 가 파싱된 경우 PDU 의 길이를 반환하고, PDU 가 없는 경우에는 0, 실패 시에는 음수를 반환한다.
 */
int Cvcoctci3_DecodeAndParseTciMessage(
  const uint8_t *msg,
  size_t msg_size,
  struct Cvcoctci3ParseParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 80211 Response 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct80211Response(
  const struct Cvcoctci3ResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16093 DSRC Response 메시지를 생성한다. *
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct16093DsrcResponse(
  const struct Cvcoctci3ResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16093 PC5 Response 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct16093Pc5Response(
  const struct Cvcoctci3ResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16094 Response 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct16094Response(
  const struct Cvcoctci3ResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 29451 Response 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct29451Response(
  const struct Cvcoctci3ResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 31611 Response 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct31611Response(
  const struct Cvcoctci3ResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI SutControl Response 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_ConstructSutControlResponse(
  const struct Cvcoctci3ResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI ProxyCv2x Response 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_ConstructProxyCv2xResponse(
  const struct Cvcoctci3ResponseConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 80211 Indication 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct80211Indication(
  const struct Cvcoctci3IndicationConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16093 DSRC Indication 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct16093DsrcIndication(
  const struct Cvcoctci3IndicationConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16093 PC5 Indication 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct16093Pc5Indication(
  const struct Cvcoctci3IndicationConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16094 Indication 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct16094Indication(
  const struct Cvcoctci3IndicationConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 29451 Indication 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct29451Indication(
  const struct Cvcoctci3IndicationConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 31611 Indication 메시지를 생성한다.
 *
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct31611Indication(
  const struct Cvcoctci3IndicationConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16093 DSRC ResponseInfo 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct16093DsrcResponseInfo(
  const struct Cvcoctci3ResponseInfoConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16093 PC5 ResponseInfo 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct16093Pc5ResponseInfo(
  const struct Cvcoctci3ResponseInfoConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16094 ResponseInfo 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct16094ResponseInfo(
  const struct Cvcoctci3ResponseInfoConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 29451 ResponseInfo 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct29451ResponseInfo(
  const struct Cvcoctci3ResponseInfoConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 31611 ResponseInfo 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct31611ResponseInfo(
  const struct Cvcoctci3ResponseInfoConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI SutControl ResponseInfo 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_ConstructSutControlResponseInfo(
  const struct Cvcoctci3ResponseInfoConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI ProxyCv2x ResponseInfo 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_ConstructProxyCv2xResponseInfo(
  const struct Cvcoctci3ResponseInfoConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 80211 Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct80211Exception(
  const struct Cvcoctci3ExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16093 DSRC Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct16093DsrcException(
  const struct Cvcoctci3ExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16093 PC5 Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct16093Pc5Exception(
  const struct Cvcoctci3ExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 16094 Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct16094Exception(
  const struct Cvcoctci3ExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 29451 Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct29451Exception(
  const struct Cvcoctci3ExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 31611 Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_Construct31611Exception(
  const struct Cvcoctci3ExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI SutControl Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_ConstructSutControlException(
  const struct Cvcoctci3ExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI ProxyCv2x Exception 메시지를 생성한다.
 * @param[in] params 메시지 생성을 위한 정보 파라미터 구조체 포인터를 전달한다. (NULL 불가)
 * @param[out] outbuf 생성된 메시지가 저장/반환될 배열 포인터를 전달한다. (NULL 불가)
 *                    되도록 TCI_MSG_MAX_SIZE 를 사용한다.
 * @param[in] outbuf_size outbuf 배열의 크기를 전달한다.
 * @retval 양수: 생성된 메시지의 길이
 * @retval 음수(-Cvcoctci3ResultCode): 실패
 */
int Cvcoctci3_ConstructProxyCv2xException(
  const struct Cvcoctci3ExceptionConstructParams *params,
  uint8_t *outbuf,
  size_t outbuf_size);


/**
 * @brief TCI 메시지의 파싱된 결과(struct Cvcoctci3ParseParams)의 내용을 화면에 출력한다.
 * @param[in] log_level 출력 로그레벨
 * @param[in] desc 설명 문자열
 * @param[in] params 출력할 파싱 결과 구조체 포인터
 */
void Cvcoctci3_PrintCvcoctci3ParseParams(
  Cvcoctci3LogLevel log_level, const char *desc, const struct Cvcoctci3ParseParams *params);


/**
 * @brief TCI SetInitialState 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 값
 */
void Cvcoctci3_PrintSetInitialState(bool data);


/**
 * @brief TCI SetWsmTxInfo 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintSetWsmTxInfo(const struct Cvcoctci3SetWsmTxInfo *data);


/**
 * @brief TCI StartWsmTx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 * @param[in] pdu_size PDU 크기
 */
void Cvcoctci3_PrintStartWsmTx(const struct Cvcoctci3StartWsmTx *data, size_t pdu_size);

/**
 * @brief TCI Pc5StartWsmTx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 * @param[in] pdu_size PDU 크기
 * */
void Cvcoctci3_PrintPc5StartWsmTx(const struct Cvcoctci3Pc5StartWsmTx *data, size_t pdu_size);

/**
 * @brief TCI StopWsmTx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintStopWsmTx(const struct Cvcoctci3StopWsmTx *data);


/**
 * @brief TCI StartWsmRx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintStartWsmRx(const struct Cvcoctci3StartWsmRx *data);


/**
 * @brief TCI StopWsmRx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintStopWsmRx(const struct Cvcoctci3StopWsmRx *data);


/**
 * @brief RequestWsmTxCount 메시지 내용을 화면에 출력한다. * 
 * @param[in] data 출력할 메시지 정보 포인터
 * */
void Cvcoctci3_PrintRequestWsmTxCount(bool data);


/**
 * @brief RequestWsmRxCount 메시지 내용을 화면에 출력한다.
 * 
 * @param[in] data 출력할 메시지 정보 포인터
 * */
void Cvcoctci3_PrintRequestWsmRxCount(bool data);


/**
 * @brief RequestWsmTxCountReset 메시지 내용을 화면에 출력한다. * 
 * @param[in] data 출력할 메시지 정보 포인터
 * */
void Cvcoctci3_PrintRequestWsmTxCountReset(bool data);


/**
 * @brief RequestWsmRxCountReset 메시지 내용을 화면에 출력한다. 
 * @param[in] data 출력할 메시지 정보 포인터
 * */
void Cvcoctci3_PrintRequestWsmRxCountReset(bool data);


/**
 * @brief TCI AddTxProfile 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintAddTxProfile(const struct Cvcoctci3AddTxProfile *data);


/**
 * @brief TCI DelTxProfile 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintDelTxProfile(const struct Cvcoctci3DelTxProfile *data);


/**
 * @brief TCI IPv6TxRecord(StartIPv6Ping,StopIPv6Ping,StartIPv6Tx,StopIPv6Tx) 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintIPv6TxRecord(const struct Cvcoctci3IPv6TxRecord *data);


/**
 * @brief TCI IPv6RxRecord(StartIPv6Rx, StopIPv6Rx) 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintIPv6RxRecord(const struct Cvcoctci3IPv6RxRecord *data);


/**
 * @brief TCI StartWsaTxPeriodic 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintStartWsaTxPeriodic(const struct Cvcoctci3StartWsaTxPeriodic *data);


/**
 * @brief TCI StopWsaTxPeriodic 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintStopWsaTxPeriodic(const struct Cvcoctci3StopWsaTxPeriodic *data);


/**
 * @brief TCI AddWsaProviderService 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintAddWsaProviderService(const struct Cvcoctci3AddWsaProviderService *data);


/**
 * @brief TCI ChangeWsaProviderService 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintChangeWsaProviderService(const struct Cvcoctci3ChangeWsaProviderService *data);


/**
 * @brief TCI DelWsaProviderService 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintDelWsaProviderService(const struct Cvcoctci3DelWsaProviderService *data);


/**
 * @brief TCI AddUserService 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintAddUserService(const struct Cvcoctci3AddUserService *data);


/**
 * @brief TCI DelUserService 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintDelUserService(const struct Cvcoctci3DelUserService *data);


/**
 * @brief TCI GetIPv6InterfaceInfo 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintGetIPv6InterfaceInfo(const struct Cvcoctci3GetIPv6InterfaceInfo *data);


/**
 * @brief TCI SetIPv6Address 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 */
void Cvcoctci3_PrintSetIPv6Address(const struct Cvcoctci3SetIPv6Address *data);


/**
 * @brief TCI Restart 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintRestart(bool data);


/**
 * @brief TCI RequestSutAvailability 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintRequestSutAvailability(bool data);


/**
 * @brief TCI RequestSutInfo 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintRequestSutInfo(bool data);


/**
 * @brief TCI SetTestId 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetTestId(const struct Cvcoctci3SetTestId *data);


/**
 * @brief TCI EnableGpsInput 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintEnableGpsInput(bool data);


/**
 * @brief TCI SetLatitude 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetLatitude(Cvcoctci3SetLatitude data);


/**
 * @brief TCI SetLongitude 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetLongitude(Cvcoctci3SetLongitude data);


/**
 * @brief SetElevation 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetElevation(Cvcoctci3SetElevation data);


/**
 * @brief SetVehicleEventFlags 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetVehicleEventFlags(const struct Cvcoctci3SetVehicleEventFlags *data);


/**
 * @brief SetPositionalAccuracy 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetPositionalAccuracy(const struct Cvcoctci3SetPositionalAccuracy *data);


/**
 * @brief SetSpeed 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetSpeed(Cvcoctci3SetSpeed data);


/**
 * @brief SetHeading 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetHeading(Cvcoctci3SetHeading data);


/**
 * @brief SetGpsTime 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetGpsTime(Cvcoctci3SetGpsTime data);


/**
 * @brief SetAccelerationSet4Way 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetAccelerationSet4Way(const struct Cvcoctci3SetAccelerationSet4Way *data);


/**
 * @brief RequestSutStatus 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 * */
void Cvcoctci3_PrintRequestSutStatus(bool data);


/**
 * @brief SetTransmissionState 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetTransmissionState(Cvcoctci3SetTransmissionState data);


/**
 * @brief SetSteeringWheelAngle 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetSteeringWheelAngle(Cvcoctci3SetSteeringWheelAngle data);


/**
 * @brief SetBrakeSystemStatus 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetBrakeSystemStatus(const struct Cvcoctci3SetBrakeSystemStatus *data);


/**
 * @brief SetVehicleSize 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetVehicleSize(const struct Cvcoctci3SetVehicleSize *data);


/**
 * @brief SetExteriorLights 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 */
void Cvcoctci3_PrintSetExteriorLights(const struct Cvcoctci3SetExteriorLights *data);

/**
 * @brief StartBsmTx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 * */
void Cvcoctci3_PrintStartBsmTx(const struct Cvcoctci3StartBsmTx *data);


/**
 * @brief StopBsmTx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 * */
void Cvcoctci3_PrintStopBsmTx(const struct Cvcoctci3StopBsmTx *data);

/**
 * @brief EnableCongestionMitigation 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 * */
void Cvcoctci3_PrintEnableCongestionMitigation(bool data);


/**
 * @brief SetTemporaryID 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 * */
void Cvcoctci3_PrintSetTemporaryID(uint8_t *data);


/**
 * @brief StartBsmRx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 * */
void Cvcoctci3_PrintStartBsmRx(const struct Cvcoctci3StartBsmRx *data);


/**
 * @brief TCI Shutdown 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 * */
void CVcoctci3_PrintShutdown(bool data);


/**
 * @brief SendUeConfigXML 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 * */
void Cvcoctci3_PrintSendUeConfigXML(const struct Cvcoctci3SendUeConfigXML *data);


/**
 * @brief SetUeconfig 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 * */
void Cvcoctci3_PrintSetUeConfig(const struct Cvcoctci3SetUeConfig *data);


/**
 * @brief SetFlowConfig 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 * */
void Cvcoctci3_PrintSetFlowConfigs(const struct Cvcoctci3SetFlowConfigs *data);


/**
 * @brief SendATcommand 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지
 * */
void Cvcoctci3_PrintSendATcommand(const struct Cvcoctci3SendATCommand *data);


/**
 * @brief StartUdpProxyTx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 * */
void Cvoctci3_PrintStartUdpProxyTx(const struct Cvcoctci3StartUdpProxyTx *data);


/**
 * @brief StartUdpProxyRx 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 * */
void Cvcoctci3_PrintStartUdpProxyRx(const struct Cvcoctci3StartUdpProxyRx *data);


/**
 * @brief StopUdpProxy 메시지 내용을 화면에 출력한다.
 * @param[in] data 출력할 메시지 구조체 포인터
 * */
void Cvcoctci3_PrintStopUdpProxy(const struct Cvcoctci3StopUdpProxy *data);

#ifdef __cplusplus
}
#endif

#endif //V2X_SW_CVCOCTCI3_API_H
