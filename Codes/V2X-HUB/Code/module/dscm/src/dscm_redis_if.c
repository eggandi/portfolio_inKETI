/**
 * @file
 * @brief Redis과의 I/F 기능(=TCP 통신) 구현
 * @date 2024-10-13
 * @author user
 */


// 모듈 헤더파일
#include "dscm.h"

DSCMRedis_VehicleDataKeysBuf g_vehicledatakeysbuf; // 이중 버퍼로 1초마다 교환됨


 /**
	* @brief 지정된 Key로 Data 를 Redis PUBRISH한다.
	* @param[in] mib 모듈 통합관리정보 포인터
	* @param[in] key Redis Key 정보 포인터
	* @param[in] data 저장할 데이터 포인터
	* @param[in] data_size 저장할 데이터 크기
	* @return 0: 성공, 음수: 실패
 	*/
int DSCM_ProcessRedisSETtoKey(DSCM_MIB *mib, KVHREDISKey *key, uint8_t *data, size_t data_size) 
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
 * @param[in] arg DSCM_MIB 포인터
 */
void *DSCM_ThreadRedisScheduleTask(void *arg)
{
	DSCM_MIB *mib = (DSCM_MIB *)arg;

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
	its.it_value.tv_sec = 1; // 처음에는 1초 후에 타이머 시작
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
	g_vehicledatakeysbuf.key_now_ptr = NULL;
	while (1) {
		ret = read(tfd, &expirations, sizeof(expirations));
		if (ret != sizeof(expirations)) {
			LOG(kKVHLOGLevel_Err,"read");
			break;
		} else {
			timer_tick_100ms++;
			if(timer_tick_100ms % 10 == 0) {
				mib->redis_if.th_scheduletask_running = true;
				// 1초마다 수행되는 작업
					// Vehicle Data Key Buffer Swap
				int key_idx = -1;
				if (g_vehicledatakeysbuf.isinitialized) {
					key_idx = g_vehicledatakeysbuf.current_idx;
					switch (g_vehicledatakeysbuf.current_idx) {
						case kDSCMRedis_VehicleDataKeysBuf_Index_0:
								LOG(kKVHLOGLevel_Dump, "DSCM Redis Publish Vehicle Data Key Buffer Swap: 0 -> 1\n");
								g_vehicledatakeysbuf.current_idx = kDSCMRedis_VehicleDataKeysBuf_Index_1;
								g_vehicledatakeysbuf.key_now_ptr = &g_vehicledatakeysbuf.keys[kDSCMRedis_VehicleDataKeysBuf_Index_1];
							break;
						case kDSCMRedis_VehicleDataKeysBuf_Index_1:
							LOG(kKVHLOGLevel_Dump, "DSCM Redis Publish Vehicle Data Key Buffer Swap: 1 -> 0\n");
								g_vehicledatakeysbuf.current_idx = kDSCMRedis_VehicleDataKeysBuf_Index_0;
								g_vehicledatakeysbuf.key_now_ptr = &g_vehicledatakeysbuf.keys[kDSCMRedis_VehicleDataKeysBuf_Index_0];
								
							break;
						default:
							break;
					}
					// 초기화
					g_vehicledatakeysbuf.key_now_ptr->isempty = true;
				}
				if (key_idx != -1) {			
				 	ret =	DSCM_ProcessPBRedisTxCallback(mib, &g_vehicledatakeysbuf.keys[key_idx]);				
				}
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
void DSCM_ProcessRedisRxCallback(int type, char* str, size_t *str_len, redisReply *reply)
{
	if (reply != NULL) {
		g_vehicledatakeysbuf.isinitialized = true;
	} else {
		LOG(kKVHLOGLevel_Event, "DSCM_ProcessRedisRxCallback: type=%d, str=%s, str_len=%zu\n", type, str, *str_len);
	}
	// Redis 응답 타입에 따른 처리
	switch (type) {
		case REDIS_REPLY_STRING:
			//LOG(kKVHLOGLevel_Dump, "Redis Reply: %s (len=%zu)\n", str, *str_len);
			break;
		case REDIS_REPLY_ARRAY:
			//LOG(kKVHLOGLevel_Dump, "Redis Reply: Array (elements=%zu)\n", reply->elements);
			// 수신받은 Key를 사용하여 GET 명령어 수행
			KVHREDISKey key;
			memcpy(key.key_str, reply->element[3]->str, reply->element[3]->len);
			key.key_length = reply->element[3]->len;
			if (key.key_length > 0) {
				if (g_dscm_mib.redis_if.th_scheduletask_running == false || g_vehicledatakeysbuf.key_now_ptr == NULL) {
					ERR("Redis Schedule Task thread not running\n");
				} else {
					#if 1
					KVHREDISKey *key_ptr = NULL;
					if (GetStringMatchingStr(key.key_str, "DMS") != NULL) {
						key_ptr = &g_vehicledatakeysbuf.key_now_ptr->key_DMS;
					} else if (GetStringMatchingStr(key.key_str, "DTG") != NULL) {
						key_ptr = &g_vehicledatakeysbuf.key_now_ptr->key_DTG;
					} else if (GetStringMatchingStr(key.key_str, "IVDCT") != NULL) {
						key_ptr = &g_vehicledatakeysbuf.key_now_ptr->key_IVDCT;
					} else if (GetStringMatchingStr(key.key_str, "VHMM") != NULL) {
						if(GetStringMatchingStr(key.key_str, "HIGH_F") != NULL) {
							key_ptr = &g_vehicledatakeysbuf.key_now_ptr->key_VH_HF;
						} else if(GetStringMatchingStr(key.key_str, "LOW_F") != NULL) {
							key_ptr = &g_vehicledatakeysbuf.key_now_ptr->key_VH_LF;
						}
					} 
					if(key_ptr != NULL) {
						g_vehicledatakeysbuf.key_now_ptr->isempty = true;
						memset(key_ptr->key_str, 0x00, KVH_REDIS_KEY_MAX_LENGTH);
						memcpy(key_ptr->key_str, key.key_str, key.key_length);
						key_ptr->key_length = key.key_length;
					}
					
					#endif
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
int DSCM_SetRedisKey(KVHREDISKey *key, KVHREDISKeySUBDOMAIN subdomain, KVHREDISKeyDataModelType datamodel, KVHREDISKeyDataModelSubType datamodelsubtype)
{
	if(key == NULL) {
		return -1;
	}
	int ret;
	ret = KVHREDIS_InitializeKey(key);
	if (ret != 0) {
		return -2;
	}

	switch (subdomain) {
		case kKVHREDISKeySUBDOMAIN_FMCM:
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
int DSCM_SetRedisPSUBSCRIBEKeyspaceToKeyChain(KVHREDISHANDLE h, KVHREDISKeySUBDOMAIN subdomain, KVHREDISKeyDataModelSubType subtype)
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;
	KVHREDISKey *key = (KVHREDISKey *)malloc(sizeof(KVHREDISKey));
	int ret = KVHREDIS_InitializeKey(key);
	if (ret != 0) {
		return -1;
	}

	switch (subdomain) {
		case kKVHREDISKeySUBDOMAIN_DTG: //VHREDIS:VH_RDB:V2XHUB:DTG:Raw
		case kKVHREDISKeySUBDOMAIN_IVDCT: //KVHREDIS:VH_RDB:V2XHUB:IVDCT:Raw
		case kKVHREDISKeySUBDOMAIN_DMS: //KVHREDIS:VH_RDB:V2XHUB:DMS:Raw
		case kKVHREDISKeySUBDOMAIN_ADS: 
		case kKVHREDISKeySUBDOMAIN_VHMM: //KVHREDIS:VH_RDB:V2XHUB:VHMM:Raw:HIGH_F, KVHREDIS:VH_RDB:V2XHUB:VHMM:Raw:LOW_F
			ret = KVHREDIS_SetKeyStaticField(key, subdomain, "VH", 2, kKVHREDISKeyDataModelType_Raw, subtype);
			if (ret != 0) {
				return -1;
			}
			break;
			return -1;
		default:
	}
	
	if(lib->key.active_chain == true) {
		ret = KVHREDIS_AddKeyToKeyChain(h, key);
		if (ret != 0) {
			ERR("Fail to add Key to Key Chain\n");
			return -1;
		}
	}

	ret = KVHREDIS_ProcessRedisPSUBSCRIBEKeyspace(h, key);
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
int DSCM_InitRedisIF(DSCM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize Redis I/F\n");
	int ret = 0;
  /*
   * Redis 클라이언트 라이브러리를 초기화한다.
   */
	// Redis I/F 라이브러리 초기화에 직접 값을 입력
	KVHREDIS_LIB *lib_psub_ctx = (KVHREDIS_LIB *)malloc(sizeof(KVHREDIS_LIB));
	memset(lib_psub_ctx, 0x00, sizeof(KVHREDIS_LIB));
	strncpy(lib_psub_ctx->redis_client.host, mib->redis_if.server_addr_str, KVH_REDIS_HOST_MAX_LENGTH);
	lib_psub_ctx->redis_client.port = mib->redis_if.server_port;
	lib_psub_ctx->redis_client.timeout.tv_sec = 0; // 1초
	lib_psub_ctx->redis_client.timeout.tv_usec = 100 * 1000; // 0초
	/*
	 * replycb 설정으로 NULL 처리하면 rx_thread가 종료
	 * rx_thread는 SUBSCRIBE 용이므로 SUBSCRIBE/PSUBSCRIBE 설정을 안해 놓으면 응답안함.
	 * RX thread는 기본적으로 동작안하고 running을 true로 설정해야 동작
	 */ 
	// psubscribe용은 RecvCallback 설정
	lib_psub_ctx->redis_client.replycb = DSCM_ProcessRedisRxCallback;
	mib->redis_if.h_psub = KVHREDIS_Init((KVHREDISHANDLE)lib_psub_ctx);
	pthread_detach(lib_psub_ctx->rx_thread.th);
	if (mib->redis_if.h_psub == NULL) {
    ERR("Fail to initialize Redis I/F\n");
    return -1;
  }
	
	KVHREDIS_LIB *lib_psub = (KVHREDIS_LIB *)mib->redis_if.h_psub;
	lib_psub->key.active_chain = true;
	ret = DSCM_SetRedisPSUBSCRIBEKeyspaceToKeyChain(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_DTG, kKVHREDISKeyDataModelSubType_None);
	ret = DSCM_SetRedisPSUBSCRIBEKeyspaceToKeyChain(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_IVDCT, kKVHREDISKeyDataModelSubType_None);
	ret = DSCM_SetRedisPSUBSCRIBEKeyspaceToKeyChain(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_DMS, kKVHREDISKeyDataModelSubType_None);
	ret = DSCM_SetRedisPSUBSCRIBEKeyspaceToKeyChain(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_ADS, kKVHREDISKeyDataModelSubType_P_HIGH_F);
	ret = DSCM_SetRedisPSUBSCRIBEKeyspaceToKeyChain(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_ADS, kKVHREDISKeyDataModelSubType_P_LOW_F);
	ret = DSCM_SetRedisPSUBSCRIBEKeyspaceToKeyChain(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_VHMM, kKVHREDISKeyDataModelSubType_P_HIGH_F);
	ret = DSCM_SetRedisPSUBSCRIBEKeyspaceToKeyChain(mib->redis_if.h_psub, kKVHREDISKeySUBDOMAIN_VHMM, kKVHREDISKeyDataModelSubType_P_LOW_F);
	lib_psub->rx_thread.running = true;

	
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


	// Test PING command to Redis server 
	redisReply *reply = redisCommand(lib_cmd->redis_client.ctx, "PING");
	if (reply == NULL) {
		LOG(kKVHLOGLevel_Err,"Command error: %s\n", lib_cmd->redis_client.ctx->errstr);
	} else {
		LOG(kKVHLOGLevel_Event, "SET Command OK: %s\n", reply->str);
		freeReplyObject(reply);

		LOG(kKVHLOGLevel_Init, "Redis I/F: Host=%s, Port=%d\n", lib_cmd->redis_client.host, lib_cmd->redis_client.port);
		if (lib_psub->key.active_chain == true) {
			KVHREDISKeyNode *key_chain = lib_psub->key.chain;
			while (key_chain->p != NULL) {
				key_chain = key_chain->p;
			}
			while (1) {
				LOG(kKVHLOGLevel_Init, "Redis PSUBSCRIBE Key in Chain: %s\n", key_chain->key->key_str);
				if(key_chain->c == NULL) {
					break;
				} else {
					key_chain = key_chain->c;
				}
			}
		}	
		LOG(kKVHLOGLevel_Init, "Success to initialize Redis I/F\n");
	}

	/**
	 * Schedule Task pthread 생성 후 Detach
	 */
	ret = pthread_create(&mib->redis_if.th_scheduletask, NULL, DSCM_ThreadRedisScheduleTask, (void *)mib);
	if (ret != 0) {
		ERR("Fail to create Redis Schedule Task thread\n");
		return -1;
	}
	pthread_detach(mib->redis_if.th_scheduletask);

  return 0;
}

