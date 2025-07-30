/**
 * @file
 * @brief wlanaccess 라이브러리의 Open API들을 정의한 헤더 파일
 * @date 2019-05-08
 * @author gyun
 */

#ifndef LIBWLANACCESS_WLANACCESS_API_H
#define LIBWLANACCESS_WLANACCESS_API_H


#ifdef __cplusplus
extern "C" {
#endif


// 시스템 헤더 파일
#include <stdbool.h>
#include <stdint.h>

// 라이브러리 헤더 파일
#include "wlanaccess-api-params.h"


/**
 * @brief 라이브러리 및 칩 디바이스를 초기화한다(상세 내용 API 매뉴얼 참조).
 * @param[in] 라이브러리 로그메시지 출력 레벨
 * @retval 양수: 플랫폼이 지원하는 V2X 통신인터페이스의 수(성공)
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_Init(WalLogLevel log_level);


/**
 * @brief 라이브러리를 초기화한다(상세 내용 API 매뉴얼 참조).
 * @param[in] 라이브러리 로그메시지 출력 레벨
 * @retval 양수: 플랫폼이 지원하는 V2X 통신인터페이스의 수(성공)
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_Open(WalLogLevel log_level);


/**
 * @brief 라이브러리를 종료한다(상세 내용 API 매뉴얼 참조).
 */
void WAL_Close(void);


/**
 * @brief MPDU 수신을 위한 콜백함수를 등록한다(상세 내용 API 매뉴얼 참조).
 * @param[in] ProcessRxMPDUCallback 콜백함수 포인터
 */
void WAL_RegisterCallbackRxMPDU(
  void (*ProcessRxMPDUCallback)(const uint8_t *mpdu, WalMPDUSize mpdu_size, const struct WalMPDURxParams *params));


/**
 * @brief TimeSlot1 -> TimeSlot0 변경 이벤트를 수신할 콜백함수를 등록한다(상세 내용 API 매뉴얼 참조).
 * @param[in] ProcessTimeSlot0ChannelActiveEventCallback 콜백함수 포인터
 */
void WAL_RegisterCallbackTimeSlot0ChannelActiveEvent(
  void (*ProcessTimeSlot0ChannelActiveEventCallback)(unsigned int if_idx));


/**
 * @brief TimeSlot0 -> TimeSlot1 변경 이벤트를 수신할 콜백함수를 등록한다(상세 내용 API 매뉴얼 참조).
 * @param[in] ProcessTimeSlot1ChannelActiveEventCallback 콜백함수 포인터
 */
void WAL_RegisterCallbackTimeSlot1ChannelActiveEvent(
  void (*ProcessTimeSlot1ChannelActiveEventCallback)(unsigned int if_idx));


/**
 * @brief 특정 인터페이스의 채널 접속을 요청한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx 채널 접속할 인터페이스 식별번호(0부터 시작)
 * @param[in] ts0_chan_num TimeSlot0 구간에 접속할 채널번호
 * @param[in] ts1_chan_num TimeSlot1 구간에 접속할 채널번호
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_AccessChannel(unsigned int if_idx, WalChannelNumber ts0_chan_num, WalChannelNumber ts1_chan_num);


/**
 * @brief 특정 인터페이스가 접속 중인 채널을 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx 접속 채널을 확인할 인터페이스 식별번호(0부터 시작)
 * @param[out] ts0_chan_num TimeSlot0 구간에 접속 중인 채널번호가 반환될 변수의 포인터
 * @param[out] ts1_chan_num TimeSlot1 구간에 접속 중인 채널번호가 반환될 변수의 포인터
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_GetCurrentChannel(unsigned int if_idx, WalChannelNumber *ts0_chan_num, WalChannelNumber *ts1_chan_num);


/**
 * @brief 특정 인터페이스의 MAC 주소를 설정한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx MAC 주소를 설정할 인터페이스 식별번호(0부터 시작)
 * @param[in] addr 설정할 MAC 주소
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_SetIfMACAddress(unsigned int if_idx, const WalMACAddress addr);


/**
 * @brief 특정 인터페이스의 MAC 주소를 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx MAC 주소를 확인할 인터페이스 식별번호(0부터 시작)
 * @param[out] addr MAC 주소가 반환될 버퍼
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_GetIfMACAddress(unsigned int if_idx, WalMACAddress addr);


/**
 * @brief MAC 주소 문자열을 바이트열로 변환한다(상세 내용 API 매뉴얼 참조).
 * @param[in] str 변환할 MAC 주소 문자열
 * @param[out] addr 변환된 MAC 주소 바이트열이 저장될 버퍼
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_ConvertMACAddressStrToOctets(const char *str, WalMACAddress addr);


/**
 * @brief MPDU를 전송한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx MPDU를 전송할 인터페이스 식별번호(0부터 시작)
 * @param[in] mpdu 송신할 MPDU가 담긴 버퍼 (CRC 제외)
 * @param[in] mpdu_size 송신할 MPDU의 길이 (CRC 제외)
 * @param[in] tx_params 송신 제어 파라미터(채널번호, 데이터레이트, 파워, 만기시각)
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_TransmitMPDU(
  unsigned int if_idx,
  const uint8_t *mpdu,
  WalMPDUSize mpdu_size,
  const struct WalMPDUTxParams *tx_params);


/**
 * @brief 송신큐를 비운다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx 송신큐를 비울 인터페이스 식별번호(0부터 시작)
 * @param[in] timeslot 송신큐를 비울 TimeSlot (TimeSlot0 또는 TimeSlot1)
 * @param[in] q_idx 비울 큐 식별번호 (패킷 우선순위에 따른 큐 식별번호)
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_FlushTransmitQueue(unsigned int if_idx, WalTimeSlot timeslot, WalTxQueueIdx q_idx);


/**
 * @brief API 결과값에 대한 설명문자열을 반환한다(상세 내용 API 매뉴얼 참조).
 * @param[in] ret API 결과값
 * @return 결과코드에 대한 설명문자열
 */
const char * WAL_GetResultStr(int ret);


/**
 * @brief 특정 인터페이스에서의 IP 통신에 사용되는 TxProfile을 등록한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx IP Tx Profile을 등록할 인터페이스 식별번호(0부터 시작)
 * @param[in] tx_profile 등록할 Tx Profile 정보
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_RegisterTxProfile(unsigned int if_idx, const struct WalTxProfile *tx_profile);


/**
 * @brief 특정 인터페이스에 등록된 TxProfile을 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx Tx Profile을 확인할 인터페이스 식별번호(0부터 시작)
 * @param[out] tx_profile Tx Profile 정보가 반환될 구조체 포인터
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_GetTxProfile(unsigned int if_idx, struct WalTxProfile *tx_profile);


/**
 * @brief 특정 인터페이스에서의 IP 통신에 사용되는 TxProfile을 등록 해제한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx Tx Profile을 삭제할 인터페이스 식별번호(0부터 시작)
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_DeleteTxProfile(unsigned int if_idx);


/**
 * @brief 특정 인터페이스의 IPv6 주소를 설정한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx IPv6 주소를 설정할 인터페이스 식별번호(0부터 시작)
 * @param[in] ipv6_addr 설정할 IPv6 주소
 * @param[in] prefix_len IPv6 주소 프리픽스 길이
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_SetIPv6Address(unsigned int if_idx, const uint8_t *ipv6_addr, WalIPv6AddressPrefixLen prefix_len);


/**
 * @brief IPv6 주소 프리픽스 정보를 이용하여 인터페이스의 IPv6 주소를 설정한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx IPv6 주소를 설정할 인터페이스 식별번호(0부터 시작)
 * @param[in] prefix IPv6 설정할 IPv6 주소의 프리픽스 정보
 * @param[in] prefix_len 설정할 IPv6 주소의 프리픽스 길이(64이하의 값을 가져야 한다)
 * @param[out] ipv6_addr 설정된 IPv6 주소가 반환될 버퍼
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_SetIPv6AddressWithPrefixInfo(
  unsigned int if_idx,
  const uint8_t *prefix,
  WalIPv6AddressPrefixLen prefix_len,
  uint8_t *ipv6_addr);


/**
 * @brief IPv6 autoconfiguration 기법을 이용하여 자동으로 인터페이스의 링크로컬 주소를 설정한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx 링크로컬 IPv6 주소를 설정할 인터페이스 식별번호(0부터 시작)
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_SetAutoLinkLocalIPv6Address(unsigned int if_idx);


/**
 * @brief 인터페이스에 설정된 IPv6 주소(들)을 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx IPv6 주소를 확인할 인터페이스 식별번호(0부터 시작)
 * @param[out] set IPv6 주소 정보(들)이 반환될 구조체 포인터
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_GetIPv6Addresses(unsigned int if_idx, struct WalIPv6AddressSet *set);


/**
 * @brief 인터페이스에 설정된 링크로컬 IPv6 주소를 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx 링크로컬 IPv6 주소를 확인할 인터페이스 식별번호(0부터 시작)
 * @param[out] ipv6_addr 링크로컬 IPv6 주소 정보가 반환될 버퍼
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_GetLinkLocalIPv6Address(unsigned int if_idx, uint8_t *ipv6_ll_addr);


/**
 * @brief 특정 인터페이스에 설정된 특정 IPv6 주소를 삭제한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx IPv6 주소를 삭제할 인터페이스 식별번호(0부터 시작)
 * @param[in] ipv6_addr 삭제할 IPv6 주소
 * @param[in] prefix_len 삭제할 IPv6 주소의 프리픽스 길이
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_DeleteIPv6Address(unsigned int if_idx, const uint8_t *ipv6_addr, WalIPv6AddressPrefixLen prefix_len);


/**
 * @brief 특정 인터페이스에 Default gateway IPv6 주소를 설정한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx Default gateway IPv6 주소를 설정할 인터페이스 식별번호(0부터 시작)
 * @param[in] default_gw_addr 설정할 Default gateway IPv6 주소
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_SetDefaultGatewayIPv6Address(unsigned int if_idx, const uint8_t *default_gw_addr);


/**
 * @brief 특정 인터페이스에 설정되어 있는 Default gateway의 IPv6 주소를 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx Default gateway IPv6 주소를 확인할 인터페이스 식별번호(0부터 시작)
 * @param[out] default_gw_addr Default gateway IPv6 주소가 반환될 버퍼
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_GetDefaultGatewayIPv6Address(unsigned int if_idx, uint8_t *default_gw_addr);


/**
 * @brief 특정 인터페이스에 설정되어 있는 Default gateway IPv6 주소를 제거한다(상세 내용 API 매뉴얼 참조).
 * @param[in] if_idx Default gateway IPv6 주소를 제거할 인터페이스 식별번호(0부터 시작)
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_DeleteDefaultGatewayIPv6Address(unsigned int if_idx);


/**
 * @brief DNS 서버 IPv6 주소를 시스템에 설정한다(상세 내용 API 매뉴얼 참조).
 * @param[in] primary_dns 설정할 Primary DNS 서버 IPv6 주소
 * @param[in] secondary_dns_present Secondary DNS 서버 IPv6 주소 설정 여부
 * @param[in] secondary_dns 설정할 Secondary DNS 서버 IPv6 주소
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_SetDNSIPv6Address(const uint8_t *primary_dns, bool secondary_dns_present, const uint8_t *secondary_dns);


/**
 * @brief 시스템에 설정된 DNS 서버 IPv6 주소를 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[out] primary_dns Primary DNS 서버의 IPv6 주소가 반환될 버퍼
 * @param[out] secondary_dns_present Secondary DNS 서버 IPv6 주소 반환 여부가 저장될 변수 포인터
 * @param[out] secondary_dns Secondary DNS 서버의 IPv6 주소가 반환될 버퍼
 * @retval 0: 성공
 * @retval 음수(-WalResultCode): 실패
 */
int WAL_GetDNSIPv6Address(uint8_t *primary_dns, bool *secondary_dns_present, uint8_t *secondary_dns);


#ifdef _UNIT_TEST_
#ifdef _USE_TAP_DEVICE_

/**
 * @brief 단위테스트 시, TAP 디바이스로부터 전달되어야 하는 이더넷 패킷을 강제로 생성하여 전달하는 테스트 전용 함수
 * @param[in] if_idx 인터페이스 식별번호
 * @param[in] buf 전달할 이더넷 패킷
 * @param[in] rx_size 전달할 이더넷 패킷의 길이
 */
void WAL_ProcessRxPktFromTapDevice(unsigned int if_idx, const uint8_t *eth_pkt, size_t eth_pkt_size);

#endif // _USE_TAP_DEVICE_
#endif // _UNIT_TEST_


#ifdef __cplusplus
}
#endif

#endif //LIBWLANACCESS_WLANACCESS_API_H
