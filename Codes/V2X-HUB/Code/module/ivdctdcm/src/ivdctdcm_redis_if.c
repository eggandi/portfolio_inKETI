/**
 * @file
 * @brief Redis과의 I/F 기능(=UDS 통신) 구현
 * @date 2024-10-13
 * @author user
 */


// 모듈 헤더파일
#include "ivdctdcm.h"


/** 
 * @brief 지정된 Key로 Data 를 Redis PUBRISH한다.
 * @param[in] mib 모듈 통합관리정보 포인터
 * @param[in] key Redis Key 정보
 * @param[in] data 저장할 데이터 포인터
 * @param[in] data_size 저장할 데이터 크기
 * @return 0: 성공, 음수: 실패
 */
int IVDCTDCM_ProcessRedisPUBLISHToKey(IVDCTDCM_MIB *mib, KVHREDISKey *key, uint8_t *data, size_t data_size) 
{
	int ret;

	// Redis에 저장한다.
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)mib->redis_if.h_cmd;
	if (lib->redis_client.ctx != NULL) {
		ret = KVHREDIS_ProcessRedisPUBLISH((KVHREDISHANDLE)lib, key, (const char *)data, data_size);
		if (ret != 0) {
			ERR("Fail to store Data to Redis server:%d\n", ret);
			return ret;
		}
	}

	return 0;
}


/** 
 * @brief 지정된 Key로 Data 를 Redis 서버에 SET한다.
 * @param[in] mib 모듈 통합관리정보 포인터
 * @param[in] key Redis Key 정보
 * @param[in] data 저장할 데이터 포인터
 * @param[in] data_size 저장할 데이터 크기
 * @return 0: 성공, 음수: 실패
 */
int IVDCTDCM_ProcessRedisSETtoKey(IVDCTDCM_MIB *mib, KVHREDISKey *key, uint8_t *data, size_t data_size) 
{
	int ret;

	// Redis에 저장한다.
	int expired_sec = 30;
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)mib->redis_if.h_cmd;
	ret = KVHREDIS_ProcessRedisSET((KVHREDISHANDLE)lib, key, NULL, (const char *)data, data_size, &expired_sec);
	if (ret != 0) {
		ERR("Fail to store Data to Redis server:%d\n", ret);
		return ret;
	}

	return 0;
}


/**
 * @brief Redis Key Chain에 Key를 추가한다.
 * @param[in] mib 모듈 통합관리정보 포인터
 */
int IVDCTDCM_SetRedisKey(KVHREDISKey *key, KVHREDISKeySUBDOMAIN subdomain, KVHREDISKeyDataModelType datamodel)
{
	if (key == NULL) {
		return -1;
	}
	int ret;
	ret = KVHREDIS_InitializeKey(key);
	if (ret != 0) {
		return -2;
	}

	switch (subdomain) {
		case kKVHREDISKeySUBDOMAIN_IVDCT:
			KVHREDIS_SetKeyStaticField(key, subdomain, "JASTECM", 7, datamodel, kKVHREDISKeyDataModelSubType_None);
			break;
		default:
			return -3;
	}

	return 0;
}



/**
 * @brief Redis I/F 기능을 초기화한다.
 * @param[in] mib 모듈 통합관리정보 포인터
 */
int IVDCTDCM_InitRedisIF(IVDCTDCM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize Redis I/F\n");
	int ret = 0;
  /*
   * Redis 클라이언트 라이브러리를 초기화한다.
	 *
   */

	// publish용은 RecvCallback이 필요없으므로 NULL로 설정
	// Redis I/F 라이브러리 초기화에 직접 값을 입력
	KVHREDIS_LIB *lib_cmd_ctx = (KVHREDIS_LIB *)malloc(sizeof(KVHREDIS_LIB));
	memset(lib_cmd_ctx, 0x00, sizeof(KVHREDIS_LIB));
	strncpy(lib_cmd_ctx->redis_client.host, mib->redis_if.server_addr_str, KVH_REDIS_HOST_MAX_LENGTH);
	lib_cmd_ctx->redis_client.port = mib->redis_if.server_port;
	lib_cmd_ctx->redis_client.timeout.tv_sec = 0; // 1초
	lib_cmd_ctx->redis_client.timeout.tv_usec = 100000; // 0초
	lib_cmd_ctx->rx_thread.running = false;
	lib_cmd_ctx->redis_client.replycb = NULL;
	mib->redis_if.h_cmd = KVHREDIS_Init((KVHREDISHANDLE)lib_cmd_ctx);
	if (mib->redis_if.h_cmd == NULL) {
    ERR("Fail to initialize Redis I/F\n");
    return -1;
  }
	KVHREDIS_LIB *lib_cmd = (KVHREDIS_LIB *)mib->redis_if.h_cmd;

	lib_cmd->key.active_chain = false;
	ret = IVDCTDCM_SetRedisKey(&mib->redis_if.key_ivdct_protobuf, kKVHREDISKeySUBDOMAIN_IVDCT, kKVHREDISKeyDataModelType_Protobuf);
	if (ret != 0) {
		ERR("Fail to add Redis Protobuf Key\n");
		return -1;
	}
	ret = IVDCTDCM_SetRedisKey(&mib->redis_if.key_ivdct_raw, kKVHREDISKeySUBDOMAIN_IVDCT, kKVHREDISKeyDataModelType_Raw);
	if (ret != 0) {
		ERR("Fail to add Redis Raw Key\n");
		return -1;
	}

	sprintf(mib->redis_if.key_ivdct_publish.key_str, "%s", IVDCTDCM_REDIS_PUBLISH_KEY);
	mib->redis_if.key_ivdct_publish.key_length = strlen(mib->redis_if.key_ivdct_publish.key_str);


	// Redis 서버에 접속을 확인

	// Test PING command to Redis server 
	redisReply *reply = redisCommand(lib_cmd->redis_client.ctx, "PING");
	if (reply == NULL) {
		LOG(kKVHLOGLevel_Err,"Command error: %s\n", lib_cmd->redis_client.ctx->errstr);
	} else {
		LOG(kKVHLOGLevel_Event, "SET Command OK: %s\n", reply->str);
		freeReplyObject(reply);

		LOG(kKVHLOGLevel_Init, "Success to initialize Redis I/F\n");
		LOG(kKVHLOGLevel_Init, "Redis I/F: Host=%s, Port=%d\n", lib_cmd->redis_client.host, lib_cmd->redis_client.port);
		LOG(kKVHLOGLevel_Init, "Redis I/F: Publish Key=%s\n", mib->redis_if.key_ivdct_publish.key_str);
		LOG(kKVHLOGLevel_Init, "Redis I/F: Raw Key=%s\n", mib->redis_if.key_ivdct_raw.key_str);
		LOG(kKVHLOGLevel_Init, "Redis I/F: Protobuf Key=%s\n", mib->redis_if.key_ivdct_protobuf.key_str);
	}

  return 0;
}

