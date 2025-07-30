/**
 * @file
 * @brief dot2 라이브러리의 Open API들을 정의한 헤더 파일
 * @date 2020-02-18
 * @author gyun
 */

#ifndef V2X_SW_DOT2_API_H
#define V2X_SW_DOT2_API_H

#ifdef __cplusplus
extern "C" {
#endif

// 라이브러리 헤더 파일
#include "dot2-api-params.h"
#include "v2x-sw.h"


/**
 * @brief dot2 라이브러리를 초기화한다(상세 내용 API 매뉴얼 참조).
 * @param[in] log_level 로그메시지 출력 레벨
 * @param[in] interval 서명파라미터 계산 주기
 * @param[in] rng_dev 난수생성기 이름(예: /dev/random, /dev/urandom).
 *                    NULL을 전달할 경우 난수생성기 대신 소프트웨어 random() 함수를 통해 난수를 생성한다.
 * @param[in] leap_secs 2004-01-01 이후로 적용된 윤초 값
 * @retval 0: 성공
 * @retval 음수(-Dot2ResultCode): 실패
 */
int Dot2_Init(Dot2LogLevel log_level, Dot2SigningParamsPrecomputeInterval interval, const char *rng_dev, Dot2LeapSeconds leap_secs);


/**
 * @brief dot2 라이브러리를 종료한다(상세 내용 API 매뉴얼 참조).
 */
void Dot2_Release(void);


/**
 * @brief 초단위 시스템 시간값을 Time32 값으로 변환한다(상세 내용 API 매뉴얼 참조).
 * @param[in] sec 변환할 시스템시간(초 단위)
 * @return 변환된 Time32 값 (2004년 1월 1일보다 과거일 경우 0)
 *
 * 시스템 시간: 1970-01-01 0시 이후부터의 초 값 (UTC)
 * Time32: 2004-01-01 0시 이후부터의 초값 (TAI)
 */
Dot2Time32 Dot2_ConvertSystemTimeToTime32(time_t sec);


/**
 * @brief Service Certificate Chain에 속한 CA인증서(rca,ica,pca/aca,eca,ra,crlg,ma)를
 *        CA인증서정보 저장소에 추가한다(상세 내용 API 매뉴얼 참조).
 * @param[in] cert 추가할 CA인증서 바이트열
 * @param[in] cert_size 추가할 CA인증서 바이트열의 길이
 * @retval 0: 성공
 * @retval 음수(-Dot2ResultCode): 실패
 *
 * 추가하고자 하는 인증서를 검증할 수 있는 상위인증서가 저장소에 먼저 저장되어 있어야 한다.
 */
int Dot2_AddSCCCert(const uint8_t *cert, Dot2CertSize cert_size);


/**
 * @brief Service Certificate Chain에 속한 CA인증서(rca,ica,pca/aca,eca,ra,crlg,ma) 파일로부터
 *        인증서정보를 추출하여 CA인증서정보 저장소에 추가한다(상세 내용 API 매뉴얼 참조).
 * @param[in] file_path CA인증서 파일경로 (상대경로 or 절대경로)
 * @retval 0: 성공
 * @retval 음수(-Dot2ResultCode): 실패
 *
 * 추가하고자 하는 인증서를 검증할 수 있는 상위인증서가 저장소에 먼저 저장되어 있어야 한다.
 */
int Dot2_LoadSCCCertFile(const char *file_path);


/**
 * @brief CMH 테이블 내 만기된 CMH 정보를 제거한다(상세 내용 API 매뉴얼 참조).
 * @parma[in] exp 기준이 되는 만기시각. 0일 경우 API 내부에서 현재시각으로 설정된다.
 */
void Dot2_RemoveExpiredCMH(Dot2Time64 exp);


/**
 * @brief 유효기간이 만료된 SCC인증서들의 정보를 SCC 인증서정보 저장소에서 제거한다(상세 내용 API 매뉴얼 참조).
 * @parma[in] exp 기준이 되는 만기시각. 0일 경우 API 내부에서 현재시각으로 설정된다.
 */
void Dot2_RemoveExpiredSCCCert(Dot2Time64 exp);


/**
 * @brief 타 장치(EE) 인증서정보 캐싱테이블 내 오래된 인증서정보를 제거한다(상세 내용 API 매뉴얼 참조).
 * @parma[in] exp 기준이 되는 만기시각. 0일 경우 API 내부에서 현재시각으로 설정된다.
 */
void Dot2_RemoveExpiredEECertCache(Dot2Time64 exp);


/**
 * @brief Application 인증서와 개인키재구성정보를 이용하여 CMHF를 생성한다(상세 내용 API 매뉴얼 참조)
 * @param[in] params 생성요청 파라미터
 * @return CMHF 생성 결과
 *
 * LCM을 사용하지 않고, 수동으로 발급받은 인증서번들을 이용하여 CMHF를 생성할 때 사용된다.
 * 반환되는 개인키는 실제로는 사용되지 않고 참고용이다 (실제 서명생성 시에는 CMHF에 포함된 포함된 개인키정보가 사용된다)
 */
struct Dot2CMHFMakeResult Dot2_MakeApplicationCertCMHF(struct Dot2AppCMHFMakeParams *params);


/**
 * @brief Pseudonym 인증서와 개인키재구성정보를 이용하여 CMHF를 생성한다(상세 내용 API 매뉴얼 참조)
 * @param[in] params 생성요청 파라미터
 * @return CMHF 생성 결과
 *
 * LCM을 사용하지 않고, 수동으로 발급받은 인증서번들을 이용하여 CMHF를 생성할 때 사용된다.
 * 반환되는 개인키는 실제로는 사용되지 않고 참고용이다 (실제 서명생성 시에는 CMHF에 포함된 포함된 개인키정보가 사용된다)
 */
struct Dot2PseudonymCMHFMakeResult Dot2_MakePseudonymCertCMHF(struct Dot2PseudonymCMHFMakeParams *params);


/**
 * @brief Identification 인증서와 개인키재구성정보를 이용하여 CMHF를 생성한다(상세 내용 API 매뉴얼 참조).
 * @param[in] params 생성요청 파라미터
 * @return CMHF 생성 결과
 *
 * LCM을 사용하지 않고, 수동으로 발급받은 인증서번들을 이용하여 CMHF를 생성할 때 사용된다.
 * 반환되는 개인키는 실제로는 사용되지 않고 참고용이다 (실제 서명생성 시에는 CMHF에 포함된 포함된 개인키정보가 사용된다)
 */
struct Dot2IdCMHFMakeResult Dot2_MakeIdentificationCertCMHF(struct Dot2IdCMHFMakeParams *params);


/**
 * @brief CMHF 바이트열로부터 정보를 추출하여 CMH 저장소에 추가한다(상세 내용 API 매뉴얼 참조).
 * @param[in] cmhf CMHF 바이트열
 * @param[in] cmhf_size CMHF 바이트열의 길이
 * @retval 0: 성공
 * @retval 음수(-Dot2ResultCode): 실패
 */
int Dot2_LoadCMHF(const uint8_t *cmhf, Dot2CMHFSize cmhf_size);


/**
 * @brief CMHF 파일로부터 정보를 추출하여 CMH 저장소에 추가한다(상세 내용 API 매뉴얼 참조).
 * @param[in] file_path CMHF 파일 경로(상대경로 및 절대경로 모두 가능)
 * @retval 0: 성공
 * @retval 음수(-Dot2ResultCode): 실패
 */
int Dot2_LoadCMHFFile(const char *file_path);


/**
 * @brief 특정 PSID에 대한 Security profile을 등록한다(상세 내용 API 매뉴얼 참조).
 * @param[in] profile 등록할 security profile
 * @retval 0: 성공
 * @retval 음수(-Dot2ResultCode): 실패
 */
int Dot2_AddSecProfile(const struct Dot2SecProfile *profile);


/**
 * @brief SPDU(Secure Protocol Data Unit)의 생성을 요청한다(상세 내용 API 매뉴얼 참조).
 * @param[in] params SPDU 생성을 위한 파라미터
 * @param[in] payload SPDU에 수납될 페이로드(예: WSA, BSM, ...). 페이로드가 없는 SPDU를 생성하고자 할 경우 NULL 전달 가능.
 * @param[in] payload_size 페이로드의 길이. 페이로드가 없는 SPDU를 생성하고자 할 경우 0 전달.
 * @return SPDU 생성 결과
 */
struct Dot2SPDUConstructResult Dot2_ConstructSPDU(struct Dot2SPDUConstructParams *params, const uint8_t *payload, Dot2SPDUSize payload_size);


/**
 * @brief (수신된) SPDU(Secured Protocol Data Unit)의 처리를 요청한다(상세 내용 API 매뉴얼 참조).
 * @param[in] spdu 처리할 SPDU (인코딩된 Ieee1609Dot2Data)
 * @param[in] spdu_size 처리할 SPDU의 길이
 * @param[in] params SPDU 처리를 위한 파라미터
 * @param[in] parsed 패킷파싱데이터 (dot2 파싱정보가 저장된 후 콜백함수를 통해 어플리케이션으로 전달된다)
 * @retval 0: 성공
 * @retval 음수(-Dot2ResultCode): 실패
 */
int Dot2_ProcessSPDU(const uint8_t *spdu, Dot2SPDUSize spdu_size, struct Dot2SPDUProcessParams *params, struct V2XPacketParseData *parsed);


/**
 * @brief Dot2_ProcessSPDU() API 호출을 통해 요청했던 SPDU 처리 결과를 전달받을 콜백함수를 등록한다.
 *        (상세 내용 API 매뉴얼 참조)
 * @param[in] callback 콜백함수 포인터
 */
void Dot2_RegisterProcessSPDUCallback(ProcessSPDUCallback callback);


/**
 * @brief LCM 동작을 위한 설정을 수행한다(상세 내용 API 매뉴얼 참조)
 * @param[in] type 설정 유형
 * @param[in] cfg_str 설정 문자열
 * @retval 0: 성공
 * @retval 음수(-Dot2ResultCode): 실패
 */
int Dot2_ConfigLCM(Dot2LCMConfigType type, const char *cfg_str);


/**
 * @brief 등록인증서(Enrollment certificate) 발급요청문의 생성을 요청한다(상세 내용 API 매뉴얼 참조)
 * @param[in] params 등록인증서 발급요청문의 생성을 위한 파라미터
 * @return 등록인증서 발급요청문 생성 결과
 */
struct Dot2ECRequestConstructResult Dot2_ConstructECRequest(struct Dot2ECRequestConstructParams *params);


/**
 * @brief 등록인증서(Enrollment certificate) 발급응답문의 처리를 요청한다(상세 내용 API 매뉴얼 참조)
 * @param[in] params 등록인증서 발급응답문의 처리를 위한 파라미터
 * @return 등록인증서 발급응답문 처리 결과
 */
struct Dot2ECResponseProcessResult Dot2_ProcessECResponse(struct Dot2ECResponseProcessParams *params);


/**
 * @brief RA에게 응용인증서 발급을 요청한다 (상세 내용 API 매뉴얼 참조)
 * @param[in] params 인증서 발급요청 파라미터
 * @return 응용인증서 발급요청 처리 결과
 */
struct Dot2AppCertProvisioningRequestResult Dot2_RequestAppCertProvisioning(struct Dot2CertProvisioningRequestParams *params);


/**
 * @brief RA에게 익명인증서 발급을 요청한다 (상세 내용 API 매뉴얼 참조)
 * @param[in] params 인증서 발급요청 파라미터
 * @return 익명인증서 발급요청 처리 결과
 */
struct Dot2PseudonymIdCertProvisioningRequestResult Dot2_RequestPseudonymCertProvisioning(struct Dot2CertProvisioningRequestParams *params);


/**
 * @brief RA에게 식별인증서 발급을 요청한다 (상세 내용 API 매뉴얼 참조)
 * @param[in] params 인증서 발급요청 파라미터
 * @return 식별인증서 발급요청 처리 결과
 */
struct Dot2PseudonymIdCertProvisioningRequestResult Dot2_RequestIdCertProvisioning(struct Dot2CertProvisioningRequestParams *params);


/**
 * @brief RA로부터 응용인증서를 다운로드한다 (상세 내용 API 매뉴얼 참조)
 * @param[in] params 응용인증서 다운로드 요청 파라미터
 * @return 응용인증서 다운로드 결과
 */
struct Dot2AppCertDownloadResult Dot2_DownloadAppCert(struct Dot2AppCertDownloadRequestParams *params);


/**
 * @brief RA로부터 익명인증서 1세트를 다운로드한다 (상세 내용 API 매뉴얼 참조)
 * @param[in] params 익명인증서 다운로드 요청 파라미터
 * @return 익명인증서 다운로드 결과
 */
struct Dot2PseudonymCertDownloadResult Dot2_DownloadPseudonymCert(struct Dot2PseudonymIdCertDownloadRequestParams *params);


/**
 * @brief RA로부터 식별인증서를 다운로드한다 (상세 내용 API 매뉴얼 참조)
 * @param[in] params 식별인증서 다운로드 요청 파라미터
 * @return 식별인증서 다운로드 결과
 */
struct Dot2IdCertDownloadResult Dot2_DownloadIdCert(struct Dot2PseudonymIdCertDownloadRequestParams *params);


/**
 * @brief RA로부터 익명/식별인증서 다운로드일정정보를 다운로드한다.(상세 내용 API 매뉴얼 참조)
 * @param[in] params 인증서 다운로드일정정보 다운로드 파라미터
 * @return 인증서 다운로드일정정보 다운로드 결과
 */
struct Dot2CertDownloadInfoDownloadResult Dot2_DownloadCertDownloadInfo(struct Dot2CertDownloadInfoRequestParams *params);


/**
 * @brief RA에서 최신 LPF(Local Policy File)를 다운로드한다(상세 내용 API 매뉴얼 참조)
 * @param[in] current_filename 현재 가지고 있는 LPF 파일명 (NULL 가능)
 * @return 서버에서 수신된 LPF 정보
 */
struct Dot2LPFRequestResult Dot2_DownloadLPF(const char *current_filename);


/**
 * @brief RA에서 최신 LCCF(Local Certificate Chain File)를 다운로드한다(상세 내용 API 매뉴얼 참조)
 * @param[in] current_filename 현재 가지고 있는 LCCF 파일명 (NULL 가능)
 * @return 서버에서 수신된 LCCF 정보
 */
struct Dot2LCCFRequestResult Dot2_DownloadLCCF(const char *current_filename);


/**
 * @brief 최신 CRL(Certificate Revocation List)를 다운로드한다(상세 내용 API 매뉴얼 참조)
 * @return CRL 다운로드 결과
 */
struct Dot2CRLDownloadResult Dot2_DownloadCRL(void);


/**
 * @brief CRL(Certificate Revocation List)을 로딩한다. (상세 내용 API 매뉴얼 참조)
 * @param[in] crl CRL 인코딩 바이트열
 * @param[in] crl_size CRL 인코딩 바이트열의 길이
 * @retval 0: 성공
 * @retval 음수(-Dot2ResultCode): 실패
 */
int Dot2_LoadCRL(const uint8_t *crl, Dot2CRLSize crl_size);


/**
 * @brief CRL(Certificate Revocation List) 파일을 로딩한다. (상세 내용 API 매뉴얼 참조)
 * @param[in] file_path CRL 인코딩 바이트열이 저장된 파일 경로
 * @retval 0: 성공
 * @retval 음수(-Dot2ResultCode): 실패
 */
int Dot2_LoadCRLFile(const char *file_path);


#ifdef __cplusplus
}
#endif

#endif //V2X_SW_DOT2_API_H
