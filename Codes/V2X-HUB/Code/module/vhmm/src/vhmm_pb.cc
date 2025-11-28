/**
 * @file
 * @brief VHMM () Protobuf 기능 구현
 * @date 2025-06-11
 * @author donggyu
 */

// 모듈 헤더파일
#include "vhmm.h"

#include <iostream>
#include <cstdint>
#include <vector>

#include "vh_hf/vh_hf.pb.validate.h"  // includes vh_hf.pb.h internally
#include "vh_lf/vh_lf.pb.validate.h"  // includes vh_lf.pb.h internally


// Protobuf 헤더파일
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>

// 데이터 필드에 값이 있는지 확인 (정의상 사용하지 않을 때는 VHMM에서 Unkonwn을 보내게 되어있지만 없을 때는 LONG_MAX를 반환함)
// TODO: 반환값에 Unknown이 있는 필드인 경우 Unknown으로 반환할 수 있도록 수정 예정 
#define KVHVHMM_PROTOBUF_IsFillField(path, name) (path).has_##name ? (path).name : static_cast<decltype((path).name)>(LONG_MAX)

//static void *g_cpp_vhmm_pb_payload = nullptr;

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
	int KVHPROTOBUF_Init()
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
	 * @brief VHMM 핸들러를 생성한다.
	 * @return VHMM_PBHandler VHMM 핸들러, 
	 * 				 C++ 입장에서는 구조체이지만 C입장에서는 사용할 수 없는 핸들러
	 * @note 이 함수는 Protobuf 라이브러리를 초기화하고 VHMM 핸들러를 생성한다.
	 *       생성된 인스턴스는 VHMM_DestroyPB() 함수를 통해 해제해야 한다.
	 */
	VHMM_PBHandler VHMM_CreatePBHandlerGNSS() 
	{
		auto *cpp_vhmm_VH_hf_payload = new VH_HF::VH_hf();
		if (!cpp_vhmm_VH_hf_payload) {
				std::cerr << "Failed to create VHMM instance." << std::endl;
				return nullptr;
		}
		return static_cast<VHMM_PBHandler>(cpp_vhmm_VH_hf_payload);
	};

/**
	 * @brief VHMM 핸들러를 생성한다.
	 * @return VHMM_PBHandler VHMM 핸들러, 
	 * 				 C++ 입장에서는 구조체이지만 C입장에서는 사용할 수 없는 핸들러
	 * @note 이 함수는 Protobuf 라이브러리를 초기화하고 VHMM 핸들러를 생성한다.
	 *       생성된 인스턴스는 VHMM_DestroyPB() 함수를 통해 해제해야 한다.
	 */
	VHMM_PBHandler VHMM_CreatePBHandlerStates() 
	{
		auto *cpp_vhmm_VH_lf_payload = new VH_HF::VH_hf();
		if (!cpp_vhmm_VH_lf_payload) {
				std::cerr << "Failed to create VHMM instance." << std::endl;
				return nullptr;
		}
		return static_cast<VHMM_PBHandler>(cpp_vhmm_VH_lf_payload);
	};


	/**
	 * @brief VHMM 핸들러를 해제한다.
	 * @param h VHMM_PBHandler VHMM 핸들러
	 * @note 이 함수는 Protobuf 라이브러리를 종료하고 VHMM 핸들러를 해제한다.
	 *       만약 h가 nullptr이면 g_cpp_vhmm_pb_payload를 해제한다.
	 */
	void VHMM_DestroyPBHandlerGNSS(VHMM_PBHandler h) 
	{
		if (!h) {
			return;
		} else {
			delete static_cast<VH_HF::VH_hf*>(h);
			h = nullptr;
		}
	};


	/**
	 * @brief VHMM 핸들러를 해제한다.
	 * @param h VHMM_PBHandler VHMM 핸들러
	 * @note 이 함수는 Protobuf 라이브러리를 종료하고 VHMM 핸들러를 해제한다.
	 *       만약 h가 nullptr이면 g_cpp_vhmm_pb_payload를 해제한다.
	 */
	void VHMM_DestroyPBHandlerStates(VHMM_PBHandler h) 
	{
		if (!h) {
			return;
		} else {
			delete static_cast<VH_LF::VH_lf*>(h);
			h = nullptr;
		}
	};


	/**
	 * @brief protobuf 직렬화 메시지를 파싱한 데이터의 validation을 수행
	 */
	static int VHMM_ProcessPBValidateGNSS(VHMM_PBHandler h, VH_HF::VH_hf *vh_hf, pgv::ValidationMsg* err)
	{
		// VHMM 인스턴스의 유효성을 검사한다.
		// 인스턴스의 종류에 따라 에러 시 반환값 구분 -1 vhmm, -2 g_cpp_vhmm_pb_payload, -3 h
		int ret = -1;
		if (vh_hf == nullptr) {
			if (h == nullptr){
				ret = -2;
			} else {
				vh_hf = static_cast<VH_HF::VH_hf*>(h);
				ret = -3;
			}	
		}
		// Validate의 err은 c코드에서 처리하지 못하므로 반환하지 않음 처리하고 싶으면 vhmm_pb.cc에 추가
		if (err == nullptr) {
			pgv::ValidationMsg inner_err;
			err = &inner_err;
		}
		if (!VH_HF::Validate(*vh_hf, err)) {
				std::cerr << "Validation failed: " << err << "\n";
				return ret;
		}

		return 0;
	}


	/**
	 * @brief protobuf 직렬화 메시지를 파싱한 데이터의 validation을 수행
	 */
	static int VHMM_ProcessPBValidateStates(VHMM_PBHandler h, VH_LF::VH_lf *vh_lf, pgv::ValidationMsg* err)
	{
		// VHMM 인스턴스의 유효성을 검사한다.
		// 인스턴스의 종류에 따라 에러 시 반환값 구분 -1 vhmm, -2 g_cpp_vhmm_pb_payload, -3 h
		int ret = -1;
		if (vh_lf == nullptr) {
			if (h == nullptr){
				ret = -2;
			} else {
				vh_lf = static_cast<VH_LF::VH_lf*>(h);
				ret = -3;
			}	
		}
		// Validate의 err은 c코드에서 처리하지 못하므로 반환하지 않음 처리하고 싶으면 vhmm_pb.cc에 추가
		if (err == nullptr) {
			pgv::ValidationMsg inner_err;
			err = &inner_err;
		}
		if (!VH_LF::Validate(*vh_lf, err)) {
				std::cerr << "Validation failed: " << err << "\n";
				return ret;
		}

		return 0;
	}


	/**
	 * @brief VHMM 핸들러의 데이터를 가져온다.
	 * @param h VHMM_PBHandler VHMM 핸들러
	 * @return KVHVHMMData* VHMM 핸들러의 C 구조체 포인터
	 * @note 이 함수는 Protobuf 라이브러리를 초기화하고 VHMM 핸들러의 데이터를 C 구조체로 변환한다.
	 */
	int VHMM_SetPBHandlerDataFromGNSS(VHMM_PBHandler h, VHMMHighFREQData *data)
	{
		/** 
		 * VHMM 인스턴스의 데이터를 가져온다.
		 */
		VH_HF::VH_hf *vh_hf = nullptr;
		if (h == nullptr) {
		} else {
			vh_hf = static_cast<VH_HF::VH_hf*>(h);
		}		
		if (vh_hf == nullptr) {
			std::cerr << "VHMM instance is null." << std::endl;
			return -1;
		}
		vh_hf->Clear(); // Clear previous data
		VH_HF::GNSS::Gnss *gnss = vh_hf->mutable_gnss();
		gnss->Clear(); // Clear previous data
		gnss->set_timestamp(data->timestamp);
		gnss->set_lat(data->lat);
		gnss->set_lon(data->lon);
		gnss->set_elev(data->elev);
		gnss->set_smajor_axis_acc(data->smajor_axis_acc);
		gnss->set_sminor_axis_acc(data->sminor_axis_acc);	
		gnss->set_smajor_axis_ori(data->smajor_axis_ori);
		gnss->set_speed(data->speed);
		gnss->set_heading(data->heading);
		gnss->set_accel_lon(data->accel_lon);
		gnss->set_accel_lat(data->accel_lat);
		gnss->set_accel_vert(data->accel_vert);
		gnss->set_yawrate(data->yawrate);
		gnss->set_fix(static_cast<VH_HF::GNSS::Gnss::FixType>(data->fix));
		gnss->set_net_rtk_fix(static_cast<VH_HF::GNSS::Gnss::NetRtkFix>(data->net_rtk_fix));

		return 0;
	};


	/**
	 * @brief VHMM 핸들러(데이터 포인터)의 데이터를 Protobuf 문자열로 직렬화한다.
	 * @param h VHMM_PBHandler VHMM 핸들러, NULL
	 * @param out_buffer 직렬화된 Protobuf 문자열을 저장할 버퍼 포인터
	 * @return size_t 직렬화된 문자열의 크기, 실패 시 -1 반환
	 */
	size_t VHMM_ProcessPBSerializeToStringGNSS(VHMM_PBHandler h, char **out_buffer) 
	{
		VH_HF::VH_hf *vh_hf = nullptr;
		if (h == nullptr) {
		} else {
			vh_hf = static_cast<VH_HF::VH_hf*>(h);
		}

		std::string output;
		if (!vh_hf->SerializeToString(&output)) {
			std::cerr << "Failed to serialize VHMM instance to string." << std::endl;
			return -1;
		}

		// 직렬화된 VHMM 인스턴스를 출력한다.
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
	 * @brief protobuf 문자열을 VHMM 데이터 포인터로 파싱한다. 
	 * @param serialized_data Protobuf 문자열
	 * @param data_size 문자열의 크기
	 * @return int 0: 성공, -1: 실패
	 * @note 이 함수는 Protobuf 문자열을 파싱하여 VHMM 핸들러에 데이터를 입력한다.
	 */
	int VHMM_ProcessPBParseFromArrayGNSS(VHMM_PBHandler h, const uint8_t *serialized_data, size_t data_size)
	{
		// Protobuf 문자열을 VHMM 핸들러에 파싱한다. 
		// VHMM 핸들러가 nullptr이면 g_cpp_vhmm_pb_payload를 사용한다.

		VH_HF::VH_hf *vh_hf = nullptr;
		if (h == nullptr)
		{
		} else {
			vh_hf = static_cast<VH_HF::VH_hf*>(h);
		}
		vh_hf->Clear();

		if (serialized_data != nullptr) 
		{
			if (!vh_hf->ParseFromArray(serialized_data, data_size)) 
			{
				std::cerr << "Failed to serialize VHMM instance to string." << std::endl;
				return -1;
			} else {	
				// 직렬화된 VHMM 인스턴스를 출력한다.
				// std::cout << vhmm->DebugString();  
				int ret = VHMM_ProcessPBValidateGNSS(nullptr, vh_hf, nullptr);
				if (ret != 0) 
				{
					std::cerr << "Failed to validate VHMM instance." << std::endl;
					return ret - 1; // Validation failed
				}
			}
		}

		return 0;
	};


}  