/**
 * @file
 * @brief Redis과의 I/F 기능(=TCP 통신) 구현
 * @date 2024-10-13
 * @author user
 */


// 모듈 헤더파일
#include "csm.h"

CSMRedis_ProtobufHandler g_protobuf_now_handler;

void *CSM_ThreadRedisPUBLISH(void *arg)
{
	CSM_MIB *mib = (CSM_MIB *)arg;

	KVHREDISHANDLE h_pub = mib->redis_if.h_pub;
	KVHREDIS_LIB *lib_pub = (KVHREDIS_LIB *)h_pub;
	CSMRedisProtobufHandlerInfo *protobuf_info = &mib->redis_if.protobuf_info;
	redisReply *reply;


  /**
	 * timerfd를 사용하여 100ms 마다 Redis PUBLISH 명령어를 수행
	 */

	int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (tfd < 0) {
		LOG(kKVHLOGLevel_Err,"timerfd_create");
		return NULL;
	}

	// timer 설정
	struct itimerspec its;
	its.it_value.tv_sec = 3; // 처음에는 1초 후에 타이머 시작
	its.it_value.tv_nsec = 0;
	its.it_interval.tv_sec = 0;
	its.it_interval.tv_nsec = 100 * 1000 * 1000; // 100ms
	// timer 시작
	if (timerfd_settime(tfd, 0, &its, NULL) < 0) {
		LOG(kKVHLOGLevel_Err,"timerfd_settime");
		close(tfd);
		return NULL;
	}
	int ret;
	uint64_t expirations;
	mib->redis_if.th_pub_running = true;
	while (1) {
		
		ret = read(tfd, &expirations, sizeof(expirations));
		if (ret != sizeof(expirations)) {
			LOG(kKVHLOGLevel_Err,"read");
			break;
		} else {
			// 타이머 이벤트 발생 시 처리할 작업
			// printf("Timer expired %llu times\n", (unsigned long long) expirations);
			char *protobuf_str = NULL;
			size_t protobuf_str_len = 0;
			if (protobuf_info->isfulfill[0]) {
				protobuf_info->isfulfill[0] = false;
				protobuf_info->isfulfill[1] = true;
				g_protobuf_now_handler = protobuf_info->h[1];
				protobuf_str_len = CSMRedis_ProcessProtobufSerializeToString(protobuf_info->h[0], &protobuf_str);
				if (protobuf_str) {
					// Redis PUBLISH 명령어 실행
					reply = redisCommand(lib_pub->redis_client.ctx, "PUBLISH CITS_100ms %b", protobuf_str, protobuf_str_len);
					if (reply == NULL) {
						ERR("Redis PUBLISH command failed\n");
					} else {
						freeReplyObject(reply);
					}	
					free(protobuf_str);
					CSMRedis_ClearProtobufHandler(protobuf_info->h[0]);
				}
			} else if (protobuf_info->isfulfill[1]) {
				protobuf_info->isfulfill[1] = false;
				protobuf_info->isfulfill[0] = true;
				g_protobuf_now_handler = protobuf_info->h[0];
				protobuf_str_len = CSMRedis_ProcessProtobufSerializeToString(protobuf_info->h[1], &protobuf_str);
				if (protobuf_str) {
					// Redis PUBLISH 명령어 실행
					reply = redisCommand(lib_pub->redis_client.ctx, "PUBLISH CITS_100ms %b", protobuf_str, protobuf_str_len);
					if (reply == NULL) {
						ERR("Redis PUBLISH command failed\n");
					} else {
						freeReplyObject(reply);
					}	
					free(protobuf_str);
					CSMRedis_ClearProtobufHandler(protobuf_info->h[1]);
				}
			}
			
		}
	}		

	close(tfd);
	return NULL;
}

/**
 * @brief Redis 서버로부터 수신된 메시지를 처리하는 콜백 함수
 * @param type Redis 응답 타입 (REDIS_REPLY_*)
 * @param str Redis 응답 문자열 (type이 REDIS_REPLY_STRING인 경우 유효)
 * @param str_len Redis 응답 문자열 길이 (type이 REDIS_REPLY_STRING인 경우 유효)
 * @param reply Redis 응답 구조체 포인터 (NULL일 수 있음)
 */
void CSM_ProcessRedisRxCallback(int type, char* str, size_t *str_len, redisReply *reply)
{
	if (reply != NULL) {
	} else {
		LOG(kKVHLOGLevel_Event, "CSM_ProcessRedisRxCallback: type=%d, str=%s, str_len=%zu\n", type, str, *str_len);
	}
	// Redis 응답 타입에 따른 처리
	switch (type) {
		case REDIS_REPLY_STRING:
			LOG(kKVHLOGLevel_Dump, "Redis Reply: %s (len=%zu)\n", str, *str_len);
			break;
		case REDIS_REPLY_ARRAY:
			KVHREDISHANDLE h = (KVHREDISHANDLE)g_csm_mib.redis_if.h_cmd;
			LOG(kKVHLOGLevel_Dump, "Redis Reply: Array (elements=%zu)\n", reply->elements);
			char buffer[2048] = {0};
			size_t buflen = sizeof(buffer);
			// 수신받은 Key를 사용하여 GET 명령어 수행
			KVHREDISKey key;
			memset(&key, 0, sizeof(KVHREDISKey));
			memcpy(key.key_str, reply->element[3]->str, reply->element[3]->len);
			key.key_length = reply->element[3]->len;
			KVHREDIS_ProcessGETRedis(h, &key, buffer, &buflen);
			if (buflen > 0) {
				// "CSM" 문자열이 발견된 경우 처리
				CSMRedisProtobufCITSMsgType psid = 0;
				switch (buffer[1]) {
					case 0x12: psid = kCSMRedisProtobufCITSMsgType_MAP; break; // MAP
					case 0x13: psid = kCSMRedisProtobufCITSMsgType_SPAT; break; // SPAT
					case 0x14: psid = kCSMRedisProtobufCITSMsgType_BSM; break; // BSM
					case 0x1A: psid = kCSMRedisProtobufCITSMsgType_PVD; break; // PVD
					case 0x1B: psid = kCSMRedisProtobufCITSMsgType_RSA; break; // RSA
					case 0x1F: psid = kCSMRedisProtobufCITSMsgType_TIM; break; // TIM
					default: break;
				}
				if (psid == 0) {
					ERR("Unknown PSID in Redis GET Result: %s (len=%zu)\n", buffer, buflen);
					break;
				}
				// Protobuf 핸들러에 메시지 설정
				if (g_csm_mib.redis_if.th_pub_running == false) {
					ERR("Redis PUBLISH thread not running\n");
				} else {
					const char *found = strstr(key.key_str, "CSM");
					if (found) {
						CSMRedis_SetProtobufCITSMsgToHandler(g_protobuf_now_handler, kCSMRedisProtobufCITSMsgFrom_CSM, psid, buffer, buflen);
					} else {
						CSMRedis_SetProtobufCITSMsgToHandler(g_protobuf_now_handler, kCSMRedisProtobufCITSMsgFrom_V2X, psid, buffer, buflen);
					}
				}
			}
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
int CSM_AddRedisKeyChain(KVHREDISHANDLE h, KVHREDISKeySUBDOMAIN subdomain, KVHREDISKeyDataModelSubType subtype)
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

	ret = KVHREDIS_AddKeyToKeyChain(h, key);
	if (ret != 0) {
		free(key);
		return -1;
	}

	return 0;
}

/**
 * @brief Redis 서버에 Keyspace Notifications 필요한 Key를 구독
 * @param[in] mib 모듈 통합관리정보 포인터
 * @param[in] psid PSID
 */
int CSM_SetRedisPSUBSCRIBEKeyspace(KVHREDISHANDLE h, KVHREDISKeySUBDOMAIN subdomain, KVHREDISKeyDataModelSubType subtype)
{
	KVHREDISKey key;
	int ret = KVHREDIS_InitializeKey(&key);
	if (ret != 0) {
		return -1;
	}

	switch (subtype) {
		case kKVHREDISKeyDataModelSubType_PVD:
		case kKVHREDISKeyDataModelSubType_BSM:
		case kKVHREDISKeyDataModelSubType_RSA:
		case kKVHREDISKeyDataModelSubType_TIM:
		case kKVHREDISKeyDataModelSubType_SPAT:
		case kKVHREDISKeyDataModelSubType_MAP:
			ret = KVHREDIS_SetKeyStaticField(&key, subdomain, "V2X", 3, kKVHREDISKeyDataModelType_ASN1KSR1600, subtype);
			if (ret != 0) {
				return -1;
			}
			break;
		default:
			return -1;
	}
	
	ret = KVHREDIS_ProcessRedisPSUBSCRIBEKeyspace(h, &key);
	if (ret != 0) {
		ERR("Fail to set Redis PSUBSCRIBE\n");
		return -1;
	}

	return 0;
}
	


/**
 * @brief Redis 서버에 Keyspace Notifications CSM에 필요한 Key를 구독
 * @param[in] mib 모듈 통합관리정보 포인터
 * @param[in] psid PSID
 */
int CSM_SetRedisPSUBSCRIBE_BSM(KVHREDISHANDLE h)
{
	int ret = 0;
	
	KVHREDISKey key;
	ret = KVHREDIS_InitializeKey(&key);
	ret = KVHREDIS_SetKeyStaticField(&key, kKVHREDISKeySUBDOMAIN_CSM, 
																				 "V2X", 3, 
																				kKVHREDISKeyDataModelType_ASN1KSR1600, 
																				kKVHREDISKeyDataModelSubType_BSM);
	ret = KVHREDIS_ProcessRedisPSUBSCRIBEKeyspace(h, &key);
	if (ret != 0) {
		ERR("Fail to set Redis PSUBSCRIBE\n");
		return -1;
	}

	return 0;
}


/**
 * @brief Redis 서버에 Keyspace Notifications CSM에 필요한 Key를 구독
 * @param[in] mib 모듈 통합관리정보 포인터
 * @param[in] psid PSID
 */
int CSM_SetRedisPSUBSCRIBE_PVD(KVHREDISHANDLE h)
{
	int ret = 0;
	KVHREDISKey key;
	ret = KVHREDIS_InitializeKey(&key);
	ret = KVHREDIS_SetKeyStaticField(&key, kKVHREDISKeySUBDOMAIN_CSM, 
																				 "V2X", 3, 
																				kKVHREDISKeyDataModelType_ASN1KSR1600, 
																				kKVHREDISKeyDataModelSubType_PVD);
	ret = KVHREDIS_ProcessRedisPSUBSCRIBEKeyspace(h, &key);
	if (ret != 0) {
		ERR("Fail to set Redis PSUBSCRIBE\n");
		return -1;
	}
	return 0;
}


/**
 * @brief Redis I/F 기능을 초기화한다.
 * @param[in] mib 모듈 통합관리정보 포인터
 */
int CSM_InitRedisIF(CSM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize Redis I/F\n");
	int ret = 0;
  /*
   * Redis 클라이언트 라이브러리를 초기화한다.
   */
	// Redis I/F 라이브러리 초기화에 직접 값을 입력
	KVHREDIS_LIB *lib_psub_ctx = (KVHREDIS_LIB *)malloc(sizeof(KVHREDIS_LIB));
	memset(lib_psub_ctx, 0x00, sizeof(KVHREDIS_LIB));
	strncpy(lib_psub_ctx->redis_client.host, "172.17.251.1", KVH_REDIS_HOST_MAX_LENGTH);
	lib_psub_ctx->redis_client.port = 6379; // 기본 포트
	lib_psub_ctx->redis_client.timeout.tv_sec = 0; // 1초
	lib_psub_ctx->redis_client.timeout.tv_usec = 100000; // 0초
	/*
	 * replycb 설정으로 NULL 처리하면 rx_thread가 종료
	 * rx_thread는 SUBSCRIBE 용이므로 SUBSCRIBE/PSUBSCRIBE 설정을 안해 놓으면 응답안함.
	 * RX thread는 기본적으로 동작안하고 running을 true로 설정해야 동작
	 */ 
	// psubscribe용은 RecvCallback 설정
	lib_psub_ctx->redis_client.replycb = CSM_ProcessRedisRxCallback;
	mib->redis_if.h_psub = KVHREDIS_Init((KVHREDISHANDLE)lib_psub_ctx);
	pthread_detach(lib_psub_ctx->rx_thread.th);
	if (mib->redis_if.h_psub == NULL) {
    ERR("Fail to initialize Redis I/F\n");
    return -1;
  }
	
	KVHREDIS_LIB *lib_psub = (KVHREDIS_LIB *)mib->redis_if.h_psub;
	lib_psub->key.active_chain = true;
	ret = CSM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_TIM);
	ret = CSM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_SPAT);
	ret = CSM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_RSA);
	ret = CSM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_MAP);
	ret = CSM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_PVD);
	ret = CSM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_BSM);
	ret = CSM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_CSM, kKVHREDISKeyDataModelSubType_BSM);
	ret = CSM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_CSM, kKVHREDISKeyDataModelSubType_PVD);
	lib_psub->rx_thread.running = true;

	// GET용은 RecvCallback이 필요없으므로 NULL로 설정
	// Redis I/F 라이브러리 초기화에 직접 값을 입력
	KVHREDIS_LIB *lib_cmd_ctx = (KVHREDIS_LIB *)malloc(sizeof(KVHREDIS_LIB));
	memset(lib_cmd_ctx, 0x00, sizeof(KVHREDIS_LIB));
	strncpy(lib_cmd_ctx->redis_client.host, "172.17.251.1", KVH_REDIS_HOST_MAX_LENGTH);
	lib_cmd_ctx->redis_client.port = 6379; // 기본 포트
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
	lib_cmd->key.active_chain = true;
	ret = CSM_AddRedisKeyChain(mib->redis_if.h_cmd, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_TIM);
	ret = CSM_AddRedisKeyChain(mib->redis_if.h_cmd, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_SPAT);
	ret = CSM_AddRedisKeyChain(mib->redis_if.h_cmd, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_RSA);
	ret = CSM_AddRedisKeyChain(mib->redis_if.h_cmd, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_MAP);
	ret = CSM_AddRedisKeyChain(mib->redis_if.h_cmd, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_PVD);
	ret = CSM_AddRedisKeyChain(mib->redis_if.h_cmd, kKVHREDISKeySUBDOMAIN_V2X, kKVHREDISKeyDataModelSubType_BSM);
	ret = CSM_AddRedisKeyChain(mib->redis_if.h_cmd, kKVHREDISKeySUBDOMAIN_CSM, kKVHREDISKeyDataModelSubType_BSM);
	ret = CSM_AddRedisKeyChain(mib->redis_if.h_cmd, kKVHREDISKeySUBDOMAIN_CSM, kKVHREDISKeyDataModelSubType_PVD);
	if (ret != 0) {
		ERR("Fail to add Redis Key Chain\n");
		return -1;
	}

	// PUBLISH용은 RecvCallback이 필요없으므로 NULL로 설정
	// Redis I/F 라이브러리 초기화에 직접 값을 입력
	KVHREDIS_LIB *lib_pub_ctx = (KVHREDIS_LIB *)malloc(sizeof(KVHREDIS_LIB));
	memset(lib_pub_ctx, 0x00, sizeof(KVHREDIS_LIB));
	strncpy(lib_pub_ctx->redis_client.host, "172.17.251.1", KVH_REDIS_HOST_MAX_LENGTH);
	lib_pub_ctx->redis_client.port = 6379; // 기본 포트
	lib_pub_ctx->redis_client.timeout.tv_sec = 0; // 1초
	lib_pub_ctx->redis_client.timeout.tv_usec = 100000; // 0초
	lib_pub_ctx->rx_thread.running = false;
	lib_pub_ctx->redis_client.replycb = NULL;
	mib->redis_if.h_pub = KVHREDIS_Init((KVHREDISHANDLE)lib_pub_ctx);
	if (mib->redis_if.h_pub == NULL) {
    ERR("Fail to initialize Redis I/F\n");
    return -1;
  }
	LOG(kKVHLOGLevel_Init, "Success to initialize Redis I/F\n");

	// Test PING command to Redis server 
	redisReply *reply = redisCommand(lib_cmd->redis_client.ctx, "PING");
	if (reply == NULL) {
		LOG(kKVHLOGLevel_Err,"Command error: %s\n", lib_cmd->redis_client.ctx->errstr);
	} else {
		LOG(kKVHLOGLevel_Event, "SET Command OK: %s\n", reply->str);
		freeReplyObject(reply);
	}
	/**	
	 * Protobuf 데이터 포인터 핸들러 초기화
	 * 2개의 핸들러를 100ms 간격으로 순차적으로 사용
	 * 예비용으로 1개 더 생성
	 */
	for (int i=0; i<CSMREDIS_PROTOBUF_DATAPOINTER_HANDLER_MAX; i++) {
		mib->redis_if.protobuf_info.h[i] = CSMRedis_CreateProtobufHandler();
		if (mib->redis_if.protobuf_info.h[i] == NULL) {
			ERR("Fail to create Protobuf Handler %d\n", i);
			return -1;
		}
	}
	if (mib->redis_if.protobuf_info.isfulfill[1] == false && mib->redis_if.protobuf_info.isfulfill[0] == false) {
		g_protobuf_now_handler = mib->redis_if.protobuf_info.h[0];
		mib->redis_if.protobuf_info.isfulfill[0] = true;
	}
	/**
	 * PUBLISH pthread 생성 후 Detach
	 */
	ret = pthread_create(&mib->redis_if.th_pub, NULL, CSM_ThreadRedisPUBLISH, (void *)mib);
	if (ret != 0) {
		ERR("Fail to create Redis PUBLISH thread\n");
		return -1;
	}
	pthread_detach(mib->redis_if.th_pub);

  return 0;
}

