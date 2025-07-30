/**
 * @file
 * @brief DGM과의 I/F 기능(=UDS 통신) 구현
 * @date 2025-07-29
 * @author user
 */


// 모듈 헤더파일
#include "ivdctdcm.h"


/**
 * @brief DGM으로 UDS를 통해 데이터를 전송한다.
 * @param[in] mib 모듈 통합관리정보
 * @param[in] data 전송할 데이터
 * @param[in] len 전송할 데이터의 길이
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int IVDCTDCM_TransmitUDSDataToDGM(IVDCTDCM_MIB *mib, uint8_t *data, KVHIFDataLen len)
{
  LOG(kKVHLOGLevel_Event, "Transmit UDS data(len: %d) to DGM\n", len);

  if (len > kKVHIFDataLen_Max) {
    ERR("Fail to transmit UDS data to DGM - too long: %u > %u\n", len, kKVHIFDataLen_Max);
    return -1;
  }

  if (0) { //KVHUDS_Send(mib->dgm_if.uds.h, data, len) < 0) {
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
int IVDCTDCM_InitDGMIF(IVDCTDCM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize DGM I/F - UDS server(DGM): %s, UDS client(me): %s\n",
      mib->dgm_if.uds.s_uds_file_path, mib->dgm_if.uds.c_uds_file_path);

  /*
   * UDS 클라이언트 라이브러리를 초기화한다.
   */
  /*ib->dgm_if.uds.h = KVHUDS_InitClient(mib->dgm_if.uds.s_uds_file_path,
                                        mib->dgm_if.uds.c_uds_file_path,
                                        NULL, // DGM으로부터 수신하는 데이터가 없으므로 콜백함수는 지정하지 않는다.
                                        mib);
																				*/
  if (mib->dgm_if.uds.h == NULL) {
    ERR("Fail to initialize DGM I/F - UDS server(DGM): %s, UDS client(me): %s\n",
        mib->dgm_if.uds.s_uds_file_path, mib->dgm_if.uds.c_uds_file_path);
    return -1;
  } else {
		mib->dgm_if.uds_init = true; // UDS 초기화 완료
		LOG(kKVHLOGLevel_Init, "Success to initialize DGM I/F - UDS server(DGM): %s, UDS client(me): %s\n",
				mib->dgm_if.uds.s_uds_file_path, mib->dgm_if.uds.c_uds_file_path);
	}


  LOG(kKVHLOGLevel_Init, "Success to initialize DGM I/F\n");
  return 0;
}


/**
 * @brief 운전자 상태정보를 DGM으로 전송한다.
 * @param[in] mib 모듈 관리정보
 * @param[in] status 운전자 상태정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int IVDCTDCM_TransmitIvdctDataToDGM(IVDCTDCM_MIB *mib, KVHIVDCTData *ivdct_data)
{
  mib->io_cnt.dgm_tx++;
  KVHLOGLevel lv = ((mib->io_cnt.dgm_tx % 10) == 1) ?  kKVHLOGLevel_InOut : kKVHLOGLevel_Event;
  LOG(lv, "Transmit driver status to DGM - ivdct_data(0x%02X), cnt: %u\n", ivdct_data, mib->io_cnt.dgm_tx);

  /*
   * UDS 패킷을 생성하여 전송한다.
   */
  KVHUDSData uds_data;
  size_t uds_data_len = sizeof(KVHUDSDataHdr);
  uds_data.hdr.dst = kKVHUDSEntityType_CORE_SDMM;
  uds_data.hdr.type = kKVHUDSDataType_STATUS_DATA;
  uds_data.hdr.subtype = kKVHUDSDataSubtype_IVDCT;
  uds_data.hdr.len = sizeof(KVHIVDCTData);
  memcpy(&uds_data.u.ivdct, ivdct_data, sizeof(KVHIVDCTData));
  uds_data_len += sizeof(KVHIVDCTData);
  int ret = IVDCTDCM_TransmitUDSDataToDGM(mib, (uint8_t *)&uds_data, uds_data_len);
  if (ret < 0) {
    ERR("Fail to transmit driver status to DGM - IVDCTDCM_TransmitUDSDataToDGM()\n");
    return -1;
  }

  LOG(kKVHLOGLevel_Event, "Success to transmit driver status to DGM\n");
  return 0;
}
