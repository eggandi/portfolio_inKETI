/**
 * @file
 * @brief dot2 라이브러리의 Open API에서 사용되는 정의값 및 매크로 등을 정의한 헤더 파일
 * @date 2020-05-23
 * @author gyun
 */


#ifndef V2X_SW_DOT2_DEFINES_H
#define V2X_SW_DOT2_DEFINES_H

#ifndef DOT2_SHA_256_LEN
#define DOT2_SHA_256_LEN (32) /// SHA-256 해시(출력데이터) 길이
#endif


#ifndef DOT2_AES_128_LEN
#define DOT2_AES_128_LEN (16) ///< AES-128 입출력 데이터 길이
#endif

#ifndef DOT2_EC_256_KEY_LEN
#define DOT2_EC_256_KEY_LEN (32) ///< 256비트 타원곡선 키 길이 (=개인키길이)
#define DOT2_EC_256_UNCOMPRESSED_POINT_LEN (1/*Form*/ + (2 * DOT2_EC_256_KEY_LEN)) ///< 비압축 256비트 타원곡선 좌표 길이 (Form || X || Y)
#define DOT2_EC_256_COMPRESSED_POINT_LEN (1/*Form*/ + (1 * DOT2_EC_256_KEY_LEN)) ///< 압축 256비트 타원곡선 좌표 길이 (Form || X)
#define DOT2_EC_256_POINT_LEN (DOT2_EC_256_UNCOMPRESSED_POINT_LEN)
#define DOT2_EC_256_UNCOMPRESSED_PUB_KEY_LEN (DOT2_EC_256_UNCOMPRESSED_POINT_LEN)
#define DOT2_EC_256_COMPRESSED_PUB_KEY_LEN (DOT2_EC_256_COMPRESSED_POINT_LEN)
#define DOT2_EC_256_PUB_KEY_LEN (DOT2_EC_256_UNCOMPRESSED_PUB_KEY_LEN)
#define DOT2_EC_256_UNCOMPRESSED_SIGN_R_LEN (DOT2_EC_256_UNCOMPRESSED_POINT_LEN)
#define DOT2_EC_256_COMPRESSED_SIGN_R_LEN (DOT2_EC_256_COMPRESSED_POINT_LEN)
#define DOT2_EC_256_XONLY_SIGN_R_LEN (DOT2_EC_256_COMPRESSED_POINT_LEN)
#define DOT2_EC_256_SIGN_R_LEN (DOT2_EC_256_UNCOMPRESSED_SIGN_R_LEN)
#define DOT2_EC_256_SIGN_S_LEN (DOT2_EC_256_KEY_LEN)
#endif

/// 단일 i-period 내에서 사용되는 익명인증서 개수
#define DOT2_DEFAULT_P_CERTS_PER_I_PERIOD (20)
/// 단일 i-period 내에서 사용되는 식별인증서 개수
#define DOT2_DEFAULT_I_CERTS_PER_I_PERIOD (1)

/// H8값의 16진수 문자열의 길이 (8바이트 * 2)
#define DOT2_H8_HEX_STR_LEN (16)
/// i-period 16진수 문자열 최대 길이 (per KISA 14-03.KCAC.V2X.OU.PDCMAN_익명 인증서 관리 규격_v1.1, p.15)
#define DOT2_I_PERIOD_HEX_STR_MAX_LEN (8)
/// j-value 16진수 문자열 최대 길이 (per KISA 14-03.KCAC.V2X.OU.PDCMAN_익명 인증서 관리 규격_v1.1, p.15)
#define DOT2_J_VALUE_HEX_STR_MAX_LEN (2)

/// 인증서 저장 디렉토리명 길이 = "H8(발급요청문)" 문자열
#define DOT2_CERT_FILE_DIR_NAME (DOT2_H8_HEX_STR_LEN)

/// 응용인증서 파일명 길이 = "H8(인증서)" 문자열 + ".cert" (예: 17d9c58c3ac798e3.cert)
#define DOT2_A_CERT_FILE_NAME_LEN (DOT2_H8_HEX_STR_LEN + 5)
/// 응용인증서 개인키 파일명 길이 = "H8(인증서)" 문자열 + ".privkey" (예: 17d9c58c3ac798e3.privkey)
#define DOT2_A_PRIV_KEY_FILE_NAME_LEN (DOT2_H8_HEX_STR_LEN + 8)
/// 응용인증서 개인키재구성값 파일명 길이 = "H8(인증서)" 문자열 + ".s" (예: 17d9c58c3ac798e3.s)
#define DOT2_A_RECON_PRIV_FILE_NAME_LEN (DOT2_H8_HEX_STR_LEN + 2)

/// 익명/식별인증서 파일명 프리픽스 최대길이. "i-period" 문자열 + "_" + "j 값" 문자열. (예: 18D_13)
#define DOT2_P_I_CERT_FILE_NAME_PREFIX_MAX_LEN (DOT2_I_PERIOD_HEX_STR_MAX_LEN + 1 + DOT2_J_VALUE_HEX_STR_MAX_LEN)
/// 익명/식별인증서 파일명 최대길이 = 파일명프리픽스 + ".cert"
#define DOT2_P_I_CERT_FILE_NAME_MAX_LEN (DOT2_P_I_CERT_FILE_NAME_PREFIX_MAX_LEN + 5)
/// 익명/식별인증서 개인키 파일명 최대길이 = 파일명프리픽스 + ".privkey"
#define DOT2_P_I_PRIV_KEY_FILE_NAME_MAX_LEN (DOT2_P_I_CERT_FILE_NAME_PREFIX_MAX_LEN + 8)
/// 익명/식별인증서 개인키재구성값 파일명 최대길이 = 파일명프리픽스 + ".s"
#define DOT2_P_I_RECON_PRIV_FILE_NAME_MAX_LEN (DOT2_P_I_CERT_FILE_NAME_PREFIX_MAX_LEN + 2)

/// 응용/익명/식별인증서에 대한 인증서/개인키/개인키재구성값 파일명 최대길이 (응용인증서의 길이값을 따른다)
#define DOT2_CERT_FILE_NAME_MAX_LEN (DOT2_A_CERT_FILE_NAME_LEN)
#define DOT2_PRIV_KEY_FILE_NAME_MAX_LEN (DOT2_A_PRIV_KEY_FILE_NAME_LEN)
#define DOT2_RECON_PRIV_FILE_NAME_MAX_LEN (DOT2_A_RECON_PRIV_FILE_NAME_LEN)

/// 식별인증서 유효기간 (초단위) - KISA v1.1에 따라, 30일 + 1시간이 사용되고 있다.
#define DOT2_ID_CERT_VALID_DURATION (((30 * 24) + 1) * 3600)

#endif //V2X_SW_DOT2_DEFINES_H
