/**
 * @file
 * @brief SMEM(Sidelink Message Exchange Module) 기능 구현
 * @date 2024-10-13
 * @author user
 */


// 모듈 헤더파일
#include "smem.h"


SMEM_MIB g_smem_mib; ///< 모듈 통합관리정보


/**
 * @brief 모듈 프로그램 사용법을 출력한다.
 * @param[in] mod_filename 모듈 프로그램 실행파일명
 */
static void SMEM_Usage(const char *mod_filename)
{
  printf("\n\n");
  printf(" Description: V2X Hub SMEM(Sidelink Message Exchange Module)\n");
  printf(" Version: %s\n", _VERSION_);
  printf("\n");

  printf(" Usage: %s start [OPTIONS]\n", mod_filename);
  printf("\n");

  printf(" OPTIONS\n");
	printf("  --redis_server_addr <addr>            Set Redis Server string type IP address. If not specified, set to \"%s\"\n", "172.17.251.1");
	printf("  --redis_server_port <port number>     Set Redis signal rx TCP port. If not specified, set to %u\n", 6379);
  printf("  --logfile                  Set log message file. If not specified, set to stderr\n");
  printf("  --dbg <level>              Set log level. If not specified, set to %d\n", kKVHLOGLevel_InOut);
  printf("                                 0:none, 1:err, 2:init, 3:in/out, 4:event, 5:dump, 6:all\n");
  printf("  --sdbg <level>             Set Sidelink library log level. If not specified, set to %d\n", SMEM_DEFAULT_SLIB_LOG_LEVEL);
  printf("                                 0:none, 1:err, 2:init, 3:event, 4:message hexdump\n");
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
    SMEM_Usage(argv[0]);
    return 0;
  }

  SMEM_MIB *mib = &g_smem_mib;
  memset(mib, 0, sizeof(SMEM_MIB));

  /*
   * 입력 파라미터를 파싱하여 저장한다.
   */
  if (SMEM_ParsingInputParameters(argc, argv, mib) < 0) {
    return -1;
  }

  /*
   * 로그기능을 초기화한다. (입력파라미터 파싱/저장(=로그레벨 설정) 후 호출되어야 한다)
   */
  INIT_LOG(mib->log_lv, mib->logfile);
  LOG(kKVHLOGLevel_Init, "\n");
  LOG(kKVHLOGLevel_Init, "========= Running SMEM(Sidelink Message Exchange Module)...\n");
  LOG(kKVHLOGLevel_Init, "Common - dbg: %d, sdbg: %d\n", mib->log_lv, mib->slib_log_lv);
  LOG(kKVHLOGLevel_Init, "Sidelink - type: %d(0:DSRC, 1:LTEV2X)\n", mib->sidelink_type);
	LOG(kKVHLOGLevel_Init, "1602 Dot2 - Enable: %d\n", mib->v2x.config.dot2.enable);
	LOG(kKVHLOGLevel_Init, "Redis I/F\n");
	LOG(kKVHLOGLevel_Init, "  - server addr: %s\n", mib->redis_if.server_addr_str);
	LOG(kKVHLOGLevel_Init, "  - server port: %d\n", mib->redis_if.server_port);
	LOG(kKVHLOGLevel_Init, "\n");

	#if 0
  /*
   * DGM과의 I/F 기능을 초기화한다.
   */
  if (SMEM_InitDGMIF(mib) < 0) {
    return -1;
  }

  /*
   * CSM과의 I/F 기능을 초기화한다.
   */
  if (SMEM_InitCSMIF(mib) < 0) {
    return -1;
  }
 	#endif

	/*
	 * Redis I/F 기능을 초기화한다.
	 */
	if (SMEM_InitRedisIF(mib) < 0) {
		return -1;
	}

  /*
   * V2X sidelink I/F 기능을 초기화한다.
   */
	if (g_smem_mib.sidelink_type == 0) {
		if (SMEM_InitSidelinkIF(mib) < 0) {
			return -1;
		}
	} else {
		if (LTEV2X_InitialV2XLibrary() < 0) {
			return -1;
		}
	}

  while (1) {
    sleep(100);
  }

  return 0;
}
