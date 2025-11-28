/**
* @file
* @brief HDEM의 RSDPR(Realtime Status Data Public Repository) 기능 구현
* @date 2024-11-17
* @author user
*
* @note RSDPR은 시스템 공유메모리 상에 위치한다.
*/


// 시스템 헤더파일

// 의존 헤더파일

// 모듈 헤더파일
#include "hdem.h"


/**
 * @brief RSDPR(실시간 상태데이터 공용 저장소)을 연다.
 * @param[in] mib 모듈 관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int HDEM_OpenRSDPR(HDEM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Open RSDPR\n");

  /*
   * RSDPR를 연다.
   */
  mib->hmi_if.status_data_trx.rsdpr = KVHRSDPR_Open();
  if (!mib->hmi_if.status_data_trx.rsdpr) {
    ERR("Fail to open RSDPR - KVHRSDPR_Open()\n");
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to open RSDPR\n");
  return 0;
}


/**
 * @brief RSDPR에서 HF ADS 상태데이터를 읽어온다.
 * @param[in] mib 모듈 관리정보
 * @param[out] data HF ADS 데이터가 저장될 구조체 포인터
 * @retval 0: 성공
 * @retval -1: 실패
 */
int HDEM_ReadRSDPR_HFADSData(HDEM_MIB *mib, KVHHFADSData *data)
{
  LOG(kKVHLOGLevel_Event, "Read RSDPR - HF ADS data\n");
  if (KVHRSDPR_ReadHFADSData(mib->hmi_if.status_data_trx.rsdpr, data) < 0) {
    ERR("Fail to read RSDPR - HF ADS data\n");
    return -1;
  }
  return 0;
}


/**
 * @brief RSDPR에서 LF ADS 상태데이터를 읽어온다.
 * @param[in] mib 모듈 관리정보
 * @param[out] data LF ADS 데이터가 저장될 구조체 포인터
 * @retval 0: 성공
 * @retval -1: 실패
 */
int HDEM_ReadRSDPR_LFADSData(HDEM_MIB *mib, KVHLFADSData *data)
{
  LOG(kKVHLOGLevel_Event, "Read RSDPR - LF ADS data\n");
  if (KVHRSDPR_ReadLFADSData(mib->hmi_if.status_data_trx.rsdpr, data) < 0) {
    ERR("Fail to RSDPR - LF ADS data\n");
    return -1;
  }
  return 0;
}


/**
 * @brief RSDPR에서 HF Hub 상태데이터를 읽어온다.
 * @param[in] mib 모듈 관리정보
 * @param[out] data HF Hub 데이터가 저장될 구조체 포인터
 * @retval 0: 성공
 * @retval -1: 실패
 */
int HDEM_ReadRSDPR_HFHubData(HDEM_MIB *mib, KVHHFHubData *data)
{
  LOG(kKVHLOGLevel_Event, "Read RSDPR - HF Hub data\n");
  if (KVHRSDPR_ReadHFHubData(mib->hmi_if.status_data_trx.rsdpr, data) < 0) {
    ERR("Fail to read RSDPR - HF Hub data\n");
    return -1;
  }
  return 0;
}

