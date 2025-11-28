/**
 * @file
 * @brief ADS와의 I/F 기능 구현
 * @date 2024-11-17
 * @author user
 */


// 모듈 헤더파일
#include "smem.h"


/**
 * @brief 수신된 DSRC 페이로드를 처리한다.
 * @param[in] psid 수신 메시지 PSID
 * @param[in] payload 페이로드
 * @param[in] payload_len 페이로드 길이
 */
void SMEM_ProcessRxDSRCPayload(uint32_t psid, uint8_t *payload, size_t payload_len)
{
  LOG(kKVHLOGLevel_Event, "Process rx DSRC payload(len: %zu)\n", payload_len);

  /*
   * 전달할 UDS 데이터를 생성한다.
   */
  size_t uds_data_len = sizeof(KVHUDSDataCITSmsgHdr) + payload_len;
  uint8_t *uds_data = (uint8_t *)malloc(uds_data_len);
  if (!uds_data) {
    ERR("Fail to process rx DSRC payload - malloc()\n");
    return;
  }
  KVHUDSDataCITSmsgHdr *hdr = (KVHUDSDataCITSmsgHdr *)uds_data;
  hdr->common.stx = KVH_UDS_DATA_STX;
  hdr->common.type = kKVHUDSDataType_CITS_MSG;
  hdr->common.subtype = kKVHUDSDataSubtype_CITS_RAW_MSG;
  hdr->common.len = payload_len;
  hdr->psid = psid;
  memcpy(uds_data + sizeof(KVHUDSDataCITSmsgHdr), payload, payload_len);

  SMEM_MIB *mib = &(g_smem_mib);

  /*
   * UDS 데이터를 DGM을 통해 ADEM으로 전송한다.
   */
  hdr->common.dst = kKVHUDSEntityType_IVN_ADEM;
  mib->cnt.adem_tx++;
  if ((mib->cnt.adem_tx % 10) == 1) {
    LOG(kKVHLOGLevel_InOut, "Transmit C-ITS raw msg UDS data to ADEM(via DGM) - cnt: %u\n", mib->cnt.adem_tx);
  } else {
    LOG(kKVHLOGLevel_Event, "Transmit C-ITS raw msg UDS data to ADEM(via DGM) - cnt: %u\n", mib->cnt.adem_tx);
  }
  DUMP(kKVHLOGLevel_Dump, "UDS DATA", uds_data, uds_data_len);
  SMEM_TransmitUDSDataToDGM(mib, uds_data, uds_data_len);

  /*
   * UDS 데이터를 CSM으로 전송한다.
   */
  hdr->common.dst = kKVHUDSEntityType_V2X_CSM;
  mib->cnt.csm_tx++;
  if ((mib->cnt.csm_tx % 10) == 1) {
    LOG(kKVHLOGLevel_InOut, "Transmit C-ITS raw msg UDS data to CSM - cnt: %u\n", mib->cnt.csm_tx);
  } else {
    LOG(kKVHLOGLevel_Event, "Transmit C-ITS raw msg UDS data to CSM - cnt: %u\n", mib->cnt.csm_tx);
  }
  DUMP(kKVHLOGLevel_Dump, "UDS DATA", uds_data, uds_data_len);
  SMEM_TransmitUDSDataToCSM(mib, uds_data, uds_data_len);

  free(uds_data);
}


/**
 * @brief SPDU 처리 콜백함수. dot2 라이브러리에서 호출된다.
 * @param[in] result 처리결과
 * @param[in] priv 패킷파싱데이터
 */
static void SMEM_ProcessSPDUCallback(Dot2ResultCode result, void *priv)
{
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
    LOG(kKVHLOGLevel_Event, "Success to process SPDU(content_type: %u). Payload size is %u\n",
        dot2_parsed->content_type, parsed->ssdu_size);
  }

  /*
   * 페이로드를 처리한다. (SAE J2735 or KS R 1600)
   */
  SMEM_ProcessRxDSRCPayload(parsed->mac_wsm.wsm.psid, parsed->ssdu, parsed->ssdu_size);
  V2X_FreePacketParseData(parsed);
}


/**
 * @brief DSRC MPDU pkt 수신시 호출되는 콜백함수 (wlanaccess 라이브러리 내 수신 쓰레드에서 호출한다)
 * @param[in] mpdu 수신된 MPDU
 * @param[in] mpdu_size 수신된 MPDU의 크기
 * @param[in] rx_params 수신 파라미터 정보
 */
static void SMEM_ProcessRxDSRCPkt(const uint8_t *mpdu, WalMPDUSize mpdu_size, const struct WalMPDURxParams *rx_params)
{
  SMEM_MIB *mib = &(g_smem_mib);

  mib->cnt.v2x_rx++;
  if ((mib->cnt.v2x_rx % 10) == 1) {
    LOG(kKVHLOGLevel_InOut, "Process rx DSRC pkt - cnt: %u\n", mib->cnt.v2x_rx);
  } else {
    LOG(kKVHLOGLevel_Event, "Process rx DSRC pkt - cnt: %u\n", mib->cnt.v2x_rx);
  }
  DUMP(kKVHLOGLevel_Dump, "DSRC PKT", (uint8_t *)mpdu, mpdu_size);

  /*
   * 패킷파싱데이터를 할당한다.
   */
  struct V2XPacketParseData *parsed = V2X_AllocateDSRCPacketParseData(mpdu, mpdu_size, rx_params);
  if (parsed == NULL) {
    ERR("Fail to process rx DSRC pkt - V2X_AllocateDSRCPacketParseData() failed\n");
    return;
  }

  /*
   * WSM MPDU를 파싱한다.
   */
  int ret;
  parsed->wsdu = Dot3_ParseWSMMPDU(parsed->pkt,
                                   parsed->pkt_size,
                                   &(parsed->mac_wsm),
                                   &(parsed->wsdu_size),
                                   &(parsed->interested_psid),
                                   &ret);
  if (parsed->wsdu == NULL) {
    ERR("Fail to process rx DSRC pkt - Dot3_ParseWSMMPDU() failed: %d\n", ret);
    V2X_FreePacketParseData(parsed);
    return;
  }

  /*
   * 관심 없는 PSID에 대한 WSM일 경우 로그만 출력하고 종료한다.
   */
  if (parsed->interested_psid == false) {
    LOG(kKVHLOGLevel_Event, "Not interested(PSID:%u) DSRC WSM is received\n", parsed->mac_wsm.wsm.psid);
    V2X_FreePacketParseData(parsed);
    return;
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
    ERR("Fail to process rx DSRC pkt - Dot2_ProcessSPDU() failed: %d\n", ret);
    V2X_FreePacketParseData(parsed);
    return;
  }

  LOG(kKVHLOGLevel_Event, "Success to request SPDU process(psid: %u)\n", parsed->mac_wsm.wsm.psid);
}


/**
 * @brief 수신 메시지 처리를 위한 security profile을 설정한다.
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int SMEM_RegisterV2XSidelinkSecurityProfile(void)
{
  int ret;
  LOG(kKVHLOGLevel_Init, "Register V2X sidelink security profile\n");

  /*
   * RSA용 Security profile을 등록한다
   *  - 수신만 수행하므로 수신 관련 설정만 등록한다.
   *  - 수신 검증을 수행하지 않는다 (임시)
   */
  struct Dot2SecProfile profile;
  memset(&profile, 0, sizeof(profile));
  profile.psid = SMEM_RSA_MSG_PSID;
  profile.tx.sign_type = kDot2SecProfileSign_Compressed;
  profile.tx.ecp_format = kDot2SecProfileEcPointFormat_Compressed;
  profile.rx.verify_data = false;
  ret = Dot2_AddSecProfile(&profile);
  if (ret < 0) {
    ERR("Fail to register V2X sidelink security profile - Dot2_AddSecProfile(PSID: %u) failed: %d\n", profile.psid, ret);
    return -1;
  }

  /*
   * TIM용 Security profile을 등록한다
   *  - 수신만 수행하므로 수신 관련 설정만 등록한다.
   *  - 수신 검증을 수행하지 않는다 (임시)
   */
  memset(&profile, 0, sizeof(profile));
  profile.psid = SMEM_TIM_MSG_PSID;
  profile.tx.sign_type = kDot2SecProfileSign_Compressed;
  profile.tx.ecp_format = kDot2SecProfileEcPointFormat_Compressed;
  profile.rx.verify_data = false;
  ret = Dot2_AddSecProfile(&profile);
  if (ret < 0) {
    ERR("Fail to register V2X sidelink security profile - Dot2_AddSecProfile(PSID: %u) failed: %d\n", profile.psid, ret);
    return -1;
  }

  /*
   * SPAT용 Security profile을 등록한다
   *  - 수신만 수행하므로 수신 관련 설정만 등록한다.
   *  - 수신 검증을 수행하지 않는다 (임시)
   */
  memset(&profile, 0, sizeof(profile));
  profile.psid = SMEM_SPAT_MSG_PSID;
  profile.tx.sign_type = kDot2SecProfileSign_Compressed;
  profile.tx.ecp_format = kDot2SecProfileEcPointFormat_Compressed;
  profile.rx.verify_data = false;
  ret = Dot2_AddSecProfile(&profile);
  if (ret < 0) {
    ERR("Fail to register V2X sidelink security profile - Dot2_AddSecProfile(PSID: %u) failed: %d\n", profile.psid, ret);
    return -1;
  }

  /*
   * MapData용 Security profile을 등록한다.
   *  - 수신만 수행하므로 수신 관련 설정만 등록한다.
   *  - 수신 검증을 수행하지 않는다 (임시)
   */
  memset(&profile, 0, sizeof(profile));
  profile.psid = SMEM_MAP_MSG_PSID;
  profile.tx.sign_type = kDot2SecProfileSign_Compressed;
  profile.tx.ecp_format = kDot2SecProfileEcPointFormat_Compressed;
  profile.rx.verify_data = false;
  ret = Dot2_AddSecProfile(&profile);
  if (ret < 0) {
    ERR("Fail to register V2X sidelink security profile - Dot2_AddSecProfile(PSID: %u) failed: %d\n", profile.psid, ret);
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to register V2X sidelink security profile\n");
  return 0;
}


/**
 * @brief V2X Sidelink 기능을초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int SMEM_InitV2XSidelinkFunction(SMEM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize V2X sidelink function\n");
  int ret;

  /*
   * 무선랜 접속계층 라이브러리를 오픈하고 패킷수신콜백함수를 등록한다.
   */
  ret = WAL_Open(mib->slib_log_lv);
  if (ret < 0) {
    ERR("Fail to initialize V2X sidelink function - WAL_Open() failed: %d\n", ret);
    return -1;
  }
  WAL_RegisterCallbackRxMPDU(SMEM_ProcessRxDSRCPkt);

  /*
   * dot2 라이브러리를 초기화하고 메시지처리 콜백함수를 등록한다.
   * 하드웨어 기반 보안 기능을 사용하는 경우, WAL_Open() API 호출 이후에 호출해야 한다.
   */
  ret = Dot2_Init(mib->slib_log_lv, kDot2SigningParamsPrecomputeInterval_Default, "/dev/random", kDot2LeapSeconds_Default);
  if (ret < 0) {
    ERR("Fail to initialize V2X sidelink function - Dot2_Init() failed: %d\n", ret);
    return -1;
  }
  Dot2_RegisterProcessSPDUCallback(SMEM_ProcessSPDUCallback);

  /*
   * dot3 라이브러리를 초기화한다.
   */
  ret = Dot3_Init(mib->slib_log_lv);
  if (ret < 0) {
    ERR("Fail to initialize V2X sidelink function - Dot3_Init() failed: %d\n", ret);
    return -1;
  }

  /*
   * WSM 수신을 위한 PSID를 등록한다.
   */
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

  LOG(kKVHLOGLevel_Init, "Success to initialize V2X sidelink function\n");
  return 0;
}


/**
 * @brief V2X sidelink I/F를 초기화한다.
 * @param[in] mib 모듈 관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int SMEM_InitSidelinkIF(SMEM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize V2X sidelink I/F\n");

  /*
   * V2X sidelink 기능을 초기화한다.
   */
  if (SMEM_InitV2XSidelinkFunction(mib) < 0) {
    return -1;
  }

  /*
   * 수신 메시지 처리를 위한 security profile을 등록한다.
   */
  if (SMEM_RegisterV2XSidelinkSecurityProfile() < 0) {
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize sidelink I/F\n");
  return 0;
}
