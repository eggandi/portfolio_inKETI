/**
 * @file
 * @brief wlanaccess 라이브러리의 Open API 파라미터를 정의한 헤더파일
 * @date 2019-08-29
 * @author gyun
 */


#ifndef LIBWLANACCESS_WLANACCESS_API_PARAMS_H
#define LIBWLANACCESS_WLANACCESS_API_PARAMS_H


// 라이브러리 헤더 파일
#include "wlanaccess-types.h"


/**
 * @brief MPDU 송신정보 파라미터
 *
 * WAL_TransmitMPDU() API를 통해 어플리케이션에서 라이브러리로 전달된다.
 */
struct WalMPDUTxParams
{
  /// 송신채널번호.
  /// kWalChannelNumber_KoreaV2XMin ~ kWalChannelNumber_KoreaV2XMax 사이의 값을 사용할 수 있다.
  WalChannelNumber chan_num;
  /// 송신파워(dBm 단위).
  WalPower tx_power;
  /// 송신데이터레이트(500kbps 단위).
  /// 유효하지 않은 값을 명시하면, 기본값(6Mbps)로 조정되어 전송된다.
  WalDataRate datarate;
  /// (현재시간으로부터의)유효기간(마이크로초 단위).
  /// 송신 전에 유효기간이 만기되면(버퍼링 등의 이유로) 패킷이 송신되지 않는다.
  uint64_t expiry;
};


/**
 * @brief MPDU 수신정보 파라미터
 *
 * ProcessRxMPDUCallback() 콜백 API를 통해 라이브러리에서 어플리케이션으로 전달된다.
 */
struct WalMPDURxParams
{
  unsigned int if_idx; ///< MPDU가 수신된 인터페이스 식별번호
  WalTimeSlot timeslot; ///< MPDU가 수신된 TimeSlot
  WalChannelNumber chan_num; ///< MPDU가 수신된 채널번호
  WalPower rx_power; ///< MPDU 수신 파워
  WalRCPI rcpi; ///< MPDU RCPI
  WalDataRate datarate; ///< MPDU 수신 데이터레이트
};


/**
 * @brief IPv6 주소정보 세트
 */
struct WalIPv6AddressSet
{
  WalIPv6AddressNum num; ///< 주소의 개수
  struct WalIPv6Address addr[kWalIPv6AddressNum_Max]; ///< 주소정보(들)
};


/**
 * @brief TxProfile 정보
 */
struct WalTxProfile
{
  WalChannelNumber chan_num; ///< 채널번호
  WalPower power; ///< 송신파워
  WalDataRate datarate; ///< 송신 데이터레이트
  WalPriority priority; ///< 송신 우선순위
};


#endif //LIBWLANACCESS_WLANACCESS_API_PARAMS_H

