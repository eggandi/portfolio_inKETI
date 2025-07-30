/**
 * @file
 * @brief dot2 라이브러리의 Open API에서 사용되는 유형들을 정의한 헤더 파일
 * @date 2020-02-18
 * @author gyun
 */


#ifndef V2X_SW_DOT2_TYPES_H
#define V2X_SW_DOT2_TYPES_H

// 시스템 헤더 파일
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// 라이브러리 헤더 파일
#include "dot2-defines.h"


/**
 * @brief Open API 처리 결과 코드
 *
 * 호출된 API에 따라 중간 프리픽스가 적용된다 (예: SPDU 관련 API 호출 시에는 중간에 _SPDU_ 프리픽스가 포함된다)
 */
enum eDot2ResultCode
{
  kDot2Result_Success = 0, ///< 성공.
  kDot2Result_RequestSuccess = 1, ///< 요청 성공.
  kDot2Result_NullParameters = 5, ///< Null 파라미터.
  //kDot2Result_TooLongData = 6, ///< 너무 긴 데이터
  kDot2Result_FailToOerDecode = 7, ///< OER 디코딩 실패
  kDot2Result_InvalidContentType = 8, ///< 유효하지 않은 컨텐츠 유형
  kDot2Result_NotSupportedContentType = 9, ///< 지원하지 않는 컨텐츠 유형
  kDot2Result_InsufficientBuffer = 10, ///< 충분하지 않은 버퍼
  kDot2Result_NullData = 11, ///< Null 데이터
  kDot2Result_InvalidProtocolVersion = 12, ///< 유효하지 않은 프로토콜 버전
  kDot2Result_InvalidInnerContentProtocolVersion = 13, ///< 컨텐츠 내 컨텐츠의 유효하지 않은 프로토콜 버전
  kDot2Result_InvalidInnerContentType = 14, ///< 컨텐츠 내 컨텐츠의 유효하지 않은 유형
  kDot2Result_InvalidExtHashType = 15, ///< 유효하지 않은 External Hash 유형
  kDot2Result_InvalidExtHashData = 16, ///< 유효하지 않은 External Hash 데이터
  //kDot2Result_TooShortData = 17, ///< 너무 짧은 데이터
  kDot2Result_NoMemory = 18, ///< 메모리 부족
  kDot2Result_FailToOerEncode = 19, ///< OER 인코딩 실패
  kDot2Result_OpensslFail = 20, ///< Openssl 동작 실패
  kDot2Result_InvalidCertIssuerIdentifierType = 21, ///< 인증서 내 유효하지 않은 상위인증서 식별자 유형
  kDot2Result_InvalidCertValidRegion = 22, ///< 인증서 내 유효하지 않은 유효지역
  kDot2Result_TooManyCertValidRegion = 23, ///< 인증서 내 유효지역 개수가 너무 많음
  kDot2Result_TooLongSsp = 25, ///< 인증서 내 SSP 가 너무 김
  kDot2Result_InvalidSsp = 26, ///< 유효하지 않은 SSP
  kDot2Result_InvalidSubjectPermissionsType = 27, ///< 유효하지 않은 서브젝트 권한 유형
  kDot2Result_TooManyPsidSspRange = 28, ///< 인증서 내 PsidSspRange 개수가 너무 많음
  kDot2Result_TooManySspInSspRange = 29, ///< 인증서 SspRange 내 SSP 개수가 너무 많음
  kDot2Result_InvalidSspRangeType = 30, ///< 유효하지 않은 SspRange 유형
  kDot2Result_InvalidVerificationKeyIndicatorType = 31, ///< 유효하지 않은 VerificationKeyIndicator 유형
  kDot2Result_InvalidVerificationKeyType = 32, ///< 유효하지 않은 키 유형
  kDot2Result_NotSupportedVerificationKeyType = 33, ///< 지원하지 않는 키 유형
  kDot2Result_InvalidSignatureType = 35, ///< 유효하지 않은 서명 유형
  kDot2Result_NotSupportedSignatureType = 36, ///< 지원하지 않는 서명 유형
  kDot2Result_InvalidCertSize = 37, ///< 유효하지 않은 인증서 길이
  kDot2Result_InvalidEccCurvePointType = 38,
  kDot2Result_TooManyCertsInTable = 39, ///< 테이블에 인증서가 너무 많음
  kDot2Result_NoCertChain = 40, ///< 인증서체인이 없음(구성할 수 없음)
  kDot2Result_SignatureVerificationFailed = 42, ///< 서명 검증 실패
  kDot2Result_InvalidSignerIdType = 44, ///< 유효하지 않은 SignerId 유형
  kDot2Result_NoIssuerCert = 45, ///< 상위인증서가 없음
  kDot2Result_NoSuchCMH = 46, ///< 해당되는 CMH가 없음
  kDot2Result_SameCMHInTable= 48, ///< 동일한 CMH가 이미 테이블에 있음
  kDot2Result_NotSupportedSignerIdType = 49, ///< 지원하지 않는 SignerId 유형
  kDot2Result_NoSignerIdCertInTable = 53, ///< 인증서정보 테이블 내에 signer_id에 해당되는 인증서정보가 없음
  kDot2Result_TooManyCMHTypeEntryInTable = 54, ///< CMH 테이블에 저장된 "CMH 유형 엔트리"의 개수가 너무 많음
  kDot2Result_DifferentCMHTypeEntryInTable = 55, ///< CMH 테이블에 동일 Psid에 대해 다른 CMH 유형을 갖는 엔트리가 존재
  kDot2Result_NoSuchCMHTypeEntryInTable = 56, ///< CMH 테이블에 해당되는 CMH 유형 엔트리가 존재하지 않음
  kDot2Result_NoAvailableCMHInTable_PSID = 57, ///< CMH 테이블 내에 일치하는 PSID를 갖는 CMH 정보가 없음
  kDot2Result_NoAvailableCMHInTable_ValidTime = 58, ///< CMH 테이블 내에 현 시점에 가용한 CMH 정보가 없음(유효기간)
  kDot2Result_NoSuchSecProfileInTable = 59, ///< Security profile 테이블 내에 해당되는 security profile이 없음
  kDot2Result_FailToAsn1 = 60, ///< asn.1 처리 함수 수행 실패
  kDot2Result_InvalidECType = 61, ///< 유효하지 않은 ECC 커브 유형
  kDot2Result_NotSupportedECType = 62, ///< 지원하지 않는 ECC 커브 유형
  kDot2Result_InvalidKeyPair = 63, ///< 유효하지 않은 키쌍
  kDot2Result_InvalidCMHFSize = 64, ///< 유효하지 않은 CMHF 크기
  kDot2Result_InvalidCMHType = 65, ///< 유효하지 않은 CMH 유형
  kDot2Result_FailToOpenDirectory = 66, ///< 디렉토리 오픈 실패
  kDot2Result_FailToAccessFile = 67, ///< 파일 접근 실패
  kDot2Result_TooLargeFile = 68, ///< 너무 큰 파일
  kDot2Result_InvalidCMHF = 69, ///< 유효하지 않은 CMHF
  kDot2Result_InvalidPrivKeyType = 70, ///< 유효하지 않은 개인키 유형
  kDot2Result_InsufficientCMHFData = 71, ///< 충분하지 않은 CMHF 데이터
  kDot2Result_DeviceSpecific = 73, ///< 디바이스 의존적인 에러
  kDot2Result_InvalidLatitude = 74, ///< 유효하지 않은 위도
  kDot2Result_InvalidLongitude = 75, ///< 유효하지 않은 경도
  kDot2Result_InvalidElevation = 76, ///< 유효하지 않은 고도
  kDot2Result_NullCurrentCMH = 77, ///< 현재 사용 중인 CMH가 없음
  kDot2Result_QueueFull = 78, ///< 큐가 가득 참
  kDot2Result_NotSupportedCertIssuerIdentifierType = 79, ///< 지원하지 않는 인증서 내 상위인증서 식별자 유형
  kDot2Result_NotVerifiedCACert = 80, ///< 검증되지 않은 상위인증서
  kDot2Result_FailToRecoverY = 81, ///< Y 좌표 복구 실패
  kDot2Result_FailAES = 82, ///< AES 동작 실패
  kDot2Result_FailToExpandReconstructionKeyPair = 83, ///< 재구성 키쌍 확장 실패
  kDot2Result_FailToInitSignParameterComputeFunction = 84, ///< 서명 파라미터 계산 기능 초기화 실패
  kDot2Result_InvalidInterval = 85, ///< 유효하지 않은 주기
  kDot2Result_NoSignerIdInSPDU = 87, ///< SPDU 내에 SignerIdentifer 정보가 없음
  kDot2Result_SignerPublicKeyReconstructionRequested = 88,
  kDot2Result_ExplicitSPDUSignerIsNotSupported = 89, ///< Explicit 형식의 SPDU 서명자(인증서)는 지원되지 않음
  kDot2Result_InvalidSignerPublicKey = 90,
  kDot2Result_InvalidSPDUSignature = 91,


  kDot2Result_SignerPublicKeyReconstructionValueRecoverRequested, ///< 서명자 공개키 재구성값 복구가 요청됨

  kDot2Result_DifferentPSID , ///< SPDU를 수납한 PDU의 PSID와 SPDU 내 PSID가 다름
  kDot2Result_SPDUVerificationInNotNecessary, ///< SPDU에 대한 검증이 불필요함
  kDot2Result_InvalidPayloadSize, ///< 유효하지 않은 페이로드 길이
  kDot2Result_TooLongRandomNumberGeneratorName, ///< 너무 긴 난수생성기 이름
  kDot2Result_NoSuchDevice, ///< 그런 디바이스가 없음
  kDot2Result_NotSupportedImplicitCACertificate, ///< Implicit 형식의 CA 인증서는 지원하지 않음
  kDot2Result_TooShrotSsp, ///< 너무 짧은 SSP
  kDot2Result_InvalidSspLen, ///< 유효하지 않은 SSP 길이
  kDot2Result_InvalidLength, ///< 유효하지 않은 길이

  kDot2Result_SystemCallFailed, ///< 시스템 콜 실패
  kDot2Result_FailToCreateThread, ///< 쓰레드 생성 실패

  kDot2Result_FailECDH, ///< ECDH 동작 실패
  kDot2Result_FailKDF2, ///< KDF2 동작 실패
  kDot2Result_FailHMAC, ///< HMAC 동작 실패
  kDot2Result_FailAESEncrypt, ///< AES 암호화 실패
  kDot2Result_FailAESDecrypt, ///< AES 암호화 실패
  kDot2Result_DifferentECIESAuthenticationTag, ///< ECIES 키 복호화 시 Authentication tag의 값이 다름

  kDot2Result_InvalidCertId, ///< 유효하지 않은 인증서 ID
  kDot2Result_InvalidCertCracaId, ///< 유효하지 않은 인증서 cracaId
  kDot2Result_InvalidEncryptionPubKey, ///< 유효하지 않은 암호화용공개키
  kDot2Result_NoSignatureInCACert, ///< CA 인증서에 서명이 포함되어 있지 않음
  kDot2Result_InvalidCertCrlSeries, ///< 유효하지 않은 인증서 CRL series
  kDot2Result_InvalidCertValidTime, ///< 유효하지 않은 인증서 유효기간


  kDot2Result_SECPROFILE_NullParameters = 1000, ///< NULL 파라미터
  kDot2Result_SECPROFILE_InvalidMinimumInterCertTime, ///< 유효하지 않은 Minimum Inter Cert time
  kDot2Result_SECPROFILE_InvalidSignatureType, ///< 유효하지 않은 서명 유형
  kDot2Result_SECPROFILE_InvalidECPointType, ///< 유효하지 않은 ECC curve point 유형
  kDot2Result_SECPROFILE_NotSupportedEccCurvePointType, ///< 지원하지 않는 ECC curve point 유형
  kDot2Result_SECPROFILE_InvalidSPDUGenerationTimeSource, ///< 유효하지 않은 SPDU 생성시각 입력원
  kDot2Result_SECPROFILE_InvalidSPDUExpiryTimeSource, ///< 유효하지 않은 SPDU 만기시각 입력원
  kDot2Result_SECPROFILE_InvalidSPDUGenerationLocationSource, ///< 유효하지 않은 SPDU 생성좌표 입력원
  kDot2Result_SECPROFILE_SameSecProfileInTable, ///< 동일한 PSID를 갖는 security profile이 이미 테이블에 있음
  kDot2Result_SECPROFILE_TooManySecProfileInTable, ///< 테이블에 security profile이 너무 많음
  kDot2Result_SECPROFILE_InvalidPSID, ///< 유효하지 않은 PSID

  kDot2Result_SPDU_NullParameters = 1100, ///< NULL 파라미터
  kDot2Result_SPDU_InvalidSPDUSize, ///< 유효하지 않은 SPDU 길이
  kDot2Result_SPDU_InvalidPSID, ///< 유효하지 않은 PSID
  kDot2Result_SPDU_InvalidSPDUConstructType, ///< 유효하지 않은 SPDU 생성 유형
  kDot2Result_SPDU_InvalidSignerIdType, ///< 유효하지 않은 SignerId 유형
  kDot2Result_SPDU_InvalidPosition, ///< 유효하지 않은 position
  kDot2Result_SPDU_InvalidPayloadSize, ///< 유효하지 않은 페이로드 길이
  kDot2Result_SPDU_InvalidSignatureType, ///< 유효하지 않은 서명유형
  kDot2Result_SPDU_EncodeSPDU, ///< SPDU 인코딩 실패
  kDot2Result_SPDU_EncodePSID, ///< PSID 수납/인코딩 실패
  kDot2Result_SPDU_EncodeGenTime, ///< 생성시각 수납/인코딩 실패
  kDot2Result_SPDU_EncodeExpTime, ///< 만기시각 수납/인코딩 실패
  kDot2Result_SPDU_EncodeSignerCert, ///< 서명자인증서 수납/인코딩 실패
  kDot2Result_SPDU_EncodeToBeSignedData, ///< ToBeSignedData 필드 수납/인코딩 실패
  kDot2Result_SPDU_DecodeSPDU, ///< SPDU 디코딩 실패
  kDot2Result_SPDU_GenerateSign, ///< 서명 생성 실패
  kDot2Result_SPDU_NoSecProfile, ///< Security profile 테이블 내에 해당되는 security profile이 없음
  kDot2Result_SPDU_UndefinedCMHType, ///< CMH 유형이 정의되지 않았음
  kDot2Result_SPDU_NoAvailableCMH, ///< 가용한 CMH가 없음
  kDot2Result_SPDU_CopyCMHECKEY, ///< CMH 내 EC_KEY 형식 개인키 복사 실패
  kDot2Result_SPDU_CopyCMHAsn1Cert, ///< CMH 내 인증서 asn.1 정보 복사 실패
  kDot2Result_SPDU_TooManyEECertCache, ///< 리스트에 EE 인증서캐시정보가 너무 많음
  kDot2Result_SPDU_ConstructCertChain, ///< 인증서체인 구축 실패
  kDot2Result_SPDU_ParsePSID, ///< SPDU 파싱 실패
  kDot2Result_SPDU_NoSignerIdCertInTable, ///< 서명자식별자가 테이블에 존재하지 않음
  kDot2Result_SPDU_SignatureVerificationFailed, ///< 서명 검증 실패

  kDot2Result_SPDUConsistency_ExpTimeBeforeGenTimeInSPDU = 1150, ///< SPDU 내 만기시각이 생성시각보다 과거임
  kDot2Result_SPDUConsistency_GenTimeIsNotInSignerValidPeriod, ///< SPDU의 생성시각이 인증서 유효기간 내에 있지 않음
  kDot2Result_SPDUConsistency_ExpTimeIsNotInSignerValidPeriod, ///< SPDU의 만시각이 인증서 유효기간 내에 있지 않음
  kDot2Result_SPDUConsistency_GenLocationIsNotInSignerValidRegion, ///< SPDU의 생성좌표가 인증서 유효영역 내에 있지 않음
  kDot2Result_SPDUConsistency_InvalidSignerRegionType, ///< 인증서 유효영역 유형이 유효하지 않아 SPDU 생성좌표에 대한 consistency 체크 실패
  kDot2Result_SPDUConsistency_NotSupportedSignerRegionType, ///< 인증서 유효영역 유형이 지원되지 않아 SPDU 생성좌표에 대한 consistency 체크 실패
  kDot2Result_SPDURelevance_TooOld, ///< SPDU의 생성시각이 너무 오래되었음
  kDot2Result_SPDURelevance_Future, ///< SPDU의 생성시각이 미래임
  kDot2Result_SPDURelevance_Expiry, ///< SPDU의 만기시각이 지났음
  kDot2Result_SPDURelevance_Location, ///< SPDU의 생성위치가 너무 멈
  kDot2Result_SPDURelevance_CertExpiry, ///< SPDU의 인증서체인 내 인증서가 만기되었음
  kDot2Result_SPDURelevance_Replay, ///< 중복된 SPDU임
  kDot2Result_SPDUProcessWorkRequestQueueFull, ///< SPDU 처리작업요청 큐가 가득참
  kDot2Result_SPDUProcessWorkWaitQueueFull, ///< SPDU 처리작업대기 큐가 가득참
  kDot2Result_SPDUProcessWorkResultQueueFull, ///< SPDU 처리작업결과 큐가 가득참
  kDot2Result_SPDUProcessWorkAbnormalType, ///< 유효하지 않은 작업 유형
  kDot2Result_SPDUSignVerificationRequested, ///< 서명검증이 요청됨
  kDot2Result_SPDUProcessWorkNotFound, ///< SPDU 처리작업정보를 찾을 수 없음 (본 에러 발생 시 버그 상황임)
  kDot2Result_SPDUProcess_Finished, ///< SPDU 처리 작업 종료
  kDot2Result_SPDUProcess_RequestSignerPublicKeyReconstructionValueRecovery, ///< 공개키재구성값 Y좌표 복구 요청
  kDot2Result_SPDUProcess_RequestSignerPublicKeyReconstruction, ///< 공개키 재구성 요청
  kDot2Result_SPDUProcess_RequestSignatureVerification, ///< 서명 검증 요청
  kDot2Result_SPDUProcess_UncompressedPublicKeyReconstructionValueIsNotSupported, ///< 비압축형식의 공개키재구성값은 지원하지 않음
  kDot2Result_SPDUProcess_SignerPublicKeyReconstructionFailed, ///< 서명자 공개키 재구성 실패
  kDot2Result_SPDUProcess_SignerRevoked, ///< 서명자(인증서)가 폐기되었음


  kDot2Result_CMHF_TooLong = 1200, ///< 너무 긴 CMHF
  kDot2Result_CMHF_TooShort, ///< 너무 짧은 CMHF
  kDot2Result_CMHF_InvalidSize, ///< 유효하지 않은 CMHF 길이
  kDot2Result_CMHF_TooLongName, ///< 너무 긴 CMHF 이름
  kDot2Result_CMHF_InvalidMagicNumber, ///< CMHF 내에 포함된 매직넘버가 유효하지 않음
  kDot2Result_CMHF_InvalidCMHType, ///< CMHF 내에 포함된 CMH 유형이 유효하지 않음
  kDot2Result_CMHF_InvalidValidRegionType, ///< CMHF 내에 포함된 유효지역유형이 유효하지 않음
  kDot2Result_CMHF_InvalidCRLSeries, ///< CMHF 내에 포함된 CRL series가 유효하지 않음
  kDot2Result_CMHF_InvalidPermissionNum, ///< CMHF 내에 포함된 권한 개수가 유효하지 않음
  kDot2Result_CMHF_InvalidIdentifiedRegionNum, ///< CMHF 내에 포함된 Identified 영역 개수가 유효하지 않음
  kDot2Result_CMHF_InvalidCertSize, ///< CMHf 내에 포함된 인증서 길이가 유효하지 않음
  kDot2Result_CMHF_InvalidCertIdType, ///< CMHF 내에 포함된 인증서 ID 유형이 유효하지 않음
  kDot2Result_CMHF_InvalidPrivKeyType, ///< CMHF 내에 포함된 개인키 유형이 유효하지 않음
  kDot2Result_CMHF_InvalidCertBinaryIdLen, ///< CMHF 내에 포함된 BinaryId 유형 인증서 ID의 길이가 유효하지 않음
  kDot2Result_CMHF_InvalidCertIdHostNameLen, ///< CMHF 내에 포함된 HostName 유형 인증서 ID의 길이가 유효하지 않음
  kDot2Result_CMHF_NoIssuer, ///< CMHF 내에 포함된 인증서에 대한 상위인증서를 찾을 수 없음
  kDot2Result_CMHF_DecodeCertificate, ///< CMHF 내에 포함된 인증서 디코딩 실패
  kDot2Result_CMHF_InvalidCertValidTime, ///< CMHF 내에 포함된 인증서 유효기간이 유효하지 않음
  kDot2Result_CMHF_InvalidH8, ///< CMHF 내에 포함된 H8 값이 유효하지 않음
  kDot2Result_CMHF_InvalidJMax, ///< 유효하지 않은 J 최대값
  kDot2Result_CMHF_TooManyCert, ///< Rotate CMH 내에 인증서가 너무 많음

  kDot2Result_CMH_ConflictCMHType = 1300, ///< 충돌하는 다른 유형의 CMH가 이미 테이블에 등록되어 있음
  kDot2Result_CMH_SequentialCMHListIsFull, ///< Sequential CMH 리스트가 가득 참
  kDot2Result_CMH_RotateCMHSetListIsFull, ///< Rotate CMH 세트 리스트가 가득 참

  kDot2Result_CERT_InvalidCertSize = 1400, ///< 유효하지 않은 인증서 길이
  kDot2Result_CERT_InvalidSCCCertClass, ///< 유효하지 않은 SCC 인증서 클래스
  kDot2Result_CERT_InvalidPermissionsCount, ///< 유효하지 않은 인증서 권한 개수
  kDot2Result_CERT_InvalidCertRegionCount, ///< 유효하지 않은 인증서 유효지역 개수
  kDot2Result_CERT_InvalidDurationType, ///< 유효하지 않은 Duration 유형
  kDot2Result_CERT_SameCertInTable, ///< 동일한 인증서가 이미 테이블에 있음
  kDot2Result_CERT_NoAppPermissions, ///< 인증서 내에 어플리케이션 권한정보가 없음
  kDot2Result_CERT_InvalidSCCCertType, ///< 유효하지 않은 SCC 인증서 유형

  kDot2Result_ASN1_DecodeCertificate = 1500, ///< 인증서 ASN.1 디코딩 실패
  kDot2Result_ASN1_InvalidOctetStringLength, ///< 유효하지 않은 ASN.1 바이트열 길이
  kDot2Result_ASN1_SetInteger, ///< asn.1 정보에 정수값 설정 실패
  kDot2Result_ASN1_EncodeToBeSignedForSignature, ///< 서명 연산을 위한 ToBeSigned 필드 인코딩 실패
  kDot2Result_ASN1_EncodeSignedCertificateRequest, ///< SignedCertificateRequest 인코딩 실패
  kDot2Result_ASN1_EncodeECRequest, ///< ECRequest 인코딩 실패
  kDot2Result_ASN1_DecodeECResponse, ///< ECResponse 디코딩 실패
  kDot2Result_ASN1_DecodeScopedEeEnrollmentCertResponse, ///< ECResponse 내 ScopedEeEnrollmentCertResponse 디코딩 실패
  kDot2Result_ASN1_DecodeLCCF, ///< LCCF 디코딩 실패
  kDot2Result_ASN1_DeocdeIdCertDownloadInfoResponse, ///< 식별인증서 다운로드일정정보 디코딩 실패
  kDot2Result_ASN1_EncodeLCCFRCACert, ///< LCCF 내 RCA 인증서 인코딩 실패
  kDot2Result_ASN1_EncodeLCCFICACert, ///< LCCF 내 ICA 인증서 인코딩 실패
  kDot2Result_ASN1_EncodeLCCFPCACert, ///< LCCF 내 ACA/PCA 인증서 인코딩 실패
  kDot2Result_ASN1_EncodeLCCFCRLGCert, ///< LCCF 내 CRLG 인증서 인코딩 실패
  kDot2Result_ASN1_ParseCertPermissions, ///< 인증서 권한 파싱 실패
  kDot2Result_ASN1_EncodePseudonymCertProvisioningRequest, ///< 익명인증서발급요청 메시지 인코딩 실패
  kDot2Result_ASN1_EncodeIdCertProvisioningRequest, ///< 식별인증서발급요청 메시지 인코딩 실패
  kDot2Result_ASN1_DecodeAppCertProvisioningAck, ///< 응용인증서응답문 디코딩 실패
  kDot2Result_ASN1_DecodeIdCertProvisioningAck, ///< 식별인증서응답문 디코딩 실패
  kDot2Result_ASN1_EncodeAppCertDownloadRequest, ///< 응용인증서다운로드요청 메시지 인코딩 실패
  kDot2Result_ASN1_EncodeIdCertDownloadRequest, ///< 식별인증서다운로드요청 메시지 인코딩 실패
  kDot2Result_ASN1_DecodePseudonymCertDownloadResponse, ///< 익명인증서다운로드응답 메시지 디코딩 실패
  kDot2Result_ASN1_DecodeIdCertDownloadResponse, ///< 식별인증서다운로드응답 메시지 디코딩 실패
  kDot2Result_LCM_InvalidIdCertDownloadResponse, ///< 식별인증서다운로드응답 메시지 디코딩 실패
  kDot2Result_ASN1_EncodeCertificate, ///< 인증서 인코딩 실패
  kDot2Result_ASN1_DecodeScmsPDU, ///< SCMS PDU 디코딩 실패

  kDot2Result_ASN1_EncodeCertProvisioningRequest, ///< 인증서발급요청문 인코딩 실패
  kDot2Result_ASN1_EncodeCertDownloadRequest, ///< 인증서다운로드요청문 인코딩 실패
  kDot2Result_ASN1_DecodeCertDownloadResponse, ///< 인증서다운로드응답문 디코딩 실패
  kDot2Result_ASN1_DecodeCertProvisioningAck, ///< 인증서발급응답문 디코딩 실패
  kDot2Result_ASN1_DeocdeCertDownloadInfoResponse, ///< 인증서다운로드일정정보 디코딩 실패
  kDot2Result_ASN1_DecodeCRL, ///< CRL 디코딩 실패

  kDot2Result_FILE_Access = 1600, ///< 파일접근 실패
  kDot2Result_FILE_InvalidLength, ///< 유효하지 않은 파일 길이
  kDot2Result_FILE_Read, ///< 파일읽기 실패
  kDot2Result_FILE_Unzip, ///< 파일 압축해제 실패

  kDot2Result_OSSL_SecExecuterECGROUPInit = 1700, ///< openssl 보안연산실행자 EC_GROUP 초기화 실패
  kDot2Result_OSSL_GetPrivKeyOctsFromECKEY, ///< EC_KEY로부터 개인키바이트열 획득 실패
  kDot2Result_OSSL_MakeECPOINTfromCompressedPointOcts, ///< 압축형식 타원곡선좌표 바이트열에서 EC_POINT 생성 실패
  kDot2Result_OSSL_MakeECPOINTfromUncompressedPointOcts, ///< 비압축형식 타원곡선좌표 바이트열에서 EC_POINT 생성 실패
  kDot2Result_OSSL_MakeECKEYfromCompressedPubKeyOcts, ///< 압축형식 공개키 바이트열에서 EC_KEY 생성 실패
  kDot2Result_OSSL_GetUncompressedPubKeyOctsFromECKEY, ///< EC_KEY로부터 비압축형식 공개키 바이트열 획득 실패
  kDot2Result_OSSL_MakeECDSASIGfromSignatureOcts, ///< 서명 바이트열로부터 ECDSA_SIG 생성 실패
  kDot2Result_OSSL_ECDSA_do_verify, ///< ECDSA_SIG 서명검증 실패 (서명이 유효하지 않아서 실패한 것이 아니라 동작 에러)
  kDot2Result_OSSL_GenerateECKEY, ///< EC_KEY 생성 실패
  kDot2Result_OSSL_ComputeSigningParameters, ///< 서명생성파라미터 계산 실패
  kDot2Result_OSSL_GetSignatureOctsFromECDSASIG, ///< ECDSA_SIG 서명으로부터 서명 바이트열 획득 실패
  kDot2Result_OSSL_ECDSA_do_sign, ///< ECDSA_SIG 서명생성 실패
  kDot2Result_OSSL_ComputeSignatureS, ///< 서명 s 계산 실패
  kDot2Result_OSSL_MakeECKEYPrivKeyFromPrivKeyOcts, ///< 개인키바이트열로부터 개인키 EC_KEY 정보 생성 실패
  kDot2Result_OSSL_MakeECKEYPairFromPrivKeyOcts, ///< 개인키바이트열로부터 개인키/공개키 EC_KEY 정보 생성 실패
  kDot2Result_OSSL_MakeBIGNUMPrivKeyReconstructParamsFromOcts, ///< 개인키재구성 파라미터바이트열로부터 BIGNUM 정보 생성 실패
  kDot2Result_OSSL_ReconstructBIGNUMPrivateKey, ///< BIGNUM 형식의 개인키 재구성 실패
  kDot2Result_OSSL_MakeECKEYPrivKeyFromBIGNUMPrivKey, ///< BIGNUM 형식 개인키로부터 EC_KEY 정보 생성 실패
  kDot2Result_OSSL_MakeBIGNUMHashFromOcts, ///< 해시바이트열로부터 BIGNUM 정보 생성 실패
  kDot2Result_OSSL_ReconstructECPOINTPublicKey, ///< EC_POINT 공개키 재구성 실패
  kDot2Result_OSSL_GetUncompressedPointOctsFromECPOINT, ///< EC_POINT 형식으로부터 바이트열 획득 실패
  kDot2Result_OSSL_InvalidReconstructedKeyPair, ///< 재구성된 키쌍이 유효하지 않음(쌍을 이루지 않음)
  kDot2Result_OSSL_MakeCocoonPrivateKey, ///< Cocoon 키 생성 실패
  kDot2Result_OSSL_ReconverY, ///< Y좌표 복구 실패
  kDot2Result_OSSL_LinkageValue, ///< LinkageValue 계산 실패

  kDot2Result_LCM_InvalidCertDurationType = 1800, ///< 인증서 duration 유형이 유효하지 않음
  kDot2Result_LCM_InvalidCertRegionNum, ///< 인증서 유효지역 개수가 유효하지 않음
  kDot2Result_LCM_InvalidCertPermissionNum, ///< 인증서 유효지역 개수가 유효하지 않음
  kDot2Result_LCM_InvalidSPDUSize, ///< SPDU의 길이가 유효하지 않음
  kDot2Result_LCM_InvalidCertSize, ///< 인증서의 길이가 유효하지 않음
  kDot2Result_LCM_InvalidLCCFSize, ///< LCCF의 길이가 유효하지 않음
  kDot2Result_LCM_DifferentECRequestHash, ///< 등록인증서 발급응답문의 requestHash값이 다름
  kDot2Result_LCM_InvalidECResponseContent, ///< 등록인증서 발급응답문의 내용이 유효하지 않음
  kDot2Result_LCM_NoEnrollmentCMHInfo, ///< 등록인증서 CMH 정보가 존재하지 않음
  kDot2Result_LCM_NoRACertInfo, ///< RA 인증서 정보가 존재하지 않음
  kDot2Result_LCM_NoACACertInfo, ///< ACA 인증서 정보가 존재하지 않음
  kDot2Result_LCM_NoSufficientCertRequestInfo, ///< 충분한 인증서 요청 관련 정보가 존재하지 않음
  kDot2Result_LCM_HTTPS_curl_easy_setopt, ///< HTTPS POST/GET 진행 중 CURL 설정 실패
  kDot2Result_LCM_HTTPS_curl_easy_init, ///< HTTPS POST/GET 진행 중 CURL 초기화 실패
  kDot2Result_LCM_HTTPS_curl_easy_perform, ///< HTTPS POST/GET 진행 중 CURL 실행 실패
  kDot2Result_LCM_HTTPS_NoConnectionInfo, ///< HTTPS 접속정보가 없음
  kDot2Result_LCM_HTTPS_InvalidResponse, ///< 유효하지 않은 HTTPS response
  kDot2Result_LCM_HTTPS_NoModifiedFile, ///< 수정된 파일이 존재하지 않음 (If-None-Match 관련)
  kDot2Result_LCM_HTTPS_DownloadInfoUnvailable, ///< RA서버로부터의 다운로드가 일시적으로 용이하지 않아 차후 재시도 필요
  kDot2Result_LCM_HTTPS_NoKeyValueInHeader, ///< HTTPS 헤더 내 Key value가 없음
  kDot2Result_LCM_HTTPS_InvalidFileNameLenInHeader, ///< HTTPS 헤더 내 파일명의 길이가 유효하지 않음
  kDot2Result_LCM_HTTPS_ServerError, ///< 서버에서 에러를 리턴함.
  kDot2Result_LCM_InvalidAppCertProvisioningAck, ///< 유효하지 않은 응용인증서발급응답문
  kDot2Result_LCM_InvalidIdCertProvisioningAck, ///< 유효하지 않은 식별인증서발급응답문
  kDot2Result_LCM_InvalidPseudonymCertDownloadResponse, ///< 유효하지 않은 익명인증서다운로드응답문
  kDot2Result_LCM_PseudonymCertDownloadResponseDecryption, ///< 익명인증서다운로드응답문 복호화 실패
  kDot2Result_LCM_IdCertDownloadResponseDecryption, ///< 식별인증서다운로드응답문 복호화 실패
  kDot2Result_LCM_DifferentAppCertProvisioningAckRequestHash, ///< 응용인증서발급응답문 내 requestHash값이 다름
  kDot2Result_LCM_DifferentIdCertProvisioningAckRequestHash, ///< 식별인증서발급응답문 내 requestHash값이 다름
  kDot2Result_LCM_Unzip, ///< ZIP 압축해제 실패
  kDot2Result_LCM_InvalidConfigType, ///< 유효하지 않은 LCM 설정 유형


  kDot2Result_LCM_InvalidCertDownloadResponse, ///< 유효하지 않은 인증서다운로드응답문
  kDot2Result_LCM_DecryptCertDownloadResponse, ///< 인증서다운로드응답문 복호화 실패
  kDot2Result_LCM_InvalidCertProvisioningAck, ///< 유효하지 않은 인증서발급응답문
  kDot2Result_LCM_DifferentCertProvisioningAckRequestHash, ///< 인증서발급응답문 내 requestHash값이 다름

  kDot2Result_CRL_InvalidContents = 1900, ///< CRL의 내용이 유효하지 않음
  kDot2Result_CRL_InvalidPeriod, ///< CRL의 현재 유효하지 않음
  kDot2Result_CRL_InvalidSize, ///< CRL의 길이가 유효하지 않음
  kDot2Result_CRL_Add, ///< 인증서폐기정보 추가 실패
  kDot2Result_CRL_Full, ///< 테이블이나 리스트가 가득 참
  kDot2Result_CRL_InvalidSignerId, ///< CRL 내 Signer 정보가 유효하지 않음

  kDot2Result_Undefined = 2000, ///< 로직상 발생할 수 없는 결과

};
typedef int Dot2ResultCode; ///< @ref eDot2ResultCode


/**
 * @brief 로그메시지 출력 레벨
 *
 * 라이브러리 내 로그전역변수에 설정된 레벨 이하의 로그가 출력된다. \n
 * 라이브러리 내 로그전역변수 레벨 설정은 Dot2_Init() API 를 통해 수행된다.
 */
enum eDot2LogLevel
{
  kDot2LogLevel_None = 0, ///< 아무 로그도 출력하지 않는다.
  kDot2LogLevel_Err, ///< 에러 로그를 출력한다.
  kDot2LogLevel_Init, ///< 초기화 로그를 출력한다.
  kDot2LogLevel_Event, ///< 각종 이벤트(패킷 송수신, 설정 등)에 관련된 로그를 출력한다.
  kDot2LogLevel_Dump, ///< 패킷 덤프 데이터 로그를 출력한다.
  kDot2LogLevel_All, ///< 모든 로그를 출력한다.
  kDot2LogLevel_Min = kDot2LogLevel_None,
  kDot2LogLevel_Max = kDot2LogLevel_All
};
typedef unsigned int Dot2LogLevel; ///< @ref eDot2LogLevel


/**
 * @brief 인증서 길이
 */
enum eDot2CertSize
{
  kDot2CertSize_Min = 12, ///< 처리 가능한 최소 길이 (DOT2_GET_ISSUER_SIGNED_CERT_TBS 매크로가 적용될 수 있는 최소 길이)
  kDot2CertSize_Max = 300, ///< 처리 가능한 최대길이 (자체적으로 정의함)
};
typedef size_t Dot2CertSize; ///< @ref eDot2CertSize


/**
 * @brief 인증서 j 값 개수 (동일 i-period 내에서 돌려 쓰는 인증서들의 j 값)
 */
enum eDot2CertJvalue
{
  /// 동일 i-period 내에서 돌려쓰는 식별인증서들의 j값 중 최대값
  /// (CAMP SCMS 규격 및 KISA v1.1 규격 기준)
  kDot2CertJvalue_IdMax = DOT2_DEFAULT_I_CERTS_PER_I_PERIOD - 1,

  /// 동일 i-period 내에서 돌려쓰는 익명인증서들의 j값 중 최대값
  /// (CAMP SCMS 규격 및 KISA v1.1 규격 기준)
  kDot2CertJvalue_PseudonymMax = DOT2_DEFAULT_P_CERTS_PER_I_PERIOD - 1,
  kDot2CertJvalue_Max = kDot2CertJvalue_PseudonymMax,
};
typedef uint32_t Dot2CertJvalue; ///< @ref eDot2CertJvalue


/**
 * @brief CMHF 사이즈
 */
enum eDot2CMHFSize
{
  kDot2CMHFSize_Min = 28, ///< 최소길이 (magic number + 필수공통정보)
  kDot2CMHFSize_Max = (kDot2CertSize_Max * (kDot2CertJvalue_Max + 1)) + 300, ///< 최대길이
};
typedef size_t Dot2CMHFSize; ///< @ref eDot2CMHFSize


/**
 * @brief LCCF(Local Certificate Chain File) 길이
 */
enum eDot2LCCFSize
{
  kDot2LCCFSize_Max = 16384, ///< 처리 가능한 최대 길이 (자체적으로 정의함)
};
typedef size_t Dot2LCCFSize; ///< @ref eDot2LCCFSize


/**
 * @brief 윤초
 */
enum eDot2LeapSeconds
{
  kDot2LeapSeconds_From2004 = 5, ///< Time32/Time64 값의 시작점인 2004-01-01로부터 현 시점(2022년 12월 기준)까지 적용된 윤초
                                 ///< 이후 언젠가 윤초가 추가되면 이 값도 증가해야 한다.
  kDot2LeapSeconds_Default = kDot2LeapSeconds_From2004
};
typedef unsigned int Dot2LeapSeconds; ///< @ref eLeapSeconds


/**
 * @brief CRL(Certificate Revocation List) 길이
 */
enum eDot2CRLSize
{
  kDot2CRLSize_Min = 1,
  kDot2CRLSize_Max = 1000*1000*1000, ///< 처리 가능한 최대 길이 (자체적으로 정의함)
};
typedef size_t Dot2CRLSize; ///< @ret eDot2CRLSize


/**
 * @brief 서명파라미터 사전 계산 주기(밀리초 단위). Openssl 기반 서명 생성 알고리즘 사용시에만 적용된다.
 *
 * 본 주기마다 서명 생성을 위한 파라미터가 사전 계산되어 내부에 저장되며,
 * 서명 생성 시점에는 저장된 파라미터를 이용하여 서명을 생성함으로써, 서명 생성 시간을 감소시킬 수 있다.
 */
enum eDot2SigningParamsPrecomputeInterval
{
  kDot2SigningParamsPrecomputeInterval_Min = 10, ///< 최소 주기
  kDot2SigningParamsPrecomputeInterval_Max = 100,
  kDot2SigningParamsPrecomputeInterval_Default = kDot2SigningParamsPrecomputeInterval_Max,
  kDot2SigningParamsPrecomputeInterval_NotUse = 0, ///< 사전 계산 기능을 사용하지 않고, 서명 생성 시에 실시간으로 계산.
};
typedef unsigned int Dot2SigningParamsPrecomputeInterval; ///< @ref eDot2SigningParamsPrecomputeInterval


/**
 * @brief Time64 형식
 *
 * TAI(=2004-01-01 00:00:00 UTC 로부터) 마이크로초 값을 나타낸다.
 * 변수의 크기 상, 2004-01-01 이후로부터 약 584,942년 동안을 나타낼 수 있음.
 *
 * 2022-12-30 기준으로, 이 값은 UTC 기반 시스템시각보다 5초 빠르다(크다)
 * (2004-01-01 ~ 2022-12-30 구간에서는 UTC에 5초의 보정이 이루어졌다)
 */
typedef uint64_t Dot2Time64;


/**
 * @brief SystemTime 형식(자체 정의)
 *
 * 마이크로초 단위의 UTC 리눅스시스템시각을 나타낸다 (1970-01-01 00:00:00 이후부터의 마이크로초 값)
 */
typedef uint64_t Dot2SystemTime;


/**
 * @brief 인증서 ID 필드에 수납되는 Hostname의 길이
 */
enum eDot2CertIdHostNameLen
{
  kDot2CertIdHostNameLen_Min = 0, ///< per IEEE 1609.2-2016 asn.1
  kDot2CertIdHostNameLen_Max = 255 ///< per IEEE 1609.2-2016 asn.1
};
typedef unsigned int Dot2CertIdHostNameLen; ///< @ref eDot2CertIdHostNameLen


/**
 * @brief Ieee1609Dot2Content 유형
 */
enum eDot2ContentType
{
  kDot2Content_UnsecuredData, ///< 비보안 데이터
  kDot2Content_SignedData, ///< 서명된 데이터
  kDot2Content_EncryptedData, ///< 암호화된 데이터
  kDot2Content_SignedCertificateRequest, ///< 서명된 인증서 요청
  kDot2Content_Min = kDot2Content_UnsecuredData,
  kDot2Content_Max = kDot2Content_SignedCertificateRequest
};
typedef unsigned int Dot2ContentType; ///< @ref eDot2ContentType


/**
 * @brief 서명자 식별자 유형
 */
enum eDot2SignerIdType
{
  kDot2SignerId_Digest, ///< 다이제스트
  kDot2SignerId_Certificate, ///< 인증서
  kDot2SignerId_Self, ///< Self
  kDot2SignerId_Profile, ///< Security Profile에 따른 서명자식별자가 적용된다(서명 생성 시에만 사용됨).
  kDot2SignerId_Min = kDot2SignerId_Digest,
  kDot2SignerId_Max = kDot2SignerId_Profile
};
typedef unsigned int Dot2SignerIdType; ///< @ref eDot2SignerIdType


/**
 * @brief ECC 커브 포인트 유형
 */
enum eDot2EcCurvePointType
{
  kDot2EcCurvePoint_X_only = 1,
  kDot2EcCurvePoint_Compressed_y_0 = 2,
  kDot2EcCurvePoint_Compressed_y_1 = 3,
  kDot2EcCurvePoint_Uncompressed = 4,
  kDot2EcCurvePoint_Min = kDot2EcCurvePoint_X_only,
  kDot2EcCurvePoint_Max = kDot2EcCurvePoint_Uncompressed
};
typedef unsigned int Dot2EcCurvePointType; ///< @ref eEcCurvePointType


/**
 * @brief PSID
 */
enum eDot2PSID
{
  kDot2PSID_Min = 0,
  kDot2PSID_SCMS = 35,
  kDot2PSID_Max = 270549119, // =0x1020407F,
  kDot2PSID_IPv6Routing = 270549118, // =0x1020407E,
  kDot2PSID_WSA = 135, // =0x87,
  kDot2PSID_CRL = 256, // =0x100
};
typedef unsigned int Dot2PSID; ///< @ref eDot2PSID


/**
 * @brief 인증서정보 내 포함되는 권한 개수
 */
enum eDot2CertPermissionNum
{
  kDot2CertPermissionNum_Max = 30, ///< 최대개수(자체 정의)
};
typedef unsigned int Dot2CertPermissionNum; ///< @ref eDot2CertPermissionNum



/**
 * @brief 1609.2 메시지(헤더 + 페이로드)의 길이
 */
enum eDot2MsgSize
{
  kDot2MsgSize_Min = 3, ///< 메시지 최소 길이(Unsecured 형식이고 페이로드 길이가 0인 경우)
  kDot2MsgSize_Max = 2000, ///< 메시지 최대 길이 (자체적으로 정의함)

  kDot2MsgSize_MaxPayload = (kDot2MsgSize_Max - kDot2MsgSize_Min), ///< 페이로드 최대길이
};
typedef size_t Dot2MsgSize; ///< @ref eDot2MsgSize


/**
 * @brief SPDU(1609.2 헤더 + 페이로드)의 길이
 */
enum eDot2SPDUSize
{
  kDot2SPDUSize_Min = 3, ///< 최소 길이(Unsecured 형식이고 페이로드 길이가 0인 경우)
  kDot2SPDUSize_Max = 2000, ///< 최대 길이 (자체적으로 정의함)
  kDot2SPDUSize_MinPayload = 0, ///< 페이로드 최소길이
  kDot2SPDUSize_MaxPayload = (kDot2SPDUSize_Max - kDot2SPDUSize_Min), ///< 페이로드 최대길이
};
typedef size_t Dot2SPDUSize; ///< @ref eDot2SPDUSize


/**
 * @brief SPDU 생성 유형
 */
enum eDot2SPDUConstructType
{
  kDot2SPDUConstructType_Unsecured, ///< Unsecured SPDU 생성
  kDot2SPDUConstructType_Signed, ///< SignedData SPDU 생성

  kDot2SPDUConstructType_Max = kDot2SPDUConstructType_Signed
};
typedef unsigned int Dot2SPDUConstructType; ///< @ref eDot2SPDUConstructType


/**
 * @brief 타원곡선 유형
 */
enum eDot2ECType
{
  kDot2ECType_NISTp256, ///< NIST P-256 타원곡선
  kDot2ECType_Brainpoolp256r1, ///< Brainpool P256r1 타원곡선
  kDot2ECType_Max = kDot2ECType_Brainpoolp256r1,
};
typedef unsigned int Dot2ECType; ///< @ref eDot2EcType


/**
 * @brief 타원곡선 좌표 형식 (좌표 바이트열의 앞부분에 위치한다 -> 좌표 형식: "Form || X || Y")
 */
enum eDot2ECPointForm
{
  kDot2ECPointForm_X_only = 0x00, ///< 좌표가 X-only 형식임을 나타내는 프리픽스 값(서명형식에서만 사용된다)
  kDot2ECPointForm_Compressed_y_0 = 0x02, ///< 좌표가 compressed-y-0 형식임을 나타내는 프리픽스 값
  kDot2ECPointForm_Compressed_y_1 = 0x03, ///< 좌표가 compressed-y-1 형식임을 나타내는 프리픽스 값
  kDot2ECPointForm_Uncompressed = 0x04, ///< 좌표가 uncompressed 형식임을 나타내는 프리픽스 값
  kDot2ECPointForm_Compressed = 0x10, ///< 좌표가 Compressed 형식임을 나타내는 프리픽스 값(편의를 위해 추가)
};
typedef uint8_t Dot2ECPointForm; ///< @ref eDot2EcPointForm


/**
 * @brief CMHF 이름 길이
 */
enum eDot2CMHFNameLen
{
  kDot2CMHFNameLen_Max = 255,
};
typedef unsigned int Dot2CMHFNameLen; ///< @ref eDot2CMHFNameLen


/**
 * @brief Time32 형식.
 *
 * TAI(=2004-01-01 00:00:00 UTC 로부터) 초 값을 나타낸다.
 * 변수의 크기 상, 2004-01-01 이후로부터 약 136년 동안을 나타낼 수 있음.
 *
 * 2022-12-30 기준으로, 이 값은 UTC 기반 시스템시각보다 5초 빠르다(크다)
 * (2004-01-01 ~ 2022-12-30 구간에서는 UTC에 5초의 보정이 이루어졌다)
 */
typedef uint32_t Dot2Time32;


/**
 * @brief 인증서로 서명되는 주기 (마이크로초 단위)
 *
 * 1609.2 Security profile에 따르면, SPDU는 인증서로 서명되기도 하고 다이제스트로 서명되기도 한다. \n
 * 한번 인증서로 서명되면, 본 주기만큼 지나기 전까지는 다이제스트로 서명되고 본 주기가 지나면 인증서로 서명된다.
 */
typedef Dot2Time64 Dot2SecProfileInterCertTime;
#define kDot2SecProfileInterCertTime_Always (0ULL) ///< 항상 인증서로 서명된다.
#define kDot2SecProfileInterCertTime_Min (kDot2SecProfileInterCertTime_Always)
#define kDot2SecProfileInterCertTime_Max (60000000000ULL) ///< 최대값 자체 정의


/**
 * @brief 서명 수행 시, 생성될 서명의 유형
 */
enum eDot2SecProfileSignType
{
  kDot2SecProfileSign_X_only, ///< x-only 서명이 사용된다.
  kDot2SecProfileSign_Uncompressed, ///< 비압축형식의 fase verification 서명이 사용된다.
  kDot2SecProfileSign_Compressed, ///< 압축형식의 fast verification 서명이 사용된다.
  kDot2SecProfileSign_Min = kDot2SecProfileSign_X_only,
  kDot2SecProfileSign_Max = kDot2SecProfileSign_Compressed,
};
typedef unsigned int Dot2SecProfileSignType; ///< @ref eDot2SecProfileSignType


/**
 * @brief Ieee1609Dot2 메시지 내의 수납되는 EC point 들의 형식
 *
 * IEEE 1609.2-2016 p.120의 내용에 따르면,
 * 인증서로 서명되는 Ieee1609Dot2Data 내 인증서들의 공개키나 공개키재구성값에 적용된다고 되어 있다.
 * (그 외에는 EC point 형식의 필드가 없다) \n
 * 하지만, 인증서는 CA가 만들어서 배포한 인증서이기 때문에, 내 자신이 이 안의 값을 자체적으로 바꾸는 것은 말이 안 된다고 본다.\n
 * 따라서 일단 이 설정은 무시하고, 원 인증서를 그대로 사용한다.
 */
enum eDot2SecProfileEcPointFormat
{
  kDot2SecProfileEcPointFormat_Uncompressed, ///< 비압축 형식
  kDot2SecProfileEcPointFormat_Compressed, ///< 압축 형식
  kDot2SecProfileEcPointFormat_Min = kDot2SecProfileEcPointFormat_Uncompressed,
  kDot2SecProfileEcPointFormat_Max = kDot2SecProfileEcPointFormat_Compressed,
};
typedef unsigned int Dot2SecProfileEcPointFormat; ///< @ref eDot2SecProfileEcPointFormat


/**
 * @brief Latitude (위도, 0.1 마이크로도 단위)
 */
enum eDot2Latitude
{
  kDot2Latitude_Min = -900000000,
  kDot2Latitude_Max = 900000000,
  kDot2Latitude_Unavailable = 900000001
};
typedef int32_t Dot2Latitude; ///< @ref eDot2Latitude


/**
 * @brief Longitude (경도, 0.1 마이크로도 단위)
 */
enum eDot2Longitude
{
  kDot2Longitude_Min = -1799999999,
  kDot2Longitude_Max = 1800000000,
  kDot2Longitude_Unavailable = 1800000001
};
typedef int32_t Dot2Longitude; ///< @ref eDot2Longitude


/**
 * @brief Elevation (고도, 10cm 단위)
 *
 * -409.5m일때 0의 값을 가진다.
 */
enum eDot2Elevation
{
  kDot2Elevation_Min = 0, ///< = -409.5 m
  kDot2Elevation_Max = (4095 + 61439) /*=65534*/, ///< 6143.9 m
  kDot2Elevation_Unavailable = (kDot2Elevation_Max + 1),
};
typedef uint16_t Dot2Elevation; ///< @ref eDot2Elevation



/**
 * @brief 서명 유형
 */
enum eDot2SignatureType
{
  kDot2SignatureType_NISTp256, ///< NISTp256 타원곡선 기반 서명
  kDot2SignatureType_Brainpoolp56r1, ///< Brainpoolp256r1 타원곡선 기반 서명
};
typedef unsigned int Dot2SignatureType; ///< @ref eDot2SignatureType


/**
 * @brief Relevance 체크에 사용되는 시간값의 출처 (= 시간 정보가 존재하는 위치)
 */
enum eDot2RelevanceTimeSource
{
  kDot2RelevanceTimeSource_SecurityHeader, ///< 보안 헤더에 수납된 시간값을 이용하여 Relevance 체크
  kDot2RelevanceTimeSource_Payload, ///< 페이로드에 수납된 시간값을 이용하여 Relevance 체크 (지원되지 않는다)
  kDot2RelevanceTimeSource_Min = kDot2RelevanceTimeSource_SecurityHeader,
  kDot2RelevanceTimeSource_Max = kDot2RelevanceTimeSource_SecurityHeader,
};
typedef unsigned int Dot2RelevanceTimeSource; ///< @ref eDot2RelevanceTimeSource


/**
 * @brief Consistency 체크에 사용되는 좌표값의 출처 (= 좌표 정보가 존재하는 위치)
 */
enum eDot2ConsistencyLocationSource
{
  kDot2ConsistencyLocationSource_SecurityHeader, ///< 보안 헤더에 수납된 좌표값을 이용하여 Consistency 체크
  kDot2ConsistencyLocationSource_Payload, ///< 페이로드에 수납된 좌표값을 이용하여 Consistency 체크 (지원되지 않는다)
  kDot2ConsistencyLocationSource_Min = kDot2ConsistencyLocationSource_SecurityHeader,
  kDot2ConsistencyLocationSource_Max = kDot2ConsistencyLocationSource_SecurityHeader,
};
typedef unsigned int Dot2ConsistencyLocationSource; ///< @ref eDot2ConsistencyLocationSource


/**
 * @brief 인증서 내 Identtfied 유형 유효지역을 나타내는 국가식별자
 */
enum eDot2CountryCode
{
  kDot2CountryCode_Min = 0,
};
typedef uint16_t Dot2CountryCode; ///< @ref eDot2CountryCode


/**
 * @brief 인증서 내 Identified 유형 유효지역 개수
 */
enum eDot2IdentifiedRegionNum
{
  kDot2IdentifiedRegionNum_Max = 20, ///< 최대개수(구현에 따라 20 이상으로 변경 가능, 표준의 기본값은 8)
};
typedef unsigned int Dot2IdentifiedRegionNum; ///< @ref eDot2IdentifiedRegionNum


/**
 * @brief 인증서 SubjectPermissions 내에 저장되는 PsidSspRange 개수
 */
enum eDot2CertPsidSspRangeNum
{
  kDot2CertPsidSspRangeNum_Max = 8, ///< 최대개수(표준에 정해져 있지 않아 임의로 정함)
};
typedef unsigned int Dot2CertPsidSspRangeNum; ///< @ref eDot2CertPsidSspRangeNum


/**
 * @brief 인증서 유효기간 duration 유형
 */
enum eDot2CertDurationType
{
  kDot2CertDurationType_Microseconds,
  kDot2CertDurationType_Milliseconds,
  kDot2CertDurationType_Seconds,
  kDot2CertDurationType_Minutes,
  kDot2CertDurationType_Hours,
  kDot2CertDurationType_SixtyHours,
  kDot2CertDurationType_Years,
  kDot2CertDurationType_Max
};
typedef unsigned int Dot2CertDurationType; ///< @ref eDot2CertDurationType


/**
 * @brief LCM 설정 유형
 */
enum eDot2LCMConfigType
{
  kDot2LCMConfigType_LPFReqURL, ///< LPF 발급요청 URL 설정
  kDot2LCMConfigType_LCCFReqURL, ///< LCCF 발급요청 URL 설정
  kDot2LCMConfigType_CRLReqURL, ///< CRL 요청 URL 설정
  kDot2LCMConfigType_AppCertProvisioningReqURL, ///< 응용인증서 발급요청 URL 설정
  kDot2LCMConfigType_PseudonymCertProvisioningReqURL, ///< 익명인증서 발급요청 URL 설정
  kDot2LCMConfigType_IdCertProvisioningReqURL, ///< 식별인증서 발급요청 URL 설정
  kDot2LCMConfigType_RCATLSCertFilePath, ///< RootCA TLS 인증서 파일 경로 설정
  kDot2LCMConfigType_TmpZipFilePath, ///< 동작 중 임시파일이 저장될 디렉토리 경로 설정
  kDot2LCMConfigType_Max = kDot2LCMConfigType_TmpZipFilePath
};
typedef unsigned int Dot2LCMConfigType; ///< @ref eDot2LCMConfigType


/**
 * @brief Butterfly key 메커니즘을 사용하는 Pseudonym 인증서에서 사용되는 i-Period 값
 *
 * KISA v1.1 규격에 따라, 2015년 1월 6일 오전 2:00가 i=0인 기준시각이다.
 * 미국 CAMP 규격은 미국동부시 기준 2015년 1월 6일 오전 4:00를 기준으로 하고 있으며,
 * 이를 한국시각으로 변경하면 2015년 1월 6일 18:00이다.
 * 즉, KISA 규격과 CAMP 규격의 i=0 시점이 다르며, 여기서는 KISA 규격을 따른다.
 *
 * KISA 규격 기준 i=0인 2015년 1월 6일 오전 2:00는 1609.2 시각(Dot2Time32, 2014년 1월 1일 이후) 기준으로 347,562,003초이다.
 */
enum eDot2IPeriod
{
  kDot2IPeriod_Current = 0, ///< 현재 i-period 값을 의미
  kDot2IPeriod_Next = 1,  ///< 현 시점 기준, 다음번 i-period 값을 의미

  // 2023-01-29 시점 기준으로 i-period 값은 420(0x1A4)이다.
  // 따라서 앞으로도 i-period=0/1 값은 사용될 일이 없으므로 실제값이 아닌, 특정 의미(이번주, 다음주)를 갖는 값으로 정의하여 사용한다. (편의성을 위해)
};
typedef unsigned int Dot2IPeriod; ///< @ref eDot2IPeriod
#define DOT2_ZERO_I_PERIOD_TIME32 (347562003UL)


/**
 * @brief Butterfly key 메커니즘을 사용하는 Identification 인증서 다운로드 시, 다운로드하고자 하는 인증서 타겟 시점
 *
 * 0일 경우, 현시점에 해당되는 인증서 다운로드를 요청하며,
 * 1 이상의 값일 경우, 현 시점으로부터 (해당 값에 식별인증서 유효기간을 곱한 만큼 이후의 시점)에 해당되는 인증서 다운로드를 요청한다.
 */
enum eDot2IdCertTargetTime
{
  Dot2IdCertTargetTime_Current = 0, ///< 현 시점에 해당되는 인증서 다운로드 요청
};
typedef unsigned int Dot2IdCertTargetTime; ///< @ref eDot2IdCertTargetTime


/**
 * @brief ECC 개인키 바이트열
 */
struct Dot2ECPrivateKey
{
  uint8_t octs[DOT2_EC_256_KEY_LEN]; ///< 바이트열
} __attribute__((packed));


/**
 * @brief ECC 타원곡선좌표(=공개키,서명R) 바이트열
 */
struct Dot2ECPoint
{
  union {
    uint8_t octs[DOT2_EC_256_POINT_LEN]; ///< 바이트열
    struct {
      Dot2ECPointForm form; ///< 형식 (Form)
      union {
        uint8_t point[DOT2_EC_256_KEY_LEN*2]; ///< X 좌표 || Y 좌표
        struct {
          uint8_t x[DOT2_EC_256_KEY_LEN]; ///< X 좌표
          uint8_t y[DOT2_EC_256_KEY_LEN]; ///< Y 좌표
        } xy;
      } u;
    } point;
  } u;
} __attribute__((packed));

#define Dot2ECPublicKey Dot2ECPoint


/**
 * @brief AES 키 바이트열
 */
struct Dot2AESKey
{
  uint8_t octs[DOT2_AES_128_LEN]; ///< 바이트열
} __attribute__((packed));


/**
 * @brief SHA256 해시 바이트열
 */
struct Dot2SHA256
{
  uint8_t octs[DOT2_SHA_256_LEN]; ///< 바이트열
} __attribute__((packed));


/**
 * @brief SPDU 처리 결과를 어플리케이션에 전달하는 콜백함수
 * @param[in] result 처리 결과
 * @param[in] priv Dot2_ProcessSPDU() API를 통해 전달된 패킷파싱데이터(struct V2XPacketParseData).
 *                 SPDU의 헤더정보 및 페이로드 데이터가 채워져서 전달된다.
 */
typedef void (*ProcessSPDUCallback)(Dot2ResultCode result, void *priv);


/**
 * @brief 송신에 관련된 Security profile 정보 형식
 */
struct Dot2TxSecProfile
{
  bool gen_time_hdr; ///< Security header에 Generation Time 필드 수납 여부
  bool exp_time_hdr; ///< Security header에 Expiry Time 필드 수납 여부
  bool gen_location_hdr; ///< Security header에 Generation Location 필드 수납 여부
  Dot2Time64 spdu_lifetime; ///< SPDU의 수명. exp_time_hdr=true일 경우, 생성시각에 본 값을 더해서 만기시각을 정한다.
  Dot2SecProfileInterCertTime min_inter_cert_time; ///< 인증서 서명 주기 (본 값만큼 경과하면 인증서로 서명한다)
  Dot2SecProfileSignType sign_type; ///< fast verification 서명 사용 여부(표준 상, 현재 Compressed만 지원)
  Dot2SecProfileEcPointFormat ecp_format; ///< 메시지에 수납되는 EC point 형식(표준 상, 현재 Compressed만 지원)
  unsigned int interval; ///< 본 PSID에 해당되는 메시지가 생성/서명되는 주기(밀리초 단위) - 인증서 만기 여부를 미리 체크하는데 사용된다.
                         ///< 0으로 설정될 경우 체크하지 않는다.
};


/**
 * @brief Relevance 체크에 관련된 Security profile 정보 형식
 */
struct Dot2RxRelevanceCheckSecProfile
{
  bool replay; ///< 중복 메시지 폐기 여부. 참일 경우 지난 validity_period 동안 동일한 SPDU가 수신된 적이 있으면 SPDU를 폐기한다.
  bool gen_time_in_past; ///< 너무 오래된 SPDU 폐기 여부. 참일 경우, 생성시각이 "현재시각-validity_period"보다 과거인 SPDU는 폐기한다.
  Dot2Time64 validity_period; ///< replay, gen_time_in_past 체크에 사용되는 시간 기준값.
  bool gen_time_in_future; ///< 미래의 SPDU 폐기 여부. 참일 경우, 생성시각이 "현재시각+acceptable_future_data_period"보다 미래인 SPDU는 폐기한다.
  Dot2Time64 acceptable_future_data_period; ///< gen_time_in_future 체크에 사용되는 시간 기준값.
  Dot2RelevanceTimeSource gen_time_src; ///< gen_time_in_past, gen_time_in_future 체크에 사용되는 생성시각정보의 출처
  bool exp_time; ///< 만기된 SPDU 폐기 여부. 참일 경우, 만기시각이 현재시각보다 과거인 SPDU는 폐기한다.
  Dot2RelevanceTimeSource exp_time_src; ///< expiry_time 체크에 사용되는 만기시각정보의 출처
  bool gen_location_distance; ///< 너무 먼 곳에서 생성된 SPDU 폐기 여부. 참일 경우, 생성위치가 "현재위치+valid_distance"보다 먼 SPDU는 폐기한다.
  unsigned int valid_distance; ///< gen_location_distance 체크에 사용되는 거리 기준값(미터단위).
  Dot2ConsistencyLocationSource gen_location_src; ///< gen_location_distance 체크에 사용되는 생성좌표정보의 출처
  bool cert_expiry; ///< 만기된 인증서로 서명된 SPDU 폐기 여부. 참일 경우, 수납된 인증서의 만기시각이 현재시각보다 과거인 SPDU는 폐기한다.
};


/**
 * @brief Consistency 체크에 관련된 Security profile 정보 형식
 */
struct Dot2RxConsistencyCheckSecProfile
{
  bool gen_location; ///< 유효하지 않은 생성좌표를 갖는 SPDU 폐기 여부. 참일 경우, 생성좌표가 인증서 유효영역을 벗어난 SPDU는 폐기한다.
  Dot2Time32 overdue_crl_tolerance; ///< 현재 사용되지 않는다.
};


/**
 * @brief 수신에 관련된 Security profile 정보 형식
 */
struct Dot2RxSecProfile
{
  bool verify_data; ///< 데이터 검증 수행 여부. 참일 경우, relevance/consistency 체크 및 서명 검증이 수행된다.
  struct Dot2RxRelevanceCheckSecProfile relevance_check;
  struct Dot2RxConsistencyCheckSecProfile consistency_check;
};


/**
 * @brief 인증서 유효기간 duration 정보
 */
struct Dot2CertValidDuration
{
  Dot2CertDurationType type; ///< duration 유형
  uint16_t duration; ///< duration 값
};


/**
 * @brief 인증서 유효기간 정보
 */
struct Dot2CertValidPeriod
{
  Dot2Time32 start; ///< 시작 시점
  struct Dot2CertValidDuration duration; ///< 기간
};


/**
 * @brief 인증서 정보
 *
 * 본 구조체는 항상 인증서 최대길이만큼의 메모리를 차지하므로, 라이브러리에서 지속적으로 사용되는 내부 저장소에서는 사용하지 않는다.
 * 내부 저장소에서 사용되는 인증서바이트열은 동적할당된 정보를 사용한다.
 */
struct Dot2Cert
{
  uint8_t octs[kDot2CertSize_Max]; ///< 인증서 바이트열
  Dot2CertSize size; ///< 인증서 바이트열의 길이
};


/**
 * @brief 2D 좌표 정보
 */
struct Dot2TwoDLocation
{
  Dot2Latitude lat; ///< 위도
  Dot2Longitude lon; ///< 경도
};


/**
 * @brief 3D 좌표 정보
 */
struct Dot2ThreeDLocation
{
  Dot2Latitude lat; ///< 위도
  Dot2Longitude lon; ///< 경도
  Dot2Elevation elev; ///< 고도
};


#endif //V2X_SW_DOT2_TYPES_H
