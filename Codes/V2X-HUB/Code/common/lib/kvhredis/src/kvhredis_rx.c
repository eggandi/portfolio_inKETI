/**
 * @file
 * @brief Redis 기능 라이브러리 구현
 * @date 2025-09-19
 * @author user
 */

 //kvhredis 헤더파일
#include "kvhredis.h"
#include "kvhredis_internal.h"


/**
 * @brief Redis 서버 응답 처리 콜백 함수 기본 구현
 * @details
 * V2X_HUB Project에서는 Redis에 바이너리 형태로 저장하기 때문에 REDIS_REPLY_STRING만 사용
 * 그러나, 다른 타입일 경우에도 처리를 하기 위해서 redisReply reply에 포인터를 전달한다. (사용자가 맞춰서 사용)
 * @param[in] type 응답 타입 
 * @param[in] str 응답 문자열
 * @param[in] str_len 응답 문자열 길이
 * @param[in] reply redisReply 구조체 포인터(REDIS_REPLY_STRING이 아닐때만 사용)
 */
void KVHREDIS_ProcessReplyCallbackDefault(int type, char* str, size_t *str_len, redisReply *reply)
{
	switch (type) {
		case REDIS_REPLY_STRING:
			printf("Redis Reply: %s (len=%zu)\n", str, *str_len);
			break;
		case REDIS_REPLY_ARRAY:
			printf("Redis Reply: Array (elements=%zu)\n", reply->elements);
			break;
		case REDIS_REPLY_INTEGER:
			printf("Redis Reply: Integer (value=%lld)\n", reply->integer);
			break;
		case REDIS_REPLY_NIL:
			printf("Redis Reply: Nil\n");
			break;
		case REDIS_REPLY_STATUS:
			printf("Redis Reply: Status (str=%s)\n", str);
			break;
		case REDIS_REPLY_ERROR:
			printf("Redis Reply: Error (str=%s)\n", str);
			break;
		case REDIS_REPLY_DOUBLE:
			printf("Redis Reply: Double (value=%f)\n", reply->dval);
			break;
		case REDIS_REPLY_BOOL:
			printf("Redis Reply: Bool (value=%lld)\n", reply->integer);
			break;
		case REDIS_REPLY_MAP:
			printf("Redis Reply: Map (elements=%zu)\n", reply->elements);
			break;
		case REDIS_REPLY_SET:
			printf("Redis Reply: Set (elements=%zu)\n", reply->elements);
			break;
		case REDIS_REPLY_ATTR:
			printf("Redis Reply: Attr (elements=%zu)\n", reply->elements);	
			break;
		case REDIS_REPLY_PUSH:
			printf("Redis Reply: Push (elements=%zu)\n", reply->elements);
			break;	
		case REDIS_REPLY_BIGNUM:
			printf("Redis Reply: BigNum (str=%s)\n", str);
			break;
		case REDIS_REPLY_VERB:
			printf("Redis Reply: Verb (str=%s)\n", str);
			break;	
		default:
			printf("Redis Reply: Unknown type %d\n", type);
			break;	
	}
}


/**
 * @brief Redis 서버에 SET 명령어를 수행
 * @param[in] lib Redis 라이브러리 핸들
 * @param[in] key Redis Key 구조체 포인터
 * @param[in] value 저장할 값
 * @param[in] value_len 저장할 값 길이
 * @retval 0(성공), -1(파라미터 오류), -2(명령어 실행 오류)
 */
int KVHREDIS_ProcessGETRedis(KVHREDISHANDLE h, KVHREDISKey *key, char *value, size_t *value_len)
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;

	if (!lib || !lib->redis_client.ctx || !key || !value || value_len == 0) {
			return -1;
	}

	if (!key) {
		return -1;
	}

	// Redis GET 명령어 실행
	redisReply *reply = redisCommand(lib->redis_client.ctx, "GET %s", key->key_str);
	if (!reply) {
		return -2; // 명령어 실행 실패
	}

	int rc = 0;
	if (reply->type == REDIS_REPLY_STRING) {
		if (value) {
			memcpy(value, reply->str, reply->len);
			*value_len = reply->len;
		}
	} else {
		rc = -1;
	}
	freeReplyObject(reply);
	return rc;
}



/**
 * @brief Redis 서버에 Keyspace Notifications 설정
 * @param[in] ctx Redis 컨텍스트
 * @retval 0(성공), -1(파라미터 오류)
 */
int KVHREDIS_SETServerKeyspaceNotifications(KVHREDISHANDLE h)
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;
	redisContext *ctx = lib->redis_client.ctx;
	redisReply *reply = redisCommand(ctx, "CONFIG GET notify-keyspace-events");
	if (reply == NULL) {
		return -1;
	}

	if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 2) {
			const char *value = reply->element[1]->str;
			if (strcmp(value, "KEA") != 0) {
					freeReplyObject(reply);
					// 설정 변경
					reply = redisCommand(ctx, "CONFIG SET notify-keyspace-events KEA");
					if (reply == NULL) {
							return -1;
					}
					freeReplyObject(reply);
			} else {
					freeReplyObject(reply);
			}
	} else {
			freeReplyObject(reply);
	}
	return 0;
}


/**
 * @brief Redis 서버에 Keyspace Notifications 구독 설정
 * @param[in] ctx Redis 컨텍스트
 * @param[in] akey Redis Key 구조체 포인터 (NULL이면 key.chain의 key 사용)
 * @retval 0(성공), -1(파라미터 오류), -2(명령어 실행 오류)
 */
int KVHREDIS_ProcessRedisPSUBSCRIBEKeyspace(KVHREDISHANDLE h, KVHREDISKey *akey)
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;
	if (!lib) {
		return -1;
	}
	redisContext *ctx = lib->redis_client.ctx;
	char patternp[1024] = {"\0"};
	/* 
	* Keyspace Notifications 구독 설정Set
	*/
	if (akey) { ///< key가 있으면 해당 key만 구독
		int key_len = akey->key_length - KVH_REDIS_KEY_SEQUENCE_LENGTH - KVH_REDIS_KEY_TIMESTAMP_UTC_MS_LENGTH - 2;
		sprintf(patternp, "PSUBSCRIBE __keyevent@0__:set __keyspace@0__:%.*s", key_len, akey->key_str);
		redisReply *reply = redisCommand(ctx, patternp);
		if (!reply) {
			printf("Command error: %s\n", ctx->errstr);
			return -1;
		} else {
			if (reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0) {
			} else {
			}
		}
		freeReplyObject(reply);
	} else if (lib->key.active_chain) { ///< key.chain 활성화 되어있으면 chain의 key를 모두 구독
		KVHREDISKeyNode *chain = lib->key.chain;
		while (chain->p) {
			chain = (KVHREDISKeyNode *)chain->p;
		}
		sprintf(patternp, "%s", "PSUBSCRIBE __keyevent@0__:set");
		uint32_t ptrp = strlen(patternp);
		patternp[ptrp++] = ' ';
		while(chain != NULL) {
			// key를 pattern에 추가
			memcpy(patternp + ptrp, "__keyspace@0__:", 15);
			ptrp += 15;
			KVHREDISKey *key = chain->key;
			size_t key_len = key->key_length - KVH_REDIS_KEY_SEQUENCE_LENGTH - KVH_REDIS_KEY_TIMESTAMP_UTC_MS_LENGTH - 2;
			memcpy(patternp + ptrp, key->key_str, key_len);
			ptrp += key_len;
			patternp[ptrp++] = ' ';
			chain = (KVHREDISKeyNode *)chain->c;
		}
		redisReply *reply = redisCommand(ctx, patternp);
		if (!reply) {
			return -2;
		}
    if (reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0) {
    } else {
    }
		freeReplyObject(reply);
	} 

	return 0;
}

/** 
 * @brief Redis Keyspace Notifications 이벤트 수신 스레드 함수
 * @details
 * Server에 key-value입력이 주어졌을 때 이벤트를 수신하여, 등록된 콜백 함수 호출
 * Key 필터는 입력 인자인 arg가 있으면 arg와 매칭될 때 콜백함수를 호출하고 사용하고
 * 없으면 라이브러리의 key chain이 활성화 되어있을 때 매칭되는 키를 찾아서 콜백함수를 호출한다.
 * server의 입력이 busy하면 테스크가 빠르게 돌기 때문에 불리할 수 있다.
 * @param[in] arg 스레드 함수 인자 (KVHREDISKey 포인터)
 * @retval 0(성공), -1(파라미터 오류)
 */
void *KVHREDIS_ThreadRedisKeyspaceNotifications(void *arg)
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)arg;
	redisContext *ctx = lib->redis_client.ctx;
	if (!ctx) {
		return NULL;
	}
	
	// 이벤트 수신 루프
	int ret = 0;
	redisReply *reply = NULL;
	while (1) {
		if(lib->rx_thread.running == false) {
			if (lib->redis_client.replycb == NULL) {
				return NULL;
			}
			sleep(1);
			continue;
		}
		printf("Waiting for Redis Keyspace Notifications...\n");
		while (1) {
			ret = redisGetReply(ctx, (void **)&reply);
			if(ret != REDIS_OK) {
				printf("redisGetReply error: %s\n", ctx->errstr);
				continue;
			} else {
				if (reply == NULL) {
					continue;
				}
			}
		
			if (strcmp(reply->element[0]->str, "pmessage") == 0 && reply->elements == 4) {
				const char *pattern = reply->element[1]->str;
        const char *channel = reply->element[2]->str;
        const char *msg = reply->element[3]->str;
				if(pattern == NULL || channel == NULL || msg == NULL) {
					freeReplyObject(reply);
					continue;
				}
				if(reply->type == REDIS_REPLY_STRING) {
					lib->redis_client.replycb(reply->type, reply->str, &reply->len, NULL);
				} else {
					lib->redis_client.replycb(reply->type, NULL, NULL, reply);
				}
				
			}
			freeReplyObject(reply);
		}
	}
	
	return NULL;
}
