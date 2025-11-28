/**
 * @file
 * @brief Driving Strategy Client Manager(DSCM) 기능 구현
 * @date 2025-08-11
 * @author donggyu
 * 
 * 본 파일에 정의된 Driving Strategy Client 기능은 차량의 새만금 자율주행 프로토콜에 따라 주행전략 워크플로 구현한다.
 * Driving Stategy는 다음 기능을 포함한다. 
 * Planning: 주행전략의 워크플로를 구성하고 Job과 Do(Action)를 정의한다.
 * DrivingStrategyManager: 주행전략 워크플로를 초기화/관리하고, 외부 시그널을 워크플로 신호로 입력
 * Engine: 주행전략 워크플로를 수행하는 역할로 Job과 Do를 관리하고 상태를 모니터링한다.
 */

// 모듈 헤더파일
#include "dscm.h"


DSCM_MIB g_dscm_mib; ///< 모듈 통합관리정보

/** 
 * @brief CRC16-CCITT-FALSE 계산
 * @param[in] buf 데이터 버퍼
 * @param[in] off 버퍼 오프셋
 * @param[in] len 버퍼 길이
 * @retval uint16_t 계산된 CRC16-CCITT-FALSE 값
*/
uint16_t DSCM_GetCRC16(const uint8_t *buf, int off, int len) {
		uint16_t crc = DSCMDC_PKT_CRC16_CCITTFALSE_INIT;
		for (int i = 0; i < len; i++) {
				crc ^= (buf[off + i] & 0xFF) << 8;
				for (int b = 0; b < 8; b++) {
						crc = ((crc & 0x8000) != 0) ? ((crc << 1) ^ DSCMDC_PKT_CRC16_CCITTFALSE_POLY) : (crc << 1);
						crc &= 0xFFFF;
				}
		}
		return crc ^ DSCMDC_PKT_CRC16_CCITTFALSE_XOROUT;
}


/**
 * @brief Base64 인코딩 JSON과 같은 텍스트 기반 데이터 
 * 				모델에 바이너리 타입의 데이터를 값으로 사용하기 위해 ASCII 문자열로 변환을 
 * @param[in] input 인코딩할 문자열 (끝이 널문자('\0')여야 함)
 * @param[out] char* 인코딩된 문자열
 */
char* DSCM_ProcessBase64Encoder(const unsigned char *input) {
    char *encoded = (char *)malloc(strlen((const char *)input) * 2);
    if (!encoded) {
        return NULL;
    }
    int val = 0;
    int valb = -6;
    for (const unsigned char *p = (const unsigned char *)input; *p; p++) {
        unsigned char c = *p;
				val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded[strlen(encoded)] = DSCM_BASE64_CHARS[(val >> valb) & 0x3F];
            valb -= 6;
				}
        
    }
    if (valb > -6) {
        encoded[strlen(encoded)] = DSCM_BASE64_CHARS[((val << 8) >> (valb + 8)) & 0x3F];
    }
    while (strlen(encoded) % 4) {
        encoded[strlen(encoded)] = '=';
    }
    return encoded;
}


/**
 * @brief 모듈 프로그램 사용법을 출력한다.
 * @param[in] mod_filename 모듈 프로그램 실행파일명
 */
static void DSCM_Usage(const char *mod_filename)
{
  printf("\n\n");
  printf(" Description: V2X Hub DSCM()\n");
  printf(" Version: %s\n", _VERSION_);
  printf("\n");

  printf(" Usage: %s start <[OPTIONS]\n", mod_filename);
  printf("\n");

  printf(" OPTIONS\n");
  printf("  --dc_eth_if <if name>               Set DC Ethernet I/F Name using the external vechicle. If not specified, set to \"%s\"\n", DSCMDC_TCP_DEFAULT_IF_DEV_NAME);
	printf("  --dc_server_addr <addr>             Set DC I/F TCP server string type IP address. If not specified, set to \"%s\"\n", DSCMDC_TCP_DEFAULT_IF_IPV4_SERVER_ADDR_STR);
	printf("  --dc_client_addr <addr>             Set DC I/F TCP client string type IP address. If not specified, set to \"%s\"\n", DSCMDC_TCP_DEFAULT_IF_IPV4_CLIENT_ADDR_STR);
  printf("  --dc_server_port <port number>      Set DC driving strategy data TCP server port. If not specified, set to %u\n", KVH_V2X_DSC_IF_TCP_PORT_DRIVINGSTRATEGY_MSG_SERVER);
  printf("  --dc_client_port <port number>      Set DC driving strategy data TCP client port. If not specified, set to %u\n", KVH_V2X_DSC_IF_TCP_PORT_DRIVINGSTRATEGY_MSG_CLIENT);
  printf("  --auth_info <string>                Set DC Login - Authentication Info(String, 16). If not specified application no working.\n");
	printf("  --hmi_eth_if <if name>              Set interval vehicle automotive ethernet I/F Name. If not specified, set to \"%s\"\n", DSCMHMI_INVEHICLE_AETH_DEFAULT_IF_DEV_NAME);
	printf("  --hmi_server_addr <addr>            Set HMI I/F TCP dest string type IP address. If not specified, set to \"%s\"\n", DSCMHMI_TCP_DEFAULT_IF_IPV4_SERVER_ADDR_STR);
	printf("  --hmi_server_port <port number>     Set HMI signal rx TCP port. If not specified, set to %u\n", KVH_IVN_HMI_IF_TCP_PORT_DRIVINGSTRATEGY_MSG_SERVER);
	printf("  --redis_server_addr <addr>            Set Redis Server string type IP address. If not specified, set to \"%s\"\n", DSCMREDIS_TCP_DEFAULT_IF_IPV4_SERVER_ADDR_STR);
	printf("  --redis_server_port <port number>     Set Redis signal rx TCP port. If not specified, set to %u\n", 6379);
	printf("  --logfile                           Set log message file. If not specified, set to stderr\n");
  printf("  --dbg <level>                       Set log level. If not specified, set to %d (0:none, 1:err, 2:init, 3:in/out, 4:event, 5:dump, 6:all)\n", kKVHLOGLevel_InOut);
  printf("\n\n");
}

/**
 * @brief 모듈 메인 함수
 * @param[in] argc 모듈 실행 시 입력되는 명령줄 내 파라미터들의 개수 (유틸리티 실행파일명 포함)
 * @param[in] argv 모듈 실행 시 입력되는 명령줄 내 파라미터들의 문자열 집합 (유틸리티 실행파일명 포함)
 * @retval 0: 성공
 * @retval -1: 실패
 */
int main(int argc, char *argv[])
{
  /*
   * 아무 파라미터 없이 실행하면 사용법을 출력한다.
   */
  if (argc < 2) {
    DSCM_Usage(argv[0]);
    return 0;
  }

  DSCM_MIB *mib = &g_dscm_mib;
  memset(mib, 0, sizeof(DSCM_MIB));

  /*
   * 입력 파라미터를 파싱하여 저장한다.
   */
  if (DSCM_ParsingInputParameters(argc, argv, mib) < 0) {
    return -1;
  }

  /*
   * 로그기능을 초기화한다. (입력파라미터 파싱/저장(=로그레벨 설정) 후 호출되어야 한다)
   */
  INIT_LOG(mib->log_lv, mib->logfile);
  LOG(kKVHLOGLevel_Init, "\n");
  LOG(kKVHLOGLevel_Init, "========= Running DSCM(DS Data Collection Module)...\n");
  LOG(kKVHLOGLevel_Init, "	Common - dbg: %d\n", mib->log_lv);
	LOG(kKVHLOGLevel_Init, "	Redis I/F\n");
	LOG(kKVHLOGLevel_Init, "   - server addr: %s\n", mib->redis_if.server_addr_str);
	LOG(kKVHLOGLevel_Init, "   - server port: %d\n", mib->redis_if.server_port);
  LOG(kKVHLOGLevel_Init, "	DC (DataCenter) I/F\n");
	LOG(kKVHLOGLevel_Init, "	 - server: %s:%d, client: %s:%d\n", mib->dc_if.dc_server_addr, mib->dc_if.dc_server_port, mib->dc_if.dc_client_addr, mib->dc_if.dc_client_port);
  LOG(kKVHLOGLevel_Init, "	DC Login - Authentication:%s\n", mib->dc_if.dc_login_info.auth_info);
  LOG(kKVHLOGLevel_Init, "	HMI I/F\n");
	LOG(kKVHLOGLevel_Init, "	 - server: %s:%d\n", mib->hmi_if.hmi_server_addr, mib->hmi_if.hmi_server_port);
	LOG(kKVHLOGLevel_Init, "\n");

 /*
   * Protobuf 기능을 초기화한다.
   */
  if (DSCM_InitPB() < 0) {
    return -1;
  }
	
 /*
   * Protobuf 기능을 초기화한다.
   */
  if (DSCM_InitRedisIF(mib) < 0) {
    return -1;
  }

 /*
   * DS와의 I/F 기능을 초기화한다.
   */
  if (DSCM_InitDCIF(mib) < 0) {
    return -1;
  }


  while (1) {
    sleep(100);
  }
	
 	DSCM_ClosePB(mib);
	
  return 0;
}

