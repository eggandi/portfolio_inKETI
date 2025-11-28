/**
 * @file
 * @brief CSM(C-ITS Service Module)의 V2X 서비스 기능 구현
 * @date 2025-08-28
 * @author Dong
 */


// 모듈 헤더파일
#include "smem.h"


/**
 * @brief j29451 라이브러리가 호출하는 BSM 송신 기본 콜백함수
 * @param[in] bsm BSM 메시지 UPER 인코딩 바이트열
 * @param[in] bsm_size BSM 메시지의 길이
 * @param[in] event 이벤트 발생 여부
 * @param[in] cert_sign 인증서로 서명해야 하는지 여부
 * @param[in] id_change ID/인증서 변경 필요 여부
 * @param[in] addr 랜덤하게 생성된 MAC 주소. id_change=true일 경우 본 MAC 주소를 장치에 설정해야 한다.
 */
void LTEV2X_J2735J29451TxDefaultCallback(const uint8_t *bsm, size_t bsm_size, bool event, bool cert_sign, bool id_change, uint8_t *addr)
{
  LOG(kKVHLOGLevel_Event,"BSM tx callback - event: %u, cert_sign: %u, id_change: %u\n", event, cert_sign, id_change);
	LTEV2XDot2Config *dot2_config = &g_smem_mib.v2x.config.dot2;
	LTEV2XJ273BSMJ29451LibraryConfig *j29451_config = &g_smem_mib.v2x.config.j2735.j29451;
	const uint8_t *wsdu = NULL;
	size_t wsdu_size = 0;
	struct Dot2SPDUConstructResult res;
	if (dot2_config->enable == true) {
		LTEV2XDot2SPDUParams *dot2 = j29451_config->txparams.dot2;
		// SPDU를 생성한.
		if (cert_sign == true) {
			LOG(kKVHLOGLevel_All, "BSM tx callback - Force to sign with certificate\n");
			dot2->signed_data.signer_id_type = kDot2SignerId_Certificate;
		} else {
			dot2->signed_data.signer_id_type = kDot2SignerId_Profile;
		}
		#if 0
		LTEV2X_MarcoJ2735RangeCheck(dot2->signed_data.gen_location.lat, kDot2Latitude_Unavailable, kDot2Latitude_Unavailable, kDot2Latitude_Min);
		LTEV2X_MarcoJ2735RangeCheck(dot2->signed_data.gen_location.lon, kDot2Longitude_Unavailable, kDot2Longitude_Unavailable, kDot2Longitude_Min);
		LTEV2X_MarcoJ2735RangeCheck(dot2->signed_data.gen_location.elev, kDot2Elevation_Unavailable, kDot2Elevation_Unavailable, kDot2Elevation_Min);
		#endif
		res = Dot2_ConstructSPDU(dot2, bsm, bsm_size);
		if (res.ret < 0) {
			ERR("BSM tx callback - Dot2_ConstructSPDU() failed: %d\n", res.ret);
			return;
		}
		wsdu = res.spdu;
		wsdu_size = (size_t)res.ret;
	} else{
		// Dot2 라이브러리 사용안함. Non Secured
		wsdu = bsm;
		wsdu_size = bsm_size;
	}

  /*
   * 필요 시 MAC 주소를 변경한다.
   */
  if (id_change == true) {
    LOG(kKVHLOGLevel_Event, "BSM tx callback - id changed - new MAC addr: "MAC_ADDR_FMT"\n", MAC_ADDR_FMT_ARGS(addr));
    memcpy(j29451_config->macaddr_arr, addr, MAC_ALEN);
  }

	//WSM을 생성한다.
	LOG(kKVHLOGLevel_All, "Success to encode BSM - %ld bytes\n", wsdu_size);

  size_t wsm_size;
	int ret = 0;
  uint8_t *wsm = Dot3_ConstructWSM(&j29451_config->txparams.wsm, wsdu, wsdu_size, &wsm_size, &ret);
  if (wsm == NULL) 
	{
    ERR("Fail to construct LTE-V2X WSM - Dot3_ConstructWSM() failed - %d\n", ret);
    return;
  }
  LOG(kKVHLOGLevel_All, "Success to construct %ld-bytes LTE-V2X WSM\n", wsm_size);


  // WSM을 전송한다.
  ret = LTEV2XHAL_TransmitMSDU(wsm, wsm_size, j29451_config->txparams.msdu);
	if (ret != 0 && ret != -7)
	{
		ERR("Fail to transmit LTE-V2X WSM - LTEV2XHAL_TransmitMSDU() failed: %d\n", ret);
	}
	LOG(kKVHLOGLevel_InOut, "Success to transmit LTE-V2X WSM\n");

	if (wsm != NULL)
	{
		free(wsm);
		wsm = NULL;
	}	

	ret = SMEM_ProcessRedisSETData(kKVHREDISKeySUBDOMAIN_CSM, kKVHREDISKeyDataModelType_ASN1KSR1600, kKVHREDISKeyDataModelSubType_BSM, bsm, bsm_size);
	if (ret != 0) {
		ERR("Fail to store BSM to Redis server:%d\n", ret);
	}															
	
	if (dot2_config->enable == true)
	{
		// SPDU 메모리 해제
		free(res.spdu);
		// 현재 인증서가 이미 만기되었거나 다음번 BSM 서명 시에 만기될 경우에는, BSM ID 변경을 요청한다.
		// 다음번 BSM 콜백함수가 호출될 때, id_change=true, cert_sign=true 가 전달된다.
		if (res.cmh_expiry == true) {
			LOG(kKVHLOGLevel_All, "BSM tx callback - Certificate will be expired. Request to change BSM ID\n");
			J29451_RequestBSMIDChange();
		}
	}

  /*
   * Power off가 된 상태이면,
   * BSM 전송을 중지하고 Path 정보를 백업한다.
   */
	#if 0 // 현재 Power Off 감지 여부 모름
  if (G_power_off == true) {
    LOG(kKVHLOGLevel_Event, "Power off detected - stop BSM transmit and backup path info\n");
    J29451_StopBSMTransmit();
    J29451_SavePathInfoBackupFile(LTEV2X_V2X_J2735_J29451_DEFAULT_PATHHISTORY_FILE_PATH);
		system("sync"); // 낸드 플래시 싱크 명령 강제 입력 (=전원이 꺼지기 전에 낸드플래시에 즉각 저장되도록 한다)
  }
	#endif
}


/**
 * @brief J29451 라이브러리를 초기화한다.
 * @details J29451 라이브러리를 초기화하고 BSM 송신 콜백함수를 등록한다.
 * @retval 0: 성공
 * @retval -1: J29451_Init 실패, -2: J29451_SetVehicleSize 실패
 */
int LTEV2X_J2735J29451Inlitial(const char *pathhistory_filepath, J29451LogLevel *loglevel, J29451BSMTxInterval *tx_interval, J29451VehicleWidth *width, J29451VehicleLength *length)
{
	int ret;
	LTEV2XJ273BSMJ29451LibraryConfig *j29451_config = &g_smem_mib.v2x.config.j2735.j29451;
	// 초기값 설정
	// ::TODO main arguments 또는 Config file 통해 입력 받도록 수정
	if (pathhistory_filepath) {
		memcpy(j29451_config->pathhistory_filepath, pathhistory_filepath, strlen(pathhistory_filepath) + 1);
		j29451_config->pathhistory_filepath[strlen(pathhistory_filepath)] = '\0';
	} else {
		memcpy(j29451_config->pathhistory_filepath, LTEV2X_V2X_J2735_J29451_DEFAULT_PATHHISTORY_FILE_PATH, sizeof(LTEV2X_V2X_J2735_J29451_DEFAULT_PATHHISTORY_FILE_PATH));
	}
	if (loglevel) {///< 로그 저장 레벨 (기본값 적용 또는 프로그램 실행 인자로 입력된다)
		j29451_config->loglevel = *loglevel > kJ29451LogLevel_Max ? kJ29451LogLevel_All : *loglevel;
	} else {
		j29451_config->loglevel = kJ29451LogLevel_None;
	}
	if (tx_interval) {///< BSM 송신 주기 (기본값 적용 또는 프로그램 실행 인자로 입력된다)
		j29451_config->tx_interval = *tx_interval;
	} else {
		j29451_config->tx_interval = kJ29451BSMTxInterval_Default;
	}
	if (width && length) {///< 차량 크기 (기본값 적용 또는 프로그램 실행 인자로 입력된다)
		j29451_config->vehicleinfo.width = *width;
		j29451_config->vehicleinfo.length = *length;
	} else {
		j29451_config->vehicleinfo.width = kJ29451VehicleWidth_Unavailable;
		j29451_config->vehicleinfo.length = kJ29451VehicleLength_Unavailable;
	}

	if (j29451_config->txparams.dot2 == NULL) {///< Dot2 파라메터가 설정되지 않았으면 기본값으로 설정
		j29451_config->txparams.dot2 = malloc(sizeof(LTEV2XDot2SPDUParams));
	}

	if ((ret = LTEV2X_SetParameterDot2(j29451_config->txparams.dot2, kDot2SPDUConstructType_Unsecured,
																																	 NULL,
																																	 32,
																																	 kDot2SignerId_Profile)) < 0) {
		ERR("Fail to set Dot2 parameters - LTEV2X_SetParameterDot2() failed: %d\n", ret);
		return -1;
	}
	Dot3ChannelNumber chan_num = 173;
	Dot3DataRate datarate =kDot3DataRate_6Mbps;
	Dot3Power transmit_power = kDot3Power_TxDefault;
	if (LTEV2X_SetParameterDot3(&j29451_config->txparams.wsm, kDot3ProtocolVersion_Current,
																														32,
																														&chan_num,
																														&datarate,
																														&transmit_power)) {
		ERR("Fail to set Dot3 parameters - LTEV2X_SetParameterDot3() failed\n");
		return -1;
	}

	LTEV2XHALPriority priority = kLTEV2XHALPriority_Default; 
	LTEV2XHALPower tx_power = kDot3Power_TxDefault;
	if (LTEV2X_SetParameterMSDU(&j29451_config->txparams.msdu, kLTEV2XHALTxFlowType_SPS,
																														 kLTEV2XHALTxFLowIndex_Default,
																														 &priority,
																														 kLTEV2XHALL2ID_Broadcast,
																														 &tx_power)) {
		ERR("Fail to set MSDU parameters - LTEV2X_SetParameterMSDU() failed\n");
		return -1;
	}
	
	// J29451 라이브러리 초기화 함수 
	ret = J29451_Init(g_smem_mib.slib_log_lv, j29451_config->macaddr_arr);
  if (ret < 0) {
    ERR("Fail to initialize j29451 library - J29451_Init() failed: %d\n", ret);
    return -1;
  }
	/**
	 * 송신 주기는 LTE-V2X에서는 Flow Controll에서 정의하고 J29451라이브러리에서도 정의 되기 때문에
	 * 두 설정과 같게 하기 위해서 J29451 라이브러리 Tx 주기 설정을 입력 받는다. 
	 */
	// J29451 라이브러리에서 Tx 콜백 함수 등록 위 interval 주기로 호출 된다. 
	if (j29451_config->txcallback) {
		J29451_RegisterBSMTransmitCallback(j29451_config->txcallback);
	} else {
		J29451_RegisterBSMTransmitCallback(LTEV2X_J2735J29451TxDefaultCallback);
		j29451_config->txcallback = LTEV2X_J2735J29451TxDefaultCallback;
	}
	/**
	 * J29451 라이브러리 표준 상 BSM의 Path History를 저장했다가 어플리케이션을 재실할 때 불러오는 기능을 수행하기 위해
	 * file로 백업된 Path history를 불러오는 기능
	 */
	J29451_LoadPathInfoBackupFile((const char *)j29451_config->pathhistory_filepath);
  LOG(kKVHLOGLevel_Init, "Success to initialize j29451 libraries, path:%s\n", j29451_config->pathhistory_filepath);
	/*
   * BSM 필수정보를 설정한다.
   */
	ret = J29451_SetVehicleSize(j29451_config->vehicleinfo.width, j29451_config->vehicleinfo.length);
	if (ret < 0) {
		ERR("Fail to start BSM transmit - J29451_SetVehicleSize() failed: %d\n", ret);
		return -2;
	}

	return 0;
}


/**
 * @brief J2735/J29451 설정을 적용한다.
 * @param dot2: Dot2 SPDU 파라미터
 * @param wsm: Dot3 WSM 파라미터
 * @param msdu: MSDU 파라미터
 */
void LTEV2X_SetJ2735J29451Config(LTEV2XDot2SPDUParams *dot2, LTEV2XDot3WSMParameters *wsm, LTEV2XMSDUParameters *msdu)
{
	LTEV2XJ273BSMJ29451LibraryConfig *j29451_config = &g_smem_mib.v2x.config.j2735.j29451;
	if (dot2) {
		memcpy(&j29451_config->txparams.dot2, dot2, sizeof(LTEV2XDot2SPDUParams));
	}
	if (wsm) {
		memcpy(&j29451_config->txparams.wsm, wsm, sizeof(LTEV2XDot3WSMParameters));
	}
	if (msdu) {
		memcpy(&j29451_config->txparams.msdu, msdu, sizeof(LTEV2XMSDUParameters));
	}
}
