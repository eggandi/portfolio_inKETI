/**
 * @file
 * @brief cvcoctci 라이브러리의 Open API 에서 사용되는 자료 유형을 정의한 헤더파일
 * @date 2019-09-14
 * @author gyun
 */

#ifndef V2X_SW_CVCOCTCI3_TYPES_H
#define V2X_SW_CVCOCTCI3_TYPES_H


// 시스템 헤더 파일
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// 라이브러리 헤더 파일
#include "cvcoctci3-defines.h"


/**
 * Update TCIv3 by young
 * Separate 16093 into 16093DSRC and 16093PC5
 * Add 31611
 * kCvcoctci3Result_InvalidPsidFormat rename to kCvcoctci3Result_InvalidVarFormat
 * 
 * @brief API 처리 결과 코드
 */
enum eCvcoctci3ResultCode
{
  kCvcoctci3Result_Success = 0,
  kCvcoctci3Result_NullParameters, ///< 널 파라미터를 전달하였음.
  kCvcoctci3Result_OerEncodeFail, ///< OER 인코딩 실패
  kCvcoctci3Result_OerDecodeFail, ///< OER 디코딩 실패
  kCvcoctci3Result_AbnormalPkt, ///< 비정상 패킷
  kCvcoctci3Result_InvalidFrameChoice, ///< 유효하지 않은 Frame choice
  kCvcoctci3Result_NotSupportedTci80211Choice, ///< 지원하지 않는 TCI80211 choice
  kCvcoctci3Result_InvalidTci80211Choice, ///< 유효하지 않은 TCI80211 choice
  kCvcoctci3Result_NotSupportedTci16093DSRCChoice,   ///< 지원하지 않는 TCI16093DSRC choice
  kCvcoctci3Result_InvalidTci16093DSRCChoice, ///< 유효하지 않은 TCI16093DSRC choice
  kCvcoctci3Result_NotSupportedTci16093PC5Choice,   ///< 지원하지 않는 TCI16093PC5 choice
  kCvcoctci3Result_InvalidTci16093PC5Choice, ///< 유효하지 않은 TCI16093PC5 choice
  kCvcoctci3Result_NotSupportedTci16094Choice,   ///< 지원하지 않는 TCI16094 choice
  kCvcoctci3Result_InvalidTci16094Choice, ///< 유효하지 않은 TCI16094 choice
  kCvcoctci3Result_NotSupportedTci29451Choice,   ///< 지원하지 않는 TCI29451 choice
  kCvcoctci3Result_InvalidTci29451Choice, ///< 유효하지 않은 TCI29451 choice
  kCvcoctci3Result_NotSupportedTci31611Choice,   ///< 지원하지 않는 TCI31611 choice
  kCvcoctci3Result_InvalidTci31611Choice, ///< 유효하지 않은 TCI31611 choice
  kCvcoctci3Result_NotSupportedTciSutControlChoice,   ///< 지원하지 않는 TciSutControl choice
  kCvcoctci3Result_InvalidTciSutControlChoice, ///< 유효하지 않은 TciSutControl choice
  kCvcoctci3Result_NotSupportedTciProxyCv2xChoice,  ///< 지원하지 않는 TciProxyCv2x choice
  kCvcoctci3Result_InvalidTciProxyCv2xChoice, ///< 유효하지 않은 TciProxyCv2x choice
  kCvcoctci3Result_InvalidMessageId, ///< 유효하지 않은 messageId
  kCvcoctci3Result_InvalidVarFormat, ///< 유효하지 않은 var 형식
  kCvcoctci3Result_InvalidPsidValue, ///< 유효하지 않은 PSID 값
  kCvcoctci3Result_InsufficientBuf, ///< 충분하지 않은 크기의 버퍼
  kCvcoctci3Result_TooManyPsid, ///< 너무 많은 PSID
  kCvcoctci3Result_TooLongPdu, ///< 너무 긴 PDU
  kCvcoctci3Result_InvalidFrameType, ///< 유효하지 않은 프레임 유형
  kCvcoctci3Result_InvalidEventParamsType, ///< 유효하지 않은 이벤트파라미터 유형
  kCvcoctci3Result_InvalidInfoContentType, ///< 유효하지 않은 info content 유형
  kCvcoctci3Result_InvalidEvent, ///< 유효하지 않은 Event
  kCvcoctci3Result_InvalidRcpiValue, ///< 유효하지 않은 RCPI 값
  kCvcoctci3Result_InvalidPduType, ///< 유효하지 않은 Pdu Type
  kCvcoctci3Result_InvalidRadioValue, ///< 유효하지 않은 Radio 값
  kCvcoctci3Result_InvalidResultCode, ///< 유효하지 않은 ResultCode 값
  kCvcoctci3Result_InvalidExceptionType, ///< 유효하지 않은 ExceptionType 값
  kCvcoctci3Result_InvalidExceptionId, ///< 유효하지 않은 ExceptionId 값
  kCvcoctci3Result_InvalidExtensionId, ///< 유효하지 않은 ExtensionId 값
  kCvcoctci3Result_TooLongOctetString, ///< 너무 긴 Octet string

  kCvcoctci3Result_NotSupportedYet,  ///< 아직 지원되지 않음
  kCvcoctci3Result_NoMemory,         ///< 메모리 부족
  kCvcoctci3Result_SystemCallFailed, ///< 시스템콜 호출 실패
};
typedef int Cvcoctci3ResultCode; ///< @ref eCvcoctci3ResultCode


/**
 * @brief 로그메시지 출력 레벨
 *
 * 높은 레벨은 낮은 레벨의 범위를 포함한다. \n
 * 즉, 로그변수가 높은 레벨로 설정되어 있으면, 그 하위레벨에 해당되는 로그는 함께 출력된다.
 */
enum eCvcoctci3LogLevel
{
  kCvcoctci3LogLevel_none = 0, /// 아무 로그도 출력하지 않는다.
  kCvcoctci3LogLevel_err, /// 에러 로그
  kCvcoctci3LogLevel_init, /// 초기화 절차에 관련된 로그
  kCvcoctci3LogLevel_config, /// 설정에 관련된 로그 (API 엔트리 포함)
  kCvcoctci3LogLevel_event, /// 각종 이벤트(패킷 송수신 포함)에 관련된 로그
  kCvcoctci3LogLevel_dump, /// 상세내용 로그(송수신 패킷 덤프 데이터 등)
  kCvcoctci3LogLevel_all, /// 모든 로그
  kCvcoctci3LogLevel_Min = kCvcoctci3LogLevel_none,
  kCvcoctci3LogLevel_Max = kCvcoctci3LogLevel_all
};
typedef int Cvcoctci3LogLevel;  ///< @ref eCvcoctci3LogLevel



/**
 * Update TCIv3 by young
 * kCvcoctci3Version_Current = 3
 * 
 * @brief Version
 */
enum eCvcoctci3Version
{
  kCvcoctci3Version_Min = 1,
  kCvcoctci3Version_Current = 3,
  kCvcoctci3Version_Max = 127,
  kCvcoctci3Version_NA = -999,
};
typedef int Cvcoctci3Version; ///< @ref eCvcoctci3Version


/**
 * @brief Radio
 */
enum eCvcoctci3Radio
{
  kCvcoctci3Radio_0,
  kCvcoctci3Radio_1,
  kCvcoctci3Radio_2,
  kCvcoctci3Radio_3,
  kCvcoctci3Radio_Min = kCvcoctci3Radio_0,
  kCvcoctci3Radio_Max = kCvcoctci3Radio_3,
  kCvcoctci3Radio_NA = -999,
};
typedef int Cvcoctci3Radio; ///< @ref eCvcoctci3Radio


/**
 * @brief Antenna
 */
enum eCvcoctci3Antenna
{
  kCvcoctci3Antenna_1 = 1,
  kCvcoctci3Antenna_2 = 2,
  kCvcoctci3Antenna_both = 3,
  kCvcoctci3Antenna_NA = -999,
};
typedef int Cvcoctci3Antenna; ///< @ref eCvcoctci3Antenna


/**
 * @brief RadioInterface
 */
struct Cvcoctci3RadioInterface
{
  /// 옵션 필드 설정 여부
  struct {
    bool antenna;
  } options;
  Cvcoctci3Radio radio; ///< 라디오 식별번호
  Cvcoctci3Antenna antenna; ///< 안테나 식별번호
};
/// struct @ref Cvcoctci3RadioInterface 구조체 초기화 매크로
#define CVCOCTCI3_RADIO_INTERFACE_INITIALIZER {\
  .options = {false},\
  .radio = kCvcoctci3Radio_NA,\
  .antenna = kCvcoctci3Antenna_NA,\
}


/**
 * @brief 밀리초 단위의 UTC 타임스탬프 (1970년 1월1일부터)
 */
typedef uint64_t Cvcoctci3Time64;
/// Time64 최소값
#define kCvcoctci3Time64_Min 0
/// Time64 최대값
#define kCvcoctci3Time64_Max 9223372036854775807ULL


/**
 * @brief TCI 프레임 유형
 * @date 2022-01-12
 * @details Update TCIv3 by young
 * @details kCvcoctci3FrameType_16093 renamed to kCvcoctci3FrameType_16093Dsrc
 * @details Add kCvcoctci3FrameType_16093Cv2x, kCvcoctci3FrameType_31611 and kCvcoctci3FrameType_ProxyCv2x
 */
enum eCvcoctci3FrameType
{
  kCvcoctci3FrameType_16093Dsrc,
  kCvcoctci3FrameType_80211,
  kCvcoctci3FrameType_16094,
  kCvcoctci3FrameType_29451,
  kCvcoctci3FrameType_SutControl,
  kCvcoctci3FrameType_16093Cv2x,
  kCvcoctci3FrameType_31611,
  kCvcoctci3FrameType_ProxyCv2x,
  kCvcoctci3FrameType_NA = -999,
};
typedef int Cvcoctci3TciFrameType; ///< @ref eCvcoctci3FrameType


/**
 * @brief TCI Request 메시지 유형
 */
enum eCvcoctci3RequestType
{
  // from TCI-wsm
  kCvcoctci3RequestType_SetInitialState,
  kCvcoctci3RequestType_SetWsmTxInfo,
  kCvcoctci3RequestType_StartWsmTx,
  kCvcoctci3RequestType_StopWsmTx,
  kCvcoctci3RequestType_StartWsmRx,
  kCvcoctci3RequestType_StopWsmRx,
  kCvcoctci3RequestType_StartWsaTxPeriodic,
  kCvcoctci3RequestType_StopWsaTxPeriodic,
  kCvcoctci3RequestType_AddWsaProviderService,
  kCvcoctci3RequestType_ChangeWsaProviderService,
  kCvcoctci3RequestType_DelWsaProviderService,
  kCvcoctci3RequestType_AddUserService,
  kCvcoctci3RequestType_DelUserService,
  kCvcoctci3RequestType_RequestWsmTxCount,
  kCvcoctci3RequestType_RequestWsmRxCount,
  kCvcoctci3RequestType_RequestWsmTxCountReset,
  kCvcoctci3RequestType_RequestWsmRxCountReset,

  // from TCI-ip
  kCvcoctci3RequestType_GetIPv6InterfaceInfo,
  kCvcoctci3RequestType_SetIPv6Address,
  kCvcoctci3RequestType_StartIPv6Tx,
  kCvcoctci3RequestType_StopIPv6Tx,
  kCvcoctci3RequestType_StartIPv6Rx,
  kCvcoctci3RequestType_StopIPv6Rx,
  kCvcoctci3RequestType_StartIPv6Ping,
  kCvcoctci3RequestType_StopIPv6Ping,
  kCvcoctci3RequestType_AddTxProfile,
  kCvcoctci3RequestType_DelTxProfile,

  // from TCI-SutControl
  kCvcoctci3RequestType_Shutdown,
  kCvcoctci3RequestType_Restart,
  kCvcoctci3RequestType_RequestSutAvailability,
  kCvcoctci3RequestType_RequestSutInfo,
  kCvcoctci3RequestType_SetTestId,
  kCvcoctci3RequestType_EnableGpsInput,
  kCvcoctci3RequestType_SetLatitude,
  kCvcoctci3RequestType_SetLongitude,
  kCvcoctci3RequestType_SetElevation,
  kCvcoctci3RequestType_SetPositionalAccuracy,
  kCvcoctci3RequestType_SetSpeed,
  kCvcoctci3RequestType_SetHeading,
  kCvcoctci3RequestType_SetAccelerationSet4Way,
  kCvcoctci3RequestType_SetGpsTime,
  kCvcoctci3RequestType_RequestSutStatus,

  // from TCI29451
  kCvcoctci3RequestType_StartBsmTx,
  kCvcoctci3RequestType_StopBsmTx,
  kCvcoctci3RequestType_EnableCongestionMitigation,
  kCvcoctci3RequestType_SetTemporaryID,
  kCvcoctci3RequestType_SetTransmissionState,
  kCvcoctci3RequestType_SetSteeringWheelAngle,
  kCvcoctci3RequestType_SetBrakeSystemStatus,
  kCvcoctci3RequestType_SetVehicleSize,
  kCvcoctci3RequestType_SetExteriorLights,
  kCvcoctci3RequestType_SetVehicleEventFlags,
  kCvcoctci3RequestType_StartBsmRx,
  kCvcoctci3RequestType_StopBsmRx,

  // from TCI16093PC5
  kCvcoctci3RequestType_Pc5StartWsmTx,
  kCvcoctci3RequestType_SendUeConfigXML,
  kCvcoctci3RequestType_SetUeConfig,
  kCvcoctci3RequestType_SetFlowConfig,
  kCvcoctci3RequestType_SendATcommand,

  // from TCIproxyCv2x
  kCvcoctci3RequestType_StartUdpProxyTx,
  kCvcoctci3RequestType_StartUdpProxyRx,
  kCvcoctci3RequestType_StopUdpProxy,

  kCvcoctci3RequestType_NA = -999,
};
typedef int Cvcoctci3RequestType; ///< @ref eCvcoctci3RequestType


/**
 * @brief PSID
 */
enum eCvcoctci3Psid
{
  kCvcoctci3Psid_NA = -999,
};
typedef int Cvcoctci3Psid; ///< @ref eCvcoctci3Psid


/**
 * @brief ContentType
 */
enum eCvcoctci3ContentType
{
  kCvcoctci3ContentType_Other = 0,
  kCvcoctci3ContentType_Ieee16092Data = 1,
  kCvcoctci3ContentType_Wsa = 2,
  kCvcoctci3ContentType_Bsm = 3,
  kCvcoctci3ContentType_Map = 4,
  kCvcoctci3ContentType_Spat = 5,
  kCvcoctci3ContentType_Tim = 6,
  kCvcoctci3ContentType_NA = -999,
};
typedef int Cvcoctci3ContentType; ///< @ref eCvcoctci3ContentType


/**
 * @brief SignerIdentifierType
 */
enum eCvcoctci3SignerIdentifierType
{
  kCvcoctci3SignerIdentifierType_Unsecure = 0,
  kCvcoctci3SignerIdentifierType_UseSecProfilePerContentType = 1,
  kCvcoctci3SignerIdentifierType_SignIncludeCertificate = 2,
  kCvcoctci3SignerIdentifierType_SignIncludeDigest = 3,
  kCvcoctci3SignerIdentifierType_NA = -999,
};
typedef int Cvcoctci3SignerIdentifierType; ///< @ref eCvcoctci3SignerIdentifierType


/// @brief HashedId8
typedef uint8_t Cvcoctci3HashedId8[HASHED_ID8_LEN];
/// @brief HashedId8 초기화 매크로
#define CVCOCTCI3_HASHED_ID8_INITIALIZER {\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 \
}


/**
 * @brief ChannelNumber80211
 */
enum eCvcoctci3ChannelNumber
{
  kCvcoctci3ChannelNumber_Min = 0,
  kCvcoctci3ChannelNumber_Max = 255,
  kCvcoctci3ChannelNumber_NA = -999,
};
typedef int Cvcoctci3ChannelNumber; ///< @ref eCvcoctci3ChannelNumber


/**
 * @brief TimeSlot
 */
enum eCvcoctci3TimeSlot
{
  kCvcoctci3TimeSlot_AltSlot0 = 1,
  kCvcoctci3TimeSlot_AltSlot1 = 2,
  kCvcoctci3TimeSlot_Continuous = 3,
  kCvcoctci3TimeSlot_Min = kCvcoctci3TimeSlot_AltSlot0,
  kCvcoctci3TimeSlot_Max = kCvcoctci3TimeSlot_Continuous,
  kCvcoctci3TimeSlot_NA = -999,
};
typedef int Cvcoctci3TimeSlot; ///< @ref eCvcoctci3TimeSlot


/**
 * @brief DataRate80211
 */
enum eCvcoctci3DataRate
{
  kCvcoctci3DataRate_Min = 0,
  kCvcoctci3DataRate_Max = 255,
  kCvcoctci3DataRate_NA = -999,
};
typedef int Cvcoctci3DataRate; ///< @ref eCvcoctci3DataRate


/**
 * @brief TxPower80211
 */
enum eCvcoctci3TxPower
{
  kCvcoctci3TxPower_Min = -128,
  kCvcoctci3TxPower_Max = 127,
  kCvcoctci3TxPower_NA = -999,
};
typedef int Cvcoctci3TxPower; ///< @ref eCvcoctci3TxPower


/**
 * @brief User priority
 */
enum eCvcoctci3UserPriority
{
  kCvcoctci3UserPriority_Min = 0,
  kCvcoctci3UserPriority_Max = 7,
  kCvcoctci3UserPriority_NA = -999,
};
typedef int Cvcoctci3UserPriority; ///< @ref eCvcoctci3UserPriority



/**
 * @brief RepeatRate (5초당 전송 회수)
 */
enum eCvcoctci3RepeatRate
{
  kCvcoctci3RepeatRate_Min = 0,
  kCvcoctci3RepeatRate_Max = 255,
  kCvcoctci3RepeatRate_NA = -999,
};
typedef int Cvcoctci3RepeatRate; ///< @ref eCvcoctci3RepeatRate


/**
 * @brief Latitude(0.1 마이크로도 단위의 위도)
 */
enum eCvcoctci3Latitude
{
  kCvcoctci3Latitude_Min = -900000000,
  kCvcoctci3Latitude_Max = 900000000,
  kCvcoctci3Latitude_NA = 900000001
};
typedef int Cvcoctci3Latitude; ///< @ref eCvcoctci3Latitude


/**
 * @brief Longitude(0.1 마이크로도 단위의 경도)
 */
enum eCvcoctci3Longitude
{
  kCvcoctci3Longitude_Min = -1799999999,
  kCvcoctci3Longitude_Max = 1800000000,
  kCvcoctci3Longitude_NA = 1800000001,
};
typedef int Cvcoctci3Longitude; ///< @ref eCvcoctci3Longitude


/**
 * @brief Elevation(10cm 단위의 고도)
 */
enum eCvcoctci3Elevation
{
  kCvcoctci3Elevation_Min = -4095,
  kCvcoctci3Elevation_Max = 61439,
  kCvcoctci3Elevation_NA = -4096,
};
typedef int Cvcoctci3Elevation; ///< @ref eCvcoctci3Elevation

/**
 * Update TCIv3 by young
 * @brief FlowIdentifier
 * */
enum eCvcoctci3FlowIdentifier
{
  kCvcoctci3FlowIdentifier_Min = 0,
  kCvcoctci3FlowIdentifier_Max = 65535,
  kCvcoctci3FlowIdentifier_NA = -999,
};
typedef int Cvcoctci3FlowIdentifier; ///< @ref eCvcoctci3FlowIdentifier

/**
 * @brief WAVE 확장헤더 포함 여부
 */
struct Cvcoctci3WaveElementsIncluded
{
  bool transmit_power_used;
  bool twod_location;
  bool threed_location;
  bool advertiser_id;
  bool psc;
  bool ipv6_addr;
  bool service_port;
  bool provider_mac_addr;
  bool edca_param_set;
  bool secondary_dns;
  bool gateway_mac_addr;
  bool chan_num;
  bool datarate;
  bool repeat_rate;
  bool rcpi_threshold;
  bool wsa_cnt_threshold;
  bool chan_access;
  bool wsa_cnt_threshold_interval;
};
/// struct @ref Cvcoctci3WaveElementsIncluded 구조체 초기화 매크로
#define CVCOCTCI3_WAVE_ELEMENTS_INCLUDED_INITIALIZER { \
  .transmit_power_used = false,\
  .twod_location = false,\
  .threed_location = false,\
  .advertiser_id = false,\
  .psc = false,\
  .ipv6_addr = false,\
  .service_port = false,\
  .provider_mac_addr = false,\
  .edca_param_set = false,\
  .secondary_dns = false,\
  .gateway_mac_addr = false,\
  .chan_num = false,\
  .datarate = false,\
  .repeat_rate = false,\
  .rcpi_threshold = false,\
  .wsa_cnt_threshold = false,\
  .chan_access = false,\
  .wsa_cnt_threshold_interval = false,\
}


/**
 * @brief RxFlag
 */
struct Cvcoctci3RxFlag
{
  bool recv_psid_match;
  bool include_pdu; ///< 1일 경우, 수신된 WAVE 패킷을 Indication 으로 전달할 때, Pdu 데이터를 포함시켜야 한다.
  bool include_pdu_param; ///< 1일 경우, 수신된 WAVE 패킷을 Indication 으로 전달할 때, EventParameters 를 포함시켜야 한다.
};
/// struct @ref Cvcoctci3RxFlag 초기화 매크로
#define CVCOCTCI3_RX_FLAG_INITIALIZER {\
  .recv_psid_match = false,\
  .include_pdu = false,\
  .include_pdu_param = false,\
}


/**
 * @brief EventFlag
 */
struct Cvcoctci3EventFlag
{
  bool e80211_pkt_rx;
  bool e16093_pkt_rx;
  bool wsm;
  bool ipv6_pkt_rx;
  bool icmp6_pkt_rx;
  bool psid_service_active;
  bool wsa_service_active;
  bool ipv6_config_changed;
  bool verification_completed_with_result;
};
/// struct @ref Cvcoctci3EventFlag 초기화 매크로
#define CVCOCTCI3_EVENT_FLAG_INITIALIZER {\
  .e80211_pkt_rx = false,\
  .e16093_pkt_rx = false,\
  .wsm = false,\
  .ipv6_pkt_rx = false,\
  .icmp6_pkt_rx = false,\
  .psid_service_active = false,\
  .wsa_service_active = false,\
  .ipv6_config_changed = false,\
  .verification_completed_with_result = false,\
}


/**
 * Update TCIv3 by young
 * Remove kCvcoctci3PduType_80211frame. DEPRECATED. Use d16093payload instead
 * Remove kCvcoctci3PduType_16093frame. DEPRECATED. Use d16093payload instead
 * 
 * @brief PduType
 */
enum eCvcoctci3PduType
{
  kCvcoctci3PduType_16092data = 1,
  kCvcoctci3PduType_16093payload = 2,
  kCvcoctci3PduType_ipv6payload = 3,
  kCvcoctci3PduType_Min = kCvcoctci3PduType_16092data,
  kCvcoctci3PduType_Max = kCvcoctci3PduType_ipv6payload,
  kCvcoctci3PduType_NA = -999,
};
typedef int Cvcoctci3PduType; ///< @ref eCvcoctci3PduType


/**
 * @brief SecurityFlag
 */
struct Cvcoctci3SecurityFlag
{
  bool bypass_security_verification;
};
/// struct @ref Cvcoctci3SecurityFlag 초기화 매크로
#define CVCOCTCI3_SECURITY_FLAG_INITIALIZER {\
  .bypass_security_verification = false,\
}


/**
 * Update TCIv3 by young
 * kCvcoctci3EventParamsChoice_d80211frame rename to kCvcoctci3EventParamsChoice_radioframe
 * 
 * @brief EventParamsChoice
 */
enum eCvcoctci3EventParamsChoice
{
  kCvcoctci3EventParamsChoice_service,
  kCvcoctci3EventParamsChoice_wsm,
  kCvcoctci3EventParamsChoice_ip,
  kCvcoctci3EventParamsChoice_radioframe,
  kCvcoctci3EventParamsChoice_security,
  kCvcoctci3EventParamsChoice_NA = -999,
};
typedef int Cvcoctci3EventParamsChoice; ///< @ref eCvcoctci3EventParamsChoice


/**
 * @brief RCPI
 */
enum eCvcoctci3Rcpi
{
  kCvcoctci3Rcpi_Min = 0,
  kCvcoctci3Rcpi_Max = 255,
  kCvcoctci3Rcpi_NA = -999,
};
typedef int Cvcoctci3Rcpi; ///< @ref eCvcoctci3Rcpi

/**
 * @brief Channel index
 */
enum eCvcoctci3ChannelIndex
{
  kCvcoctci3ChannelIndex_NotUsed = 0,
  kCvcoctci3ChannelIndex_FirstEntry = 1,
  kCvcoctci3ChannelIndex_Min = kCvcoctci3ChannelIndex_NotUsed,
  kCvcoctci3ChannelIndex_Max = 31,
  kCvcoctci3ChannelIndex_NA = -999,
};
typedef int Cvcoctci3ChannelIndex; ///< @ref eCvcoctci3ChannelIndex


/**
 * @brief EventHandling
 */
struct Cvcoctci3EventHandling
{
  struct {
    bool rx_flag;
    bool event_flag;
    bool forward_pdu;
    bool security_flag;
    bool event_params_choice;
  } options;

  struct Cvcoctci3RxFlag rx_flag;
  struct Cvcoctci3EventFlag event_flag;
  Cvcoctci3PduType forward_pdu;
  struct Cvcoctci3SecurityFlag security_flag;
  Cvcoctci3EventParamsChoice event_params_choice;
};
/// struct @ref Cvcoctci3EventHandling 초기화 매크로
#define CVCOCTCI3_EVENT_HANDLING_INITIALIZER { \
  .options = { false, false, false, false, false },\
  .rx_flag = CVCOCTCI3_RX_FLAG_INITIALIZER,\
  .event_flag = CVCOCTCI3_EVENT_FLAG_INITIALIZER,\
  .forward_pdu = kCvcoctci3PduType_NA,\
  .security_flag = CVCOCTCI3_SECURITY_FLAG_INITIALIZER,\
  .event_params_choice = kCvcoctci3EventParamsChoice_NA,\
}


/**
 * @brief SetLatitude 메시지 형식
 */
typedef Cvcoctci3Latitude Cvcoctci3SetLatitude;


/**
 * @brief SetLongitude 메시지 형식
 */
typedef Cvcoctci3Longitude Cvcoctci3SetLongitude;


/**
 * @brief SetElevation 메시지 형식
 */
typedef Cvcoctci3Elevation Cvcoctci3SetElevation;


/**
 * @brief SetSpeed 메시지 형식
 */
enum eCvcoctci3Speed
{
  kCvcoctci3Speed_Min = 0,
  kCvcoctci3Speed_Max = 8191,
};
typedef int Cvcoctci3SetSpeed; ///< @ref eCvcoctci3Speed


/**
 * @brief SetHeading 메시지 형식
 */
typedef int Cvcoctci3SetHeading;


/**
 * @brief EnableCongestionMitigation 메시지 형식
 */
typedef bool Cvcoctci3EnableCongestionMitigation;


/**
 * @brief SetGpsTime 메시지 형식
 */
#define Cvcoctci3SetGpsTime Cvcoctci3Time64


/**
 * @brief SetTransmissionState 메시지 형식
 */
enum eCvcoctci3SetTransmissionState
{
  kCvcoctci3SetTransmissionState_Neutral,
  kCvcoctci3SetTransmissionState_Park,
  kCvcoctci3SetTransmissionState_ForwardGears,
  kCvcoctci3SetTransmissionState_ReverseGears,
  kCvcoctci3SetTransmissionState_Reserved1,
  kCvcoctci3SetTransmissionState_Reserved2,
  kCvcoctci3SetTransmissionState_Reserved3,
  kCvcoctci3SetTransmissionState_Unavailable,
};
typedef int Cvcoctci3SetTransmissionState; ///< @ref eCvcoctci3SetTransmissionState


/**
 * @brief SetSteeringWheelAngle 메시지 형식
 */
enum eCvcoctci3SetSteeringWheelAngle
{
  kCvcoctci3SetSteeringWheelAngle_Min = -126,
  kCvcoctci3SetSteeringWheelAngle_Max = 127,
};
typedef int Cvcoctci3SetSteeringWheelAngle; ///< @ref eCvcoctci3SetSteeringWheelAngle


/**
 * @brief VehicleWidth
 */
enum eCvcoctci3VehicleWidth
{
  kCvcoctci3VehicleWidth_Min = 0,
  kCvcoctci3VehicleWidth_Max = 1023,
};
typedef int Cvcoctci3VehicleWidth; ///< @ref eCvcoctci3VehicleWidth


/**
 * @brief VehicleLength
 */
enum eCvcoctci3VehicleLength
{
  kCvcoctci3VehicleLength_Min = 0,
  kCvcoctci3VehicleLength_Max = 4095,
};
typedef int Cvcoctci3VehicleLength; ///< @ref eCvcoctci3VehicleLength


/**
 * @brief SetVehicleSize 메시지 형식
 */
struct Cvcoctci3SetVehicleSize
{
  Cvcoctci3VehicleWidth width;
  Cvcoctci3VehicleLength length;
};


/**
 * @brief SetExteriorLights 메시지 형식
 */
struct Cvcoctci3SetExteriorLights
{
  bool low_beam_headlight_on;
  bool high_beam_headlight_on;
  bool left_turn_signal_on;
  bool right_turn_signal_on;
  bool hazard_signal_on;
  bool automatic_light_control_on;
  bool daytime_running_lights_on;
  bool fog_light_on;
  bool parking_light_on;
};


/**
 * @brief BrakeAppliedStatus
 */
struct Cvcoctci3BrakeAppliedStatus
{
  bool unavailable;
  bool left_front;
  bool left_rear;
  bool right_front;
  bool right_rear;
};


/**
 * @brief TractionControlStatus
 */
enum eCvcoctci3TractionControlStatus
{
  kCvcoctci3TractionControlStatus_Unavailable,
  kCvcoctci3TractionControlStatus_Off,
  kCvcoctci3TractionControlStatus_On,
  kCvcoctci3TractionControlStatus_Engaged,
};
typedef int Cvcoctci3TractionControlStatus; ///< @ref eCvcoctci3TractionControlStatus


/**
 * @brief AntiLockBrakeStatus
 */
enum eCvcoctci3AntiLockBrakeStatus
{
  kCvcoctci3AntiLockBrakeStatus_Unavailable,
  kCvcoctci3AntiLockBrakeStatus_Off,
  kCvcoctci3AntiLockBrakeStatus_On,
  kCvcoctci3AntiLockBrakeStatus_Engaged,
};
typedef int Cvcoctci3AntiLockBrakeStatus; ///< @ref eCvcoctci3AntiLockBrakeStatus


/**
 * @brief StabilityControlStatus
 */
enum eCvcoctci3StabilityControlStatus
{
  kCvcoctci3StabilityControlStatus_Unavailable,
  kCvcoctci3StabilityControlStatus_Off,
  kCvcoctci3StabilityControlStatus_On,
  kCvcoctci3StabilityControlStatus_Engaged,
};
typedef int Cvcoctci3StabilityControlStatus; ///< @ref eCvcoctci3StabilityControlStatus


/**
 * @brief BrakeBoostApplied
 */
enum eCvcoctci3BrakeBoostApplied
{
  kCvcoctci3BrakeBoostApplied_Unavailable,
  kCvcoctci3BrakeBoostApplied_Off,
  kCvcoctci3BrakeBoostApplied_On,
};
typedef int Cvcoctci3BrakeBoostApplied; ///< @ref eCvcoctci3BrakeBoostApplied


/**
 * @brief AuxiliaryBrakeStatus
 */
enum eCvcoctci3AuxiliaryBrakeStatus
{
  kCvcoctci3AuxiliaryBrakeStatus_Unavailable,
  kCvcoctci3AuxiliaryBrakeStatus_Off,
  kCvcoctci3AuxiliaryBrakeStatus_On,
  kCvcoctci3AuxiliaryBrakeStatus_Reserved,
};
typedef int Cvcoctci3AuxiliaryBrakeStatus; ///< @ref eCvcoctci3AuxiliaryBrakeStatus


/**
 * @brief SetBrakeSystemStatus 메시지 형식
 */
struct Cvcoctci3SetBrakeSystemStatus
{
  struct Cvcoctci3BrakeAppliedStatus brake_applied_status;
  Cvcoctci3TractionControlStatus traction_control_status;
  Cvcoctci3AntiLockBrakeStatus anti_lock_brake_status;
  Cvcoctci3StabilityControlStatus stability_control_status;
  Cvcoctci3BrakeBoostApplied brake_boost_applied;
  Cvcoctci3AuxiliaryBrakeStatus auxiliary_brake_status;
};


/**
 * @brief Acceleration
 */
enum eCvcoctci3Acceleration
{
  kCvcoctci3Acceleration_Min = -2000,
  kCvcoctci3Acceleration_Max = 2001,
};
typedef int Cvcoctci3Acceleration; ///< @ref eCvcoctci3Acceleration


/**
 * @brief VerticalAcceleration
 */
enum eCvcoctci3VerticalAcceleration
{
  kCvcoctci3VerticalAcceleration_Min = -127,
  kCvcoctci3VerticalAcceleration_Max = 127,
};
typedef int Cvcoctci3VerticalAcceleration; ///< @ref eCvcoctci3VerticalAcceleration


/**
 * @brief YawRate
 */
enum eCvcoctci3YawRate
{
  kCvcoctci3YawRate_Min = -32767,
  kCvcoctci3YawRate_Max = 32767,
};
typedef int Cvcoctci3YawRate; ///< @ref eCvcoctci3YawRate


/**
 * @brief SetAccelerationSet4Way 메시지 형식
 */
struct Cvcoctci3SetAccelerationSet4Way
{
  Cvcoctci3Acceleration latitude;
  Cvcoctci3Acceleration longitude;
  Cvcoctci3VerticalAcceleration vertical;
  Cvcoctci3YawRate yaw_rate;
};


/**
 * @brief SemiMajorAxisAccuracy
 */
enum eCvcoctci3SemiMajorAxisAccuracy
{
  kCvcoctci3SemiMajorAxisAccuracy_Min = 0,
  kCvcoctci3SemiMajorAxisAccuracy_Max = 255,
  kCvcoctci3SemiMajorAxisAccuracy_NA = 255,
};
typedef int Cvcoctci3SemiMajorAxisAccuracy; ///< @ref eCvcoctci3SemiMajorAxisAccuracy


/**
 * @brief SemiMinorAxisAccuracy
 */
enum eCvcoctci3SemiMinorAxisAccuracy
{
  kCvcoctci3SemiMinorAxisAccuracy_Min = 0,
  kCvcoctci3SemiMinorAxisAccuracy_Max = 255,
  kCvcoctci3SemiMinorAxisAccuracy_NA = 255,
};
typedef int Cvcoctci3SemiMinorAxisAccuracy; ///< @ref eCvcoctci3SemiMinorAxisAccuracy


/**
 * @brief SemiMajorAxisOrientation
 */
enum eCvcoctci3SemiMajorAxisOrientation
{
  kCvcoctci3SemiMajorAxisOrientation_Min = 0,
  kCvcoctci3SemiMajorAxisOrientation_Max = 65535,
  kCvcoctci3SemiMajorAxisOrientation_NA = 65535,
};
typedef int Cvcoctci3SemiMajorAxisOrientation; ///<@ref eCvcoctci3SemiMajorAxisOrientation


/**
 * @brief SetPositionalAccuracy
 */
struct Cvcoctci3SetPositionalAccuracy
{
  Cvcoctci3SemiMajorAxisAccuracy semi_major_axis_accuracy;
  Cvcoctci3SemiMinorAxisAccuracy semi_minor_axis_accuracy;
  Cvcoctci3SemiMajorAxisOrientation semi_major_axis_orientation;
};


/**
 * @brief SetTestId 메시지 형식
 */
struct Cvcoctci3SetTestId
{
#define CVCOCTCI3_TEST_ID_MAX_LEN 255 ///< CVCOC TCI 시험에서 사용되는 TEST ID 의 최대 길이
  size_t len; ///< TestId 길이
  char str[CVCOCTCI3_TEST_ID_MAX_LEN+1]; ///< TestId
};


/**
 * @brief SetVehicleEventFlags
 */
struct Cvcoctci3SetVehicleEventFlags
{
  bool eventHazardLights;
  bool eventStopLineViolation;
  bool eventABSactivated;
  bool eventTractionControlLoss;
  bool eventStabilityControlActivated;
  bool eventHazardousMaterials;
  bool eventReserved1;
  bool eventHardBraking;
  bool eventLightsChanged;
  bool eventWipersChanged;
  bool eventFlatTire;
  bool eventDisabledVehicle;
  bool eventAirBagDeployment;
};


/**
 * Update TCIv3 by young
 * ssp is added
 * 
 * @brief SecurityPermission
 */
struct Cvcoctci3SecurityPermission
{
#define SSP_MAX_LEN 65535
  size_t len; ///< 길이
  uint8_t buf[SSP_MAX_LEN+1]; ///< buffer
};
/// struct @ref Cvcoctci3PduFilter 초기화 매크로
#define CVCOCTCI3_SECURITY_PERMISSION_INITIALIZER {\
  .len = 0,\
  .buf = {0,},\
}


/**
 * Update TCIv3 by young
 * SecurityPermission is added
 * 
 * @brief SecurityContext
 */
struct Cvcoctci3SecurityContext
{
  struct {
    bool cert_id;
    bool ssp;
  } options;
  Cvcoctci3ContentType content_type;
  Cvcoctci3SignerIdentifierType  signer_id_type;
  uint8_t cert_id[8];
  struct Cvcoctci3SecurityPermission ssp;
};
/// struct @ref Cvcoctci3SecurityContext 초기화 매크로
#define CVCOCTCI3_SECURITY_CONTEXT_INITIALIZER {\
  .options = { false },\
  .content_type = kCvcoctci3ContentType_NA,\
  .signer_id_type = kCvcoctci3SignerIdentifierType_NA,\
  .cert_id = CVCOCTCI3_HASHED_ID8_INITIALIZER,\
  .ssp = CVCOCTCI3_SECURITY_PERMISSION_INITIALIZER,\
}


/**
 * Update TCIv3 by young
 * cert_id, content_type and signer_id_type change to security(SecurityContext)
 * flowIdentifier is added(OPTIONAL)
 * transmitPowerLevel, userPriority, channelIdentifier, dataRate and timeslot changed to OPTIONAL
 * 
 * @brief SetWsmTxInfo 메시지 형식
 */
struct Cvcoctci3SetWsmTxInfo
{
  struct {
    bool info_elements_included;
    bool dst_mac_addr;            // d8011/d16094 - 고정값 true
    bool expiry_time;             // d8011/d16094 - 고정값 false
    bool repeat_rate;             // d8011/d16094 - 고정값 false
    bool transmit_power_level;
    bool user_priority;
    bool chan_id;
    bool datarate;
    bool timeslot;
    bool flow_id;
  } options;

  Cvcoctci3Psid psid;
  struct Cvcoctci3RadioInterface radio;

  /* Cvcoctci3SecurityContext으로 변경 */
  struct Cvcoctci3SecurityContext security;

  Cvcoctci3ChannelNumber chan_id;
  Cvcoctci3TimeSlot timeslot;
  Cvcoctci3DataRate datarate;
  Cvcoctci3TxPower transmit_power_level;
  Cvcoctci3UserPriority user_priority;

  uint8_t dst_mac_addr[MAC_ALEN]; // d8011/d16094 - 고정값 FF:FF:FF:FF:FF:FF
  Cvcoctci3Time64 expiry_time;
  Cvcoctci3RepeatRate repeat_rate;

  struct Cvcoctci3WaveElementsIncluded info_elements_included;
  Cvcoctci3FlowIdentifier flow_id;
};
/// struct @ref Cvcoctci3SetWsmTxInfo 초기화 매크로
#define CVCOCTCI3_SET_WSM_TX_INFO_INITIALIZER {\
  .options = { false, false, false, false, false, false, false, false, false, false },\
  .psid = kCvcoctci3Psid_NA,\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
  .security = CVCOCTCI3_SECURITY_CONTEXT_INITIALIZER,\
  .chan_id = kCvcoctci3ChannelNumber_NA,\
  .timeslot = kCvcoctci3TimeSlot_NA,\
  .datarate = kCvcoctci3DataRate_NA,\
  .transmit_power_level = kCvcoctci3TxPower_NA,\
  .user_priority = kCvcoctci3UserPriority_NA,\
  .dst_mac_addr = CVCOCTCI3_MAC_ADDR_INITIALIZER,\
  .expiry_time = 0,\
  .repeat_rate = kCvcoctci3RepeatRate_NA,\
  .info_elements_included = CVCOCTCI3_WAVE_ELEMENTS_INCLUDED_INITIALIZER,\
  .flow_id = kCvcoctci3FlowIdentifier_NA,\
}


/**
 * Update TCIv3 by young
 * repeat_rate changed to OPTIONAL
 * 
 * @brief StartWsmTx 메시지 형식
 */
struct Cvcoctci3StartWsmTx
{
  struct {
    bool repeat_rate;
  } options;
  Cvcoctci3Psid psid;
  struct Cvcoctci3RadioInterface radio;
  Cvcoctci3RepeatRate repeat_rate;
};
/// struct @ref Cvcoctci3StartWsmTx 초기화 매크로
#define CVCOCTCI3_START_WSM_TX_INITIALIZER {\
  .options = { false },\
  .psid = kCvcoctci3Psid_NA,\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
  .repeat_rate = kCvcoctci3RepeatRate_NA,\
}


/**
 * Update TCIv3 by young
 * Create Pc5StartWsmTx
 * 
 * @brief Pc5StartWsmTx 메시지 형식
 */
struct Cvcoctci3Pc5StartWsmTx
{
  struct Cvcoctci3RadioInterface radio;
  Cvcoctci3Psid psid;
  Cvcoctci3FlowIdentifier flow_id;
  Cvcoctci3RepeatRate repeat_interval;
  int packet_count;
};
/// struct @ref Cvcoctci3StartWsmTx 초기화 매크로
#define CVCOCTCI3_PC5_START_WSM_TX_INITIALIZER {\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
  .psid = kCvcoctci3Psid_NA,\
  .flow_id = kCvcoctci3FlowIdentifier_NA,\
  .repeat_interval = kCvcoctci3RepeatRate_NA,\
  .packet_count = -999,\
}


/**
 * @brief StopWsmTx 메시지 형식
 */
struct Cvcoctci3StopWsmTx
{
  Cvcoctci3Psid psid;
  struct Cvcoctci3RadioInterface radio;
};
/// struct @ref Cvcoctci3StopWsmTx 초기화 매크로
#define CVCOCTCI3_STOP_WSM_TX_INITIALIZER {\
  .psid = kCvcoctci3Psid_NA,\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
}

/**
 * Update TCIv3 by young
 * pdu_filter is added
 * 
 * @brief PduFilter
 */
struct Cvcoctci3PduFilter
{
#define PDU_FILTER_MAX_LEN 4 ///< PDU filter 최대 길이
  size_t len; ///< PDU filter 길이
  uint8_t buf[PDU_FILTER_MAX_LEN+1]; ///< PDU filter
};
/// struct @ref Cvcoctci3PduFilter 초기화 매크로
#define CVCOCTCI3_PDU_FILTER_INITIALIZER {\
  .len = 0,\
  .buf = {0,},\
}


/**
 * Update TCIv3 by young
 * chan_id and timeslot changed to OPTIONAL
 * dst_mac_addr, pdu_filter and ssp is added(OPTIONAL)
 * 
 * @brief StartWsmRx 메시지 형식
 */
struct Cvcoctci3StartWsmRx
{
  struct {
    bool psid; ///< PSID 옵션필드 존재 여부
    bool chan_id;
    bool timeslot;
    bool dst_mac_addr;
    bool pdu_filter;
    bool ssp;
  } options;
  Cvcoctci3Psid psid; ///< PSID
  struct Cvcoctci3RadioInterface radio; ///< 전송 라디오 인터페이스 정보
  Cvcoctci3ChannelNumber chan_id; ///< 전송 채널
  Cvcoctci3TimeSlot timeslot; ///< 전송 시간슬롯
  struct Cvcoctci3EventHandling event_handling; ///< 이벤트 핸들링
  uint8_t dst_mac_addr[MAC_ALEN]; ///< 대상 MAC 주소(LTE-V2X에 사용)
  struct Cvcoctci3PduFilter pdu_filter; ///< 패킷 필터링에 사용되는 고유한 옥텟
  struct Cvcoctci3SecurityPermission ssp;          ///< Service Specific Permission
};
/// struct @ref Cvcoctci3StartWsmRx 초기화 매크로
#define CVCOCTCI3_START_WSM_RX_INITIALIZER {\
  .options = { false, false, false, false, false, false },\
  .psid = kCvcoctci3Psid_NA,\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
  .chan_id = kCvcoctci3ChannelNumber_NA,\
  .timeslot = kCvcoctci3TimeSlot_NA,\
  .event_handling = CVCOCTCI3_EVENT_HANDLING_INITIALIZER,\
  .dst_mac_addr = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
  .pdu_filter = CVCOCTCI3_PDU_FILTER_INITIALIZER,\
  .ssp = CVCOCTCI3_SECURITY_PERMISSION_INITIALIZER,\
}


/**
 * @brief StopWsmRx 메시지 형식
 */
struct Cvcoctci3StopWsmRx
{
  struct {
    bool psid; ///< PSID 옵션필드 존재 여부
  } options;

  Cvcoctci3Psid psid; ///< PSID
  struct Cvcoctci3RadioInterface radio; ///< 전송 라디오 인터페이스 정보
};
/// struct @ref Cvcoctci3StopWsmRx 초기화 매크로
#define CVCOCTCI3_STOP_WSM_RX_INITIALIZER {\
  .options = { false },\
  .psid = kCvcoctci3Psid_NA,\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
}


/**
 * @brief Wsa Count Threshold
 */
enum eCvcoctci3WsaCountThreshold
{
  kCvcoctci3WsaCountThreshold_Min = 0,
  kCvcoctci3WsaCountThreshold_Max = 255,
  kCvcoctci3WsaCountThreshold_NA = -999,
};
typedef int Cvcoctci3WsaCountThreshold; ///< @ref eCvcoctci3WsaCountThreshold


/**
 * @brief Wsa Count threshold interval
 */
enum eCvcoctci3WsaCountThresholdInterval
{
  kCvcoctci3WsaCountThresholdInterval_Min = 0,
  kCvcoctci3WsaCountThresholdInterval_Max = 255,
  kCvcoctci3WsaCountThresholdInterval_NA = -999,
};
typedef int Cvcoctci3WsaCountThresholdInterval; ///< @ref eCvcoctci3WsaCountThresholdInterval



/**
 * @brief ProviderServiceContext
 */
struct Cvcoctci3Psc
{
#define PSC_MAX_LEN 31 ///< PSC 최대길이
  size_t len;
  uint8_t psc[PSC_MAX_LEN+1];
};
/// struct @ref Cvcoctci3Psc 초기화 매크로
#define CVCOCTCI3_PSC_INITIALIZER {\
  .len = 0,\
  .psc = {0,},\
}


/**
 * @brief ProtocolType
 */
enum eCvcoctci3ProtocolType
{
  kCvcoctci3ProtocolType_NA = -999,
};
typedef int Cvcoctci3ProtocolType; ///< @ref eCvcoctci3ProtocolType


/**
 * Update TCIv3 by young
 * Add the protocolType
 * 
 * @brief ServiceInfoExts 정보
 */
struct Cvcoctci3ServiceInfoExts
{
  struct {
    bool psc;
    bool ipv6_addr;
    bool service_port;
    bool provider_mac_addr;
    bool rcpi_threshold;
    bool wsa_cnt_threshold;
    bool wsa_cnt_threshold_interval;
    bool protocol_type;
  } options;

  struct Cvcoctci3Psc psc;
  uint8_t ipv6_addr[IPv6_ALEN];
  uint16_t service_port;
  uint8_t provider_mac_addr[MAC_ALEN];
  Cvcoctci3Rcpi rcpi_threshold;
  Cvcoctci3WsaCountThreshold wsa_cnt_threshold;
  Cvcoctci3WsaCountThresholdInterval wsa_cnt_threshold_interval;
  Cvcoctci3ProtocolType protocol_type;
};
/// struct @ref Cvcoctci3ServiceInfoExts 초기화 매크로
#define CVCOCTCI3_SERVICE_INFO_EXTS_INITIALIZER {\
  .options = {false, },\
  .psc = CVCOCTCI3_PSC_INITIALIZER,\
  .ipv6_addr = {0,},\
  .service_port = 0,\
  .provider_mac_addr = {0, },\
  .rcpi_threshold = kCvcoctci3Rcpi_NA,\
  .wsa_cnt_threshold = kCvcoctci3WsaCountThreshold_NA,\
  .wsa_cnt_threshold_interavl = kCvcoctci3WsaCountThresholdInterval_NA,\
  .protocol_type = 0,\
}


/**
 * Update TCIv3 by young
 * Remove the channelNumber
 * Add the channelIndex
 * 
 * @brief WSA 에 수납되는 ServiceInfo 정보
 */
struct Cvcoctci3ServiceInfo
{
  struct {
    bool exts;
  } options;

  Cvcoctci3Psid psid;
  Cvcoctci3ChannelIndex chan_idx;
  struct Cvcoctci3ServiceInfoExts exts;
};
/// struct @ref Cvcoctci3ServiceInfo 초기화 매크로
#define CVCOCTCI3_SERVICE_INFO_INITIALIZER {\
  .options = {false},\
  .psid = kCvcoctci3Psid_NA,\
  .chan_num = kCvcoctci3ChannelNumber_NA,\
  .chan_idx = kCvcoctci3ChannelIndex_NA,\
  .exts = CVCOCTCI3_SERVICE_INFO_EXTS_INITIALIZER,\
}


/**
 * @brief WSA 수납되는 ServiceInfo 정보들
 */
struct Cvcoctci3ServiceInfos
{
  size_t cnt;
  struct Cvcoctci3ServiceInfo info[WSA_SERVICE_INFO_MAX_NUM];
};
/// struct @ref Cvcoctci3ServiceInfos 초기화 매크로
#define CVCOCTCI3_SERVICE_INFOS_INITIALIZER {\
  .cnt = 0,\
  .info = {CVCOCTCI3_SERVICE_INFO_INITIALIZER,},\
}


/**
 * @brief Operating class
 */
enum eCvcoctci3OperatingClass
{
  kCvcoctci3OperatingClass_Min = 0,
  kCvcoctci3OperatingClass_Max = 255,
  kCvcoctci3OperatingClass_NA = -999,
};
typedef int Cvcoctci3OperatingClass; ///< @ref eCvcoctci3OperatingClass


/**
 * @brief ChannelAccess80211
 */
enum eCvcoctci3ChannelAccess
{
  kCvcoctci3ChannelAccess_Continuous = 0,
  kCvcoctci3ChannelAccess_AlternatingTimeSlot0 = 1,
  kCvcoctci3ChannelAccess_AlternatingTimeSlot1 = 2,
  kCvcoctci3ChannelAccess_Min = 0,
  kCvcoctci3ChannelAccess_Max = 255,
  kCvcoctci3ChannelAccess_NA = -999
};
typedef int Cvcoctci3ChannelAccess; ///< @ref eCvcoctci3ChannelAccess


/**
 * @brief EdcaParameterRecord
 */
struct Cvcoctci3EdcaParameterRecord
{
  int res;          // 0..1
  int aci;          // 0..3
  int acm;          // 0..1
  int aifsn;        // 0..15
  int ecwmax;      // 0..15
  int ecwmin;      // 0..15
  int txoplimit;   // 0..65535
};
/// struct @ref Cvcoctci3EdcaParameterRecord 초기화 매크로
#define CVCOCTCI3_EDCA_PARAMETER_RECORD_INITIALIZER(x) {\
  .res = 0,\
  .aci = x,\
  .acm = 0,\
  .aifsn = 0,\
  .e_cwmax = 0,\
  .e_cwmin = 0,\
  .txop_limit = 0,\
}

/**
 * @brief EdcaParameterSet
 */
struct Cvcoctci3EdcaParameterSet
{
  struct Cvcoctci3EdcaParameterRecord ac_be;
  struct Cvcoctci3EdcaParameterRecord ac_bk;
  struct Cvcoctci3EdcaParameterRecord ac_vi;
  struct Cvcoctci3EdcaParameterRecord ac_vo;
};
/// struct @ref Cvcoctci3EdcaParameterRecord 초기화 매크로
#define CVCOCTCI3_EDCA_PARAMETER_SET {\
  .ac_be = CVCOCTCI3_EDCA_PARAMETER_RECORD_INITIALIZER(0),\
  .ac_bk = CVCOCTCI3_EDCA_PARAMETER_RECORD_INITIALIZER(1),\
  .ac_vi = CVCOCTCI3_EDCA_PARAMETER_RECORD_INITIALIZER(2),\
  .ac_vo = CVCOCTCI3_EDCA_PARAMETER_RECORD_INITIALIZER(3),\
}


/**
 * @brief ChannelInfo 내 확장필드 들
 */
struct Cvcoctci3ChannelInfoExts
{
  struct {
    bool edca_param_set;
    bool chan_access;
  } options;
  struct Cvcoctci3EdcaParameterSet edca_param_set;
  Cvcoctci3ChannelAccess chan_access;
};
/// struct @ref Cvcoctci3ChannelInfoExts 초기화 매크로
#define CVCOCTCI3_CHANNEL_INFO_EXTS_INITIALIZER {\
  .options = {false, },\
  .edca_param_set = CVCOCTCI3_EDCA_PARAMETER_SET,\
  .chan_access = kCvcoctci3ChannelAccess_NA,\
}


/**
 * @brief WSA 에 수납되는 ChannelInfo 정보
 */
struct Cvcoctci3ChannelInfo
{
  struct {
    bool exts;
  } options;

  Cvcoctci3OperatingClass op_class;
  Cvcoctci3ChannelNumber chan_num;
  Cvcoctci3TxPower power_level;
  bool adaptable_datarate;
  Cvcoctci3DataRate datarate;
  struct Cvcoctci3ChannelInfoExts exts;
};
/// struct @ref Cvcoctci3ChannelInfo 초기화 매크로
#define CVCOCTCI3_CHAN_INFO_INITIALIZER {\
  .op_class = kCvcoctci3OperatingClass_NA,\
  .chan_num = kCvcoctci3ChannelNumber_NA,\
  .power_level = kCvcoctci3TxPower_NA,\
  .adaptable_datarate = false,\
  .datarate = kCvcoctci3DataRate_NA,\
  .exts = CVCOCTCI3_CHANNEL_INFO_EXTS_INITIALIZER,\
}


/**
 * @brief WSA 에 수납되는 ChannelInfo 정보들
 */
struct Cvcoctci3ChannelInfos
{
  size_t cnt;
  struct Cvcoctci3ChannelInfo info[WSA_CHAN_INFO_MAX_NUM];
};
/// struct @ref Cvcoctci3ChannelInfos 초기화 매크로
#define CVCOCTCI3_CHAN_INFOS_INITIALIZER {\
  .cnt = 0,\
  .info = {CVCOCTCI3_CHAN_INFO_INITIALIZER,},\
}


/**
 * @brief Router lifetime
 */
enum eCvcoctci3RouterLifetime
{
  kCvcoctci3RouterLifetime_Min = 0,
  kCvcoctci3RouterLifetime_Max = 65535,
  kCvcoctci3RouterLifetime_NA = -999,
};
typedef int Cvcoctci3RouterLifetime; ///< @ref eCvcoctci3RouterLifetime


/**
 * @brief IP prefix length
 */
enum eCvcoctci3IPv6PrefixLength
{
  kCvcoctci3IPv6PrefixLength_Min = 0,
  kCvcoctci3IPv6PrefixLength_Max = 255,
  kCvcoctci3IPv6PrefixLength_NA = -999,
};
typedef int Cvcoctci3IPv6PrefixLength; ///< @ref eCvcoctci3IPv6PrefixLength


/**
 * @brief Routing advertisement 확장 정보
 */
struct Cvcoctci3RoutingAdvertisementExts
{
  struct {
    bool second_dns; ///< Secondary DNS 주소 옵션정보 수납 여부
    bool gw_mac_addr; ///< Gateway MAC 주소 옵션정보 수납 여부
  } options;
  uint8_t secondary_dns[IPv6_ALEN]; ///< Secondary DNS 주소
  uint8_t gw_mac_addr[MAC_ALEN]; ///< Gateway MAC 주소
};
/// struct @ref Cvcoctci3RoutingAdvertisementExts 초기화 매크로
#define CVCOCTCI3_ROUTING_ADVERTISEMENT_EXTS_INITIALIZER {\
  .options = {false, false},\
  .secondary_dns = CVCOCTCI3_IPv6_ADDR_INITIALIZER,\
  .gw_mac_addr = CVCOCTCI3_MAC_ADDR_INITIALIZER,\
}


/**
 * @brief WAVE Routing Advertisement
 */
struct Cvcoctci3RoutingAdvertisement
{
  struct {
    bool exts;
  } options;

  Cvcoctci3RouterLifetime router_lifetime;
  uint8_t ip_prefix[IPv6_ALEN];
  Cvcoctci3IPv6PrefixLength ip_prefix_len;
  uint8_t default_gw[IPv6_ALEN];
  uint8_t primary_dns[IPv6_ALEN];
  struct Cvcoctci3RoutingAdvertisementExts exts;
};
/// struct @ref Cvcoctci3RoutingAdvertisement 초기화 매크로
#define CVCOCTCI3_ROUTING_ADVERTISEMENT_INITIALIZER {\
  .options = {false,},\
  .router_lifetime = kCvcoctci3RouterLifetime_NA,\
  .ip_prefix = {0,},\
  .ip_prefix_len = kCvcoctci3IPv6PrefixLength_NA,\
  .default_gw = {0,},\
  .primary_dns = {0,},\
  .exts = {0,},\
}


/**
 * @brief AdvertiserId
 */
struct Cvcoctci3AdvertiserId
{
#define ADVERTISER_ID_MAX_LEN 32 ///< AdvertiserId 최대 길이
  size_t len; ///< AdvertiserId 길이
  uint8_t id[ADVERTISER_ID_MAX_LEN+1]; ///< AdvertiserId
};
/// struct @ref Cvcoctci3AdvertiserId 초기화 매크로
#define CVCOCTCI3_ADVERTISER_ID_INITIALIZER {\
  .len = 0,\
  .id = {0,},\
}


/**
 * Update TCIv3 by young
 * 
 * @brief 2D Location 구조체
 * */
struct Cvcoctci3TwoDLocation
{
  Cvcoctci3Latitude latitude;
  Cvcoctci3Longitude longitude;
};
/// struct @ref Cvcoctci3TwoDLocation 초기화 매크로
#define CVCOCTCI3_TWO_D_LOCATION_INITIALIZER {\
  .latitude = kCvcoctci3Latitude_NA,\
  .longitude = kCvcoctci3Longitude_NA,\
}


/**
 * Update TCIv3 by young
 * 
 * @brief 3D Location 구조체
 * */
struct Cvcoctci3ThreeDLocation
{
  Cvcoctci3Latitude latitude;
  Cvcoctci3Longitude longitude;
  Cvcoctci3Elevation elevation;
};
/// struct @ref Cvcoctci3ThreeDLocation 초기화 매크로
#define CVCOCTCI3_THREE_D_LOCATION_INITIALIZER {\
  .latitude = kCvcoctci3Latitude_NA,\
  .longitude = kCvcoctci3Longitude_NA,\
  .elevation = kCvcoctci3Elevation_NA,\
}


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info ARFCN-ValueEUTRA-r9
 */
enum eCvcoctci3Earfcn
{
  kCvcoctci3Earfcn_Min = 0,
  kCvcoctci3Earfcn_Max = 262143,
  kCvcoctci3Earfcn_NA = -999,
};
typedef int Cvcoctci3Earfcn; ///< @ref eCvcoctci3Earfcn


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info P-Max
 */
enum eCvcoctci3PMax
{
  kCvcoctci3PMax_Min = -30,
  kCvcoctci3PMax_Max = 33,
  kCvcoctci3PMax_NA = -999,
};
typedef int Cvcoctci3PMax; ///< @ref eCvcoctci3PMax


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info SL-Priority-r13
 */
enum eCvcoctci3SlPriority
{
  kCvcoctci3SlPriority_Min = 1,
  kCvcoctci3SlPriority_Max = 8,
  kCvcoctci3SlPriority_NA = -999,
};
typedef int Cvcoctci3SlPriority; ///< @ref eCvcoctci3SlPriority


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info SL-TxPoolIdentity-r12
 */
enum eCvcoctci3SlTxPoolIdentity
{
  kCvcoctci3SlTxPoolIdentity_Min = 1,
  kCvcoctci3SlTxPoolIdentity_Max = 8,
  kCvcoctci3SlTxPoolIdentity_NA = -999,
};
typedef int Cvcoctci3SlTxPoolIdentity; ///< @ref eCvcoctci3SlTxPoolIdentity


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info mcs
 */
enum eCvcoctci3Mcs
{
  kCvcoctci3Mcs_Min = 0,
  kCvcoctci3Mcs_Max = 31,
  kCvcoctci3Mcs_NA = -999,
};
typedef int Cvcoctci3Mcs; ///< @ref eCvcoctci3Mcs


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info pdb
 */
enum eCvcoctci3Pdb
{
  kCvcoctci3Pdb_Min = 0,
  kCvcoctci3Pdb_Max = 1023,
  kCvcoctci3Pdb_NA = -999,
};
typedef int Cvcoctci3Pdb; ///< @ref eCvcoctci3Pdb


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info mtu
 */
enum eCvcoctci3Mtu
{
  kCvcoctci3Mtu_Min = 1,
  kCvcoctci3Mtu_Max = 8192,
  kCvcoctci3Mtu_NA = -999,
};
typedef int Cvcoctci3Mtu; ///< @ref eCvcoctci3Mtu


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info bandhwidth per sec
 */
enum eCvcoctci3BwPerSec
{
  kCvcoctci3BwPerSec_Min = 1,
  kCvcoctci3BwPerSec_Max = 1000,
  kCvcoctci3BwPerSec_NA = -999,
};
typedef int Cvcoctci3BwPerSec; ///< @ref eCvcoctci3BwPerSec


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info bandwidth multiplier
 * */
enum eCvcoctci3BandwidthMultiplier
{
  kCvcoctci3BandwidthMultiplier_Bytes = 0,
  kCvcoctci3BandwidthMultiplier_KB,
  kCvcoctci3BandwidthMultiplier_MB,
  kCvcoctci3BandwidthMultiplier_GB,
  kCvcoctci3BandwidthMultiplier_NA = -999,
};
typedef int Cvcoctci3BandwidthMultiplier; ///< @ref eCvcoctci3BandwidthMultiplier


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info Bandwidth per sec 구조체
 * */
struct Cvcoctci3BandwidthPerSec
{
  Cvcoctci3BwPerSec number;
  Cvcoctci3BandwidthMultiplier multiplier;
};
/// struct @ref Cvcoctci3BandwidthPerSec 초기화 매크로
#define CVCOCTCI3_BANDWIDTH_INITIALIZER {\
  .number = kCvcoctci3BwPerSec_NA,\
  .Cvcoctci3BandwidthMultiplier = kCvcoctci3BandwidthMultiplier_NA,\
}


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info periodicity
 * */
enum eCvcoctci3Periodicity
{
  kCvcoctci3Periodicity_SF20 = 0,
  kCvcoctci3Periodicity_SF50,
  kCvcoctci3Periodicity_SF100,
  kCvcoctci3Periodicity_SF200,
  kCvcoctci3Periodicity_SF300,
  kCvcoctci3Periodicity_SF400,
  kCvcoctci3Periodicity_SF500,
  kCvcoctci3Periodicity_SF600,
  kCvcoctci3Periodicity_SF700,
  kCvcoctci3Periodicity_SF800,
  kCvcoctci3Periodicity_SF900,
  kCvcoctci3Periodicity_SF1000,
  kCvcoctci3Periodicity_NA = -999,
  
};
typedef int Cvcoctci3Periodicity; ///< @ref eCvcoctci3Periodicity


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info max speed
 */
enum eCvcoctci3MaxSpeed
{
  kCvcoctci3MaxSpeed_Min = 0,
  kCvcoctci3MaxSpeed_Max = 255,
  kCvcoctci3MaxSpeed_NA = -999,
};
typedef int Cvcoctci3MaxSpeed; ///< @ref eCvcoctci3MaxSpeed


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info max range
 */
enum eCvcoctci3MaxRagne
{
  kCvcoctci3MaxRange_Min = 1,
  kCvcoctci3MaxRange_Max = 4096,
  kCvcoctci3MaxRange_NA = -999,
};
typedef int Cvcoctci3MaxRange; ///< @ref eCvcoctci3MaxRagne


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info max cbr
 */
enum eCvcoctci3MaxCbr
{
  kCvcoctci3MaxCbr_Min = 0,
  kCvcoctci3MaxCbr_Max = 100,
  kCvcoctci3MaxCbr_NA = -999,
};
typedef int Cvcoctci3MaxCbr; ///< @ref eCvcoctci3MaxCbr


/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info - pre configuration
 * */
struct Cvcoctci3PreConfiguration
{
  struct {
    bool anchor_carrier_req_list;
    bool cbr_preconfig_lst;
  } options;

};

/**
 * Update TCIv3 by young
 * 
 * @brief LTE V2X channel info(Cv2x)
 */
struct Cvcoctci3LTEV2xChannelInfo
{
  struct {
    bool tx_pool_earfcn;
    bool p_max;
    bool pppp;
    bool tx_pool;
    bool min_mcs;
    bool max_mcs;
    bool min_pdb;
    bool mtu;
    bool max_bw_per_sec;
    bool min_periodicity;
    bool max_speed;
    bool max_range;
    bool max_cbr;
    bool preconfig_info;
  } options;

  Cvcoctci3Earfcn tx_pool_earfcn;
  Cvcoctci3PMax p_max;
  Cvcoctci3SlPriority pppp;
  Cvcoctci3SlTxPoolIdentity tx_pool;
  Cvcoctci3Mcs min_mcs;
  Cvcoctci3Mcs max_mcs;
  Cvcoctci3Pdb min_pdb;
  Cvcoctci3Mtu mtu;
  struct Cvcoctci3BandwidthPerSec max_bw_per_sec;
  Cvcoctci3Periodicity min_periodicity;
  Cvcoctci3MaxSpeed max_speed;
  Cvcoctci3MaxRange max_range;
  Cvcoctci3MaxCbr max_cbr;
};
/// struct @ref Cvcoctci3LTEV2xChannelInfo 초기화 매크로
#define CVCOCTCI3_CV2X_CHAN_INFO_INITIALIZER {\
  .options = {false, false, false, false, false, false, false, false, false, false, false, false, false, false},\
  .tx_pool_earfcn = kCvcoctci3Earfcn_NA,\
  .p_max = kCvcoctci3PMax_NA,\
  .pppp = kCvcoctci3SlPriority_NA,\
  .tx_pool = kCvcoctci3SlTxPoolIdentity_NA,\
  .min_mcs = kCvcoctci3Mcs_NA,\
  .max_mcs = kCvcoctci3Mcs_NA,\
  .min_pdb = kCvcoctci3Pdb_NA,\
  .mtu = kCvcoctci3Mtu_NA,\
  .max_bw_per_sec = CVCOCTCI3_BANDWIDTH_INITIALIZER,\
  .min_periodicity = kCvcoctci3Periodicity_NA,\
  .max_speed = kCvcoctci3MaxSpeed_NA,\
  .max_range = kCvcoctci3MaxRange_NA,\
  .max_cbr = kCvcoctci3MaxCbr_NA,\
}


/**
 * Update TCIv3 by young
 * 
 * @brief WSA Extended channel info
 */
struct Cvcoctci3ExtendedChannelInfo
{
  struct {
    bool chan_info;
    bool cv2x_chan_info;
  } options;

  int id;
  struct Cvcoctci3ChannelInfo chan_info;
  struct Cvcoctci3LTEV2xChannelInfo cv2x_chan_info;
};
/// struct @ref Cvcoctci3ExtendedChannelInfo 초기화 매크로
#define CVCOCTCI3_EXTENDED_CHAN_INFO_INITIALIZER {\
  .options = {false, false},\
  .id = 0,\
  .chan_info = CVCOCTCI3_CHAN_INFO_INITIALIZER,\
  .cv2x_chan_info = CVCOCTCI3_CV2X_CHAN_INFO_INITIALIZER,\
}


/**
 * Update TCIv3 by young
 * 
 * @brief WSA Extended channel infos
 */
struct Cvcoctci3ExtendedChannelInfos
{
#define EXTENDED_CHANNEL_INFOS_MAX_LEN 65535
  size_t cnt;
  struct Cvcoctci3ExtendedChannelInfo extended_chan_info[EXTENDED_CHANNEL_INFOS_MAX_LEN];
};
/// struct @ref Cvcoctci3ExtendedChannelInfos 초기화 매크로
#define CVCOCTCI3_EXTENDED_CHAN_INFOS_INITIALIZER {\
  .cnt = 0,\
  .exts = {CVCOCTCI3_EXTENDED_CHAN_INFO_INITIALIZER,},\
}


/**
 * Update TCIv3 by young
 * 
 * @brief WSA header extension
 */
struct Cvcoctci3SrvAdvMsgHeaderExt
{
  struct {
    bool repeat_rate;
    bool twod_location;
    bool threed_location;
    bool advertiser_id;
    bool extended_chan_infos;
  } options;

  Cvcoctci3RepeatRate repeat_rate;
  struct Cvcoctci3TwoDLocation towd_location;
  struct Cvcoctci3ThreeDLocation threed_location;
  struct Cvcoctci3AdvertiserId advertiser_id;
  /* extended_chan_infos */
};
/// struct @ref Cvcoctci3SrvAdvMsgHeaderExt 초기화 매크로
#define CVCOCTCI3_SRV_ADV_MSG_HEADER_EXT_INITIALIZER {\
  .options = {false, false, false, false, false},\
  .repeat_rate = kCvcoctci3RepeatRate_NA,\
  .twod_location = CVCOCTCI3_TWO_D_LOCATION_INITIALIZER,\
  .threed_location = CVCOCTCI3_THREE_D_LOCATION_INITIALIZER,\
  .advertiser_id = CVCOCTCI3_ADVERTISER_ID_INITIALIZER,\
}


/**
 * Update TCIv3 by young
 * 
 * @brief WSA header extensions
 */
struct Cvcoctci3SrvAdvMsgHeaderExts
{
#define HEADER_EXTS_MAX_LEN 65535
  size_t cnt;
  struct Cvcoctci3SrvAdvMsgHeaderExt exts[HEADER_EXTS_MAX_LEN];
};
/// struct @ref Cvcoctci3SrvAdvMsgHeaderExts 초기화 매크로
#define CVCOCTCI3_SRV_ADV_MSG_HEADER_EXTS_INITIALIZER {\
  .cnt = 0,\
  .exts = {CVCOCTCI3_SRV_ADV_MSG_HEADER_EXT_INITIALIZER,},\
}


/**
 * Update TCIv3 by young
 * wsa_chan_id, chan_access, repeat_rate, service_infos and chan_infos change to OPTIONAL
 * header_exts, flow_id is added(OPTIONAL)
 * 
 * @brief StartWsaTxPeriodic 메시지 형식
 */
struct Cvcoctci3StartWsaTxPeriodic
{
  struct {
    bool dst_mac_addr;
    bool info_element_included;
    bool advertiser_id;
    bool wra;
    bool datarate;
    bool priority;
    bool transmit_power_level;
    bool wsa_chan_id;
    bool chan_access;
    bool repeat_rate;
    bool service_infos;
    bool chan_infos;
    bool header_exts;
    bool flow_id;
    /* headerexts */
  } options;
  
  struct Cvcoctci3RadioInterface radio;
  uint8_t dst_mac_addr[MAC_ALEN];
  Cvcoctci3ChannelNumber wsa_chan_id;
  Cvcoctci3TimeSlot chan_access;
  Cvcoctci3RepeatRate repeat_rate;
  bool ip_service;
  struct Cvcoctci3SecurityContext security;
  int signature_lifetime;
  struct Cvcoctci3WaveElementsIncluded info_element_included;
  struct Cvcoctci3AdvertiserId advertiser_id;
  struct Cvcoctci3ServiceInfos service_infos;
  struct Cvcoctci3ChannelInfos chan_infos;
  struct Cvcoctci3RoutingAdvertisement wra;
  Cvcoctci3DataRate datarate;
  Cvcoctci3UserPriority priority;
  Cvcoctci3TxPower transmit_power_level;
  // struct Cvcoctci3SrvAdvMsgHeaderExts header_exts;
  Cvcoctci3FlowIdentifier flow_id;
};
/// struct @ref Cvcoctci3StartWsaTxPeriodic 초기화 매크로
#define CVCOCTCI3_START_WSA_TX_PERIODIC_INITIALIZER = {\
  .options = {false,false,false,false,false,false,false,false,false,false,false,false,false,false},\
  .radio = kCvcoctci3Radio_NA,\
  .dst_mac_addr = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
  .wsa_chan_id = kCvcoctci3ChannelNumber_NA,\
  .chan_access = kCvcoctci3TimeSlot_NA,\
  .repeat_rate = kCvcoctci3RepeatRate_NA,\
  .ip_service = false,\
  .security = CVCOCTCI3_SECURITY_CONTEXT_INITIALIZER,\
  .signature_lifetime = 0,\
  .info_element_included = CVCOCTCI3_WAVE_ELEMENTS_INCLUDED_INITIALIZER,\
  .advertiser_id = CVCOCTCI3_ADVERTISER_ID_INITIALIZER,\
  .service_infos = CVCOCTCI3_SERVICE_INFOS_INITIALIZER,\
  .chan_infos = CVCOCTCI3_CHAN_INFOS_INITIALIZER,\
  .routing_advertisement = CVCOCTCI3_ROUTING_ADVERTISEMENT_INITIALIZER,\
  .datarate = kCvcoctci3DataRate_NA,\
  .priority = kCvcoctci3UserPriority_NA,\
  .transmit_power_level = kCvcoctci3TxPower_NA,\
  .header_exts = CVCOCTCI3_SRV_ADV_MSG_HEADER_EXTS_INITIALIZER,\
  .flow_id = kCvcoctci3FlowIdentifier_NA,\
}


/**
 * @brief StopWsaTxPeriodic 요청 데이터 형식
 */
struct Cvcoctci3StopWsaTxPeriodic
{
  struct Cvcoctci3RadioInterface radio;
};
/// struct @ref Cvcoctci3StopWsaTxPeriodic 초기화 매크로
#define CVCOCTCI3_STOP_WSA_TX_PERIODIC_INITIALIZER {\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
}


/**
 * @brief AddWsaProviderService 요청 데이터 형식
 */
struct Cvcoctci3AddWsaProviderService
{
  struct Cvcoctci3RadioInterface radio;
  struct Cvcoctci3ServiceInfos infos;
};
/// struct @ref Cvcoctci3AddWsaProviderService 초기화 매크로
#define CVCOCTCI3_ADD_WSA_PROVIDER_SERVICE_INITIALIZER {\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
  .info = CVCOCTCI3_SERVICE_INFOS_INITIALIZER,\
}


/**
 * @brief ChangeWsaProviderService 요청 데이터 형식
 */
struct Cvcoctci3ChangeWsaProviderService
{
  struct Cvcoctci3RadioInterface radio;
  struct Cvcoctci3ServiceInfos infos;
};
/// struct @ref Cvcoctci3ChangeWsaProviderService 초기화 매크로
#define CVCOCTCI3_CHANGE_WSA_PROVIDER_SERVICE_INITIALIZER {\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
  .info = CVCOCTCI3_SERVICE_INFOS_INITIALIZER,\
}


/**
 * @brief DelWsaProviderService 요청 데이터 형식
 */
struct Cvcoctci3DelWsaProviderService
{
  struct Cvcoctci3RadioInterface radio;
  struct Cvcoctci3ServiceInfos infos;
};
/// struct @ref Cvcoctci3DelWsaProviderService 초기화 매크로
#define CVCOCTCI3_DEL_WSA_PROVIDER_SERVICE_INITIALIZER {\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
  .info = CVCOCTCI3_SERVICE_INFOS_INITIALIZER,\
}


/**
 * @brief UserRequestType
 */
enum eCvcoctci3UserRequestType
{
  kCvcoctci3UserRequestType_AutoAccessOnMatch = 0, ///< 원하는 서비스 발견 시 자동 채널 접속 수행
  kCvcoctci3UserRequestType_NoSchAccess = 1, ///< 원하는 서비스 발견 시 자동 채널 접속 미수행
  kCvcoctci3UserRequestType_Min = kCvcoctci3UserRequestType_AutoAccessOnMatch,
  kCvcoctci3UserRequestType_Max = kCvcoctci3UserRequestType_NoSchAccess,
  kCvcoctci3UserRequestType_NA = -999,
};
typedef int Cvcoctci3UserRequestType; ///< @ref eCvcoctci3UserRequestType


/**
 * Update TCIv3 by young
 * Add anyWsa and modified kCvcoctci3WsaType_Max
 * 
 * @brief WsaType
 */
enum eCvcoctci3WsaType
{
  kCvcoctci3WsaType_SecureWsa = 1, ///< 서명 WSA
  kCvcoctci3WsaType_UnsecureWsa = 2, ///< 비보안 WSA
  kCvcoctci3WsaType_AnyWsa = 3,     ///< 모든 WSA
  kCvcoctci3WsaType_Min = kCvcoctci3WsaType_SecureWsa,
  kCvcoctci3WsaType_Max = kCvcoctci3WsaType_AnyWsa,
  kCvcoctci3WsaType_NA = -999,
};
typedef int Cvcoctci3WsaType; ///< @ref eCvcoctci3WsaType



/**
 * @brief Immediate access 를 수행하는 기간 (Sync interval 단위)
 *
 * 예: Immediate access = 3 -> 3 sync interval(보통 300msec) 동안 immediate access 수행
 */
enum eCvcoctci3ImmediateAccess
{
  kCvcoctci3ImmediateAccess_Min = 0,
  kCvcoctci3ImmediateAccess_Max = 255,
  kCvcoctci3ImmediateAccess_NotImmediateAccess = 0,
  kCvcoctci3ImmediateAccess_NA = -999,
};
typedef int Cvcoctci3ImmediateAccess; ///< @ref eCvcoctci3ImmediateAccess


/**
 * Update TCIv3 by young
 * Modified wsa_type(add anyWsa)
 * Add req_psid_in_sign_cert, ssp
 * 
 * @brief AddUserService 요청 데이터 형식
 */
struct Cvcoctci3AddUserService
{
  /// 각 옵션 필드의 설정 여부
  struct {
    bool psc;
    bool chan_id;
    bool src_mac_addr;
    bool advertiser_id;
    bool link_quality;
    bool immediate_access;
    bool wsa_chan_id;
    bool chan_access;
    bool req_psid_in_sign_cert;
    bool ssp;
  } options;

  Cvcoctci3Psid psid;
  struct Cvcoctci3RadioInterface radio;
  Cvcoctci3UserRequestType req_type;
  Cvcoctci3WsaType wsa_type;
  struct Cvcoctci3Psc psc;
  Cvcoctci3ChannelNumber chan_id;
  uint8_t src_mac_addr[MAC_ALEN];
  struct Cvcoctci3AdvertiserId advertiser_id;
  int link_quality;
  Cvcoctci3ImmediateAccess immediate_access;
  Cvcoctci3ChannelNumber wsa_chan_id;
  Cvcoctci3TimeSlot chan_access;
  struct Cvcoctci3EventHandling event_handling;
  bool req_psid_in_sign_cert;
  struct Cvcoctci3SecurityPermission ssp;
};
/// struct @ref Cvcoctci3AddUserService 초기화 매크로
#define CVCOCTCI3_ADD_USER_SERVICE_INITIALIZER {\
  .options = {false,false,false,false,false,false,false,false,false,false},\
  .psid = kCvcoctci3Psid_NA,\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
  .req_type = kCvcoctci3UserRequestType_NA,\
  .wsa_type = kCvcoctci3WsaType_NA,\
  .psc = CVCOCTCI3_PSC_INITIALIZER,\
  .chan_id = kCvcoctci3ChannelNumber_NA,\
  .src_mac_addr =  CVCOCTCI3_MAC_ADDR_INITIALIZER,\
  .advertiser_id = CVCOCTCI3_ADVERTISER_ID_INITIALIZER,\
  .link_quality = -999,\
  .immediate_access = kCvcoctci3ImmediateAccess_NA,\
  .wsa_chan_id = kCvcoctci3ChannelNumber_NA,\
  .chan_access = kCvcoctci3TimeSlot_NA,\
  .event_handling = CVCOCTCI3_EVENT_HANDLING_INITIALIZER,\
  .req_psid_in_sign_cert = false,\
  .ssp = CVCOCTCI3_SECURITY_PERMISSION_INITIALIZER,\
}


/**
 * @brief DelUserService 요청 데이터 형식
 */
struct Cvcoctci3DelUserService
{
  Cvcoctci3Psid psid; ///< PSID
  struct Cvcoctci3RadioInterface radio; ///< 라디오 인터페이스
};
/// struct @ref Cvcoctci3DelUserService 초기화 매크로
#define CVCOCTCI3_DEL_USER_SERVICE_INITIALIZER {\
  .psid = kCvcoctci3Psid_NA,\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
}


/**
 * @brief GetIPv6InterfaceInfo 요청 데이터 형식
 */
struct Cvcoctci3GetIPv6InterfaceInfo
{
  struct Cvcoctci3RadioInterface radio; ///< 라디오 인터페이스
};
/// struct @ref Cvcoctci3GetIPv6InterfaceInfo 초기화 매크로
#define CVCOCTCI3_GET_IPv6_INTERFACE_INFO_INITIALIZER {\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
}


/**
 * @brief InterfaceName
 */
struct Cvcoctci3InterfaceName
{
#define CVCOCTCI3_IF_NAME_MAX_LEN 255 ///< 인터페이스 이름 최대 길이
  size_t len; ///< 인터페이스 이름 길이
  uint8_t name[CVCOCTCI3_IF_NAME_MAX_LEN+1]; ///< 인터페이스 이름
};
/// struct @ref Cvcoctci3InterfaceName 초기화 매크로
#define CVCOCTCI3_IF_NAME_INITIALIZER {\
  .len = 0,\
  .name = {0,},\
}


/**
 * @brief SetIPv6Address 요청 데이터 형식
 */
struct Cvcoctci3SetIPv6Address
{
  struct {
    bool ip_addr;
  } options;
  struct Cvcoctci3RadioInterface radio;
  struct Cvcoctci3InterfaceName if_name;
  uint8_t ip_addr[IPv6_ALEN];
};
/// struct @ref Cvcoctci3SetIPv6Address 초기화 매크로
#define CVCOCTCI3_SET_IPv6_ADDRESS_INITIALIZER {\
  .options = {false},\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
  .if_name = CVCOCTCI3_IF_NAME_INITIALIZER,\
  .ip_addr = CVCOCTCI3_IPv6_ADDR_INITIALIZER,\
}


/**
 * @brief Protocol
 */
enum eCvcoctci3Protocol
{
  kCvcoctci3Protocol_tcp = 0,
  kCvcoctci3Protocol_udp = 1,
  kCvcoctci3Protocol_icmpv6 = 2,
  kCvcoctci3Protocol_NA = -999,
};
typedef int Cvcoctci3Protocol; ///< @ref eCvcoctci3Protocol


/**
 * @brief IPv6TxRecord
 */
struct Cvcoctci3IPv6TxRecord
{
  struct {
    bool dst_port;
    bool repeat_rate;
    bool event_handling;
  } options;

  struct Cvcoctci3RadioInterface radio;
  struct Cvcoctci3InterfaceName if_name;
  uint8_t dst_ip_addr[IPv6_ALEN];
  uint16_t dst_port;
  Cvcoctci3Protocol protocol;
  Cvcoctci3RepeatRate repeat_rate;
  struct Cvcoctci3EventHandling event_handling;
};
/// Cvcoctci3StartIPv6Ping 정보
#define Cvcoctci3StartIPv6Ping Cvcoctci3IPv6TxRecord
/// Cvcoctci3StopIPv6Ping 정보
#define Cvcoctci3StopIPv6Ping Cvcoctci3IPv6TxRecord
/// Cvcoctci3StartIPv6Tx 정보
#define Cvcoctci3StartIPv6Tx Cvcoctci3IPv6TxRecord
/// Cvcoctci3StopIPv6Tx 정보
#define Cvcoctci3StopIPv6Tx Cvcoctci3IPv6TxRecord


/**
 * @brief IPv6RxRecord
 */
struct Cvcoctci3IPv6RxRecord
{
  struct {
    bool event_handling;
  } options;

  struct Cvcoctci3RadioInterface radio;
  struct Cvcoctci3InterfaceName if_name;
  uint16_t listen_port;
  Cvcoctci3Protocol protocol;
  struct Cvcoctci3EventHandling event_handling;
};
/// Cvcoctci3StartIPv6Rx 정보
#define Cvcoctci3StartIPv6Rx Cvcoctci3IPv6RxRecord
/// Cvcoctci3StopIPv6Rx 정보
#define Cvcoctci3StopIPv6Rx Cvcoctci3IPv6RxRecord


/**
 * @brief OperationalRateSet
 */
struct Cvcoctci3OperationalRateSet
{
#define CVCOCTCI3_OPERATIONAL_RATE_SET_MAX_LEN 50
  size_t len;
  uint8_t op_rate_set[CVCOCTCI3_OPERATIONAL_RATE_SET_MAX_LEN];
};


/**
 * @brief AddTxProfile
 */
struct Cvcoctci3AddTxProfile
{
  struct {
    bool op_rate_set;
    bool edca_param_set;
    bool immediate_access;
  } options;

  struct Cvcoctci3RadioInterface radio;
  Cvcoctci3ChannelNumber chan_id;
  Cvcoctci3TimeSlot timeslot;
  struct Cvcoctci3OperationalRateSet op_rate_set;
  struct Cvcoctci3EdcaParameterSet edca_param_set;
  Cvcoctci3ImmediateAccess immediate_access;
  bool adaptable;
  Cvcoctci3TxPower transmit_power_level;
  Cvcoctci3DataRate datarate;
};


/**
 * @brief DelTxProfile
 */
struct Cvcoctci3DelTxProfile
{
  struct Cvcoctci3RadioInterface radio;
  Cvcoctci3ChannelNumber chan_id;
};


/**
 * @brief StartBsmTx 요청 데이터 형식
 */
struct Cvcoctci3StartBsmTx
{
  struct {
    bool repeat_rate;
  } options;
  Cvcoctci3Psid psid;
  struct Cvcoctci3RadioInterface radio;
  Cvcoctci3RepeatRate repeat_rate;
};


/**
 * @brief StopBsmTx 요청 데이터 형식
 */
struct Cvcoctci3StopBsmTx
{
  Cvcoctci3Psid psid;
  struct Cvcoctci3RadioInterface radio;
};


/**
 * @brief StartBsmRx 메시지 형식
 */
struct Cvcoctci3StartBsmRx
{
  struct {
    bool psid;
  } options;

  Cvcoctci3Psid psid;
  struct Cvcoctci3RadioInterface radio;
  Cvcoctci3ChannelNumber chan_id;    // = 172
  Cvcoctci3TimeSlot timeslot;    // = continuous
  struct Cvcoctci3EventHandling event_handling;
};


/**
 * @brief StopBsmRx 요청 데이터 형식
 */
struct Cvcoctci3StopBsmRx
{
  struct {
    bool psid;
  } options;

  Cvcoctci3Psid psid;    // =32
  struct Cvcoctci3RadioInterface radio;
};


/**
 * @brief ResultCode
 */
enum eCvcoctci3TciResultCode
{
  kCvcoctci3TciResultCode_Success = 0,
  kCvcoctci3TciResultCode_Failure = 1,
  kCvcoctci3TciResultCode_Min = kCvcoctci3TciResultCode_Success,
  kCvcoctci3TciResultCode_Max = kCvcoctci3TciResultCode_Failure,
};
typedef int Cvcoctci3TciResultCode; ///< @ref eCvcoctci3TciResultCode


/**
 * @brief ExceptionType
 */
enum eCvcoctci3ExceptionType
{
  kCvcoctci3ExceptionType_Info = 0,
  kCvcoctci3ExceptionType_Warning = 1,
  kCvcoctci3ExceptionType_Error = 2,
  kCvcoctci3ExceptionType_Min = kCvcoctci3ExceptionType_Info,
  kCvcoctci3ExceptionType_Max = kCvcoctci3ExceptionType_Error,
  kCvcoctci3ExceptionType_NA = -999,
};
typedef int Cvcoctci3ExceptionType; ///< @ref eCvcoctci3ExceptionType


/**
 * @brief ExceptionId
 */
enum eCvcoctci3ExceptionId
{
  kCvcoctci3ExceptionId_CriticalError = 1,
  kCvcoctci3ExceptionId_IncorrectParameterValue = 2,
  kCvcoctci3ExceptionId_MissingParameter = 3,
  kCvcoctci3ExceptionId_RadioInterfaceUnavailable = 4,
  kCvcoctci3ExceptionId_Min = kCvcoctci3ExceptionId_CriticalError,
  kCvcoctci3ExceptionId_Max = kCvcoctci3ExceptionId_RadioInterfaceUnavailable,
  kCvcoctci3ExceptionId_NA = -999,
};
typedef int Cvcoctci3ExceptionId; ///< @ref eCvcoctci3ExceptionId


/**
 * @brief Exception 메시지 형식
 */
struct Cvcoctci3Exception
{
  struct {
    bool id;
    bool module;
    bool description;
  } options;  ///< 각 옵션 필드 설정 여부

  Cvcoctci3ExceptionType type;  ///< 예외 유형
  Cvcoctci3ExceptionId id;      ///< 예외 식별자
  const char *module;          ///< module 문자열을 가리키는 포인터
#define EXCEPTION_MODULE_MAX_SIZE 255
  size_t module_size;          ///< module 길이 (0~255)
  const char *description;     ///< description 문자열을 가리키는 포인터
#define EXCEPTION_DESC_MAX_SIZE 1200
  size_t description_size;     ///< description 길이 (0~1200)
};
/// struct @ref Cvcoctci3Exception 초기화 매크로
#define CVCOCTCI3_EXCEPTION_INITIALIZER {\
  .options = { false, false, false },\
  .type = kCvcoctci3ExceptionType_NA,\
  .id = kCvcoctci3ExceptionId_NA,\
  .module = NULL,\
  .module_size = 0,\
  .description = NULL,\
  .description_size = 0,\
}


/**
 * @brief Event
 */
enum eCvcoctci3Event
{
  kCvcoctci3Event_80211PktRx = 1,
  kCvcoctci3Event_16093PktRx = 2,
  kCvcoctci3Event_WsmPktRx = 3,
  kCvcoctci3Event_IPv6PktRx = 4,
  kCvcoctci3Event_Icmp6PktRx = 5,
  kCvcoctci3Event_IPv6ConfigChanged = 6,
  kCvcoctci3Event_Dot3ChannelAssigned = 7,
  kCvcoctci3Event_Dot3RequestMatchedAvailAppService = 8,
  kCvcoctci3Event_Dot2VerificationCompleteWithResult = 9,
  kCvcoctci3Event_Exception = 15,
  kCvcoctci3Event_NA = -999,
};
typedef int Cvcoctci3Event; ///< @ref eCvcoctci3Event


/**
 * Update TCIv3 by young
 * kCvcoctci3EventParamsType_80211frame rename to kCvcoctci3EventParamsType_radioframe
 * 
 * @brief EventParams 유형 (Choice tag)
 */
enum eCvcoctci3EventParamsType
{
  kCvcoctci3EventParamsType_Service = 0,
  kCvcoctci3EventParamsType_Wsm = 1,
  kCvcoctci3EventParamsType_Ip = 2,
  kCvcoctci3EventParamsType_radioframe = 3,
  kCvcoctci3EventParamsType_Security = 4,
  kCvcoctci3EventParamsType_NA = -999,
};
typedef int Cvcoctci3EventParamsType; ///< @ref eCvcoctci3EventParamsType


/**
 * @brief Pdu
 */
struct Cvcoctci3Pdu
{
  Cvcoctci3PduType pdu_type;
  uint8_t *pdu_data;          ///< PDU 데이터를 가리키는 포인터
  size_t pdu_data_size;       ///< PDU 데이터 크기 (0~2304) @ref MAX_PDU_SIZE
};
/// struct @ref Cvcoctci3Pdu 초기화 매크로
#define CVCOCTCI3_PDU_INITIALIZER {\
  .pdu_type = kCvcoctci3PduType_NA,\
  .pdu_data = NULL,\
  .pdu_data_size = 0,\
}


/**
 * @brief ServiceParameters
 */
struct Cvcoctci3ServiceParameters
{
#define PSID_MAX_CNT 255
  unsigned int psid_cnt; ///< Psid 개수
  Cvcoctci3Psid psid[PSID_MAX_CNT]; ///< Psid 들
};
/// struct @ref Cvcoctci3ServiceParameters 초기화 매크로
#define CVCOCTCI3_SERVICE_PARAMETERS_INITIALIZER {\
  .psid_cnt = 0,\
  .psid = {kCvcoctci3Psid_NA,},\
}

/**
 * @brief WsmParameters
 * @date 2021-01-12
 * @details Update TCI version3 by young
 * @details Add options of chan_id, datarate, rx_power_level, src_mac_addr, rssi
 * @details Add rssi parameter
 * @details Add init macro of options, rssi
 */
struct Cvcoctci3WsmParameters
{
  struct {                    // Update TCIv3 by young
    bool chan_id;             // Update TCIv3 by young
    bool datarate;            // Update TCIv3 by young
    bool rx_power_level;      // Update TCIv3 by young
    bool src_mac_addr;        // Update TCIv3 by young
    bool rssi;                // Update TCIv3 by young
  } options;  ///< 옵션 필드 수납 여부

  Cvcoctci3Psid psid;                ///< 수신된 WSM 의 Psid
  Cvcoctci3Radio radio;              ///< 수신된 라디오 인터페이스
  uint8_t wsmp_version;             ///< 수신된 WSMP 의 버전 (0~15)
  Cvcoctci3ChannelNumber chan_id;    ///< 수신된 채널
  Cvcoctci3DataRate datarate;        ///< 수신된 데이터레이트
  Cvcoctci3TxPower rx_power_level;   ///< 수신파워
  uint8_t src_mac_addr[MAC_ALEN];   ///< 송신지 MAC 주소
  Cvcoctci3Rcpi rssi;                ///< RSSI, Update TCIv3 by young
};
/// struct @ref Cvcoctci3WsmParameters 초기화 매크로
// Update TCIv3 by young
#define CVCOCTCI3_WSM_PARAMETERS_INITIALIZER {\
  .options = {false, false, false, false, false},\
  .psid = kCvcoctci3Psid_NA,\
  .radio = kCvcoctci3Radio_NA,\
  .wsmp_version = 0,\
  .chan_id = kCvcoctci3ChannelNumber_NA,\
  .datarate = kCvcoctci3DataRate_NA,\
  .rx_power_level = kCvcoctci3TxPower_NA,\
  .src_mac_addr = CVCOCTCI3_MAC_ADDR_INITIALIZER,\
  .rssi = kCvcoctci3Rcpi_NA,\
}


/**
 * @brief IpParameters
 */
struct Cvcoctci3IpParameters
{
#define IF_NAME_MAX_SIZE 255
  char if_name[IF_NAME_MAX_SIZE+1];        ///< 수신 인터페이스 이름
  uint8_t src_ip_addr[IPv6_ALEN];          ///< 송신지 IP 주소
  Cvcoctci3Protocol protocol;               ///< 수신된 IP 패킷의 상위 프로토콜
};
/// struct @ref Cvcoctci3IpParameters 초기화 매크로
#define CVCOCTCI3_IP_PARAMETERS_INITIALIZER {\
  .if_name = {0, },\
  .src_ip_addr = CVCOCTCI3_IPv6_ADDR_INITIALIZER\
  .protocol = kCvcoctci3Protocol_NA,\
}


/**
 * @brief Dot11Parameters
 * @date 2022-01-12
 * @details Update TCIv3 by young
 * @details Cvcoctci3D80211Parameters rename to Cvcoctci3RadioParameters
 */
struct Cvcoctci3RadioParameters
{
  Cvcoctci3Radio radio;    ///< 802.11 프레임이 수신된 라디오 인터페이스 번호
  Cvcoctci3Rcpi rcpi;      ///< RCPI
};
/// struct @ref Cvcoctci3RadioParameters 초기화 매크로
#define CVCOCTCI3_D80211_PARAMETERS_INITIALIZER {\
  .radio = kCvcoctci3Radio_NA,\
  .rcpi = kCvcoctci3Rcpi_NA,\
}


/**
 * @brief SecurityResultCode
 */
enum eCvcoctci3SecurityResultCode
{
  kCvcoctci3SecurityResultCode_Success = 1,
  kCvcoctci3SecurityResultCode_InconsistentInputParameters,
  kCvcoctci3SecurityResultCode_SpduParsingInvalidInput,
  kCvcoctci3SecurityResultCode_SpduParsingUnsupportedCriticalInformationField,
  kCvcoctci3SecurityResultCode_SpduParsingCertificateNotFound,
  kCvcoctci3SecurityResultCode_SpduParsingGenerationTimeNotAvailable,
  kCvcoctci3SecurityResultCode_SpduParsingGenerationLocationNotAvailable,
  kCvcoctci3SecurityResultCode_SpduCertificateChainNotEnoughInformationToConstructChain,
  kCvcoctci3SecurityResultCode_SpduCertificateChainChainEndedAtUntrustedRoot,
  kCvcoctci3SecurityResultCode_SpduCertificateChainChainWasTooLongForImplementation,
  kCvcoctci3SecurityResultCode_SpduCertificateChainCertificateRevoked,
  kCvcoctci3SecurityResultCode_SpduCertificateChainOverdueCRL,
  kCvcoctci3SecurityResultCode_SpduCertificateChainInconsistentExpiryTimes,
  kCvcoctci3SecurityResultCode_SpduCertificateChainInconsistentStartTimes,
  kCvcoctci3SecurityResultCode_SpduCertificateChainInconsistentChainPermissions,
  kCvcoctci3SecurityResultCode_SpduCryptoVerificationFailure,
  kCvcoctci3SecurityResultCode_SpduConsistencyFutureCertificateAtGenerationTime,
  kCvcoctci3SecurityResultCode_SpduConsistencyExpiredCertificateAtGenerationTime,
  kCvcoctci3SecurityResultCode_SpduConsistencyExpiryDateTooEarly,
  kCvcoctci3SecurityResultCode_SpduConsistencyExpiryDateTooLate,
  kCvcoctci3SecurityResultCode_SpduConsistencyGenerationLocationOutsideValidityRegion,
  kCvcoctci3SecurityResultCode_SpduConsistencyNoGenerationLocation,
  kCvcoctci3SecurityResultCode_SpduConsistencyUnauthorizedPSID,
  kCvcoctci3SecurityResultCode_SpduInternalConsistencyExpiryTimeBeforeGenerationTime,
  kCvcoctci3SecurityResultCode_SpduInternalConsistencyextDataHashDoesntMatch,
  kCvcoctci3SecurityResultCode_SpduInternalConsistencynoExtDataHashProvided,
  kCvcoctci3SecurityResultCode_SpduInternalConsistencynoExtDataHashPresent,
  kCvcoctci3SecurityResultCode_SpduLocalConsistencyPSIDsDontMatch,
  kCvcoctci3SecurityResultCode_SpduLocalConsistencyChainWasTooLongForSDEE,
  kCvcoctci3SecurityResultCode_SpduRelevanceGenerationTimeTooFarInPast,
  kCvcoctci3SecurityResultCode_SpduRelevanceGenerationTimeTooFarInFuture,
  kCvcoctci3SecurityResultCode_SpduRelevanceExpiryTimeInPast,
  kCvcoctci3SecurityResultCode_SpduRelevanceGenerationLocationTooDistant,
  kCvcoctci3SecurityResultCode_SpduRelevanceReplayedSpdu,
  kCvcoctci3SecurityResultCode_SpduCertificateExpired,
  kCvcoctci3SecurityResultCode_NA = -999,
};
typedef int Cvcoctci3SecurityResultCode; ///< @ref eCvcoctci3SecurityResultCode


/**
 * @brief SecResultParams
 */
struct Cvcoctci3SecResultParams
{
  struct {
    bool description;
  } options;  ///< 옵션 필드 수납 여부

  /**
   * Update TCIv3 by young
   * Cvcoctci3SecurityResultCode changed to Cvcoctci3TciResultCode
   * */
  Cvcoctci3TciResultCode sec_result_code; ///< 보안처리 결과
#define SEC_RESULT_DESCRIPTION_MAX_SIZE 255
  char description[SEC_RESULT_DESCRIPTION_MAX_SIZE+1]; ///< 설명 문자열을 가리키는 포인터
};
/// struct @ref Cvcoctci3SecResultParams 초기화 매크로
#define CVCOCTCI3_SEC_RESULT_PARAMS_INITIALIZER {\
  .options = {false},\
  .sec_result_code = kCvcoctci3SecurityResultCode_NA,\
  .description = {0},\
}


/**
 * @brief InfoContent 유형 (Choice tag)
 */
enum eCvcoctci3InfoContentType
{
  kCvcoctci3InfoContentType_IPv6InterfaceInfo,
  kCvcoctci3InfoContentType_SutInfo,
  kCvcoctci3InfoContentType_ATcmdInfo,
  kCvcoctci3InfoContentType_PacketCount,
  kCvcoctci3InfoContentType_SutStatus,
  kCvcoctci3InfoContentType_NA = -999,
};
typedef int Cvcoctci3InfoContentType; ///< @ref eCvcoctci3InfoContentType


/**
 * @brief IPv6Address 리스트
 */
struct Cvcoctci3IPv6AddressList
{
#define IF_IPv6_ADDR_MAX_NUM 10
  uint8_t cnt;
  uint8_t addr[IF_IPv6_ADDR_MAX_NUM][IPv6_ALEN];
};
#define CVCOCTCI3_IPv6_ADDR_LIST_INITIALIZER {\
  .cnt = 0,\
  .addr = {CVCOCTCI3_IPv6_ADDR_INITIALIZER},\
}


/**
 * @brief Ipv6InterfaceInfo
 *
 * 인터페이스에 대한 IPv6 인터페이스 정보
 */
struct Cvcoctci3IPv6InterfaceInfo
{
  struct {
    bool default_gw;
    bool primary_dns;
    bool gw_mac_addr;
  } options;    ///< 옵션필드 수납 여부

  char if_name[IF_NAME_MAX_SIZE+1];              ///< 인터페이스 이름
  struct Cvcoctci3IPv6AddressList ip_addr_list;   ///< IPv6 주소(들)
  uint8_t mac_addr[MAC_ALEN];                    ///< MAC 주소
  uint8_t default_gw[IPv6_ALEN];                 ///< Default gateway
  uint8_t primary_dns[IPv6_ALEN];                ///< Primary DNS
  uint8_t gw_mac_addr[MAC_ALEN];                 ///< Gateway MAC address
};
/// struct @ref Cvcoctci3IPv6InterfaceInfo 초기화 매크로
#define CVCOCTCI3_IPv6_IF_INFO_INITIALIZER {\
  .options = {false, false, false},\
  .if_name = {0},\
  .ip_addr = CVCOCTCI3_IPv6_ADDR_LIST_INITIALIZER,\
  .mac_addr = CVCOCTCI3_MAC_ADDR_INITIALIZER,\
  .default_gw = CVCOCTCI3_IPv6_ADDR_INITIALIZER,\
  .primary_dns = CVCOCTCI3_IPv6_ADDR_INITIALIZER,\
  .gw_mac_addr = CVCOCTCI3_MAC_ADDR_INITIALIZER,\
}



/**
 * @brief Ipv6InterfaceInfo(들)
 *
 * 다수의 인터페이스들에 대한 IPv6 인터페이스 정보 집합
 */
struct Cvcoctci3IPv6InterfaceInfos
{
#define IF_MAX_NUM 10
  uint8_t info_cnt;     ///< 인터페이스 정보 개수
  struct Cvcoctci3IPv6InterfaceInfo info[IF_MAX_NUM];  ///< 인터페이스 정보(들)
};
/// struct @ref Cvcoctci3IPv6InterfaceInfos 초기화 매크로
#define CVCOCTCI3_IPv6_IF_INFOS_INITIALIZER {\
  .info_cnt = 0,\
  .info = {CVCOCTCI3_IPv6_IF_INFO_INITIALIZER},\
}


/**
 * @brief ComponentType
 */
enum eCvcoctci3ComponentType
{
  kCvcoctci3ComponentType_Hardware = 0,
  kCvcoctci3ComponentType_Firmware = 1,
  kCvcoctci3ComponentType_Software = 2,
  kCvcoctci3ComponentType_TciApp = 3,
  kCvcoctci3ComponentType_NA = -999,
};
typedef int Cvcoctci3ComponentType; ///< @ref eCvcoctci3ComponentType


/**
 * @brief VersionInfoBlock
 */
struct Cvcoctci3VersionInfoBlock
{
  struct {
    bool release_date;
    bool description;
  } options;  ///< 옵션필드 수납 설정

  Cvcoctci3ComponentType component_type;           ///< 컴포넌트 유형
#define VERSION_ID_MAX_SIZE 50
  char version_id[VERSION_ID_MAX_SIZE+1];      ///< 버전 ID
#define RELEASE_DATE_MAX_SIZE 50
  char release_date[RELEASE_DATE_MAX_SIZE+1];  ///< 배포일자
#define DESCRIPTION_MAX_SIZE 100
  char description[DESCRIPTION_MAX_SIZE+1];    ///< 기타 설명
};
/// struct @ref Cvcoctci3VersionInfoBlock 초기화 매크로
#define CVCOCTCI3_VERSION_INFO_BLOCK_INITIALIZER {\
  .options = {false, false},\
  .component_type = kCvcoctci3ComponentType_NA,\
  .version_id = {0},\
  .release_date = {0},\
  .description = {0},\
}


/**
 * @brief VersionInfoBlock(들)
 */
struct Cvcoctci3VersionInfoBlocks
{
#define VERSION_INFO_MAX_NUM 10
  int info_cnt;     ///< 정보 개수
  struct Cvcoctci3VersionInfoBlock info[VERSION_INFO_MAX_NUM]; ///< 정보(들)
};
/// struct @ref Cvcoctci3VersionInfoBlocks 초기화 매크로
#define CVCOCTCI3_VERSION_INFO_BLOCKS_INITIALIZER {\
  .info_cnt = 0,\
  .info = { CVCOCTCI3_VERSION_INFO_BLOCK_INITIALIZER, },\
}


/**
 * @brief SutInfo
 */
struct Cvcoctci3SutInfo
{
  struct {
    bool model_name;
  } options;    ///< 옵션필드 수납 여부

#define MODEL_NAME_MAX_SIZE 255
  char model_name[MODEL_NAME_MAX_SIZE+1];  ///< 모델명
  struct Cvcoctci3VersionInfoBlocks version_infos;  ///< 버전정보(들)
};
/// struct @ref Cvcoctci3SutInfo 초기화 매크로
#define CVCOCTCI3_SUT_INFO_INITIALIZER {\
  .options = { false },\
  .model_name = { 0 },\
  .version_infos = CVCOCTCI3_VERSION_INFO_BLOCKS_INITIALIZER,\
}



/**
 * Update TCIv3 by young
 * Add pc5 crc
 * 
 * @brief CRC
 */
struct Cvcoctci3Pc5Crc
{
#define CRC_LEN 4
  size_t len; ///< 길이
  uint8_t buf[CRC_LEN+1]; ///< buffer
};
/// struct @ref Cvcoctci3PduFilter 초기화 매크로
#define CVCOCTCI3_PC5_CRC_INITIALIZER {\
  .len = 0,\
  .buf = {0,},\
}


/**
 * Update TCIv3 by young
 * Add pc5 pdu
 * 
 * @brief PDU
 */
struct Cvcoctci3Pc5Pdu
{
  size_t len; ///< 길이
  uint8_t buf[kCvcoctci3Mtu_Max+1]; ///< buffer
};
/// struct @ref Cvcoctci3PduFilter 초기화 매크로
#define CVCOCTCI3_PC5_PDU_INITIALIZER {\
  .len = 0,\
  .buf = {0,},\
}


/**
 * Update TCIv3 by young
 * Add sendUeConfigXML
 * 
 * @brief SendUeConfigXML
 * */
struct Cvcoctci3SendUeConfigXML
{
  struct {
    bool crc;
  } options;
  
  int counter;
  int total;
  struct Cvcoctci3Pc5Crc crc;
  struct Cvcoctci3Pc5Pdu pdu;
};
/// struct @ref Cvcoctci3SendUeConfigXML 초기화 매크로
#define CVCOCTCI3_SEND_UE_CONFIG_XML_INITIALIZER {\
  .options = { false },\
  .counter = 0,\
  .total = 0,\
  .crc = CVCOCTCI3_PC5_CRC_INITIALIZER,\
  .pdu = CVCOCTCI3_PC5_PDU_INITIALIZER,\
}

/**
 * Update TCIv3 by young
 * Add pc5 bandwidth
 * 
 * @brief Version
 */
enum eCvcoctci3SLBandwidthR12
{
  kCvcoctci3SLBandwidthR12_n6 = 0,
  kCvcoctci3SLBandwidthR12_n15,
  kCvcoctci3SLBandwidthR12_n25,
  kCvcoctci3SLBandwidthR12_n50,
  kCvcoctci3SLBandwidthR12_n75,
  kCvcoctci3SLBandwidthR12_n100,
  kCvcoctci3SLBandwidthR12_Min = kCvcoctci3SLBandwidthR12_n6,
  kCvcoctci3SLBandwidthR12_Max = kCvcoctci3SLBandwidthR12_n100,
  kCvcoctci3SLBandwidthR12_NA = -999,
};
typedef int Cvcoctci3SLBandwidthR12; ///< @ref eCvcoctci3SLBandwidthR12


/**
 * Update TCIv3 by young
 * Add setUeConfig
 * 
 * @brief SetUeConfig
 * */
struct Cvcoctci3SetUeConfig
{
  struct {
    bool earfcn;
    bool p_max;
    bool bw;
    bool min_mcs;
    bool max_mcs;
    bool num_sub_ch;
    bool sub_ch_size;
    bool pdb;
  } options;

  Cvcoctci3Earfcn earfcn;         ///< transmit pool frequency
  Cvcoctci3PMax p_max;            ///< tx power
  Cvcoctci3SLBandwidthR12 bw;     ///< bandwidth
  Cvcoctci3Mcs min_mcs;           ///< modulation coding rate - min
  Cvcoctci3Mcs max_mcs;           ///< modulation coding rate - max
  int num_sub_ch;                 ///< number of subchannels
  int sub_ch_size;                ///< subchannel size
  Cvcoctci3Pdb pdb;               ///< packet delay budget (i.e. expiry time)
};
/// struct @ref Cvcoctci3SetUeConfig 초기화 매크로
#define CVCOCTCI3_SET_UE_CONFIG_INITIALIZER {\
  .options = { false, false, false, false, false, false, false, false },\
  .earfcn = kCvcoctci3Earfcn_NA,\
  .p_max = kCvcoctci3PMax_NA,\
  .bw = kCvcoctci3SLBandwidthR12_NA,\
  .min_mcs = kCvcoctci3Mcs_NA,\
  .max_mcs = kCvcoctci3Mcs_NA,\
  .num_sub_ch = 0,\
  .sub_ch_size = 0,\
  .pdb = kCvcoctci3Pdb_NA,\
}


/**
 * Update TCIv3 by young
 * Add flow type
 * 
 * @brief Flow type
 * */
enum eCvcoctci3FlowType 
{
  kCvcoctci3FlowType_sps = 0,
  kCvcoctci3FlowType_event,
  kCvcoctci3FlowType_Min = kCvcoctci3FlowType_sps,
  kCvcoctci3FlowType_Max = kCvcoctci3FlowType_event,
  kCvcoctci3FlowType_NA = -999,
};
typedef int Cvcoctci3FlowType; ///< @ref eCvcoctci3FlowType


/**
 * Update TCIv3 by young
 * Add pc5 user priority (It is the same as the existing user priority, but the range is different)
 * 
 * @brief pc5 user priority
 * */
enum eCvcoctci3Pc5UserPriority
{
  kCvcoctci3Pc5UserPriority_Min = 1,
  kCvcoctci3Pc5UserPriority_Max = 8,
  kCvcoctci3Pc5UserPriority_NA = -999,
};
typedef int Cvcoctci3Pc5UserPriority; ///< @ref eCvcoctci3Pc5UserPriority


/**
 * Update TCIv3 by young
 * Add sps reservation size
 * 
 * @brief sps reservation size
 * */
enum eCvcoctci3SpsReservationSize
{
  kCvcoctci3SpsReservationSize_Min = 0,
  kCvcoctci3SpsReservationSize_Max = kCvcoctci3Mtu_Max,
  kCvcoctci3SpsReservationSize_NA = -999,
};
typedef int Cvcoctci3SpsReservationSize; ///< @ref eCvcoctci3SpsReservationSize


/**
 * Update TCIv3 by young
 * Add setFlowConfig
 * 
 * @brief SetFlowConfig
 * */
struct Cvcoctci3SetFlowConfig
{
  struct {
    bool service_id;
    bool sps_reservation_size;
    bool periodicity;
    bool tx_power;
    bool harq;
    bool tx_pool_id;
  } options;

  Cvcoctci3FlowIdentifier flow_id;                    ///< identifier for the specific flow, seleted by a requester
  Cvcoctci3FlowType flow_type;
  Cvcoctci3Pc5UserPriority pppp;                      ///< user priority
  int service_id;                                     ///< used for IPv6 UDP broadcast
  Cvcoctci3SpsReservationSize sps_reservation_size;   ///< SPS reservation size bytes for the SPS flow creation
  Cvcoctci3Periodicity periodicity;                   ///< reservation periodicity (SPS flow or event)
  Cvcoctci3PMax tx_power;                             
  bool harq;                                          ///< Harq On (TRUE) - OFF (FALSE)
  int tx_pool_id;
};
/// struct @ref Cvcoctci3SetFlowConfig 초기화 매크로
#define CVCOCTCI3_SET_FLOW_CONFIG_INITIALIZER {\
  .options = { false, false, false, false, false, false },\
  .flow_id = kCvcoctci3FlowIdentifier_NA,\
  .flow_type = kCvcoctci3FlowType_NA,\
  .pppp = kCvcoctci3Pc5UserPriority_NA,\
  .service_id = 0,\
  .sps_reservation_size = kCvcoctci3SpsReservationSize_NA,\
  .periodicity = kCvcoctci3Periodicity_NA,\
  .tx_power = kCvcoctci3PMax_NA,\
  .harq = false,\
  .tx_pool_id = 0,\
}


/**
 * Update TCIv3 by young
 * Add setFlowConfigs
 * 
 * @brief SetFlowConfigs
 * */
struct Cvcoctci3SetFlowConfigs
{
#define SET_FLOW_CONFIG_MAX_NUM (32)
  size_t cnt;
  struct Cvcoctci3SetFlowConfig flow_config[SET_FLOW_CONFIG_MAX_NUM];
};
/// struct @ref Cvcoctci3SetFlowConfigs 초기화 매크로
#define CVCOCTCI3_SET_FLOW_CONFIGS_INITIALIZER {\
  .cnt = 0,\
  flow_config = {CVCOCTCI3_SET_FLOW_CONFIG_INITIALIZER, }\
}

/**
 * Update TCIv3 by young
 * Add pc5 sendATcommand
 * 
 * @brief SendATcommand
 */
struct Cvcoctci3SendATCommand
{
  size_t len; ///< 길이
  uint8_t buf[kCvcoctci3Mtu_Max+1]; ///< buffer
};
/// struct @ref Cvcoctci3SendATCommand 초기화 매크로
#define CVCOCTCI3_SNED_AT_COMMAND_INITIALIZER {\
  .len = 0,\
  .buf = {0,},\
}

/**
 * Update TCIv3 by young
 * Add atCmdInfo
 * 
 * @brief atCmdInfo
 * */
struct Cvcoctci3ATCmdInfo
{
  size_t len; ///< 길이
  uint8_t buf[kCvcoctci3Mtu_Max+1]; ///< buffer
};
/// struct @ref Cvcoctci3ATCmdInfo 초기화 매크로
#define CVCOCTCI3_AT_CMD_INFO_INITIALIZER {\
  .len = 0,\
  .buf = {0, },\
}


/**
 * Update TCIv3 by young
 * Add pktCount
 * 
 * @brief pktCount
 * */
enum eCvcoctci3PktCount
{
  kCvcoctci3PktCount_Min = 0,
  kCvcoctci3PktCount_Max = 9223372036854775807,
  kCvcoctci3PktCount_NA = -999,
};
typedef int64_t Cvcoctci3PktCount; ///< @ref eCvcoctci3PktCount


/**
 * Update TCIv3 by young
 * Add sutStatus
 * 
 * @brief sutStatus
 * */
struct Cvcoctci3SutStatus
{
  size_t len;
  uint8_t buf[kCvcoctci3Mtu_Max+1];
};
/// struct @ref Cvcoctci3SutStatus 초기화 매크로
#define CVCOCTCI3_SUT_STATUS_INITIALIZER {\
  .len = 0,\
  .buf = {0, },\
}


/**
 * Update TCIv3 by young
 * Add ProxyIpPort
 * */
enum eCvcoctci3ProxyIpPort
{
  kCvcoctci3ProxyIpPort_Min = 0,
  kCvcoctci3ProxyIpPort_Max = 65535,
  kCvcoctci3ProxyIpPort_NA = -999,
};
typedef int Cvcoctci3ProxyIpPort; ///< @ref eCvcoctci3ProxyIpPort


/**
 * Update TCIv3 by young
 * Add ProxyIpAddress
 * */
struct Cvcoctci3ProxyIpAddress
{
#define PROXY_IP_ADDRESS_MAX_LEN 255
  size_t len;
  uint8_t buf[PROXY_IP_ADDRESS_MAX_LEN+1];
};
/// struct @ref Cvcoctci3ProxyIpAddress 초기화 매크로
#define CVCOCTCI3_PROXY_IP_ADDRESS_INITIALIZER {\
  .len = 0,\
  .buf = {0, },\
}

/**
 * Update TCIv3 by young
 * Add startUdpProxyTx
 * 
 * @brief startUdpProxyTx
 * */
struct Cvcoctci3StartUdpProxyTx
{
  struct {
    bool flow_id;
  } options;
  Cvcoctci3ProxyIpPort port;
  struct Cvcoctci3RadioInterface radio;
  Cvcoctci3FlowIdentifier flow_id;
  int pc5_mtu;
};
/// struct @ref Cvcoctci3StartUdpProxyTx 초기화 매크로
#define CVCOCTCI3_START_UDP_PROXY_TX_INITIALIZER {\
  .options = {false},\
  .port = kCvcoctci3ProxyIpPort_NA,\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
  .flow_id = kCvcoctci3FlowIdentifier_NA,\
  .pc5_mtu = 0,\
}

/**
 * Update TCIv3 by young
 * Add StartUdpProxyRxProto
 * */
enum eCvcoctci3StartUdpProxyRxProto
{
  kCvcoctci3StartUdpProxyRxProto_Pcap = 0,
  kCvcoctci3StartUdpProxyRxProto_Min = kCvcoctci3StartUdpProxyRxProto_Pcap,
  kCvcoctci3StartUdpProxyRxProto_Max = kCvcoctci3StartUdpProxyRxProto_Pcap,
  kCvcoctci3StartUdpProxyRxProto_NA = -999,
};
typedef int Cvcoctci3StartUdpProxyRxProto; ///< @ref eCvcoctci3StartUdpProxyRxProto


/**
 * Update TCIv3 by young
 * Add StartUdpProxyRxOptions
 * */
struct Cvcoctci3StartUdpProxyRxOptions
{
  bool radiotap;
};
/// struct @ref Cvcoctci3StartUdpProxyRxOptions 초기화 매크로
#define CVCOCTCI3_START_UDP_PROXY_RX_OPTIONS_INITIALIZER {\
  .radiotap = false,\
}

/**
 * Update TCIv3 by young
 * Add startUdpProxyRx
 * 
 * @brief startUdpProxyRx
 * */
struct Cvcoctci3StartUdpProxyRx
{
  struct {
    bool flow_id;
    bool udp_proxy_rx_options;
  } options;

  struct Cvcoctci3ProxyIpAddress dest_address;
  Cvcoctci3ProxyIpPort dest_port;
  struct Cvcoctci3RadioInterface radio;
  Cvcoctci3FlowIdentifier flow_id;
  int pc5_mtu;
  Cvcoctci3StartUdpProxyRxProto proto;
  struct Cvcoctci3StartUdpProxyRxOptions udp_proxy_rx_options;
};
/// struct @ref Cvcoctci3StartUdpProxyRx 초기화 매크로
#define CVCOCTCI3_START_UDP_PROXY_RX_INITIALIZER {\
  .options = {false, false},\
  .dest_address = CVCOCTCI3_PROXY_IP_ADDRESS_INITIALIZER,\
  .dest_port = kCvcoctci3ProxyIpPort_NA,\
  .radio = CVCOCTCI3_RADIO_INTERFACE_INITIALIZER,\
  .flow_id = kCvcoctci3FlowIdentifier_NA,\
  .pc5_mtu = 0,\
  .proto = kCvcoctci3StartUdpProxyRxProto_NA,\
  .udp_proxy_rx_options = CVCOCTCI3_START_UDP_PROXY_RX_OPTIONS_INITIALIZER,\
}


enum eCvcoctci3StopUdpProxyDirection
{
  kCvcoctci3StopUdpProxyDirection_Tx = 0,
  kCvcoctci3StopUdpProxyDirection_Rx = 1,
  kCvcoctci3StopUdpProxyDirection_Min = 0,
  kCvcoctci3StopUdpProxyDirection_Max = 1,
  kCvcoctci3StopUdpProxyDirection_NA = -999,
};
typedef int Cvcoctci3StopUdpProxyDirection; ///< @ref eCvcoctci3StopUdpProxyDirection


/**
 * Update TCIv3 by young
 * Add StopUdpProxy
 * 
 * @brief Stop StopUdpProxy
 * */
struct Cvcoctci3StopUdpProxy
{
  struct {
    bool dest_address;
  } options;

  Cvcoctci3StopUdpProxyDirection direction;
  Cvcoctci3ProxyIpPort port;
  struct Cvcoctci3ProxyIpAddress dest_address;
};
/// struct @ref Cvcoctci3StopUdpProxy 초기화 매크로
#define CVCOCTCI3_STOP_UDP_PROXY_INITIALIZER {\
  .options = {false},\
  .direction = kCvcoctci3StopUdpProxyDirection_NA,\
  .port = kCvcoctci3ProxyIpPort_NA,\
  .dest_address = CVCOCTCI3_PROXY_IP_ADDRESS_INITIALIZER,\
}

#endif //V2X_SW_CVCOCTCI3_TYPES_H
