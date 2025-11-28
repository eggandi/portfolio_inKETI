/**
 * @file
 * @brief SMEM(Sidelink Message Exchange Module)의 V2X 서비스 기능 구현
 * @date 2025-08-28
 * @author Dong
 */


// 모듈 헤더파일
#include "smem.h"


/**
 * @brief LTEV2XHALL2ID 열거형 체크 후 설정 
 * @param[in] dst_l2_id LTEV2XHALL2ID
 * @retval LTEV2XHALL2ID
 */
LTEV2XHALL2ID LTEV2X_SetParameterMSDUL2ID(LTEV2XHALL2ID dst_l2_id)
{
	int ret;
	switch (dst_l2_id) {
		case kLTEV2XHALL2ID_Invalid:
		case kLTEV2XHALL2ID_Min:
		case kLTEV2XHALL2ID_Max: // =kLTEV2XHALL2ID_Broadcast
			ret = dst_l2_id;
			break;
		default :
			LTEV2X_MarcoJ2735RangeCheck(ret, dst_l2_id, kLTEV2XHALL2ID_Max, kLTEV2XHALL2ID_Min);
			break;
	}
	return (LTEV2XHALL2ID)ret;
}


/**
 * @brief MPDU 파라메터 설정
 * @param[in] params MPDU 입력 파라메터 입력 받을 포인터
 * @param[in] tx_flow_type 송신 플로우 유형 (0: Ad-Hoc, 1: SPS)
 * @param[in] tx_flow_index 송신 플로우 인덱스 (0 or 1, 송신 유형이 SPS인 경우에만 사용된다)
 * @param[in] priority 송신 우선순위 (송신 유형이 Ad-Hoc인 경우에만 사용된다)
 * @param[in] dst_l2_id 목적지 L2 ID
 * @param[in] tx_power  송신 세기 (dBm)
 * @retval 0: 성공
 * @retval -1: 실패, -입력값(범위 오류)
 */
int LTEV2X_SetParameterMSDU(LTEV2XMSDUParameters *params, LTEV2XHALTxFlowType tx_flow_type, 
																													LTEV2XHALTxFlowIndex tx_flow_index, 
																													LTEV2XHALPriority *priority, 
																													LTEV2XHALL2ID dst_l2_id, 
																													LTEV2XHALPower *tx_power) 
{
	if (!params) {
		return -1;
	}

	LTEV2X_MarcoJ2735RangeCheck(params->tx_flow_type, tx_flow_type, kLTEV2XHALTxFlowType_Max, kLTEV2XHALTxFLowType_Min);

	LTEV2X_MarcoJ2735RangeCheck(params->tx_flow_index, tx_flow_index, kLTEV2XHALTxFLowIndex_Max, kLTEV2XHALTxFLowIndex_Min);

	if (priority == NULL) {
		params->priority = kLTEV2XHALPriority_None;
	} else {
		LTEV2X_MarcoJ2735RangeCheck(params->priority, *priority, kLTEV2XHALPriority_Max, kLTEV2XHALPriority_Min);
	}


	if ((int)(params->dst_l2_id = LTEV2X_SetParameterMSDUL2ID(dst_l2_id)) < 0)
	{
		return params->dst_l2_id;
	}

	if (tx_power == NULL) {
		params->tx_power = kLTEV2XHALPower_Default;
	} else {
		LTEV2X_MarcoJ2735RangeCheck(params->tx_power, *tx_power, kLTEV2XHALPower_Max, kLTEV2XHALPower_Min);
	}
	
	return 0;
}


/**
 * @brief LTEV2X MPDU pkt 수신시 호출되는 콜백함수 (wlanaccess 라이브러리 내 수신 쓰레드에서 호출한다)
 * @param[in] mpdu 수신된 MPDU
 * @param[in] mpdu_size 수신된 MPDU의 크기
 * @param[in] rx_params 수신 파라미터 정보
 */
void LTEV2X_ProcessRxLTEV2XPkt(const uint8_t *msdu, LTEV2XHALMSDUSize msdu_size, struct LTEV2XHALMSDURxParams rx_params)
{
  SMEM_MIB *mib = &(g_smem_mib);
	rx_params = rx_params;
  mib->cnt.v2x_rx++;
  if ((mib->cnt.v2x_rx % 10) == 1) {
    LOG(kKVHLOGLevel_InOut, "Process rx LTEV2X pkt - cnt: %u\n", mib->cnt.v2x_rx);
  } else {
    LOG(kKVHLOGLevel_Event, "Process rx LTEV2X pkt - cnt: %u\n", mib->cnt.v2x_rx);
  }
  DUMP(kKVHLOGLevel_Dump, "LTEV2X PKT", (uint8_t *)msdu, msdu_size);

	/*
   * 패킷파싱데이터를 할당한다.
   */
	struct V2XPacketParseData *parsed = V2X_AllocateCV2XPacketParseData(msdu, msdu_size);
  if (parsed == NULL) {
    ERR("Fail to process rx LTEV2X pkt - V2X_AllocateLTEV2XPacketParseData() failed\n");
    return;
  }

  /*
   * WSM MPDU를 파싱한다.
   */
  int ret;
	parsed->wsdu = Dot3_ParseWSM( parsed->pkt,
																parsed->pkt_size,
																&(parsed->mac_wsm.wsm),
																&(parsed->wsdu_size),
																&(parsed->interested_psid),
																&ret);
  if (parsed->wsdu == NULL) {
    ERR("Fail to process rx LTEV2X pkt - Dot3_ParseWSMMPDU() failed: %d\n", ret);
    V2X_FreePacketParseData(parsed);
    return;
  }

  /*
   * 관심 없는 PSID에 대한 WSM일 경우 로그만 출력하고 종료한다.
   */
  if (parsed->interested_psid == false) {
    LOG(kKVHLOGLevel_Event, "Not interested(PSID:%u) LTEV2X WSM is received\n", parsed->mac_wsm.wsm.psid);
    //V2X_FreePacketParseData(parsed);
    //return;
  }
  /*

   * SPDU를 처리한다 - 결과는 콜백함수를 통해 전달된다.
   */
  struct Dot2SPDUProcessParams params;
  memset(&params, 0, sizeof(params));
  params.rx_time = 0;
  params.rx_psid = parsed->mac_wsm.wsm.psid;
  params.rx_pos.lat = kDot2Latitude_Unavailable; ///< consistency/relevance check를 하지 않으므로 unknown을 입력해도 된다.
  params.rx_pos.lon = kDot2Longitude_Unavailable; ///< consistency/relevance check를 하지 않으므로 unknown을 입력해도 된다.
  ret = Dot2_ProcessSPDU(parsed->wsdu, parsed->wsdu_size, &params, parsed);
  if (ret < 0) {
    ERR("Fail to process rx LTEV2X pkt - Dot2_ProcessSPDU() failed: %d\n", ret);
    V2X_FreePacketParseData(parsed);
    return;
  }

  LOG(kKVHLOGLevel_Event, "Success to request SPDU process(psid: %u)\n", parsed->mac_wsm.wsm.psid);
}
/**
 * @brief 수신된 LTEV2X 페이로드를 처리한다.
 * @param[in] psid 수신 메시지 PSID
 * @param[in] payload 페이로드
 * @param[in] payload_len 페이로드 길이
 */
void LTEV2X_ProcessRxLTEV2XPayload(uint32_t psid, uint8_t *payload, size_t payload_len)
{
  LOG(kKVHLOGLevel_Event, "Process rx LTEV2X payload(len: %zu)\n", payload_len);
	
 	/*
	* 페이로드를 처리한다. (SAE J2735 or KS R 1600)
	*/
	int ret = SMEM_ProcessRedisSETKSR1600(psid, payload, payload_len);
	if (ret < 0) {
		ERR("Fail to process rx LTEV2X payload - SMEM_ProcessRedisSETKSR1600() failed: %d\n", ret);
	}

	return;
}


/**
 * @brief LTE-V2X 송신 플로우를 등록한다.
 * @retval 0: 성공
 * @retval -1: 실패
 */
int LTEV2X_SetLTEV2XRegisterTransmitFlow(LTEV2XHALTxFlowIndex flow, J29451BSMTxInterval interval, LTEV2XHALPriority priority)
{
  LOG(kKVHLOGLevel_Init, "Preapare LTE-V2X transmit\n");

  struct LTEV2XHALTxFlowParams params;
  memset(&params, 0x00, sizeof(struct LTEV2XHALTxFlowParams));
  params.index = flow;
  params.interval = interval;
  params.priority = priority;
  params.size = 2048;

  int ret = LTEV2XHAL_RegisterTransmitFlow(params);
  if (ret < 0) {
    ERR("Fail to prepare LTE-V2X transmit - LTEV2XHAL_RegisterTransmitFlow() failed: %d\n", ret);
    return -1;
  }

  LOG(kKVHLOGLevel_Init,"Success to prepare LTE-V2X transmit\n");
  return 0;
}


/**
 * @brief V2X 라이브러리들을 초기화한다.
 * @retval 0: 성공
 * @retval -1: 실패
 */
int LTEV2X_InitialV2XLibrary(void)
{
	// LTEV2E 로그 레벨 설정
  LTEV2XHALLogLevel hal_log_level = (LTEV2XHALLogLevel ) g_smem_mib.slib_log_lv;
	if (strlen(g_smem_mib.v2x.dev_name) == 0)
	{
		memcpy(g_smem_mib.v2x.dev_name, "/dev/spidev1.1", sizeof("/dev/spidev1.1"));
	}
  // LTEV2X 접속계층 라이브러리 초기화하고 패킷수신콜백함수를 등록한다.
  int ret = LTEV2XHAL_Init(hal_log_level, g_smem_mib.v2x.dev_name);
  if (ret < 0) {
    ERR("Fail to initialize ltev2x-hal library - LTEV2XHAL_Init() failed: %d\n", ret);
    return -1;
  }
	// Dot2 라이브러리 초기화 
	if (g_smem_mib.v2x.config.dot2.enable == true)
	{
		ret = Dot2_Init(hal_log_level, kDot2SigningParamsPrecomputeInterval_Default, NULL, kDot2LeapSeconds_Default);
		if (ret < 0) {
			ERR("Fail to initialize dot2 library - Dot2_Init() failed: %d\n", ret);
			return -2;
		}else{
			LOG(kKVHLOGLevel_Init,"Success to initialize dot2 library\n");
		}
	}
	LTEV2XHAL_RegisterCallbackProcessMSDU(LTEV2X_ProcessRxLTEV2XPkt);

	ret = Dot3_Init(hal_log_level);
  if (ret < 0) {
    ERR("Fail to initialize dot3 library - Dot3_Init() failed: %d\n", ret);
    return -3;
  }else{
		LOG(kKVHLOGLevel_Init, "Success to initialize dot3 library\n");
	}

  /*
   * WSM 수신을 위한 PSID를 등록한다.
   */
	ret = Dot3_AddWSR(SMEM_BSM_MSG_PSID);
  if (ret < 0) {
    ERR("Fail to initialize V2X sidelink function - Dot3_AddWSR(psid: %u) - %d\n", SMEM_BSM_MSG_PSID, ret);
    return -1;
  }
	ret = Dot3_AddWSR(82050); // BSM 추가 등록
  if (ret < 0) {
    ERR("Fail to initialize V2X sidelink function - Dot3_AddWSR(psid: %u) - %d\n", SMEM_BSM_MSG_PSID, ret);
    return -1;
  }
  ret = Dot3_AddWSR(SMEM_RSA_MSG_PSID);
  if (ret < 0) {
    ERR("Fail to initialize V2X sidelink function - Dot3_AddWSR(psid: %u) - %d\n", SMEM_RSA_MSG_PSID, ret);
    return -1;
  }
  ret = Dot3_AddWSR(SMEM_TIM_MSG_PSID);
  if (ret < 0) {
    ERR("Fail to initialize V2X sidelink function - Dot3_AddWSR(psid: %u) - %d\n", SMEM_TIM_MSG_PSID, ret);
    return -1;
  }
  ret = Dot3_AddWSR(SMEM_SPAT_MSG_PSID);
  if (ret < 0) {
    ERR("Fail to initialize V2X sidelink function - Dot3_AddWSR(psid: %u) - %d\n", SMEM_SPAT_MSG_PSID, ret);
    return -1;
  }
  ret = Dot3_AddWSR(SMEM_MAP_MSG_PSID);
  if (ret < 0) {
    ERR("Fail to initialize V2X sidelink function - Dot3_AddWSR(psid: %u) - %d\n", SMEM_MAP_MSG_PSID, ret);
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize V2X library\n");

	g_smem_mib.v2x.config.j2735.rx.BSM_enable = true; // J2735 수신은 항상 활성화
	g_smem_mib.v2x.config.j2735.rx.MAP_enable = true; // J2735 수신은 항상 활성화
	g_smem_mib.v2x.config.j2735.rx.PVD_enable = false; // J2735 수신은 항상 활성화
	g_smem_mib.v2x.config.j2735.rx.RSA_enable = true; // J2735 수신은 항상 활성화
	g_smem_mib.v2x.config.j2735.rx.RTCM_enable = false; // J2735 수신은 항상 활성화
	g_smem_mib.v2x.config.j2735.rx.SPAT_enable = true; // J2735 수신은 항상 활성화
	g_smem_mib.v2x.config.j2735.rx.TIM_enable = true; // J2735 수신은 항상 활성화

if (g_smem_mib.v2x.config.dot2.enable == true) {
		ret = LTEV2X_Dot2InitialSecurity();
		if (ret < 0) {
			ERR("Fail to initialize Dot2 Security Profiles - Dot2 Security Profiles failed: %d\n", ret);
			return -3;
		}else{
			LOG(kKVHLOGLevel_Init, "Success to initialize Dot2 Security Profiles\n");
		}
	}
	
	J29451LogLevel j29451_log_level = (J29451LogLevel)g_smem_mib.slib_log_lv;
	// ToDo:차량크기 정보는 수집정보를 통해 일어 올수 있도록 수정
	J29451VehicleWidth v_width = 100;
	J29451VehicleLength v_length = 200;
	ret = LTEV2X_J2735J29451Inlitial(NULL, &j29451_log_level, NULL, &v_width, &v_length);
	if (ret < 0) {
    ERR("Fail to initialize J2735 J29451 library - LTEV2X_J2735J29451Inlitial() failed: %d\n", ret);
    return -3;
  }else{
		LOG(kKVHLOGLevel_Init, "Success to initialize J2735 J29451 library\n");
	}
	LTEV2XJ273BSMJ29451LibraryConfig *j29451_config = &g_smem_mib.v2x.config.j2735.j29451;
	ret = J29451_StartBSMTransmit(j29451_config->tx_interval);
	if (ret < 0) {
		ERR("Fail to start BSM transmit - J29451_StartBSMTransmit() failed: %d\n", ret);
		return -1;
	}

	if (j29451_config->txparams.msdu.tx_flow_type != kLTEV2XHALTxFlowType_Ad_Hoc) {
		if (LTEV2X_SetLTEV2XRegisterTransmitFlow(j29451_config->txparams.msdu.tx_flow_index, j29451_config->tx_interval, j29451_config->txparams.msdu.priority)) {
			return -1;
		}
	}

  return 0;
}