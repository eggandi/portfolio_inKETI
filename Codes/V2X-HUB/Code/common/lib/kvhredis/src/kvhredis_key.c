/**
 * @file
 * @brief Redis 기능 라이브러리 구현
 * @date 2025-09-19
 * @author user
 */

 //kvhredis 헤더파일
#include "kvhredis.h"


/**
 * @brief Redis Key 체인 활성화 설정
 * @param[in] h Redis 라이브러리 핸들	
 * @param[in] active 활성화 여부
 * @retval 0(성공), -1(파라미터 오류)
 */
int KVHREDIS_SetKeyActiveChain(KVHREDISHANDLE h, bool active)
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;
	if (!lib) {
		return -1;
	}
	lib->key.active_chain = active;
	return 0;
}


/**
 * @brief Redis Key 체인 해시 인덱스 사용 설정
 * @param[in] h Redis 라이브러리 핸들
 * @param[in] enable 여부
 * @retval 0(성공), -1(파라미터 오류)
	*/
int KVHREDIS_SetKeyEnableHashIndex(KVHREDISHANDLE h, bool enable)
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;
	if (!lib) {
		return -1;
	}
	lib->key.enable_hash_index = enable;
	return 0;
}

/**
 * @brief keytree.prefix값을 문자열로 반환
 * @param[in] prefix 타입
 * @retval 문자열 포인터(성공), NULL(실패)
*/
char *KVHREDIS_SetToStringPrefix(KVHREDISKeySUBDOMAIN prefix)
{
	switch (prefix) {
		case kKVHREDISKeySUBDOMAIN_DrivingStrategyServer:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_DrivingStrategyServer;
		case kKVHREDISKeySUBDOMAIN_FMS_Server:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_FMS_Server;
		case kKVHREDISKeySUBDOMAIN_V2N:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_V2N;
		case kKVHREDISKeySUBDOMAIN_V2X:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_V2X;
		case kKVHREDISKeySUBDOMAIN_DTG:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_DTG;
		case kKVHREDISKeySUBDOMAIN_IVDCT:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_IVDCT;
		case kKVHREDISKeySUBDOMAIN_DMS:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_DMS;
		case kKVHREDISKeySUBDOMAIN_HMI:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_HMI;
		case kKVHREDISKeySUBDOMAIN_ADS:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_ADS;
		case kKVHREDISKeySUBDOMAIN_GlobalSequence:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_GlobalSequence;
		case kKVHREDISKeySUBDOMAIN_CSM:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_CSM;
		case kKVHREDISKeySUBDOMAIN_VHMM:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_VHMM;
		case kKVHREDISKeySUBDOMAIN_ISAM:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_ISAM;
		case kKVHREDISKeySUBDOMAIN_FMCM:
			return KVH_REDIS_KEY_PREFIX_SUBDOMAIN_FMCM;
		default:
			return NULL;
	}
}


/**
 * @brief keytree.datauid.datamodeltype 값을 문자열로 반환
 * @param[in] datamodeltype 데이터 모델 타입
 * @retval 문자열 포인터(성공), NULL(실패)
*/
char *KVHREDIS_SetToStringDataModelType(KVHREDISKeyDataModelType datamodeltype)
{
	switch (datamodeltype) {
		case kKVHREDISKeyDataModelType_Protobuf:
			return "Protobuf";
		case kKVHREDISKeyDataModelType_ASN1KSR1600:
			return "ASN1_KSR1600";
		case kKVHREDISKeyDataModelType_JSON:
			return "JSON";
		case kKVHREDISKeyDataModelType_XML:
			return "XML";
		case kKVHREDISKeyDataModelType_String:
			return "String";
		case kKVHREDISKeyDataModelType_Binary:
			return "Binary";
		case kKVHREDISKeyDataModelType_Raw:
			return "Raw";
		default:
			return NULL;
	}
}


/**
 * @brief keytree.datauid.datamodeltype 값을 문자열로 반환
 * @param[in] datamodeltype 데이터 모델 타입
 * @retval 문자열 포인터(성공), NULL(실패)
*/
char *KVHREDIS_SetToStringDataModelSubType(KVHREDISKeyDataModelSubType datamodelsubtype)
{
	switch (datamodelsubtype) {
		case kKVHREDISKeyDataModelSubType_BSM:
			return "BSM";
		case kKVHREDISKeyDataModelSubType_PVD:
			return "PVD";
		case kKVHREDISKeyDataModelSubType_RSA:
			return "RSA";
		case kKVHREDISKeyDataModelSubType_TIM:
			return "TIM";
		case kKVHREDISKeyDataModelSubType_SPAT:
			return "SPAT";
		case kKVHREDISKeyDataModelSubType_MAP:
			return "MAP";
		case kKVHREDISKeyDataModelSubType_COVESA:
			return "COVESA";
		case kKVHREDISKeyDataModelSubType_SENSORIS:
			return "SENSORIS";
		case kKVHREDISKeyDataModelSubType_P_LOW_F:
			return "LOW_F";
		case kKVHREDISKeyDataModelSubType_P_HIGH_F:
			return "HIGH_F";
		case kKVHREDISKeyDataModelSubType_CITS_100ms:
			return "CITS_100ms";
		case kKVHREDISKeyDataModelSubType_CITS_INFO_100ms:
			return "CITS_INFO_100ms";
		case kKVHREDISKeyDataModelSubType_R_LOW_F:
			return "LOW_F";
		case kKVHREDISKeyDataModelSubType_R_HIGH_F:
			return "HIGH_F";
		case kKVHREDISKeyDataModelSubType_None:
			return "";
		default:
			return NULL;
	}
}


/**
 * @brief keytree.datauid.timestamp_utc 값을 문자열로 반환
 * @param[in] time_utc UTC 타임스탬프 (밀리초 단위)
 * @retval 문자열 포인터(성공), NULL(실패)
 */
char *KVHREDIS_SetToStringTimestampUTC(uint64_t time_utc)
{
	static char str_time_utc[KVH_REDIS_KEY_TIMESTAMP_UTC_MS_LENGTH + 1]; // 최대 20자리
	snprintf(str_time_utc, sizeof(str_time_utc), "%0*llu", KVH_REDIS_KEY_TIMESTAMP_UTC_MS_LENGTH, (unsigned long long)time_utc);
	return str_time_utc;
}


/**
 * @brief keytree.datauid.sequence 값을 문자열로 반환
 * @param[in] sequence 시퀀스 번호
 * @retval 문자열 포인터(성공), NULL(실패)
 */
char *KVHREDIS_SetToStringSequence(uint32_t sequence)
{
	static char str_sequence[KVH_REDIS_KEY_SEQUENCE_LENGTH + 1]; // 최대 10자리
	snprintf(str_sequence, sizeof(str_sequence), "%0*llu", KVH_REDIS_KEY_SEQUENCE_LENGTH, (unsigned long long)sequence);
	return str_sequence;
}


/**
 * @brief Redis Key timestamp_utc, sequence 업데이트
 * @param[in] key Redis Key 구조체 포인터
 * @retval 0(성공), -1(파라미터 오류)
 */
int KVHREDIS_ProcessUpdateKeySequence(KVHREDISKey *key)
{
	// Redis Key sequence 업데이트
	if (!key) {
		return -1;
	}

	memcpy(key->key_info.sequence, KVHREDIS_SetToStringSequence(key->sequence++), KVH_REDIS_KEY_SEQUENCE_LENGTH);

	uint32_t sequence_shift = key->key_length - KVH_REDIS_KEY_SEQUENCE_LENGTH;
	memcpy(key->key_str + sequence_shift, key->key_info.sequence, KVH_REDIS_KEY_SEQUENCE_LENGTH);
	return 0;
}


/**
 * @brief Redis Key timestamp_utc, sequence 업데이트
 * @param[in] key Redis Key 구조체 포인터
 * @retval 0(성공), -1(파라미터 오류)
 */
int KVHREDIS_ProcessUpdateKeyTimestamp(KVHREDISKey *key)
{
	// Redis Key timestamp_utc 업데이트
	if (!key) {
		return -1;
	}

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts); // 현재 시각
	uint64_t timestamp_utc = 0;
	timestamp_utc += (uint64_t)ts.tv_sec * 1000;
	timestamp_utc += (uint64_t)ts.tv_nsec / 1000000;

	memcpy(key->key_info.timestamp_utc, KVHREDIS_SetToStringTimestampUTC(timestamp_utc), KVH_REDIS_KEY_TIMESTAMP_UTC_MS_LENGTH);

	uint32_t timestamp_shift = key->key_length - KVH_REDIS_KEY_TIMESTAMP_UTC_MS_LENGTH - KVH_REDIS_KEY_SEQUENCE_LENGTH - 1;
	memcpy(key->key_str + timestamp_shift, key->key_info.timestamp_utc, KVH_REDIS_KEY_TIMESTAMP_UTC_MS_LENGTH);
	return 0;
}


/**
 * @brief Redis Key entityuid 설정
 * @param[in] key Redis Key 구조체 포인터
 * @param[in] entityuid entityuid 문자열
 * @param[in] entityuid_len entityuid 문자열 길이
 * @retval 0(성공), -1(파라미터 오류), -3(메모리 할당 오류)
 */
int KVHREDIS_SetKeyEntityUID(KVHREDISKey *key, const char *entityuid, size_t entityuid_len) {
	if (!key || !entityuid || entityuid_len == 0) {
		return -1;
	}

	strncpy(key->key_info.entityuid, entityuid, KVH_REDIS_KEY_ENTITYUID_LENGTH);
	key->key_length += entityuid_len;
	return 0;
}


/**
 * @brief Redis Key 데이터 모델 서브 타입 설정
 * @param[in] key Redis Key 구조체 포인터
 * @param[in] type 데이터 모델 서브 타입
 * @retval 0(성공), -1(파라미터 오류), -2(데이터 모델 서브 타입 변환 오류), -3(메모리 할당 오류)
 */
int KVHREDIS_SetKeyDataModelSubType(KVHREDISKey *key, KVHREDISKeyDataModelSubType subtype) {
	if (!key) {
		return -1;
	}
	char *subtype_str = KVHREDIS_SetToStringDataModelSubType(subtype);
	if (!subtype_str) {
		return -2;
	}

	strncpy(key->key_info.datamodelsubtype, subtype_str, KVH_REDIS_KEY_DATAMODELSUBTYPE_LENGTH);
	key->key_length += strlen(subtype_str);
	return 0;
}


/**
 * @brief Redis Key 데이터 모델 타입 설정
 * @param[in] key Redis Key 구조체 포인터
 * @param[in] type 데이터 모델 타입
 * @retval 0(성공), -1(파라미터 오류), -2(데이터 모델 타입 변환 오류), -3(메모리 할당 오류)
 */
int KVHREDIS_SetKeyDataModelType(KVHREDISKey *key, KVHREDISKeyDataModelType type) {
	if (!key) {
		return -1;
	}
	char *type_str = KVHREDIS_SetToStringDataModelType(type);
	if (!type_str) {
		return -2;
	}

	strncpy(key->key_info.datamodeltype, type_str, KVH_REDIS_KEY_DATAMODELTYPE_LENGTH);
	key->key_length += strlen(type_str);
	return 0;
}


/**
 * @brief Redis Key 접두어 설정
 * @param[in] key Redis Key 구조체 포인터
 * @param[in] target 접두어 설정 대상
 * @retval 0(성공), -1(파라미터 오류), -2(접두어 변환 오류), -3(메모리 할당 오류)
 */
int KVHREDIS_SetKeyPrefix(KVHREDISKey *key, KVHREDISKeySUBDOMAIN target) {
	if (!key) {
		return -1;
	}
	char *prefix_str = KVHREDIS_SetToStringPrefix(target);
	if (!prefix_str) {
		return -2;
	}

	strncpy(key->key_info.prefix, prefix_str, KVH_REDIS_KEY_PREFIX_MAX_LENGTH);
	key->key_length += strlen(prefix_str);
	
	return 0;
}


/**
 * @brief Redis Key 접두어 설정
 * @param[in] key Redis Key 구조체 포인터
 * @retval 0(성공), -1(파라미터 오류)
 */
int KVHREDIS_SetKeyMagicKey(KVHREDISKey *key) {
	if (!key) {
		return -1;
	}
	// Key 접두어 설정
	/// TODO::Redis에 저장된 Magic Key를 읽어서 사용하는 방식으로 수정 예정
	memcpy(key->key_info.magickey, KVH_REDIS_KEY_DEFAULT_MAGIC_KEY, KVH_REDIS_KEY_MAGIC_KEY_LENGTH);
	return 0;
}

/**
 * @brief Redis Key의 static field 설정
 * @param[in] key Redis Key 구조체 포인터
 * @param[in] target 접두어 설정 대상
 * @param[in] entityuid entityuid 문자열
 * @param[in] entityuid_len entityuid 문자열 길이
 * @param[in] type 데이터 모델 타입
 * @retval 0(성공), -1(파라미터 오류), -10(접두어 변환 오류), -20(데이터 모델 타입 변환 오류), -30(entityuid 설정 오류)
 */
int KVHREDIS_SetKeyStaticField(KVHREDISKey *key, KVHREDISKeySUBDOMAIN target,
																								  const char *entityuid, size_t entityuid_len, 
																								  KVHREDISKeyDataModelType type,
																									KVHREDISKeyDataModelSubType sub_type)
{
	if (!key) {
		return -1;
	}
	int ret;
	// Key Prifix 설정
	ret = KVHREDIS_SetKeyPrefix(key, target);
	if (ret < 0) {
		return ret;
	}
	// Key DataModelType 설정
	ret = KVHREDIS_SetKeyDataModelType(key, type);
	if (ret < 0) {
		return ret;
	}
	// Key DataModelType 설정
	ret = KVHREDIS_SetKeyDataModelSubType(key, sub_type);
	if (ret < 0) {
		return ret;
	}
	// Key EntityUID 설정
	ret = KVHREDIS_SetKeyEntityUID(key, entityuid, entityuid_len);
	if (ret < 0) {
		return ret;
	}

	if (strlen(key->key_info.datamodelsubtype) == 0) {
		snprintf(key->key_str, KVH_REDIS_KEY_MAX_LENGTH, "%s:%s:%s:%s:%020llu:%010u", key->key_info.magickey, 
																																									key->key_info.prefix,
																																									key->key_info.datamodeltype,
																																									key->key_info.entityuid,
																																									0ULL, // timestamp_utc
																																									0U);  // sequence
	} else {
		snprintf(key->key_str, KVH_REDIS_KEY_MAX_LENGTH, "%s:%s:%s:%s:%s:%020llu:%010u", key->key_info.magickey, 
																																									 key->key_info.prefix,
																																									 key->key_info.datamodeltype,
																																									 key->key_info.datamodelsubtype,
																																									 key->key_info.entityuid,
																																									 0ULL, // timestamp_utc
																																									 0U);  // sequence
	}
	
	key->key_length = strlen(key->key_str);
	return 0;
}


/**
 * @brief Redis Key 검색
 * @param[in] h Redis 라이브러리 핸들
 * @param[in] index 검색할 Key index (NULL이면 리턴
 * @param[in] index_len 검색할 Key index 길이 (index가 NULL이면 무시)
 * @retval NULL(검색 실패), Redis Key 구조체 포인터(검색 성공)
 */
KVHREDISKey *KVHREDIS_SelectKeyFromKeyChain(KVHREDISHANDLE h, KVHREDISKeyIndex *idx)
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;
	if (!lib) {
		return NULL;
	}
	if (idx == NULL || idx->index == NULL || idx->index_length == 0) {
		return NULL;
	}

	if (!lib->key.active_chain || lib->key.chain == NULL) {
		return NULL;
	}
	
	KVHREDISKey *ret = NULL;
	KVHREDISKeyNode *current = lib->key.chain;
	while (current->p) {
		current = current->p;
	}
	while (current != NULL) {
		if (current->index) {
			if (lib->key.enable_hash_index) {
				if (memcmp(current->index, idx->index, KVH_REDIS_KEY_INDEX_HASH_LENGTH) == 0) {
					lib->key.chain = current;
					ret = current->key;
					return ret;
				}
			} else {
				if (strncmp(current->key->key_str, (char*)idx->index, idx->index_length) == 0) {
					lib->key.chain = current;
					ret = current->key;
					return ret;
				}
			}
		} else {
			if (strncmp(current->key->key_str, (char*)idx->index, idx->index_length) == 0) {
				lib->key.chain = current;
				ret = current->key;
				return ret;
			}
		}
		current = current->c;
	}

	return NULL;
}

/**
 * @brief Redis Key의 SHA256 해시값 계산
 * @param[in] key Redis Key 구조체 포인터
 * @param[in] key_len key 길이
 * @retval NULL(파라미터 오류, 메모리 할당 오류), 해시값 포인터(성공)
 */
unsigned char *KVHREDIS_GetHashFromKey(KVHREDISKey *key, size_t key_len) {
	if (!key) {
		return NULL;
	}
	unsigned char *hash = (unsigned char *)malloc(SHA256_DIGEST_LENGTH);
	if (!hash) {
		return NULL;
	}
	SHA256((unsigned char*)key->key_str, key_len, hash);

	return hash;
}


/**
 * @brief Redis Key 추가
 * @param[in] h Redis 라이브러리 핸들  
 * @param[in] key 추가할 Redis Key 구조체 포인터
 * @retval 0(성공), -1(파라미터 오류), -2(메모리 할당 오류)
 */
int KVHREDIS_AddKeyToKeyChain(KVHREDISHANDLE h, KVHREDISKey *key)
{
	KVHREDIS_LIB *lib = (KVHREDIS_LIB *)h;
	if (!lib) {
		return -1;
	}
	if (key == NULL) {
		return -1;
	}
	// First Node인 경우
	KVHREDISKeyNode *new_node = NULL;
	if (lib->key.chain == NULL) {
		new_node = (KVHREDISKeyNode *)malloc(sizeof(KVHREDISKeyNode));
		if (!new_node) {
			return -1;
		}
		lib->key.chain = new_node;
		new_node->p = NULL;
		new_node->c = NULL;
		new_node->key = key;
	} else {
		// 새로운 노드 추가
		new_node = (KVHREDISKeyNode *)malloc(sizeof(KVHREDISKeyNode));
		if (!new_node) {
			return -1;
		}
		new_node->p = NULL;
		new_node->c = NULL;
		new_node->key = key;
		
		// 체인의 마지막 노드를 찾아서 연결
		KVHREDISKeyNode *current = lib->key.chain;
		while (current->c != NULL) {
			current = current->c;
		}
		current->c = new_node;
		new_node->p = current;
	}

	if (new_node->key) {
		if (lib->key.enable_hash_index) {
			// SHA256 해시 계산
			size_t sha_len = new_node->key->key_length - KVH_REDIS_KEY_TIMESTAMP_UTC_MS_LENGTH - KVH_REDIS_KEY_SEQUENCE_LENGTH - 2;
			unsigned char *hash = KVHREDIS_GetHashFromKey(new_node->key, sha_len);

			new_node->index = (unsigned char *)malloc(KVH_REDIS_KEY_INDEX_HASH_LENGTH);
			if (!new_node->index) {
				free(new_node);
				lib->key.chain = NULL;
				return -1;
			}
			memcpy(new_node->index, hash, KVH_REDIS_KEY_INDEX_HASH_LENGTH);
			free(hash);
		} else {
			new_node->index = NULL;
		}
	}

	return 0;
}



/**
 * @brief Redis Key 초기화
 * @param[in] key Redis Key 구조체 포인터
 * @retval 0(성공), -1(파라미터 오류)
 */
int KVHREDIS_InitializeKey(KVHREDISKey *key) 
{
	if (!key) {
		return -1;
	}
	memset(key, 0x00, sizeof(KVHREDISKey));
	
	int ret = KVHREDIS_SetKeyMagicKey(key);
	if (ret < 0) {
		return ret;
	}
	key->key_length += KVH_REDIS_KEY_MAGIC_KEY_LENGTH + KVH_REDIS_KEY_TIMESTAMP_UTC_MS_LENGTH + KVH_REDIS_KEY_SEQUENCE_LENGTH;

	return 0;
}


