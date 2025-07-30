/**
 * @file
 * @brief dot3 라이브러리의 Open API들을 정의한 헤더 파일
 * @date 2019-06-04
 * @author gyun
 */

#ifndef V2X_LIBDOT3_DOT3_API_H
#define V2X_LIBDOT3_DOT3_API_H


#ifdef __cplusplus
extern "C" {
#endif


// 시스템 헤더 파일
#include <stdlib.h>

// 라이브러리 헤더 파일
#include "dot3-api-params.h"


/**
 * @brief dot3 라이브러리를 초기화한다(상세 내용 API 매뉴얼 참조).
 * @param[in] log_level 로그메시지 출력레벨
 * @retval 0: 성공
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_Init(Dot3LogLevel log_level);


/**
 * @brief dot3 라이브러리를 해제한다(상세 내용 API 매뉴얼 참조).
 */
void Dot3_Release(void);


/**
 * @brief API 결과값에 대한 설명문자열을 반환한다(상세 내용 API 매뉴얼 참조).
 * @param[in] ret API 결과값
 * @return 결과코드에 대한 설명문자열
 */
const char * Dot3_GetResultStr(int ret);


/**
 * @brief 송신하고자 하는 WSM의 생성을 요청한다(상세 내용 API 매뉴얼 참조).
 * @param[in] params WSM 헤더구성정보
 * @param[in] payload WSM에 수납될 페이로드(예: Ieee1609Dot2Data)가 담긴 버퍼
 * @param[in] payload_size payload 버퍼에 담긴 페이로드의 길이
 * @param[out] wsm_size 생성된 WSM의 길이가 반환될 변수 포인터
 * @param[out] err 실패 시 에러코드(-Dot3ResultCode)가 반환될 변수 포인터
 * @return 생성된 WSM
 * @retval NULL: 실패
 */
uint8_t * Dot3_ConstructWSM(
  struct Dot3WSMConstructParams *params,
  const uint8_t *payload,
  Dot3WSMPayloadSize payload_size,
  size_t *wsm_size,
  int *err);


/**
 * @brief 송신하고자 하는 WSM MPDU의 생성을 요청한다(상세 내용 API 매뉴얼 참조).
 * @param[in] params WSM 헤더 및 MAC 헤더 구성정보
 * @param[in] payload WSM에 수납될 페이로드(예: Ieee1609Dot2Data)가 담긴 버퍼
 * @param[in] payload_size payload 버퍼에 담긴 페이로드의 길이
 * @param[out] mpdu_size 생성된 MPDU의 길이가 반환될 변수 포인터
 * @param[out] err 실패 시 에러코드(-Dot3ResultCode)가 반환될 변수 포인터
 * @return 생성된 MPDU
 * @retval NULL: 실패
 */
uint8_t * Dot3_ConstructWSMMPDU(
  struct Dot3MACAndWSMConstructParams *params,
  const uint8_t *payload,
  Dot3WSMPayloadSize payload_size,
  size_t *mpdu_size,
  int *err);


/**
 * @brief 수신된 WSM에 대한 파싱을 요청한다(상세 내용 API 매뉴얼 참조).
 * @param[in] wsm 파싱할 WSM이 담긴 버퍼
 * @param[in] wsm_size wsm 버퍼에 담긴 WSM의 길이
 * @param[out] params WSM 헤더정보가 반환될 구조체 포인터
 * @param[out] payload_size WSM body에 수납된 페이로드의 길이가 반환될 변수 포인터
 * @param[out] wsr_registered WSM에 수납된 PSID가 관심서비스의 PSID인지 여부가 저장될 변수 포인터
 * @param[out] err 실패 시 에러코드(-Dot3ResultCode)가 반환될 변수 포인터
 * @return WSM body에 수납되어 있는 페이로드
 * @retval NULL: WSM body가 비어 있는 경우 또는 실패
 */
uint8_t * Dot3_ParseWSM(
  const uint8_t *wsm,
  Dot3WSMSize wsm_size,
  struct Dot3WSMParseParams *params,
  size_t *payload_size,
  bool *wsr_registered,
  int *err);


/**
 * @brief 수신된 WSM MPDU에 대한 파싱을 요청한다(상세 내용 API 매뉴얼 참조).
 * @param[in] mpdu 파싱할 MPDU가 담긴 버퍼
 * @param[in] mpdu_size mpdu 버퍼에 담긴 WSM의 길이
 * @param[out] params WSM 헤더정보가 반환될 구조체 포인터
 * @param[out] payload_size 반환되는 페이로드의 길이가 저장될 변수 포인터
 * @param[out] wsr_registered WSM에 수납된 PSID가 관심서비스의 PSID인지 여부가 저장될 변수 포인터
 * @param[out] err 실패 시 에러코드(-Dot3ResultCode)가 반환될 변수 포인터
 * @return WSM body에 수납되어 있는 페이로드
 * @retval NULL: WSM body가 비어 있는 경우 또는 실패
 */
uint8_t * Dot3_ParseWSMMPDU(
  const uint8_t *mpdu,
  Dot3MPDUSize mpdu_size,
  struct Dot3MACAndWSMParseParams *params,
  size_t *payload_size,
  bool *wsr_registered,
  int *ret);


/**
 * @brief 생성 가능한 WSM(헤더 포함) 최대허용길이를 설정한다(상세 내용 API 매뉴얼 참조).
 * @param[in] max_len 생성 가능한 WSM의 최대길이(WSM 헤더 포함)
 * @retval 0: 성공
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_SetWSMMaxLength(Dot3WSMSize max_len);


/**
 * @brief WSA 생성을 요청한다(상세 내용 API 매뉴얼 참조).
 * @param[in] params WSA 헤더 및 WRA 구성정보
 * @param[out] wsa_size 생성된 WSA의 길이가 반환될 변수의 포인터
 * @param[out] err 실패 시 에러코드(-Dot3ResultCode)가 반환될 변수 포인터
 * @return 생성된 WSA
 * @retval NULL: 실패
 */
uint8_t * Dot3_ConstructWSA(const struct Dot3ConstructWSAParams *params, size_t *wsa_size, int *err);


/**
 * @brief WSA 파싱을 요청한다(상세 내용 API 매뉴얼 참조).
 * @param[in] wsa 파싱할 WSA가 담긴 버퍼
 * @param[in] wsa_size wsa 버퍼에 담긴 WSA의 길이
 * @param[out] params 파싱된 정보가 저장될 정보구조체의 포인터
 * @retval 0: 성공
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_ParseWSA(const uint8_t *wsa, size_t wsa_size, struct Dot3ParseWSAParams *params);


/**
 * @brief WSA 처리를 요청한다(상세 내용 API 매뉴얼 참조).
 * @param[in] wsa 처리할 WSA가 담긴 버퍼
 * @param[in] wsa_size wsa 버퍼에 담긴 WSA의 길이
 * @param[in] src_mac_addr WSA 송신지 MAC 주소
 * @param[in] wsa_type WSA 유형
 * @param[in] rcpi WSA 수신 세기
 * @param[in] tx_lat WSA 송신지 위도
 * @param[in] tx_lon WSA 송신지 경도
 * @param[in] tx_elev WSA 송신지 고도
 * @param[out] params 파싱된 정보가 저장될 정보구조체의 포인터
 * @retval 0: 성공
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_ProcessWSA(
  const uint8_t *wsa,
  size_t wsa_size,
  const Dot3MACAddress src_mac_addr,
  Dot3WSAType wsa_type,
  Dot3RCPI rcpi,
  Dot3Latitude tx_lat,
  Dot3Longitude tx_lon,
  Dot3Elevation tx_elev,
  struct Dot3ParseWSAParams *params);


/**
 * @brief WSR을 등록한다(상세 내용 API 매뉴얼 참조).
 * @param[in] psid WSR 저장소에 등록할 PSID
 * @retval 1 이상: (WSR 추가 후) 현재 등록되어 있는 WSR의 개수
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_AddWSR(Dot3PSID psid);


/**
 * @brief 등록되어 있는 WSR을 삭제한다(상세 내용 API 매뉴얼 참조).
 * @param[in] psid 삭제할 WSR의 PSID
 * @retval 0 이상: (WSR 삭제 후) 현재 등록되어 있는 WSR의 개수
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_DeleteWSR(Dot3PSID psid);


/**
 * @brief 등록되어 있는 모든 WSR들을 삭제한다(상세 내용 API 매뉴얼 참조).
 */
void Dot3_DeleteAllWSRs(void);


/**
 * @brief 특정 PSID를 갖는 WSR이 등록되어 있는지 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] psid 확인하고자 하는 PSID
 * @retval 0: 성공
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_CheckWSRWithPSID(Dot3PSID psid);


/**
 * @brief 등록되어 있는 WSR의 개수를 확인한다(상세 내용 API 매뉴얼 참조).
 * @return 등록되어 있는 WSR의 개수(0 이상)
 */
Dot3WSRNum Dot3_GetWSRNum(void);


/**
 * @brief PSR을 등록한다(상세 내용 API 매뉴얼 참조).
 * @param[in] PSR 저장소에 등록할 PSR 정보
 * @retval 1 이상: (PSR 추가 후) 현재 등록되어 있는 PSR의 개수
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_AddPSR(const struct Dot3PSR *psr);


/**
 * @brief 특정 PSR의 내용을 변경한다(상세 내용 API 매뉴얼 참조).
 * @param[in] psid 변경하고자 하는 PSR의 PSID
 * @param[in] psc 변경할 PSC
 * @retval 0: 성공
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_ChangePSR(Dot3PSID psid, const char *psc);


/**
 * @brief 특정 PSR을 삭제한다(상세 내용 API 매뉴얼 참조).
 * @param[in] psid 삭제하고자 하는 PSR의 PSID
 * @retval 0 이상: (PSR 삭제 후) 현재 등록되어 있는 PSR의 개수
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_DeletePSR(Dot3PSID psid);


/**
 * @brief 모든 PSR들을 삭제한다(상세 내용 API 매뉴얼 참조).
 */
void Dot3_DeleteAllPSRs(void);


/**
 * @brief 특정 PSID를 갖는 PSR 정보를 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] psid 확인하고자 하는 PSR의 PSID
 * @param[out] psr PSR 정보가 반환될 변수의 포인터
 * @retval 0: 성공
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_GetPSRWithPSID(Dot3PSID psid, struct Dot3PSR *psr);


/**
 * @brief 등록되어 있는 PSR의 개수를 확인한다(상세 내용 API 매뉴얼 참조).
 * @return 등록되어 있는 PSR의 개수
 */
Dot3PSRNum Dot3_GetPSRNum(void);


/**
 * @brief PCI 정보를 설정한다(상세 내용 API 매뉴얼 참조).
 * @param[in] pci 설정할 PCI 정보가 담긴 정보구조체 포인터
 * @retval 0 이상: 테이블에 등록되어 있는 PCI 정보의 개수
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_SetPCI(const struct Dot3PCI *pci);


/**
 * @brief 특정 채널에 관련된 PCI 정보를 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] chan_num 채널 번호
 * @param[out] pci PCI 정보가 반환될 정보구조체 포인터
 * @retval 0: 성공
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_GetPCIWithChannelNumber(Dot3ChannelNumber chan_num, struct Dot3PCI *pci);


/**
 * @brief 등록되어 있는 PCI의 개수를 확인한다(상세 내용 API 매뉴얼 참조).
 * @return 등록되어 있는 PCI의 개수
 */
Dot3PCINum Dot3_GetPCINum(void);


/**
 * @brief USR을 등록한다(상세 내용 API 매뉴얼 참조).
 * @param[in] USR 저장소에 등록할 USR 정보
 * @retval 1 이상: (USR 추가 후) 현재 등록되어 있는 USR의 개수
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_AddUSR(const struct Dot3USR *usr);


/**
 * @brief 특정 USR을 삭제한다(상세 내용 API 매뉴얼 참조).
 * @param[in] psid 삭제하고자 하는 USR의 PSID
 * @retval 0 이상: (USR 삭제 후) 현재 등로되어 있는 USR의 개수
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_DeleteUSR(Dot3PSID psid);


/**
 * @brief 모든 USR들을 삭제한다(상세 내용 API 매뉴얼 참조).
 */
void Dot3_DeleteAllUSRs(void);


/**
 * @brief 특정 PSID를 갖는 USR 정보를 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] psid 확인하고자 하는 USR의 PSID
 * @param[out] usr USR 정보가 반환될 변수의 포인터
 * @retval 0: 성공
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_GetUSRWithPSID(Dot3PSID psid, struct Dot3USR *usr);


/**
 * @brief 등록되어 있는 USR의 개수를 확인한다(상세 내용 API 매뉴얼 참조).
 * @return 등록되어 있는 USR의 개수
 */
Dot3USRNum Dot3_GetUSRNum(void);


/**
 * @brief 모든 UAS 정보(들)을 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[out] err 실패 시 에러코드(-Dot3ResultCode)가 반환될 변수 포인터
 * @return UAS 정보 집합
 * @retval NULL: 실패
 */
struct Dot3UASSet * Dot3_GetAllUASs(int *err);


/**
 * @brief 특정 PSID를 갖는 UAS 정보(들)을 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] psid 확인하고자 하는 UAS의 PSID
 * @param[out] err 실패 시 에러코드(-Dot3ResultCode)가 반환될 변수 포인터
 * @return UAS 정보 집합
 * @retval NULL: 실패
 */
struct Dot3UASSet * Dot3_GetUASsWithPSID(Dot3PSID psid, int *err);


/**
 * @brief 특정 송신지 MAC 주소를 갖는 UAS 정보(들)을 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] addr 확인하고자 하는 UAS의 송신지 MAC 주소
 * @param[out] err 실패 시 에러코드(-Dot3ResultCode)가 반환될 변수 포인터
 * @return UAS 정보 집합
 * @retval NULL: 실패
 */
struct Dot3UASSet * Dot3_GetUASsWithSourceMACAddress(const Dot3MACAddress addr, int *err);


/**
 * @brief 특정 PSID와 송신지 MAC 주소를 갖는 UAS 정보(들)을 확인한다(상세 내용 API 매뉴얼 참조).
 * @param[in] psid 확인하고자 하는 UAS의 PSID
 * @param[in] addr 확인하고자 하는 UAS의 송신지 MAC 주소
 * @param[out] err 실패 시 에러코드(-Dot3ResultCode)가 반환될 변수 포인터
 * @return UAS 정보 집합
 * @retval NULL: 실패
 */
struct Dot3UASSet * Dot3_GetUASsWithPSIDAndSourceMACAddress(Dot3PSID psid, const Dot3MACAddress addr, int *err);


/**
 * @brief 가장 큰 RCPI 값을 갖는 UAS 정보(들)을 반환한다(상세 내용 API 매뉴얼 참조).
 * @param[out] err 실패 시 에러코드(-Dot3ResultCode)가 반환될 변수 포인터
 * @return UAS 정보 집합
 * @retval NULL: 실패
 */
struct Dot3UASSet * Dot3_GetUASsWithMaxRCPI(int *err);


/**
 * @brief 모든 UAS 정보(들)을 삭제한다(상세 내용 API 매뉴얼 참조).
 */
void Dot3_DeleteAllUASs(void);


/**
 * @brief UAS 관리 기능을 시작한다(상세 내용 API 매뉴얼 참조).
 * @param[in] interval UAS 관리 기능이 동작하는 주기
 * @retval 0: 성공
 * @retval 음수(-Dot3ResultCode): 실패
 */
int Dot3_StartUASManagement(Dot3UASManagementInterval interval);


/**
 * @brief UAS 관리 기능을 중지한다(상세 내용 API 매뉴얼 참조).
 */
void Dot3_StopUASManagement(void);


#ifdef __cplusplus
}
#endif


#endif //V2X_LIBDOT3_DOT3_API_H
