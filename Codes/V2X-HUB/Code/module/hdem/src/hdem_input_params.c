/**
 * @file
 * @brief HDEM 실행 파라미터 처리 기능
 * @date 2024-10-11
 * @author user
 */


// 모듈 헤더파일
#include "hdem.h"


/**
 * @brief 입력 파라미터들에 대한 기본값을 설정한다.
 * @parma[in] mib 모듈 통합관리정보
 */
static void HDEM_SetDefaultInputParameters(HDEM_MIB *mib)
{
  strncpy(mib->dgm_if.uds.s_uds_file_path, KVH_UDS_S_FILE_IVN_HDEM, KVH_FILE_PATH_MAX_LEN);
  strncpy(mib->dgm_if.uds.c_uds_file_path, KVH_UDS_C_FILE_IVN_HDEM, KVH_FILE_PATH_MAX_LEN);
  mib->hmi_if.status_data_trx.udp.server_port = KVH_IVN_HMI_IF_UDP_PORT_STATUS_DATA_TRX;
  mib->hmi_if.cits_msg_tx.udp.server_port = KVH_IVN_ADS_IF_UDP_PORT_CITS_MSG_TX;
  mib->logfile = NULL;
  mib->log_lv = kKVHLOGLevel_InOut;
}


/**
 * @brief 옵션값에 따라 각 옵션을 처리한다.
 * @parma[in] mib 모듈 통합관리정보
 * @param[in] option 옵션값 (struct option 의 4번째 멤버변수)
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int HDEM_ProcessParsedOption(HDEM_MIB *mib, int option)
{
  int ret = 0;
  switch (option) {
    case 0: { // "dbg"
      mib->log_lv = (KVHLOGLevel)strtoul(optarg, NULL, 10);
      break;
    }
    case 1: { // "dgm_suds"
      strncpy(mib->dgm_if.uds.s_uds_file_path, optarg, KVH_FILE_PATH_MAX_LEN);
      break;
    }
    case 2: { // "dgm_cuds"
      strncpy(mib->dgm_if.uds.c_uds_file_path, optarg, KVH_FILE_PATH_MAX_LEN);
      break;
    }
    case 3: { // "status_port"
      mib->hmi_if.status_data_trx.udp.server_port = (uint16_t)strtoul(optarg, NULL, 10);
      break;
    }
    case 4: { // "cits_port"
      mib->hmi_if.cits_msg_tx.udp.server_port = (uint16_t)strtoul(optarg, NULL, 10);
      break;
    }
    case 5: { // "logfile"
      mib->logfile = strdup(optarg);
      break;
    }
    default: {
      printf("Invalid option\n");
      ret = -1;
    }
  }
  return ret;
}


/**
 * @brief 모듈 실행 시 함께 입력된 파라미터들을 파싱하여 설정정보에 저장한다.
 * @param[in] argc 모듈 실행 시 입력되는 명령줄 내 파라미터들의 개수 (모듈 실행파일명 포함)
 * @param[in] argv 모듈 실행 시 입력되는 명령줄 내 파라미터들의 문자열 집합 (모듈 실행파일명 포함)
 * @parma[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int HDEM_ParsingInputParameters(int argc, char *argv[], HDEM_MIB *mib)
{
  int c, option_idx = 0;
  struct option options[] = {
    {"dbg", required_argument, 0, 0/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"dgm_suds", required_argument, 0, 1/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"dgm_cuds", required_argument, 0, 2/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"status_port", required_argument, 0, 3/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"cits_port", required_argument, 0, 4/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"logfile", required_argument, 0, 5/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {0, 0, 0, 0} // 옵션 배열은 {0,0,0,0} 센티넬에 의해 만료된다.
  };

  /*
   * 기본설정정보를 설정한다.
   */
  strncpy(mib->hmi_if.hmi_ip, argv[1], sizeof(mib->hmi_if.hmi_ip)-1);
  HDEM_SetDefaultInputParameters(mib);

  /*
   * 입력 파라미터를 파싱하여 저장한다.
   */
  while (1) {

    // 옵션 파싱
    c = getopt_long(argc, argv, "", options, &option_idx);
    if (c == -1) {  // 모든 파라미터 파싱 완료
      break;
    }

    // 파싱된 옵션 처리 -> 저장
    int ret = HDEM_ProcessParsedOption(mib, c);
    if (ret < 0) {
      return ret;
    }
  }

  return 0;
}
