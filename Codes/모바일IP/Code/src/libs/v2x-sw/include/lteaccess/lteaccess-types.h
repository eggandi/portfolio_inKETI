/** 
 * @file
 * @brief lteaccess 라이브러리의 Open API에서 사용되는 자료 유형을 정의한 헤더 파일
 * @date 2020-10-28
 * @author gyun
 */


#ifndef V2X_SW_LTEACCESS_TYPES_H
#define V2X_SW_LTEACCESS_TYPES_H


// 시스템 헤더 파일
#include <stddef.h>
#include <stdint.h>


/**
 * @brief API 수행 결과 코드
 */
enum eLalResultCode
{
  kLalResult_Success = 0, ///< 성공
  kLalResult_NotSupported = 1, ///< 지원하지 않음
  kLalResult_DeviceSpecific = 2, ///< 디바이스 의존적인 에러
  kLalResult_NoMemory = 3, ///< 메모리 부족
  kLalResult_RadioInactive = 4, ///< 라디오가 비활성화 상태
  kLalResult_TxRxSuspended = 5, ///< 송신/수신 기능이 정지된 상태
  kLalResult_TxSuspended = 6, ///< 송신기능이 정지된 상태
  kLalResult_PartiallySent = 7, ///< 일부만 전송됨
  kLalResult_InvalidParams = 8, ///< 유효하지 않은 파라미터
  kLalResult_TxFlowListFull = 9, ///< 송신플로우 리스트가 가득 참
  kLalResult_Duplicated = 10, ///< 중복됨
  kLalResult_NotInitialized = 11, ///< 초기화되지 않음
  kLalResult_Unspecified,
};
typedef int LalResultCode; ///< @ref eLalResultCode


/**
 * @brief 로그메시지 출력 레벨
 *
 * 높은 레벨은 낮은 레벨의 범위를 포함한다. \n
 * 즉, 로그변수가 높은 레벨로 설정되어 있으면, 그 하위레벨에 해당되는 로그는 함께 출력된다.
 */
enum eLalLogLevel
{
  kLalLogLevel_None = 0, ///< 아무 로그도 출력하지 않는다.
  kLalLogLevel_Err, ///< 에러 로그를 출력한다.
  kLalLogLevel_Init, ///< 초기화 로그를 출력한다.
  kLalLogLevel_Event, ///< 각종 이벤트(패킷 송수신, 설정 등)에 관련된 로그를 출력한다.
  kLalLogLevel_Dump, ///< 패킷 덤프 데이터 로그를 출력한다.
  kLalLogLevel_All, ///< 모든 로그를 출력한다.
  kLalLogLevel_Min = kLalLogLevel_None,
  kLalLogLevel_Max = kLalLogLevel_All,
};
typedef unsigned int LalLogLevel; ///< @ref eLalLogLevel


/**
 * @brief MSDU(MAC Service Data Unit) 크기
 */
enum eLalMSDUSize
{
  kLalMSDUSize_Min = 1, ///< 어플리케이션에서 라이브러리로 전달 가능한 MSDU 최소길이.
                        ///< 라이브러리 내에서 부착되는 Family ID는 포함되지 않는다.
  kLalMSDUSize_Max = 1500, ///< 어플리케이션에서 라이브러리로 전달 가능한 MSDU 최대길이(임의로 지정).
                           ///< 라이브러리 내에서 부착되는 Family ID는 포함되지 않는다.
};
typedef size_t LalMSDUSize; ///< @ref eLalMSDUSize


/**
 * @brief 채널번호
 */
enum eLalChannelNumber
{
  kLalChannelNumber_Min = 0, ///< 접속 가능한 최소 채널번호(5.00GHz 중심주파수). 장치가 실제 지원하는 범위는 다를 수 있다.
  kLalChannelNumber_CCH = 178, ///< CCH 채널번호
  kLalChannelNumber_Max = 200, ///< 접속 가능한 최대 채널번호(6.00GHz 중심주파수). 장치가 실제 지원하는 범위는 다를 수 있다.
  kLalChannelNumber_NA = 999, ///< 지정되지 않은, 알 수 없는, 임의의 채널

  // 한국 V2X 주파수 채널 대역
  kLalChannelNumber_KoreaV2XMin = 172, ///< 한국에서 송신 가능한 최소 V2X 채널번호
  kLalChannelNumber_KoreaV2XCCH = 180, ///< 한국의 CCH
  kLalChannelNumber_KoreaV2XMax = 184, ///< 한국에서 송신 가능한 최대 V2X 채널번호
};
typedef unsigned int LalChannelNumber; ///< @ref eLalChannelNumber


/*
 * 파워 (dBm 단위)
 */
enum eLalPower
{
  kLalPower_Min = -128,
  kLalPower_Max = 127,
  kLalPower_MaxInClassC = 20, ///< 클래스 C 최대 포트 출력
  kLalPower_NA = 999 ///< 알 수 없는, 지정되지 않은 파워
};
typedef int LalPower; ///< @ref eLalPower


/**
 * @brief PSID (Provider Service IDentifier)
 */
enum eLalPSID
{
  kLalPSID_Min = 0, ///< PSID 최소값
  kLalPSID_Max = 0x1020407F/*270549119*/, ///< PSID 최대값
  kLalPSID_IPv6Routing = 0x1020407E/*270549118*/, ///< IPv6 Routing 서비스를 나타내는 PSID
  kLalPSID_WSA = 0x87/*135*/, ///< WSA임을 나타내는 PSID
  kLalPSID_NA = 999999999/*0x3B9AC9FF*/, ///< 지정되지 않은 PSID, 알 수 없는 PSID
};
typedef unsigned int LalPSID; ///< @ref eLalPSID


/**
 * @brief 우선순위. 값이 작을수록 우선순위가 높다.
 */
enum eLalPriority
{
  kLalPriority_MostUrgent = 0, ///< 가장 급한 우선순위
  kLalPriority_Background = 7, ///< 백그라운드 우선순위
  kLalPriority_Min = kLalPriority_MostUrgent,
  kLalPriority_Max = kLalPriority_Background
};
typedef unsigned int LalPriority; ///< @ref eLalPriority


/**
 * @brief 송신 플로우 유형
 */
enum eLalTxFlowType
{
  kLalTxFlowType_SPS, ///< 주기적 송신플로우 (Semi Persistent Scheduling)
  kLalTxFlowType_Event, ///< 이벤트성(비주기적) 송신플로우 - 전송주기가 불규칙하거나 kLalTxFlowInterval_Max 이상일 경우.
  kLalTxFlowType_Max = kLalTxFlowType_Event,
};
typedef unsigned int LalTxFlowType; ///< @ref eLalTxFlowType


/**
 * @brief 송신 플로우 주기 (msec 단위)
 */
enum eLalTxFlowInterval
{
  kLalTxFlowInterval_Min = 100, ///< from AG15 default value
  kLalTxFlowInterval_Max = 1000, ///< from AG15 default value
  kLalTxFlowInterval_Unit = 100, ///< 송신플로우 주기 단위 (모든 주기는 이 단위로 나누어 떨어져야 한다)
};
typedef unsigned int LalTxFlowInterval; ///< @ref eLalTxFlowInterval


/**
 * @brief MCS(Modulation and Coding Scheme index)
 *
 * 참고: https://www.researchgate.net/figure/LTE-V2X-modulation-and-coding-schemes-and-corresponding-values_tbl3_333461628
 */
enum eLalMCSindex
{
  kLalMCSindex_QPSK_0_13 = 0, ///< QPSK, Coding rate 0.13
  kLalMCSindex_QPSK_0_17, ///< QPSK, Coding rate 0.17
  kLalMCSindex_QPSK_0_21, ///< QPSK, Coding rate 0.21
  kLalMCSindex_QPSK_0_27, ///< QPSK, Coding rate 0.27
  kLalMCSindex_QPSK_0_33, ///< QPSK, Coding rate 0.33
  kLalMCSindex_QPSK_0_41, ///< QPSK, Coding rate 0.41
  kLalMCSindex_QPSK_0_48, ///< QPSK, Coding rate 0.48
  kLalMCSindex_QPSK_0_57, ///< QPSK, Coding rate 0.57
  kLalMCSindex_QPSK_0_65, ///< QPSK, Coding rate 0.65
  kLalMCSindex_QPSK_0_73, ///< QPSK, Coding rate 0.73
  kLalMCSindex_QPSK_0_82, ///< QPSK, Coding rate 0.82
  kLalMCSindex_16QAM_0_41, ///< 16QAM, Coding rate 0.41
  kLalMCSindex_16QAM_0_46, ///< 16QAM, Coding rate 0.46
  kLalMCSindex_16QAM_0_52, ///< 16QAM, Coding rate 0.52
  kLalMCSindex_16QAM_0_59, ///< 16QAM, Coding rate 0.59
  kLalMCSindex_16QAM_0_67, ///< 16QAM, Coding rate 0.67
  kLalMCSindex_16QAM_0_72, ///< 16QAM, Coding rate 0.72
  kLalMCSindex_16QAM_0_75, ///< 16QAM, Coding rate 0.75
  kLalMCSindex_16QAM_0_84, ///< 16QAM, Coding rate 0.84
  kLalMCSindex_16QAM_0_92, ///< 16QAM, Coding rate 0.92
  kLalMCSindex_16QAM_1_00, ///< 16QAM, Coding rate 1.00
  kLalMCSindex_Min = kLalMCSindex_QPSK_0_13,
  kLalMCSindex_Max = kLalMCSindex_16QAM_1_00,
};
typedef unsigned int LalMCSindex; ///< @ref eLalMCSindex


#endif //V2X_SW_LTEACCESS_TYPES_H
