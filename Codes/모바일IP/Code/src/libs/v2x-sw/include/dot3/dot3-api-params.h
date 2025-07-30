/**
 * @file
 * @brief dot3 라이브러리의 Open API 파라미터를 정의한 헤더파일
 * @date 2019-09-06
 * @author gyun
 */

#ifndef V2X_SW_DOT3_API_PARAMS_H
#define V2X_SW_DOT3_API_PARAMS_H


// 라이브러리 헤더 파일
#include "dot3-types.h"


/**
 * @brief MPDU 생성/파싱 정보를 포함하는 정보구조체(MAC 헤더 수납 정보)
 */
struct Dot3MACProcessParams
{
  uint8_t dst_mac_addr[MAC_ALEN]; ///< MAC 헤더 ADDR1 필드에 수납되는 목적지 MAC 주소
  uint8_t src_mac_addr[MAC_ALEN]; ///< MAC 헤더 ADDR2 필드에 수납되는 송신지(자신) MAC 주소
  Dot3Priority priority; ///< MAC 헤더 QoS Control 필드에 수납되는 메시지 우선 순위
};


/**
 * @brief WSM 생성/파싱 정보를 포함하는 정보구조체(WSM 헤더 수납 정보)
 */
struct Dot3WSMProcessParams
{
  Dot3ProtocolVersion version; ///< WSMP 헤더에 수납되는 프로토콜 버전 (생성 시에는 값을 입력할 필요 없음)
  Dot3PSID psid; ///< WSMP 헤더에 수납되는 PSID
  Dot3ChannelNumber chan_num; ///< WSMP 헤더 확장필드에 수납되는 채널번호 (NA 일 경우 수납 X)
  Dot3DataRate datarate; ///< WSMP 헤더 확장필드에 수납되는 데이터레이트 (NA 일 경우 수납 X)
  Dot3Power transmit_power; ///< WSMP 헤더 확장필드에 수납되는 전송파워 (NA 일 경우 수납 X)
};
/// WSM 생성을 위한 정보를 포함하는 정보구조체
#define Dot3WSMConstructParams Dot3WSMProcessParams
/// WSM 파싱 정보를 포함하는 정보구조체
#define Dot3WSMParseParams Dot3WSMProcessParams


/**
 * @brief WSM MPDU 생성/파싱 정보를 포함하는 정보구조체(MAC 헤더 및 WSM 헤더 수납 정보)
 */
struct Dot3MACAndWSMProcessParams
{
  struct Dot3MACProcessParams mac; ///< MAC 헤더 수납정보
  struct Dot3WSMProcessParams wsm; ///< WSM 헤더 수납정보
};
/// WSM MPDU 생성을 위한 정보를 포함하는 정보구조체
#define Dot3MACAndWSMConstructParams Dot3MACAndWSMProcessParams
/// WSM MPDU 파싱을 위한 정보를 포함하는 정보구조체
#define Dot3MACAndWSMParseParams Dot3MACAndWSMProcessParams


/**
 * @brief WSA 생성(Dot3_ConstructWSA()) 파라미터
 */
struct Dot3ConstructWSAParams
{
  struct {
    bool wra; ///< WRA 파라미터 존재 여부
  } present;
  struct Dot3WSAHdr hdr; ///< WSA 헤더 정보
  struct Dot3WRA wra; ///< WRA 정보
};


/**
 * @brief WSA 파싱(Dot3_ParseWSA()) 파라미터
 */
struct Dot3ParseWSAParams
{
  struct {
    bool wra; ///< WRA 필드 존재 여부
  } present;
  struct Dot3WSAHdr hdr; ///< WSA 헤더 정보
  Dot3WSINum wsi_num; ///< WSA 내 수납된 Service Info 개수
  struct Dot3WSI wsis[kDot3WSINum_Max]; ///< WSA 내 수납된 Service Info 들
  Dot3WCINum wci_num; ///< WSA 내 수납된 Channel Info 개수
  struct Dot3WCI wcis[kDot3WCINum_Max]; ///< WSA 내 수납된 Channel Info 들
  struct Dot3WRA wra; ///< WSA 내 수납된 WRA
};

#endif //V2X_SW_DOT3_API_PARAMS_H
