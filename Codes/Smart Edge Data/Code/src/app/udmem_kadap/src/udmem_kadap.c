/**
 * @file
 * @brief KADaP 클라우드와의 UDMEM(Uplink/Downlink Message Exchange Module) 기능 구현
 * @date 2024-12-01
 * @author user
 */


// 모듈 헤더파일
#include "udmem_kadap.h"


UDMEM_KADAP_MIB g_udmem_kadap_mib; ///< 모듈 통합관리정보


/**
 * @brief 모듈 프로그램 사용법을 출력한다.
 * @param[in] mod_filename 모듈 프로그램 실행파일명
 */
static void UDMEM_KADAP_Usage(const char *mod_filename)
{
  printf("\n\n");
  printf(" Description: V2X Hub UDMEM(Uplink/Downlink Message Exchange Module) with KADaP cloud\n");
  printf(" Version: %s\n", _VERSION_);
  printf("\n");

  printf(" Usage: %s start [OPTIONS]\n\n", mod_filename);
  printf("\n");

  printf(" OPTIONS\n");
  printf("  --cloud_ip <ip addr>   Set KADaP cloud server IP address. If not specified, set to \"%s\"\n", UDMEM_KADAP_CLOUD_SERVER_IP_ADDR);
  printf("  --cloud_port <port>    Set KADaP cloud server port number. If not specified, set to \"%d\"\n", UDMEM_KADAP_CLOUD_SERVER_PORT);
  printf("  --interval <interval>  Set data upload interval in usec. If not specified, set to \"%d\"\n", UDMEM_KADAP_STATUS_DATA_UPLOAD_INTERVAL);
  printf("                             If you set this value as under 100msec, most data will be lost\n");
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
    UDMEM_KADAP_Usage(argv[0]);
    return 0;
  }

  UDMEM_KADAP_MIB *mib = &g_udmem_kadap_mib;
  memset(mib, 0, sizeof(UDMEM_KADAP_MIB));

  /*
   * 입력 파라미터를 파싱하여 저장한다.
   */
  if (UDMEM_KADAP_ParsingInputParameters(argc, argv, mib) < 0) {
    return -1;
  }

  /*
   * 로그기능을 초기화한다. (입력파라미터 파싱/저장(=로그레벨 설정) 후 호출되어야 한다)
   */
  INIT_LOG(mib->log_lv, mib->logfile);
  LOG(kKVHLOGLevel_Init, "\n");
  LOG(kKVHLOGLevel_Init, "========= Running UDMEM(Uplink/Dowlink Message Exchange Module) with KADaP cloud...\n");
  LOG(kKVHLOGLevel_Init, "Common - dbg: %d\n", mib->log_lv);
  LOG(kKVHLOGLevel_Init, "Cloud I/F - server address: %s(%u), upload interval: %ldusec\n",
      mib->cloud_if.status_data_upload.server_ip, mib->cloud_if.status_data_upload.server_port, mib->cloud_if.status_data_upload.interval);
  LOG(kKVHLOGLevel_Init, "\n");

  /*
   * RSDPR I/F를 초기화한다.
   */
  if (UDMEM_KADAP_OpenRSDPR(mib) < 0) {
    return -1;
  }

  /*
   * KADaP I/F를 초기화한다.
   */
  if (UDMEM_KADAP_InitCloudIF(mib) < 0) {
    return -1;
  }

  while(1) {
    sleep(100);
  }

  return 0;
}
