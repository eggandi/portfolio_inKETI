/**
 * @file
 * @brief wlanaccess 라이브러리의 Open API에서 사용되는 자료 유형을 정의한 헤더파일
 * @date 2019-05-08
 * @author gyun
 */

#ifndef LIBWLANACCESS_WLANACCESS_TYPES_H
#define LIBWLANACCESS_WLANACCESS_TYPES_H


// 시스템 헤더 파일
#include <stddef.h>
#include <stdint.h>

// 라이브러리 헤더 파일
#include "wlanaccess-defines.h"


/**
 * @brief API 수행 결과 코드
 */
enum eWalResultCode
{
  kWalResult_Success, ///< 성공
  kWalResult_NullParameters, ///< 널 파라미터
  kWalResult_TxQueueFull, ///< 송신큐가 가득 참
  kWalResult_NoIPv6Address, ///< IPv6 주소가 없음
  kWalResult_NoDefaultGateway, ///< Default gateway 정보가 존재하지 않음
  kWalResult_NoDNSIPv6Address, ///< DNS 서버 주소 정보가 존재하지 않음
  kWalResult_InvalidIfIndex, ///< 유효하지 않은 인터페이스 식별번호
  kWalResult_InvalidChannelNumber, ///< 유효하지 않은 채널번호
  kWalResult_InvalidTimeSlot, ///< 유효하지 않은 TimeSlot
  kWalResult_InvalidTxPower, ///< 유효하지 않은 송신파워
  kWalResult_InvalidDataRate, ///< 유효하지 않은 데이터레이트
  kWalResult_InvalidIPv6AddressPrefixLen, ///< 유효하지 않은 IPv6 주소 프리픽스 길이
  kWalResult_InvalidIPv6AddressStr, ///< 유효하지 않은 IPv6 주소 문자열
  kWalResult_InvalidIPv6Address, ///< 유효하지 않은 IPv6 주소
  kWalResult_InvalidMPDUSize, ///< 유효하지 않은 MPDU 길이
  kWalResult_InvalidMACAddress, ///< 유효하지 않은 MAC 주소
  kWalResult_InvalidMACAddressStr, ///< 유효하지 않은 MAC 주소 문자열
  kWalResult_InvalidQueueIndex, ///< 유효하지 않은 큐 식별번호
  kWalResult_InvalidPriority, ///< 유효하지 않은 우선순위
  kWalResult_PPSFail, ///< PPS 관련 실패
  kWalResult_NotRegistered, ///< 등록되어 있지 않음
  kWalResult_UnknownIPv6Address, ///< 알 수 없는(등록되지 않은) IPv6 주소
  kWalResult_DevSpecificError, ///< 디바이스(하드웨어) 관련 에러
  kWalResult_NoMemory, ///< 메모리 할당 실패
  kWalResult_Unspecified, ///< 지정되지 않은 에러
  kWalResult_Count, ///< 정의된 결과코드의 개수
};
typedef int WalResultCode;  ///< @ref eWalResultCode


/**
 * @brief 로그메시지 출력 레벨
 *
 * 높은 레벨은 낮은 레벨의 범위를 포함한다. \n
 * 즉, 로그변수가 높은 레벨로 설정되어 있으면, 그 하위레벨에 해당되는 로그는 함께 출력된다.
 */
enum eWalLogLevel
{
  kWalLogLevel_None = 0, ///< 아무 로그도 출력하지 않는다.
  kWalLogLevel_Err, ///< 에러 로그를 출력한다.
  kWalLogLevel_Init, ///< 초기화 로그를 출력한다.
  kWalLogLevel_Event, ///< 각종 이벤트(패킷 송수신, 설정 등)에 관련된 로그를 출력한다.
  kWalLogLevel_Dump, ///< 패킷 덤프 데이터 로그를 출력한다.
  kWalLogLevel_All, ///< 모든 로그를 출력한다.
  kWalLogLevel_Min = kWalLogLevel_None,
  kWalLogLevel_Max = kWalLogLevel_All,
};
typedef unsigned int WalLogLevel; ///< @ref eWalLogLevel


/**
 * @brief MPDU 크기
 */
enum eWalMPDUSize
{
  kWalMPDUSize_Min = (MAC_QOS_HLEN + MAC_CRC_LEN), ///< 최소길이
  kWalMPDUSize_Max = (MAC_QOS_HLEN + 2304 + MAC_CRC_LEN), ///< 최대길이
  kWalMPDUSize_MinWithoutCRC = (MAC_QOS_HLEN), ///< CRC 필드 제외 최소길이
  kWalMPDUSize_MaxWithoutCRC = (MAC_QOS_HLEN + 2304) ///< CRC 필드 제외 최대길이
};
typedef size_t WalMPDUSize; ///< @ref eWalMPDUSize


/**
 * @brief 채널번호
 */
enum eWalChannelNumber
{
  kWalChannelNumber_Min = 0, ///< 접속 가능한 최소 채널번호(5.00GHz 중심주파수). 장치가 실제 지원하는 범위는 다를 수 있다.
  kWalChannelNumber_Init = 190, ///< 초기화 시 접속 채널번호 (Condor5/Condor6 지원)
  kWalChannelNumber_CCH = 178, ///< CCH 채널번호
  kWalChannelNumber_Max = 200, ///< 접속 가능한 최대 채널번호(6.00GHz 중심주파수). 장치가 실제 지원하는 범위는 다를 수 있다.
  kWalChannelNumber_NA = 999, ///< 지정되지 않은, 알 수 없는, 임의의 채널

  // 한국 V2X 주파수 채널 대역
  kWalChannelNumber_KoreaV2XMin = 172, ///< 한국에서 송신 가능한 최소 V2X 채널번호
  kWalChannelNumber_KoreaV2XCCH = 180, ///< 한국의 CCH
  kWalChannelNumber_KoreaV2XMax = 184, ///< 한국에서 송신 가능한 최대 V2X 채널번호
};
typedef unsigned int WalChannelNumber; ///< @ref eWalChannelNumber


/**
 * @brief DataRate (500kbps 단위)
 */
enum eWalDataRate
{
  kWalDataRate_3Mbps = 6, // for 10MHz channel
  kWalDataRate_4p5Mbps = 9, // for 10MHz channel
  kWalDataRate_6Mbps = 12, // for 10MHz/20MHz channel
  kWalDataRate_9Mbps = 18, // for 10MHz/20MHz channel
  kWalDataRate_12Mbps = 24, // for 10MHz/20MHz channel
  kWalDataRate_18Mbps = 36, // for 10MHz/20MHz channel
  kWalDataRate_24Mbps = 48, // for 10MHz/20MHz channel
  kWalDataRate_27Mbps = 54, // for 10MHz channel
  kWalDataRate_36Mbps = 72, // for 20MHz channel
  kWalDataRate_48Mbps = 96, // for 20MHz channel
  kWalDataRate_54Mbps = 108, // for 20MHz channel
  kWalDataRate_10MHzMin = kWalDataRate_3Mbps,
  kWalDataRate_10MHzMax = kWalDataRate_27Mbps,
  kWalDataRate_20MHzMin = kWalDataRate_6Mbps,
  kWalDataRate_20MHzMax = kWalDataRate_54Mbps,
  kWalDataRate_Min = kWalDataRate_10MHzMin,
  kWalDataRate_Max = kWalDataRate_20MHzMax,
  kWalDataRate_TxDefault = kWalDataRate_6Mbps, ///< 기본 송신 데이터레이트
  kWalDataRate_NA = 999, ///< 지정되지 않은, 알 수 없는, 임의의 데이터레이트
};
typedef unsigned int WalDataRate;    ///< @ref eWalDataRate


/**
 * @brief Power(dBm 단위)
 */
enum eWalPower
{
  kWalPower_Min = -128, ///< 최소 파워
  kWalPower_Max = 127, ///< 최대 파워
  kWalPower_MaxInClassC = 20, ///< 클래스 C 최대 포트 출력
  kWalPower_MaxEIRPInClassC = 33, ///< 클래스 C 최대 EIRP(안테나 출력)
  kWalPower_TxDefault = kWalPower_MaxInClassC, ///< 기본 송신파워
  kWalPower_NA = 999 ///< 알 수 없는, 지정되지 않은 파워값
};
typedef int WalPower; ///< @ref eWalPower


/**
 * @brief Priority
 */
enum eWalPriority
{
  kWalPriority_Min = 0, ///< 우선순위 최소값
  kWalPriority_Max = 7, ///< 우선순위 최대값
  kWalPriority_WSA = kWalPriority_Max, ///< WSA 용 우선순위
  kWalPriority_NA = 999 ///< 알 수 없는, 지정되지 않은 우선순위
};
typedef unsigned int WalPriority; ///< @ref eWalPriority


/**
 * @brief RCPI(Received Channel Power Indicator)
 */
enum eWalRCPI
{
  kWalRCPI_Min = 0, ///< 최소 RCPI 값(=-110dBm)
  kWalRCPI_Max = 220, ///< 최대 RCPI 값(=0dBm)
  kWalRCPI_NA = 999 ///< 알 수 없는, 지정되지 않은 RCPI
};
typedef unsigned int WalRCPI; ///< @ref eWalRCPI;


/**
 * @brief TimeSlot
 */
enum eWalTimeSlot
{
  kWalTimeSlot_0 = 0, ///< TimeSlot0
  kWalTimeSlot_1 = 1, ///< TimeSlot1
  kWalTimeSlot_Any = 2, ///< TimeSlot0 또는 TimeSlot1
  kWalTimeSlot_Continuous = 2, ///< Continuous 접속 (TimeSlot0 & TimeSlot1)
  kWalTimeSlot_NA = 999, ///< 지정되지 않은 TimeSlot, 알 수 없는 TimeSlot
};
typedef unsigned int WalTimeSlot; ///< @ref eWalTimeSlot


/**
 * @brief MAC 주소 형식
 */
typedef uint8_t WalMACAddress[MAC_ALEN];


/**
 * @brief 송신큐 식별자
 */
enum eWalTxQueueIdx
{
  kWalTxQueueIdx_AC_VO = 1, ///< Voice
  kWalTxQueueIdx_AC_VI = 2, ///< Video
  kWalTxQueueIdx_AC_BE = 3, ///< Best effort
  kWalTxQueueIdx_AC_BK = 4, ///< Backtround
  kWalTxQueueIdx_All = 5, ///< 모든 Access Category 송신큐
  kWalTxQueueIdx_Min = kWalTxQueueIdx_AC_VO,
  kWalTxQueueIdx_Max = kWalTxQueueIdx_All,
};
typedef unsigned int WalTxQueueIdx; ///< @ref eWalTxQueueIdx


/**
 * @brief IPv6 주소 prefix 길이(비트수)
 */
enum eWalIPv6AddressPrefixLen
{
  kWalIPv6AddressPrefixLen_Min = 0,
  kWalIPv6AddressPrefixLen_Max = 128,
};
typedef size_t WalIPv6AddressPrefixLen; ///< @ref eWalIPv6AddressPrefixLen


/**
 * @brief IPv6 주소 유형
 */
enum eWalIPv6AddressType
{
  kWalIPv6AddressType_Global = (0x000),
  kWalIPv6AddressType_Loopback = (0x0010),
  kWalIPv6AddressType_LinkLocal = (0x0020),
  kWalIPv6AddressType_SiteLocal = (0x0040),
};
typedef unsigned int WalIPv6AddressType; ///< @ref eWalIPv6AddressType


/**
 * @brief IPv6 주소 개수
 */
enum eWalIPv6AddressNum
{
  kWalIPv6AddressNum_Min = 0, ///< 최소 개수
  kWalIPv6AddressNum_Max = 10, ///< 최대 개수 (임의로 결정)
};
typedef unsigned int WalIPv6AddressNum; ///< @ref eWalIPv6AddressNum


/**
 * @brief IPv6 주소 정보
 */
struct WalIPv6Address
{
  WalIPv6AddressType type; ///< 주소 유형
  uint8_t addr[IPv6_ALEN]; ///< 주소
  WalIPv6AddressPrefixLen prefix_len; ///< 프리픽스 길이
};


#endif //LIBWLANACCESS_WLANACCESS_TYPES_H
