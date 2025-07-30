
#include "relay_v2x_dot2.h"
#include "relay_config.h"

static void RELAY_INNO_LoadCMHFFiles(const char *dir_path);
void RELAY_INNO_ProcessSPDUCallback(Dot2ResultCode result, void *priv);


extern int RELAY_INNO_V2X_Dot2_Security_Init()
{
	int ret;
  _DEBUG_PRINT("Initialize security\n");

  /*
   * 상위인증서들의 정보를 등록한다.
   */
  ret = Dot2_LoadSCCCertFile("certificates/trustedcerts/rca");
  if (ret < 0) {
    _DEBUG_PRINT("Fail to add RCA cert - Dot2_LoadSCCCertFile() failed: %d\n", ret);
    return -1;
  }
  ret = Dot2_LoadSCCCertFile("certificates/trustedcerts/ica");
  if (ret < 0) {
    _DEBUG_PRINT("Fail to add ICA cert - Dot2_LoadSCCCertFile() failed: %d\n", ret);
    return -1;
  }
  ret = Dot2_LoadSCCCertFile("certificates/trustedcerts/pca");
  if (ret < 0) {
    _DEBUG_PRINT("Fail to add PCA cert - Dot2_LoadSCCCertFile() failed: %d\n", ret);
    return -1;
  }

  /*
   * 서명 생성을 위한 CMHF를 등록한다.
   */
  RELAY_INNO_LoadCMHFFiles("certificates/obu");

  /*
   * PSID(32(BSM),135(WSA),82056(MAP),82055(SPAT),82057(RTCM),82054(TIM)) Security profile을 등록한다.
   */
  struct Dot2SecProfile profile;
  memset(&profile, 0, sizeof(profile));
  profile.tx.gen_time_hdr = true;
  profile.tx.gen_location_hdr = false;
  profile.tx.exp_time_hdr = false;
  profile.tx.spdu_lifetime = 30 * 1000 * 1000;
  profile.tx.min_inter_cert_time = 450 * 1000;
  profile.tx.sign_type = kDot2SecProfileSign_Compressed;
  profile.tx.ecp_format = kDot2SecProfileEcPointFormat_Compressed;
  profile.tx.interval = 100; // 100msec 주기로 송신

	profile.rx.verify_data = true;
	profile.rx.relevance_check.replay = false;
	profile.rx.relevance_check.gen_time_in_past = false;
	profile.rx.relevance_check.validity_period = 10000ULL; // 10msec
	profile.rx.relevance_check.gen_time_in_future = false;
	profile.rx.relevance_check.acceptable_future_data_period = 60000000ULL; // 1분
	profile.rx.relevance_check.gen_time_src = kDot2RelevanceTimeSource_SecurityHeader;
	profile.rx.relevance_check.exp_time = false;
	profile.rx.relevance_check.exp_time_src = kDot2RelevanceTimeSource_SecurityHeader;
	profile.rx.relevance_check.gen_location_distance = false;
	profile.rx.relevance_check.cert_expiry = false;
	profile.rx.consistency_check.gen_location = false;
  
	unsigned int psid_arry[8] = {32,135,82056,82055,82051,82053,82057,82054};
	for(int  i = 0; i < 8; i++)
	{
		profile.psid = psid_arry[i];
		if (G_relay_inno_config.v2x.j2735.bsm.rx_enable) {
			profile.rx.verify_data = false; // 수신 기능은 사용하지 않는다.
		} else {
			profile.rx.verify_data = true;
		}
		if(profile.rx.verify_data == true)
		{
			if(RELAY_INNO_V2X_Psid_Filter(profile.psid) == false)
			{
				_DEBUG_PRINT("PSID(%u) is filtered\n", profile.psid);
				continue;
			}
		}
		ret = Dot2_AddSecProfile(&profile);
		if (ret < 0) {
			_DEBUG_PRINT("Fail to register security profile - Dot2_AddSecProfile() failed: %d\n", ret);
			return -1;
		}else{
			_DEBUG_PRINT("Success to register security profile - PSID: %u\n", profile.psid);
		}
	}

	Dot2_RegisterProcessSPDUCallback(RELAY_INNO_ProcessSPDUCallback);
  _DEBUG_PRINT("Success to initialize dot2 library\n");

  _DEBUG_PRINT("Success to initialize security\n");
  return 0;
}

/**
 * @brief 특정 디렉토리에 저장되어 있는 모든 CMHF 파일들을 dot2 라이브러리에 로딩한다.
 * @param[in] dir_path CMHF 파일들이 저장된 디렉토리 경로 (상대경로, 절대경로 모두 가능)
 */
static void RELAY_INNO_LoadCMHFFiles(const char *dir_path)
{
  _DEBUG_PRINT("Load CMHF files in %s\n", dir_path);

  /*
   * 디렉토리를 연다.
   */
  DIR *dir;
  struct dirent *ent;
  dir = opendir(dir_path);
  assert(dir);

  /*
   * CMHF 파일의 경로가 저장될 버퍼를 할당한다.
   */
  size_t file_path_size = strlen(dir_path) + 128;
  char *file_path = (char *)calloc(1, file_path_size);
  assert(file_path);

  /*
   * 디렉토리 내 모든 CMHF 파일을 import하여 등록한다.
   */
  unsigned int add_cnt = 0;
  int ret;
  while ((ent = readdir(dir)) != NULL)
  {
    // 파일의 경로를 구한다. (입력된 디렉터리명과 탐색된 파일명의 결합)
    memset(file_path, 0, file_path_size);
    strcpy(file_path, dir_path);
    *(file_path + strlen(dir_path)) = '/';
    strcat(file_path, ent->d_name);

    _DEBUG_PRINT("Load CMHF file(%s)\n", file_path);

    // CMHF를 등록한다.
    ret = Dot2_LoadCMHFFile(file_path);
    if (ret < 0) {
      _DEBUG_PRINT("Fail to load CMHF file(%s) - Dot2_LoadCMHFFile() failed: %d\n", file_path, ret);
      continue;
    }
    _DEBUG_PRINT("Success to load CMHF file\n");
    add_cnt++;
  }
  free(file_path);
	file_path = NULL;
  closedir(dir);

  _DEBUG_PRINT("Sucess to load %u CMHF files\n", add_cnt);
}