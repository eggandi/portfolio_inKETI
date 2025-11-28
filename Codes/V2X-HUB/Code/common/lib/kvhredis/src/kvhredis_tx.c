/**
 * @file
 * @brief Redis 기능 라이브러리 구현
 * @date 2025-09-19
 * @author user
 */

 //kvhredis 헤더파일
#include "kvhredis.h"
#include "kvhredis_internal.h"


KVHREDISHSETValueType *g_hset_empty = NULL;

/**
 * @brief Redis 서버에 HSET 명령어를 수행
 * @param[in] lib Redis 라이브러리 핸들
 * @param[in] key Redis Key 구조체 포인터
 * @param[in] idx Redis Key index 구조체 포인터 (NULL이면 key.chain의 key 사용)
 * @param[in] expire_sec 키 만료 시간(초 단위, NULL이면 만료 시간 설정 안함)
 * @param[in] format KVHREDISHSETValueType* 가변인자, NULL로 종료
 * @p
 * @retval 0(성공), -1(파라미터 오류), -2(명령어 실행 오류)
 */
int KVHREDIS_ProcessRedisHSET(KVHREDISHANDLE h, KVHREDISKey *key, KVHREDISKeyIndex *idx, int *expire_sec, ...) 
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;
	if (!lib || !lib->redis_client.ctx == 0) {
			return -1;
	}
	/*
		* key가 NULL일때, key chain에 등록해놨으면 등록된 키를 사용	
		* index가 없으면 현재 chain의 key를 사용
		*/ 
	if (!key) {
			if (lib->key.active_chain) {
				if (idx->index && idx->index_length > 0) {
					key = KVHREDIS_SelectKeyFromKeyChain(h, idx);
					if (!key) {
						return -1;
					}
				} else {
					key = lib->key.chain->key;
				}
				key = lib->key.chain->key;
			} else {
				return -1;
			}
	}

	if(g_hset_empty == NULL) {
		g_hset_empty = malloc(sizeof(KVHREDISHSETValueType));
		g_hset_empty->field_name[0] = '\0';
		g_hset_empty->value = malloc(1);
		g_hset_empty->value[0] = '\0';
		g_hset_empty->value_len = 0;		
	}

	// timestamp, sequence 갱신	
	KVHREDIS_ProcessUpdateKeyTimestamp(key);
	KVHREDIS_ProcessUpdateKeySequence(key);

	// 가변인자 처리
	va_list args;
	va_start(args, expire_sec);
	
	KVHREDISHSETValueType *hset_values[KVH_REDIS_HSET_MAX_FIELDS];
	for (int field_count = 0; field_count < KVH_REDIS_HSET_MAX_FIELDS; field_count++) {
		KVHREDISHSETValueType *hset_value = va_arg(args, KVHREDISHSETValueType*);
		if (hset_value == NULL) {
			hset_values[field_count] = g_hset_empty;
			continue; // NULL이면 종료
		}
		hset_values[field_count] = hset_value;
	}	
	va_end(args);

	// Redis HSET 명령어 실행
	redisReply *reply = NULL;
	if(expire_sec) {
		reply = redisCommand(lib->redis_client.ctx, "HSET %s %b %b %b %b %b %b %b %b EX 30", key->key_str, hset_values[0]->field_name, strlen(hset_values[0]->field_name), hset_values[0]->value, hset_values[0]->value_len,
																																																			 hset_values[1]->field_name, strlen(hset_values[1]->field_name), hset_values[1]->value, hset_values[1]->value_len,
																																																			 hset_values[2]->field_name, strlen(hset_values[2]->field_name), hset_values[2]->value, hset_values[2]->value_len,
																																																			 hset_values[3]->field_name, strlen(hset_values[3]->field_name), hset_values[3]->value, hset_values[3]->value_len);
	} else {
		reply = redisCommand(lib->redis_client.ctx, "HSET %s %b %b %b %b %b %b %b %b EX 30", key->key_str, hset_values[0]->field_name, strlen(hset_values[0]->field_name), hset_values[0]->value, hset_values[0]->value_len,
																																																			 hset_values[1]->field_name, strlen(hset_values[1]->field_name), hset_values[1]->value, hset_values[1]->value_len,
																																																			 hset_values[2]->field_name, strlen(hset_values[2]->field_name), hset_values[2]->value, hset_values[2]->value_len,
																																																			 hset_values[3]->field_name, strlen(hset_values[3]->field_name), hset_values[3]->value, hset_values[3]->value_len);
	}
	if (reply == NULL) {
		printf("Command error: %s\n", lib->redis_client.ctx->errstr);
		redisReply *r = redisCommand(lib->redis_client.ctx, "PING");
		if (r == NULL) {
				printf("Command error: %s\n", lib->redis_client.ctx->errstr);
				if (lib->redis_client.ctx->err == REDIS_ERR_IO) {
						perror("I/O error");   // errno 값도 같이 확인
				}
		} else {
				if (r->type == REDIS_REPLY_ERROR) {
						printf("Redis error: %s\n", r->str);
				} else {
						printf("Reply: %s\n", reply->str);
				}
				freeReplyObject(reply);
		}
		return -3;
	} 
	
	int ret = 0;
	if (reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0) {
	} else {
		ret = -1;
	}
	freeReplyObject(reply);
	return ret;	
}


/**
 * @brief Redis 서버에 SET 명령어를 수행
 * @param[in] lib Redis 라이브러리 핸들
 * @param[in] key Redis Key 구조체 포인터
 * @param[in] idx Redis Key index 구조체 포인터 (NULL이면 key.chain의 key 사용)
 * @param[in] value 저장할 값
 * @param[in] value_len 저장할 값 길이
 * @param[in] expire_sec 키 만료 시간(초 단위, NULL이면 만료 시간 설정 안함)
 * @retval 0(성공), -1(파라미터 오류), -2(명령어 실행 오류)
 */
int KVHREDIS_ProcessRedisSET(KVHREDISHANDLE h, KVHREDISKey *key, KVHREDISKeyIndex *idx, const char *value, size_t value_len, int *expire_sec) 
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;
	if (!lib || !lib->redis_client.ctx || !value || value_len == 0) {
			return -1;
	}
	/*
		* key가 NULL일때, key chain에 등록해놨으면 등록된 키를 사용	
		* index가 없으면 현재 chain의 key를 사용
		*/ 
	if (!key) {
			if (lib->key.active_chain) {
				if (idx->index && idx->index_length > 0) {
					key = KVHREDIS_SelectKeyFromKeyChain(h, idx);
					if (!key) {
						return -1;
					}
				} else {
					key = lib->key.chain->key;
				}
				key = lib->key.chain->key;
			} else {
				return -1;
			}
	}

	KVHREDIS_ProcessUpdateKeyTimestamp(key);
	KVHREDIS_ProcessUpdateKeySequence(key);

	// Redis SET 명령어 실행
	redisReply *reply = NULL;
	if(expire_sec) {
		reply = redisCommand(lib->redis_client.ctx, "SET %s %b EX 30", key->key_str, value, value_len);
	} else {
		reply = redisCommand(lib->redis_client.ctx, "SET %s %b", key->key_str, value, value_len);
	}
	if (reply == NULL) {
		printf("Command error: %s\n", lib->redis_client.ctx->errstr);
		redisReply *r = redisCommand(lib->redis_client.ctx, "PING");
		if (r == NULL) {
				printf("Command error: %s\n", lib->redis_client.ctx->errstr);
				if (lib->redis_client.ctx->err == REDIS_ERR_IO) {
						perror("I/O error");   // errno 값도 같이 확인
				}
		} else {
				if (r->type == REDIS_REPLY_ERROR) {
						printf("Redis error: %s\n", r->str);
				} else {
						printf("Reply: %s\n", reply->str);
				}
				freeReplyObject(reply);
		}
		return -2;
	} 
	
	int ret = 0;
	if (reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0) {
	} else {
		ret = -1;
	}
	freeReplyObject(reply);
	return ret;	
}


/**
 * @brief Redis 서버에 PUBLISH 명령어를 수행
 * @param[in] lib Redis 라이브러리 핸들
 * @param[in] key Redis Key 구조체 포인터
 * @param[in] value 저장할 값
 * @param[in] value_len 저장할 값 길이
 * @retval 0(성공), -1(파라미터 오류), -2(명령어 실행 오류)
 */
int KVHREDIS_ProcessRedisPUBLISH(KVHREDISHANDLE h, KVHREDISKey *key, const char *value, size_t value_len) 
{
		KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;	
		if (lib == NULL) {
				return -1;
		}

		if (!lib->redis_client.ctx || !value || value_len == 0) {
				return -2;
		}

		if (!key) {
				return -3;
		}

		// Redis PUBLISH 명령어 실행
		redisReply *reply = redisCommand(lib->redis_client.ctx, "PUBLISH %b %b", key->key_str, key->key_length, value, value_len);
		if (reply == NULL) {
			printf("Redis PUBLISH command failed :%s\n", lib->redis_client.ctx->errstr);
		} else {
			freeReplyObject(reply);
		}	
		return 0;	
}
