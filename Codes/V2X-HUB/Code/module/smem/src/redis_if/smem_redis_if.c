/**
 * @file
 * @brief Redis과의 I/F 기능(=UDS 통신) 구현
 * @date 2024-10-13
 * @author user
 */


// 모듈 헤더파일
#include "smem.h"


/**
 * @brief KSR1600 메시지를 Redis 서버에 저장한다.
 * @param[in] psid PSID
 * @param[in] data 저장할 데이터 포인터
 * @param[in] data_size 저장할 데이터 크기
 * @return 0: 성공, 음수: 실패
 */
int SMEM_ProcessRedisSETKSR1600(uint32_t psid, uint8_t *data, size_t data_size) 
{

	// Redis에 저장할 키를 생성
	int ret;
	KVHREDISKey key;
	ret = KVHREDIS_InitializeKey(&key);
	if (ret != 0) {
		return -1;
	}

	switch (psid) {
		case SMEM_BSM_MSG_PSID:
		case 82050: ///< 20250903 Dong 추가
			KVHREDIS_SetKeyStaticField(&key, kKVHREDISKeySUBDOMAIN_V2X, "V2X", 3, kKVHREDISKeyDataModelType_ASN1KSR1600, kKVHREDISKeyDataModelSubType_BSM);
			break;
		case SMEM_PVD_MSG_PSID:
			KVHREDIS_SetKeyStaticField(&key, kKVHREDISKeySUBDOMAIN_V2X, "V2X", 3, kKVHREDISKeyDataModelType_ASN1KSR1600, kKVHREDISKeyDataModelSubType_PVD);
			break;
		case SMEM_RSA_MSG_PSID:
			KVHREDIS_SetKeyStaticField(&key, kKVHREDISKeySUBDOMAIN_V2X, "V2X", 3, kKVHREDISKeyDataModelType_ASN1KSR1600, kKVHREDISKeyDataModelSubType_RSA);
			break;
		case SMEM_TIM_MSG_PSID:
			KVHREDIS_SetKeyStaticField(&key, kKVHREDISKeySUBDOMAIN_V2X, "V2X", 3, kKVHREDISKeyDataModelType_ASN1KSR1600, kKVHREDISKeyDataModelSubType_TIM);
			break;
		case SMEM_SPAT_MSG_PSID:
			KVHREDIS_SetKeyStaticField(&key, kKVHREDISKeySUBDOMAIN_V2X, "V2X", 3, kKVHREDISKeyDataModelType_ASN1KSR1600, kKVHREDISKeyDataModelSubType_SPAT);
			break;
		case SMEM_MAP_MSG_PSID:
			KVHREDIS_SetKeyStaticField(&key, kKVHREDISKeySUBDOMAIN_V2X, "V2X", 3, kKVHREDISKeyDataModelType_ASN1KSR1600, kKVHREDISKeyDataModelSubType_MAP);
			break;
		default:
			ERR("Not supported PSID(%u) message is received\n", psid);
			return -2;
	}

	// Redis에 저장한다.
	/*
	* MIB에 저장된 idx값을 넣었을 때 hash_index를 사용하지 않으면 idx와 동일한 키가 있는 경우 해당 키를 사용
	* 이때, 비교는 idx의 길이만큼 비교.
	*/
	KVHREDISKeyIndex idx;
	idx.index = (unsigned char *)key.key_str;
	idx.index_length = key.key_length - KVH_REDIS_KEY_TIMESTAMP_UTC_MS_LENGTH - KVH_REDIS_KEY_SEQUENCE_LENGTH;
	int expired_sec = 30;
	ret = KVHREDIS_ProcessRedisSET(g_smem_mib.redis_if.h, NULL, &idx, (const char *)data, data_size, &expired_sec);
	if ( ret != 0) {
		ERR("Fail to store KSR1600 Data(PSID:%d) to Redis server:%d\n", psid, ret);
		return ret;
	}
	memset(&key, 0x00, sizeof(KVHREDISKey));

	return 0;
}

/** 
 * @brief 입력 데이터를 Redis 서버에 저장한다.
 * @param[in] subdomain 서브도메인
 * @param[in] datamodeltype 데이터모델 타입
 * @param[in] subdatatype 서브 데이터모델 타입
 * @param[in] data 저장할 데이터 포인터
 * @param[in] data_size 저장할 데이터 크기
 * @return 0: 성공, 음수: 실패
 */
int SMEM_ProcessRedisSETData(KVHREDISKeySUBDOMAIN subdomain, KVHREDISKeyDataModelType datamodeltype, KVHREDISKeyDataModelSubType subdatatype, const uint8_t *data, size_t data_size) 
{
	int ret;
	KVHREDISKey key;
	ret = KVHREDIS_InitializeKey(&key);
	if (ret != 0) {
		return - 1;
	}
	ret = KVHREDIS_SetKeyStaticField(&key, subdomain, "V2X", 3, datamodeltype, subdatatype);
	if (ret != 0) {
		ERR("Fail to set Redis key static field\n");
		return - 2;
	}

	// Redis에 저장한다.
	KVHREDISKeyIndex idx;
	idx.index = (unsigned char *)key.key_str;
	idx.index_length = key.key_length - KVH_REDIS_KEY_TIMESTAMP_UTC_MS_LENGTH - KVH_REDIS_KEY_SEQUENCE_LENGTH - 2;
	int expired_sec = 30;
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)g_smem_mib.redis_if.h;
	ret = KVHREDIS_ProcessRedisSET((KVHREDISHANDLE)lib, NULL, &idx, (const char *)data, data_size, &expired_sec);
	if (ret != 0) {
		ERR("Fail to store Data to Redis server:%d\n", ret);
		return ret;
	}
	return 0;
}


/**
 * @brief Redis 수신 콜백 함수
 * @param[in] type Redis Reply 타입
 * @param[in] str Redis Reply 문자열 포인터
 * @param[in] str_len Redis Reply 문자열 길이 포인터
 * @param[in] reply Redis Reply 포인터
 * @return 없음
 */
void SMEM_ProcessRedisRxCallback(int type, char* str, size_t *str_len, redisReply *reply)
{
	if (reply != NULL) {
		reply = (redisReply*)reply;
	}

	switch (type) {
		case REDIS_REPLY_STRING:
			LOG(kKVHLOGLevel_Dump, "Redis Reply: %s (len=%zu)\n", str, *str_len);
			
			break;
		default:
			ERR("Redis Reply: Unknown type %d\n", type);
			break;	
	}
}


/**
 * @brief Redis Key Chain에 Key를 추가한다.
 * @param[in] mib 모듈 통합관리정보 포인터
 */
int SMEM_AddRedisKeyChain(KVHREDISHANDLE h, KVHREDISKeySUBDOMAIN subdomain, KVHREDISKeyDataModelSubType subtype)
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;
	KVHREDISKey *key = (KVHREDISKey *)malloc(sizeof(KVHREDISKey));
	int ret;
	ret = KVHREDIS_InitializeKey(key);
	if (ret != 0) {
		free(key);
		return -1;
	}

	switch (subtype) {
		case kKVHREDISKeyDataModelSubType_PVD:
		case kKVHREDISKeyDataModelSubType_BSM:
		case kKVHREDISKeyDataModelSubType_RSA:
		case kKVHREDISKeyDataModelSubType_TIM:
		case kKVHREDISKeyDataModelSubType_SPAT:
		case kKVHREDISKeyDataModelSubType_MAP:
			KVHREDIS_SetKeyStaticField(key, subdomain, "V2X", 3, kKVHREDISKeyDataModelType_ASN1KSR1600, subtype);
			break;
		default:
			return -1;
	}

	ret = KVHREDIS_AddKeyToKeyChain(lib, key);
	if (ret != 0) {
		free(key);
		return -1;
	}

	return 0;
}


/**
 * @brief Redis I/F 기능을 초기화한다.
 * @param[in] mib 모듈 통합관리정보 포인터
 */
int SMEM_InitRedisIF(SMEM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize Redis I/F\n");

  /*
   * Redis 클라이언트 라이브러리를 초기화한다.
	 *
   */
	KVHREDIS_LIB *lib_ctx = (KVHREDIS_LIB *)malloc(sizeof(KVHREDIS_LIB));
	memset(lib_ctx, 0x00, sizeof(KVHREDIS_LIB));
	strncpy(lib_ctx->redis_client.host, "172.17.251.1", KVH_REDIS_HOST_MAX_LENGTH);
	lib_ctx->redis_client.port = 6379; // 기본 포트
	lib_ctx->redis_client.replycb = NULL;
  mib->redis_if.h = KVHREDIS_Init((KVHREDISHANDLE) lib_ctx);
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)mib->redis_if.h;

  if (mib->redis_if.h == NULL) {
    ERR("Fail to initialize Redis I/F\n");
    return -1;
  }
	//replycb 설정으로 NULL 처리하면 rx_thread가 종료
	
	// RX thread는 기본적으로 동작안하고 running을 true로 설정해야 동작
	lib->rx_thread.running = false;

	int ret = 0;
	lib->key.active_chain = true;
	ret = SMEM_AddRedisKeyChain((KVHREDISHANDLE) lib, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_TIM);
	ret = SMEM_AddRedisKeyChain((KVHREDISHANDLE) lib, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_SPAT);
	ret = SMEM_AddRedisKeyChain((KVHREDISHANDLE) lib, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_RSA);
	ret = SMEM_AddRedisKeyChain((KVHREDISHANDLE) lib, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_MAP);
	ret = SMEM_AddRedisKeyChain((KVHREDISHANDLE) lib, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_PVD);
	ret = SMEM_AddRedisKeyChain((KVHREDISHANDLE) lib, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_BSM);
	ret = SMEM_AddRedisKeyChain((KVHREDISHANDLE) lib, kKVHREDISKeySUBDOMAIN_CSM, kKVHREDISKeyDataModelSubType_BSM);
	ret = SMEM_AddRedisKeyChain((KVHREDISHANDLE) lib, kKVHREDISKeySUBDOMAIN_CSM, kKVHREDISKeyDataModelSubType_PVD);
	if (ret != 0) {
		ERR("Fail to add Redis Key Chain\n");
		return -1;
	}
  sleep(1);
	LOG(kKVHLOGLevel_Init, "Success to initialize Redis I/F\n");
	// Redis 서버에 연결 확인
	redisReply *reply = redisCommand(lib->redis_client.ctx, "PING");
	if (reply == NULL) {
		printf("Command error: %s\n", lib->redis_client.ctx->errstr);
		return -1;
	} else {
		printf("SET Command OK: %s\n", reply->str);
		freeReplyObject(reply);
	}
  return 0;
}

