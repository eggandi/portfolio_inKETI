/**
 * @file
 * @brief CSMRedis () Protobuf 기능 구현
 * @date 2025-06-11
 * @author donggyu
 */

// 모듈 헤더파일
#include "csm.h"

#include <iostream>
#include <cstdint>
#include <vector>
#include <new>
#include <mutex>

#include "csmredis/csmredis.pb.validate.h"  // includes csmredis.pb.h internally

// Protobuf 헤더파일
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>

// 데이터 필드에 값이 있는지 확인 (정의상 사용하지 않을 때는 CSMRedis에서 Unkonwn을 보내게 되어있지만 없을 때는 LONG_MAX를 반환함)
// TODO: 반환값에 Unknown이 있는 필드인 경우 Unknown으로 반환할 수 있도록 수정 예정 
#define KVHCSMRedis_PROTOBUF_IsFillField(path, name) (path).has_##name ? (path).name : static_cast<decltype((path).name)>(LONG_MAX)

static CSMRedis::CsmRedis *g_cpp_csmredis_pb_payload = nullptr;

typedef struct{
	bool initialized; ///< Protobuf 라이브러리 초기화 여부
} CSMRedisProtobufInfo;

static CSMRedisProtobufInfo *g_kvhprotobuf_info;

namespace { std::mutex g_pb_mtx; }  // 공용 보호용

extern "C" 
{
	/**
	 * @brief Protobuf 라이브러리를 초기화한다.
	 * @return int 0: 성공, -1: 실패
	 * @note 이 함수는 Protobuf 라이브러리를 초기화하고 CSMRedisProtobufInfo 
	 * *프로토버퍼 라이브러리에 있어야 하는데 일단 여기에 만들어 넣음
	 */
	int CSMRedis_InitProtobufLibrary()
	{
		if (g_kvhprotobuf_info && g_kvhprotobuf_info->initialized) 
		{
			return false; // 이미 초기화된 경우
		} else {
			g_kvhprotobuf_info = new CSMRedisProtobufInfo();
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
	 * @details 이 함수는 Protobuf 라이브러리를 종료하고 CSMRedisProtobufInfo 프로토버퍼를 해제한다.
	 * 					프로토버퍼 라이브러리에 있어야 하는데 일단 여기에 만들어 넣음	 
	 * @return int 0: 성공, -1: 실패
	 */
	int CSMRedis_ShutdownProtobufLibrary()
	{
		if (!g_kvhprotobuf_info || !g_kvhprotobuf_info->initialized)
		{
			return false; // 초기화되지 않은 경우
		}
		delete g_kvhprotobuf_info;
		g_kvhprotobuf_info = nullptr;
		delete g_cpp_csmredis_pb_payload;
		g_cpp_csmredis_pb_payload = nullptr;
		google::protobuf::ShutdownProtobufLibrary();
		return true;
	}
	

	/**
	 * @brief CsmRedis 핸들러를 생성한다.
	 * @details 이 함수는 Protobuf 라이브러리를 초기화하고 CsmRedis 핸들러를 생성한다.
	 *      	  생성된 인스턴스는 CSMRedis_DestroyProtobuf() 함수를 통해 해제해야 한다.
	 * @return CSMRedis_ProtobufHandler CsmRedis 핸들러, 
	 * 				 C++ 입장에서는 구조체이지만 C입장에서는 사용할 수 없는 핸들러

	 */
	CSMRedis_ProtobufHandler CSMRedis_CreateProtobufHandler() 
	{
		CSMRedis::CsmRedis *csmredis = nullptr;
		csmredis = new CSMRedis::CsmRedis();
		if (!csmredis) {
				std::cerr << "Failed to create CsmRedis instance." << std::endl;
				return nullptr;
		}
		if (!g_kvhprotobuf_info || !g_kvhprotobuf_info->initialized)
		{
			CSMRedis_InitProtobufLibrary(); // Ensure Protobuf library is initialized
			g_kvhprotobuf_info->initialized = true;
		}
		
		return static_cast<CSMRedis_ProtobufHandler>(csmredis);
	};


	/**
	 * @brief CsmRedis 핸들러를 해제한다.
	 * @param h CSMRedis_ProtobufHandler CsmRedis 핸들러
	 * @note 이 함수는 Protobuf 라이브러리를 종료하고 CsmRedis 핸들러를 해제한다.
	 *       만약 h가 nullptr이면 g_cpp_csmredis_pb_payload를 해제한다.
	 */
	void CSMRedis_DestroyProtobuf(CSMRedis_ProtobufHandler h) 
	{
		if (!h) {
			delete g_cpp_csmredis_pb_payload;
			g_cpp_csmredis_pb_payload = nullptr;
		} else {
			delete static_cast<CSMRedis::CsmRedis*>(h);
			h = nullptr;
			g_cpp_csmredis_pb_payload = nullptr;
		}
		// Shutdown Protobuf library
		//KVHPROTOBUF_Shutdown(); // Ensure Protobuf library is shutdown
	};


	/**
	 * @brief protobuf 직렬화 메시지를 파싱한 데이터의 validation을 수행
	 */
	int CSMRedis_ProcessProtobufValidate(CSMRedis_ProtobufHandler h, CSMRedis::CsmRedis *csmredis, pgv::ValidationMsg* err)
	{
		// CSMRedis 인스턴스의 유효성을 검사한다.
		// 인스턴스의 종류에 따라 에러 시 반환값 구분 -1 csmredis, -2 g_cpp_csmredis_pb_payload, -3 h
		int ret = -1;
		if (csmredis == nullptr) {
			if (h == nullptr){
				csmredis = g_cpp_csmredis_pb_payload;
				ret = -2;
			} else {
				csmredis = static_cast<CSMRedis::CsmRedis*>(h);
				ret = -3;
			}	
		}
		// Validate의 err은 c코드에서 처리하지 못하므로 반환하지 않음 처리하고 싶으면 csmredis_pb.cc에 추가
		if (err == nullptr) {
			pgv::ValidationMsg inner_err;
			err = &inner_err;
		}
		if (!CSMRedis::Validate(*csmredis, err)) {
				std::cerr << "Validation failed: " << err << "\n";
				return ret;
		}

		return 0;
	}


	/**
	 * @brief CsmRedis 핸들러(데이터 포인터)의 데이터를 초기화한다.
	 * @param[in] h CSMRedis_ProtobufHandler CsmRedis 핸들러, NULL
	 */	
	void CSMRedis_ClearProtobufHandler(CSMRedis_ProtobufHandler h)
	{
		CSMRedis::CsmRedis *csmredis = nullptr;
		if (h == nullptr) {
			csmredis = g_cpp_csmredis_pb_payload;
		} else {
			csmredis = static_cast<CSMRedis::CsmRedis*>(h);
		}
		std::lock_guard<std::mutex> lk(g_pb_mtx);
		csmredis->Clear();
	}


	/**
	 * @brief CsmRedis 핸들러(데이터 포인터)의 데이터를 Protobuf 문자열로 직렬화한다.
	 * @param[in] h CSMRedis_ProtobufHandler CsmRedis 핸들러, NULL
	 * @param[in] msg_from CSMRedisProtobufCITSMsgFrom 메시지 출처
	 * @param[in] msg_type CSMRedisProtobufCITSMsgType 메시지 타입
	 * @param[in] cits_msg Protobuf 문자열 버퍼 포인터
	 * @param[in] msg_size cits_msg 버퍼 크기
	 * @return size_t 직렬화된 문자열의 크기, 실패 시 -1 반환
	 */
	size_t CSMRedis_SetProtobufCITSMsgToHandler(CSMRedis_ProtobufHandler h, CSMRedisProtobufCITSMsgFrom msg_from, CSMRedisProtobufCITSMsgType msg_type, char *cits_msg, size_t msg_size) 
	{
		CSMRedis::CsmRedis *csmredis = nullptr;
		if (h == nullptr) {	
			csmredis = g_cpp_csmredis_pb_payload;
		} else {
			csmredis = static_cast<CSMRedis::CsmRedis*>(h);
		}
		auto now = std::chrono::system_clock::now();
    auto epoch_ms = std::chrono::duration_cast<std::chrono::milliseconds>( now.time_since_epoch()).count();
		CSMRedis::CsmRedis::J2735_Msg* msg = nullptr;
		std::lock_guard<std::mutex> lk(g_pb_mtx);
		switch (msg_type) {
			case kCSMRedisProtobufCITSMsgType_MAP:// MAP
				msg = csmredis->add_v2x_map_arr();
				break;
			case kCSMRedisProtobufCITSMsgType_SPAT: // SPAT
				msg = csmredis->add_v2x_spat_arr();
				break;
			case kCSMRedisProtobufCITSMsgType_RSA:// RSA
				msg = csmredis->add_v2x_rsa_arr();
				break;
			case kCSMRedisProtobufCITSMsgType_TIM:// TIM
				msg = csmredis->add_v2x_tim_arr();
				break;
			case kCSMRedisProtobufCITSMsgType_BSM:// BSM
			case kCSMRedisProtobufCITSMsgType_BSM_KSR:// BSM
				if (msg_from == kCSMRedisProtobufCITSMsgFrom_V2X) {
					msg = csmredis->add_v2x_bsm_arr();
				} else {
					msg = csmredis->add_csm_bsm_arr();
				}
				break;
			case kCSMRedisProtobufCITSMsgType_PVD:// PVD
				if (msg_from == kCSMRedisProtobufCITSMsgFrom_V2X) {
					msg = csmredis->add_v2x_pvd_arr();
				} else {
					msg = csmredis->add_csm_pvd_arr();
				}
			default: break;
		}

		if(msg != nullptr) {
			msg->set_msg(std::string(cits_msg, msg_size));
			msg->set_timestamp(static_cast<uint64_t>(epoch_ms));

			pgv::ValidationMsg* err = nullptr;
			CSMRedis::Validate(*csmredis, err);
			if (err == nullptr) {
			} else {
				std::cerr << "Validation errors: " << *err << std::endl;
			}
		} else {
			return -1;
		}
		
		return 0;
	};

	/**
	 * @brief CsmRedis 핸들러(데이터 포인터)의 데이터를 Protobuf 문자열로 직렬화한다.
	 * @param h CSMRedis_ProtobufHandler CsmRedis 핸들러, NULL
	 * @param out_buffer 직렬화된 Protobuf 문자열을 저장할 버퍼 포인터
	 * @return size_t 직렬화된 문자열의 크기, 실패 시 -1 반환
	 */
	size_t CSMRedis_ProcessProtobufSerializeToString(CSMRedis_ProtobufHandler h, char **out_buffer) 
	{
		 CSMRedis::CsmRedis *src = (h==nullptr) ? g_cpp_csmredis_pb_payload : static_cast<CSMRedis::CsmRedis*>(h);

	
		CSMRedis::CsmRedis snapshot;
    {
        std::lock_guard<std::mutex> lk(g_pb_mtx);   // ⬅️ 아주 짧게: 복사만
        snapshot = *src;
    }
		// 직렬화된 CsmRedis 인스턴스를 출력한다.
		// 출력 버퍼가 NULL 아니면 직렬화된 문자열을 out_buffer에 할당한다.
		std::string output;
		if (!snapshot.SerializeToString(&output)) {
			std::cerr << "Failed to serialize CsmRedis instance to string." << std::endl;
			return -1;
		}
    if (!out_buffer) return 0;
    if (output.empty()) { *out_buffer = nullptr; return 0; }

    *out_buffer = (char*)malloc(output.size());     // %b만 쓰면 널문자 불필수
    if (!*out_buffer) return (size_t)-2;
    memcpy(*out_buffer, output.data(), output.size());
    return output.size();
	}

}  // namespace