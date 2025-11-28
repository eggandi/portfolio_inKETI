/**
 * @file
 * @brief Redis 기능 라이브러리 구현
 * @date 2025-09-19
 * @author user
 */

 //kvhredis 헤더파일
#include "kvhredis.h"
#include "kvhredis_internal.h"

KVHREDIS_LIB g_redis_lib;


/**
 * @brief Redis 서버에 연결을 수행
 * @param[in] lib Redis 라이브러리 핸들
 * @retval 0(성공), -1(파라미터 오류), -2(메모리 할당 오류), -3(연결 오류)
 */
int KVHREDIS_ProcessConnection(KVHREDIS_LIB *lib) 
{
	if (!lib) {
		return -1;
	}
	struct timeval to = {1, 0};
	lib->redis_client.ctx = redisConnectWithTimeout(lib->redis_client.host, lib->redis_client.port, to);
	if (lib->redis_client.ctx == NULL) {
			return -2;
	}
	if (lib->redis_client.ctx->err) {
		fprintf(stderr, "Redis connection error: %s\n", lib->redis_client.ctx->errstr);
		redisFree(lib->redis_client.ctx);
		lib->redis_client.ctx = NULL;
		return -3;
	}
	return 0;
}


/**
 * @brief Redis 라이브러리
 * @details
 * Redis는 오픈소스 기반으로 개발된 인메모리 데이터 구조 저장소로
 * Key-Value 형태로 데이터를 저장한다. 
 * key의 해시는 메모리의 주소, Value은 해당 메모리에 저장되는 데이터를 말한다. 
 * key는 사용자가 직접 만들어서 사용하거나, 핸들러의 key chain을 사용할 수 있다. 
 * key chain에는 index로 key의 static filed를 해시해서 저장하는 설정을 사용 가능.
 * kvhredis 라이브러리는 hiredis c 라이브러리를 기반으로 한다. `
 * replycb는 KVHREDIS_ProcessReplyCallbackDefault 골백 함수가 기본으로 할당되고,
 * 사용자가 필요에 따라 변경
 * 
 */
KVHREDISHANDLE KVHREDIS_Init(KVHREDISHANDLE h)
{
	KVHREDIS_LIB *lib = NULL;
	// 전역변수 사용 아니면 전용 핸들러 사용
	if(h == NULL) {
		lib = &g_redis_lib;
		memset(lib, 0x00, sizeof(KVHREDIS_LIB));
	} else {
		lib = (KVHREDIS_LIB *)h;

	}
	
	// Redis 클라이언트 설정
	lib->redis_client.ctx = NULL;	// Connection 안된 상태
	
	if (h) {

	} else {
		strncpy(lib->redis_client.host, KVH_REDIS_HOST_DEFAULT, KVH_REDIS_HOST_MAX_LENGTH);
		lib->redis_client.port = KVH_REDIS_PORT_DEFAULT; // 기본 포트
		lib->redis_client.timeout.tv_sec = 1; // 1초
		lib->redis_client.timeout.tv_usec = 0; // 0초
		lib->redis_client.replycb = KVHREDIS_ProcessReplyCallbackDefault;
	}

	lib->key.chain = NULL;
	lib->key.active_chain = false;
	lib->key.enable_hash_index = false; // 기본값은 해시 인덱스 사용 안함

	int ret;
	// Redis 서버 연결
	ret = KVHREDIS_ProcessConnection(lib);
	if (ret < 0) {
		return NULL;
	}



	if (pthread_create(&(lib->rx_thread.th), NULL, KVHREDIS_ThreadRedisKeyspaceNotifications, (void*)lib) != 0) {
    return NULL;
  }
	
	return (KVHREDISHANDLE)lib;
}
 
/**
 * @brief Redis 라이브러리 종료
 * @param[in] h Redis 라이브러리 핸들
 */
void KVHREDIS_Close(KVHREDISHANDLE h)
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;
	if (!lib) {
		return;
	}
	// Redis 연결 종료
	if (lib->redis_client.ctx) {
		redisFree(lib->redis_client.ctx);
		lib->redis_client.ctx = NULL;
	}

	// Key 체인 메모리 해제
	if (lib->key.active_chain) {
		KVHREDISKeyNode *current = lib->key.chain;
		while(current->p != NULL) {
			current = current->p;
		}
		while (current != NULL) {
			KVHREDISKeyNode *next = current->c;
			if (current->index) {
				free(current->index);
				current->index = NULL;
			}
			free(current);
			current = next;
		}
		lib->key.chain = NULL;
		return;
	}

	memset(lib, 0x00, sizeof(KVHREDIS_LIB));

}
