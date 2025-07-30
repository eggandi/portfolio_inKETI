/**
 * @file
 * @brief KADaP 클라우드와의 UDMEM(Uplink/Downlink Message Exchange Module) 기능 구현
 * @date 2024-12-01
 * @author user
 */


#ifndef V2X_HUB_UDMEM_KADAP_H
#define V2X_HUB_UDMEM_KADAP_H


// 시스템 헤더파일
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

// 공용 헤더파일
#include "kvh_common.h"
#include "sudo_queue.h"
#include "ext_data_fmt/kvh_ext_data_fmt.h"
#include "internal_data_fmt/kvh_internal_data_fmt_uds.h"

// 공용 라이브러리 헤더파일
#include "kvhlog.h"
#include "kvhrsdpr.h"
#include "kvhshm.h"
#include "kvhudp.h"
#include "kvhuds.h"
#include "kvhq.h"


/// 상태데이터의 업로드 주기 (마이크로초 주기)
#define UDMEM_KADAP_STATUS_DATA_UPLOAD_INTERVAL (1000000)

/// KADaP 클라우드 서버 접속정보
#define UDMEM_KADAP_CLOUD_SERVER_IP_ADDR "211.199.20.7"
#define UDMEM_KADAP_CLOUD_SERVER_PORT (4906)

/// date_time 업로드 데이터 문자열 길이
#define UDMEM_KADAP_DATE_TIME_STR_LEN (23)


/**
 * @brief 모듈 통합관리정보(Management Information Base)
 */
typedef struct
{
  char *logfile; ///< 로그 저장 파일경로
  KVHLOGLevel log_lv; ///< 로그 저장 레벨 (기본값 적용 또는 프로그램 실행 인자로 입력된다)

  /// 클라우드와의 I/F 관리정보
  struct
  {
    /// 상태데이터 업로드 관리정보
    /// 업로드 전송하는 주체는 단일 쓰레드(상태데이터 업로드 타이머)이므로 송신큐가 필요하지 않다.
    struct {
      char server_ip[KVH_IPV4_ADDR_STR_MAX_LEN]; ///< 클라우드 서버 IP주소
      uint16_t server_port; ///< 클라우드 서버 포트번호
      long interval; ///< 데이터 업로드 주기(마이크로초 단위)
      struct {
        int timerfd; ///< 업로드 타이머 파일 디스크립터
        pthread_t th; ///< 업로드 쓰레드 식별자
        volatile bool th_running; ///< 쓰레드가 동작 중인지 여부
      } tx;
    } status_data_upload;
  } cloud_if;

  /// RSDPR과의 I/F 관리정보
  struct {
    KVHRSDPRHANDLE rsdpr; ///< RSDPR(실시간 상태데이터 공용 저장소) 라이브러리 핸들
  } rsdpr_if;

} UDMEM_KADAP_MIB;

extern UDMEM_KADAP_MIB g_udmem_kadap_mib;
extern const char *g_udmem_kadap_cloud_upload_contents_prefix;
extern const char *g_udmem_kadap_cloud_upload_contents_suffix;


int UDMEM_KADAP_ParsingInputParameters(int argc, char *argv[], UDMEM_KADAP_MIB *mib);

// cloud_if
int UDMEM_KADAP_InitCloudIF(UDMEM_KADAP_MIB *mib);
int UDMEM_KADAP_InitStatusDataCloudUploadFunction(UDMEM_KADAP_MIB *mib);
int UDMEM_KADAP_HTTP_POST(const char *url, const char *contents);

// rsdpr_if
int UDMEM_KADAP_OpenRSDPR(UDMEM_KADAP_MIB *mib);
int UDMEM_KADAP_ReadRSDPR(UDMEM_KADAP_MIB *mib, KVHLFADSData *lf_ads, KVHHFADSData *hf_ads, KVHLFHubData *lf_hub, KVHHFHubData *hf_hub, KVHDMSData *dms);
int HDEM_ReadRSDPR_LFADSData(UDMEM_KADAP_MIB *mib, KVHLFADSData *data);
int HDEM_ReadRSDPR_HFADSData(UDMEM_KADAP_MIB *mib, KVHHFADSData *data);
int HDEM_ReadRSDPR_LFHubData(UDMEM_KADAP_MIB *mib, KVHLFHubData *data);
int HDEM_ReadRSDPR_HFHubData(UDMEM_KADAP_MIB *mib, KVHHFHubData *data);


#endif //V2X_HUB_UDMEM_KADAP_H
