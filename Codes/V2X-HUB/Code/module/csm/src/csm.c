/**
 * @file
 * @brief CSM(C-ITS Service Module) 기능 구현
 * @date 2024-11-19
 * @author user
 */


// 모듈 헤더파일
#include "csm.h"


CSM_MIB g_csm_mib; ///< 모듈 통합관리정보


/**
 * @brief 모듈 프로그램 사용법을 출력한다.
 * @param[in] mod_filename 모듈 프로그램 실행파일명
 */
static void CSM_Usage(const char *mod_filename)
{
  printf("\n\n");
  printf(" Description: V2X Hub CSM(C-ITS Service Module)\n");
  printf(" Version: %s\n", _VERSION_);
  printf("\n");

  printf(" Usage: %s start [OPTIONS]\n\n", mod_filename);
  printf("\n");

  printf(" OPTIONS\n");
	printf("  --redis_host_address		Set Redis server address. If not specified, set to \"192.168.137.100\"\n");
  printf("  --redis_host_port			Set Redis server port. If not specified, set to %d\n", 6379);
  printf("  --logfile                  Set log message file. If not specified, set to stderr\n");
  printf("  --dbg <level>              Set log level. If not specified, set to %d\n", kKVHLOGLevel_InOut);
  printf("                                 0:none, 1:err, 2:init, 3:in/out, 4:event, 5:dump, 6:all\n");
	
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
    CSM_Usage(argv[0]);
    return 0;
  }

  CSM_MIB *mib = &g_csm_mib;
  memset(mib, 0, sizeof(CSM_MIB));

  /*
   * 입력 파라미터를 파싱하여 저장한다.
   */
  if (CSM_ParsingInputParameters(argc, argv, mib) < 0) {
    return -1;
  }

  /*
   * 로그기능을 초기화한다. (입력파라미터 파싱/저장(=로그레벨 설정) 후 호출되어야 한다)
   */
  INIT_LOG(mib->log_lv, mib->logfile);
  LOG(kKVHLOGLevel_Init, "\n");
  LOG(kKVHLOGLevel_Init, "========= Running CSM(C-ITS Service Module)...\n");
  LOG(kKVHLOGLevel_Init, "Common - dbg: %d\n", mib->log_lv);
  
  LOG(kKVHLOGLevel_Init, "\n");

  /*
   * C-ITS 라이브러리를 초기화한다.
   */
  mib->cits.h = KVHCITS_Init();
  if (mib->cits.h == NULL) {
    ERR("Fail to KVHCITS_Init()\n");
    return -1;
  }
#if 0
  /*
   * RSDPR을 연다.
   */
  mib->rsdpr_h = KVHRSDPR_Init();
  if (mib->rsdpr_h == NULL) {
    ERR("Fail to KVHRSDPR_Init()\n");
    return -1;
  }

  /*
   * DGM I/F를 초기화한다.
   */
  if (CSM_InitDGMIF(mib) < 0) {
    return -1;
  }

  /*
   * SMEM I/F를 초기화한다.
   */
  if (CSM_InitSMEMIF(mib) < 0) {
    return -1;
  }
#endif
	/*
	 * Protobuf 라이브러리를 초기화한다.
	 */
	if (CSMRedis_InitProtobufLibrary() < 0) {
		return -1;
	}

  /*
   * Redis I/F를 초기화한다.
   */
  if (CSM_InitRedisIF(mib) < 0) {
    return -1;
  }

  while (1) {
    sleep(100);
  }	
	
	CSMRedis_ShutdownProtobufLibrary();
  return 0;
}
