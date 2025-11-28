/**
 * @file
 * @brief DSCMRedis () Protobuf 기능 구현
 * @date 2025-10-21
 * @author donggyu
 */

// 모듈 헤더파일
#include "dscm.h"
#include "dscm_pb.h"

//static void *g_cpp_vhmm_pb_payload = nullptr;

KVHPROTOBUF_INFO *g_kvhprotobuf_info = nullptr;

extern "C" 
{
	/**
	 * @brief Protobuf 라이브러리를 초기화한다.
	 * @return int 0: 성공, -1: 실패
	 * @note 이 함수는 Protobuf 라이브러리를 초기화하고 KVHPROTOBUF_INFO 
	 * *프로토버퍼 라이브러리에 있어야 하는데 일단 여기에 만들어 넣음
	 */
	int DSCM_InitPB()
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
	};


	/**
	 * @brief Protobuf 라이브러리를 종료한다.
	 * @return int 0: 성공, -1: 실패
	 * @note 이 함수는 Protobuf 라이브러리를 종료하고 KVHPROTOBUF_INFO 프로토버퍼를 해제한다.
	 * *프로토버퍼 라이브러리에 있어야 하는데 일단 여기에 만들어 넣음	 
	 */
	int DSCM_ClosePB()
	{
		if (!g_kvhprotobuf_info || !g_kvhprotobuf_info->initialized)
		{
			return false; // 초기화되지 않은 경우
		}
		delete static_cast<KVHPROTOBUF_INFO*>(g_kvhprotobuf_info);
		google::protobuf::ShutdownProtobufLibrary();
		return true;
	};

}  