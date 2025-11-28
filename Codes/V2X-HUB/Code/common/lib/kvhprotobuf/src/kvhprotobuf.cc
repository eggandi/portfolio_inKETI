/**
 * @file
 * @brief IVDCT () Protobuf 기능 구현
 * @date 2025-06-11
 * @author donggyu
 */

#include "kvhprotobuf.h"
#include "kvhprotobuf_internal.h"

static KVHPROTOBUF_INFO *g_kvhprotobuf_info;

extern "C"{
	int OPEN_API KVHPROTOBUF_Init()
	{
		if(g_kvhprotobuf_info && g_kvhprotobuf_info->initialized) 
		{
			return false; // 이미 초기화된 경우
		}else{
			g_kvhprotobuf_info = new KVHPROTOBUF_INFO();
			if(!g_kvhprotobuf_info) {
				return -1; // 메모리 할당 실패
			}
			GOOGLE_PROTOBUF_VERIFY_VERSION;
			g_kvhprotobuf_info->initialized = true;	
		}

		return true;
	}

	int KVHPROTOBUF_Shutdown()
	{
		if(!g_kvhprotobuf_info || !g_kvhprotobuf_info->initialized)
		{
			return false; // 초기화되지 않은 경우
		}
		google::protobuf::ShutdownProtobufLibrary();
		return true;
	}
}
