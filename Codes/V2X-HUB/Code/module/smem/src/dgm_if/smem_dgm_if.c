/**
 * @file
 * @brief DGM과의 I/F 기능(=UDS 통신) 구현
 * @date 2024-10-13
 * @author user
 */


// 모듈 헤더파일
#include "smem.h"


/**
 * @brief DGM과의 인터페이스 기능을 초기화한다. (DGM은 UDS 서버, SMEM은 UDS 클라이언트로 동작한다)
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int SMEM_InitDGMIF(SMEM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize DGM I/F - UDS server(DGM): %s, UDS client(me): %s\n",
      mib->dgm_if.uds.s_uds_file_path, mib->dgm_if.uds.c_uds_file_path);

  /*
   * UDS 클라이언트 라이브러리를 초기화한다.
   */
  mib->dgm_if.uds.h = KVHUDS_InitClient(mib->dgm_if.uds.s_uds_file_path,
                                        mib->dgm_if.uds.c_uds_file_path,
                                        NULL, /* SMEM는 DGM으로부터 UDS 데이터를 수신하지는 않는다 */
                                        mib);
  if (mib->dgm_if.uds.h == NULL) {
    ERR("Fail to initialize DGM I/F - UDS server(DGM): %s, UDS client(me): %s\n",
        mib->dgm_if.uds.s_uds_file_path, mib->dgm_if.uds.c_uds_file_path);
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize DGM I/F\n");
  return 0;
}


/**
 * @brief DGM으로 UDS를 통해 데이터를 전송한다.
 * @param[in] mib 모듈 통합관리정보
 * @param[in] data 전송할 데이터
 * @param[in] len 전송할 데이터의 길이
 * @retval 0: 성공
 * @retval -1: 실패
 *
 * @note 전달되는 데이터는 Sidelink 메시지(SAE J2735, KS R1600)이다.
 */
int SMEM_TransmitUDSDataToDGM(SMEM_MIB *mib, uint8_t *data, KVHIFDataLen len)
{
  LOG(kKVHLOGLevel_Event, "Transmit UDS data(len: %d) to DGM\n", len);
  if (len > kKVHIFDataLen_Max) {
    ERR("Fail to transmit UDS data(len: %d) to DGM - too long: %d > %d\n", len, kKVHIFDataLen_Max);
    return -1;
  }
  if (KVHUDS_Send(mib->dgm_if.uds.h, data, len) < 0) {
    ERR("Fail to transmit UDS data(len: %d) to DGM - KVHUDS_Send()\n", len);
    return -1;
  }
  return 0;
}
