/**
 * @file
 * @brief DSM 실행 파라미터 처리 기능
 * @date 2025-06-11
 * @author donggyu
 */


// 모듈 헤더파일
#include "dscm.h"


/**
 * @brief 입력 파라미터들에 대한 기본값을 설정한다.
 * @parma[in] mib 모듈 통합관리정보
 */
static void DSCM_SetDefaultInputParameters(DSCM_MIB *mib)
{
	memset(mib, 0, sizeof(DSCM_MIB));
	/// DC Interface
	mib->dc_if.dc_eth_if[0] = '\0';
	mib->dc_if.dc_login_info.login_state = false;
	sprintf(mib->dc_if.dc_login_info.auth_info, "%s", "VFJNTkwwMDAwNjpLRVkxNTU1"); // 기본값: "TRNNL00006:KEY1555"를 Base64 인코딩한 문자열
	// redis 기본값
	strncpy(mib->redis_if.server_addr_str, "127.0.0.1", sizeof(mib->redis_if.server_addr_str));
	mib->redis_if.server_port = 6379; // Redis 기본 포트번호
	
	/// HMI Interface
	strncpy(mib->hmi_if.hmi_eth_if, DSCMHMI_INVEHICLE_AETH_DEFAULT_IF_DEV_NAME, sizeof(mib->hmi_if.hmi_eth_if));
	strncpy(mib->hmi_if.hmi_server_addr, DSCMHMI_TCO_DEFAULT_IF_IPV4_SERVER_ADDR_STR, sizeof(mib->hmi_if.hmi_server_addr));
	mib->hmi_if.hmi_server_port = KVH_IVN_HMI_IF_TCP_PORT_DRIVINGSTRATEGY_MSG_SERVER;

	mib->logfile = NULL;
	mib->log_lv = kKVHLOGLevel_InOut;

	/// Message Broker 사용여부
	mib->dc_if.mb_enable = true;
	mib->hmi_if.mb_enable = true;
}


/**
 * @brief 옵션값에 따라 각 옵션을 처리한다.
 * @parma[in] mib 모듈 통합관리정보
 * @param[in] option 옵션값 (struct option 의 4번째 멤버변수)
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int DSCM_ProcessParsedOption(DSCM_MIB *mib, int option)
{
  int ret = 0;
  switch (option) {

    case 3: { // "DC Ethernet I/F Name using the external vechicle."
      strncpy(mib->dc_if.dc_eth_if, optarg, sizeof(mib->dc_if.dc_eth_if));
      break;
    }
		case 4: { // "DC I/F TCP server string type IP address"
      strncpy(mib->dc_if.dc_server_addr, optarg, sizeof(mib->dc_if.dc_server_addr));
      break;
    }
    case 5: { // "DC I/F TCP client string type IP address"
      strncpy(mib->dc_if.dc_client_addr, optarg, sizeof(mib->dc_if.dc_client_addr));
      break;
    }
		case 6: { // "DC driving strategy data TCP server port. If not specified, Endian(HOST)"
			mib->dc_if.dc_server_port = (uint16_t)strtoul(optarg, NULL, 10);
			break;
		}
		case 7: { // "DC driving strategy data TCP client port. If not specified, Endian(HOST)"
			mib->dc_if.dc_client_port = (uint16_t)strtoul(optarg, NULL, 10);
			break;
		}
		case 8: { // "DC Login - Authentication Info. If not specified application no working."
      strncpy(mib->dc_if.dc_login_info.auth_info, optarg, sizeof(mib->dc_if.dc_login_info.auth_info));
      break;
    }
		case 9: { // "HMI Ethernet I/F Name using the IVN domain."
      strncpy(mib->hmi_if.hmi_eth_if, optarg, sizeof(mib->hmi_if.hmi_eth_if));
      break;
    }
		case 10: { // "HMI I/F TCP server string type IP address"
      strncpy(mib->hmi_if.hmi_server_addr, optarg, sizeof(mib->hmi_if.hmi_server_addr));
      break;
    }
		case 11: { // "HMI driving strategy data TCP server port. If not specified, Endian(HOST)"
			mib->hmi_if.hmi_server_port = (uint16_t)strtoul(optarg, NULL, 10);
			break;
		}
		case 12: { // "logfile"
      mib->logfile = strdup(optarg);
      break;
    }
		case 13: { // "dbg"
      mib->log_lv = (KVHLOGLevel)strtoul(optarg, NULL, 10);
      break;
    }
		case 14: { // "DC Message Broker 사용여부"
      mib->dc_if.mb_enable = (KVHLOGLevel)strtoul(optarg, NULL, 10);
      break;
    }
		case 15: { // "HMI Message Broker 사용여부"
      mib->hmi_if.mb_enable = (KVHLOGLevel)strtoul(optarg, NULL, 10);
      break;
    }
		case 16: { // "redis_server_address"
      strncpy(mib->redis_if.server_addr_str, optarg, INET_ADDRSTRLEN);
      break;
    }
    case 17: { // "redis_server_port"
      mib->redis_if.server_port = strtoul(optarg, NULL, 10);
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
int DSCM_ParsingInputParameters(int argc, char *argv[], DSCM_MIB *mib)
{
  int c, option_idx = 0;
  struct option options[] = {
    
    {"dscm_suds", required_argument, 0, 1/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"dscm_cuds", required_argument, 0, 2/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"dc_eth_if", required_argument, 0, 3/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"dc_server_addr", required_argument, 0, 4/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
		{"dc_client_addr", required_argument, 0, 5/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
		{"dc_server_port", required_argument, 0, 6/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
		{"dc_client_port", required_argument, 0, 7/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
		{"auth_info", required_argument, 0, 8/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
		{"hmi_eth_if", required_argument, 0, 9/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
		{"hmi_server_addr", required_argument, 0, 10/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
		{"hmi_server_port", required_argument, 0, 11/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"logfile", required_argument, 0, 12/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
		{"dbg", required_argument, 0, 13/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
		{"dc_mb_enable", required_argument, 0, 14/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
		{"hmi_mb_enable", required_argument, 0, 15/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
		{"redis_server_addr", required_argument, 0, 16/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {"redis_server_port", required_argument, 0, 17/*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {0, 0, 0, 0} // 옵션 배열은 {0,0,0,0} 센티넬에 의해 만료된다.
  };

  /*
   * 기본설정정보를 설정한다.
   */
  if (memcmp(argv[1], "start", 5) != 0) {
    printf("Invalid operation - %s\n", argv[1]);
    return -1;
  }

  /*
   * 기본설정정보를 설정한다.
   */
  DSCM_SetDefaultInputParameters(mib);

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
    int ret = DSCM_ProcessParsedOption(mib, c);
    if (ret < 0) {
      return ret;
    }
  }

  return 0;
}
