/**
* @file
* @brief UDMEM의 RSDPR(Realtime Status Data Public Repository) I/F 기능 구현
* @date 2024-12-01
* @author user
*
* @note RSDPR은 시스템 공유메모리 상에 위치한다.
*/


// 시스템 헤더파일

// 의존 헤더파일

// 모듈 헤더파일
#include "udmem_kadap.h"


/**
 * @brief RSDPR(실시간 상태데이터 공용 저장소)을 연다.
 * @param[in] mib 모듈 관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int UDMEM_KADAP_OpenRSDPR(UDMEM_KADAP_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Open RSDPR\n");

  /*
   * RSDPR를 연다.
   */
  mib->rsdpr_if.rsdpr = KVHRSDPR_Open();
  if (!mib->rsdpr_if.rsdpr) {
    ERR("Fail to open RSDPR - KVHRSDPR_Open()\n");
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to open RSDPR\n");
  return 0;
}


/**
 * @brief RSDPR에서 상태데이터를 읽어온다.
 * @param[in] mib 모듈 관리정보
 * @param[out] lf_ads LF ADS 상태데이터가 저장될 변수 포인터
 * @param[out] hf_ads HF ADS 상태데이터가 저장될 변수 포인터
 * @param[out] lf_hub LF Hub 상태데이터가 저장될 변수 포인터
 * @param[out] hf_hub HF Hub 상태데이터가 저장될 변수 포인터
 * @param[out] dms DMS 상태데이터가 저장될 변수 포인터
 * @retval 0: 성공
 * @retval -1: 실패
 */
int UDMEM_KADAP_ReadRSDPR(
  UDMEM_KADAP_MIB *mib,
  KVHLFADSData *lf_ads,
  KVHHFADSData *hf_ads,
  KVHLFHubData *lf_hub,
  KVHHFHubData *hf_hub,
  KVHDMSData *dms)
{
  LOG(kKVHLOGLevel_Event, "Read RSDPR\n");

  /*
   * LF ADS 상태데이터를 읽어온다.
   * LF ADS 상태데이터가 업데이트되고 있지 않은 경우(예: ADS 미연결 시스템), 해당 정보는 unknown으로 설정된다.
   */
  if (KVHRSDPR_ReadLFADSData(mib->rsdpr_if.rsdpr, lf_ads) < 0) {
    LOG(kKVHLOGLevel_Event, "Fail to read RSDPR - KVHRSDPR_ReadLFADSData() - It's OK. LF ADS data will be set to unknwon.\n");
    lf_ads->present.system = false;
    lf_ads->present.vehicle = false;
  }

  /*
   * HF ADS 상태데이터를 읽어온다.
   * HF ADS 상태데이터가 업데이트되고 있지 않은 경우(예: ADS 미연결 시스템), 해당 정보는 unknown으로 설정된다.
   */
  if (KVHRSDPR_ReadHFADSData(mib->rsdpr_if.rsdpr, hf_ads) < 0) {
    LOG(kKVHLOGLevel_Event, "Fail to read RSDPR - KVHRSDPR_ReadHFADSData() - It's OK. HF ADS data will be set to unknwon.\n");
    hf_ads->present.ads = false;
    hf_ads->present.gnss = false;
  }

  /*
   * LF Hub 상태데이터를 읽어온다.
   * LF Hub 상태데이터는 반드시 존재해야 한다.
   */
  if (KVHRSDPR_ReadLFHubData(mib->rsdpr_if.rsdpr, lf_hub) < 0) {
    ERR("Fail to read RSDPR - KVHRSDPR_ReadLFHubData()\n");
    return -1;
  }

  /*
   * HF Hub 상태데이터를 읽어온다.
   * HF Hub 상태데이터는 반드시 존재해야 한다.
   */
  if (KVHRSDPR_ReadHFHubData(mib->rsdpr_if.rsdpr, hf_hub) < 0) {
    ERR("Fail to read RSDPR - KVHRSDPR_ReadHFHubData()\n");
    return -1;
  }

  /*
   * DMS 상태데이터를 읽어온다.
   * DMS 상태데이터가 업데이트되고 있지 않은 경우(예: DMS 미연결 시스템), 해당 정보는 unknown으로 설정된다.
   */
  if (KVHRSDPR_ReadDMSData(mib->rsdpr_if.rsdpr, dms) < 0) {
    LOG(kKVHLOGLevel_Event, "Fail to read RSDPR - KVHRSDPR_ReadDMSData() - It's OK. DMS data will be set to unknwon.\n");
    dms->status = KVH_DRIVER_STATUS_NORMAL_UNKNOWN;
  }

  LOG(kKVHLOGLevel_Event, "Success to read RSDPR\n");
  return 0;
}

