Preferences: Open Settings (JSON)/**
 * @file
 * @brief KADaP 클라우드와의 I/F 기능 구현
 * @date 2024-12-01
 * @author user
 */


// 모듈 헤더파일
#include "udmem_kadap.h"


/// 클라우드 업로드 컨텐츠의 가장 앞쪽(json 인코딩 데이터의 앞쪽)에 붙는 데이터
const char *g_udmem_kadap_cloud_upload_contents_prefix = "{\"query\":\"mutation Mutation($input: V2XInput) { postKETI(input: $input) { carid timestamp date_time lat lon elev speed heading accel_lon accel_lat accel_vert yawrate fix net_rtk_fix supported_driving_mode current_driving_mode length width height weight driver_status }}\",\"variables\":{\"input\":";
/// 클라우드 업로드 컨텐츠의 가장 뒤쪽(json 인코딩 데이터의 뒤쪽)에 붙는 데이터
const char *g_udmem_kadap_cloud_upload_contents_suffix = "}}";


/**
 * @brief KADaP 클라우드와의 I/F 초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int UDMEM_KADAP_InitCloudIF(UDMEM_KADAP_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize KADaP cloud I/F\n");

  /*
   * RSDPR을 연다.
   */
  if (UDMEM_KADAP_OpenRSDPR(mib) < 0) {
    return -1;
  }

  /*
   * 상태데이터 업로드 기능을 초기화한다.
   */
  if (UDMEM_KADAP_InitStatusDataCloudUploadFunction(mib) < 0) {
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize KADaP cloud I/F\n");
  return 0;
}
