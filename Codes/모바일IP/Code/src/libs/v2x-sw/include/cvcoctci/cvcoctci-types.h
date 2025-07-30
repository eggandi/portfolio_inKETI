/**
 * @addtogroup cvcoctci_lib cvcoctci library API
 * @{
 *
 * @file
 * @brief cvcoctci 라이브러리의 Open API 에서 사용되는 자료 유형을 정의한 헤더파일
 * @date 2019-09-14
 * @author gyun
 */

#ifndef V2X_SW_CVCOCTCI_TYPES_H
#define V2X_SW_CVCOCTCI_TYPES_H


// 시스템 헤더 파일
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// 라이브러리 헤더 파일
#include "cvcoctci-defines.h"


/**
 * @brief API 처리 결과 코드
 */
enum eCvcoctciResultCode
{
  kCvcoctciResult_Success = 0,
  kCvcoctciResult_NullParameters, ///< 널 파라미터를 전달하였음.
  kCvcoctciResult_OerEncodeFail, ///< OER 인코딩 실패
  kCvcoctciResult_OerDecodeFail, ///< OER 디코딩 실패
  kCvcoctciResult_AbnormalPkt, ///< 비정상 패킷
  kCvcoctciResult_InvalidFrameChoice, ///< 유효하지 않은 Frame choice
  kCvcoctciResult_NotSupportedTci80211Choice, ///< 지원하지 않는 TCI80211 choice
  kCvcoctciResult_InvalidTci80211Choice, ///< 유효하지 않은 TCI80211 choice
  kCvcoctciResult_NotSupportedTci16093Choice,   ///< 지원하지 않는 TCI16093 choice
  kCvcoctciResult_InvalidTci16093Choice, ///< 유효하지 않은 TCI16093 choice
  kCvcoctciResult_NotSupportedTci16094Choice,   ///< 지원하지 않는 TCI16094 choice
  kCvcoctciResult_InvalidTci16094Choice, ///< 유효하지 않은 TCI16094 choice
  kCvcoctciResult_NotSupportedTci29451Choice,   ///< 지원하지 않는 TCI29451 choice
  kCvcoctciResult_InvalidTci29451Choice, ///< 유효하지 않은 TCI29451 choice
  kCvcoctciResult_NotSupportedTciSutControlChoice,   ///< 지원하지 않는 TciSutControl choice
  kCvcoctciResult_InvalidTciSutControlChoice, ///< 유효하지 않은 TciSutControl choice
  kCvcoctciResult_InvalidMessageId, ///< 유효하지 않은 messageId
  kCvcoctciResult_InvalidPsidFormat, ///< 유효하지 않은 PSID 형식
  kCvcoctciResult_InvalidPsidValue, ///< 유효하지 않은 PSID 값
  kCvcoctciResult_InsufficientBuf, ///< 충분하지 않은 크기의 버퍼
  kCvcoctciResult_TooManyPsid, ///< 너무 많은 PSID
  kCvcoctciResult_TooLongPdu, ///< 너무 긴 PDU
  kCvcoctciResult_InvalidFrameType, ///< 유효하지 않은 프레임 유형
  kCvcoctciResult_InvalidEventParamsType, ///< 유효하지 않은 이벤트파라미터 유형
  kCvcoctciResult_InvalidInfoContentType, ///< 유효하지 않은 info content 유형
  kCvcoctciResult_InvalidEvent, ///< 유효하지 않은 Event
  kCvcoctciResult_InvalidRcpiValue, ///< 유효하지 않은 RCPI 값
  kCvcoctciResult_InvalidPduType, ///< 유효하지 않은 Pdu Type
  kCvcoctciResult_InvalidRadioValue, ///< 유효하지 않은 Radio 값
  kCvcoctciResult_InvalidResultCode, ///< 유효하지 않은 ResultCode 값
  kCvcoctciResult_InvalidExceptionType, ///< 유효하지 않은 ExceptionType 값
  kCvcoctciResult_InvalidExceptionId, ///< 유효하지 않은 ExceptionId 값
  kCvcoctciResult_InvalidExtensionId, ///< 유효하지 않은 ExtensionId 값
  kCvcoctciResult_TooLongOctetString, ///< 너무 긴 Octet string

  kCvcoctciResult_NotSupportedYet,  ///< 아직 지원되지 않음
  kCvcoctciResult_NoMemory,         ///< 메모리 부족
  kCvcoctciResult_SystemCallFailed, ///< 시스템콜 호출 실패
};
typedef int CvcoctciResultCode; ///< @ref eCvcoctciResultCode


/**
 * @brief 로그메시지 출력 레벨
 *
 * 높은 레벨은 낮은 레벨의 범위를 포함한다. \n
 * 즉, 로그변수가 높은 레벨로 설정되어 있으면, 그 하위레벨에 해당되는 로그는 함께 출력된다.
 */
enum eCvcoctciLogLevel
{
  kCvcoctciLogLevel_none = 0, /// 아무 로그도 출력하지 않는다.
  kCvcoctciLogLevel_err, /// 에러 로그
  kCvcoctciLogLevel_init, /// 초기화 절차에 관련된 로그
  kCvcoctciLogLevel_config, /// 설정에 관련된 로그 (API 엔트리 포함)
  kCvcoctciLogLevel_event, /// 각종 이벤트(패킷 송수신 포함)에 관련된 로그
  kCvcoctciLogLevel_dump, /// 상세내용 로그(송수신 패킷 덤프 데이터 등)
  kCvcoctciLogLevel_all, /// 모든 로그
  kCvcoctciLogLevel_min = kCvcoctciLogLevel_none,
  kCvcoctciLogLevel_max = kCvcoctciLogLevel_all
};
typedef int CvcoctciLogLevel;  ///< @ref eCvcoctciLogLevel



/**
 * @brief Version
 */
enum eCvcoctciVersion
{
  kCvcoctciVersion_Min = 1,
  kCvcoctciVersion_Current = 2,
  kCvcoctciVersion_Max = 127,
  kCvcoctciVersion_NA = -999,
};
typedef int CvcoctciVersion; ///< @ref eCvcoctciVersion


/**
 * @brief Radio
 */
enum eCvcoctciRadio
{
  kCvcoctciRadio_0,
  kCvcoctciRadio_1,
  kCvcoctciRadio_2,
  kCvcoctciRadio_3,
  kCvcoctciRadio_Min = kCvcoctciRadio_0,
  kCvcoctciRadio_Max = kCvcoctciRadio_3,
  kCvcoctciRadio_NA = -999,
};
typedef int CvcoctciRadio; ///< @ref eCvcoctciRadio


/**
 * @brief Antenna
 */
enum eCvcoctciAntenna
{
  kCvcoctciAntenna_1 = 1,
  kCvcoctciAntenna_2 = 2,
  kCvcoctciAntenna_both = 3,
  kCvcoctciAntenna_NA = -999,
};
typedef int CvcoctciAntenna; ///< @ref eCvcoctciAntenna


/**
 * @brief RadioInterface
 */
struct CvcoctciRadioInterface
{
  /// 옵션 필드 설정 여부
  struct {
    bool antenna;
  } options;
  CvcoctciRadio radio; ///< 라디오 식별번호
  CvcoctciAntenna antenna; ///< 안테나 식별번호
};
/// struct @ref CvcoctciRadioInterface 구조체 초기화 매크로
#define CVCOCTCI_RADIO_INTERFACE_INITIALIZER {\
  .options = {false},\
  .radio = kCvcoctciRadio_NA,\
  .antenna = kCvcoctciAntenna_NA,\
}


/**
 * @brief 밀리초 단위의 UTC 타임스탬프 (1970년 1월1일부터)
 */
typedef uint64_t CvcoctciTime64;
/// Time64 최소값
#define kCvcoctciTime64_Min 0
/// Time64 최대값
#define kCvcoctciTime64_Max 9223372036854775807ULL


/**
 * @brief TCI 프레임 유형
 */
enum eCvcoctciFrameType
{
  kCvcoctciFrameType_16093,
  kCvcoctciFrameType_80211,
  kCvcoctciFrameType_16094,
  kCvcoctciFrameType_29451,
  kCvcoctciFrameType_SutControl,
  kCvcoctciFrameType_NA = -999,
};
typedef int CvcoctciTciFrameType; ///< @ref eCvcoctciFrameType


/**
 * @brief TCI Request 메시지 유형
 */
enum eCvcoctciRequestType
{
  // from TCI-wsm
  kCvcoctciRequestType_SetInitialState,
  kCvcoctciRequestType_SetWsmTxInfo,
  kCvcoctciRequestType_StartWsmTx,
  kCvcoctciRequestType_StopWsmTx,
  kCvcoctciRequestType_StartWsmRx,
  kCvcoctciRequestType_StopWsmRx,
  kCvcoctciRequestType_StartWsaTxPeriodic,
  kCvcoctciRequestType_StopWsaTxPeriodic,
  kCvcoctciRequestType_AddWsaProviderService,
  kCvcoctciRequestType_ChangeWsaProviderService,
  kCvcoctciRequestType_DelWsaProviderService,
  kCvcoctciRequestType_AddUserService,
  kCvcoctciRequestType_DelUserService,

  // from TCI-ip
  kCvcoctciRequestType_GetIPv6InterfaceInfo,
  kCvcoctciRequestType_SetIPv6Address,
  kCvcoctciRequestType_StartIPv6Tx,
  kCvcoctciRequestType_StopIPv6Tx,
  kCvcoctciRequestType_StartIPv6Rx,
  kCvcoctciRequestType_StopIPv6Rx,
  kCvcoctciRequestType_StartIPv6Ping,
  kCvcoctciRequestType_StopIPv6Ping,
  kCvcoctciRequestType_AddTxProfile,
  kCvcoctciRequestType_DelTxProfile,

  // from TCI-SutControl
  kCvcoctciRequestType_Shutdown,
  kCvcoctciRequestType_Restart,
  kCvcoctciRequestType_RequestSutAvailability,
  kCvcoctciRequestType_RequestSutInfo,
  kCvcoctciRequestType_SetTestId,
  kCvcoctciRequestType_EnableGpsInput,
  kCvcoctciRequestType_SetLatitude,
  kCvcoctciRequestType_SetLongitude,
  kCvcoctciRequestType_SetElevation,
  kCvcoctciRequestType_SetPositionalAccuracy,
  kCvcoctciRequestType_SetSpeed,
  kCvcoctciRequestType_SetHeading,
  kCvcoctciRequestType_SetAccelerationSet4Way,
  kCvcoctciRequestType_SetGpsTime,

  // from TCI29451
  kCvcoctciRequestType_StartBsmTx,
  kCvcoctciRequestType_StopBsmTx,
  kCvcoctciRequestType_EnableCongestionMitigation,
  kCvcoctciRequestType_SetTemporaryID,
  kCvcoctciRequestType_SetTransmissionState,
  kCvcoctciRequestType_SetSteeringWheelAngle,
  kCvcoctciRequestType_SetBrakeSystemStatus,
  kCvcoctciRequestType_SetVehicleSize,
  kCvcoctciRequestType_SetExteriorLights,
  kCvcoctciRequestType_SetVehicleEventFlags,
  kCvcoctciRequestType_StartBsmRx,
  kCvcoctciRequestType_StopBsmRx,

  kCvcoctciRequestType_NA = -999,
};
typedef int CvcoctciRequestType; ///< @ref eCvcoctciRequestType


/**
 * @brief PSID
 */
enum eCvcoctciPsid
{
  kCvcoctciPsid_NA = -999,
};
typedef int CvcoctciPsid; ///< @ref eCvcoctciPsid


/**
 * @brief ContentType
 */
enum eCvcoctciContentType
{
  kCvcoctciContentType_Other = 0,
  kCvcoctciContentType_Ieee16092Data = 1,
  kCvcoctciContentType_Wsa = 2,
  kCvcoctciContentType_Bsm = 3,
  kCvcoctciContentType_Map = 4,
  kCvcoctciContentType_Spat = 5,
  kCvcoctciContentType_Tim = 6,
  kCvcoctciContentType_NA = -999,
};
typedef int CvcoctciContentType; ///< @ref eCvcoctciContentType


/**
 * @brief SignerIdentifierType
 */
enum eCvcoctciSignerIdentifierType
{
  kCvcoctciSignerIdentifierType_Unsecure = 0,
  kCvcoctciSignerIdentifierType_UseSecProfilePerContentType = 1,
  kCvcoctciSignerIdentifierType_SignIncludeCertificate = 2,
  kCvcoctciSignerIdentifierType_SignIncludeDigest = 3,
  kCvcoctciSignerIdentifierType_NA = -999,
};
typedef int CvcoctciSignerIdentifierType; ///< @ref eCvcoctciSignerIdentifierType


/// @brief HashedId8
typedef uint8_t CvcoctciHashedId8[HASHED_ID8_LEN];
/// @brief HashedId8 초기화 매크로
#define CVCOCTCI_HASHED_ID8_INITIALIZER {\
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 \
}


/**
 * @brief ChannelNumber80211
 */
enum eCvcoctciChannelNumber
{
  kCvcoctciChannelNumber_Min = 0,
  kCvcoctciChannelNumber_Max = 255,
  kCvcoctciChannelNumber_NA = -999,
};
typedef int CvcoctciChannelNumber; ///< @ref eCvcoctciChannelNumber


/**
 * @brief TimeSlot
 */
enum eCvcoctciTimeSlot
{
  kCvcoctciTimeSlot_AltSlot0 = 1,
  kCvcoctciTimeSlot_AltSlot1 = 2,
  kCvcoctciTimeSlot_Continuous = 3,
  kCvcoctciTimeSlot_Min = kCvcoctciTimeSlot_AltSlot0,
  kCvcoctciTimeSlot_Max = kCvcoctciTimeSlot_Continuous,
  kCvcoctciTimeSlot_NA = -999,
};
typedef int CvcoctciTimeSlot; ///< @ref eCvcoctciTimeSlot


/**
 * @brief DataRate80211
 */
enum eCvcoctciDataRate
{
  kCvcoctciDataRate_Min = 0,
  kCvcoctciDataRate_Max = 255,
  kCvcoctciDataRate_NA = -999,
};
typedef int CvcoctciDataRate; ///< @ref eCvcoctciDataRate


/**
 * @brief TxPower80211
 */
enum eCvcoctciTxPower
{
  kCvcoctciTxPower_Min = -128,
  kCvcoctciTxPower_Max = 127,
  kCvcoctciTxPower_NA = -999,
};
typedef int CvcoctciTxPower; ///< @ref eCvcoctciTxPower


/**
 * @brief User priority
 */
enum eCvcoctciUserPriority
{
  kCvcoctciUserPriority_Min = 0,
  kCvcoctciUserPriority_Max = 7,
  kCvcoctciUserPriority_NA = -999,
};
typedef int CvcoctciUserPriority; ///< @ref eCvcoctciUserPriority



/**
 * @brief RepeatRate (5초당 전송 회수)
 */
enum eCvcoctciRepeatRate
{
  kCvcoctciRepeatRate_Min = 0,
  kCvcoctciRepeatRate_Max = 255,
  kCvcoctciRepeatRate_NA = -999,
};
typedef int CvcoctciRepeatRate; ///< @ref eCvcoctciRepeatRate


/**
 * @brief Latitude(0.1 마이크로도 단위의 위도)
 */
enum eCvcoctciLatitude
{
  kCvcoctciLatitude_Min = -900000000,
  kCvcoctciLatitude_Max = 900000000,
  kCvcoctciLatitude_NA = 900000001
};
typedef int CvcoctciLatitude; ///< @ref eCvcoctciLatitude


/**
 * @brief Longitude(0.1 마이크로도 단위의 경도)
 */
enum eCvcoctciLongitude
{
  kCvcoctciLongitude_Min = -1799999999,
  kCvcoctciLongitude_Max = 1800000000,
  kCvcoctciLongitude_NA = 1800000001,
};
typedef int CvcoctciLongitude; ///< @ref eCvcoctciLongitude


/**
 * @brief Elevation(10cm 단위의 고도)
 */
enum eCvcoctciElevation
{
  kCvcoctciElevation_Min = -4095,
  kCvcoctciElevation_Max = 61439,
  kCvcoctciElevation_NA = -4096,
};
typedef int CvcoctciElevation; ///< @ref eCvcoctciElevation


/**
 * @brief WAVE 확장헤더 포함 여부
 */
struct CvcoctciWaveElementsIncluded
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
/// struct @ref CvcoctciWaveElementsIncluded 구조체 초기화 매크로
#define CVCOCTCI_WAVE_ELEMENTS_INCLUDED_INITIALIZER { \
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
struct CvcoctciRxFlag
{
  bool recv_psid_match;
  bool include_pdu; ///< 1일 경우, 수신된 WAVE 패킷을 Indication 으로 전달할 때, Pdu 데이터를 포함시켜야 한다.
  bool include_pdu_param; ///< 1일 경우, 수신된 WAVE 패킷을 Indication 으로 전달할 때, EventParameters 를 포함시켜야 한다.
};
/// struct @ref CvcoctciRxFlag 초기화 매크로
#define CVCOCTCI_RX_FLAG_INITIALIZER {\
  .recv_psid_match = false,\
  .include_pdu = false,\
  .include_pdu_param = false,\
}


/**
 * @brief EventFlag
 */
struct CvcoctciEventFlag
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
/// struct @ref CvcoctciEventFlag 초기화 매크로
#define CVCOCTCI_EVENT_FLAG_INITIALIZER {\
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
 * @brief PduType
 */
enum eCvcoctciPduType
{
  kCvcoctciPduType_80211frame = 1,
  kCvcoctciPduType_16093frame = 2,
  kCvcoctciPduType_16092data = 3,
  kCvcoctciPduType_16093payload = 4,
  kCvcoctciPduType_ipv6payload = 5,
  kCvcoctciPduType_Min = kCvcoctciPduType_80211frame,
  kCvcoctciPduType_Max = kCvcoctciPduType_ipv6payload,
  kCvcoctciPduType_NA = -999,
};
typedef int CvcoctciPduType; ///< @ref eCvcoctciPduType


/**
 * @brief SecurityFlag
 */
struct CvcoctciSecurityFlag
{
  bool bypass_security_verification;
};
/// struct @ref CvcoctciSecurityFlag 초기화 매크로
#define CVCOCTCI_SECURITY_FLAG_INITIALIZER {\
  .bypass_security_verification = false,\
}


/**
 * @brief EventParamsChoice
 */
enum eCvcoctciEventParamsChoice
{
  kCvcoctciEventParamsChoice_service,
  kCvcoctciEventParamsChoice_wsm,
  kCvcoctciEventParamsChoice_ip,
  kCvcoctciEventParamsChoice_d80211frame,
  kCvcoctciEventParamsChoice_security,
  kCvcoctciEventParamsChoice_NA = -999,
};
typedef int CvcoctciEventParamsChoice; ///< @ref eCvcoctciEventParamsChoice


/**
 * @brief RCPI
 */
enum eCvcoctciRcpi
{
  kCvcoctciRcpi_Min = 0,
  kCvcoctciRcpi_Max = 255,
  kCvcoctciRcpi_NA = -999,
};
typedef int CvcoctciRcpi; ///< @ref eCvcoctciRcpi

/**
 * @brief Channel index
 */
enum eCvcoctciChannelIndex
{
  kCvcoctciChannelIndex_NotUsed = 0,
  kCvcoctciChannelIndex_FirstEntry = 1,
  kCvcoctciChannelIndex_Min = kCvcoctciChannelIndex_NotUsed,
  kCvcoctciChannelIndex_Max = 31,
  kCvcoctciChannelIndex_NA = -999,
};
typedef int CvcoctciChannelIndex; ///< @ref eCvcoctciChannelIndex


/**
 * @brief EventHandling
 */
struct CvcoctciEventHandling
{
  struct {
    bool rx_flag;
    bool event_flag;
    bool forward_pdu;
    bool security_flag;
    bool event_params_choice;
  } options;

  struct CvcoctciRxFlag rx_flag;
  struct CvcoctciEventFlag event_flag;
  CvcoctciPduType forward_pdu;
  struct CvcoctciSecurityFlag security_flag;
  CvcoctciEventParamsChoice event_params_choice;
};
/// struct @ref CvcoctciEventHandling 초기화 매크로
#define CVCOCTCI_EVENT_HANDLING_INITIALIZER { \
  .options = { false, false, false, false, false },\
  .rx_flag = CVCOCTCI_RX_FLAG_INITIALIZER,\
  .event_flag = CVCOCTCI_EVENT_FLAG_INITIALIZER,\
  .forward_pdu = kCvcoctciPduType_NA,\
  .security_flag = CVCOCTCI_SECURITY_FLAG_INITIALIZER,\
  .event_params_choice = kCvcoctciEventParamsChoice_NA,\
}


/**
 * @brief SetLatitude 메시지 형식
 */
typedef CvcoctciLatitude CvcoctciSetLatitude;


/**
 * @brief SetLongitude 메시지 형식
 */
typedef CvcoctciLongitude CvcoctciSetLongitude;


/**
 * @brief SetElevation 메시지 형식
 */
typedef CvcoctciElevation CvcoctciSetElevation;


/**
 * @brief SetSpeed 메시지 형식
 */
enum eCvcoctciSpeed
{
  kCvcoctciSpeed_Min = 0,
  kCvcoctciSpeed_Max = 8191,
};
typedef int CvcoctciSetSpeed; ///< @ref eCvcoctciSpeed


/**
 * @brief SetHeading 메시지 형식
 */
typedef int CvcoctciSetHeading;


/**
 * @brief EnableCongestionMitigation 메시지 형식
 */
typedef bool CvcoctciEnableCongestionMitigation;


/**
 * @brief SetGpsTime 메시지 형식
 */
#define CvcoctciSetGpsTime CvcoctciTime64


/**
 * @brief SetTransmissionState 메시지 형식
 */
enum eCvcoctciSetTransmissionState
{
  kCvcoctciSetTransmissionState_Neutral,
  kCvcoctciSetTransmissionState_Park,
  kCvcoctciSetTransmissionState_ForwardGears,
  kCvcoctciSetTransmissionState_ReverseGears,
  kCvcoctciSetTransmissionState_Reserved1,
  kCvcoctciSetTransmissionState_Reserved2,
  kCvcoctciSetTransmissionState_Reserved3,
  kCvcoctciSetTransmissionState_Unavailable,
};
typedef int CvcoctciSetTransmissionState; ///< @ref eCvcoctciSetTransmissionState


/**
 * @brief SetSteeringWheelAngle 메시지 형식
 */
enum eCvcoctciSetSteeringWheelAngle
{
  kCvcoctciSetSteeringWheelAngle_Min = -126,
  kCvcoctciSetSteeringWheelAngle_Max = 127,
};
typedef int CvcoctciSetSteeringWheelAngle; ///< @ref eCvcoctciSetSteeringWheelAngle


/**
 * @brief VehicleWidth
 */
enum eCvcoctciVehicleWidth
{
  kCvcoctciVehicleWidth_Min = 0,
  kCvcoctciVehicleWidth_Max = 1023,
};
typedef int CvcoctciVehicleWidth; ///< @ref eCvcoctciVehicleWidth


/**
 * @brief VehicleLength
 */
enum eCvcoctciVehicleLength
{
  kCvcoctciVehicleLength_Min = 0,
  kCvcoctciVehicleLength_Max = 4095,
};
typedef int CvcoctciVehicleLength; ///< @ref eCvcoctciVehicleLength


/**
 * @brief SetVehicleSize 메시지 형식
 */
struct CvcoctciSetVehicleSize
{
  CvcoctciVehicleWidth width;
  CvcoctciVehicleLength length;
};


/**
 * @brief SetExteriorLights 메시지 형식
 */
struct CvcoctciSetExteriorLights
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
struct CvcoctciBrakeAppliedStatus
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
enum eCvcoctciTractionControlStatus
{
  kCvcoctciTractionControlStatus_Unavailable,
  kCvcoctciTractionControlStatus_Off,
  kCvcoctciTractionControlStatus_On,
  kCvcoctciTractionControlStatus_Engaged,
};
typedef int CvcoctciTractionControlStatus; ///< @ref eCvcoctciTractionControlStatus


/**
 * @brief AntiLockBrakeStatus
 */
enum eCvcoctciAntiLockBrakeStatus
{
  kCvcoctciAntiLockBrakeStatus_Unavailable,
  kCvcoctciAntiLockBrakeStatus_Off,
  kCvcoctciAntiLockBrakeStatus_On,
  kCvcoctciAntiLockBrakeStatus_Engaged,
};
typedef int CvcoctciAntiLockBrakeStatus; ///< @ref eCvcoctciAntiLockBrakeStatus


/**
 * @brief StabilityControlStatus
 */
enum eCvcoctciStabilityControlStatus
{
  kCvcoctciStabilityControlStatus_Unavailable,
  kCvcoctciStabilityControlStatus_Off,
  kCvcoctciStabilityControlStatus_On,
  kCvcoctciStabilityControlStatus_Engaged,
};
typedef int CvcoctciStabilityControlStatus; ///< @ref eCvcoctciStabilityControlStatus


/**
 * @brief BrakeBoostApplied
 */
enum eCvcoctciBrakeBoostApplied
{
  kCvcoctciBrakeBoostApplied_Unavailable,
  kCvcoctciBrakeBoostApplied_Off,
  kCvcoctciBrakeBoostApplied_On,
};
typedef int CvcoctciBrakeBoostApplied; ///< @ref eCvcoctciBrakeBoostApplied


/**
 * @brief AuxiliaryBrakeStatus
 */
enum eCvcoctciAuxiliaryBrakeStatus
{
  kCvcoctciAuxiliaryBrakeStatus_Unavailable,
  kCvcoctciAuxiliaryBrakeStatus_Off,
  kCvcoctciAuxiliaryBrakeStatus_On,
  kCvcoctciAuxiliaryBrakeStatus_Reserved,
};
typedef int CvcoctciAuxiliaryBrakeStatus; ///< @ref eCvcoctciAuxiliaryBrakeStatus


/**
 * @brief SetBrakeSystemStatus 메시지 형식
 */
struct CvcoctciSetBrakeSystemStatus
{
  struct CvcoctciBrakeAppliedStatus brake_applied_status;
  CvcoctciTractionControlStatus traction_control_status;
  CvcoctciAntiLockBrakeStatus anti_lock_brake_status;
  CvcoctciStabilityControlStatus stability_control_status;
  CvcoctciBrakeBoostApplied brake_boost_applied;
  CvcoctciAuxiliaryBrakeStatus auxiliary_brake_status;
};


/**
 * @brief Acceleration
 */
enum eCvcoctciAcceleration
{
  kCvcoctciAcceleration_Min = -2000,
  kCvcoctciAcceleration_Max = 2001,
};
typedef int CvcoctciAcceleration; ///< @ref eCvcoctciAcceleration


/**
 * @brief VerticalAcceleration
 */
enum eCvcoctciVerticalAcceleration
{
  kCvcoctciVerticalAcceleration_Min = -127,
  kCvcoctciVerticalAcceleration_Max = 127,
};
typedef int CvcoctciVerticalAcceleration; ///< @ref eCvcoctciVerticalAcceleration


/**
 * @brief YawRate
 */
enum eCvcoctciYawRate
{
  kCvcoctciYawRate_Min = -32767,
  kCvcoctciYawRate_Max = 32767,
};
typedef int CvcoctciYawRate; ///< @ref eCvcoctciYawRate


/**
 * @brief SetAccelerationSet4Way 메시지 형식
 */
struct CvcoctciSetAccelerationSet4Way
{
  CvcoctciAcceleration latitude;
  CvcoctciAcceleration longitude;
  CvcoctciVerticalAcceleration vertical;
  CvcoctciYawRate yaw_rate;
};


/**
 * @brief SemiMajorAxisAccuracy
 */
enum eCvcoctciSemiMajorAxisAccuracy
{
  kCvcoctciSemiMajorAxisAccuracy_Min = 0,
  kCvcoctciSemiMajorAxisAccuracy_Max = 255,
  kCvcoctciSemiMajorAxisAccuracy_NA = 255,
};
typedef int CvcoctciSemiMajorAxisAccuracy; ///< @ref eCvcoctciSemiMajorAxisAccuracy


/**
 * @brief SemiMinorAxisAccuracy
 */
enum eCvcoctciSemiMinorAxisAccuracy
{
  kCvcoctciSemiMinorAxisAccuracy_Min = 0,
  kCvcoctciSemiMinorAxisAccuracy_Max = 255,
  kCvcoctciSemiMinorAxisAccuracy_NA = 255,
};
typedef int CvcoctciSemiMinorAxisAccuracy; ///< @ref eCvcoctciSemiMinorAxisAccuracy


/**
 * @brief SemiMajorAxisOrientation
 */
enum eCvcoctciSemiMajorAxisOrientation
{
  kCvcoctciSemiMajorAxisOrientation_Min = 0,
  kCvcoctciSemiMajorAxisOrientation_Max = 65535,
  kCvcoctciSemiMajorAxisOrientation_NA = 65535,
};
typedef int CvcoctciSemiMajorAxisOrientation; ///<@ref eCvcoctciSemiMajorAxisOrientation


/**
 * @brief SetPositionalAccuracy
 */
struct CvcoctciSetPositionalAccuracy
{
  CvcoctciSemiMajorAxisAccuracy semi_major_axis_accuracy;
  CvcoctciSemiMinorAxisAccuracy semi_minor_axis_accuracy;
  CvcoctciSemiMajorAxisOrientation semi_major_axis_orientation;
};


/**
 * @brief SetTestId 메시지 형식
 */
struct CvcoctciSetTestId
{
#define CVCOCTCI_TEST_ID_MAX_LEN 255 ///< CVCOC TCI 시험에서 사용되는 TEST ID 의 최대 길이
  size_t len; ///< TestId 길이
  char str[CVCOCTCI_TEST_ID_MAX_LEN+1]; ///< TestId
};


/**
 * @brief SetVehicleEventFlags
 */
struct CvcoctciSetVehicleEventFlags
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
 * @brief SetWsmTxInfo 메시지 형식
 */
struct CvcoctciSetWsmTxInfo
{
  struct {
    bool cert_id;
    bool info_elements_included;
    bool dst_mac_addr;            // d8011/d16094 - 고정값 true
    bool expiry_time;             // d8011/d16094 - 고정값 false
    bool repeat_rate;             // d8011/d16094 - 고정값 false
  } options;

  CvcoctciPsid psid;
  struct CvcoctciRadioInterface radio;

  CvcoctciContentType content_type;
  CvcoctciSignerIdentifierType signer_id_type;
  CvcoctciHashedId8 cert_id;

  CvcoctciChannelNumber chan_id;
  CvcoctciTimeSlot timeslot;
  CvcoctciDataRate datarate;
  CvcoctciTxPower transmit_power_level;
  CvcoctciUserPriority user_priority;

  uint8_t dst_mac_addr[MAC_ALEN]; // d8011/d16094 - 고정값 FF:FF:FF:FF:FF:FF
  CvcoctciTime64 expiry_time;
  CvcoctciRepeatRate repeat_rate;

  struct CvcoctciWaveElementsIncluded info_elements_included;
};
/// struct @ref CvcoctciSetWsmTxInfo 초기화 매크로
#define CVCOCTCI_SET_WSM_TX_INFO_INITIALIZER {\
  .options = { false, false, false, false, false, false },\
  .psid = kCvcoctciPsid_NA,\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
  .antenna = kCvcoctciAntenna_NA,\
  .content_type = kCvcoctciContentType_NA,\
  .signer_id_type = kCvcoctciSignerIdentifierType_NA,\
  .cert_id = CVCOCTCI_HASHED_ID8_INITIALIZER,\
  .chan_id = kCvcoctciChannelNumber_NA,\
  .timeslot = kCvcoctciTimeSlot_NA,\
  .datarate = kCvcoctciDataRate_NA,\
  .transmit_power_level = kCvcoctciTxPower_NA,\
  .user_priority = kCvcoctciUserPriority_NA,\
  .dst_mac_addr = CVCOCTCI_MAC_ADDR_INITIALIZER,\
  .expiry_time = 0,\
  .repeat_rate = kCvcoctciRepeatRate_NA,\
  .info_elements_included = CVCOCTCI_WAVE_ELEMENTS_INCLUDED_INITIALIZER,\
}


/**
 * @brief StartWsmTx 메시지 형식
 */
struct CvcoctciStartWsmTx
{
  CvcoctciPsid psid;
  struct CvcoctciRadioInterface radio;
  CvcoctciRepeatRate repeat_rate;
};
/// struct @ref CvcoctciStartWsmTx 초기화 매크로
#define CVCOCTCI_START_WSM_TX_INITIALIZER {\
  .psid = kCvcoctciPsid_NA,\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
  .repeat_rate = kCvcoctciRepeatRate_NA,\
}


/**
 * @brief StopWsmTx 메시지 형식
 */
struct CvcoctciStopWsmTx
{
  CvcoctciPsid psid;
  struct CvcoctciRadioInterface radio;
};
/// struct @ref CvcoctciStopWsmTx 초기화 매크로
#define CVCOCTCI_STOP_WSM_TX_INITIALIZER {\
  .psid = kCvcoctciPsid_NA,\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
}


/**
 * @brief StartWsmRx 메시지 형식
 */
struct CvcoctciStartWsmRx
{
  struct {
    bool psid; ///< PSID 옵션필드 존재 여부
  } options;
  CvcoctciPsid psid; ///< PSID
  struct CvcoctciRadioInterface radio; ///< 전송 라디오 인터페이스 정보
  CvcoctciChannelNumber chan_id; ///< 전송 채널
  CvcoctciTimeSlot timeslot; ///< 전송 시간슬롯
  struct CvcoctciEventHandling event_handling; ///< 이벤트 핸들링
};
/// struct @ref CvcoctciStartWsmRx 초기화 매크로
#define CVCOCTCI_START_WSM_RX_INITIALIZER {\
  .options = { false },\
  .psid = kCvcoctciPsid_NA,\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
  .chan_id = kCvcoctciChannelNumber_NA,\
  .timeslot = kCvcoctciTimeSlot_NA,\
  .event_handling = CVCOCTCI_EVENT_HANDLING_INITIALIZER,\
}


/**
 * @brief StopWsmRx 메시지 형식
 */
struct CvcoctciStopWsmRx
{
  struct {
    bool psid; ///< PSID 옵션필드 존재 여부
  } options;

  CvcoctciPsid psid; ///< PSID
  struct CvcoctciRadioInterface radio; ///< 전송 라디오 인터페이스 정보
};
/// struct @ref CvcoctciStopWsmRx 초기화 매크로
#define CVCOCTCI_STOP_WSM_RX_INITIALIZER {\
  .options = { false },\
  .psid = kCvcoctciPsid_NA,\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
}


/**
 * @brief SecurityContext
 */
struct CvcoctciSecurityContext
{
  struct {
    bool cert_id;
  } options;
  CvcoctciContentType content_type;
  CvcoctciSignerIdentifierType  signer_id_type;
  uint8_t cert_id[8];
};


/**
 * @brief Wsa Count Threshold
 */
enum eCvcoctciWsaCountThreshold
{
  kCvcoctciWsaCountThreshold_Min = 0,
  kCvcoctciWsaCountThreshold_Max = 255,
  kCvcoctciWsaCountThreshold_NA = -999,
};
typedef int CvcoctciWsaCountThreshold; ///< @ref eCvcoctciWsaCountThreshold


/**
 * @brief Wsa Count threshold interval
 */
enum eCvcoctciWsaCountThresholdInterval
{
  kCvcoctciWsaCountThresholdInterval_Min = 0,
  kCvcoctciWsaCountThresholdInterval_Max = 255,
  kCvcoctciWsaCountThresholdInterval_NA = -999,
};
typedef int CvcoctciWsaCountThresholdInterval; ///< @ref eCvcoctciWsaCountThresholdInterval



/**
 * @brief ProviderServiceContext
 */
struct CvcoctciPsc
{
#define PSC_MAX_LEN 31 ///< PSC 최대길이
  size_t len;
  uint8_t psc[PSC_MAX_LEN+1];
};
/// struct @ref CvcoctciPsc 초기화 매크로
#define CVCOCTCI_PSC_INITIALIZER {\
  .len = 0,\
  .psc = {0,},\
}


/**
 * @brief ServiceInfoExts 정보
 */
struct CvcoctciServiceInfoExts
{
  struct {
    bool psc;
    bool ipv6_addr;
    bool service_port;
    bool provider_mac_addr;
    bool rcpi_threshold;
    bool wsa_cnt_threshold;
    bool wsa_cnt_threshold_interval;
  } options;

  struct CvcoctciPsc psc;
  uint8_t ipv6_addr[IPv6_ALEN];
  uint16_t service_port;
  uint8_t provider_mac_addr[MAC_ALEN];
  CvcoctciRcpi rcpi_threshold;
  CvcoctciWsaCountThreshold wsa_cnt_threshold;
  CvcoctciWsaCountThresholdInterval wsa_cnt_threshold_interval;
};
/// struct @ref CvcoctciServiceInfoExts 초기화 매크로
#define CVCOCTCI_SERVICE_INFO_EXTS_INITIALIZER {\
  .options = {false, },\
  .psc = CVCOCTCI_PSC_INITIALIZER,\
  .ipv6_addr = {0,},\
  .service_port = 0,\
  .provider_mac_addr = {0, },\
  .rcpi_threshold = kCvcoctciRcpi_NA,\
  .wsa_cnt_threshold = kCvcoctciWsaCountThreshold_NA,\
  .wsa_cnt_threshold_interavl = kCvcoctciWsaCountThresholdInterval_NA,\
}


/**
 * @brief WSA 에 수납되는 ServiceInfo 정보
 */
struct CvcoctciServiceInfo
{
  struct {
    bool exts;
  } options;

  CvcoctciPsid psid;
  CvcoctciChannelNumber chan_num;
  CvcoctciChannelIndex chan_idx;
  struct CvcoctciServiceInfoExts exts;
};
/// struct @ref CvcoctciServiceInfo 초기화 매크로
#define CVCOCTCI_SERVICE_INFO_INITIALIZER {\
  .options = {false},\
  .psid = kCvcoctciPsid_NA,\
  .chan_num = kCvcoctciChannelNumber_NA,\
  .chan_idx = kCvcoctciChannelIndex_NA,\
  .exts = CVCOCTCI_SERVICE_INFO_EXTS_INITIALIZER,\
}


/**
 * @brief WSA 수납되는 ServiceInfo 정보들
 */
struct CvcoctciServiceInfos
{
  size_t cnt;
  struct CvcoctciServiceInfo info[WSA_SERVICE_INFO_MAX_NUM];
};
/// struct @ref CvcoctciServiceInfos 초기화 매크로
#define CVCOCTCI_SERVICE_INFOS_INITIALIZER {\
  .cnt = 0,\
  .info = {CVCOCTCI_SERVICE_INFO_INITIALIZER,},\
}


/**
 * @brief Operating class
 */
enum eCvcoctciOperatingClass
{
  kCvcoctciOperatingClass_Min = 0,
  kCvcoctciOperatingClass_Max = 255,
  kCvcoctciOperatingClass_NA = -999,
};
typedef int CvcoctciOperatingClass; ///< @ref eCvcoctciOperatingClass


/**
 * @brief ChannelAccess80211
 */
enum eCvcoctciChannelAccess
{
  kCvcoctciChannelAccess_Continuous = 0,
  kCvcoctciChannelAccess_AlternatingSch = 1,
  kCvcoctciChannelAccess_AlternatingCch = 2,
  kCvcoctciChannelAccess_Min = 0,
  kCvcoctciChannelAccess_Max = 255,
  kCvcoctciChannelAccess_NA = -999
};
typedef int CvcoctciChannelAccess; ///< @ref eCvcoctciChannelAccess


/**
 * @brief EdcaParameterRecord
 */
struct CvcoctciEdcaParameterRecord
{
  int res;          // 0..1
  int aci;          // 0..3
  int acm;          // 0..1
  int aifsn;        // 0..15
  int ecwmax;      // 0..15
  int ecwmin;      // 0..15
  int txoplimit;   // 0..65535
};
/// struct @ref CvcoctciEdcaParameterRecord 초기화 매크로
#define CVCOCTCI_EDCA_PARAMETER_RECORD_INITIALIZER(x) {\
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
struct CvcoctciEdcaParameterSet
{
  struct CvcoctciEdcaParameterRecord ac_be;
  struct CvcoctciEdcaParameterRecord ac_bk;
  struct CvcoctciEdcaParameterRecord ac_vi;
  struct CvcoctciEdcaParameterRecord ac_vo;
};
/// struct @ref CvcoctciEdcaParameterRecord 초기화 매크로
#define CVCOCTCI_EDCA_PARAMETER_SET {\
  .ac_be = CVCOCTCI_EDCA_PARAMETER_RECORD_INITIALIZER(0),\
  .ac_bk = CVCOCTCI_EDCA_PARAMETER_RECORD_INITIALIZER(1),\
  .ac_vi = CVCOCTCI_EDCA_PARAMETER_RECORD_INITIALIZER(2),\
  .ac_vo = CVCOCTCI_EDCA_PARAMETER_RECORD_INITIALIZER(3),\
}


/**
 * @brief ChannelInfo 내 확장필드 들
 */
struct CvcoctciChannelInfoExts
{
  struct {
    bool edca_param_set;
    bool chan_access;
  } options;
  struct CvcoctciEdcaParameterSet edca_param_set;
  CvcoctciChannelAccess chan_access;
};
/// struct @ref CvcoctciChannelInfoExts 초기화 매크로
#define CVCOCTCI_CHANNEL_INFO_EXTS_INITIALIZER {\
  .options = {false, },\
  .edca_param_set = CVCOCTCI_EDCA_PARAMETER_SET,\
  .chan_access = kCvcoctciChannelAccess_NA,\
}


/**
 * @brief WSA 에 수납되는 ChannelInfo 정보
 */
struct CvcoctciChannelInfo
{
  struct {
    bool exts;
  } options;

  CvcoctciOperatingClass op_class;
  CvcoctciChannelNumber chan_num;
  CvcoctciTxPower power_level;
  bool adaptable_datarate;
  CvcoctciDataRate datarate;
  struct CvcoctciChannelInfoExts exts;
};
/// struct @ref CvcoctciChannelInfo 초기화 매크로
#define CVCOCTCI_CHAN_INFO_INITIALIZER {\
  .op_class = kCvcoctciOperatingClass_NA,\
  .chan_num = kCvcoctciChannelNumber_NA,\
  .power_level = kCvcoctciTxPower_NA,\
  .adaptable_datarate = false,\
  .datarate = kCvcoctciDataRate_NA,\
  .exts = CVCOCTCI_CHANNEL_INFO_EXTS_INITIALIZER,\
}


/**
 * @brief WSA 에 수납되는 ChannelInfo 정보들
 */
struct CvcoctciChannelInfos
{
  size_t cnt;
  struct CvcoctciChannelInfo info[WSA_CHAN_INFO_MAX_NUM];
};
/// struct @ref CvcoctciChannelInfos 초기화 매크로
#define CVCOCTCI_CHAN_INFOS_INITIALIZER {\
  .cnt = 0,\
  .info = {CVCOCTCI_CHAN_INFO_INITIALIZER,},\
}


/**
 * @brief Router lifetime
 */
enum eCvcoctciRouterLifetime
{
  kCvcoctciRouterLifetime_Min = 0,
  kCvcoctciRouterLifetime_Max = 65535,
  kCvcoctciRouterLifetime_NA = -999,
};
typedef int CvcoctciRouterLifetime; ///< @ref eCvcoctciRouterLifetime


/**
 * @brief IP prefix length
 */
enum eCvcoctciIPv6PrefixLength
{
  kCvcoctciIPv6PrefixLength_Min = 0,
  kCvcoctciIPv6PrefixLength_Max = 255,
  kCvcoctciIPv6PrefixLength_NA = -999,
};
typedef int CvcoctciIPv6PrefixLength; ///< @ref eCvcoctciIPv6PrefixLength


/**
 * @brief Routing advertisement 확장 정보
 */
struct CvcoctciRoutingAdvertisementExts
{
  struct {
    bool second_dns; ///< Secondary DNS 주소 옵션정보 수납 여부
    bool gw_mac_addr; ///< Gateway MAC 주소 옵션정보 수납 여부
  } options;
  uint8_t secondary_dns[IPv6_ALEN]; ///< Secondary DNS 주소
  uint8_t gw_mac_addr[MAC_ALEN]; ///< Gateway MAC 주소
};
/// struct @ref CvcoctciRoutingAdvertisementExts 초기화 매크로
#define CVCOCTCI_ROUTING_ADVERTISEMENT_EXTS_INITIALIZER {\
  .options = {false, false},\
  .secondary_dns = CVCOCTCI_IPv6_ADDR_INITIALIZER,\
  .gw_mac_addr = CVCOCTCI_MAC_ADDR_INITIALIZER,\
}


/**
 * @brief WAVE Routing Advertisement
 */
struct CvcoctciRoutingAdvertisement
{
  struct {
    bool exts;
  } options;

  CvcoctciRouterLifetime router_lifetime;
  uint8_t ip_prefix[IPv6_ALEN];
  CvcoctciIPv6PrefixLength ip_prefix_len;
  uint8_t default_gw[IPv6_ALEN];
  uint8_t primary_dns[IPv6_ALEN];
  struct CvcoctciRoutingAdvertisementExts exts;
};
/// struct @ref CvcoctciRoutingAdvertisement 초기화 매크로
#define CVCOCTCI_ROUTING_ADVERTISEMENT_INITIALIZER {\
  .options = {false,},\
  .router_lifetime = kCvcoctciRouterLifetime_NA,\
  .ip_prefix = {0,},\
  .ip_prefix_len = kCvcoctciIPv6PrefixLength_NA,\
  .default_gw = {0,},\
  .primary_dns = {0,},\
  .exts = {0,},\
}


/**
 * @brief AdvertiserId
 */
struct CvcoctciAdvertiserId
{
#define ADVERTISER_ID_MAX_LEN 32 ///< AdvertiserId 최대 길이
  size_t len; ///< AdvertiserId 길이
  uint8_t id[ADVERTISER_ID_MAX_LEN+1]; ///< AdvertiserId
};
/// struct @ref CvcoctciAdvertiserId 초기화 매크로
#define CVCOCTCI_ADVERTISER_ID_INITIALIZER {\
  .len = 0,\
  .id = {0,},\
}


/**
 * @brief StartWsaTxPeriodic 메시지 형식
 */
struct CvcoctciStartWsaTxPeriodic
{
  struct {
    bool dst_mac_addr;
    bool info_element_included;
    bool advertiser_id;
    bool wra;
    bool datarate;
    bool priority;
    bool transmit_power_level;
  } options;

  struct CvcoctciRadioInterface radio;
  uint8_t dst_mac_addr[MAC_ALEN];
  CvcoctciChannelNumber wsa_chan_id;
  CvcoctciTimeSlot chan_access;
  CvcoctciRepeatRate repeat_rate;
  bool ip_service;
  struct CvcoctciSecurityContext security;
  int signature_lifetime;
  struct CvcoctciWaveElementsIncluded info_element_included;
  struct CvcoctciAdvertiserId advertiser_id;
  struct CvcoctciServiceInfos service_infos;
  struct CvcoctciChannelInfos chan_infos;
  struct CvcoctciRoutingAdvertisement wra;
  CvcoctciDataRate datarate;
  CvcoctciUserPriority priority;
  CvcoctciTxPower transmit_power_level;
};
/// struct @ref CvcoctciStartWsaTxPeriodic 초기화 매크로
#define CVCOCTCI_START_WSA_TX_PERIODIC_INITIALIZER = {\
  .options = {false,false,false,false,false,false,false},\
  .radio = kCvcoctciRadio_NA,\
  .dst_mac_addr = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},\
  .wsa_chan_id = kCvcoctciChannelNumber_NA,\
  .chan_access = kCvcoctciTimeSlot_NA,\
  .repeat_rate = kCvcoctciRepeatRate_NA,\
  .ip_service = false,\
  .security = CVCOCTCI_SECURITY_CONTEXT_INITIALIZER,\
  .signature_lifetime = 0,\
  .info_element_included = CVCOCTCI_WAVE_ELEMENTS_INCLUDED_INITIALIZER,\
  .advertiser_id = CVCOCTCI_ADVERTISER_ID_INITIALIZER,\
  .service_infos = CVCOCTCI_SERVICE_INFOS_INITIALIZER,\
  .chan_infos = CVCOCTCI_CHAN_INFOS_INITIALIZER,\
  .routing_advertisement = CVCOCTCI_ROUTING_ADVERTISEMENT_INITIALIZER,\
  .datarate = kCvcoctciDataRate_NA,\
  .priority = kCvcoctciUserPriority_NA,\
  .transmit_power_level = kCvcoctciTxPower_NA,\
}


/**
 * @brief StopWsaTxPeriodic 요청 데이터 형식
 */
struct CvcoctciStopWsaTxPeriodic
{
  struct CvcoctciRadioInterface radio;
};
/// struct @ref CvcoctciStopWsaTxPeriodic 초기화 매크로
#define CVCOCTCI_STOP_WSA_TX_PERIODIC_INITIALIZER {\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
}


/**
 * @brief AddWsaProviderService 요청 데이터 형식
 */
struct CvcoctciAddWsaProviderService
{
  struct CvcoctciRadioInterface radio;
  struct CvcoctciServiceInfos infos;
};
/// struct @ref CvcoctciAddWsaProviderService 초기화 매크로
#define CVCOCTCI_ADD_WSA_PROVIDER_SERVICE_INITIALIZER {\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
  .info = CVCOCTCI_SERVICE_INFOS_INITIALIZER,\
}


/**
 * @brief ChangeWsaProviderService 요청 데이터 형식
 */
struct CvcoctciChangeWsaProviderService
{
  struct CvcoctciRadioInterface radio;
  struct CvcoctciServiceInfos infos;
};
/// struct @ref CvcoctciChangeWsaProviderService 초기화 매크로
#define CVCOCTCI_CHANGE_WSA_PROVIDER_SERVICE_INITIALIZER {\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
  .info = CVCOCTCI_SERVICE_INFOS_INITIALIZER,\
}


/**
 * @brief DelWsaProviderService 요청 데이터 형식
 */
struct CvcoctciDelWsaProviderService
{
  struct CvcoctciRadioInterface radio;
  struct CvcoctciServiceInfos infos;
};
/// struct @ref CvcoctciDelWsaProviderService 초기화 매크로
#define CVCOCTCI_DEL_WSA_PROVIDER_SERVICE_INITIALIZER {\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
  .info = CVCOCTCI_SERVICE_INFOS_INITIALIZER,\
}


/**
 * @brief UserRequestType
 */
enum eCvcoctciUserRequestType
{
  kCvcoctciUserRequestType_AutoAccessOnMatch = 0, ///< 원하는 서비스 발견 시 자동 채널 접속 수행
  kCvcoctciUserRequestType_NoSchAccess = 1, ///< 원하는 서비스 발견 시 자동 채널 접속 미수행
  kCvcoctciUserRequestType_Min = kCvcoctciUserRequestType_AutoAccessOnMatch,
  kCvcoctciUserRequestType_Max = kCvcoctciUserRequestType_NoSchAccess,
  kCvcoctciUserRequestType_NA = -999,
};
typedef int CvcoctciUserRequestType; ///< @ref eCvcoctciUserRequestType


/**
 * @brief WsaType
 */
enum eCvcoctciWsaType
{
  kCvcoctciWsaType_SecureWsa = 1, ///< 서명 WSA
  kCvcoctciWsaType_UnsecureWsa = 2, ///< 비보안 WSA
  kCvcoctciWsaType_Min = kCvcoctciWsaType_SecureWsa,
  kCvcoctciWsaType_Max = kCvcoctciWsaType_UnsecureWsa,
  kCvcoctciWsaType_NA = -999,
};
typedef int CvcoctciWsaType; ///< @ref eCvcoctciWsaType



/**
 * @brief Immediate access 를 수행하는 기간 (Sync interval 단위)
 *
 * 예: Immediate access = 3 -> 3 sync interval(보통 300msec) 동안 immediate access 수행
 */
enum eCvcoctciImmediateAccess
{
  kCvcoctciImmediateAccess_Min = 0,
  kCvcoctciImmediateAccess_Max = 255,
  kCvcoctciImmediateAccess_NotImmediateAccess = 0,
  kCvcoctciImmediateAccess_NA = -999,
};
typedef int CvcoctciImmediateAccess; ///< @ref eCvcoctciImmediateAccess


/**
 * @brief AddUserService 요청 데이터 형식
 */
struct CvcoctciAddUserService
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
  } options;

  CvcoctciPsid psid;
  struct CvcoctciRadioInterface radio;
  CvcoctciUserRequestType req_type;
  CvcoctciWsaType wsa_type;
  struct CvcoctciPsc psc;
  CvcoctciChannelNumber chan_id;
  uint8_t src_mac_addr[MAC_ALEN];
  struct CvcoctciAdvertiserId advertiser_id;
  int link_quality;
  CvcoctciImmediateAccess immediate_access;
  CvcoctciChannelNumber wsa_chan_id;
  CvcoctciTimeSlot chan_access;
  struct CvcoctciEventHandling event_handling;
};
/// struct @ref CvcoctciAddUserService 초기화 매크로
#define CVCOCTCI_ADD_USER_SERVICE_INITIALIZER {\
  .options = {false,false,false,false,false,false,false,false},\
  .psid = kCvcoctciPsid_NA,\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
  .req_type = kCvcoctciUserRequestType_NA,\
  .wsa_type = kCvcoctciWsaType_NA,\
  .psc = CVCOCTCI_PSC_INITIALIZER,\
  .chan_id = kCvcoctciChannelNumber_NA,\
  .src_mac_addr =  CVCOCTCI_MAC_ADDR_INITIALIZER,\
  .advertiser_id = CVCOCTCI_ADVERTISER_ID_INITIALIZER,\
  .link_quality = -999,\
  .immediate_access = kCvcoctciImmediateAccess_NA,\
  .wsa_chan_id = kCvcoctciChannelNumber_NA,\
  .chan_access = kCvcoctciTimeSlot_NA,\
  .event_handling = CVCOCTCI_EVENT_HANDLING_INITIALIZER,\
}


/**
 * @brief DelUserService 요청 데이터 형식
 */
struct CvcoctciDelUserService
{
  CvcoctciPsid psid; ///< PSID
  struct CvcoctciRadioInterface radio; ///< 라디오 인터페이스
};
/// struct @ref CvcoctciDelUserService 초기화 매크로
#define CVCOCTCI_DEL_USER_SERVICE_INITIALIZER {\
  .psid = kCvcoctciPsid_NA,\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
}


/**
 * @brief GetIPv6InterfaceInfo 요청 데이터 형식
 */
struct CvcoctciGetIPv6InterfaceInfo
{
  struct CvcoctciRadioInterface radio; ///< 라디오 인터페이스
};
/// struct @ref CvcoctciGetIPv6InterfaceInfo 초기화 매크로
#define CVCOCTCI_GET_IPv6_INTERFACE_INFO_INITIALIZER {\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
}


/**
 * @brief InterfaceName
 */
struct CvcoctciInterfaceName
{
#define CVCOCTCI_IF_NAME_MAX_LEN 255 ///< 인터페이스 이름 최대 길이
  size_t len; ///< 인터페이스 이름 길이
  uint8_t name[CVCOCTCI_IF_NAME_MAX_LEN+1]; ///< 인터페이스 이름
};
/// struct @ref CvcoctciInterfaceName 초기화 매크로
#define CVCOCTCI_IF_NAME_INITIALIZER {\
  .len = 0,\
  .name = {0,},\
}


/**
 * @brief SetIPv6Address 요청 데이터 형식
 */
struct CvcoctciSetIPv6Address
{
  struct {
    bool ip_addr;
  } options;
  struct CvcoctciRadioInterface radio;
  struct CvcoctciInterfaceName if_name;
  uint8_t ip_addr[IPv6_ALEN];
};
/// struct @ref CvcoctciSetIPv6Address 초기화 매크로
#define CVCOCTCI_SET_IPv6_ADDRESS_INITIALIZER {\
  .options = {false},\
  .radio = CVCOCTCI_RADIO_INTERFACE_INITIALIZER,\
  .if_name = CVCOCTCI_IF_NAME_INITIALIZER,\
  .ip_addr = CVCOCTCI_IPv6_ADDR_INITIALIZER,\
}


/**
 * @brief Protocol
 */
enum eCvcoctciProtocol
{
  kCvcoctciProtocol_tcp = 0,
  kCvcoctciProtocol_udp = 1,
  kCvcoctciProtocol_icmpv6 = 2,
  kCvcoctciProtocol_NA = -999,
};
typedef int CvcoctciProtocol; ///< @ref eCvcoctciProtocol


/**
 * @brief IPv6TxRecord
 */
struct CvcoctciIPv6TxRecord
{
  struct {
    bool dst_port;
    bool repeat_rate;
    bool event_handling;
  } options;

  struct CvcoctciRadioInterface radio;
  struct CvcoctciInterfaceName if_name;
  uint8_t dst_ip_addr[IPv6_ALEN];
  uint16_t dst_port;
  CvcoctciProtocol protocol;
  CvcoctciRepeatRate repeat_rate;
  struct CvcoctciEventHandling event_handling;
};
/// CvcoctciStartIPv6Ping 정보
#define CvcoctciStartIPv6Ping CvcoctciIPv6TxRecord
/// CvcoctciStopIPv6Ping 정보
#define CvcoctciStopIPv6Ping CvcoctciIPv6TxRecord
/// CvcoctciStartIPv6Tx 정보
#define CvcoctciStartIPv6Tx CvcoctciIPv6TxRecord
/// CvcoctciStopIPv6Tx 정보
#define CvcoctciStopIPv6Tx CvcoctciIPv6TxRecord


/**
 * @brief IPv6RxRecord
 */
struct CvcoctciIPv6RxRecord
{
  struct {
    bool event_handling;
  } options;

  struct CvcoctciRadioInterface radio;
  struct CvcoctciInterfaceName if_name;
  uint16_t listen_port;
  CvcoctciProtocol protocol;
  struct CvcoctciEventHandling event_handling;
};
/// CvcoctciStartIPv6Rx 정보
#define CvcoctciStartIPv6Rx CvcoctciIPv6RxRecord
/// CvcoctciStopIPv6Rx 정보
#define CvcoctciStopIPv6Rx CvcoctciIPv6RxRecord


/**
 * @brief OperationalRateSet
 */
struct CvcoctciOperationalRateSet
{
#define CVCOCTCI_OPERATIONAL_RATE_SET_MAX_LEN 50
  size_t len;
  uint8_t op_rate_set[CVCOCTCI_OPERATIONAL_RATE_SET_MAX_LEN];
};


/**
 * @brief AddTxProfile
 */
struct CvcoctciAddTxProfile
{
  struct {
    bool op_rate_set;
    bool edca_param_set;
    bool immediate_access;
  } options;

  struct CvcoctciRadioInterface radio;
  CvcoctciChannelNumber chan_id;
  CvcoctciTimeSlot timeslot;
  struct CvcoctciOperationalRateSet op_rate_set;
  struct CvcoctciEdcaParameterSet edca_param_set;
  CvcoctciImmediateAccess immediate_access;
  bool adaptable;
  CvcoctciTxPower transmit_power_level;
  CvcoctciDataRate datarate;
};


/**
 * @brief DelTxProfile
 */
struct CvcoctciDelTxProfile
{
  struct CvcoctciRadioInterface radio;
  CvcoctciChannelNumber chan_id;
};


/**
 * @brief StartBsmTx 요청 데이터 형식
 */
struct CvcoctciStartBsmTx
{
  CvcoctciPsid psid;
  struct CvcoctciRadioInterface radio;
  CvcoctciRepeatRate repeat_rate;
};


/**
 * @brief StopBsmTx 요청 데이터 형식
 */
struct CvcoctciStopBsmTx
{
  CvcoctciPsid psid;
  struct CvcoctciRadioInterface radio;
};


/**
 * @brief StartBsmRx 메시지 형식
 */
struct CvcoctciStartBsmRx
{
  struct {
    bool psid;
  } options;

  CvcoctciPsid psid;
  struct CvcoctciRadioInterface radio;
  CvcoctciChannelNumber chan_id;    // = 172
  CvcoctciTimeSlot timeslot;    // = continuous
  struct CvcoctciEventHandling event_handling;
};


/**
 * @brief StopBsmRx 요청 데이터 형식
 */
struct CvcoctciStopBsmRx
{
  struct {
    bool psid;
  } options;

  CvcoctciPsid psid;    // =32
  struct CvcoctciRadioInterface radio;
};


/**
 * @brief ResultCode
 */
enum eCvcoctciTciResultCode
{
  kCvcoctciTciResultCode_Success = 0,
  kCvcoctciTciResultCode_Failure = 1,
  kCvcoctciTciResultCode_Min = kCvcoctciTciResultCode_Success,
  kCvcoctciTciResultCode_Max = kCvcoctciTciResultCode_Failure,
};
typedef int CvcoctciTciResultCode; ///< @ref eCvcoctciTciResultCode


/**
 * @brief ExceptionType
 */
enum eCvcoctciExceptionType
{
  kCvcoctciExceptionType_Info = 0,
  kCvcoctciExceptionType_Warning = 1,
  kCvcoctciExceptionType_Error = 2,
  kCvcoctciExceptionType_Min = kCvcoctciExceptionType_Info,
  kCvcoctciExceptionType_Max = kCvcoctciExceptionType_Error,
  kCvcoctciExceptionType_NA = -999,
};
typedef int CvcoctciExceptionType; ///< @ref eCvcoctciExceptionType


/**
 * @brief ExceptionId
 */
enum eCvcoctciExceptionId
{
  kCvcoctciExceptionId_CriticalError = 1,
  kCvcoctciExceptionId_IncorrectParameterValue = 2,
  kCvcoctciExceptionId_MissingParameter = 3,
  kCvcoctciExceptionId_RadioInterfaceUnavailable = 4,
  kCvcoctciExceptionId_Min = kCvcoctciExceptionId_CriticalError,
  kCvcoctciExceptionId_Max = kCvcoctciExceptionId_RadioInterfaceUnavailable,
  kCvcoctciExceptionId_NA = -999,
};
typedef int CvcoctciExceptionId; ///< @ref eCvcoctciExceptionId


/**
 * @brief Exception 메시지 형식
 */
struct CvcoctciException
{
  struct {
    bool id;
    bool module;
    bool description;
  } options;  ///< 각 옵션 필드 설정 여부

  CvcoctciExceptionType type;  ///< 예외 유형
  CvcoctciExceptionId id;      ///< 예외 식별자
  const char *module;          ///< module 문자열을 가리키는 포인터
#define EXCEPTION_MODULE_MAX_SIZE 255
  size_t module_size;          ///< module 길이 (0~255)
  const char *description;     ///< description 문자열을 가리키는 포인터
#define EXCEPTION_DESC_MAX_SIZE 1200
  size_t description_size;     ///< description 길이 (0~1200)
};
/// struct @ref CvcoctciException 초기화 매크로
#define CVCOCTCI_EXCEPTION_INITIALIZER {\
  .options = { false, false, false },\
  .type = kCvcoctciExceptionType_NA,\
  .id = kCvcoctciExceptionId_NA,\
  .module = NULL,\
  .module_size = 0,\
  .description = NULL,\
  .description_size = 0,\
}


/**
 * @brief Event
 */
enum eCvcoctciEvent
{
  kCvcoctciEvent_80211PktRx = 1,
  kCvcoctciEvent_16093PktRx = 2,
  kCvcoctciEvent_WsmPktRx = 3,
  kCvcoctciEvent_IPv6PktRx = 4,
  kCvcoctciEvent_Icmp6PktRx = 5,
  kCvcoctciEvent_IPv6ConfigChanged = 6,
  kCvcoctciEvent_Dot3ChannelAssigned = 7,
  kCvcoctciEvent_Dot3RequestMatchedAvailAppService = 8,
  kCvcoctciEvent_Dot2VerificationCompleteWithResult = 9,
  kCvcoctciEvent_Exception = 15,
  kCvcoctciEvent_NA = -999,
};
typedef int CvcoctciEvent; ///< @ref eCvcoctciEvent


/**
 * @brief EventParams 유형 (Choice tag)
 */
enum eCvcoctciEventParamsType
{
  kCvcoctciEventParamsType_Service = 0,
  kCvcoctciEventParamsType_Wsm = 1,
  kCvcoctciEventParamsType_Ip = 2,
  kCvcoctciEventParamsType_80211frame = 3,
  kCvcoctciEventParamsType_Security = 4,
  kCvcoctciEventParamsType_NA = -999,
};
typedef int CvcoctciEventParamsType; ///< @ref eCvcoctciEventParamsType


/**
 * @brief Pdu
 */
struct CvcoctciPdu
{
  CvcoctciPduType pdu_type;
  uint8_t *pdu_data;          ///< PDU 데이터를 가리키는 포인터
  size_t pdu_data_size;       ///< PDU 데이터 크기 (0~2304) @ref MAX_PDU_SIZE
};
/// struct @ref CvcoctciPdu 초기화 매크로
#define CVCOCTCI_PDU_INITIALIZER {\
  .pdu_type = kCvcoctciPduType_NA,\
  .pdu_data = NULL,\
  .pdu_data_size = 0,\
}


/**
 * @brief ServiceParameters
 */
struct CvcoctciServiceParameters
{
#define PSID_MAX_CNT 255
  unsigned int psid_cnt; ///< Psid 개수
  CvcoctciPsid psid[PSID_MAX_CNT]; ///< Psid 들
};
/// struct @ref CvcoctciServiceParameters 초기화 매크로
#define CVCOCTCI_SERVICE_PARAMETERS_INITIALIZER {\
  .psid_cnt = 0,\
  .psid = {kCvcoctciPsid_NA,},\
}

/**
 * @brief WsmParameters
 */
struct CvcoctciWsmParameters
{
  CvcoctciPsid psid;                ///< 수신된 WSM 의 Psid
  CvcoctciRadio radio;              ///< 수신된 라디오 인터페이스
  uint8_t wsmp_version;             ///< 수신된 WSMP 의 버전 (0~15)
  CvcoctciChannelNumber chan_id;    ///< 수신된 채널
  CvcoctciDataRate datarate;        ///< 수신된 데이터레이트
  CvcoctciTxPower rx_power_level;   ///< 수신파워
  uint8_t src_mac_addr[MAC_ALEN]; ///< 송신지 MAC 주소
};
/// struct @ref CvcoctciWsmParameters 초기화 매크로
#define CVCOCTCI_WSM_PARAMETERS_INITIALIZER {\
  .psid = kCvcoctciPsid_NA,\
  .radio = kCvcoctciRadio_NA,\
  .wsmp_version = 0,\
  .chan_id = kCvcoctciChannelNumber_NA,\
  .datarate = kCvcoctciDataRate_NA,\
  .rx_power_level = kCvcoctciTxPower_NA,\
  .src_mac_addr = CVCOCTCI_MAC_ADDR_INITIALIZER,\
}


/**
 * @brief IpParameters
 */
struct CvcoctciIpParameters
{
#define IF_NAME_MAX_SIZE 255
  char if_name[IF_NAME_MAX_SIZE+1];        ///< 수신 인터페이스 이름
  uint8_t src_ip_addr[IPv6_ALEN];          ///< 송신지 IP 주소
  CvcoctciProtocol protocol;               ///< 수신된 IP 패킷의 상위 프로토콜
};
/// struct @ref CvcoctciIpParameters 초기화 매크로
#define CVCOCTCI_IP_PARAMETERS_INITIALIZER {\
  .if_name = {0, },\
  .src_ip_addr = CVCOCTCI_IPv6_ADDR_INITIALIZER\
  .protocol = kCvcoctciProtocol_NA,\
}


/**
 * @brief Dot11Parameters
 */
struct CvcoctciD80211Parameters
{
  CvcoctciRadio radio;    ///< 802.11 프레임이 수신된 라디오 인터페이스 번호
  CvcoctciRcpi rcpi;      ///< RCPI
};
/// struct @ref CvcoctciD80211Parameters 초기화 매크로
#define CVCOCTCI_D80211_PARAMETERS_INITIALIZER {\
  .radio = kCvcoctciRadio_NA,\
  .rcpi = kCvcoctciRcpi_NA,\
}


/**
 * @brief SecurityResultCode
 */
enum eCvcoctciSecurityResultCode
{
  kCvcoctciSecurityResultCode_Success = 1,
  kCvcoctciSecurityResultCode_InconsistentInputParameters,
  kCvcoctciSecurityResultCode_SpduParsingInvalidInput,
  kCvcoctciSecurityResultCode_SpduParsingUnsupportedCriticalInformationField,
  kCvcoctciSecurityResultCode_SpduParsingCertificateNotFound,
  kCvcoctciSecurityResultCode_SpduParsingGenerationTimeNotAvailable,
  kCvcoctciSecurityResultCode_SpduParsingGenerationLocationNotAvailable,
  kCvcoctciSecurityResultCode_SpduCertificateChainNotEnoughInformationToConstructChain,
  kCvcoctciSecurityResultCode_SpduCertificateChainChainEndedAtUntrustedRoot,
  kCvcoctciSecurityResultCode_SpduCertificateChainChainWasTooLongForImplementation,
  kCvcoctciSecurityResultCode_SpduCertificateChainCertificateRevoked,
  kCvcoctciSecurityResultCode_SpduCertificateChainOverdueCRL,
  kCvcoctciSecurityResultCode_SpduCertificateChainInconsistentExpiryTimes,
  kCvcoctciSecurityResultCode_SpduCertificateChainInconsistentStartTimes,
  kCvcoctciSecurityResultCode_SpduCertificateChainInconsistentChainPermissions,
  kCvcoctciSecurityResultCode_SpduCryptoVerificationFailure,
  kCvcoctciSecurityResultCode_SpduConsistencyFutureCertificateAtGenerationTime,
  kCvcoctciSecurityResultCode_SpduConsistencyExpiredCertificateAtGenerationTime,
  kCvcoctciSecurityResultCode_SpduConsistencyExpiryDateTooEarly,
  kCvcoctciSecurityResultCode_SpduConsistencyExpiryDateTooLate,
  kCvcoctciSecurityResultCode_SpduConsistencyGenerationLocationOutsideValidityRegion,
  kCvcoctciSecurityResultCode_SpduConsistencyNoGenerationLocation,
  kCvcoctciSecurityResultCode_SpduConsistencyUnauthorizedPSID,
  kCvcoctciSecurityResultCode_SpduInternalConsistencyExpiryTimeBeforeGenerationTime,
  kCvcoctciSecurityResultCode_SpduInternalConsistencyextDataHashDoesntMatch,
  kCvcoctciSecurityResultCode_SpduInternalConsistencynoExtDataHashProvided,
  kCvcoctciSecurityResultCode_SpduInternalConsistencynoExtDataHashPresent,
  kCvcoctciSecurityResultCode_SpduLocalConsistencyPSIDsDontMatch,
  kCvcoctciSecurityResultCode_SpduLocalConsistencyChainWasTooLongForSDEE,
  kCvcoctciSecurityResultCode_SpduRelevanceGenerationTimeTooFarInPast,
  kCvcoctciSecurityResultCode_SpduRelevanceGenerationTimeTooFarInFuture,
  kCvcoctciSecurityResultCode_SpduRelevanceExpiryTimeInPast,
  kCvcoctciSecurityResultCode_SpduRelevanceGenerationLocationTooDistant,
  kCvcoctciSecurityResultCode_SpduRelevanceReplayedSpdu,
  kCvcoctciSecurityResultCode_SpduCertificateExpired,
  kCvcoctciSecurityResultCode_NA = -999,
};
typedef int CvcoctciSecurityResultCode; ///< @ref eCvcoctciSecurityResultCode


/**
 * @brief SecResultParams
 */
struct CvcoctciSecResultParams
{
  struct {
    bool description;
  } options;  ///< 옵션 필드 수납 여부

  CvcoctciSecurityResultCode sec_result_code; ///< 보안처리 결과
#define SEC_RESULT_DESCRIPTION_MAX_SIZE 255
  char description[SEC_RESULT_DESCRIPTION_MAX_SIZE+1]; ///< 설명 문자열을 가리키는 포인터
};
/// struct @ref CvcoctciSecResultParams 초기화 매크로
#define CVCOCTCI_SEC_RESULT_PARAMS_INITIALIZER {\
  .options = {false},\
  .sec_result_code = kCvcoctciSecurityResultCode_NA,\
  .description = {0},\
}


/**
 * @brief InfoContent 유형 (Choice tag)
 */
enum eCvcoctciInfoContentType
{
  kCvcoctciInfoContentType_IPv6InterfaceInfo,
  kCvcoctciInfoContentType_SutInfo,
  kCvcoctciInfoContentType_NA = -999,
};
typedef int CvcoctciInfoContentType; ///< @ref eCvcoctciInfoContentType


/**
 * @brief IPv6Address 리스트
 */
struct CvcoctciIPv6AddressList
{
#define IF_IPv6_ADDR_MAX_NUM 10
  uint8_t cnt;
  uint8_t addr[IF_IPv6_ADDR_MAX_NUM][IPv6_ALEN];
};
#define CVCOCTCI_IPv6_ADDR_LIST_INITIALIZER {\
  .cnt = 0,\
  .addr = {CVCOCTCI_IPv6_ADDR_INITIALIZER},\
}


/**
 * @brief Ipv6InterfaceInfo
 *
 * 인터페이스에 대한 IPv6 인터페이스 정보
 */
struct CvcoctciIPv6InterfaceInfo
{
  struct {
    bool default_gw;
    bool primary_dns;
    bool gw_mac_addr;
  } options;    ///< 옵션필드 수납 여부

  char if_name[IF_NAME_MAX_SIZE+1];              ///< 인터페이스 이름
  struct CvcoctciIPv6AddressList ip_addr_list;   ///< IPv6 주소(들)
  uint8_t mac_addr[MAC_ALEN];                    ///< MAC 주소
  uint8_t default_gw[IPv6_ALEN];                 ///< Default gateway
  uint8_t primary_dns[IPv6_ALEN];                ///< Primary DNS
  uint8_t gw_mac_addr[MAC_ALEN];                 ///< Gateway MAC address
};
/// struct @ref CvcoctciIPv6InterfaceInfo 초기화 매크로
#define CVCOCTCI_IPv6_IF_INFO_INITIALIZER {\
  .options = {false, false, false},\
  .if_name = {0},\
  .ip_addr = CVCOCTCI_IPv6_ADDR_LIST_INITIALIZER,\
  .mac_addr = CVCOCTCI_MAC_ADDR_INITIALIZER,\
  .default_gw = CVCOCTCI_IPv6_ADDR_INITIALIZER,\
  .primary_dns = CVCOCTCI_IPv6_ADDR_INITIALIZER,\
  .gw_mac_addr = CVCOCTCI_MAC_ADDR_INITIALIZER,\
}



/**
 * @brief Ipv6InterfaceInfo(들)
 *
 * 다수의 인터페이스들에 대한 IPv6 인터페이스 정보 집합
 */
struct CvcoctciIPv6InterfaceInfos
{
#define IF_MAX_NUM 10
  uint8_t info_cnt;     ///< 인터페이스 정보 개수
  struct CvcoctciIPv6InterfaceInfo info[IF_MAX_NUM];  ///< 인터페이스 정보(들)
};
/// struct @ref CvcoctciIPv6InterfaceInfos 초기화 매크로
#define CVCOCTCI_IPv6_IF_INFOS_INITIALIZER {\
  .info_cnt = 0,\
  .info = {CVCOCTCI_IPv6_IF_INFO_INITIALIZER},\
}


/**
 * @brief ComponentType
 */
enum eCvcoctciComponentType
{
  kCvcoctciComponentType_Hardware = 0,
  kCvcoctciComponentType_Firmware = 1,
  kCvcoctciComponentType_Software = 2,
  kCvcoctciComponentType_TciApp = 3,
  kCvcoctciComponentType_NA = -999,
};
typedef int CvcoctciComponentType; ///< @ref eCvcoctciComponentType


/**
 * @brief VersionInfoBlock
 */
struct CvcoctciVersionInfoBlock
{
  struct {
    bool release_date;
    bool description;
  } options;  ///< 옵션필드 수납 설정

  CvcoctciComponentType component_type;           ///< 컴포넌트 유형
#define VERSION_ID_MAX_SIZE 50
  char version_id[VERSION_ID_MAX_SIZE+1];      ///< 버전 ID
#define RELEASE_DATE_MAX_SIZE 50
  char release_date[RELEASE_DATE_MAX_SIZE+1];  ///< 배포일자
#define DESCRIPTION_MAX_SIZE 100
  char description[DESCRIPTION_MAX_SIZE+1];    ///< 기타 설명
};
/// struct @ref CvcoctciVersionInfoBlock 초기화 매크로
#define CVCOCTCI_VERSION_INFO_BLOCK_INITIALIZER {\
  .options = {false, false},\
  .component_type = kCvcoctciComponentType_NA,\
  .version_id = {0},\
  .release_date = {0},\
  .description = {0},\
}


/**
 * @brief VersionInfoBlock(들)
 */
struct CvcoctciVersionInfoBlocks
{
#define VERSION_INFO_MAX_NUM 10
  int info_cnt;     ///< 정보 개수
  struct CvcoctciVersionInfoBlock info[VERSION_INFO_MAX_NUM]; ///< 정보(들)
};
/// struct @ref CvcoctciVersionInfoBlocks 초기화 매크로
#define CVCOCTCI_VERSION_INFO_BLOCKS_INITIALIZER {\
  .info_cnt = 0,\
  .info = { CVCOCTCI_VERSION_INFO_BLOCK_INITIALIZER, },\
}


/**
 * @brief SutInfo
 */
struct CvcoctciSutInfo
{
  struct {
    bool model_name;
  } options;    ///< 옵션필드 수납 여부

#define MODEL_NAME_MAX_SIZE 255
  char model_name[MODEL_NAME_MAX_SIZE+1];  ///< 모델명
  struct CvcoctciVersionInfoBlocks version_infos;  ///< 버전정보(들)
};
/// struct @ref CvcoctciSutInfo 초기화 매크로
#define CVCOCTCI_SUT_INFO_INITIALIZER {\
  .options = { false },\
  .model_name = { 0 },\
  .version_infos = CVCOCTCI_VERSION_INFO_BLOCKS_INITIALIZER,\
}

#endif //V2X_SW_CVCOCTCI_TYPES_H

/**
 * @}
 */
