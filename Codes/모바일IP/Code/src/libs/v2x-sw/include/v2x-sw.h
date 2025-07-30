/** 
 * @file
 * @brief v2x-sw에서 공통으로 사용되는 항목들을 정의한 헤더 파일
 * @date 2020-09-05
 * @author gyun
 */


#ifndef V2X_SW_V2X_SW_H
#define V2X_SW_V2X_SW_H


// 시스템 헤더 파일
#include <string.h>

// 라이브러리 헤더 파일
#include "wlanaccess/wlanaccess-api-params.h"
#include "dot3/dot3-api-params.h"
#include "dot2/dot2-api-params.h"


#ifndef MAC_ADDR_FMT
#define MAC_ADDR_FMT "%02X:%02X:%02X:%02X:%02X:%02X" ///< MAC 주소 출력 형식 정의
#endif
#ifndef MAC_ADDR_FMT_ARGS
#define MAC_ADDR_FMT_ARGS(addr) addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]  ///< MAC 주소 출력 인자 정의
#endif


/**
 * @brief 패킷파싱데이터 정보 형식
 *
 * 본 정보는 V2X_AllocatePacketParseData()를 통해 생성되어야 하고, V2X_FreePacketParseData()를 통해 해제되어야 한다.
 */
struct V2XPacketParseData
{
  struct WalMPDURxParams rx_params; ///< MPDU 수신파라미터 정보 (접속계층 라이브러리에 의해 채워진다)
  struct Dot3MACAndWSMParseParams mac_wsm; ///< MAC 및 WSM 헤더 파싱 정보 (dot3 라이브러리에 의해 채워진다)
  struct Dot2SPDUParseData spdu; ///< SPDU 헤더 파싱 정보 (dot2 라이브러리에 의해 채워진다)
  bool interested_psid; ///< 관심있는 PSID인지 여부(=WSR 등록 여부)
  size_t wsm_size; ///< WSM 패킷의 길이
  uint8_t *wsm; ///< mpdu 버퍼 내에서 WSM 패킷의 시작지점을 참조한다.
  size_t wsdu_size; ///< WSDU 길이
  uint8_t *wsdu; ///< WSM Service Data Unit. WSM body에 수납된 데이터(=Ieee1609Dot2Data).
  size_t ssdu_size; ///< SSDU 길이
  uint8_t *ssdu; ///< Secured Service Data Unit. Ieee1609Dot2에 수납된 페이로드(예: J2735 메시지, WSA).
  size_t pkt_size; ///< 패킷 길이
  uint8_t pkt[]; ///< 수신된 패킷 (DSRC의 경우 MPDU, C-V2X의 경우 WSM이 저장된다)
};


/**
 * @brief 패킷파싱데이터 구조체 메모리를 할당하고, 수신된 DSRC MPDU 및 수신정보를 저장한다.
 * @param[in] mpdu 수신된 MPDU
 * @param[in] mpdu_size 수신된 MPDU 길이
 * @param[in] params 접속계층 수신정보 (NULL 가능)
 * @return 할당된 패킷파싱데이터 구조체 포인터
 * @retval NULL: 실패
 */
static inline struct V2XPacketParseData *
V2X_AllocateDSRCPacketParseData(const uint8_t *mpdu, size_t mpdu_size, const struct WalMPDURxParams *params)
{
  struct V2XPacketParseData *data = (struct V2XPacketParseData *)calloc(1, sizeof(struct V2XPacketParseData) + mpdu_size);
  if (data) {
    if (mpdu && (mpdu_size > 0)) {
      memcpy(data->pkt, mpdu, mpdu_size);
      data->pkt_size = mpdu_size;
      data->wsm = data->pkt + MAC_QOS_HLEN + LLC_HLEN;
      data->wsm_size = data->pkt_size - (MAC_QOS_HLEN + LLC_HLEN);
    }
    if (params) {
      memcpy(&(data->rx_params), params, sizeof(struct WalMPDURxParams));
    }
    return data;
  }
  return NULL;
}


/**
 * @brief 패킷파싱데이터 구조체 메모리를 할당하고, 수신된 C-V2X WSM을 저장한다.
 * @param[in] wsm 수신된 WSM
 * @param[in] wsm_size 수신된 WSM 길이
 * @return 할당된 패킷파싱데이터 구조체 포인터
 * @retval NULL: 실패
 */
static inline struct V2XPacketParseData * V2X_AllocateCV2XPacketParseData(const uint8_t *wsm, size_t wsm_size)
{
  struct V2XPacketParseData *data = (struct V2XPacketParseData *)calloc(1, sizeof(struct V2XPacketParseData) + wsm_size);
  if (data) {
    if (wsm && (wsm_size > 0)) {
      memcpy(data->pkt, wsm, wsm_size);
      data->pkt_size = wsm_size;
      data->wsm = data->pkt;
      data->wsm_size = wsm_size;
    }
    return data;
  }
  return NULL;
}


/**
 * @brief 패킷파싱데이터 구조체 메모리를 해제한다.
 * @param[in] data 해제할 패킷파싱데이터 구조체
 */
static inline void V2X_FreePacketParseData(struct V2XPacketParseData *data)
{
  if (data->wsdu) { free(data->wsdu); }
  if (data->ssdu) { free(data->ssdu); }
  free(data);
}


#endif //V2X_SW_V2X_SW_H
