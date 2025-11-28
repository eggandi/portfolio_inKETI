/**
 * @file
 * @brief IVDCT () Protobuf 기능 구현
 * @date 2025-06-11
 * @author donggyu
 */

// 모듈 헤더파일
#include "smem.h"

#include <iostream>
#include <cstdint>
#include <vector>

#include "smem/ivdct.pb.validate.h"  // includes ivdct.pb.h internally
#include "smem/diag/diag.pb.validate.h"

#include "internal_data_fmt/kvh_internal_data_fmt_ivdct.h"

// Protobuf 헤더파일
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>

// 데이터 필드에 값이 있는지 확인 (정의상 사용하지 않을 때는 IVDCT에서 Unkonwn을 보내게 되어있지만 없을 때는 LONG_MAX를 반환함)
// TODO: 반환값에 Unknown이 있는 필드인 경우 Unknown으로 반환할 수 있도록 수정 예정 
#define KVHIVDCT_PROTOBUF_IsFillField(path, name) (path).has_##name ? (path).name : static_cast<decltype((path).name)>(LONG_MAX)

static IVDCT::Ivdct *g_cpp_smem_pb_payload = nullptr;


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
	 * @return SMEM_PBHandler Ivdct 핸들러, 
	 * 				 C++ 입장에서는 구조체이지만 C입장에서는 사용할 수 없는 핸들러
	 * @note 이 함수는 Protobuf 라이브러리를 초기화하고 Ivdct 핸들러를 생성한다.
	 *       생성된 인스턴스는 SMEM_DestroyPB() 함수를 통해 해제해야 한다.
	 */
	SMEM_PBHandler SMEM_CreatePB() 
	{
		KVHPROTOBUF_Init(); // Ensure Protobuf library is initialized
		g_cpp_smem_pb_payload = new IVDCT::Ivdct();
		if (!g_cpp_smem_pb_payload) {
				std::cerr << "Failed to create Ivdct instance." << std::endl;
				return nullptr;
		}
		return static_cast<SMEM_PBHandler>(g_cpp_smem_pb_payload);
	};


	/**
	 * @brief Ivdct 핸들러를 해제한다.
	 * @param h SMEM_PBHandler Ivdct 핸들러
	 * @note 이 함수는 Protobuf 라이브러리를 종료하고 Ivdct 핸들러를 해제한다.
	 *       만약 h가 nullptr이면 g_cpp_smem_pb_payload를 해제한다.
	 */
	void SMEM_DestroyPB(SMEM_PBHandler h) 
	{
		if (!h) {
			delete g_cpp_smem_pb_payload;
			g_cpp_smem_pb_payload = nullptr;
		} else {
			delete static_cast<IVDCT::Ivdct*>(h);
			h = nullptr;
			g_cpp_smem_pb_payload = nullptr;
		}
		// Shutdown Protobuf library
		KVHPROTOBUF_Shutdown(); // Ensure Protobuf library is shutdown
	};


	/**
	 * @brief protobuf 직렬화 메시지를 파싱한 데이터의 validation을 수행
	 */
	static int SMEM_ProcessPBValidate(SMEM_PBHandler h, IVDCT::Ivdct *ivdct, pgv::ValidationMsg* err)
	{
		// IVDCT 인스턴스의 유효성을 검사한다.
		// 인스턴스의 종류에 따라 에러 시 반환값 구분 -1 ivdct, -2 g_cpp_smem_pb_payload, -3 h
		int ret = -1;
		if (ivdct == nullptr) {
			if (h == nullptr){
				ivdct = g_cpp_smem_pb_payload;
				ret = -2;
			} else {
				ivdct = static_cast<IVDCT::Ivdct*>(h);
				ret = -3;
			}	
		}
		// Validate의 err은 c코드에서 처리하지 못하므로 반환하지 않음 처리하고 싶으면 smem_pb.cc에 추가
		if (err == nullptr) {
			pgv::ValidationMsg inner_err;
			err = &inner_err;
		}
		if (!IVDCT::Validate(*ivdct, err)) {
				std::cerr << "Validation failed: " << err << "\n";
				return ret;
		}

		return 0;
	}


	/**
	 * @brief Ivdct 핸들러(데이터 포인터)의 데이터를 Protobuf 문자열로 직렬화한다.
	 * @param h SMEM_PBHandler Ivdct 핸들러, NULL
	 * @param out_buffer 직렬화된 Protobuf 문자열을 저장할 버퍼 포인터
	 * @return size_t 직렬화된 문자열의 크기, 실패 시 -1 반환
	 */
	size_t SMEM_ProcessPBSerializeToString(SMEM_PBHandler h, char **out_buffer) 
	{
		IVDCT::Ivdct *ivdct = nullptr;
		if (h == nullptr) {
			ivdct = g_cpp_smem_pb_payload;
		} else {
			ivdct = static_cast<IVDCT::Ivdct*>(h);
		}

		std::string output;
		if (!ivdct->SerializeToString(&output)) {
			std::cerr << "Failed to serialize Ivdct instance to string." << std::endl;
			return -1;
		}

		// 직렬화된 Ivdct 인스턴스를 출력한다.
		std::cout << "Serialized Ivdct instance. " << std::endl;
		// 출력 버퍼가 nullptr이 아니면 직렬화된 문자열을 out_buffer에 할당한다.
		if (out_buffer != nullptr) {
			*out_buffer = (char*)malloc(output.size()); // +1 for null terminator
			printf("Serialized Size:%ld\n", output.size());
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
	int SMEM_ProcessPBParseFromArray(SMEM_PBHandler h, const uint8_t *serialized_data, size_t data_size)
	{
		// Protobuf 문자열을 Ivdct 핸들러에 파싱한다. 
		// Ivdct 핸들러가 nullptr이면 g_cpp_smem_pb_payload를 사용한다.

		IVDCT::Ivdct *ivdct = nullptr;
		if (h == nullptr)
		{
			ivdct = g_cpp_smem_pb_payload;
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
				// std::cout << ivdct->DebugString();  
				int ret = SMEM_ProcessPBValidate(nullptr, ivdct, nullptr);
				if (ret != 0) 
				{
					std::cerr << "Failed to validate Ivdct instance." << std::endl;
					return ret - 1; // Validation failed
				}
			}
		}

		return 0;
	};


	/**
	 * @brief Ivdct 테스트용 메시지를 생성한다.
	 * @return void
	 * @note 이 함수는 Protobuf 라이브러리를 초기화하고 Ivdct 핸들러에 데이터를 입력한다.
	 */
	void SMEM_TestSetPBData(SMEM_PBHandler h) 
	{
		IVDCT::Ivdct *ivdct = nullptr;
		if (h == nullptr)
		{
			ivdct = g_cpp_smem_pb_payload;
		} else {
			ivdct = static_cast<IVDCT::Ivdct*>(h);
		}		

		// 1) Ivdct 메시지
    // 2) system 서브메시지
    {
        auto* sys = ivdct->mutable_system();
        // uint64 status_ct = 1
        sys->set_status_ct(1623855600ULL);
        // enum .IVDCT.SYSTEM.STATUS.Status status = 2
        sys->set_status(IVDCT::SYSTEM::STATUS::Status::SYSTEM_STATUS_UNKNOWN);
    }

    // 3) driving 서브메시지
    {
        auto* drv = ivdct->mutable_driving();
        // speed
        {
            auto* spd = drv->mutable_speed();
            spd->set_wheel_speed_ct(5ULL);
            spd->set_wheel_speed(72U);
            spd->set_yawrate_ct(3ULL);
            spd->set_yawrate(2);
            spd->set_accel_pedal_pos_ct(4ULL);
            spd->set_accel_pedal_pos(25U);
        }
        // engine
        {
            auto* eng = drv->mutable_engine();
            eng->set_torque_1(1);
						eng->set_torque_2(1);
            eng->set_rpm(3500U);
        }
        // gear
        {
            auto* gear = drv->mutable_gear();
						gear->set_ratio(10);
            gear->set_current(10);
        }
        // steering_wheel
        {
            auto* sw = drv->mutable_steering_wheel();
            sw->set_angle_ct(2ULL);
            sw->set_angle(15);
        }
        // brake
        {
            auto* br = drv->mutable_brake();
            br->set_pedal_pos(80U);
						br->set_temp_warning(IVDCT::DRIVING::BRAKE::Temp_warning::BRAKE_TEMP_WARNING_DEACTIVATE_WARNING);
						br->set_tcs(IVDCT::DRIVING::BRAKE::Tcs::BRAKE_TCS_ON);
						br->set_abs(IVDCT::DRIVING::BRAKE::Abs::BRAKE_ABS_DEACTIVATE);
        }
    }

    // 4) diag 서브메시지
    {
        auto* dg = ivdct->mutable_diag();
        // battery
        {
            auto* bat = dg->mutable_battery();
						bat->set_voltage(12);
        }
        // engine
        {
            auto* eng = dg->mutable_engine();
            eng->set_coolant_temp(90);
        }
        // fuel
        {
            auto* fuel = dg->mutable_fuel();
            fuel->set_economy(10);
        }
    }

    // 5) lights 서브메시지
    {
        auto* lt = ivdct->mutable_lights();
        lt->set_hazard_sig(IVDCT::LIGHTS::HAZARD_SIG::Hazard_sig::LIGHT_HAZARD_SIG_OFF);
    }

    // 6) weight 서브메시지
    {
        auto* wt = ivdct->mutable_weight();
        // 반복 필드: axle_weight
        {
            // 첫 번째 Axle_weight 추가
            auto* aw1 = wt->add_axle_weight();
            aw1->set_location_ct(2ULL);
            aw1->set_location(100);
            aw1->set_weight_ct(1ULL);
            aw1->set_weight(800U);
            // 두 번째 Axle_weight (예시)
            auto* aw2 = wt->add_axle_weight();
            aw2->set_location_ct(2ULL);
            aw2->set_location(200);
            aw2->set_weight_ct(1ULL);
            aw2->set_weight(900U);
        }
    }
	};
}  