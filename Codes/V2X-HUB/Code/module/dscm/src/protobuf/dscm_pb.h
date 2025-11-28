/**
 * @file
 * @brief DSCMRedis () Protobuf 기능 구현
 * @date 2025-10-21
 * @author donggyu
 */


#ifndef V2X_HUB_DSCM_PB_H
#define V2X_HUB_DSCM_PB_H
#ifdef __cplusplus

#include "dscm.h"

#include <iostream>
#include <cstdint>
#include <vector>
#include <new>

// 주행전략 차내장치 상태정보 V2X HUB Syntax
#include "ivdct/ivdct.pb.validate.h"  // includes ivdct.pb.h internally
#include "vh_hf/vh_hf.pb.validate.h"  // includes vh_hf.pb.h internally

// 주행전략 전송 프로토콜 Syntax
#include "dscmdc/enums.pb.h"  
#include "dscmdc/common.pb.h"  
#include "dscmdc/core.pb.h"  
#include "dscmdc/strategy.pb.h"  
#include "dscmdc/vehicle_data.pb.h"
#include "dscmdc/transport.pb.h"

// Protobuf 헤더파일
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>

// 데이터 필드에 값이 있는지 확인 (정의상 사용하지 않을 때는 DSCM에서 Unkonwn을 보내게 되어있지만 없을 때는 LONG_MAX를 반환함)
// TODO: 반환값에 Unknown이 있는 필드인 경우 Unknown으로 반환할 수 있도록 수정 예정 

//static void *g_cpp_vhmm_pb_payload = nullptr;

extern "C"
{
	typedef struct{
		bool initialized; ///< Protobuf 라이브러리 초기화 여부
	} KVHPROTOBUF_INFO;

	extern KVHPROTOBUF_INFO *g_kvhprotobuf_info;

	// dscm_pb.h
	#define KVH_PROTOBUF_IsFillField(path, name) (path).has_##name ? (path).name : static_cast<decltype((path).name)>(LONG_MAX)
}
#endif // __cplusplus
#endif