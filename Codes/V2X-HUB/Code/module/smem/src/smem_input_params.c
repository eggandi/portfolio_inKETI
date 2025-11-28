/**
 * @file
 * @brief SMEM 실행 파라미터 처리 기능
 * @date 2024-10-13
 * @author user
 */


// 모듈 헤더파일
#include "smem.h"


/**
 * @brief 입력 파라미터들에 대한 기본값을 설정한다.
 * @parma[in] mib 모듈 통합관리정보
 */
static void SMEM_SetDefaultInputParameters(SMEM_MIB *mib)
{
	// redis 기본값
	strncpy(mib->redis_if.server_addr_str, "172.17.251.1", sizeof(mib->redis_if.server_addr_str));
	mib->redis_if.server_port = 6379; // Redis 기본 포트번호
	
  mib->logfile = NULL;
  mib->log_lv = kKVHLOGLevel_InOut;
  mib->slib_log_lv = SMEM_DEFAULT_SLIB_LOG_LEVEL;
}


/**
 * @brief 옵션값에 따라 각 옵션을 처리한다.
 * @parma[in] mib 모듈 통합관리정보
 * @param[in] option 옵션값 (struct option 의 4번째 멤버변수)
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int SMEM_ProcessParsedOption(SMEM_MIB *mib, int option)
{
  int ret = 0;
  switch (option) {
    case 0: { // "dbg"
      mib->log_lv = (KVHLOGLevel)strtoul(optarg, NULL, 10);
      break;
    }
    case 1: { // "sdbg"
      mib->slib_log_lv = (long)strtoul(optarg, NULL, 10);
      break;
    }
		case 2: { // "redis_server_address"
      strncpy(mib->redis_if.server_addr_str, optarg, INET_ADDRSTRLEN);
      break;
    }
    case 3: { // "redis_server_port"
      mib->redis_if.server_port = strtoul(optarg, NULL, 10);
      break;
    }
    case 6: { // "logfile"
      mib->logfile = strdup(optarg);
      break;
    }
		case 7: { // "Sidelink Type"
      mib->sidelink_type = (int)strtoul(optarg, NULL, 10);
      break;
    }
    case 8: { // "Dot2 Enable"
      mib->v2x.config.dot2.enable = (int)strtoul(optarg, NULL, 10);
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
int SMEM_ParsingInputParameters(int argc, char *argv[], SMEM_MIB *mib)
{
  int c, option_idx = 0;
  struct option options[] = {
    {"dbg", required_argument, 0, 0/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"sdbg", required_argument, 0, 1/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"redis_server_addr", required_argument, 0, 2/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"redis_server_port", required_argument, 0, 3/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"logfile", required_argument, 0, 6/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
		{"sidelink_type", required_argument, 0, 7/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"dot2_enable", required_argument, 0, 8/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {0, 0, 0, 0} // 옵션 배열은 {0,0,0,0} 센티넬에 의해 만료된다.
  };

  /*
   * 기본설정정보를 설정한다.
   */
  if (memcmp(argv[1], "start", 5) != 0) {
    printf("Invalid operation - %s\n", argv[1]);
    return -1;
  }
  SMEM_SetDefaultInputParameters(mib);

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
    int ret = SMEM_ProcessParsedOption(mib, c);
    if (ret < 0) {
      return ret;
    }
  }

  return 0;
}

