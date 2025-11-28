/**
 * @file
 * @brief DMSDCM 실행 파라미터 처리 기능
 * @date 2024-12-13
 * @author user
 */


// 모듈 헤더파일
#include "dmsdcm.h"


/**
 * @brief 입력 파라미터들에 대한 기본값을 설정한다.
 * @parma[in] mib 모듈 통합관리정보
 */
static void DMSDCM_SetDefaultInputParameters(DMSDCM_MIB *mib)
{
	strncpy(mib->redis_if.server_addr_str, "192.168.137.100", INET_ADDRSTRLEN);
	mib->redis_if.server_port = 6379; // Redis 기본 포트번호
  strncpy(mib->dms_if.dev_name, DMSDCM_DEFAULT_IF_DEV_NAME, sizeof(mib->dms_if.dev_name)-1);
  mib->dms_if.baudrate = DMSDCM_DEFAULT_IF_BAUDRATE;
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
static int DMSDCM_ProcessParsedOption(DMSDCM_MIB *mib, int option)
{
  int ret = 0;
  switch (option) {
    case 0: { // "dbg"
      mib->log_lv = (KVHLOGLevel)strtoul(optarg, NULL, 10);
      break;
    }
		case 1: { // "redis_host_address"
      strncpy(mib->redis_if.server_addr_str, optarg, INET_ADDRSTRLEN);
      break;
    }
    case 2: { // "redis_host_port"
      mib->redis_if.server_port = strtoul(optarg, NULL, 10);
      break;
    }
    case 3: { // "dms_rs232_dev"
      strncpy(mib->dms_if.dev_name, optarg, sizeof(mib->dms_if.dev_name)-1);
      break;
    }
    case 4: { // "dms_rs232_baudrate"
      mib->dms_if.baudrate = strtol(optarg, NULL, 10);
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
int DMSDCM_ParsingInputParameters(int argc, char *argv[], DMSDCM_MIB *mib)
{
  int c, option_idx = 0;
  struct option options[] = {
    {"dbg", required_argument, 0, 0/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"redis_server_addr", required_argument, 0, 1/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"redis_server_port", required_argument, 0, 2/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"dms_rs232_dev", required_argument, 0, 3/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"dms_rs232_baudrate", required_argument, 0, 4/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"logfile", required_argument, 0, 5/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {0, 0, 0, 0} // 옵션 배열은 {0,0,0,0} 센티넬에 의해 만료된다.
  };

  /*
   * 기본설정정보를 설정한다.
   */
  if (memcmp(argv[1], "start", 5) != 0) {
    printf("Invalid operation - %s\n", argv[1]);
    return -1;
  }
  DMSDCM_SetDefaultInputParameters(mib);

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
    int ret = DMSDCM_ProcessParsedOption(mib, c);
    if (ret < 0) {
      return ret;
    }
  }

  return 0;
}
