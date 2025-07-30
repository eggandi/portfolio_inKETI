/**
 * @file
 * @brief cvcoctci 라이브러리의 Open API 파라미터를 정의한 헤더파일
 * @date 2019-09-14
 * @author gyun
 */

#ifndef V2X_SW_CVCOCTCI3_API_PARAMS_H
#define V2X_SW_CVCOCTCI3_API_PARAMS_H

// 라이브러리 헤더 파일
#include "cvcoctci3-types.h"


/**
 * Update TCIv3 by young
 * Add request_wsm_tx_count, request_wsm_tx_count_reset, request_wsm_rx_count, request_wsm_rx_count_reset
 * Add PC5 - send_ue_config_xml, set_ue_config, set_flow_configs and send_at_command
 * 
 * @brief Cvcoctci3_ParseTciMessage() API 파라미터
 */
struct Cvcoctci3ParseParams
{
  Cvcoctci3Version version; ///< TCI 버전
  Cvcoctci3Time64 time; ///< 시각
  Cvcoctci3TciFrameType frame_type; ///< 프레임 유형
  Cvcoctci3RequestType req_type; ///< request 메시지 유형
  uint8_t msg_id; ///< msgID
  union {
    bool set_initial_state;
    bool enable_gps_input;
    bool enable_congestion_mitigation;
    bool shutdown;
    bool restart;
    bool request_sut_availability;
    bool request_sut_info;
    bool request_sut_status;
    bool request_wsm_tx_count;
    bool request_wsm_rx_count;
    bool request_wsm_tx_count_reset;
    bool request_wsm_rx_count_reset;
    Cvcoctci3Latitude set_latitude;
    Cvcoctci3Longitude set_longitude;
    Cvcoctci3Elevation set_elevation;
    Cvcoctci3SetSpeed set_speed;
    Cvcoctci3SetHeading set_heading;
    Cvcoctci3Time64 set_gps_time;
    Cvcoctci3SetTransmissionState set_transmission_state;
    Cvcoctci3SetSteeringWheelAngle set_steering_wheel_angle;
    uint8_t set_temporary_id[CVCOCTCI3_TEMPORARY_ID_LEN];
    struct Cvcoctci3SetWsmTxInfo set_wsm_tx_info;
    struct Cvcoctci3Pc5StartWsmTx pc5_start_wsm_tx;
    struct Cvcoctci3StartWsmTx start_wsm_tx;
    struct Cvcoctci3StopWsmTx stop_wsm_tx;
    struct Cvcoctci3StartWsaTxPeriodic start_wsa_tx_periodic;
    struct Cvcoctci3StopWsaTxPeriodic stop_wsa_tx_periodic;
    struct Cvcoctci3StartWsmRx start_wsm_rx;
    struct Cvcoctci3StopWsmRx stop_wsm_rx;
    struct Cvcoctci3AddWsaProviderService add_psr;
    struct Cvcoctci3ChangeWsaProviderService change_psr;
    struct Cvcoctci3DelWsaProviderService del_psr;
    struct Cvcoctci3AddUserService add_usr;
    struct Cvcoctci3DelUserService del_usr;
    struct Cvcoctci3GetIPv6InterfaceInfo get_ipv6_info;
    struct Cvcoctci3SetIPv6Address set_ipv6_addr;
    struct Cvcoctci3StartIPv6Ping start_ipv6_ping;
    struct Cvcoctci3StopIPv6Ping stop_ipv6_ping;
    struct Cvcoctci3StartIPv6Tx start_ipv6_tx;
    struct Cvcoctci3StopIPv6Tx stop_ipv6_tx;
    struct Cvcoctci3StartIPv6Rx start_ipv6_rx;
    struct Cvcoctci3StopIPv6Rx stop_ipv6_rx;
    struct Cvcoctci3AddTxProfile add_txprofile;
    struct Cvcoctci3DelTxProfile del_txprofile;
    struct Cvcoctci3StartBsmTx start_bsm_tx;
    struct Cvcoctci3StopBsmTx stop_bsm_tx;
    struct Cvcoctci3StartBsmRx start_bsm_rx;
    struct Cvcoctci3StopBsmRx stop_bsm_rx;
    struct Cvcoctci3SetPositionalAccuracy set_pos_accuracy;
    struct Cvcoctci3SetAccelerationSet4Way set_accel_set;
    struct Cvcoctci3SetVehicleEventFlags set_vehicle_event_flags;
    struct Cvcoctci3SetVehicleSize set_vehicle_size;
    struct Cvcoctci3SetExteriorLights set_exterior_lights;
    struct Cvcoctci3SetBrakeSystemStatus set_brake_system_status;
    struct Cvcoctci3SetTestId set_test_id;
    struct Cvcoctci3SendUeConfigXML send_ue_config_xml;
    struct Cvcoctci3SetUeConfig set_ue_config;
    struct Cvcoctci3SetFlowConfigs set_flow_configs;
    struct Cvcoctci3SendATCommand send_at_command;
    struct Cvcoctci3StartUdpProxyTx start_udp_proxy_tx;
    struct Cvcoctci3StartUdpProxyRx start_udp_proxy_rx;
    struct Cvcoctci3StopUdpProxy stop_udp_proxy;
  } u; ///< req_type 별 메시지
};


/**
 * @brief Cvcoctci3_Construct**Response() API 파라미터
 */
struct Cvcoctci3ResponseConstructParams
{
  struct {
    bool exception; ///< exception 옵션필드 수납 여부
  } options;
  uint8_t msg_id; ///< 대응되는 Request 메시지의 msg_id
  Cvcoctci3TciResultCode result_code; ///< Request 처리 결과 코드
  struct Cvcoctci3Exception exception; ///< 예외 발생 정보
};
/// struct @ref Cvcoctci3ResponseConstructParams 구조체 초기화 매크로
#define CVCOCTCI3_RESPONSE_CONSTRUCT_PARAMS_INITIALIZER {\
  .options = { false },\
  .msg_id = 0,\
  .result_code = kCvcoctci3TciResultCode_Success,\
  .exception = CVCOCTCI3_EXCEPTION_INITIALIZER,\
}


/**
 * @brief Cvcoctci3_Construct**Indication() API 파라미터
 * @date 2022-01-12
 * @details Update TCIv3 by young
 * @details d80211frame rename to radioframe
 */
struct Cvcoctci3IndicationConstructParams
{
  struct {
    bool event_params; ///< eventParams 옵션 필드 수납 여부
    bool pdu; ///< pdu 옵션 필드 수납 여부
    bool exception; ///< exception 옵션 필드 수납 여부
  } options;

  Cvcoctci3Radio radio; ///< 수신된 라디오
  Cvcoctci3Event event; ///< 수신 이벤트 유형
  Cvcoctci3EventParamsType event_params_type; ///< 이벤트 파라미터 유형
  struct Cvcoctci3Pdu pdu; ///< 수신된 PDU
  struct Cvcoctci3Exception exception; ///< 예외 발생 정보

  union {
    struct Cvcoctci3ServiceParameters service; ///< 서비스 발견 이벤트
    struct Cvcoctci3WsmParameters wsm; ///< WSM 수신 이벤트
    struct Cvcoctci3IpParameters ip; ///< IP 수신 이벤트
    // d80211frame rename to radioframe - Update TCIv3 by young
    struct Cvcoctci3RadioParameters radioframe; ///< 802.11 프레임 수신 이벤트
    struct Cvcoctci3SecResultParams security; ///< 보안 처리 이벤트
  } event_params; ///< 이벤트 파라미터
};
/// struct @ref Cvcoctci3IndicationConstructParams 구조체 초기화 매크로
#define CVCOCTCI3_INDICATION_CONSTRUCT_PARAMS_INITIALIZER {\
  .options = {false, false, false},\
  .radio = kCvcoctci3Radio_NA,\
  .event = kCvcoctci3Event_NA,\
  .event_params_type = kCvcoctci3EventParamsType_NA,\
  .pdu = CVCOCTCI3_PDU_INITIALIZER,\
  .exception = CVCOCTCI3_EXCEPTION_INITIALIZER,\
}


/**
 * @brief Cvcoctci3_Construct**ResponseInfo() API 파라미터
 */
struct Cvcoctci3ResponseInfoConstructParams
{
  struct {
    bool info; ///< info 옵션 필드 수납 여부
    bool exception; ///< exception 옵션 필드 수납 여부
  } options;

  uint8_t msg_id; ///< 대응되는 Request 메시지의 msg_id
  Cvcoctci3TciResultCode result_code; ///< Request 처리 결과 코드
  Cvcoctci3InfoContentType info_type; ///< 반환정보 유형
  union {
    struct Cvcoctci3IPv6InterfaceInfos infos; ///< IPv6 인터페이스 정보(들)
    struct Cvcoctci3SutInfo sut_info; ///< SUT 정보
    struct Cvcoctci3ATCmdInfo at_cmd_info; ///< AT command 정보
    Cvcoctci3PktCount pkt_count; ///< 패킷 수
    struct Cvcoctci3SutStatus sut_status; ///< SUT 상태
  } info; ///< 반환 정보
  struct Cvcoctci3Exception exception; ///< 예외 발생 정보
};


/**
 * @brief Cvcoctci3_Construct**Exception() API 파라미터
 */
#define Cvcoctci3ExceptionConstructParams Cvcoctci3Exception

#endif //V2X_SW_CVCOCTCI3_API_PARAMS_H
