/**
 * @file
 * @brief dot2 라이브러리의 Open API 파라미터를 정의한 헤더 파일
 * @date 2020-02-18
 * @author gyun
 */


#ifndef V2X_SW_DOT2_API_PARAMS_H
#define V2X_SW_DOT2_API_PARAMS_H

// 시스템 헤더 파일
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// 라이브러리 헤더 파일
#include "dot2-defines.h"
#include "dot2-types.h"


/**
 * @brief 수신된 SPDU의 헤더에 수납된 정보들이 저장되는 구조체
 */
struct Dot2SPDUParseData
{
  Dot2ContentType content_type; ///< Ieee1609Dot2Data 컨텐츠 유형
  struct {
    Dot2SignerIdType signer_id_type; ///< 서명자 식별자 유형 (서명데이터, 서명인증서요청인 경우 유효)
    bool ext_h_present; ///< SPDU 내 external hash 가 존재하는지 여부
    uint8_t ext_h[DOT2_SHA_256_LEN]; ///< SPDU에 수납된 external hash
    Dot2PSID psid; ///< SPDU에 수납된 PSID
    bool gen_time_present; ///< SPDU 내 생성시각정보가 존재하는지 여부
    Dot2Time64 gen_time; ///< SPDU에 수납된 생성시각정보 (TAI 마이크로초)
    bool expiry_time_present; ///< SPDU 내 만기시각정보가 존재하는지 여부
    Dot2Time64 expiry_time; ///< SPDU에 수납된 만기시각정보 (TAI 마이크로초)
    bool gen_location_present; ///< SPDU 내 생성지점정보가 존재하는지 여부
    struct Dot2ThreeDLocation gen_location; // SPDU에 수납된 생성지점정보
  } signed_data; ///< Signed SPDU인 경우 (content_type = kDot2Content_SignedData)
};


/**
 * 어플리케이션 인증서에 대한 CMF 생성 요청 파라미터
 */
struct Dot2AppCMHFMakeParams
{
  struct Dot2ECPrivateKey init_priv_key; ///< 초기 개인키 (예: 인증서번들 내 dwnl_sgn.priv 파일)
  struct Dot2ECPrivateKey recon_priv; ///< 개인키 재구성값 (예: 인증서번들 내 *.s 파일)
  struct Dot2Cert cert; ///< 인증서바이트열 (예: 인증서번들 내 *.cert 파일)
  struct Dot2Cert issuer; ///< 상위인증서바이트열 (예: 인증서번들 내 pca 파일)
};


/**
 * @brief CMHF 생성 결과
 */
struct Dot2CMHFMakeResult
{
  int ret; ///< 실패 시 결과코드(-Dot2ResultCode)
  char *cmhf_name; ///< 생성된 CMHF의 이름 (사용 후 free()해 주어야 한다)
  uint8_t *cmhf; ///< 생성된 CMHF 바이트열 (사용 후 free()해 주어야 한다)
  Dot2CMHFSize cmhf_size; ///< 생성된 CMHF 바이트열의 길이
  struct Dot2ECPrivateKey priv_key; ///< 재구성된 개인키 (실제로 사용되지는 않으며 참고용으로 반환된다)
};


/**
 * 익명 인증서에 대한 CMF 생성 요청 파라미터
 */
struct Dot2PseudonymCMHFMakeParams
{
  uint32_t i; ///< 인증서 i 값
  Dot2CertJvalue j_max; ///< 인증서 j 최대값 (KISA v1.1 규격 기준으로 kDot2CertJvalue_PseudonymMax 값을 가져야 한다)
  struct Dot2AESKey exp_key; ///< 확장함수 키 (예: 인증서번들 내 sgn_expnsn.key 파일)
  struct Dot2ECPrivateKey seed_priv; ///< 시드 개인키 (예: 인증서번들 내 dwnl_sgn.priv 파일)
  struct Dot2Cert certs[DOT2_DEFAULT_P_CERTS_PER_I_PERIOD];; ///< j_max+1개의 인증서 바이트열들 (예: 인증서번들 내 *.cert 파일)
  struct Dot2ECPrivateKey recon_privs[DOT2_DEFAULT_P_CERTS_PER_I_PERIOD]; ///< j_max+1개의 개인키 재구성값들 (예 : 인증서번들 내 *.s 파일)
  struct Dot2Cert issuer; ///< 상위인증서바이트열 (예: 인증서번들 내 pca 파일)
};


/**
 * @brief 익명 인증서에 대한 CMHF 생성 결과
 */
struct Dot2PseudonymCMHFMakeResult
{
  int ret; ///< 실패 시 결과코드(-Dot2ResultCode)
  char *cmhf_name; ///< 생성된 CMHF의 이름 (사용 후 free()해 주어야 한다)
  uint8_t *cmhf; ///< 생성된 CMHF 바이트열 (사용 후 free()해 주어야 한다)
  Dot2CMHFSize cmhf_size; ///< 생성된 CMHF 바이트열의 길이
  struct Dot2ECPrivateKey priv_keys[DOT2_DEFAULT_P_CERTS_PER_I_PERIOD]; ///< 재구성된 개인키들 (실제로 사용되지는 않으며 참고용으로 반환된다)
};


/**
 * 식별 인증서에 대한 CMF 생성 요청 파라미터
 */
struct Dot2IdCMHFMakeParams
{
  uint32_t i; ///< 인증서 i 값
  struct Dot2AESKey exp_key; ///< 확장함수 키 (예: 인증서번들 내 sgn_expnsn.key 파일)
  struct Dot2ECPrivateKey seed_priv; ///< 시드 개인키 (예: 인증서번들 내 dwnl_sgn.priv 파일)
  struct Dot2Cert cert; ///< 인증서 바이트열 (예: 인증서번들 내 *.cert 파일)
  struct Dot2ECPrivateKey recon_priv; ///< 개인키 재구성값 (예 : 인증서번들 내 *.s 파일)
  struct Dot2Cert issuer; ///< 상위인증서바이트열 (예: 인증서번들 내 pca 파일)
};


/**
 * @brief 식별 인증서에 대한 CMHF 생성 결과
 */
struct Dot2IdCMHFMakeResult
{
  int ret; ///< 실패 시 결과코드(-Dot2ResultCode)
  char *cmhf_name; ///< 생성된 CMHF의 이름 (사용 후 free()해 주어야 한다)
  uint8_t *cmhf; ///< 생성된 CMHF 바이트열 (사용 후 free()해 주어야 한다)
  Dot2CMHFSize cmhf_size; ///< 생성된 CMHF 바이트열의 길이
  struct Dot2ECPrivateKey priv_key; ///< 재구성된 개인키 (실제로 사용되지는 않으며 참고용으로 반환된다)
};


/**
 * @brief "Security profile" 정보 형식
 */
struct Dot2SecProfile
{
  Dot2PSID psid; ///< PSID
  struct Dot2TxSecProfile tx; ///< 송신에 관련된 Security profile
  struct Dot2RxSecProfile rx; ///< 수신에 관련된 Security profile
};


/**
 * @brief SPDU 생성 파라미터
 */
struct Dot2SPDUConstructParams
{
  Dot2SPDUConstructType type; ///< 생성하고자 하는 SPDU의 유형
  Dot2Time64 time; ///< SPDU 생성 시각 (어플리케이션이 0으로 설정할 경우, API 내부에서 현재시각으로 설정된다)
  struct {
    Dot2PSID psid; ///< PSID
    Dot2SignerIdType signer_id_type; ///< 서명자인증서 식별자 유형
    struct Dot2ThreeDLocation gen_location; ///< SPDU 생성지점 (Unavailable 값 사용 가능)
    bool cmh_change; ///< CMH 세트 내에서 현재 서명에 사용 중인 CMH를 다른 CMH로 변경할지 여부 (Week Rotate CMH를 사용할 경우에 해당)
  } signed_data;
};


/**
 * @brief SPDU 생성 결과
 */
struct Dot2SPDUConstructResult
{
  int ret; ///< 양수: 생성된 SPDU의 길이, 음수(-Dot2ResultCode): 실패
  uint8_t *spdu; ///< 생성된 SPDU (동적할당된 정보이므로, 어플리케이션은 사용 후 free()를 통해 해제해 주어야 한다)
  bool cmh_expiry; ///< 주기적인 서명이 사용될 경우, 서명에 사용 중인 CMH의 유효기간이 현시점 기준으로 만료되었거나 혹은 다음번 서명시점에 만료될지 여부.
                   ///< (Rotate CMH의 경우, 서명에 사용 중인 CMH 세트의 유효기간이 현시점 기준으로 만료되었거나 혹은 다음번 서명시점에 만료될지 여부)
                   ///< 다음번 서명시점에 만료될지 여부는 Security profile(송신정보)에 등록된 주기(interval)에 따라 계산된다.
                   ///< 어플리케이션은 본 정보를 이용하여 다음번 주기의 페이로드 생성 시에 참고할 수 있다.
                   ///< (예: 다음번 BSM 생성 시에 TemporaryID가 변경된 BSM을 생성한 후 SPDU를 생성할 수 있다)
};
#define DOT2_SPDU_CONSTRUCT_RESULT_INITIALIZER { \
  .ret = 0,                                      \
  .spdu = NULL,                                  \
  .cmh_expiry = false                            \
}


/**
 * @brief SPDU 처리 파라미터
 */
struct Dot2SPDUProcessParams
{
  Dot2Time64 rx_time; ///< SPDU 수신 시각 (0으로 설정할 경우, API 내부에서 현재시각으로 설정된다)
  Dot2PSID rx_psid; ///< SPDU 수신 PSID (SPDU를 수납한 PDU(예: WSM)의 PSID)
  struct Dot2TwoDLocation rx_pos; ///< SPDU 수신좌표 (Unavailable 값 사용 가능)
};


/**
 * @brief 등록인증서(Enrollment certificate) 발급요청문 생성 요청 파라미터
 */
struct Dot2ECRequestConstructParams
{
  Dot2Time32 time; ///< 생성요청 시각 (어플리케이션이 0으로 설정할 경우, API 내부에서 현재시각으로 설정된다)
  struct Dot2CertValidPeriod valid_period; ///< 등록인증서 유효기간.
  ///< (valid_period.start=0으로 설정할 경우 API 내부에서 현재시각으로 설정된다)
  struct {
    Dot2IdentifiedRegionNum region_num; ///< 유효지역 개수
    Dot2CountryCode region[kDot2IdentifiedRegionNum_Max]; ///< 유효지역 식별자
  } valid_region; ///< 등록인증서에 포함될 유효지역 정보 (KCAC.V2X.CERTPROF p.43에 따르면 한국을 나타내는 countryOnly 정보가 사용된다)

  struct {
    Dot2CertPermissionNum num; ///< 권한 개수
    Dot2PSID psid[kDot2CertPermissionNum_Max]; ///< 권한
  } permissions; ///< 등록인증서에 포함될 권한 정보
};


/**
 * @brief 등록인증서(Enrollment certificate) 발급요청문 생성 결과 파라미터
 */
struct Dot2ECRequestConstructResult
{
  int ret; ///< 성공시: 생성된 등록인증서발급요청문의 길이, 실패시: 음수(-Dot2ResultCode)
  uint8_t *ec_req; ///< 생성된 등록인증서발급요청문(실패 시 NULL). 동적할당된 정보이므로 사용 후 free()해 주어야 함.
  uint8_t ec_req_h8[8]; ///< 생성된 등록인증서발급요청문의 HashedId8 값
  struct Dot2ECPrivateKey init_priv_key; ///< 초기 개인키
};


/**
 * @brief 등록인증서(Enrollment certificate) 발급응답문 처리 요청 파라미터
 */
struct Dot2ECResponseProcessParams
{
  uint8_t ec_req_h8[8]; ///< 등록인증서발급요청문의 HashedId8 값
  const uint8_t *ec_resp; ///< ECA로부터 수신한 등록인증서 발급응답문(ECResponse) 바이트열 (없을 경우 NULL 전달 가능)
  Dot2SPDUSize ec_resp_size; ///< ec_resp의 길이
  struct Dot2Cert ec; ///< ECA로부터 수신한 등록인증서(Enrollment certificate) 바이트열
  struct Dot2ECPrivateKey init_priv_key; ///< 초기 개인키
  struct Dot2ECPrivateKey recon_priv; ///< ECA로부터 수신한 개인키재구성값 바이트열
  struct Dot2Cert eca_cert; ///< ECA로부터 수신한 ECA(Enrollment Certificate Authority) 인증서 바이트열
  struct Dot2Cert ra_cert; ///< ECA로부터 수신한 RA(Registration Authority) 인증서 바이트열
  struct Dot2Cert rca_cert; ///< ECA로부터 수신한 RCA(Root Certificate Authority) 인증서 바이트열
  const uint8_t *lccf; ///< ECA로부터 수신한 LCCF(Local Certificate Chain File) 바이트열
  Dot2LCCFSize lccf_size; ///< lccf의 길이
};


/**
 * 등록인증서(Enrollment certificate) 발급응답문 처리 결과 파라미터
 */
struct Dot2ECResponseProcessResult
{
  int ret; ///< 처리 결과 코드. 0: 성공, 음수(-Dot2ResultCode): 실패
  bool req_h_matched; ///< 등록인증서 발급요청문의 H8값과 발급응답문 내 포함된 requestHash값이 같은지 여부
  struct Dot2ECPrivateKey enrollment_priv_key; ///< 등록인증서 개인키 (재구성된 개인키) -> 실제로 사용되지는 않는다(대신 CMHF가 사용됨)
  char *enrollment_cmhf_name; ///< 등록인증서 CMHF 파일명 문자열. 동적할당된 정보이므로 사용 후 free()해 주어야 한다.
  uint8_t *enrollment_cmhf; ///< 등록인증서 CMHF 바이트열. 동적할당된 정보이므로 사용 후 free()해 주어야 한다.
  size_t enrollment_cmf_size; ///< enrollment_cmhf의 길이
  uint8_t *rca_cert; ///< RCA 인증서 바이트열 (NULL일 수 있음) 동적할당된 정보이므로 사용 후 free()해 주어야 한다.
  Dot2CertSize rca_cert_size; ///< rca_cert의 길이
  uint8_t *ica_cert; ///< ICA 인증서 바이트열 (NULL일 수 있음) 동적할당된 정보이므로 사용 후 free()해 주어야 한다.
  Dot2CertSize ica_cert_size; ///< ica_cert의 길이
  uint8_t *pca_cert; ///< PCA 인증서 바이트열 (NULL일 수 있음) 동적할당된 정보이므로 사용 후 free()해 주어야 한다.
  Dot2CertSize pca_cert_size; ///< pca_cert의 길이
  uint8_t *crlg_cert; ///< CRLG 인증서 바이트열 (NULL일 수 있음) 동적할당된 정보이므로 사용 후 free()해 주어야 한다.
  Dot2CertSize crlg_cert_size; ///< crlg_cert의 길이
};


/**
 * @brief 인증서 발급요청 파라미터 (대상: 응용/익명/식별 인증서)
 */
struct Dot2CertProvisioningRequestParams
{
  bool return_options; ///< 인증서 다운로드 결과 중 옵션 정보의 반환 여부
  Dot2Time32 current_time; ///< 현재 시각 (0으로 설정하는 경우, API 내부에서 현시점으로 자동 설정된다)
  Dot2Time32 start_time; ///< 발급받고자 하는 인증서의 유효기간 시작시점 (0으로 설정하는 경우, API 내부에서 현시점으로 자동 설정된다)
};


/**
 * @brief 인증서 발급요청 결과 파라미터 (응용/익명/식별인증서 발급요청 결과 파라미터 중 공통으로 적용되는 정보)
 */
struct Dot2CertProvisioningRequestResult
{
  uint8_t req_h8[8]; ///< 생성된 인증서발급요청문에 대한 H8
  struct Dot2ECPrivateKey verify_priv_key; ///< 응용인증서: 서명용 임시 개인키, 익명/식별인증서: 서명용 시드(caterpillar) 개인키
  struct Dot2ECPrivateKey cert_enc_priv_key; ///< 응용인증서: 인증서복호화용 임시 개인키,
                                             ///< 익명/식별인증서: 인증서복호화용 시드(caterpillar) 개인키
  char *cert_dl_url; ///< 이후 인증서다운로드 요청 시 사용할 인증서 다운로드 URL (사용 후 free()해 주어야 한다)
  Dot2Time32 cert_dl_time; ///< 인증서 다운로드가능시점
  Dot2Time32 current_time; ///< 현재시각(본 API 리턴 직전 시점)
  int remained_cert_dl_time; ///< 현재시각으로부터 인증서 다운로드가능시점까지 남은 시각(초단위)
};


/**
 * @brief 인증서 발급요청 결과 중 옵션 정보 (대상: 응용/익명/식별인증서)
 */
struct Dot2CertProvisioningRequestOptionResult
{
  uint8_t *req; ///< RA로 전송한 인증서 발급요청문 바이트열 (사용 후 free()해 주어야 한다)
  Dot2SPDUSize req_size; ///< 인증서 발급요청문 바이트열의 길이
  uint8_t *ack; ///< RA로부터 수신한 인증서 발급응답문 바이트열 (사용 후 free()해 주어야 한다)
  Dot2SPDUSize ack_size; ///< 인증서 발급응답문 바이트열의 길이
};


/**
 * @brief 응용인증서 발급요청 결과 파라미터
 */
struct Dot2AppCertProvisioningRequestResult
{
  int ret; ///< 성공시: 0, 실패시: 음수(-Dot2ResultCode)
  struct Dot2CertProvisioningRequestResult common;
  struct Dot2CertProvisioningRequestOptionResult options;
};


/**
 * @brief 익명/식별인증서 발급요청 결과 파라미터
 */
struct Dot2PseudonymIdCertProvisioningRequestResult
{
  int ret; ///< 성공시: 0, 실패시: 음수(-Dot2ResultCode)
  struct Dot2CertProvisioningRequestResult common;
  struct Dot2AESKey verify_exp_key; ///< 서명용 확장함수 키 (인증서 다운로드 시 서명용 cocoon 개인키 생성에 사용)
  struct Dot2AESKey cert_enc_exp_key; ///< 인증서복호화용 확장함수 키 (인증서 다운로드 시 인증서복호화용 cocoon 개인키 생성에 사용)
  struct Dot2CertProvisioningRequestOptionResult options;
};


/**
 * @brief 인증서 다운로드 요청 파라미터 (응용/익명/식별인증서 다운로드 요청 파라미터 중 공통으로 적용되는 정보)
 */
struct Dot2CertDownloadRequestParams
{
  uint8_t req_h8[8]; ///< RA로 전송했던 인증서 발급요청문에 대한 H8
  struct Dot2ECPrivateKey verify_priv_key; ///< 응용인증서: 서명용 임시 개인키, 익명/식별인증서: 서명용 시드(caterpillar) 개인키
  struct Dot2ECPrivateKey cert_enc_priv_key; ///< 응용인증서: 인증서복호화용 임시 개인키,
                                             ///< 익명/식별인증서: 인증서복호화용 시드(caterpillar) 개인키
  char *cert_dl_url; ///< 인증서 다운로드 URL
};


/**
 * @brief 응용인증서 다운로드 요청 파라미터
 */
struct Dot2AppCertDownloadRequestParams
{
  bool return_options; ///< 인증서 다운로드 결과 중 옵션 정보의 반환 여부
  struct Dot2CertDownloadRequestParams common;
};


/**
 * @brief 익명인증서/식별인증서 다운로드 요청 파라미터
 */
struct Dot2PseudonymIdCertDownloadRequestParams
{
  bool return_options; ///< 인증서 다운로드 결과 중 옵션 정보의 반환 여부
  struct Dot2CertDownloadRequestParams common;
  struct Dot2AESKey verify_exp_key; ///< 서명용 확장함수 키 (인증서 다운로드 시 서명용 cocoon 개인키 생성에 사용)
  struct Dot2AESKey cert_enc_exp_key; ///< 인증서복호화용 확장함수 키 (인증서 다운로드 시 인증서복호화용 cocoon 개인키 생성에 사용)
  Dot2IPeriod i_period; ///< 다운로드하고자 하는 익명인증서(들)의 i-period 값 (0으로 설정할 경우, 현 시점의 i-period 값 + 1이 적용된다)
  Dot2IdCertTargetTime target_time; ///< 다운로드하고자 하는 식별인증서의 타겟 시점 (0으로 설정할 경우, 현 시점)
};


/**
 * @brief 인증서 다운로드 결과 파라미터 (응용/익명/식별인증서 다운로드 결과 파라미터 중 공통으로 적용되는 정보)
 */
struct Dot2CertDownloadResult
{
  char *cmhf_name; ///< 발급된 인증서 CMHF 이름 (사용 후 free() 해 주어야 한다)
  uint8_t *cmhf; ///< 발급된 인증서 CMHF 바이트열 (사용 후 free() 해 주어야 한다)
  Dot2CMHFSize cmhf_size; ///< 발급된 인증서 CMHF 바이트열의 길이
};


/**
 * @brief 인증서 다운로드 옵션 결과 파라미터 (대상: 응용/식별인증서)
 *
 * dot2 라이브러리는 보안통신을 수행할 때 본 정보(개인키,인증서)를 직접 사용하지 않고 (해당 정보가 포함된) CMHF를 사용한다.
 * 따라서 본 정보는 실제동작에는 사용되지 않는 옵션 정보로써, 참고용 또는 디버깅용으로 사용될 수 있다.
 * 요청파라미터의 return_options가 true일 경우에만 채워져 반환된다.
 */
struct Dot2CertDownloadOptionResult
{
  char dir_name[DOT2_CERT_FILE_DIR_NAME+1]; ///< 인증서 저장디렉토리명 = "H8(발급요청문)"
  char cert_filename[DOT2_CERT_FILE_NAME_MAX_LEN+1]; ///< 발급된 인증서 저장파일명 = "H8(인증서).cert"
  char priv_key_filename[DOT2_PRIV_KEY_FILE_NAME_MAX_LEN+1]; ///< 개인키 저장파일명 = "H8(인증서).privkey"
  char recon_priv_filename[DOT2_RECON_PRIV_FILE_NAME_MAX_LEN+1]; ///< 개인키재구성값 저장파일명 = "H8(인증서).s"
  struct Dot2Cert cert; ///< 발급된 인증서 바이트열
  struct Dot2ECPrivateKey priv_key; ///< (재구성된) 개인키 바이트열
  struct Dot2ECPrivateKey recon_priv; ///< 서버로부터 수신한 개인키 재구성값 바이트열
};


/**
 * @brie 익명인증서 다운로드 옵션 결과 파라미터
 *
 * dot2 라이브러리는 보안통신을 수행할 때 본 정보(개인키,인증서)를 직접 사용하지 않고 (해당 정보가 포함된) CMHF를 사용한다.
 * 따라서 본 정보는 실제동작에는 사용되지 않는 옵션 정보로써, 참고용 또는 디버깅용으로 사용될 수 있다.
 * 요청파라미터의 return_options가 true일 경우에만 채워져 반환된다.
 */
struct Dot2PseudonymCertDownloadOptionResult
{
  char dir_name[DOT2_CERT_FILE_DIR_NAME+1]; ///< 인증서 저장디렉토리명 = "H8(발급요청문)"
  char cert_filenames[DOT2_DEFAULT_P_CERTS_PER_I_PERIOD][DOT2_CERT_FILE_NAME_MAX_LEN+1]; ///< 발급된 인증서 저장파일명 = "H8(인증서).cert"
  char priv_key_filenames[DOT2_DEFAULT_P_CERTS_PER_I_PERIOD][DOT2_PRIV_KEY_FILE_NAME_MAX_LEN+1]; ///< 개인키 저장파일명 = "H8(인증서).privkey"
  char recon_priv_filenames[DOT2_DEFAULT_P_CERTS_PER_I_PERIOD][DOT2_RECON_PRIV_FILE_NAME_MAX_LEN+1]; ///< 개인키재구성값 저장파일명 = "H8(인증서).s"
  struct Dot2Cert certs[DOT2_DEFAULT_P_CERTS_PER_I_PERIOD]; ///< 발급된 인증서 바이트열
  struct Dot2ECPrivateKey priv_keys[DOT2_DEFAULT_P_CERTS_PER_I_PERIOD]; ///< (재구성된) 개인키 바이트열
  struct Dot2ECPrivateKey recon_privs[DOT2_DEFAULT_P_CERTS_PER_I_PERIOD]; ///< 서버로부터 수신한 개인키 재구성값 바이트열
};


/**
 * @brief 응용인증서 다운로드 결과 파라미터
 */
struct Dot2AppCertDownloadResult
{
  int ret; ///< 성공시: 0, 실패시: 음수(-Dot2ResultCode)
  struct Dot2CertDownloadResult common;
  struct Dot2CertDownloadOptionResult options;
};


/**
 * @brief 익명인증서 다운로드 결과 파라미터
 */
struct Dot2PseudonymCertDownloadResult
{
  int ret; ///< 성공시: 0, 실패시: 음수(-Dot2ResultCode)
  struct Dot2CertDownloadResult common;
  struct Dot2PseudonymCertDownloadOptionResult options;
};


/**
 * @brief 식별인증서 다운로드 결과 파라미터
 */
struct Dot2IdCertDownloadResult
{
  int ret; ///< 성공시: 0, 실패시: 음수(-Dot2ResultCode)
  struct Dot2CertDownloadResult common;
  struct Dot2CertDownloadOptionResult options;
};


/**
 * @brief 인증서 다운로드일정정보 요청 파라미터 -> 익명/식별인증서 다운로드 요청 파라미터와 동일하다.
 */
struct Dot2CertDownloadInfoRequestParams
{
  uint8_t req_h8[8]; ///< RA로 전송했던 인증서 발급요청문에 대한 H8
  char *cert_dl_url; ///< 인증서 다운로드일정정보 다운로드 URL
};


/**
 * @brief 인증서 다운로드일정정보 다운로드 결과 파라미터
 *
 * 대상: 익명/식별인증서 (응용인증서는 다운로드일정정보를 사용하지 않는다)
 */
struct Dot2CertDownloadInfoDownloadResult
{
  int ret; ///< 성공시: 0, 실패시: 음수(-Dot2ResultCode)
  Dot2Time32 cert_dl_time; ///< 인증서 다운로드가능시점
  Dot2Time32 current_time; ///< 현재시각(본 API 리턴 직전 시점)
  int remained_cert_dl_time; ///< API 리턴 직전 시점으로부터 인증서 다운로드가능시점까지 남은 시각(초단위)
};


/**
 * @brief LPF(Local Policy File) 발급요청 결과 파라미터
 */
struct Dot2LPFRequestResult
{
  int ret; ///< 성공시: 0, 실패시: 음수(-Dot2ResultCode)
  char *lpf_filename; ///< 서버에서 수신한 LPF 파일명 (사용 후 free() 해 주어야 한다)
  uint8_t *lpf; ///< 서버에서 수신한 LPF 바이트열 (사용 후 free() 해 주어야 한다)
  Dot2SPDUSize lpf_size; ///< 서버에서 수신한 LPF 바이트열의 길이
};


/**
 * @brief LCCF(Local Certificate Chain File) 발급요청 결과 파라미터
 */
struct Dot2LCCFRequestResult
{
  int ret; ///< 성공시: 0, 실패시: 음수(-Dot2ResultCode)
  char *lccf_filename; ///< 서버에서 수신한 LCCF 파일명 (사용 후 free() 해 주어야 한다)
  uint8_t *lccf; ///< 서버에서 수신한 LCCF 바이트열 (사용 후 free() 해 주어야 한다)
  Dot2LCCFSize lccf_size; ///< 서버에서 수신한 LCCF 바이트열의 길이
  uint8_t *rca_cert; ///< LCCF에서 추출한 RCA 인증서 바이트열 (NULL일 수 있음) (사용 후 free() 해 주어야 한다)
  Dot2CertSize rca_cert_size; ///< rca_cert의 길이
  uint8_t *ica_cert; ///< LCCF에서 추출한 ICA 인증서 바이트열 (NULL일 수 있음) (사용 후 free() 해 주어야 한다)
  Dot2CertSize ica_cert_size; ///< ica_cert의 길이
  uint8_t *pca_cert; ///< LCCF에서 추출한 PCA 인증서 바이트열 (NULL일 수 있음) (사용 후 free() 해 주어야 한다)
  Dot2CertSize pca_cert_size; ///< pca_cert의 길이
  uint8_t *crlg_cert; ///< LCCF에서 추출한 CRLG 인증서 바이트열 (NULL일 수 있음) (사용 후 free() 해 주어야 한다)
  Dot2CertSize crlg_cert_size; ///< crlg_cert의 길이
};


/**
 * @brief CRL(Certificate Revocation List) 다운로드 결과 파라미터
 */
struct Dot2CRLDownloadResult
{
  int ret; ///< 성공시: 0, 실패시: 음수(-Dot2ResultCode)
  uint8_t *crl; ///< 서버에서 수신한 CRL 바이트열 (사용 후 free() 해 주어야 한다)
  Dot2CRLSize crl_size; ///< 서버에서 수신한 CRL 바이트열의 길이
};


#endif //V2X_SW_DOT2_API_PARAMS_