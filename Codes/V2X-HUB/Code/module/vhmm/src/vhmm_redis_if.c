/**
 * @file
 * @brief Redis과의 I/F 기능(=UDS 통신) 구현
 * @date 2024-10-13
 * @author user
 */


// 모듈 헤더파일
#include "vhmm.h"



/** 
 * @brief 지정된 Key로 Data 를 Redis PUBRISH한다.
 * @param[in] mib 모듈 통합관리정보 포인터
 * @param[in] key Redis Key 정보
 * @param[in] data 저장할 데이터 포인터
 * @param[in] data_size 저장할 데이터 크기
 * @return 0: 성공, 음수: 실패
 */
int VHMM_ProcessRedisPUBLISHToKey(VHMM_MIB *mib, KVHREDISKey *key, uint8_t *data, size_t data_size) 
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
 * @brief 상태정보 메시지를 Redis 서버에 저장한다.
 * @param[in] mib 모듈 통합관리정보 포인터
 * @param[in] key Redis Key 정보
 * @param[in] data 저장할 데이터 포인터
 * @param[in] data_size 저장할 데이터 크기
 * @return 0: 성공, 음수: 실패
 */
int VHMM_ProcessRedisSETtoKey(VHMM_MIB *mib, KVHREDISKey *key, uint8_t *data, size_t data_size) 
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
 * @brief Redis SET/PUBLIISH 하는 쓰레드
 * @param[in] arg VHMM_MIB 포인터
 */
void *VHMM_ThreadRedisPUBLISH(void *arg)
{
	VHMM_MIB *mib = (VHMM_MIB *)arg;

	KVHREDISHANDLE h_cmd = mib->redis_if.h_cmd;
	KVHREDIS_LIB *lib_cmd = (KVHREDIS_LIB *)h_cmd;
	KVHREDISHANDLE gnss_pb_h = mib->protobuf_if.vhmm_gnss_pb_handler;

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
	uint64_t timer_tick_100ms;
	mib->redis_if.th_pub_running = true;
	while (1) {
		ret = read(tfd, &expirations, sizeof(expirations));
		if (ret != sizeof(expirations)) {
			LOG(kKVHLOGLevel_Err,"read");
			break;
		} else {
			timer_tick_100ms++;
			if (timer_tick_100ms % 10 == 0) {
				// 1초 마다 처리할 작업
			}
			// 100ms 마다 처리할 작업
			VHMMHighFREQData gnss_data;
			memset(&gnss_data, 0, sizeof(VHMMHighFREQData));
			ret = VHMM_GetGNSSData(&g_vhmm_mib, &gnss_data);
			if (ret != 0) {
				ERR("VHMM_GetGNSSData failed\n");
				
				continue;
			}
			ret = VHMM_SetPBHandlerDataFromGNSS(gnss_pb_h, &gnss_data);
			if (ret != 0) {
				ERR("VHMM_SetPBHandlerDataFromGNSS failed\n");
				continue;
			}
			char *gnss_protobuf_str_ptr = NULL;
			size_t gnss_protobuf_str_len = 0;
			gnss_protobuf_str_len = VHMM_ProcessPBSerializeToStringGNSS(gnss_pb_h, &gnss_protobuf_str_ptr);
			if (gnss_protobuf_str_ptr == NULL) {
				ERR("VHMM_ProcessPBSerializeToStringGNSS failed\n");
				continue;
			} else {
				ret = VHMM_ProcessRedisPUBLISHToKey(mib, &mib->gnss.key_vhmm_gnss_publish, (uint8_t *)gnss_protobuf_str_ptr, gnss_protobuf_str_len);
				if (ret != 0) {
					ERR("VHMM_ProcessRedisPUBLISHToKey failed:%d\n", ret);
				}
				ret = VHMM_ProcessRedisSETtoKey(mib, &mib->gnss.key_vhmm_gnss_protobuf, (uint8_t *)gnss_protobuf_str_ptr, gnss_protobuf_str_len);
				if (ret != 0) {
					ERR("VHMM_ProcessRedisSETtoKey failed:%d\n", ret);
				}
				ret = VHMM_ProcessRedisSETtoKey(mib, &mib->gnss.key_vhmm_gnss_raw, (uint8_t *)&gnss_data, sizeof(VHMMHighFREQData));
								if (ret != 0) {
					ERR("VHMM_ProcessRedisSETtoKey failed:%d\n", ret);
				}
				free(gnss_protobuf_str_ptr);
			}
		}		
	}
	close(tfd);
	return NULL;
}


void *GetStringMatchingStr(const char *buffer, const char *match_str)
{
	if (buffer == NULL || strlen(buffer) == 0 || match_str == NULL || strlen(match_str) == 0) {
		return NULL;
	}
	char *found = strstr(buffer, match_str);
	if (found) {
		return (void *)found;
	}
	return NULL;
}


/**
 * @brief Redis 서버로부터 수신된 메시지를 처리하는 콜백 함수
 * @param type Redis 응답 타입 (REDIS_REPLY_*)
 * @param str Redis 응답 문자열 (type이 REDIS_REPLY_STRING인 경우 유효)
 * @param str_len Redis 응답 문자열 길이 (type이 REDIS_REPLY_STRING인 경우 유효)
 * @param reply Redis 응답 구조체 포인터 (NULL일 수 있음)
 */
void VHMM_ProcessRedisRxCallback(int type, char* str, size_t *str_len, redisReply *reply)
{
	if (reply != NULL) {
	} else {
		LOG(kKVHLOGLevel_Event, "VHMM_ProcessRedisRxCallback: type=%d, str=%s, str_len=%zu\n", type, str, *str_len);
	}
	// Redis 응답 타입에 따른 처리
	switch (type) {
		case REDIS_REPLY_STRING:
			LOG(kKVHLOGLevel_Dump, "Redis Reply: %s (len=%zu)\n", str, *str_len);
			break;
		case REDIS_REPLY_ARRAY:
			KVHREDISHANDLE h = (KVHREDISHANDLE)g_vhmm_mib.redis_if.h_cmd;
			LOG(kKVHLOGLevel_Dump, "Redis Reply: Array (elements=%zu)\n", reply->elements);
			char buffer[2048] = {0};
			size_t buflen = 0;
			// 수신받은 Key를 사용하여 GET 명령어 수행
			KVHREDISKey key;
			memcpy(key.key_str, reply->element[3]->str, reply->element[3]->len);
			key.key_length = reply->element[3]->len;
			KVHREDIS_ProcessGETRedis(h, &key, buffer, &buflen);
			if (buflen > 0) {
				if (g_vhmm_mib.redis_if.th_pub_running == false) {
					ERR("Redis PUBLISH thread not running\n");
				} else {
					const char *found = NULL;
					found = (const char *)GetStringMatchingStr(key.key_str, "DTG");
					found = (const char *)GetStringMatchingStr(key.key_str, "IVDCT");
					found = (const char *)GetStringMatchingStr(key.key_str, "DMS");
					found = (const char *)GetStringMatchingStr(key.key_str, "ADS");
					//found = (const char *)GetStringMatchingStr(key.key_str, "VHMM");
					if (found) {
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
int VHMM_SetRedisKey(KVHREDISKey *key, KVHREDISKeySUBDOMAIN subdomain, KVHREDISKeyDataModelType datamodel, KVHREDISKeyDataModelSubType datamodelsubtype)
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
		case kKVHREDISKeySUBDOMAIN_VHMM:
			KVHREDIS_SetKeyStaticField(key, subdomain, "VH", 2, datamodel, datamodelsubtype);
			break;
		default:
			return -3;
	}

	return 0;
}


/**
 * @brief Redis 서버에 Keyspace Notifications 필요한 Key를 구독
 * @param[in] mib 모듈 통합관리정보 포인터
 * @param[in] psid PSID
 */
int VHMM_SetRedisPSUBSCRIBEKeyspace(KVHREDISHANDLE h, KVHREDISKeySUBDOMAIN subdomain, KVHREDISKeyDataModelSubType subtype)
{
	KVHREDISKey key;
	int ret = KVHREDIS_InitializeKey(&key);
	if (ret != 0) {
		return -1;
	}

	switch (subdomain) {
		case kKVHREDISKeySUBDOMAIN_DTG:
		case kKVHREDISKeySUBDOMAIN_IVDCT:
		case kKVHREDISKeySUBDOMAIN_DMS: 
		case kKVHREDISKeySUBDOMAIN_ADS: 
			ret = KVHREDIS_SetKeyStaticField(&key, subdomain, "VH", 2, kKVHREDISKeyDataModelType_ASN1KSR1600, subtype);
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
 * @brief Redis I/F 기능을 초기화한다.
 * @param[in] mib 모듈 통합관리정보 포인터
 */
int VHMM_InitRedisIF(VHMM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize Redis I/F\n");
	int ret = 0;
  /*
   * Redis 클라이언트 라이브러리를 초기화한다.
   */
	#if 0 // @todo:상태정보 핸들러 구현 후 활성화
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
	lib_psub_ctx->redis_client.replycb = VHMM_ProcessRedisRxCallback;
	mib->redis_if.h_psub = KVHREDIS_Init((KVHREDISHANDLE)lib_psub_ctx);
	pthread_detach(lib_psub_ctx->rx_thread.th);
	if (mib->redis_if.h_psub == NULL) {
    ERR("Fail to initialize Redis I/F\n");
    return -1;
  }
	
	KVHREDIS_LIB *lib_psub = (KVHREDIS_LIB *)mib->redis_if.h_psub;
	lib_psub->key.active_chain = true;
	ret = VHMM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_DTG, kKVHREDISKeyDataModelSubType_None);
	ret = VHMM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_IVDCT, kKVHREDISKeyDataModelSubType_None);
	ret = VHMM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_DMS, kKVHREDISKeyDataModelSubType_None);
	ret = VHMM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_ADS, kKVHREDISKeyDataModelSubType_P_HIGH_F);
	ret = VHMM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_ADS, kKVHREDISKeyDataModelSubType_P_LOW_F);
	//ret = VHMM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_VHMM, kKVHREDISKeyDataModelSubType_P_HIGH_F);
	//ret = VHMM_SetRedisPSUBSCRIBEKeyspace(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_VHMM, kKVHREDISKeyDataModelSubType_P_LOW_F);
	lib_psub->rx_thread.running = true;

	#endif
	// Redis PUBLISH/SET 용 클라이언트 초기화
	// RecvCallback이 필요없으므로 NULL로 설정
	// Redis I/F 라이브러리 초기화에 직접 값을 입력
	KVHREDIS_LIB *lib_cmd_ctx = (KVHREDIS_LIB *)malloc(sizeof(KVHREDIS_LIB));
	memset(lib_cmd_ctx, 0x00, sizeof(KVHREDIS_LIB));
	strncpy(lib_cmd_ctx->redis_client.host, mib->redis_if.server_addr_str, KVH_REDIS_HOST_MAX_LENGTH);
	lib_cmd_ctx->redis_client.port = mib->redis_if.server_port;
	lib_cmd_ctx->redis_client.timeout.tv_sec = 0; // 1초
	lib_cmd_ctx->redis_client.timeout.tv_usec = 100 * 1000; // 0초
	lib_cmd_ctx->rx_thread.running = false;
	lib_cmd_ctx->redis_client.replycb = NULL;
	mib->redis_if.h_cmd = KVHREDIS_Init((KVHREDISHANDLE)lib_cmd_ctx);
	if (mib->redis_if.h_cmd == NULL) {
    ERR("Fail to initialize Redis I/F\n");
    return -1;
  }
	KVHREDIS_LIB *lib_cmd = (KVHREDIS_LIB *)mib->redis_if.h_cmd;

	lib_cmd->key.active_chain = false;
	ret = VHMM_SetRedisKey(&mib->gnss.key_vhmm_gnss_protobuf, kKVHREDISKeySUBDOMAIN_VHMM, kKVHREDISKeyDataModelType_Protobuf, kKVHREDISKeyDataModelSubType_P_HIGH_F);
	ret = VHMM_SetRedisKey(&mib->gnss.key_vhmm_gnss_raw, kKVHREDISKeySUBDOMAIN_VHMM, kKVHREDISKeyDataModelType_Raw, kKVHREDISKeyDataModelSubType_R_HIGH_F);
	if (ret != 0) {
		ERR("Fail to set Redis Key\n");
		return -1;
	}

	sprintf(mib->gnss.key_vhmm_gnss_publish.key_str, "%s", VHMM_GNSS_REDIS_PUBLISH_KEY);
	mib->gnss.key_vhmm_gnss_publish.key_length = strlen(mib->gnss.key_vhmm_gnss_publish.key_str);


	// Test PING command to Redis server 
	redisReply *reply = redisCommand(lib_cmd->redis_client.ctx, "PING");
	if (reply == NULL) {
		LOG(kKVHLOGLevel_Err,"Command error: %s\n", lib_cmd->redis_client.ctx->errstr);
	} else {
		LOG(kKVHLOGLevel_Event, "SET Command OK: %s\n", reply->str);
		freeReplyObject(reply);
		
		LOG(kKVHLOGLevel_Init, "Redis I/F: Host=%s, Port=%d\n", lib_cmd->redis_client.host, lib_cmd->redis_client.port);
		LOG(kKVHLOGLevel_Init, "Redis I/F: Publish Key=%s\n", mib->gnss.key_vhmm_gnss_publish.key_str);
		LOG(kKVHLOGLevel_Init, "Redis I/F: Raw Key=%s\n", mib->gnss.key_vhmm_gnss_raw.key_str);
		LOG(kKVHLOGLevel_Init, "Redis I/F: Protobuf Key=%s\n", mib->gnss.key_vhmm_gnss_protobuf.key_str);
		LOG(kKVHLOGLevel_Init, "Success to initialize Redis I/F\n");
	}

	/**
	 * PUBLISH pthread 생성 후 Detach
	 */
	ret = pthread_create(&mib->redis_if.th_pub, NULL, VHMM_ThreadRedisPUBLISH, (void *)mib);
	if (ret != 0) {
		ERR("Fail to create Redis PUBLISH thread\n");
		return -1;
	}
	pthread_detach(mib->redis_if.th_pub);

  return 0;
}

