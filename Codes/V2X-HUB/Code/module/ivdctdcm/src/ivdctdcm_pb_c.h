
#include <stdio.h>
#include <stdint.h>

#ifndef __IVDCTDCM_PB_C_H__
#define __IVDCTDCM_PB_C_H__


	/* 타임스탬프 타입 정의 */
	typedef uint64_t collection_timestamp;
	/* 엔진 정보 */
	typedef enum {	// 장치 동작상태
		SYSTEM_STATUS_UNKNOWN  = 0,	// 알수없음
		SYSTEM_STATUS_NORMAL   = 1,	// 정상
		SYSTEM_STATUS_ABNORMAL = 2	// 비정상
	}Ivdct_System_Status_c;
	/* 시스템 정보 */
	struct Ivdct_System_c
	{
		collection_timestamp status_ct; ///< 시스템 상태 수집 타임스탬프
		Ivdct_System_Status_c status;
	};
	/* 주행 속도 정보 */
	struct Ivdct_Driving_Speed_c
	{
		collection_timestamp wheel_speed_ct; ///< 휠 속도 수집 타임스탬프
		uint32_t wheel_speed; ///< 휠 속도 (Unit:1/256[km/h], Over:64255(250.250.99609375) ,Unknown:64256)
		collection_timestamp yawrate_ct; ///< 수직 축 각속도 수집 타임스탬프
		int32_t yawrate; ///< 수직 축 각속도 (Unit:1/8192[rad/s], Min:0(-3.92), Max:64255(3.9236279296875) ,Unknown:64256)
		collection_timestamp accel_pedal_pos_ct; ///< 가속 페달 위치 수집 타임스탬프
		uint32_t accel_pedal_pos; ///< 가속 페달 위치 (Unit:0.4[%], Max:250(100) ,Unknown:251)
	};
	/* 엔진 정보 */
	struct Ivdct_Driving_Engine_c
	{
		collection_timestamp torque_1_ct; ///< 엔진 토크 수집 타임스탬프
		int32_t torque_1; ///< 엔진 토크 (Unit:1[%], Above:-125, Over:125 ,Unknown:126)
		collection_timestamp torque_2_ct; ///< 엔진 토크(소수점) 수집 타임스탬프
		uint32_t torque_2; ///< 엔진 토크(소수점)(Unit:0.125[%], Max:7(0.875) ,Unknown:8)
		collection_timestamp rpm_ct; ///< 엔진 분당 회전 수 수집 타임스탬프
		uint32_t rpm; ///< 엔진 분당 회전 수 (Unit:0.125[rpm], Over:64255(8031.875) ,Unknown:64256)
	};
	/* 기어 정보 */
	struct Ivdct_Driving_Gear_c
	{
		collection_timestamp ratio_ct; ///< 기어비 수집 타임스탬프
		uint32_t ratio; ///< 기어비 (Unit:0.001, Over:64255(64.255) ,Unknown:64256)
		collection_timestamp current_ct; ///< 기어 단수 수집 타임스탬프
		int32_t current; ///< 기어 단수 (Unit:1[gear], Reverse gear:-125..-1, Neutral:0, Forward gear:1..125, Unknown:126)
	};
	/* 조향각 정보 */
	struct Ivdct_Driving_Steering_wheel_c
	{
		collection_timestamp angle_ct; ///< 조향각 수집 타임스탬프
		uint32_t angle; ///< 조향각 (Unit:1/1024[rad], Min:0(-31.374), Max:64255(31.31.3750234375) ,Unknown:64256)
	};
	/* 브레이크 정보 */
	typedef enum  {	// 브레이크 온도 경고
		BRAKE_TEMP_WARNING_DEACTIVATE_WARNING  = 0,							// 비활성화(경고)
		BRAKE_TEMP_WARNING_ACTIVATE_NO_WARNING = 1,							// 활성화(미경고)
		BRAKE_TEMP_WARNING_NOT_AVAILABLE 			= 3,							// 사용불가
		BRAKE_TEMP_WARNING_UNKNOWN  						= 4,							// 알수없음
	} Ivdct_Driving_Brake_Temp_warning_c;
	/* TCS 활성화 상태 */
	typedef enum Tcs {		// TCS 활성화 상태
		BRAKE_TCS_ON 											= 0,	// 꺼짐
		BRAKE_TCS_OFF 											= 1,	// 켜짐
		BRAKE_TCS_ERROR										= 2,	// 오류
		BRAKE_TCS_NOT_INSTALLED_AVAILABLE 	= 3,	// 미설치/사용불가
		BRAKE_TCS_UNKNOWN  								= 4,	// 알수없음
	} Ivdct_Driving_Brake_Tcs_c;
	/* ABS 활성화 상태 */
	typedef enum {		// ABS 활성화 상태
		BRAKE_ABS_DEACTIVATE  							= 0,	// 비활성화
		BRAKE_ABS_ACTIVATE 								= 1,	// 활성화
		BRAKE_ABS_NOT_INSTALLED_AVAILABLE 	= 3,	// 미설치/사용불가
		BRAKE_ABS_UNKNOWN  								= 4,	// 알수없음
	} Ivdct_Driving_Brake_Abs_c;
	/* 브레이크 정보 */
	struct Ivdct_Driving_Brake_c
	{
		collection_timestamp pedal_pos_ct; ///< 브레이크 페달 위치 수집 타임스탬프
		uint32_t pedal_pos; ///< 브레이크 페달 위치 (Unit:0.04[%], Over:250(100) ,Unavaiable:251)
		collection_timestamp temp_warning_ct; ///< 브레이크 온도 경고 수집 타임스탬프
		Ivdct_Driving_Brake_Temp_warning_c temp_warning; ///< 브레이크 온도 경고 (enumeration:0,1,3,4)
		collection_timestamp tcs_ct; ///< TCS 활성화 상태 수집 타임스탬프
		Ivdct_Driving_Brake_Tcs_c tcs; ///< TCS 활성화 상태 (enumeration:0..4)
		collection_timestamp abs_ct; ///< ABS 활성화 상태 수집 타임스탬프
		Ivdct_Driving_Brake_Abs_c abs; ///< ABS 활성화 상태 (enumeration:0,1,3,4)
	};
	/* 주행 정보 */
	struct Ivdct_Driving_c
	{
		struct Ivdct_Driving_Speed_c speed;
		struct Ivdct_Driving_Engine_c engine;
		struct Ivdct_Driving_Gear_c gear;
		struct Ivdct_Driving_Steering_wheel_c steering_wheel;
		struct Ivdct_Driving_Brake_c brake;
	};
	/* 배터리 전압 정보 */
	struct Ivdct_Diag_Battery_c
	{
		collection_timestamp voltage_ct; ///< 배터리 전압 수집 타임스탬프
		uint32_t voltage; ///< 배터리 전압 (Unit:0.05[V], Over:64255(3212.75) ,Unavaiable:64256)
	};
	/* 엔진 냉각수 정보*/
	struct Ivdct_Diag_Engine_c
	{
		collection_timestamp coolant_temp_ct; ///< 엔진 냉각수 온도 수집 타임스탬프
		uint32_t coolant_temp; ///< 엔진 냉각수 온도 (Unit:1[degree], Above:-40, Over:210 ,Unknown:211)
	};
	/* 연료 연비 정보 */
	struct Ivdct_Diag_Fuel_c
	{
		collection_timestamp economy_ct; ///< 연비 수집 타임스탬프
		uint32_t economy; ///< 연비 (Unit:1/512[km/L], Over:64256(125.5) ,Unavaiable:64257);
	};
	/* 진단 정보 */
	struct Ivdct_Diag_c
	{
		struct Ivdct_Diag_Battery_c battery; ///< 배터리 정보
		struct Ivdct_Diag_Engine_c engine; ///< 엔진 정보
		struct Ivdct_Diag_Fuel_c fuel; ///< 연료 정보	
	};
	/* 비상등 상태 */
	typedef enum  {	// 비상등 상태
		LIGHT_HAZARD_SIG_OFF  									= 0, 	// 꺼짐
		LIGHT_HAZARD_SIG_FLASHING 							= 1,	// 깜빡임
		LIGHT_HAZARD_SIG_ERROR 								= 2,	// 오류
		LIGHT_HAZARD_SIG_NOT_AVAILABLE_CHANGE  = 3,	// 사용불가(변화없음)
		LIGHT_HAZARD_SIG_UNKNOWN  							= 4,	// 알수없음
	} Ivdct_Lights_Hazard_sig_c;
	/* 비상등 정보 */
	struct Ivdct_Lights_c
	{
		collection_timestamp hazard_sig_ct; ///< 비상등 상태 수집 타임스탬프
		Ivdct_Lights_Hazard_sig_c hazard_sig; ///< 비상등 상태 (enumeration:0..4)
	};
	/* 축 하중 정보 */
	struct Ivdct_Weight_Axle_weight_c
	{
		collection_timestamp location_ct; ///< 축 하중 수집 타임스탬프
		int32_t location; ///< 축 위치 (Unit:1[states/bit] ,Unknown:256)
		uint64_t weight_ct; ///< 축 하중 수집 타임스탬프 (Unit:1[ms], 0..(2^64-1))
		uint32_t weight; ///< 축 하중 (Unit:0.5[kg], Over:64255(32125.5) ,Unknown:64256)
	};
	/* 하중 정보 */
	struct Ivdct_Weight_c
	{	
		struct Ivdct_Weight_Axle_weight_c axle_weight[KVHIVDCT_Weight_AxleWeight_Repeated_Max]; ///< 축 하중 수집 타임스탬프
	};

	struct IVDCTDCM_c ///< Forward declaration for IVDCTDCM_c structure
	{
		struct Ivdct_System_c system;
		struct Ivdct_Driving_c driving;
		struct Ivdct_Diag_c diag;
		struct Ivdct_Lights_c lights;
		struct Ivdct_Weight_c weight;
	};
#endif // __IVDCTDCM_PB_C_H__