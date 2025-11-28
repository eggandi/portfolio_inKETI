/**
 * @file
 * @brief DGM과의 I/F 기능(=UDS 통신) 구현
 * @date 2024-10-17
 * @author user
 */


// 모듈 헤더파일
#include "hdem.h"


/**
 * @brief UDS 데이터 수신 콜백함수 (kvhuds 내 수신 쓰레드에서 호출된다)
 * @parma[in] h 라이브러리 기능 핸들 (초기화 API에서 리턴된 핸들)
 * @param[in] data 수신 데이터 (본 데이터는 동적할당되었으므로, 어플리케이션은 데이터 처리 후 free() 해 주어야 한다)
 * @param[in] len 수신 데이터 길이
 * @parma[in] priv 어플리케이션 전용 정보
 *
 * @note DGM으로부터 수신되는 UDS 데이터는 다음과 같다.
 *   - C-ITS 서비스 데이터
 */
static void HDEM_ProcessRxUDSDataFromDGM(KVHUDSHANDLE h, uint8_t *data, KVHIFDataLen len, void *priv)
{
  HDEM_MIB *mib = (HDEM_MIB *)priv;
  mib->trx_cnt.dgm.rx++;
  LOG(kKVHLOGLevel_InOut, "Process rx UDS data from DGM(len: %u) - cnt: %u\n", len, mib->trx_cnt.dgm.rx);

  if (!h || !data || (len == 0) || !priv) {
    ERR("Fail to process rx UDS data from DGM - null (h: %p, data: %p, len: %d, priv: %p)\n", h, data, len, priv);
    return;
  }
  if (len < sizeof(KVHUDSDataHdr)) {
    ERR("Fail to process rx UDS data from DGM - too short: %d < %d\n", len, sizeof(KVHUDSDataHdr));
    goto out;
  }

  /*
   * 수신된 UDS data를 처리한다.
   */
  KVHUDSData *uds_data = (KVHUDSData *)data;
  if (uds_data->hdr.dst != kKVHUDSEntityType_IVN_HDEM) {
    ERR("Fail to process rx UDS data from DGM - not destined to me(dst: %d)\n", uds_data->hdr.dst);
    goto out;
  }
  if ((uds_data->hdr.type == kKVHUDSDataType_CITS_MSG) &&
      (uds_data->hdr.subtype == kKVHUDSDataSubtype_CITS_SVC_DATA)) {
    HDEM_TransmitCITSSvcDataUDPPktToHMI((HDEM_MIB *)priv,
                                        data + sizeof(KVHUDSDataCITSSvcDataHdr),
                                        uds_data->hdr.len);
  } else {
    ERR("Fail to process rx UDS data from DGM - invalid type: %d and subtype %d\n",
        uds_data->hdr.type, uds_data->hdr.subtype);
  }

out:
  free(data);
}


/**
 * @brief DGM으로 UDS를 통해 데이터를 전송한다.
 * @param[in] mib 모듈 통합관리정보
 * @param[in] data 전송할 데이터
 * @param[in] len 전송할 데이터의 길이
 * @retval 0: 성공
 * @retval -1: 실패
 */
int HDEM_TransmitUDSDataToDGM(HDEM_MIB *mib, uint8_t *data, KVHIFDataLen len)
{
  mib->trx_cnt.dgm.tx++;
  LOG(kKVHLOGLevel_InOut, "Transmit UDS data(len: %u) to DGM - cnt: %u\n", mib->trx_cnt.dgm.tx);

  if (len > kKVHIFDataLen_Max) {
    ERR("Fail to transmit UDS data to DGM - too long: %u > %u\n", len, kKVHIFDataLen_Max);
    return -1;
  }

  if (KVHUDS_Send(mib->dgm_if.uds.h, data, len) < 0) {
    ERR("Fail to transmit UDS data(len: %d) to DGM - KVHUDS_Send()\n", len);
    return -1;
  }
  return 0;
}


/**
 * @brief DGM과의 인터페이스 기능을 초기화한다. (DGM은 UDS 서버, ADEM은 UDS 클라이언트로 동작한다)
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int HDEM_InitDGMIF(HDEM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize DGM I/F - UDS server(DGM): %s, UDS client(me): %s\n",
      mib->dgm_if.uds.s_uds_file_path, mib->dgm_if.uds.c_uds_file_path);

  /*
   * UDS 클라이언트 라이브러리를 초기화한다.
   */
  mib->dgm_if.uds.h = KVHUDS_InitClient(mib->dgm_if.uds.s_uds_file_path,
                                        mib->dgm_if.uds.c_uds_file_path,
                                        HDEM_ProcessRxUDSDataFromDGM,
                                        mib);
  if (mib->dgm_if.uds.h == NULL) {
    ERR("Fail to initialize DGM I/F - UDS server(DGM): %s, UDS client(me): %s\n",
        mib->dgm_if.uds.s_uds_file_path, mib->dgm_if.uds.c_uds_file_path);
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize DGM I/F\n");
  return 0;
}
