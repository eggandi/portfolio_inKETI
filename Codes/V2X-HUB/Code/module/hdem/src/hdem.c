/**
 * @file
 * @brief HDEM(HMI Data Exchange Module) 기능 구현
 * @date 2024-10-11
 * @author user
 */


// 모듈 헤더파일
#include "hdem.h"


HDEM_MIB g_hdem_mib; ///< 모듈 통합관리정보


/**
 * @brief 모듈 프로그램 사용법을 출력한다.
 * @param[in] mod_filename 모듈 프로그램 실행파일명
 */
static void HDEM_Usage(const char *mod_filename)
{
  printf("\n\n");
  printf(" Description: V2X Hub HDEM(HMI Data Exchange Module)\n");
  printf(" Version: %s\n", _VERSION_);
  printf("\n");

  printf(" Usage: %s <HMI ip> [OPTIONS]\n\n", mod_filename);
  printf("             HMI ip: HMI ip address\n");
  printf("\n");

  printf(" OPTIONS\n");
  printf("  --dgm_suds <file path>        Set DGM I/F UDS server file path. If not specified, set to \"%s\"\n", KVH_UDS_S_FILE_IVN_HDEM);
  printf("  --dgm_cuds <file path>        Set DGM I/F UDS client file path. If not specified, set to \"%s\"\n", KVH_UDS_C_FILE_IVN_HDEM);
  printf("  --status_port <port number>   Set My status data rx UDP port. If not specified, set to %u\n", KVH_IVN_HMI_IF_UDP_PORT_STATUS_DATA_TRX);
  printf("  --cits_port <port number>     Set HMI's C-ITS service data rx UDP port. If not specified, set to %u\n", KVH_IVN_ADS_IF_UDP_PORT_CITS_MSG_TX);
  printf("  --logfile              Set log message file. If not specified, set to stderr\n");
  printf("  --dbg <level>          Set log level. If not specified, set to %d\n", kKVHLOGLevel_InOut);
  printf("                             0:none, 1:err, 2:init, 3:in/out, 4:event, 5:dump, 6:all\n");
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
    HDEM_Usage(argv[0]);
    return 0;
  }

  HDEM_MIB *mib = &g_hdem_mib;
  memset(mib, 0, sizeof(HDEM_MIB));

  /*
   * 입력 파라미터를 파싱하여 저장한다.
   */
  if (HDEM_ParsingInputParameters(argc, argv, mib) < 0) {
    return -1;
  }

  /*
   * 로그기능을 초기화한다. (입력파라미터 파싱/저장(=로그레벨 설정) 후 호출되어야 한다)
   */
  INIT_LOG(mib->log_lv, mib->logfile);
  LOG(kKVHLOGLevel_Init, "\n");
  LOG(kKVHLOGLevel_Init, "========= Running HDEM(HMI Data Exchange Module)...\n");
  LOG(kKVHLOGLevel_Init, "Common - dbg: %d\n", mib->log_lv);
  LOG(kKVHLOGLevel_Init, "DGM I/F - uds server(DGM): %s, uds client(me): %s\n",
      mib->dgm_if.uds.s_uds_file_path, mib->dgm_if.uds.c_uds_file_path);
  LOG(kKVHLOGLevel_Init, "HMI I/F\n");
  LOG(kKVHLOGLevel_Init, "  - HMI IP address: %s\n", mib->hmi_if.hmi_ip);
  LOG(kKVHLOGLevel_Init, "  - Status data rx udp port(in V2X Hub): %u\n", mib->hmi_if.status_data_trx.udp.server_port);
  LOG(kKVHLOGLevel_Init, "  - C-ITS service data rx udp port(in HMI): %u\n", mib->hmi_if.cits_msg_tx.udp.server_port);
  LOG(kKVHLOGLevel_Init, "\n");

  /*
   * DGM I/F를 초기화한다.
   */
  if (HDEM_InitDGMIF(mib) < 0) {
    return -1;
  }

  /*
   * HMI I/F를 초기화한다.
   */
  if (HDEM_InitHMIIF(mib) < 0) {
    return -1;
  }

  while (1) {
    sleep(100);
  }

  return 0;
}
