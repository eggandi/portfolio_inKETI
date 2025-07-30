/** 
 * @file
 * @brief j29451 라이브러리 API를 정의한 헤더 파일
 * @date 2020-10-03
 * @author gyun
 */


#ifndef V2X_SW_J29451_API_H
#define V2X_SW_J29451_API_H

#ifdef __cplusplus
extern "C" {
#endif


// 라이브러리 의존 헤더 파일
#if defined(_FFASN1C_)
#include "ffasn1-j2735-2016.h"
#endif

// 라이브러리 헤더 파일
#include "j29451-api-params.h"
#include "v2x-sw.h"


/**
 * @brief j29451 라이브러리를 초기화한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] log_level 로그메시지 출력 레벨
 * @param[out] addr 랜덤하게 생성된 MAC주소가 저장될 버퍼
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_Init(J29451LogLevel log_level, uint8_t *addr);


/**
 * @brief j29451 라이브러리를 종료한다(상세 내용은 API 매뉴얼 참조).
 */
void J29451_Release(void);


/**
 * @brief BSM 송신 요청을 전달받을 콜백함수를 등록한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] callback 콜백함수 포인터
 */
void J29451_RegisterBSMTransmitCallback(ProcessBSMTransmitCallback callback);


/**
 * @brief 주기적인 BSM 송신의 시작을 요청한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] tx_interval 송신주기
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_StartBSMTransmit(J29451BSMTxInterval tx_interval);


/**
 * @brief BSM 송신의 중지를 요청한다(상세 내용은 API 매뉴얼 참조).
 */
void J29451_StopBSMTransmit(void);


/**
 * @brief ID 변경을 수행하도록 요청한다(상세 내용은 API 매뉴얼 참조).
 */
void J29451_RequestBSMIDChange(void);


/**
 * @brief 가장 최근에 전송된 BSM에 수납된 Path 정보를 백업한다.
 * @param[in] file_path 정보가 저장될 파일 경로
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SavePathInfoBackupFile(const char *file_path);


/**
 * @brief 백업파일로부터 Path 정보를 로딩한다.
 * @param[in] file_path 정보가 저장된 파일 경로
 */
void J29451_LoadPathInfoBackupFile(const char *file_path);


/**
 * @brief Hard braking 이벤트 발생여부 판정 기능을 활성화/비활성화한다.
 * @param[in] activate 활성화 시 true, 비활성화 시 false
 */
void J29451_ActivateHardBrakingEventDecision(bool activate);


/**
 * @brief Path 정보(Path history, Path prediction)를 초기화한다.
 *
 * 본 API는 인증(표준적합성 시험)에서만 사용된다.
 *  - TS의 명령(SetGpsTime)에 의해 시간이 변경되면, Path 정보 생성 메커니즘이 오동작하므로 Path 정보를 초기화해 줄 필요가 있다.
 *  - 해당 명령이 발생할 경우는, DUT의 동작이 시간 상 연속성을 가지지 않는 경우이므로,
 *    과거 정보와 연속성을 갖는 Path 정보는 초기화하여 새롭게 생성되도록 한다.
 */
void J29451_InitPathInfo(void);


/**
 * @brief 인증 모드로 설정한다.
 *
 * 인증을 위한 표준적합성시험(TCI 기반) 진행 시 호출한다.
 */
void J29451_SetCertificationMode(void);


/**
 * @brief 차량 외부등 상태 정보를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] lights 설정할 외부등 상태 정보
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetVehicleExteriorLights(const struct J29451ExteriorLights *lights);


/**
 * @brief 저장되어 있는 차량 외부등 상태 정보를 해제한다(상세 내용은 API 매뉴얼 참조).
 */
void J29451_ClearVehicleExteriorLights(void);


/**
 * @brief 차량에 발생한 이벤트를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] event 설정할 이벤트 정보
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetVehicleEventFlags(const struct J29451VehicleEventFlags *event);


/**
 * @brief 저장되어 있는 차량 이벤트 발생 정보를 해제한다(상세 내용은 API 매뉴얼 참조).
 */
void J29451_ClearVehicleEventFlags(void);


/**
 * @brief 차량의 기어 상태를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] transmission 설정할 기어 상태
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetVehicleTransmissionState(J29451TransmissionState transmission);


/**
 * @brief 차량의 속도를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] speed 설정할 속도
 */
void J29451_SetVehicleSpeed(J29451Speed speed);


/**
 * @brief 차량의 스티어링 휠 각도를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] angle 설정할 각도
 */
void J29451_SetVehicleSteeringWheelAngle(J29451SteeringWheelAngle angle);


/**
 * @brief 차량의 브레이크 적용 상태를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] brakes 설정할 브레이크 적용 상태 정보
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetVehicleBrakeAppliedStatus(const struct J29451BrakeAppliedStatus *brakes);


/**
 * @brief 저장된 차량 브레이크 적용 상태를 해제한다(상세 내용은 API 매뉴얼 참조).
 */
void J29451_ClearVehicleBrakeAppliedStatus(void);


/**
 * @brief 차량의 traction control 상태를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] status 설정할 traction control 상태 정보
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetVehicleTractionControlStatus(J29451TractionControlStatus status);


/**
 * @brief 차량의 anti lock brake 상태를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] status 설정할 anti lock brake 상태 정보
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetVehicleAntiLockBrakeStatus(J29451AntiLockBrakeStatus status);


/**
 * @brief 차량의 stability control 상태를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] status 설정할 stability control 상태 정보
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetVehicleStabilityControlStatus(J29451StabilityControlStatus status);


/**
 * @brief 차량의 brake boost applied 상태를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] brake_boost 설정할 brake boost applied 상태 정보
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetVehicleBrakeBoostApplied(J29451BrakeBoostApplied brake_boost);


/**
 * @brief 차량의 보조 브레이크 상태를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] status 설정할 보조 브레이크 상태 정보
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetVehicleAuxiliaryBrakeStatus(J29451AuxiliaryBrakeStatus status);


/**
 * @brief 차량의 크기를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] width 차량의 넓이
 * @param[in] length 차량의 길이
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetVehicleSize(J29451VehicleWidth width, J29451VehicleLength length);


/**
 * @brief 사용자 GNSS 데이터의 사용을 활성화한다(상세 내용은 API 매뉴얼 참조).
 */
void J29451_EnableUserGNSSData(void);


/**
 * @brief 사용자 GNSS 데이터의 사용을 비활성화한다(상세 내용은 API 매뉴얼 참조).
 */
void J29451_DisableUserGNSSData(void);


/**
 * @brief 위도를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] lat 위도
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetUserGNSSLatitude(J29451Latitude lat);


/**
 * @brief 경도를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] lon 경도
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetUserGNSSLongitude(J29451Longitude lon);


/**
 * @brief 고도를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] elev 고도
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetUserGNSSElevation(J29451Elevation elev);


/**
 * @brief 속도를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] speed 속도
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetUserGNSSSpeed(J29451Speed speed);


/**
 * @brief 헤딩을 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] heading 헤딩
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetUserGNSSHeading(J29451Heading heading);


/**
 * @brief 위치정확도 정보를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] semi_major Semi major axis accuracy
 * @param[in] semi_minor Semi minor axis accuracy
 * @param[in] orientation Semi major axis orientation
 * @retval 0: 성공
 * @retval 음수(-J29451ResultCode): 실패
 */
int J29451_SetUserGNSSPositionalAccuracy(
  J29451SemiMajorAxisAccuracy semi_major,
  J29451SemiMinorAxisAccuracy semi_minor,
  J29451SemiMajorAxisOrientation orientation);


/**
 * @brief 차량의 4방향 가속정보를 설정한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] lon 종방향 가속도
 * @param[in] lat 횡방향 가속도
 * @param[in] vert 수직 가속도
 * @param[in] yaw yaw rate
 */
void J29451_SetUserGNSSAccelerationSet4Way(
  J29451Acceleration lon,
  J29451Acceleration lat,
  J29451VerticalAcceleration vert,
  J29451YawRate yaw);


#if defined(_FFASN1C_)
/**
 * @brief 메시지 프레임을 디코딩한다(상세 내용은 API 매뉴얼 참조).
 * @param[in] msg 디코딩할 메시지 프레임
 * @param[in] msg_size 메시지 프레임의 길이
 * @return 메시지 프레임 디코딩 정보 포인터
 * @retval NULL: 디코딩 실패
 */
j2735MessageFrame * J29451_DecodeMessageFrame(const uint8_t *msg, size_t msg_size);


/**
 * @brief 디코딩된 메시지 프레임 정보를 해제한다.
 * @param[in] msg 디코딩된 메시지 프레임 정보
 */
void J29451_FreeDecodedMessageFrame(j2735MessageFrame *msg);

#endif


#ifdef __cplusplus
}
#endif


#endif //V2X_SW_J29451_API_H
