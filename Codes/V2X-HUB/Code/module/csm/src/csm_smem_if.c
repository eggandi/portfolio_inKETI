/**
 * @file
 * @brief SMEM과의 I/F 기능 구현
 * @date 2024-11-17
 * @author user
 */


// 모듈 헤더파일
#include "csm.h"


/**
 * @brief SMEM으로부터의 UDS 데이터 수신 콜백함수 (kvhuds 내 수신 쓰레드에서 호출된다)
 * @parma[in] h 라이브러리 기능 핸들 (초기화 API에서 리턴된 핸들)
 * @param[in] data 수신 데이터 (본 데이터는 동적할당되었으므로, 어플리케이션은 데이터 처리 후 free() 해 주어야 한다)
 * @param[in] len 수신 데이터 길이
 * @parma[in] priv 어플리케이션 전용 정보
 */
static void CSM_ProcessRxUDSDataFromSMEM(KVHUDSHANDLE h, uint8_t *data, KVHIFDataLen len, void *priv)
{
  LOG(kKVHLOGLevel_Event, "Process rx UDS data from SMEM\n");

  if (!h || !data || (len == 0) || !priv) {
    ERR("Fail to process rx UDS data from SMEM - null (h: %p, data: %p, len: %d, priv: %p)\n", h, data, len, priv);
    return;
  }
  DUMP(kKVHLOGLevel_Dump, "UDS data from SMEM", data, len);

  CSM_MIB *mib = (CSM_MIB *)priv;

  /*
   * 수신된 UDS 데이터를 처리한다 -> C-ITS 서비스 데이터 저장소에 저장한다.
   */
  if (len < sizeof(KVHUDSDataCITSmsgHdr)) {
    ERR("Fail to process rx UDS data from SMEM - too short: %d < %d\n", len, sizeof(KVHUDSDataHdr));
    goto out;
  }
  KVHUDSDataCITSmsgHdr *uds_data_hdr = (KVHUDSDataCITSmsgHdr *)data;
  if (uds_data_hdr->common.dst != kKVHUDSEntityType_V2X_CSM) {
    ERR("Fail to process rx UDS data from SMEM - not destined to me(dst: %d)\n", uds_data_hdr->common.dst);
    goto out;
  }

  /*
   * 수신된 C-ITS 원시 메시지를 처리한다.
   */
  if ((uds_data_hdr->common.type == kKVHUDSDataType_CITS_MSG) &&
      (uds_data_hdr->common.subtype == kKVHUDSDataSubtype_CITS_RAW_MSG)) {
    KVHRetCode rc = KVHCITS_Push(mib->cits.h,
                                 data + sizeof(KVHUDSDataCITSmsgHdr),
                                 uds_data_hdr->common.len,
                                 kKVHCITSSvcDataValidTime_Default);
    if (rc < 0) {
      ERR("Fail to process rx C-ITS raw msg(PSID: %d) from SMEM - KVHCITS_Push() - rc: %d\n", uds_data_hdr->psid, rc);
    } else {
      LOG(kKVHLOGLevel_InOut, "Success to push C-ITS raw msg(PSID: %d) from SMEM\n", uds_data_hdr->psid);
    }
  } else {
    ERR("Fail to process rx UDS data from SMEM - invalid type: %d and subtype %d\n",
        uds_data_hdr->common.type, uds_data_hdr->common.subtype);
  }

  LOG(kKVHLOGLevel_Event, "Success to process rx UDS data from SMEM\n");

out:
  free(data);
}


/**
 * @brief SMEM과의 I/F를 초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int CSM_InitSMEMIF(CSM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize SMEM I/F\n");

  /*
   * UDS 서버 라이브러리를 초기화한다.
   */
  mib->smem_if.uds.h = KVHUDS_InitServer(mib->smem_if.uds.s_uds_file_path,
                                         mib->smem_if.uds.c_uds_file_path,
                                         CSM_ProcessRxUDSDataFromSMEM,
                                         mib);
  if (mib->smem_if.uds.h == NULL) {
    ERR("Fail to initialize SMEM I/F - UDS server(me): %s, UDS client(SMEM): %s - KVHUDS_InitServer()\n",
        mib->smem_if.uds.s_uds_file_path, mib->smem_if.uds.c_uds_file_path);
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize SMEM I/F\n");
  return 0;
}
