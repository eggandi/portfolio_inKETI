/**
 * @file
 * @brief SMEM(C-ITS Service Module)의 LTEV2X Dot2 서비스 기능
 * @date 2025-08-28
 * @author Dong
 */


// 모듈 헤더파일
#include "smem.h"


/**
 * @brief Dot2 SPDU 처리 콜백 함수 TODO:
 * @param[in] result 처리 결과
 * @param[in] priv 사용자 데이터
 */
void LTEV2X_Dot2ProcessSPDUCallback(Dot2ResultCode result, void *priv){

  if (!priv) {
    ERR("Fail to process SPDU - null priv\n");
    return;
  }

  struct V2XPacketParseData *parsed = (struct V2XPacketParseData *)priv;
  struct Dot2SPDUParseData *dot2_parsed = &(parsed->spdu);

  if (result != kDot2Result_Success) {
    ERR("Fail to process SPDU. result is %d\n", result);
    V2X_FreePacketParseData(parsed);
    return;
  }

  /*
   * UnsecuredData 형식의 dot2 메시지 처리 결과를 출력한다.
   */
  if (dot2_parsed->content_type == kDot2Content_UnsecuredData) {
    LOG(kKVHLOGLevel_Event, "Success to process UNSECURED SPDU. Payload size is %u\n", parsed->ssdu_size);
  } else if (dot2_parsed->content_type == kDot2Content_SignedData) {
    LOG(kKVHLOGLevel_Event, "Success to process SIGNED SPDU. Payload size is %u\n", parsed->ssdu_size);
  } else {
    LOG(kKVHLOGLevel_Event, "Success to process SPDU(content_type: %u). Payload size is %u\n", dot2_parsed->content_type, parsed->ssdu_size);
  }

  /*
   * 페이로드를 처리한다. (SAE J2735 or KS R 1600)
   */
	LTEV2X_ProcessRxLTEV2XPayload(parsed->mac_wsm.wsm.psid, parsed->ssdu, parsed->ssdu_size);

	V2X_FreePacketParseData(parsed);
}


/**
 * @brief 기본 보안 프로파일을 설정한다.
 * @param[in] profile 설정할 보안 프로파일
 */
int LTEV2X_Dot2FillDefaultSecurityProfile(struct Dot2SecProfile *profile)
{
	if (!profile) {
		return -1;
	}
	
	memset(profile, 0, sizeof(*profile));
	
	profile->tx.sign_type = kDot2SecProfileSign_Compressed;
	profile->tx.ecp_format = kDot2SecProfileEcPointFormat_Compressed;
	profile->tx.interval = 100; // 100msec 주기로 송신

	if (g_smem_mib.v2x.config.dot2.enable == true) {
		profile->tx.gen_time_hdr = true;
		profile->tx.gen_location_hdr = false;
		profile->tx.exp_time_hdr = false;
		profile->tx.spdu_lifetime = 30 * 1000 * 1000;
		profile->tx.min_inter_cert_time = 450 * 1000;

		profile->rx.verify_data = true;
		profile->rx.relevance_check.replay = false;
		profile->rx.relevance_check.gen_time_in_past = false;
		profile->rx.relevance_check.validity_period = 10000ULL; // 10msec
		profile->rx.relevance_check.gen_time_in_future = false;
		profile->rx.relevance_check.acceptable_future_data_period = 60000000ULL; // 1분
		profile->rx.relevance_check.gen_time_src = kDot2RelevanceTimeSource_SecurityHeader;
		profile->rx.relevance_check.exp_time = false;
		profile->rx.relevance_check.exp_time_src = kDot2RelevanceTimeSource_SecurityHeader;
		profile->rx.relevance_check.gen_location_distance = false;
		profile->rx.relevance_check.cert_expiry = false;
		profile->rx.consistency_check.gen_location = false;
	} else {
		profile->rx.verify_data = false;
	}


	return 0;
}

/**
 * @brief Dot2 Profile PSID 필터
 * @param[in] psid 
 * @retval true | false
 */
bool LTEV2X_Dot2SecurityProfilePSIDFilter(unsigned int psid)
{
	LTEV2XJ2735RxEnableList *rx_list = &g_smem_mib.v2x.config.j2735.rx;
	switch (psid)
	{
		case 135: ///<- @ref kDot2PSID_WSA
			if (g_smem_mib.v2x.config.wsa_enable == false) goto psid_false;
			break;
    case 32: ///<- @ref kDot2PSID_BSM
		case 82050: ///<- @ref kDot2PSID_BSM
			if (rx_list->BSM_enable == false) goto psid_false;
			break;
		case 82056: ///<- @ref kDot2PSID_MAP
			if (rx_list->MAP_enable == false) goto psid_false;
			break;
		case 82055: ///<- @ref kDot2PSID_SPAT
			if (rx_list->SPAT_enable == false) goto psid_false;
			break;
		case 82051: ///<- @ref kDot2PSID_PVD
			if (rx_list->PVD_enable == false) goto psid_false;
			break;
		case 82053: ///<- @ref kDot2PSID_RSA
			if (rx_list->RSA_enable == false) goto psid_false;
			break;
		case 82057: ///<- @ref kDot2PSID_RTCM
			if (rx_list->RTCM_enable == false) goto psid_false;
			break;
		case 82054: ///<- @ref kDot2PSID_TIM
			if (rx_list->TIM_enable == false) goto psid_false;
			break;
		default : break;
	}		
	return true;
psid_false:
	return false;

}


/**
 * @brief 특정 디렉토리에 저장되어 있는 모든 CMHF 파일들을 dot2 라이브러리에 로딩한다.
 * @param[in] dir_path CMHF 파일들이 저장된 디렉토리 경로 (상대경로, 절대경로 모두 가능)
 */
void LTEV2X_Dot2LoadCMHFFiles(const char *dir_path)
{
  LOG(kKVHLOGLevel_Init,"Load CMHF files in %s\n", dir_path);

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
  while ((ent = readdir(dir)) != NULL)  {// 파일의 경로를 구한다. (입력된 디렉터리명과 탐색된 파일명의 결합)
    memset(file_path, 0, file_path_size);
    strcpy(file_path, dir_path);
    *(file_path + strlen(dir_path)) = '/';
    strcat(file_path, ent->d_name);

    LOG(kKVHLOGLevel_Init,"Load CMHF file(%s)\n", file_path);

    // CMHF를 등록한다.
    ret = Dot2_LoadCMHFFile(file_path);
    if (ret < 0) {
      ERR("Fail to load CMHF file(%s) - Dot2_LoadCMHFFile() failed: %d\n", file_path, ret);
      continue;
    }
    LOG(kKVHLOGLevel_Init,"Success to load CMHF file\n");
    add_cnt++;
  }
  free(file_path);
	file_path = NULL;
  closedir(dir);

  LOG(kKVHLOGLevel_Init,"Success to load %u CMHF files\n", add_cnt);
}


/**
 * @brief Dot2 초기 보안 설정
 */
int LTEV2X_Dot2InitialSecurity()
{
	int ret;
  LOG(kKVHLOGLevel_Init,"Initialize security\n");
	LTEV2XDot2Config *dot2_config = &g_smem_mib.v2x.config.dot2;
	if (dot2_config->cmhf_obu_enable) { // obu 인증서
		/*
		* 상위인증서들의 정보를 등록한다.
		*/
		ret = Dot2_LoadSCCCertFile("certificates/trustedcerts/rca");
		if (ret < 0) {
			ERR("Fail to add RCA cert - Dot2_LoadSCCCertFile() failed: %d\n", ret);
			return -1;
		}
		ret = Dot2_LoadSCCCertFile("certificates/trustedcerts/ica");
		if (ret < 0) {
			ERR("Fail to add ICA cert - Dot2_LoadSCCCertFile() failed: %d\n", ret);
			return -1;
		}
		ret = Dot2_LoadSCCCertFile("certificates/trustedcerts/pca");
		if (ret < 0) {
			ERR("Fail to add PCA cert - Dot2_LoadSCCCertFile() failed: %d\n", ret);
			return -1;
		}
		/*
		* 서명 생성을 위한 CMHF를 등록한다.
		*/
		
		LTEV2X_Dot2LoadCMHFFiles("certificates/obu");
	} else if (dot2_config->cmhf_rsu_enable) {
		//::TODO 장치를 RSU로 사용할 때를 대비, V2XHUB는 OBU로만 동작 
	}

	struct Dot2SecProfile *profile = &dot2_config->profile;
	LTEV2X_Dot2FillDefaultSecurityProfile(profile);

  /*
   * PSID(82050(BSM),135(WSA),82056(MAP),82055(SPAT),82057(RTCM),82054(TIM)) Security profile을 등록한다.
   */
	unsigned int psid_arry[8] = {82050, 135, 82056, 82055, 82051, 82053, 82057, 82054};
	for (int  i = 0; i < 8; i++)
	{
		profile->psid = psid_arry[i];
		// PSID 필터링
		if ((profile->rx.verify_data = LTEV2X_Dot2SecurityProfilePSIDFilter(profile->psid)) == false)
		{
			LOG(kKVHLOGLevel_Event,"PSID(%u) is filtered\n", profile->psid);
			continue;
		}
		
		ret = Dot2_AddSecProfile(profile);
		if (ret < 0) {
			ERR("Fail to register security profile - Dot2_AddSecProfile() failed: %d\n", ret);
			return -1;
		}else{
			LOG(kKVHLOGLevel_Init,"Success to register security profile - PSID: %u\n", profile->psid);
		}
	}

	Dot2_RegisterProcessSPDUCallback(LTEV2X_Dot2ProcessSPDUCallback);
  LOG(kKVHLOGLevel_Init,"Success to initialize dot2 library\n");

  LOG(kKVHLOGLevel_Init,"Success to initialize security\n");
  return 0;
}


/**
 * @brief Dot2PSID 열거형 체크 후 설정 
 * @param[in] psid Dot2 PSID
 * @retval Dot2PSID
 */
Dot2PSID LTEV2X_SetParameterDot2PSID(Dot2PSID psid)
{
	int ret;
	switch (psid) {
		case kDot2PSID_Min:
		case kDot2PSID_SCMS:
		case kDot2PSID_Max:
		case kDot2PSID_IPv6Routing:
		case kDot2PSID_WSA:
		case kDot2PSID_CRL:
			ret = psid;
		break;
		default :
			LTEV2X_MarcoJ2735RangeCheck(ret, psid, kDot2PSID_Max, kDot2PSID_Min);
			break;
	}
	return (Dot2PSID)ret;
}


/**
 * @brief Dot2 SPDU 생성 파라메터 설정
 * @param[in] params Dot2 SPDU parameters를 입력받을 포인터
 * @param[in] type 생성하고자 하는 SPDU의 유형
 * @param[in] time SPDU 생성 시각 (어플리케이션이 0으로 설정할 경우, API 내부에서 현재시각으로 설정된다)
 * @param[in] psid Dot2 SPDU PSID
 * @param[in] signer_id_type 서명자인증서 식별자 유형
 * @param[in] gen_location SPDU 생성지점 (Unavailable 값 사용 가능)
 * @retval 0: 성공
 * @retval -1: 실패, -입력값(범위 오류)
 */
int LTEV2X_SetParameterDot2(LTEV2XDot2SPDUParams *params, Dot2SPDUConstructType type,
																													Dot2Time64 *time,
																													Dot2PSID psid,
																													Dot2SignerIdType signer_id_type)
{
	if (!params) {
		return -1;
	}
	LTEV2X_MarcoJ2735RangeCheck(params->type, type, kDot2SPDUConstructType_Max, 0);

	if (time == NULL) {
		params->time = 0;
	} else {
		params->time = *time;
	}

	if ((int)(params->signed_data.psid = LTEV2X_SetParameterDot2PSID(psid)) < 0)
	{
		return params->signed_data.psid;
	}
	LTEV2X_MarcoJ2735RangeCheck(params->signed_data.signer_id_type, signer_id_type, kDot2SignerId_Max, kDot2SignerId_Min);
	LTEV2X_MarcoJ2735RangeCheck(params->signed_data.gen_location.lat, kDot2Latitude_Unavailable, kDot2Latitude_Unavailable, kDot2Latitude_Min);
	LTEV2X_MarcoJ2735RangeCheck(params->signed_data.gen_location.lon, kDot2Longitude_Unavailable, kDot2Longitude_Unavailable, kDot2Longitude_Min);
	LTEV2X_MarcoJ2735RangeCheck(params->signed_data.gen_location.elev, kDot2Elevation_Unavailable, kDot2Elevation_Unavailable, kDot2Elevation_Min);
	params->signed_data.cmh_change = false;

	return 0;
}

