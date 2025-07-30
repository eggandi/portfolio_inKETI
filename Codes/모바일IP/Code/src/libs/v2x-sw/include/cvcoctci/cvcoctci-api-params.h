/**
 * @addtogroup cvcoctci_lib cvcoctci library API
 * @{
 *
 * @file
 * @brief cvcoctci 라이브러리의 Open API 파라미터를 정의한 헤더파일
 * @date 2019-09-14
 * @author gyun
 */

#ifndef V2X_SW_CVCOCTCI_API_PARAMS_H
#define V2X_SW_CVCOCTCI_API_PARAMS_H

// 라이브러리 헤더 파일
#include "cvcoctci-types.h"


/**
 * @brief Cvcoctci_ParseTciMessage() API 파라미터
 */
struct CvcoctciParseParams
{
  CvcoctciVersion version; ///< TCI 버전
  CvcoctciTime64 time; ///< 시각
  CvcoctciTciFrameType frame_type; ///< 프레임 유형
  CvcoctciRequestType req_type; ///< request 메시지 유형
  uint8_t msg_id; ///< msgID
  union {
    bool set_initial_state;
    bool enable_gps_input;
    bool enable_congestion_mitigation;
    bool shutdown;
    bool restart;
    bool request_sut_availability;
    bool request_sut_info;
    CvcoctciLatitude set_latitude;
    CvcoctciLongitude set_longitude;
    CvcoctciElevation set_elevation;
    CvcoctciSetSpeed set_speed;
    CvcoctciSetHeading set_heading;
    CvcoctciTime64 set_gps_time;
    CvcoctciSetTransmissionState set_transmission_state;
    CvcoctciSetSteeringWheelAngle set_steering_wheel_angle;
    uint8_t set_temporary_id[CVCOCTCI_TEMPORARY_ID_LEN];
    struct CvcoctciSetWsmTxInfo set_wsm_tx_info;
    struct CvcoctciStartWsmTx start_wsm_tx;
    struct CvcoctciStopWsmTx stop_wsm_tx;
    struct CvcoctciStartWsaTxPeriodic start_wsa_tx_periodic;
    struct CvcoctciStopWsaTxPeriodic stop_wsa_tx_periodic;
    struct CvcoctciStartWsmRx start_wsm_rx;
    struct CvcoctciStopWsmRx stop_wsm_rx;
    struct CvcoctciAddWsaProviderService add_psr;
    struct CvcoctciChangeWsaProviderService change_psr;
    struct CvcoctciDelWsaProviderService del_psr;
    struct CvcoctciAddUserService add_usr;
    struct CvcoctciDelUserService del_usr;
    struct CvcoctciGetIPv6InterfaceInfo get_ipv6_info;
    struct CvcoctciSetIPv6Address set_ipv6_addr;
    struct CvcoctciStartIPv6Ping start_ipv6_ping;
    struct CvcoctciStopIPv6Ping stop_ipv6_ping;
    struct CvcoctciStartIPv6Tx start_ipv6_tx;
    struct CvcoctciStopIPv6Tx stop_ipv6_tx;
    struct CvcoctciStartIPv6Rx start_ipv6_rx;
    struct CvcoctciStopIPv6Rx stop_ipv6_rx;
    struct CvcoctciAddTxProfile add_txprofile;
    struct CvcoctciDelTxProfile del_txprofile;
    struct CvcoctciStartBsmTx start_bsm_tx;
    struct CvcoctciStopBsmTx stop_bsm_tx;
    struct CvcoctciStartBsmRx start_bsm_rx;
    struct CvcoctciStopBsmRx stop_bsm_rx;
    struct CvcoctciSetPositionalAccuracy set_pos_accuracy;
    struct CvcoctciSetAccelerationSet4Way set_accel_set;
    struct CvcoctciSetVehicleEventFlags set_vehicle_event_flags;
    struct CvcoctciSetVehicleSize set_vehicle_size;
    struct CvcoctciSetExteriorLights set_exterior_lights;
    struct CvcoctciSetBrakeSystemStatus set_brake_system_status;
    struct CvcoctciSetTestId set_test_id;
  } u; ///< req_type 별 메시지
};


/**
 * @brief Cvcoctci_Construct**Response() API 파라미터
 */
struct CvcoctciResponseConstructParams
{
  struct {
    bool exception; ///< exception 옵션필드 수납 여부
  } options;
  uint8_t msg_id; ///< 대응되는 Request 메시지의 msg_id
  CvcoctciTciResultCode result_code; ///< Request 처리 결과 코드
  struct CvcoctciException exception; ///< 예외 발생 정보
};
/// struct @ref CvcoctciResponseConstructParams 구조체 초기화 매크로
#define CVCOCTCI_RESPONSE_CONSTRUCT_PARAMS_INITIALIZER {\
  .options = { false },\
  .msg_id = 0,\
  .result_code = kCvcoctciTciResultCode_Success,\
  .exception = CVCOCTCI_EXCEPTION_INITIALIZER,\
}


/**
 * @brief Cvcoctci_Construct**Indication() API 파라미터
 */
struct CvcoctciIndicationConstructParams
{
  struct {
    bool event_params; ///< eventParams 옵션 필드 수납 여부
    bool pdu; ///< pdu 옵션 필드 수납 여부
    bool exception; ///< exception 옵션 필드 수납 여부
  } options;

  CvcoctciRadio radio; ///< 수신된 라디오
  CvcoctciEvent event; ///< 수신 이벤트 유형
  CvcoctciEventParamsType event_params_type; ///< 이벤트 파라미터 유형
  struct CvcoctciPdu pdu; ///< 수신된 PDU
  struct CvcoctciException exception; ///< 예외 발생 정보

  union {
    struct CvcoctciServiceParameters service; ///< 서비스 발견 이벤트
    struct CvcoctciWsmParameters wsm; ///< WSM 수신 이벤트
    struct CvcoctciIpParameters ip; ///< IP 수신 이벤트
    struct CvcoctciD80211Parameters d80211frame; ///< 802.11 프레임 수신 이벤트
    struct CvcoctciSecResultParams security; ///< 보안 처리 이벤트
  } event_params; ///< 이벤트 파라미터
};
/// struct @ref CvcoctciIndicationConstructParams 구조체 초기화 매크로
#define CVCOCTCI_INDICATION_CONSTRUCT_PARAMS_INITIALIZER {\
  .options = {false, false, false},\
  .radio = kCvcoctciRadio_NA,\
  .event = kCvcoctciEvent_NA,\
  .event_params_type = kCvcoctciEventParamsType_NA,\
  .pdu = CVCOCTCI_PDU_INITIALIZER,\
  .exception = CVCOCTCI_EXCEPTION_INITIALIZER,\
}


/**
 * @brief Cvcoctci_Construct**ResponseInfo() API 파라미터
 */
struct CvcoctciResponseInfoConstructParams
{
  struct {
    bool info; ///< info 옵션 필드 수납 여부
    bool exception; ///< exception 옵션 필드 수납 여부
  } options;

  uint8_t msg_id; ///< 대응되는 Request 메시지의 msg_id
  CvcoctciTciResultCode result_code; ///< Request 처리 결과 코드
  CvcoctciInfoContentType info_type; ///< 반환정보 유형
  union {
    struct CvcoctciIPv6InterfaceInfos infos; ///< IPv6 인터페이스 정보(들)
    struct CvcoctciSutInfo sut_info; ///< SUT 정보
  } info; ///< 반환 정보
  struct CvcoctciException exception; ///< 예외 발생 정보
};


/**
 * @brief Cvcoctci_Construct**Exception() API 파라미터
 */
#define CvcoctciExceptionConstructParams CvcoctciException

#endif //V2X_SW_CVCOCTCI_API_PARAMS_H

/**
 * @}
 */
