/**
 * @file
 * @brief IVDCT () Protobuf 기능 구현
 * @date 2025-06-11
 * @author donggyu
 */

// 모듈 헤더파일
#include "ivdctdcm.h"

#include <iostream>
#include <cstdint>
#include <vector>

#include "ivdct/ivdct.pb.validate.h"  // includes ivdct.pb.h internally
//#include "ivdct/diag/diag.pb.validate.h"
//#include "ivdct/driving/driving.pb.validate.h"
//#include "ivdct/lights/lights.pb.validate.h"
//#include "ivdct/weight/weight.pb.validate.h"

#include "internal_data_fmt/kvh_internal_data_fmt_ivdct.h"

// Protobuf 헤더파일
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>

// 데이터 필드에 값이 있는지 확인 (정의상 사용하지 않을 때는 IVDCT에서 Unkonwn을 보내게 되어있지만 없을 때는 LONG_MAX를 반환함)
// TODO: 반환값에 Unknown이 있는 필드인 경우 Unknown으로 반환할 수 있도록 수정 예정 
#define KVHIVDCT_PROTOBUF_IsFillField(path, name) (path).has_##name ? (path).name : static_cast<decltype((path).name)>(LONG_MAX)

static IVDCT::Ivdct *g_cpp_ivdctdcm_pb_payload = nullptr;


typedef struct{
	bool initialized; ///< Protobuf 라이브러리 초기화 여부
} KVHPROTOBUF_INFO;

static KVHPROTOBUF_INFO *g_kvhprotobuf_info;

extern "C" 
{
	/**
	 * @brief Protobuf 라이브러리를 초기화한다.
	 * @return int 0: 성공, -1: 실패
	 * @note 이 함수는 Protobuf 라이브러리를 초기화하고 KVHPROTOBUF_INFO 
	 * *프로토버퍼 라이브러리에 있어야 하는데 일단 여기에 만들어 넣음
	 */
	extern int KVHPROTOBUF_Init()
	{
		if (g_kvhprotobuf_info && g_kvhprotobuf_info->initialized) 
		{
			return false; // 이미 초기화된 경우
		} else {
			g_kvhprotobuf_info = new KVHPROTOBUF_INFO();
			if (!g_kvhprotobuf_info) {
				return -1; // 메모리 할당 실패
			}
			GOOGLE_PROTOBUF_VERIFY_VERSION;
			g_kvhprotobuf_info->initialized = true;	
		}

		return true;
	}


	/**
	 * @brief Protobuf 라이브러리를 종료한다.
	 * @return int 0: 성공, -1: 실패
	 * @note 이 함수는 Protobuf 라이브러리를 종료하고 KVHPROTOBUF_INFO 프로토버퍼를 해제한다.
	 * *프로토버퍼 라이브러리에 있어야 하는데 일단 여기에 만들어 넣음	 
	 */
	int KVHPROTOBUF_Shutdown()
	{
		if (!g_kvhprotobuf_info || !g_kvhprotobuf_info->initialized)
		{
			return false; // 초기화되지 않은 경우
		}
		google::protobuf::ShutdownProtobufLibrary();
		return true;
	}
	

	/**
	 * @brief Ivdct 핸들러를 생성한다.
	 * @return IVDCTDCM_PBHandler Ivdct 핸들러, 
	 * 				 C++ 입장에서는 구조체이지만 C입장에서는 사용할 수 없는 핸들러
	 * @note 이 함수는 Protobuf 라이브러리를 초기화하고 Ivdct 핸들러를 생성한다.
	 *       생성된 인스턴스는 IVDCTDCM_DestroyPB() 함수를 통해 해제해야 한다.
	 */
	IVDCTDCM_PBHandler IVDCTDCM_CreatePB() 
	{
		KVHPROTOBUF_Init(); // Ensure Protobuf library is initialized
		g_cpp_ivdctdcm_pb_payload = new IVDCT::Ivdct();
		if (!g_cpp_ivdctdcm_pb_payload) {
				std::cerr << "Failed to create Ivdct instance." << std::endl;
				return nullptr;
		}
		return static_cast<IVDCTDCM_PBHandler>(g_cpp_ivdctdcm_pb_payload);
	};


	/**
	 * @brief Ivdct 핸들러를 해제한다.
	 * @param h IVDCTDCM_PBHandler Ivdct 핸들러
	 * @note 이 함수는 Protobuf 라이브러리를 종료하고 Ivdct 핸들러를 해제한다.
	 *       만약 h가 nullptr이면 g_cpp_ivdctdcm_pb_payload를 해제한다.
	 */
	void IVDCTDCM_DestroyPB(IVDCTDCM_PBHandler h) 
	{
		if (!h) {
			delete g_cpp_ivdctdcm_pb_payload;
			g_cpp_ivdctdcm_pb_payload = nullptr;
		} else {
			delete static_cast<IVDCT::Ivdct*>(h);
			h = nullptr;
			g_cpp_ivdctdcm_pb_payload = nullptr;
		}
		// Shutdown Protobuf library
		KVHPROTOBUF_Shutdown(); // Ensure Protobuf library is shutdown
	};


	/**
	 * @brief protobuf 직렬화 메시지를 파싱한 데이터의 validation을 수행
	 */
	int IVDCTDCM_ProcessPBValidate(IVDCTDCM_PBHandler h, IVDCT::Ivdct *ivdct, pgv::ValidationMsg* err)
	{
		// IVDCT 인스턴스의 유효성을 검사한다.
		// 인스턴스의 종류에 따라 에러 시 반환값 구분 -1 ivdct, -2 g_cpp_ivdctdcm_pb_payload, -3 h
		int ret = -1;
		if (ivdct == nullptr) {
			if (h == nullptr){
				ivdct = g_cpp_ivdctdcm_pb_payload;
				ret = -2;
			} else {
				ivdct = static_cast<IVDCT::Ivdct*>(h);
				ret = -3;
			}	
		}
		// Validate의 err은 c코드에서 처리하지 못하므로 반환하지 않음 처리하고 싶으면 ivdctdcm_pb.cc에 추가
		if (err == nullptr) {
			pgv::ValidationMsg inner_err;
			err = &inner_err;
		}
		if (!IVDCT::Validate(*ivdct, err)) {
				std::cerr << "Validation failed: %s" << err << "\n";
				ret = 0;
				return ret;// Validation failed Not Checked
		}

		return 0;
	}


	/**
	 * @brief Ivdct 핸들러의 데이터를 가져온다.
	 * @param h IVDCTDCM_PBHandler Ivdct 핸들러
	 * @return KVHIVDCTData* Ivdct 핸들러의 C 구조체 포인터
	 * @note 이 함수는 Protobuf 라이브러리를 초기화하고 Ivdct 핸들러의 데이터를 C 구조체로 변환한다.
	 */
	int IVDCTDCM_GetPBHandleData(IVDCTDCM_PBHandler h, KVHIVDCTData** out_data)
	{
		/** 
		 * IVDCT 인스턴스의 데이터를 가져온다.
		 */
		IVDCT::Ivdct *ivdct = nullptr;
		if (h == nullptr) {
			ivdct = g_cpp_ivdctdcm_pb_payload;
		} else {
			ivdct = static_cast<IVDCT::Ivdct*>(h);
		}		
		if (ivdct == nullptr) {
			std::cerr << "Ivdct instance is null." << std::endl;
			return -1;
		}
		// IVDCT 인스턴스의 데이터를 저장할 C 구조체 할당
		KVHIVDCTData* c_ivdctdcm_pb_payload = (KVHIVDCTData*)malloc(sizeof(KVHIVDCTData));
		memset(c_ivdctdcm_pb_payload, 0, sizeof(KVHIVDCTData));
		
		if (ivdct->has_system()) { /// 시스템 상태 수집 타임스탬프와 상태를 C 구조체에 채운다.
			c_ivdctdcm_pb_payload->system.status_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->system(), status_ct());
			c_ivdctdcm_pb_payload->system.status = KVHIVDCT_PROTOBUF_IsFillField(ivdct->system(), status());
		}

		if (ivdct->has_driving()) { /// 운전 상태 수집 타임스탬프와 속도, 엔진, 기어, 스티어링 휠, 브레이크를 C 구조체에 채운다.
			if (ivdct->driving().has_speed()) { /// 운전 속도 수집 타임스탬프와 휠 속도, 각속도, 가속 페달 위치를 C 구조체에 채운다.
				c_ivdctdcm_pb_payload->driving.speed.wheel_speed_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().speed(), wheel_speed_ct());
				c_ivdctdcm_pb_payload->driving.speed.wheel_speed = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().speed(), wheel_speed());
				c_ivdctdcm_pb_payload->driving.speed.yawrate_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().speed(), yawrate_ct());
				c_ivdctdcm_pb_payload->driving.speed.yawrate = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().speed(), yawrate());
				c_ivdctdcm_pb_payload->driving.speed.accel_pedal_pos_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().speed(), accel_pedal_pos_ct());
				c_ivdctdcm_pb_payload->driving.speed.accel_pedal_pos = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().speed(), accel_pedal_pos());
			}
			if (ivdct->driving().has_engine()) { /// 엔진 상태 수집 타임스탬프와 엔진 토크, 엔진 RPM을 C 구조체에 채운다.
				c_ivdctdcm_pb_payload->driving.engine.torque_1_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().engine(), torque_1_ct());
				c_ivdctdcm_pb_payload->driving.engine.torque_1 = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().engine(), torque_1());
				c_ivdctdcm_pb_payload->driving.engine.torque_2_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().engine(), torque_2_ct());
				c_ivdctdcm_pb_payload->driving.engine.torque_2 = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().engine(), torque_2());
				c_ivdctdcm_pb_payload->driving.engine.rpm_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().engine(), rpm_ct());
				c_ivdctdcm_pb_payload->driving.engine.rpm = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().engine(), rpm());
			}
			if (ivdct->driving().has_gear()) {	/// 기어 상태 수집 타임스탬프와 기어비, 현재 기어 단수를 C 구조체에 채운다.
				c_ivdctdcm_pb_payload->driving.gear.ratio_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().gear(), ratio_ct());
				c_ivdctdcm_pb_payload->driving.gear.ratio = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().gear(), ratio());
				c_ivdctdcm_pb_payload->driving.gear.current_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().gear(), current_ct());
				c_ivdctdcm_pb_payload->driving.gear.current = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().gear(), current());
			}
			if (ivdct->driving().has_steering_wheel()) { /// 스티어링 휠 상태 수집 타임스탬프와 각도, 각속도를 C 구조체에 채운다.
				c_ivdctdcm_pb_payload->driving.steering_wheel.angle_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().steering_wheel(), angle_ct());
				c_ivdctdcm_pb_payload->driving.steering_wheel.angle = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().steering_wheel(), angle());
			}
			if (ivdct->driving().has_brake()) { /// 브레이크 상태 수집 타임스탬프와 페달 위치, 온도 경고, TCS, ABS를 C 구조체에 채운다.
				c_ivdctdcm_pb_payload->driving.brake.pedal_pos_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().brake(), pedal_pos_ct());
				c_ivdctdcm_pb_payload->driving.brake.pedal_pos = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().brake(), pedal_pos());
				c_ivdctdcm_pb_payload->driving.brake.temp_warning_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().brake(), temp_warning_ct());
				c_ivdctdcm_pb_payload->driving.brake.temp_warning = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().brake(), temp_warning());
				c_ivdctdcm_pb_payload->driving.brake.tcs_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().brake(), tcs_ct());
				c_ivdctdcm_pb_payload->driving.brake.tcs = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().brake(), tcs());
				c_ivdctdcm_pb_payload->driving.brake.abs_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().brake(), abs_ct());
				c_ivdctdcm_pb_payload->driving.brake.abs = KVHIVDCT_PROTOBUF_IsFillField(ivdct->driving().brake(), abs());
			}
		}

		if (ivdct->has_diag()) { /// 진단 상태 수집 타임스탬프와 배터리, 엔진, 연료 정보를 C 구조체에 채운다.
			if (ivdct->diag().has_battery()) {	///	배터리 상태 수집 타임스탬프와 전압, 전류를 C 구조체에 채운다.
				c_ivdctdcm_pb_payload->diag.battery.voltage_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->diag().battery(), voltage_ct());
				c_ivdctdcm_pb_payload->diag.battery.voltage = KVHIVDCT_PROTOBUF_IsFillField(ivdct->diag().battery(), voltage());
			}
			if (ivdct->diag().has_engine()) {	/// 엔진 상태 수집 타임스탬프와 냉각수 온도, 엔진 RPM을 C 구조체에 채운다.
				c_ivdctdcm_pb_payload->diag.engine.coolant_temp_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->diag().engine(), coolant_temp_ct());
				c_ivdctdcm_pb_payload->diag.engine.coolant_temp = KVHIVDCT_PROTOBUF_IsFillField(ivdct->diag().engine(), coolant_temp());
			}
			if (ivdct->diag().has_fuel()) {	/// 연료 상태 수집 타임스탬프와 연료량, 연료 압력, 연료 온도, 연비를 C 구조체에 채운다.
				c_ivdctdcm_pb_payload->diag.fuel.economy_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->diag().fuel(), economy_ct());
				c_ivdctdcm_pb_payload->diag.fuel.economy = KVHIVDCT_PROTOBUF_IsFillField(ivdct->diag().fuel(), economy());
			}
		}
		if (ivdct->has_lights()) {	/// 조명 상태 수집 타임스탬프와 헤드라이트, 브레이크 라이트, 방향 지시등, 경적, 비상등을 C 구조체에 채운다.
			c_ivdctdcm_pb_payload->lights.hazard_sig_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->lights(), hazard_sig_ct());
			c_ivdctdcm_pb_payload->lights.hazard_sig = KVHIVDCT_PROTOBUF_IsFillField(ivdct->lights(), hazard_sig());
		}
		
		for (int i = 1; i < ivdct->weight().axle_weight_size(); ++i) {
			if (i < KVH_IVDCT_AXLE_WEIGHT_MAX_NUM) {	/// 축 하중 수집 타임스탬프와 위치, 하중을 C 구조체에 채운다.
				c_ivdctdcm_pb_payload->weight.axle_weight[i].location_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->weight().axle_weight(i), location_ct());
				c_ivdctdcm_pb_payload->weight.axle_weight[i].location = KVHIVDCT_PROTOBUF_IsFillField(ivdct->weight().axle_weight(i), location());
				c_ivdctdcm_pb_payload->weight.axle_weight[i].weight_ct = KVHIVDCT_PROTOBUF_IsFillField(ivdct->weight().axle_weight(i), weight_ct());
				c_ivdctdcm_pb_payload->weight.axle_weight[i].weight = KVHIVDCT_PROTOBUF_IsFillField(ivdct->weight().axle_weight(i), weight());
			}
		}
		
		/// Ivdct 핸들러의 C 구조체 포인터를 out_data에 할당한다.
		if (*out_data == NULL) {
			*out_data = (KVHIVDCTData*)malloc(sizeof(KVHIVDCTData));
		}		
		memcpy(*out_data, c_ivdctdcm_pb_payload, sizeof(KVHIVDCTData));
		
		free(c_ivdctdcm_pb_payload); // Free the temporary C structure
		c_ivdctdcm_pb_payload = nullptr;
		return 0;
	};


	/**
	 * @brief Ivdct 핸들러(데이터 포인터)의 데이터를 Protobuf 문자열로 직렬화한다.
	 * @param h IVDCTDCM_PBHandler Ivdct 핸들러, NULL
	 * @param out_buffer 직렬화된 Protobuf 문자열을 저장할 버퍼 포인터
	 * @return size_t 직렬화된 문자열의 크기, 실패 시 -1 반환
	 */
	size_t IVDCTDCM_ProcessPBSerializeToString(IVDCTDCM_PBHandler h, char **out_buffer) 
	{
		IVDCT::Ivdct *ivdct = nullptr;
		if (h == nullptr) {
			ivdct = g_cpp_ivdctdcm_pb_payload;
		} else {
			ivdct = static_cast<IVDCT::Ivdct*>(h);
		}

		std::string output;
		if (!ivdct->SerializeToString(&output)) {
			std::cerr << "Failed to serialize Ivdct instance to string." << std::endl;
			return -1;
		}

		// 출력 버퍼가 nullptr이 아니면 직렬화된 문자열을 out_buffer에 할당한다.
		if (out_buffer != nullptr) {
			*out_buffer = (char*)malloc(output.size()); // +1 for null terminator
			if (*out_buffer == nullptr) {
				std::cerr << "Failed to allocate memory for output buffer." << std::endl;
				return -2; 
			}
			memcpy(*out_buffer, output.data(), output.size());
			return output.size(); 
		} else {
			return 0;
		}
	};


	/**
	 * @brief protobuf 문자열을 Ivdct 데이터 포인터로 파싱한다. 
	 * @param serialized_data Protobuf 문자열
	 * @param data_size 문자열의 크기
	 * @return int 0: 성공, -1: 실패
	 * @note 이 함수는 Protobuf 문자열을 파싱하여 Ivdct 핸들러에 데이터를 입력한다.
	 */
	int IVDCTDCM_ProcessPBParseFromArray(IVDCTDCM_PBHandler h, const uint8_t *serialized_data, size_t data_size)
	{
		// Protobuf 문자열을 Ivdct 핸들러에 파싱한다. 
		// Ivdct 핸들러가 nullptr이면 g_cpp_ivdctdcm_pb_payload를 사용한다.

		IVDCT::Ivdct *ivdct = nullptr;
		if (h == nullptr)
		{
			ivdct = g_cpp_ivdctdcm_pb_payload;
		} else {
			ivdct = static_cast<IVDCT::Ivdct*>(h);
		}
		ivdct->Clear(); 

		if (serialized_data != nullptr) 
		{
			if (!ivdct->ParseFromArray(serialized_data, data_size)) 
			{
				std::cerr << "Failed to serialize Ivdct instance to string." << std::endl;
				return -1;
			} else {	
				// 직렬화된 Ivdct 인스턴스를 출력한다.
				//std::cout << ivdct->DebugString();  
				int ret = IVDCTDCM_ProcessPBValidate(nullptr, ivdct, nullptr);
				if (ret != 0) 
				{
					std::cerr << "Failed to validate Ivdct instance." << std::endl;
					return ret - 1; // Validation failed
				}
			}
		}

		return 0;
	};

}  