/**
 * @file
 * @brief HDEM(HMI Data Exchange Module) 구현
 * @date 2024-10-11
 * @author user
 */


#ifndef V2X_HUB_HDEM_H
#define V2X_HUB_HDEM_H


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
#include "kvhjson.h"
#include "kvhlog.h"
#include "kvhrsdpr.h"
#include "kvhshm.h"
#include "kvhudp.h"
#include "kvhuds.h"
#include "kvhq.h"


/// HMI로의 전송 주기 (마이크로초 주기)
#define HDEM_STATUS_DATA_PKT_TX_INTERVAL_MIN KVH_DATA_UPDATE_INTERVAL_MIN
#define HDEM_STATUS_DATA_PKT_TX_INTERVAL_MAX KVH_DATA_UPDATE_INTERVAL_MAX


/**
 * @brief 교환데이터 목적지
 */
enum eHDEMDataDst
{
  kHDEMDataDst_DGM, ///< DGM(Domain Gateway Module)
  kHDEMDataDst_HMI, ///< HMI
};
typedef long HDEMDataDst; ///< @ref eHDEMDataDst


/**
 * @brief 모듈 통합관리정보(Management Information Base)
 */
typedef struct
{
  char *logfile; ///< 로그 저장 파일경로
  KVHLOGLevel log_lv; ///< 로그 저장 레벨 (기본값 적용 또는 프로그램 실행 인자로 입력된다)

  /// DGM과의 I/F 관리정보
  /// UDS로 데이터를 전송하는 주체는 단일 쓰레드(HMI 상태데이터 UDP 패킷 수신 쓰레드)이므로 송신큐가 필요하지 않다.
  struct {
    KVHUDSINFO uds; ///< UDS 통신기능 관리정보 (DGM과의 UDS 통신 시 사용 - 클라이언트로 동작)
  } dgm_if;

  /// HMI와의 I/F 관리정보
  struct
  {
    char hmi_ip[KVH_IPV4_ADDR_STR_MAX_LEN+1]; ///< HMI IP주소

    /// 상태데이터 교환 관리정보
    /// UDP로 데이터를 전송하는 주체는 단일 쓰레드(상태데이터 패킷 송신타이머)이므로 송신큐가 필요하지 않다.
    struct {
      KVHUDPINFO udp; ///< UDP 교환 (서버로 동작)
      KVHRSDPRHANDLE rsdpr; ///< RSDPR(실시간 상태데이터 공용 저장소) 라이브러리 핸들
      /// 송신 관리정보
      struct {
        int timerfd; ///< 패킷전송 타이머 파일 디스크립터
        pthread_t th; ///< 패킷전송 쓰레드 식별자
        volatile bool th_running; ///< 쓰레드가 동작 중인지 여부
        uint32_t pkt_seq; ///< 전송패킷 순서번호
      } tx;
    } status_data_trx;

    /// C-ITS 메시지(원시 및 요약) 송신 관리정보
    /// UDP로 데이터를 전송하는 주체는 단일 쓰레드(DGM으로부터의 UDS 데이터 수신 쓰레드)이므로 송신큐가 필요하지 않다.
    struct {
      KVHUDPINFO udp; ///< UDP 송신 (클라이언트로 동작)
      uint32_t pkt_seq; ///< 전송패킷 순서번호
    } cits_msg_tx;

    /// 주행전략 메시지 송신 관리정보
    /// UDP로 데이터를 전송하는 주체는 단일 쓰레드(DGM으로부터의 UDS 데이터 수신 쓰레드)이므로 송신큐가 필요하지 않다.
    struct {
      KVHUDPINFO udp; ///< UDP 송신 (클라이언트로 동작)
    } drv_strategy_tx;

    /// 주행제어 메시지(MADM 전용) 교환 관리정보
    /// UDP로 데이터를 전송하는 주체는 단일 쓰레드(DGM으로부터의 UDS 데이터 수신 쓰레드)이므로 송신큐가 필요하지 않다.
    struct {
      KVHUDPINFO udp; ///< UDP 교환 (서버로 동작)
    } drv_ctrl_msg_trx;
  } hmi_if;

  struct {
    struct {
      unsigned int cits_tx; ///< HMI로의 C-ITS 데이터 송신 누적횟수
      unsigned int hf_hub_status_tx; ///< HMI로의 HF Hub 상태데이터 송신 누적횟수
      unsigned int lf_hub_status_tx; ///< HMI로의 LF Hub 상태데이터 송신 누적횟수
      unsigned int hf_ads_status_tx; ///< HMI로의 HF ADS 상태데이터 송신 누적횟수
      unsigned int lf_ads_status_tx; ///< HMI로의 LF ADS 상태데이터 송신 누적횟수
      unsigned int dms_status_tx; ///< HMI로의 DMS 상태데이터 송신 누적횟수
      unsigned int hmi_status_rx; ///< HMI으로부터 상태데이터 수신 누적횟수
    } hmi; ///< HMI와의 데이터 송수신 누적횟수
    struct {
      unsigned int rx; ///< DGM으로부터의 수신 누적횟수
      unsigned int tx; ///< DGM으로의 송신 누적횟수
    } dgm; ///< DGM과의 데이터 송수신 누적횟수
  } trx_cnt; ///< 송수신 누적횟수

} HDEM_MIB;

extern HDEM_MIB g_hdem_mib;


// hdem_dgm_if.c
int HDEM_InitDGMIF(HDEM_MIB *mib);
int HDEM_TransmitUDSDataToDGM(HDEM_MIB *mib, uint8_t *data, KVHIFDataLen len);

// hdem_hmi_if.c
int HDEM_InitHMIIF(HDEM_MIB *mib);
int HDEM_TransmitCITSSvcDataUDPPktToHMI(HDEM_MIB *mib, uint8_t *payload, KVHIFDataLen payload_len);

// hdem_rsdpr.c
int HDEM_OpenRSDPR(HDEM_MIB *mib);

// hdem_egress.c
int HDEM_InitEgressPktTxFunction(HDEM_MIB *mib);

// hdem_status_data_trx.c
int HDEM_InitStatusDataPktTxToHMIFunction(HDEM_MIB *mib);

// hdem_input_params.c
int HDEM_ParsingInputParameters(int argc, char *argv[], HDEM_MIB *mib);

// hdem_json.c
uint8_t *HDEM_ConstructJSONPkt(
  HDEM_MIB *mib,
  KVHJSONPktDataType pkt_type,
  uint8_t *data,
  KVHIFDataLen len,
  KVHIFDataLen *pkt_len);

// hdem_process_data.c
int HDEM_ProcessData(HDEM_MIB *mib, uint8_t *data, KVHIFDataLen len, HDEMDataDst dst);

// hdem_q.c
int HDEM_InitQueue(HDEM_MIB *mib);
int HDEM_PushQueue(HDEM_MIB *mib, HDEMDataDst dst, uint8_t *data, size_t len);

// hdem_status_data_rdpr.c
int HDEM_OpenStatusDataRDPR(HDEM_MIB *mib);
int HDEM_ReadStatusDataRDPR_HFADSData(HDEM_MIB *mib, KVHHFADSData *data);

// hdem_udp.c
int HDEM_InitUDPServer(HDEM_MIB *mib, uint16_t port);
int HDEM_TransmitUDPData(HDEM_MIB *mib, uint8_t *data, KVHIFDataLen len);
int HDEM_InitHMITransmitFunction(HDEM_MIB *mib);

// hdem_uds.c
int HDEM_InitUDSClient(HDEM_MIB *mib, char *s_uds_file_path, char *c_uds_file_path);
void HDEM_TransmitUDSData(HDEM_MIB *mib, uint8_t *data, KVHIFDataLen len);


#endif //V2X_HUB_HDEM_H
