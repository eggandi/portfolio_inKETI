/**
 * @file
 * @brief HMI와의 I/F 기능 구현
 * @date 2024-11-17
 * @author user
 */


// 모듈 헤더파일
#include "hdem.h"


/**
 * @brief 수신된 HMI 상태데이터를 처리한다.
 * @param[in] mib 모듈 통합관리정보
 * @param[in] data 처리할 데이터
 * @param[in] len 처리할 데이터의 길이
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int HDEM_ProcessHMIStatusData(HDEM_MIB *mib, uint8_t *data, KVHIFDataLen len)
{
  /*
   * 수신된 데이터를 처리한다. (JSON 파싱하여 내부메모리 형식으로 변환)
   */
  KVHHMIData hmi_data;
  if (KVHJSON_DecodeHMIData((const char *)data, len, &hmi_data) < 0) {
    ERR("Fail to process HMI status data - KVHJSON_DecodeHMIData()\n");
    return -1;
  }

  /*
   * UDS 데이터를 생성한다.
   */
  KVHUDSData uds_data;
  KVHIFDataLen uds_data_len = sizeof(KVHUDSDataHdr);
  uds_data.hdr.stx = KVH_UDS_DATA_STX;
  uds_data.hdr.dst = kKVHUDSEntityType_CORE_SDMM;
  uds_data.hdr.type = kKVHUDSDataType_STATUS_DATA;
  uds_data.hdr.subtype = kKVHUDSDataSubtype_HMI;
  uds_data.hdr.len = sizeof(KVHHMIData);
  memcpy(&(uds_data.u.hmi), &hmi_data, sizeof(KVHHMIData));
  uds_data_len += sizeof(KVHHMIData);

  mib->trx_cnt.hmi.hmi_status_rx++;
  LOG(kKVHLOGLevel_InOut, "Process rx HMI status data UDP pkt(len: %d) - cnt: %u\n", len, mib->trx_cnt.hmi.hmi_status_rx);

  /*
   * UDS 데이터를 DGM으로 전송한다.
   */
  HDEM_TransmitUDSDataToDGM(mib, (uint8_t *)&uds_data, (KVHIFDataLen)uds_data_len);

  return 0;
}


/**
 * @brief HMI로부터 상태데이터 UDP 패킷을 수신하면 호출되는 콜백함수 (kvshudp 내 수신 쓰레드에서 호출된다)
 * @parma[in] h UDP 라이브러리 기능 핸들 (초기화 API에서 리턴된 핸들)
 * @param[in] data 수신 데이터 (본 데이터는 동적할당되었으므로, 어플리케이션은 데이터 처리 후 free() 해 주어야 한다)
 * @param[in] len 수신 데이터 길이
 * @parma[in] priv 어플리케이션 전용 정보
 */
static void HDEM_ProcessRxHMIStatusUDPPkt(KVHUDPHANDLE h, uint8_t *data, KVHIFDataLen len, void *priv)
{
  LOG(kKVHLOGLevel_Event, "Process rx HMI status data UDP pkt(len: %d)\n", len);

  if (!h || !data || (len == 0) || !priv) {
    ERR("Fail to process rx HMI status data UDP pkt - null (h: %p, data: %p, len: %d, priv: %p)\n", h, data, len, priv);
    return;
  }
  if (len < sizeof(KVHJSONPktHeader)) {
    ERR("Fail to process rx HMI status data UDP pkt - too short %d < %d\n", len, sizeof(KVHJSONPktHeader));
    return;
  }

  uint8_t *payload = data + sizeof(KVHJSONPktHeader);
  KVHIFDataLen payload_len = len - (KVHIFDataLen)(sizeof(KVHJSONPktHeader) - sizeof(KVHJSONPktTail));
  DUMP(kKVHLOGLevel_Dump, "Rx HMI status data UDP pkt", data, len);
  STR(kKVHLOGLevel_Dump, "Rx HMI status data JSON", (char *)payload);

#if 0
  KVHJSONPktHeader *pkt_hdr = (KVHJSONPktHeader *)data;
  // TODO:: 헤더 값 체크 루틴 추가
#endif

  /*
   * 수신된 데이터를 처리한다.
   */
  HDEM_ProcessHMIStatusData((HDEM_MIB *)priv, payload, payload_len);
  free(data);
}


/**
 * @brief HMI와 상태데이터를 교환하는 I/F를 초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int HDEM_InitHMIIF_StatusDataTrx(HDEM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize HMI I/F(Status data trx)\n");

  /*
   * UDP 서버를 초기화한다.
   */
  mib->hmi_if.status_data_trx.udp.h = KVHUDP_InitServer(mib->hmi_if.status_data_trx.udp.server_port,
                                                       HDEM_ProcessRxHMIStatusUDPPkt,
                                                       mib);
  if (mib->hmi_if.status_data_trx.udp.h == NULL) {
    ERR("Fail to initialize HMI I/F(Status data trx) - KVHUDP_InitServer()\n");
    return -1;
  }

  /*
   * RSDPR을 연다.
   */
  if (HDEM_OpenRSDPR(mib) < 0) {
    return -1;
  }

  /*
   * 상태데이터 패킷 송신 기능을 초기화한다.
   */
  if (HDEM_InitStatusDataPktTxToHMIFunction(mib) < 0) {
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize HMI I/F(Status data trx)\n");
  return 0;
}


/**
 * @brief HMI로 C-ITS 서비스데이터를 송신하는 I/F를 초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int HDEM_InitHMIIF_CITSSvcDataTransmit(HDEM_MIB *mib)
{
  (void)mib;
  LOG(kKVHLOGLevel_Init, "Initialize HMI I/F (C-ITS service data tx)\n");

  /*
   * UDP 클라이언트를 초기화한다.
   */
  mib->hmi_if.cits_msg_tx.udp.h = KVHUDP_InitClient(mib->hmi_if.hmi_ip,
                                                    mib->hmi_if.cits_msg_tx.udp.server_port,
                                                    NULL, // HMI로부터 수신되는 데이터는 없으므로 콜백함수를 정의하지 않는다.
                                                    mib);
  if (mib->hmi_if.cits_msg_tx.udp.h == NULL) {
    ERR("Fail to initialize HMI I/F (C-ITS service data tx) - KVHUDP_InitClient()\n");
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize HMI I/F (C-ITS service data tx)\n");
  return 0;
}


/**
 * @brief HMI로 주행전략을 송신하는 I/F를 초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int HDEM_InitHMIIF_DrvStrategyTx(HDEM_MIB *mib)
{
  (void)mib;
  LOG(kKVHLOGLevel_Init, "Initialize HMI I/F(Driving strategy tx)\n");
  // TODO::
  LOG(kKVHLOGLevel_Init, "Success to initialize HMI I/F(Driving strategy tx)\n");
  return 0;
}


/**
 * @brief HMI와 주행제어 메시지를 교환하는 I/F를 초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int HDEM_InitHMIIF_DrvCtrlMsgTrx(HDEM_MIB *mib)
{
  (void)mib;
  LOG(kKVHLOGLevel_Init, "Initialize HMI I/F(Driving ctrl msg trx)\n");
  // TODO::
  LOG(kKVHLOGLevel_Init, "Success to initialize HMI I/F(Driving ctrl msg trx)\n");
  return 0;
}


/**
 * @brief HMI와의 I/F 초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int HDEM_InitHMIIF(HDEM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize HMI I/F\n");

  /*
   * 상태데이터 교환 I/F를 초기화한다.
   */
  if (HDEM_InitHMIIF_StatusDataTrx(mib) < 0) {
    return -1;
  }

  /*
   * C-ITS 서비스 데이터 송신 I/F를 초기화한다.
   */
  if (HDEM_InitHMIIF_CITSSvcDataTransmit(mib) < 0) {
    return -1;
  }

  /*
   * 주행전략 송신 I/F를 초기화한다.
   */
  if (HDEM_InitHMIIF_DrvStrategyTx(mib) < 0) {
    return -1;
  }

  /*
   * 주행제어 메시지 교환 I/F를 초기화한다.
   */
  if (HDEM_InitHMIIF_DrvCtrlMsgTrx(mib) < 0) {
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize HMI I/F\n");
  return 0;
}


/**
 * @brief HMI로 C-ITS 서비스데이터 UDP 패킷을 전송한다.
 * @param[in] mib 모듈 관리정보
 * @param[in] payload UDP 페이로드
 * @param[in] payload_len UDP 페이로드 길이
 */
int HDEM_TransmitCITSSvcDataUDPPktToHMI(HDEM_MIB *mib, uint8_t *payload, KVHIFDataLen payload_len)
{
  mib->trx_cnt.hmi.cits_tx++;
  LOG(kKVHLOGLevel_InOut, "Transmit C-ITS svc data UDP pkt to HMI(len: %zu) - cnt: %u\n",
      payload_len, mib->trx_cnt.hmi.cits_tx);

  /*
   * UDP 패킷을 생성한다.
   */
  size_t pkt_len = sizeof(KVHCITSPktHdr) + payload_len + sizeof(KVHCITSPktTail);
  uint8_t *pkt = malloc(pkt_len);
  if (!pkt) {
    ERR("Fail to transmit C-ITS svc data UDP pkt to HMI(len: %u) - malloc()\n", payload_len);
    return -1;
  }
  KVHCITSPktHdr *hdr = (KVHCITSPktHdr *)pkt;
  hdr->stx = htonl(KVH_CITS_PKT_STX);
  hdr->seq = htonl(mib->hmi_if.cits_msg_tx.pkt_seq++);
  hdr->msg_type = htonl(kKVHCITSPktMsgType_CITSSvcData);
  hdr->payload_len = htonl(payload_len);
  memcpy(pkt + sizeof(KVHCITSPktHdr), payload, payload_len);
  KVHCITSPktTail *tail = (KVHCITSPktTail *)(pkt + sizeof(KVHCITSPktHdr) + payload_len);
  tail->etx = htonl(KVH_CITS_PKT_ETX);
  DUMP(kKVHLOGLevel_Dump, "C-ITS UDP PKT to HMI", pkt, pkt_len);
  STR(kKVHLOGLevel_Dump, "C-ITS JSON to HMI", (char *)payload);

  /*
   * 패킷을 전송한다.
   */
  if (pkt_len > KVH_IF_DATA_MAX_LEN) {
    ERR("Fail to transmit C-ITS svc data UDP pkt to HMI - too long: %u > %u\n", pkt_len, KVH_IF_DATA_MAX_LEN);
    free(pkt);
    return -1;
  }
  if (KVHUDP_Send(mib->hmi_if.cits_msg_tx.udp.h, pkt, pkt_len) < 0) {
    ERR("Fail to transmit C-ITS svc data UDP pkt to HMI(len: %u) - KVHUDP_Send()\n", pkt_len);
    free(pkt);
    return -1;
  }

  LOG(kKVHLOGLevel_Event, "Success to transmit C-ITS svc data UDP pkt to HMI\n");

  free(pkt);
  return 0;
}
