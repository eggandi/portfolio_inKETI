/** 
 * @file
 * @brief j29451 라이브러리 API 및 내부에서 사용되는 유형들을 정의한 헤더 파일
 * @date 2020-10-03
 * @author gyun
 */


#ifndef V2X_SW_J29451_TYPES_H
#define V2X_SW_J29451_TYPES_H


// 시스템 헤더 파일
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


/**
 * @brief API 수행 결과 코드
 */
enum eJ29451ResultCode
{
  kJ29451Result_Success = 0, ///< 성공
  kJ29451Result_InvalidParameters = 1, ///< 유효하지 않은 파라미터
  kJ29451Result_FailToCreateTimer = 2, ///< 타이머 생성 실패
  kDot2Result_NoCallback = 3, ///< 콜백함수가 등록되어 있지 않음
  kDot2Result_PositionUnavailable = 4, ///< 위치정보가 유효하지 않음
  kJ29451Result_Busy = 5, ///< 이미 동작 중임
  kJ29451Result_NoMemory = 6, ///< 메모리 부족
  kJ29451Result_GPSFail = 7, ///< GPS 관련 실패
  kJ29451Result_FailToCreateThread = 8, ///< 쓰레드 생성 실패
  kJ29451Result_NoPathHistory = 9, ///< Path history 정보가 없음
  kJ29451Result_UserGNSSDataNotAllowed = 10, ///< 사용자 GNSS 데이터가 허용되지 않음.
  kJ29451Result_NotRunning = 11, ///< 동작 중이 아님
  kJ29451Result_EncodeFail = 12, ///< asn.1 인코딩 실패
  kJ29451Result_InsufficientData = 13, ///< 충분하지 않은 데이터(BSM을 전송할 수 없음)
  kJ29451Result_Unspecified, ///< 정의되지 않은 에러
};
typedef int J29451ResultCode; ///< @ref eJ29451ResultCode;


/**
 * @brief 로그메시지 출력 레벨
 */
enum eJ29451LogLevel
{
  kJ29451LogLevel_None = 0, ///< 아무 로그도 출력하지 않는다.
  kJ29451LogLevel_Err = 1, ///< 에러 로그를 출력한다.
  kJ29451LogLevel_Event = 2, ///< 각종 이벤트(패킷 송수신, 설정 등)에 관련된 로그를 출력한다.
  kJ29451LogLevel_All = 3, ///< 모든 로그를 출력한다.
  kJ29451LogLevel_Min = kJ29451LogLevel_None,
  kJ29451LogLevel_Max = kJ29451LogLevel_All,
};
typedef unsigned int J29451LogLevel; ///< @ref eJ29451LogLevel


/**
 * @brief 위도 값 (0.1 마이크로도 단위)
 */
enum eJ29451Latitude
{
  kJ29451Latitude_Min = -900000000,
  kJ29451Latitude_Max = 900000000,
  kJ29451Latitude_Unavailable = 900000001,
};
typedef int J29451Latitude;


/**
 * @brief 경도 값 (0.1 마이크로도 단위)
 */
enum eJ29451Longitude
{
  kJ29451Longitude_Min = -1799999999,
  kJ29451Longitude_Max = 1800000000,
  kJ29451Longitude_Unavailable = 1800000001,
};
typedef int J29451Longitude;


/**
 * @brief 고도 값 (0.1미터 단위)
 */
enum eJ29451Elevation
{
  kJ29451Elevation_Min = -4095, ///< -409.5 미터 이하
  kJ29451Elevation_Max = 61439, ///< 6143.9 미터 이상
  kJ29451Elevation_Unavailable = -4096,
};
typedef int J29451Elevation; ///< @ref eJ29451Elevation


/**
 * @brief 속도 값 (0.02m/s 단위)
 */
enum eJ29451Speed
{
  kJ29451Speed_Min = 0,
  kJ29451Speed_Max = 8190,
  kJ29451Speed_Unavailable = 8191,
};
typedef unsigned int J29451Speed; ///< @ref eJ29451Speed


/**
 * @brief 헤딩 값 (북쪽으로부터 시계방향으로 0.0125 도 단위)
 */
enum eJ29451Heading
{
  kJ29451Heading_Min = 0, ///< 0도(=360도)
  kJ29451Heading_Max = 28799, ///< 359.9875도
  kJ29451Heading_Unavailable = 28800,
};
typedef unsigned int J29451Heading; ///< @ref eJ29451Heading


/**
 * @brief semi-major axis accuracy at one standard dev.
 *        (0.05미터 단위)
 */
enum eJ29451SemiMajorAxisAccuracy
{
  kJ29451SemiMajorAxisAccuracy_Min = 0, ///< 0미터
  kJ29451SemiMajorAxisAccuracy_Max = 254, ///< 12.7 미터 이상
  kJ29451SemiMajorAxisAccuracy_Unavailable = 255,
};
typedef unsigned int J29451SemiMajorAxisAccuracy; ///< @ref eJ29451SemiMajorAxisAccuracy


/**
 * @brief semi-minor axis accuracy at one standard dev.
 *        (0.05미터 단위)
 */
enum eJ29451SemiMinorAxisAccuracy
{
  kJ29451SemiMinorAxisAccuracy_Min = 0, ///< 0미터
  kJ29451SemiMinorAxisAccuracy_Max = 254, ///< 12.7 미터 이상
  kJ29451SemiMinorAxisAccuracy_Unavailable = 255,
};
typedef unsigned int J29451SemiMinorAxisAccuracy; ///< @ref eJ29451SemiMinorAxisAccuracy


/**
 * @brief orientation of semi-major axis. relative to true north (0~359.9945078786 degrees)
 *        (360/65535 = 0.0054932479 도 단위)
 */
enum eJ29451SemiMajorAxisOrientation
{
  kJ29451SemiMajorAxisOrientation_Min = 0, ///< 0도
  kJ29451SemiMajorAxisOrientation_Max = 65534, ///< 359.9945078786 도
  kJ29451SemiMajorAxisOrientation_Unavailable = 65535,
};
typedef unsigned int J29451SemiMajorAxisOrientation; ///< @ref eJ29451SemiMajorAxisOrientation


/**
 * @brief 기어 상태
 */
enum eJ29451TransmissionState
{
  kJ29451TransmissionState_Neutral = 0,
  kJ29451TransmissionState_Park = 1,
  kJ29451TransmissionState_ForwardGears = 2,
  kJ29451TransmissionState_ReverseGears = 3,
  kJ29451TransmissionState_Unavailable = 7,
  kJ29451TransmissionState_Min = kJ29451TransmissionState_Neutral,
  kJ29451TransmissionState_Max = kJ29451TransmissionState_ReverseGears,
};
typedef unsigned int J29451TransmissionState; ///< @ref eJ29451TransmissionState


/**
 * @brief 스티어링 휠 각도 (1.5도 단위). 우측: 양수, 좌측: 음수
 */
enum eJ29451SteeringWheelAngle
{
  kJ29451SteeringWheelAngle_Min = -126, ///< -189도 이하
  kJ29451SteeringWheelAngle_Max = 126, ///< 189도 이상
  kJ29451SteeringWheelAngle_Unavailable = 127,
};
typedef int J29451SteeringWheelAngle; ///< @ref eJ29451SteeringWheelAngle


/**
 * @brief 가속도 (0.01m/s^2 단위)
 */
enum eJ29451Acceleration
{
  kJ29451Acceleration_Min = -2000, ///< -20 m/s^2 이하
  kJ29451Acceleration_Max = 2000, ///< 20 m/s^2 이상
  kJ29451Acceleration_Unavailable = 2001,
};
typedef int J29451Acceleration; ///< @ref eJ29451Acceleration


/**
 * @brief 수직 가속도 (0.02G 단위 = 0.1962 m/s^2)
 */
enum eJ29451VerticalAcceleration
{
  kJ29451VerticalAcceleration_Min = -126, ///< -2.52G 이하
  kJ29451VerticalAcceleration_Max = 127, ///< 2.54G 이상
  kJ29451VerticalAcceleration_Unavailable = -127,
};
typedef int J29451VerticalAcceleration; ///< @ref eJ29451VerticalAcceleration


/**
 * @brief yaw rate (0.01 도/s 단위). 우측: 양수, 좌측: 음수
 */
enum eJ29451YawRate
{
  kJ29451YawRate_Min = -32767,
  kJ29451YawRate_Straight = 0,
  kJ29451YawRate_Max = 32767,
  kJ29451YawRate_Unavailable = -32768, ///< 규격에 Unavailable 값이 지정되어 있지 않아 임의로 지정.
                                       ///< BSM에는 수납되지 않고 내부적으로만 사용된다.
};
typedef int J29451YawRate; ///< @ref eJ29451YawRate


/**
 * @brief TCS 상태
 */
enum eJ29451TractionControlStatus
{
  kJ29451TractionControlStatus_Unavailable = 0,	/* not equipped or unavailable */
  kJ29451TractionControlStatus_Off,
  kJ29451TractionControlStatus_On,	/* but not engaged */
  kJ29451TractionControlStatus_Engaged,
  kJ29451TractionControlStatus_Min = kJ29451TractionControlStatus_Unavailable,
  kJ29451TractionControlStatus_Max = kJ29451TractionControlStatus_Engaged,
};
typedef unsigned int J29451TractionControlStatus; ///< @ref eJ29451TractionControlStatus;


/**
 * @brief ABS 상태
 */
enum eJ29451AntiLockBrakeStatus
{
  kJ29451AntiLockBrakeStatus_Unavailable = 0,	/* not equipped or unavailable */
  kJ29451AntiLockBrakeStatus_Off,
  kJ29451AntiLockBrakeStatus_On,	/* but not engaged */
  kJ29451AntiLockBrakeStatus_Engaged,
  kJ29451AntiLockBrakeStatus_Min = kJ29451AntiLockBrakeStatus_Unavailable,
  kJ29451AntiLockBrakeStatus_Max = kJ29451AntiLockBrakeStatus_Engaged,
};
typedef unsigned int J29451AntiLockBrakeStatus; ///< @ref eJ29451AntiLockBrakeStatus


/**
 * @brief SCS 상태
 */
enum eJ29451StabilityControlStatus
{
  kJ29451StabilityControlStatus_Unavailable = 0,	/* not equipped or unavailable */
  kJ29451StabilityControlStatus_Off,
  kJ29451StabilityControlStatus_On,	/* but not engaged */
  kJ29451StabilityControlStatus_Engaged,
  kJ29451StabilityControlStatus_Min = kJ29451StabilityControlStatus_Unavailable,
  kJ29451StabilityControlStatus_Max = kJ29451StabilityControlStatus_Engaged,
};
typedef unsigned int J29451StabilityControlStatus; ///< @ref eJ29451StabilityControlStatus


/**
 * @brief BrakeBoost 적용 상태
 */
enum eJ29451BrakeBoostApplied
{
  kJ29451BrakeBoostApplied_Unavailable = 0,	/* not equipped or unavailable */
  kJ29451BrakeBoostApplied_Off,
  kJ29451BrakeBoostApplied_On,
  kJ29451BrakeBoostApplied_Min = kJ29451BrakeBoostApplied_Unavailable,
  kJ29451BrakeBoostApplied_Max = kJ29451BrakeBoostApplied_On,
};
typedef unsigned int J29451BrakeBoostApplied; ///< @ref eJ29451BrakeBoostApplied


/**
 * @brief 보조 브레이크 상태
 */
enum eJ29451AuxiliaryBrakeStatus
{
  kJ29451AuxiliaryBrakeStatus_Unavailable = 0,	/* not equipped or unavailable */
  kJ29451AuxiliaryBrakeStatus_Off,
  kJ29451AuxiliaryBrakeStatus_On,	/* engaged */
  kJ29451AuxiliaryBrakeStatus_Reserved,
  kJ29451AuxiliaryBrakeStatus_Min = kJ29451AuxiliaryBrakeStatus_Unavailable,
  kJ29451AuxiliaryBrakeStatus_Max = kJ29451AuxiliaryBrakeStatus_On,
};
typedef unsigned int J29451AuxiliaryBrakeStatus; ///< @ref eJ29451AuxiliaryBrakeStatus


/**
 * @brief 차량 넓이 (1cm 단위)
 */
enum eJ29451VehicleWidth
{
  kJ29451VehicleWidth_Min = 1,
  kJ29451VehicleWidth_Max = 1023,
  kJ29451VehicleWidth_Unavailable = 0,
};
typedef unsigned int J29451VehicleWidth; ///< @ref eJ29451VehicleWidth


/**
 * @brief 차량 길이 (1cm 단위)
 */
enum eJ29451VehicleLength
{
  kJ29451VehicleLength_Min = 1,
  kJ29451VehicleLength_Max = 4095,
  kJ29451VehicleLength_Unavailable = 0,
} ;
typedef unsigned int J29451VehicleLength; ///< @ref eJ29451VehicleLength


/**
 * @brief 커브 반지름 (10cm, 0.1m 단위)
 * 양수: 시계방향 회전
 * 음수: 시계반대방향 회전
 */
enum eJ29451RadiusOfCurvature
{
  kJ29451RadiusOfCurvature_Min = -32767,
  kJ29451RadiusOfCurvature_Max = 32765,
  kJ29451RadiusOfCurvature_Straight = 32767,
};
typedef int J29451RadiusOfCurvature; ///< @ref eJ29451RadiusOfCurvature


/**
 * @brief 정보 신뢰도 (0.5% 단위)
 */
enum eJ29451Confidence
{
  kJ29451Confidence_Min = 0,
  kJ29451Confidence_Max = 200,
  kJ29451Confidence_Default = 0,
};
typedef int J29451Confidence; ///< @ref eJ29451Confidence


/**
 * @brief BSM 송신 주기 (밀리초 단위)
 */
enum eJ29451BSMTxInterval
{
  kJ29451BSMTxInterval_Min = 100,
  kJ29451BSMTxInterval_Default = 100, ///< J2945/1 규격에 정의된 기본값
  kJ29451BSMTxInterval_Max = 1000
};
typedef unsigned int J29451BSMTxInterval; ///< @ref eJ29451BSMTxInterval


/**
 * @brief 외부등 상태
 *
 * 차량정보를 이용하는 어플리케이션에 의해 설정된다.
 */
struct J29451ExteriorLights
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
 * @brief 브레이크 적용 상태
 *
 * 차량정보를 이용하는 어플리케이션에 의해 설정된다.
 */
struct J29451BrakeAppliedStatus
{
  bool unavailable;
  bool left_front;
  bool left_rear;
  bool right_front;
  bool right_rear;
};


/**
 * @brief 이벤트 발생 정보
 *
 * 차량정보를 이용하는 어플리케이션에 의해 설정된다.
 */
struct J29451VehicleEventFlags
{
  bool hazard_lights; ///< 비상 깜빡이
  bool stop_line_violation; ///< 교차로 정지선 위반
  bool abs_activated; ///< 100msec 이상 ABS 활성화
  bool traction_control_loss; ///< 100msec 이상 TCS 활성화
  bool stability_control_activated; ///< 100msec 이상 SCS 활성화
  bool hazardous_materials; ///< 위험물질 탑재
  bool reserved1; ///< 사용되지 않음
  bool hard_braking; ///< 급브레이크
  bool lights_changed; ///< 최근 2초 내에 외부등 상태가 변경됨
  bool wiper_changed; ///< 최근 2초 내에 와이퍼 상태가 변경됨
  bool flat_tire; ///< 타이어 바람 빠짐
  bool disabled_vehicle; ///< 고장
  bool airbag_deployment; ///< 에어백 전개
};


/**
 * @brief 어플리케이션에게 BSM 송신을 요청하는 콜백함수
 * @param[in] bsm BSM 메시지 UPER 인코딩 바이트열
 * @param[in] bsm_size BSM 메시지의 길이
 * @param[in] event 이벤트 발생 여부
 * @param[in] cert_sign 인증서로 서명해야 하는지 여부
 * @param[in] id_change ID/인증서 변경 필요 여부
 * @param[in] addr 랜덤하게 생성된 MAC 주소. id_change=true일 경우 본 MAC 주소를 장치에 설정해야 한다.
 */
typedef void (*ProcessBSMTransmitCallback)(
  const uint8_t *bsm,
  size_t bsm_size,
  bool event,
  bool cert_sign,
  bool id_change,
  uint8_t *addr);



#endif //V2X_SW_J29451_TYPES_H
