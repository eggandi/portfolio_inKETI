/**
 * @file
 * @brief DSCMRedis () Protobuf DataCenter 기능 구현
 * @date 2025-10-21
 * @author donggyu
 */

// 모듈 헤더파일
#include "dscm_pb.h"

extern "C" 
{
	
	using namespace std;
	using namespace DSCMDC;
	size_t DSCM_ProcessPBSerializeToString_V2cVehicleData(DSCMDC::V2cVehicleData *V2cVehicleData, char **out_buffer)
	{
		std::string output;
		if (!V2cVehicleData->SerializeToString(&output)) {
			std::cerr << "Failed to serialize V2cVehicleData instance to string." << std::endl;
			return -1;
		}
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
	}


	/**
	 * @brief DSCM Redis Tx Callback 처리 함수
	 * @details Redis에서 수신된 Key를 처리하는 콜백 함수
	 * 					1초 주기로 Redis에서 마지막 수신된 key를 처리한다. 
	 * @param[in] mib DSCM_MIB 포인터
	 * @param[in] key_ptr KVHREDISKey 포인터
	 * @return int 처리 결과 코드 (0: 성공, 음수: 실패)
	 */
	int DSCM_ProcessPBRedisTxCallback(DSCM_MIB *mib, DSCMRedis_VehicleDataKeys *keys_ptr)
	{
		// Redis에서 수신된 Key를 처리하는 로직을 여기에 구현
		// 예: Key에 해당하는 데이터를 Protobuf 메시지로 파싱하거나, 필요한 작업 수행 등
		int ret = 0;
		if(keys_ptr == nullptr) {
			return -1;
		}
		auto *V2cVehicleData = new DSCMDC::V2cVehicleData();
		V2cVehicleData->Clear();
		// Redis Key GET 처리
		for (int key_enum = 0; key_enum < 7; key_enum++) {
			KVHREDISKey *key_ptr = &keys_ptr->key_ADS_HF + key_enum;
			if (key_ptr->key_length == 0) {
				continue;
			}
			KVHREDISHANDLE h = (KVHREDISHANDLE)g_dscm_mib.redis_if.h_cmd;
			char buffer[2048] = {0};
			size_t buflen = 0;
			ret = KVHREDIS_ProcessGETRedis(h, key_ptr, buffer, &buflen);
			if (buflen <= 0) {
				ERR("DSCM Redis GET failed for key: %s, ret: %d", key_ptr->key_str, ret);
				continue;
			}
			
			switch (key_enum) {
				case kDSCMRedis_VehicleDataKeyS_Type_ADS_HF: // key_ADS_HF
					//DSCM_SetPBV2cVehicleDataFromADSHF_CStrcuture(V2cVehicleData, buffer, buflen);
					break;
				case kDSCMRedis_VehicleDataKeyS_Type_ADS_LF: // key_ADS_LF
					break;
				case kDSCMRedis_VehicleDataKeyS_Type_DMS: // key_DMS
					break;
				case kDSCMRedis_VehicleDataKeyS_Type_DTG: // key_DTG
					break;
				case kDSCMRedis_VehicleDataKeyS_Type_IVDCT: // key_IVDCT
					DSCM_SetPBV2cVehicleDataFromIVDCT_CStructure(static_cast<DSCM_PBHandler_Ptr_V2cVehicleData>(V2cVehicleData), (KVHIVDCTData *)buffer);
					break;
				case kDSCMRedis_VehicleDataKeyS_Type_VH_HF: // key_VH_HF
					DSCM_SetPBV2cVehicleDataFromGNSS_CStructure(static_cast<DSCM_PBHandler_Ptr_V2cVehicleData>(V2cVehicleData), (KVHGNSSData *)buffer);
					break;
				case kDSCMRedis_VehicleDataKeyS_Type_VH_LF: // key_VH_LF
					break;
				case kDSCMRedis_VehicleDataKeyS_Type_None: // None
					break;
				default:
					break;
			}
			memset(buffer, 0, sizeof(buffer));
		}
		// 처리된 keys_ptr을 초기화
		memset(keys_ptr, 0, sizeof(DSCMRedis_VehicleDataKeys));
		keys_ptr->isempty = true;

		// V2cVehicleData를 Protobuf 직렬화
		char *sendbuf = NULL;
		size_t sendsize = DSCM_ProcessPBSerializeToString_V2cVehicleData(V2cVehicleData, &sendbuf);
		if (sendsize <= 0) {
			ERR("DSCM Protobuf V2cVehicleData serialization failed, ret: %d\n", sendsize);
			delete(V2cVehicleData);
			return -1;
		}
		// 주행전략 서버로 송신 함수콜
		ret = DSCM_ProcessTxDCDrivingStrategy_V2cVehicleData(mib, (const uint8_t *)sendbuf, sendsize);
		if (ret < 0) {
			ERR("DSCM ProcessTxDCDrivingStrategyV2cVehicleData failed, ret: %d\n", ret);
		}
		free(sendbuf);
		delete(V2cVehicleData);
		return 0;
	}

	/**
	 * @brief DSCM DataCenter핸들러를 생성한다.
	 * @return DSCM_PBHandler_Ptr DSCM 핸들러, 
	 * 				 C++ 입장에서는 구조체이지만 C입장에서는 사용할 수 없는 핸들러
	 * @note 이 함수는 Protobuf 라이브러리를 초기화하고 DSCM 핸들러를 생성한다.
	 *       생성된 인스턴스는 DSCM_DestroyPB() 함수를 통해 해제해야 한다.
	 */
	DSCM_PBHandler_Ptr DSCM_CreatePBHandler_V2cLoginReq() 
	{
		auto *cpp_dscm_DC_v2cloginreq = new V2cLoginReq();
		if (!cpp_dscm_DC_v2cloginreq) {
				std::cerr << "Failed to create DSCM instance." << std::endl;
				return nullptr;
		}
		return static_cast<DSCM_PBHandler_Ptr>(cpp_dscm_DC_v2cloginreq);
	};


	/**
	 * @brief DSCM DataCenter 핸들러를 해제한다.
	 * @param[in] h DSCM_PBHandler_Ptr DSCM 핸들러
	 * @note 이 함수는 Protobuf 라이브러리를 종료하고 DSCM 핸들러를 해제한다.
	 *       만약 h가 nullptr이면 g_cpp_dscm_pb_payload를 해제한다.
	 */
	void DSCM_DestroyPBHandler_V2cLoginReq(DSCM_PBHandler_Ptr h) 
	{
		if (!h) {
			return;
		} else {
			delete static_cast<V2cLoginReq*>(h);
			h = nullptr;
		}
		return;
	};


	/**
	 * @brief DSCM V2cLoginReq 핸들러에 값을 넣는다. 
	 * @param[in] h DSCM_PBHandler_Ptr DSCM 핸들러
	 * @param[in] data 데이터 넣을 구조체 포인터 : 문자열
	 * @return KVHDSCMData* DSCM 핸들러의 C 구조체 포인터
	 * @note 이 함수는 Protobuf 라이브러리를 초기화하고 DSCM 핸들러의 데이터를 C 구조체로 변환한다.
	 */
	int DSCM_SetPBHandleV2cLoginReqFrom_Str(DSCM_PBHandler_Ptr h, const char *data)
	{
		/** 
		 * DSCM 인스턴스의 데이터를 가져온다.
		 */
		V2cLoginReq *v2xlogingreq = nullptr;
		if (h == nullptr) {
		} else {
			v2xlogingreq = static_cast<V2cLoginReq*>(h);
		}		
		if (v2xlogingreq == nullptr) {
			std::cerr << "DSCM instance is null." << std::endl;
			return -1;
		}
		v2xlogingreq->Clear(); // Clear previous data
		v2xlogingreq->set_allocated_authentication_info(new string(data));

		return 0;
	};


	/**
	 * @brief DSCM 핸들러(데이터 포인터)의 데이터를 Protobuf 문자열로 직렬화한다.
	 * @param[in] h DSCM_PBHandler_Ptr DSCM 핸들러, NULL
	 * @param[in] out_buffer 직렬화된 Protobuf 문자열을 저장할 버퍼 포인터
	 * @return size_t 직렬화된 문자열의 크기, 실패 시 -1 반환
	 */
	size_t DSCM_ProcessPBSerializeToString_V2cLoginReq(DSCM_PBHandler_Ptr h, char **out_buffer) 
	{
		V2cLoginReq *v2xlogingreq = nullptr;
		if (h == nullptr) {
			return 0;
		} else {
			v2xlogingreq = static_cast<V2cLoginReq*>(h);
		}

		std::string output;
		if (!v2xlogingreq->SerializeToString(&output)) {
			std::cerr << "Failed to serialize DSCM instance to string." << std::endl;
			return -1;
		}

		// 직렬화된 DSCM 인스턴스를 출력한다.
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

#if 0
	/**
	 * @brief protobuf 문자열을 DSCM 데이터 포인터로 파싱한다. 
	 * @param[in] serialized_data Protobuf 문자열
	 * @param[in] data_size 문자열의 크기
	 * @return int 0: 성공, -1: 실패
	 * @note 이 함수는 Protobuf 문자열을 파싱하여 DSCM 핸들러에 데이터를 입력한다.
	 */
	int DSCM_ProcessPBParseFromArrayDataCenter(DSCM_PBHandler_Ptr h, const uint8_t *serialized_data, size_t data_size)
	{
		// Protobuf 문자열을 DSCM 핸들러에 파싱한다. 
		// DSCM 핸들러가 nullptr이면 g_cpp_dscm_pb_payload를 사용한다.

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
				std::cerr << "Failed to serialize DSCM instance to string." << std::endl;
				return -1;
			} else {	
				// 직렬화된 DSCM 인스턴스를 출력한다.
				// std::cout << dscm->DebugString();  

			}
		};
		return 0;
	};
#endif
}  