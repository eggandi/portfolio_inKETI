/**
 * @file
 * @brief DMSDCM (DMS Date Collection Module) 기능 구현
 * @date 2024-12-13
 * @author user
 */


// 모듈 헤더파일
#include "dmsdcm.h"


DMSDCM_MIB g_dmsdcm_mib; ///< 모듈 통합관리정보


/**
 * @brief 모듈 프로그램 사용법을 출력한다.
 * @param[in] mod_filename 모듈 프로그램 실행파일명
 */
static void DMSDCM_Usage(const char *mod_filename)
{
  printf("\n\n");
  printf(" Description: V2X Hub DMSDCM(DMS Data Collection Module)\n");
  printf(" Version: %s\n", _VERSION_);
  printf("\n");

  printf(" Usage: %s start <[OPTIONS]\n", mod_filename);
  printf("\n");

  printf(" OPTIONS\n");
	printf("  --redis_host_address		Set Redis server address. If not specified, set to \"192.168.137.100\"\n");
  printf("  --redis_host_port			Set Redis server port. If not specified, set to %d\n", 6379);
  printf("  --dev <file path>           Set DMS I/F serial port device name. If not specified, set to \"%s\"\n", DMSDCM_DEFAULT_IF_DEV_NAME);
  printf("  --baudrate <file path>      Set DMS I/F serial port baudrate. If not specified, set to \"%d\"\n", DMSDCM_DEFAULT_IF_BAUDRATE);
  printf("  --logfile                   Set log message file. If not specified, set to stderr\n");
  printf("  --dbg <level>               Set log level. If not specified, set to %d\n", kKVHLOGLevel_InOut);
  printf("                                  0:none, 1:err, 2:init, 3:in/out, 4:event, 5:dump, 6:all\n");
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
    DMSDCM_Usage(argv[0]);
    return 0;
  }

  DMSDCM_MIB *mib = &g_dmsdcm_mib;
  memset(mib, 0, sizeof(DMSDCM_MIB));

  /*
   * 입력 파라미터를 파싱하여 저장한다.
   */
  if (DMSDCM_ParsingInputParameters(argc, argv, mib) < 0) {
    return -1;
  }

  /*
   * 로그기능을 초기화한다. (입력파라미터 파싱/저장(=로그레벨 설정) 후 호출되어야 한다)
   */
  INIT_LOG(mib->log_lv, mib->logfile);
  LOG(kKVHLOGLevel_Init, "\n");
  LOG(kKVHLOGLevel_Init, "========= Running DMSDCM(DMS Data Collection Module)...\n");
  LOG(kKVHLOGLevel_Init, "Common - dbg: %d\n", mib->log_lv);
	LOG(kKVHLOGLevel_Init, "	Redis I/F\n");
	LOG(kKVHLOGLevel_Init, "  - server addr: %s\n", mib->redis_if.server_addr_str);
	LOG(kKVHLOGLevel_Init, "  - server port: %d\n", mib->redis_if.server_port);
  LOG(kKVHLOGLevel_Init, "DMS I/F\n");
  LOG(kKVHLOGLevel_Init, "  - dev name: %s\n", mib->dms_if.dev_name);
  LOG(kKVHLOGLevel_Init, "  - baudrate: %d\n", mib->dms_if.baudrate);
  LOG(kKVHLOGLevel_Init, "\n");

	/*
   * IVDCT와의 I/F 기능을 초기화한다.
   */
  if (DMSDCM_InitRedisIF(mib) < 0) {
    return -1;
  }

  /*
   * DMS와의 I/F 기능을 초기화한다.
   */
  if (DMSDCM_InitDMSIF(mib) < 0) {
    return -1;
  }

  while (1) {
    sleep(100);
  }

  return 0;
}

