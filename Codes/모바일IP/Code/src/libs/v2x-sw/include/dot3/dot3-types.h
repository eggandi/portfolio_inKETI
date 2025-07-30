/**
 * @file
 * @brief dot3 라이브러리의 Open API에서 사용되는 유형들을 정의한 헤더파일
 * @date 2019-06-04
 * @author gyun
 */

#ifndef V2X_LIBDOT3_DOT3_TYPES_H
#define V2X_LIBDOT3_DOT3_TYPES_H


// 시스템 헤더 파일
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// 라이브러리 헤더 파일
#include "dot3-defines.h"


/**
 * @brief Open API 처리 결과 코드
 */
enum eDot3ResultCode
{
  kDot3Result_Success, ///< 성공
  kDot3Result_NullParameters,  //< 널 파라미터가 전달된 경우
  kDot3Result_InvalidWSMPayloadSize, ///< 유효하지 않은 WSM 페이로드 길이
  kDot3Result_InvalidPSID, ///< 유효하지 않은 PSID
  kDot3Result_InvalidPSIDFormat,  ///< 유효하지 않은 PSID 형식
  kDot3Result_InvalidPriority, ///< 유효하지 않은 priority 값
  kDot3Result_InvalidChannelNumber, ///< 유효하지 않은 채널번호 값
  kDot3Result_InvalidDataRate, ///< 유효하지 않은 DataRate 값
  kDot3Result_InvalidPower, ///< 유효하지 않은 파워 값
  kDot3Result_InvalidOperatingClass, ///< 유효하지 않은 operating class
  kDot3Result_InvalidWSARCPIThreshold, ///< 유효하지 않은 WSA RCPI threshold
  kDot3Result_InvalidWSACountThreshold, ///< 유효하지 않은 WSA Count Threshold
  kDot3Result_InvalidWSACountThresholdInterval, ///< 유효하지 않은 WSA Count Threshold Interval
  kDot3Result_InvalidWSAType, ///< 유효하지 않은 WSA type
  kDot3Result_InvalidRCPI, ///< 유효하지 않은 RCPI
  kDot3Result_InvalidUASManagementInterval, ///< 유효하지 않은 UAS 관리 주기
  kDot3Result_InvalidChannelIndex, ///< 유효하지 않은 Channel Index
  kDot3Result_InvalidWSMPNHeaderSubType, ///< 유효하지 않은 subtype (WSMP-N-Header)
  kDot3Result_InvalidWSMPNHeaderExtensionID, ///< 유효하지 않은 ExtensionId (WSMP-N-Header)
  kDot3Result_InvalidWSMPNHeaderTPID, ///< 유효하지 않은 TPID (WSMP-N-Header)
  kDot3Result_InvalidWSMPNHeaderWSMPVersion, ///< 유효하지 않은 WSMP 버전 (WSMP-N-Header)
  kDot3Result_InvalidLowerLayerProtocolVersion, ///< 유효하지 않은 하위계층 프로토콜 버전
  kDot3Result_InvalidLowerLayerFrameType, ///< 유효하지 않은 하위계층 프레임 유형
  kDot3Result_InvalidWSAIdentifier, ///< 유효하지 않은 WSA identifier
  kDot3Result_InvalidWSAContentCount, ///< 유효하지 않은 WSA content count
  kDot3Result_InvalidChannelAccess, ///< 유효하지 않은 Channel access
  kDot3Result_InvalidAdvertiserIDLen, ///< 유효하지 않은 Advertiser Id 길이
  kDot3Result_InvalidPSCLen, ///< 유효하지 않은 PSC 길이
  kDot3Result_InvalidLatitude, ///< 유효하지 않은 위도 값
  kDot3Result_InvalidLongitude, ///< 유효하지 않은 경도 값
  kDot3Result_InvalidElevation, ///< 유효하지 않은 고도 값
  kDot3Result_InvalidWSAHdrExtensionID, ///< 유효하지 않은 WSA 확장필드 ID
  kDot3Result_InvalidWCIExtensionID, ///< 유효하지 않은 WSA Channel Info 확장필드 ID
  kDot3Result_InvalidWSIExtensionID, ///< 유효하지 않은 WSA Service Info 확장필드 ID
  kDot3Result_InvalidWSAMessage, ///< 유효하지 않은 WSA 메시지 유형
  kDot3Result_InvalidWSAVersion, ///< 유효하지 않은 WSA 버전
  kDot3Result_InvalidIPv6PrefixLen, ///< 유효하지 않은 IPv6 prefix length
  kDot3Result_InvalidWRARouterLifetime, ///< 유효하지 않은 WRA Router lifetime
  kDot3Result_InvalidWSMMaxLength, ///< 유효하지 않은 WSM max length
  kDot3Result_InvalidRepeatRate, ///< 유효하지 않은 RepeatRate
  kDot3Result_InvalidWSMSize, ///< 유효하지 않은 WSM 길이
  kDot3Result_InvalidMPDUSize, ///< 유효하지 않은 MPDU 길이
  kDot3Result_InvalidAIFSN, ///< 유효하지 않은 AIFSN
  kDot3Result_InvalidECWMin, ///< 유효하지 않은 ECWmin
  kDot3Result_InvalidECWMax, ///< 유효하지 않은 ECWmax
  kDot3Result_Asn1Encode, ///< asn.1 인코딩 실패
  kDot3Result_Asn1Decode, ///< asn.1 디코딩 실패
  kDot3Result_Asn1AbnormalOp, ///< 비정상 asn.1 동작
  kDot3Result_NotWildcardBSSID, ///< BSSID가 wildcard BSSID가 아님.
  kDot3Result_NotSupportedEtherType, ///< 지원되지 않는 EtherType
  kDot3Result_WSRTableFull, ///< WSR 테이블이 가득 참.
  kDot3Result_DuplicatedWSR, ///< 동일한 PSID를 갖는 WSR이 존재함.
  kDot3Result_NoSuchWSR, ///< 해당 WSR이 테이블에 존재하지 않음.
  kDot3Result_PSRTableFull, ///< PSR 테이블이 가득 참.
  kDot3Result_DuplicatedPSR, ///< 동일한 PSID를 갖는 PSR이 존재함.
  kDot3Result_NoSuchPSR, ///< 해당 PSR이 테이블에 존재하지 않음.
  kDot3Result_PCITableFull, ///< PCI 테이블이 가득 참.
  kDot3Result_NoSuchPCI, ///< 해당 PCI 가 테이블에 존재하지 않음
  kDot3Result_USRTableFull, ///< USR 테이블이 가득 참.
  kDot3Result_DuplicatedUSR, ///< 동일한 PSID를 갖는 USR이 존재함.
  kDot3Result_NoSuchUSR, ///< 해당 USR이 테이블에 존재하지 않음.
  kDot3Result_UASTableFull, ///< UAS 테이블이 가득 참.
  kDot3Result_AlreadyRunning, ///< 이미 동작 중임.
  kDot3Result_NoRelatedChannelInfo, ///< PSR에 연관된 Channel info가 없음.
  kDot3Result_NoMemory, ///< 메모리 부족
  kDot3Result_SystemCallFailed, ///< 시스템콜 호출 실패
  kDot3Result_Count, ///< 정의된 결과코드의 개수
};
typedef int Dot3ResultCode; ///< @ref eDot3ResultCode


/**
 * @brief 로그메시지 출력 레벨
 *
 * 높은 레벨은 낮은 레벨의 범위를 포함한다. \n
 * 즉, 로그변수가 높은 레벨로 설정되어 있으면, 그 하위레벨에 해당되는 로그는 함께 출력된다.
 */
enum eDot3LogLevel
{
  kDot3LogLevel_None = 0, ///< 아무 로그도 출력하지 않는다.
  kDot3LogLevel_Err, ///< 에러 로그
  kDot3LogLevel_Init, ///< 초기화 절차에 관련된 로그
  kDot3LogLevel_Event, ///< 각종 이벤트(패킷 송수신 포함)에 관련된 로그
  kDot3LogLevel_Dump, ///< 상세내용 로그(송수신 패킷 덤프 데이터 등)
  kDot3LogLevel_All, ///< 모든 로그
  kDot3LogLevel_Min = kDot3LogLevel_None,
  kDot3LogLevel_Max = kDot3LogLevel_All
};
typedef unsigned int Dot3LogLevel; ///< @ref eDot3LogLevel


/**
 * @brief WSM 헤더 길이
 */
enum eDot3WSMHdrSize
{
  kDot3WSMHdrSize_Min = 4, ///< 최소길이: 2바이트 기본 필드, 1바이트 PSID 필드, 1바이트 Length 필드, 확장필드 제외
  kDot3WSMHdrSize_Max = 18, ///< 최대길이: 2바이트 기본 필드, 4바이트 PSID 필드, 2바이트 Length 필드, {3개 확장필드=10바이트} 모두 포함
};
typedef size_t Dot3WSMHdrSize; ///< @ref eDot3WSMHdrSize


/**
 * @brief WSM body에 수납되는 페이로드 길이
 */
enum eDot3WSMPayloadSize
{
  kDot3WSMPayloadSize_Min = 0,
  kDot3WSMPayloadSize_Max = (2302 - kDot3WSMHdrSize_Max), ///< 최대길이 (=최대LSDU 길이 - 최대WSM헤더길이)
};
typedef size_t Dot3WSMPayloadSize; ///< @ref eDot3WSMPayloadSize


/**
 * @brief WSM 길이(헤더 포함)
 */
enum eDot3WSMSize
{
  kDot3WSMSize_Min = kDot3WSMHdrSize_Min, ///< 최소길이 (= 최소WSM헤더길이)
  kDot3WSMSize_DefaultMaxInMIB = 1400, ///< MIB에 설정되는 기본 최대길이 (per 802.11)
  kDot3WSMSize_Max = (2304 - 2), ///< 최대길이 (= 최대MSDU길이 - LLC헤더길이) (per 802.11)
};
typedef size_t Dot3WSMSize; ///< @ref eDot3WSMSize;


/**
 * @brief MPDU 길이
 */
enum eDot3MPDUSize
{
  kDot3MPDUSize_Min = (26 + 2 + kDot3WSMSize_Min), ///< 최소길이 (= MAC헤더길이 + LLC헤더길이 + 최소WSM길이)
  kDot3MPDUSize_Max = (26 + 2304), ///< 최대길이 (= MAC헤더길이 + 최대MSDU길이, CRC는 제외) (per 802.11)
};
typedef size_t Dot3MPDUSize; ///< @ref eDot3MPDUSize;


/**
 * @brief PSID (Provider Service IDentifier)
 */
enum eDot3PSID
{
  kDot3PSID_Min = 0, ///< PSID 최소값
  kDot3PSID_Max = 0x1020407F/*270549119*/, ///< PSID 최대값
  kDot3PSID_IPv6Routing = 0x1020407E/*270549118*/, ///< IPv6 Routing 서비스를 나타내는 PSID
  kDot3PSID_WSA = 0x87/*135*/, ///< WSA임을 나타내는 PSID
  kDot3PSID_NA = 999999999/*0x3B9AC9FF*/, ///< 지정되지 않은 PSID, 알 수 없는 PSID
};
typedef unsigned int Dot3PSID; ///< @ref eDot3PSID


/**
 * @brief 프로토콜 버전
 */
enum eDot3ProtocolVersion
{
  kDot3ProtocolVersion_Current = 3,
};
typedef unsigned int Dot3ProtocolVersion; ///< @ref eDot3ProtocolVersion


/**
 * @brief PSR(Provider Service Request) 개수
 */
enum eDot3PSRNum
{
  kDot3PSRNum_Min = 0,
  kDot3PSRNum_Max = 128, ///< 저장 가능한 PSR 최대 개수(per 1609.3-2016)
};
typedef unsigned int Dot3PSRNum; ///< @ref eDot3PSRNum


/**
 * @brief PCI(Provider Channel Info) 개수
 */
enum eDot3PCINum
{
  kDot3PCINum_Min = 0,
  kDot3PCINum_Max = 128, ///< 저장 가능한 PCI 최대 개수(per 1609.3-2016)
};
typedef unsigned int Dot3PCINum; ///< @ref eDot3PCINum


/**
 * @brief USR(User Service Request) 개수
 */
enum eDot3USRNum
{
  kDot3USRNum_Min = 0,
  kDot3USRNum_Max = 4096, ///< 저장 가능한 USR 최대 개수
};
typedef unsigned int Dot3USRNum; ///< @ref eDot3USRNum


/**
 * @brief WSR(WSM Service Request) 개수
 */
enum eDot3WSRNum
{
  kDot3WSRNum_Min = 0,
  kDot3WSRNum_Max = 65536, ///< 저장 가능한 WSR 최대 개수(per 1609.3-2016)
};
typedef unsigned int Dot3WSRNum; ///< @ref eDot3WSRNum


/**
 * @brief UAS(User Available Service) 개수
 */
enum eDot3UASNum
{
  kDot3UASNum_Min = 0,
  kDot3UASNum_Max = 4096, ///< 저장 가능한 UAS 최대 개수(per 1609.3-2016)
};
typedef unsigned int Dot3UASNum; ///< @ref eDot3UASNum


/**
 * @brief WSI(WSA Service info) 관련 수
 */
enum eDot3WSINum
{
  kDot3WSINum_Min = 0,
  kDot3WSINum_Max = 31, ///< 처리 가능한 WSA 내 Service Info 최대 개수(per 1609.3-2016)
                        ///< asn.1에는 제한이 없지만, 표준문서에 31로 정의되어 있다.
};
typedef unsigned int Dot3WSINum; ///< @ref eDot3WSINum


/**
 * @brief WCI(WSA channel info) 관련 수
 */
enum eDot3WCINum
{
  kDot3WCINum_Min = 0,
  kDot3WCINum_Max = 31, ///< 처리 가능한 WSA 내 Channel Info 최대 개수(per 1609.3-2016)
                        ///< asn.1에는 제한이 없지만, 표준문서에 31로 정의되어 있다.
};
typedef unsigned int Dot3WCINum; ///< @ref eDot3WCINum


/**
 * @brief TimeSlot
 */
enum eDot3TimeSlot
{
  kDot3TimeSlot_0 = 0, ///< TimeSlot0
  kDot3TimeSlot_1 = 1, ///< TimeSlot1
  kDot3TimeSlot_Any = 2, ///< TimeSlot0 또는 TimeSlot1
  kDot3TimeSlot_Continuous = 2, ///< Continuous 접속 (TimeSlot0 & TimeSlot1)
  kDot3TimeSlot_NA = 999, ///< 지정되지 않은 TimeSlot, 알 수 없는 TimeSlot
};
typedef unsigned int Dot3TimeSlot; ///< @ref eDot3TimeSlot


/**
 * @brief DataRate (500kbps 단위)
 */
enum eDot3DataRate
{
  kDot3DataRate_3Mbps = 6, // for 10MHz channel
  kDot3DataRate_4p5Mbps = 9, // for 10MHz channel
  kDot3DataRate_6Mbps = 12, // for 10MHz/20MHz channel
  kDot3DataRate_9Mbps = 18, // for 10MHz/20MHz channel
  kDot3DataRate_12Mbps = 24, // for 10MHz/20MHz channel
  kDot3DataRate_18Mbps = 36, // for 10MHz/20MHz channel
  kDot3DataRate_24Mbps = 48, // for 10MHz/20MHz channel
  kDot3DataRate_27Mbps = 54, // for 10MHz channel
  kDot3DataRate_36Mbps = 72, // for 20MHz channel
  kDot3DataRate_48Mbps = 96, // for 20MHz channel
  kDot3DataRate_54Mbps = 108, // for 20MHz channel
  kDot3DataRate_10MHzMin = kDot3DataRate_3Mbps,
  kDot3DataRate_10MHzMax = kDot3DataRate_27Mbps,
  kDot3DataRate_20MHzMin = kDot3DataRate_6Mbps,
  kDot3DataRate_20MHzMax = kDot3DataRate_54Mbps,
  kDot3DataRate_Min = kDot3DataRate_10MHzMin,
  kDot3DataRate_Max = kDot3DataRate_20MHzMax,
  kDot3DataRate_TxDefault = kDot3DataRate_6Mbps, ///< 기본 송신 데이터레이트
  kDot3DataRate_NA = 999, ///< 지정되지 않은, 알 수 없는, 임의의 데이터레이트
};
typedef unsigned int Dot3DataRate; ///< @ref eDot3DataRate


/**
 * @brief Power(dBm 단위)
 */
enum eDot3Power
{
  kDot3Power_Min = -128, ///< 최소 파워
  kDot3Power_Max = 127, ///< 최대 파워
  kDot3Power_MaxInClassC = 20, ///< 클래스 C 최대 포트 출력
  kDot3Power_MaxEIRPInClassC = 33, ///< 클래스 C 최대 EIRP(안테나 출력)
  kDot3Power_TxDefault = kDot3Power_MaxInClassC, ///< 기본 송신파워
  kDot3Power_NA = 999 ///< 알 수 없는, 지정되지 않은 파워값
};
typedef int Dot3Power; ///< @ref eDot3Power


/**
 * @brief User Priority
 */
enum eDot3Priority
{
  kDot3Priority_Min = 0, ///< 우선순위 최소값
  kDot3Priority_Max = 7, ///< 우선순위 최대값
  kDot3Priority_WSA = kDot3Priority_Max, ///< WSA 용 우선순위
  kDot3Priority_NA = 999 ///< 알 수 없는, 지정되지 않은 우선순위
};
typedef unsigned int Dot3Priority; ///<  @ref eDot3Priority


/**
 * @brief 채널번호
 */
enum eDot3ChannelNumber
{
  kDot3ChannelNumber_Min = 0, ///< 접속 가능한 최소 채널번호(=5.00GHz 중심주파수)
  kDot3ChannelNumber_CCH = 178, ///< CCH 채널번호
  kDot3ChannelNumber_Max = 200, ///< 접속 가능한 최대 채널번호(=6.00GHz 중심주파수)
  kDot3ChannelNumber_NA = 999, ///< 지정되지 않은, 알 수 없는, 임의의 채널

  // 한국 V2X 주파수 채널 대역
  kDot3ChannelNumber_KoreaV2XMin = 172,
  kDot3ChannelNumber_KoreaV2XCCH = 180,
  kDot3ChannelNumber_KoreaV2XMax = 184,
};
typedef unsigned int Dot3ChannelNumber; ///< @ref eDot3ChannelNumber


/**
 * @brief Operating class
 */
enum eDot3OperatingClass
{
  kDot3OperatingClass_Min = 0,
  kDot3OperatingClass_5G_10MHz = 17, ///< 5GHz 대역의 10MHz 채널 Operating class(미국 기준)
  kDot3OperatingClass_5G_20MHz = 18, ///< 5GHz 대역의 20MHz 채널 Operating class(미국 기준)
  kDot3OperatingClass_Max = 255,
  kDot3OperatingClass_Default = kDot3OperatingClass_5G_10MHz,
};
typedef unsigned int Dot3OperatingClass; ///< @ref eDot3OperatingClass


/**
 * @brief RCPI(Received Channel Power Indicator)
 */
enum eDot3RCPI
{
  kDot3RCPI_Min = 0, ///< 최소 RCPI 값(=-110dBm)
  kDot3RCPI_Max = 220, ///< 최대 RCPI 값(=0dBm)
  kDot3RCPI_Unknown = 999 ///< 알 수 없는 RCPI
};
typedef unsigned int Dot3RCPI; ///< @ref eDot3RCPI


/**
 * @brief WSA 유형 (dot3WsaType)
 */
enum eDot3WSAType
{
  kDot3WSAType_Unsecured = 1, ///< Unsecured WSA
  kDot3WSAType_Secured = 2, ///< Secured WSA
  kDot3WSAType_Min = kDot3WSAType_Unsecured,
  kDot3WSAType_Max = kDot3WSAType_Secured,
};
typedef unsigned int Dot3WSAType; ///< @ref eDot3WSAType


/**
 * @brief WSA 식별번호 (Wsa Identifier)
 */
enum eDot3WSAIdentifier
{
  kDot3WSAIdentifier_Min = 0,
  kDot3WSAIdentifier_Max = 15,
};
typedef unsigned int Dot3WSAIdentifier; ///< @ref eDot3WSAIdentifier


/**
 * @brief WSA content count
 *
 * 동일 WSA identifier 에 대해 WSA 가 변경될 때마다 1씩 증가하며, 최대값에 도달하면 최소값으로 wrap-around 된다.
 */
enum eDot3WSAContentCount
{
  kDot3WSAContentCount_Min = 0,
  kDot3WSAContentCount_Max = 15,
};
typedef unsigned int Dot3WSAContentCount; ///< @ref eDot3WSAContentCount


/**
 * @brief WSA Repeat Rate (5초당 전송되는 WSA 의 개수)
 */
enum eDot3WSARepeatRate
{
  kDot3WSARepeatRate_Min = 0, ///< 1회만 전송(단발성)
  kDot3WSARepeatRate_Default = 50, ///< 기본값(5초당 50개 = 100msec 주기)
  kDot3WSARepeatRate_Max = 255,
};
typedef unsigned int Dot3WSARepeatRate; ///< @ref eDot3WSARepeatRate


/**
 * @brief 채널 접속 유형 (dot3ProviderChannelAccess)
 */
enum eDot3ProviderChannelAccess
{
  kDot3ProviderChannelAccess_Continuous = 0, ///< TimeSlot0 과 TimeSlot1 에서 서비스를 제공 (=continuous)
  kDot3ProviderChannelAccess_AlternatingTimeSlot1Only = 1, ///< TimeSlot1 에서 서비스 제공 (=alternating)
  kDot3ProviderChannelAccess_AlternatingTimeSlot0Only = 2, ///< TimeSlot0 에서 서비스 제공 (=alternating)
  kDot3ProviderChannelAccess_Any = 3, ///< 어떤 형태든 가능
  kDot3ProviderChannelAccess_Default = kDot3ProviderChannelAccess_AlternatingTimeSlot1Only,
  kDot3ProviderChannelAccess_Min = kDot3ProviderChannelAccess_Continuous,
  kDot3ProviderChannelAccess_Max = kDot3ProviderChannelAccess_Any
};
typedef unsigned int Dot3ProviderChannelAccess; ///< @ref eDot3ProviderChannelAccess


/**
 * @brief IPv6 주소 형식
 */
typedef uint8_t Dot3IPv6Address[IPv6_ALEN];


/**
 * @brief MAC 주소 형식
 */
typedef uint8_t Dot3MACAddress[MAC_ALEN];


/**
 * @brief WSA count threshold
 *
 * 유효한 WSA로 판단되기 위한 수신 개수.
 * Dot3WSACountThresholdInterval 동안 이 값 이상의 회수만큼 수신된 WSA만 유효한 것으로 판단한다.
 */
enum eDot3WSACountThreshold
{
  kDot3WSACountThreshold_Min = 1,
  kDot3WSACountThreshold_Max = 255,
};
typedef unsigned int Dot3WSACountThreshold; ///< @ref eDot3WSACountThreshold


/**
 * @brief WSA count threshold interval
 *
 * wsa_cnt_threshold 를 카운트하기 위한 100ms 단위 인터벌.
 * WSA count threshold interval 기간 동안 WSA count threshold 이상 수신된 WSA만 유효한 것으로 판단한다.
 */
enum eDot3WSACountThresholdInterval
{
  kDot3WSACountThresholdInterval_Min = 1,
  kDot3WSACountThresholdInterval_Max = 255
};
typedef unsigned int Dot3WSACountThresholdInterval; ///< @ref eDot3WSACountThresholdInterval


/**
 * @brief Latitude (위도, 0.1 마이크로도 단위)
 */
enum eDot3Latitude
{
  kDot3Latitude_Min = -900000000,
  kDot3Latitude_Max = 900000000,
  kDot3Latitude_Unavailable = 900000001
};
typedef int32_t Dot3Latitude; ///< @ref eDot3Latitude


/**
 * @brief Longitude (경도, 0.1 마이크로도 단위)
 */
enum eDot3Longitude
{
  kDot3Longitude_Min = -1799999999,
  kDot3Longitude_Max = 1800000000,
  kDot3Longitude_Unavailable = 1800000001
};
typedef int32_t Dot3Longitude; ///< @ref eDot3Longitude


/**
 * @brief Elevation (고도, 10cm 단위)
 */
enum eDot3Elevation
{
  kDot3Elevation_Min = -4095/*0xF001*/, ///< 최소 고도(-409.5m)
  kDot3Elevation_Max = 61439/*0xEFFF*/, ///< 최대 고도(6143.9m)
  kDot3Elevation_Unavailable = -4096/*0xF000*/
};
typedef int32_t Dot3Elevation; ///< @ref eDot3Elevation


/**
 * @brief WSA Advertiser id 길이
 */
enum eDot3WSAAdvertiserIDLen
{
  kDot3WSAAdvertiserIDLen_Min = 1,
  kDot3WSAAdvertiserIDLen_Max = 32,
};
typedef size_t Dot3WSAAdvertiserIDLen; ///< @ref eDot3WSAAdvertiserIDLen


/**
 * @brief 서비스광고메시지 유형
 *
 * 1609.3 표준에서는 kDot3SrvAdvMessageType_WSA만이 사용된다.
 */
enum eDot3SrvAdvMessageType
{
  kDot3SrvAdvMessageType_WSA = 0, ///< WSA message in IEEE 1609.3, SAM in ISO 24102-5
  kDot3SrvAdvMessageType_SAR = 1, ///< SAR message used in ISO 24102-5
  kDot3SrvAdvMessageType_Min = kDot3SrvAdvMessageType_WSA,
  kDot3SrvAdvMessageType_Max = kDot3SrvAdvMessageType_SAR
};
typedef unsigned int Dot3SrvAdvMessageType; ///< @ref eDot3SrvAdvMessageType


/**
 * @brief WSA 헤더에 수납되는 버전 값
 */
enum eDot3WSAVersion
{
  kDot3WSAVersion_Min = 0,
  kDot3WSAVersion_Max = 15,
  kDot3WSAVersion_Current = 3, ///< 현 버전의 표준에 정의된 WSA 버전
};
typedef unsigned int Dot3WSAVersion; ///< @ref eDot3WSAVersion


/**
 * @brief WSA 헤더에 수납되는 Change count 값
 */
enum eDot3WSAChangeCount
{
  kDot3WSAChangeCount_Min = 0,
  kDot3WSAChangeCount_Max = 15,
};
typedef unsigned int Dot3WSAChangeCount; ///< @ref eDot3WSAChangeCount


/**
 * @brief WSA 내 service info 필드의 Channel index 값. 1부터 시작한다.
 */
enum eDot3WSAChannelIndex
{
  kDot3WSAChannelIndex_Min = 1,
  kDot3WSAChannelIndex_Max = kDot3WCINum_Max
};
typedef unsigned int Dot3WSAChannelIndex; ///< @ref eDot3WSAChannelIndex


/**
 * @brief Provider Service Context 길이
 */
enum eDot3PSCLen
{
  kDot3PSCLen_Min = 0,
  kDot3PSCLen_Max = 31,
};
typedef size_t Dot3PSCLen; ///< @ref eDot3PSCLen


/**
 * @brief WRA에 수납되는 Router Life Time 값 (초 단위)
 */
enum eDot3WRARouterLifetime
{
  kDot3WRARouterLifetime_Min = 1,
  kDot3WRARouterLifetime_Max = 65535
};
typedef unsigned int Dot3WRARouterLifetime; ///< @ref eDot3WRARouterLifetime


/**
 * @brief IPv6 프리픽스 길이를 나타내는 값
 */
enum eDot3IPv6PrefixLen
{
  kDot3IPv6PrefixLen_Min = 1,
  kDot3IPv6PrefixLen_Max = 128
};
typedef unsigned int Dot3IPv6PrefixLen; ///< @ref eDot3IPv6PrefixLen


/**
 * @brief ACI(Access Category Index) 값 (2비트 길이를 가진다)
 */
enum eDot3ACI
{
  kDot3ACI_BE = 0, ///< AC_BE에 대한 ACI(per IEEE 802.11-2012 p.569)
  kDot3ACI_BK = 1, ///< AC_BK에 대한 ACI(per IEEE 802.11-2012 p.569)
  kDot3ACI_VI = 2, ///< AC_VI에 대한 ACI(per IEEE 802.11-2012 p.569)
  kDot3ACI_VO = 3, ///< AC_VO에 대한 ACI(per IEEE 802.11-2012 p.569)
  kDot3ACI_Min = kDot3ACI_BE,
  kDot3ACI_Max = kDot3ACI_VO
};
typedef unsigned int Dot3ACI; ///< @ref eDot3ACI


/**
 * @brief ACM(Admission Control Mandatory) 여부
 */
enum eDot3ACM
{
  kDot3ACM_Default = 0, ///< 기본값
};
typedef bool Dot3ACM; ///< @ref eDot3ACM


/**
 * @brief AIFSN 값 (4비트 길이를 가진다)
 */
enum eDot3AIFSN
{
  kDot3AIFSN_AC_BE = 6, ///< AC_BE의 기본 AIFSN 값(per IEEE 802.11-2012 p.569)
  kDot3AIFSN_AC_BK = 9, ///< AC_BK의 기본 AIFSN 값(per IEEE 802.11-2012 p.569)
  kDot3AIFSN_AC_VI = 3, ///< AC_VI의 기본 AIFSN 값(per IEEE 802.11-2012 p.569)
  kDot3AIFSN_AC_VO = 2, ///< AC_VO의 기본 AIFSN 값(per IEEE 802.11-2012 p.569)
  kDot3AIFSN_Min = 0,
  kDot3AIFSN_Max = 15,
};
typedef unsigned int Dot3AIFSN; ///< @ref eDot3AIFSN


/**
 * @brief ECWmin, ECWmax 값 (4비트 길이를 가진다)
 */
enum eDot3ECW
{
  kDot3ECW_Min = 0,
  kDot3ECW_AC_BE_ECWMin = 4, ///< AC_BE의 기본 ECWmin 값(per IEEE 802.11-2012 p.569)
  kDot3ECW_AC_BE_ECWMax = 10, ///< AC_BE의 기본 ECWmax 값(per IEEE 802.11-2012 p.569)
  kDot3ECW_AC_BK_ECWMin = 4, ///< AC_BK의 기본 ECWmin 값(per IEEE 802.11-2012 p.569)
  kDot3ECW_AC_BK_ECWMax = 10, ///< AC_BK의 기본 ECWmax 값(per IEEE 802.11-2012 p.569)
  kDot3ECW_AC_VI_ECWMin = 3, ///< AC_VI의 기본 ECWmin 값(per IEEE 802.11-2012 p.569)
  kDot3ECW_AC_VI_ECWMax = 4, ///< AC_VI의 기본 ECWmax 값(per IEEE 802.11-2012 p.569)
  kDot3ECW_AC_VO_ECWMin = 2, ///< AC_VO의 기본 ECWmin 값(per IEEE 802.11-2012 p.569)
  kDot3ECW_AC_VO_ECWMax = 3, ///< AC_VO의 기본 ECWmax 값(per IEEE 802.11-2012 p.569)
  kDot3ECW_Max = 15,
};
typedef unsigned int Dot3ECW; ///< @ref eDot3ECW


/**
 * @brief TXOPLimit 값 (16비트 길이를 가진다)
 */
enum eDot3TXOPLimit
{
  kDot3TXOPLimit_AC_BE = 0, ///< AC_BE의 기본 TXOPLimit 값(per IEEE 802.11-2012 p.569)
  kDot3TXOPLimit_AC_BK = 0, ///< AC_BK의 기본 TXOPLimit 값(per IEEE 802.11-2012 p.569)
  kDot3TXOPLimit_AC_VI = 0, ///< AC_VI의 기본 TXOPLimit 값(per IEEE 802.11-2012 p.569)
  kDot3TXOPLimit_AC_VO = 0, ///< AC_VO의 기본 TXOPLimit 값(per IEEE 802.11-2012 p.569)
  kDot3TXOPLimit_Min = 0,
  kDot3TXOPLimit_Max = UINT16_MAX,
};
typedef uint16_t Dot3TXOPLimit; ///< @ref eDot3TXOPLimit


/**
 * @brief UAS(User Available Service) 정보의 수명(초 단위)
 */
enum eDot3UASLifetime
{
  kDot3UASLifetime_Min = 0,
  kDot3UASLifetime_Infinite = 0, ///< 영속적임
  kDot3UASLifetime_Default = 60, ///< 기본 수명
  kDot3UASLifetime_Max = 3600,
};
typedef unsigned int Dot3UASLifetime; ///< @ref eDot3UASLifetime


/**
 * @brief UAS(User Available Service) 관리 주기 (100msec 단위)
 */
enum eDot3UASManagementInterval
{
  kDot3UASManagementInterval_Min = 1, ///< 100msec
  kDot3UASManagementInterval_Default = 10, ///< 1sec
  kDot3UASManagementInterval_Max = 300 ///< 30sec
};
typedef unsigned int Dot3UASManagementInterval; ///< @ref eDot3UASManagementInterval


/**
 * @brief PSC(Provider Service Context) 형식
 */
struct Dot3PSC
{
  Dot3PSCLen len; ///< PSC 길이
  char psc[kDot3PSCLen_Max + 1]; ///< PSC
};


/**
 * @brief WSA Advertiser id
 */
struct Dot3WSAAdvertiserID
{
  Dot3WSAAdvertiserIDLen len; ///< AdvertiserId 길이
  char id[kDot3WSAAdvertiserIDLen_Max + 1]; ///< AdvertiserId
};


/**
 * @brief WSA 2DLocation (WSA 헤더 확장필드에 수납되는 2DLocation)
 */
struct Dot3WSATwoDLocation
{
  Dot3Latitude latitude; ///< 위도
  Dot3Longitude longitude; ///< 경도
};


/**
 * @brief WSA 3DLocation (WSA 헤더 확장필드에 수납되는 3DLocation)
 */
struct Dot3WSAThreeDLocation
{
  Dot3Latitude latitude; ///< 위도
  Dot3Longitude longitude; ///< 경도
  Dot3Elevation elevation; ///< 고도
};


/**
 * @brief EDCA Parameter Record (각 AC별 EDCA 파라미터 정보 구조체)
 */
struct Dot3EDCAParameterRecord
{
  Dot3ACI aci; ///< ACI
  Dot3ACM acm; ///< Admission Control이 필수로 수행되어야 하는지 여부
  Dot3AIFSN aifsn; ///< AIFSN
  Dot3ECW ecwmin; ///< ECWmin
  Dot3ECW ecwmax; ///< ECWmax
  Dot3TXOPLimit txoplimit; ///< TXOPLimit
};


/**
 * @brief EDCA Parameter Set
 */
struct Dot3EDCAParameterSet
{
  struct Dot3EDCAParameterRecord record[AC_NUM]; ///< AC 별 EDCA Parameter Set
};


/**
 * @brief WSA 헤더 정보
 */
struct Dot3WSAHdr
{
  // 필수필드
  Dot3SrvAdvMessageType msg_id; ///< 서비스광고 메시지 유형 (WSA 생성 시에는 사용되지 않는다)
  Dot3WSAVersion version; ///< WSA 버전 (WSA 생성 시에는 사용되지 않는다)
  Dot3WSAIdentifier wsa_id; ///< WSA 식별자
  Dot3WSAContentCount content_count; ///< WSA content count

  // 확장필드
  struct {
    bool repeat_rate; ///< RepeatRate 확장필드 수납 여부
    bool twod_location; ///< 2DLocation 확장필드 수납 여부
    bool threed_location; ///< 3DLocation 확장필드 수납 여부
    bool advertiser_id; ///< Advertiser id 확장필드 수납 여부
  } extensions;
  Dot3WSARepeatRate repeat_rate; ///< RepeatRate 확장필드
  struct Dot3WSATwoDLocation twod_location; ///< 2DLocation 확장필드
  struct Dot3WSAThreeDLocation threed_location; ///< 3DLocation 확장필드
  struct Dot3WSAAdvertiserID advertiser_id; ///< AdvertiserID 확장필드
};


/**
 * @brief WSR(WSM Service Request) 정보
 */
struct Dot3WSR
{
  Dot3PSID psid; ///< PSID
};


/**
 * @brief PSR 정보 (=dot3ProviderServiceRequestTableEntry in MIB)
 */
struct Dot3PSR
{
  // 필수 정보
  Dot3WSAIdentifier wsa_id; ///< WSA 헤더에 수납되는 WSA identifier
  Dot3PSID psid; ///< PSID
  Dot3ChannelNumber service_chan_num; ///< 서비스가 제공되는 채널
  bool ip_service; ///< IP 서비스인지 여부

  // 옵션 정보
  Dot3IPv6Address ipv6_address; ///< IP 서비스 제공자의 IPv6 주소
  uint16_t service_port; ///< IP 서비스인 경우, IP 서비스 제공자의 포트번호
  struct {
    bool psc; ///< PSC 정보 제공 여부
    bool provider_mac_addr; ///< Provider MAC address 정보 제공 여부
    bool rcpi_threshold; ///< RCPI threshold 정보 제공 여부
    bool wsa_cnt_threshold; ///< WSA count threshold 정보 제공 여부
    bool wsa_cnt_threshold_interval; ///< WSA count threshold interval 정보 제공 여부
  } present;
  struct Dot3PSC psc; ///< Provider Service Context
  Dot3MACAddress provider_mac_addr; ///< (서비스 제공자와 WSA 전송자가 다를 경우) 서비스 제공자의 MAC 주소
  Dot3RCPI rcpi_threshold; ///< 유효한 WSA 로 판단되기 위한 RCPI 임계값
  Dot3WSACountThreshold wsa_cnt_threshold; ///< 유효한 WSA 로 판단되기 위한 수신 개수
  Dot3WSACountThresholdInterval wsa_cnt_threshold_interval; ///< wsa_cnt_threshold 를 카운트하기 위한 100ms 단위 인터벌(1~)

  // (현재) 미사용필드
  // Dot3MACAddress dst_mac_addr; ///< (미사용 항목) WSA 목적지 MAC 주소. PSR 단위가 아닌 통합 프로세스 단위에서 제어한다.
  // Dot3WSAType wsa_type; ///< (미사용 항목) WSA 유형. PSR 단위가 아닌 통합 프로세스 단위에서 제어한다.
  // bool best_available_chan; ///< (미사용 항목) dot3가 적절한 채널을 선택한다. PSR 단위가 아닌 통합 프로세스 단위에서 제어한다.
  // Dot3ChannelNumber wsa_chan_num; ///< (미사용 항목) WSA가 전송되는 채널. PSR 단위가 아닌 통합 프로세스 단위에서 제어한다.
  // Dot3WSARepeatRate repeat_rate; ///< (미사용 항목) 5초당 WSA 전송 회수. PSR 단위가 아닌 통합 프로세스 단위에서 제어한다.
  // Info Elements Indicator - WSA 헤더 확장필드 포함 여부 -> Dot3_ConstructWSA()에서 사용.
  // Signature Lifetime - 서명 유효기간 -> PSR 단위가 아닌 시스템 단위에서 제어한다.
};


/**
 * @brief PCI(Provider Channel Info) 정보 (=dot3ProviderChannelInfoTable in MIB)
 */
struct Dot3PCI
{
  // 필수 정보
  Dot3OperatingClass operating_class; ///< operating class
  Dot3ChannelNumber chan_num; ///< 채널번호
  Dot3Power transmit_power_level; ///< 이 채널에서 사용할 수 있는 최대 EIRP(dBm)
  Dot3DataRate datarate; ///< 이 채널에서 사용할 수 있는 데이터레이트 또는 최소 데이터레이트
  bool adaptable_datarate; ///< true: datarate 이상 사용 가능. false: 본 채널에서 datarate 만 사용 가능

  // 옵션 정보
  struct {
    bool edca_param_set; ///< EDCA Parameter Set 옵션정보 존재 여부
    bool chan_access; ///< Channel access 옵션정보 존재 여부
  } present;
  struct Dot3EDCAParameterSet edca_param_set; ///< 본 채널에서 사용되는 EDCA Parameter Set
  Dot3ProviderChannelAccess chan_access; ///< 본 채널에서 사용되는 채널접속유형
};


/**
 * @brief USR(User Service Request) 정보 (=dot3UserServiceRequestTableEntry in MIB)
 */
struct Dot3USR
{
  // 필수 정보
  Dot3PSID psid; ///< PSID
  Dot3WSAType wsa_type; ///< WSA 유형

  // 옵션 정보
  struct {
    bool psc; ///< PSC 옵션정보 존재 여부
    bool src_mac_addr; ///< 송신지 MAC 주소 옵션정보 존재 여부
    bool advertiser_id; ///< Advertiser ID 옵션정보 존재 여부
    bool chan_num; ///< 채널번호 옵션정보 존재 여부
  } present;
  struct Dot3PSC psc; ///< PSC
  Dot3MACAddress src_mac_addr; ///< 송신지 MAC 주소
  struct Dot3WSAAdvertiserID advertiser_id; ///< Advertiser ID
  Dot3ChannelNumber chan_num; ///< 채널번호
};


/**
 * @brief WSI(WSA Service info) 정보 (WSA 에 수납되는 Service info)
 */
struct Dot3WSI
{
  // 필수 필드
  Dot3PSID psid; ///< PSID
  Dot3WSAChannelIndex channel_index; ///< 본 서비스가 제공되는 채널에 관련된 WCI 번호(1부터 시작)

  // 확장 필드
  struct {
    bool psc; ///< PSC 필드 수납 여부
    bool ipv6_address; ///< IPv6 주소 필드 수납 여부
    bool service_port; ///< Service port 필드 수납 여부
    bool provider_mac_address; ///< Provider MAC address 필드 수납 여부
    bool rcpi_threshold; ///< RCPI threshold 필드 수납 여부
    bool wsa_cnt_threshold; ///< WSA count threshold 필드 수납 여부
    bool wsa_cnt_threshold_interval; ///< WSA count threshold interval 필드 수납 여부
  } extensions;
  struct Dot3PSC psc; ///< PSC
  Dot3IPv6Address ipv6_address; ///< IPv6 서비스인 경우, 서버의 IPv6 주소
  uint16_t service_port; ///< IPv6 서비스인 경우, 서버의 포트 번호
  Dot3MACAddress provider_mac_address; ///< (서비스 제공자와 WSA 전송자가 다를 경우) 서비스 제공자의 MAC 주소
  Dot3RCPI rcpi_threshold; ///< RCPI 기준값(수신측에서 본 값 이상으로 수신된 WSA만 유효한 것으로 간주된다).
  Dot3WSACountThreshold wsa_cnt_threshold; ///< WSA 수신 횟수 기준값(수신측에서 본 값 이상의 회수로 수신된 WSA만 유효한 것으로 간주된다).
  Dot3WSACountThresholdInterval wsa_cnt_threshold_interval; ///< WSA 수신 횟수를 측정하는 시간 주기
};


/**
 * @brief WCI(WSA Channel Info) 정보 (WSA 에 수납되는 Channel info)
 */
struct Dot3WCI
{
  // 필수 필드
  Dot3OperatingClass operating_class; ///< operating class
  Dot3ChannelNumber chan_num; ///< 채널번호
  Dot3Power transmit_power_level; ///< 이 채널에서 사용할 수 있는 최대 EIRP(dBm)
  Dot3DataRate datarate; ///< 이 채널에서 사용할 수 있는 데이터레이트 또는 최소 데이터레이트
  bool adaptable_datarate; ///< true: 본 채널에서 datarate 만 사용 가능. false: datarate 이상 사용 가능

  // 확장 필드
  struct {
    bool chan_access; ///< Channel Access 필드 수납 여부
    bool edca_param_set; ///< EDCA Parameter Set 필드 수납 여부
  } extension;
  Dot3ProviderChannelAccess chan_access; ///< 본 채널에 적용되는 채널접속 유형
  struct Dot3EDCAParameterSet edca_param_set; ///< 본 채널에 적용되는 EDCA Parameter Set
};


/**
 * @brief WRA(WAVE Routing Advertisement) 정보 (WSA 에 수납되는 WRA)
 */
struct Dot3WRA
{
  // 필수 필드
  Dot3WRARouterLifetime router_lifetime; ///< Router lifetime
  Dot3IPv6Address ip_prefix; ///< IP prefix
  Dot3IPv6PrefixLen ip_prefix_len; ///< IP prefix length
  Dot3IPv6Address default_gw; ///< Default gateway IP 주소
  Dot3IPv6Address primary_dns; ///< Primary DNS

  // 확장 필드
  struct {
    bool secondary_dns; ///< Secondary DNS 주소 존재 여부
    bool gateway_mac_addr; ///< Gateway MAC 주소 존재 여부
  } present;
  Dot3IPv6Address secondary_dns; ///< Secondary DNS
  Dot3MACAddress gateway_mac_addr; ///< Gateway MAC 주소
};


/**
 * @brief UAS(User Available Service) 정보
 */
struct Dot3UAS
{
  // 필수 정보 - 수신정보
  Dot3MACAddress src_mac_addr; ///< WSA 송신자 MAC 주소
  Dot3WSAType wsa_type; ///< 수신된 WSA의 유형
  Dot3RCPI rcpi; ///< WSA 수신 RCPI
  Dot3Latitude tx_lat; ///< WSA 송신자의 위도 정보 (Unavailable 가능)
  Dot3Longitude tx_lon; ///< WSA 송신자의 경도 정보 (Unavailable 가능)
  Dot3Elevation tx_elev; ///< WSA 송신자의 고도 정보 (Unavailable 가능)
  bool available; ///< 본 정보가 유효한지 여부(RCPI threshold, WSA count threshold 조건을 만족하는 경우에 true가 된다)

  // 필수 정보 - 수신 WSA의 Service Info 및 Channel Info 필드에 수납된 필수 정보
  Dot3WSAIdentifier wsa_id; ///< WSA ID
  Dot3PSID psid; ///< PSID
  Dot3OperatingClass operating_class; ///< operating class
  Dot3ChannelNumber chan_num; ///< 채널번호
  Dot3Power transmit_power_level; ///< 이 채널에서 사용할 수 있는 최대 EIRP(dBm)
  Dot3DataRate datarate; ///< 이 채널에서 사용할 수 있는 데이터레이트 또는 최소 데이터레이트
  bool adaptable_datarate; ///< true: 본 채널에서 datarate 만 사용 가능. false: datarate 이상 사용 가능

  // 옵션 정보
  struct {
    bool advertiser_id; ///< WSA 송신자의 Advertiser ID 정보 존재 여부
    bool psc; ///< PSC 정보 존재 여부
    bool ipv6_address; ///< IPv6 주소 정보 존재 여부
    bool service_port; ///< Service port 정보 존재 여부
    bool provider_mac_address; ///< Provider MAC address 정보 존재 여부
    bool rcpi_threshold; ///< RCPI threshold 정보 존재 여부
    bool wsa_cnt_threshold; ///< WSA count threshold 정보 존재 여부
    bool wsa_cnt_threshold_interval; ///< WSA count threshold interval 정보 존재 여부
    bool edca_param_set; ///< EDCA Parameter Set 정보 존재 여부
    bool chan_access; ///< Channel Access 정보 존재 여부
    bool wra; ///< WRA 정보 존재 여부
  } present;
  struct Dot3WSAAdvertiserID advertiser_id; ///< WSA 송신자의 Advertiser ID 정보
  struct Dot3PSC psc; ///< PSC
  Dot3IPv6Address ipv6_address; ///< IPv6 서비스인 경우, 서버의 IPv6 주소
  uint16_t service_port; ///< IPv6 서비스인 경우, 서버의 포트 번호
  Dot3MACAddress provider_mac_address; ///< (서비스 제공자와 WSA 전송자가 다를 경우) 서비스 제공자의 MAC 주소
  Dot3RCPI rcpi_threshold; ///< RCPI 기준값(수신측에서 본 값 이상으로 수신된 WSA만 유효한 것으로 간주된다).
  Dot3WSACountThreshold wsa_cnt_threshold; ///< WSA 수신 횟수 기준값(수신측에서 본 값 이상의 회수로 수신된 WSA만 유효한 것으로 간주된다).
  Dot3WSACountThresholdInterval wsa_cnt_threshold_interval; ///< WSA 수신 횟수를 측정하는 시간 주기
  struct Dot3EDCAParameterSet edca_param_set; ///< 본 채널에 적용되는 EDCA Parameter Set
  Dot3ProviderChannelAccess chan_access; ///< 본 채널에 적용되는 채널접속 유형
  struct Dot3WRA wra; ///< WRA 정보
};


/**
 * @brief UAS(User Available Service) 정보 집합
 */
struct Dot3UASSet
{
  Dot3UASNum num; ///< UAS 정보 개수
  struct Dot3UAS uas[]; ///< UAS 정보(들). num 개수만큼 존재한다.
};


#endif //V2X_LIBDOT3_DOT3_TYPES_H
