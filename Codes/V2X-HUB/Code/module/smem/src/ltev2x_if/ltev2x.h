/**
 * @file
 * @brief CSM(C-ITS Service Module) V2X 서비스 구현
 * @date 2025-08-28
 * @author Dong
 */

 // V2X 라이브러리 헤더
#include "dot2/dot2.h"
#include "dot3/dot3.h"
#include "j29451/j29451.h"

#include "ltev2x-hal/ltev2x-hal.h"
#include "ffasn1c/ffasn1-dot2-2021.h"
#include "ffasn1c/ffasn1-dot3-2016.h"
#include "ffasn1c/ffasn1-j2735-2016.h"

#ifndef V2X_HUB_CSM_V2X_H
#define V2X_HUB_CSM_V2X_H

#define LTEV2X_V2X_J2735_J29451_DEFAULT_PATHHISTORY_FILE_PATH "./bsm_path.history"

/**
 * @brief V2X 라이브러리 동작 유형
 */
enum eOperationType
{
  kOperationType_rx_only, ///< 수신동작만 수행
  kOperationType_trx, ///< 송수신동작 수행
  kOperationType_max = kOperationType_trx
};
typedef unsigned int Operation; ///< @ref eOperationType


/**
 * @brief 로그메시지 출력 레벨
 */
enum eLTEV2XDbgMsgLevel
{
  kLTEV2XDbgMsgLevel_None, ///< 미출력
  kLTEV2XDbgMsgLevel_Err, ///< 에러
  kLTEV2XDbgMsgLevel_Init, ///< 에러
  kLTEV2XDbgMsgLevel_Event, ///< 이벤트 출력
  kLTEV2XDbgMsgLevel_Dump, ///< 메시지 hexdump 출력
  kLTEV2XDbgMsgLevel_max = kLTEV2XDbgMsgLevel_Dump
};
typedef unsigned int LTEV2XDbgMsgLevel; ///< @ref eDbgMsgLevel


/**
 * @brief BSM Enable Type
 */
enum eLTEV2XJ2735BSMEnableType{
  kLTEV2XJ2735BSMDISABLE       = 0x0,
	kLTEV2XJ2735BSMDISABLEMENUALENABLE = 0x1,
	kLTEV2XJ2735BSMDISABLEJ29451ENABLE = 0x2,
}; 
typedef unsigned int LTEV2XJ2735BSMEnableType; ///< @ref eLTEV2XJ2735BSMEnableType


/**
 * @brief V2X Library Parameters 구조체 LTEV2X wrapping
 */
typedef struct Dot2SPDUConstructParams LTEV2XDot2SPDUParams;
typedef struct Dot3WSMProcessParams LTEV2XDot3WSMParameters;
typedef struct LTEV2XHALMSDUTxParams LTEV2XMSDUParameters;

typedef void (*LTEV2X_J2735J29451TxCallback)(const uint8_t *bsm, size_t bsm_size, bool event, bool cert_sign, bool id_change, uint8_t *addr);

/**
 * @brief V2X-J2735 설정
 */
typedef struct 
{
	//초기값으로 설정
	uint8_t pathhistory_filepath[256]; // 문자열(배열 끝이 반드시 '\0')
	J29451LogLevel loglevel;
	J29451BSMTxInterval tx_interval;
	struct {
		J29451VehicleWidth width;
		J29451VehicleLength length;
	} vehicleinfo;

	uint8_t macaddr_arr[6];
	LTEV2X_J2735J29451TxCallback txcallback;

	//tx 파라메터
	struct {
		LTEV2XDot2SPDUParams *dot2;
		LTEV2XDot3WSMParameters wsm;
		LTEV2XMSDUParameters msdu;
	}txparams;
} LTEV2XJ273BSMJ29451LibraryConfig;


/**
 * @brief V2X-J2735 송신 메시지 리스트 설정
 */
typedef struct
{
	struct {
		union {
        LTEV2XJ2735BSMEnableType set;  // enum 값으로 접근
        struct {
            unsigned int menual_enable  : 1; // 1비트만 사용
            unsigned int j29451_enable  : 1; // 1비트만 사용
        };
    };
	}BSM;//BSM은 메뉴얼과 J29451 라이브러리 둘 중 하나만 사용
} LTEV2XJ2735TxEnableList;


/**
 * @brief V2X-J2735 수신 메시지 리스트 설정
 */
typedef struct
{
	bool BSM_enable; ///< BSM 수신 동작 여부
	bool MAP_enable; ///< MAP 수신 동작 여부
	bool SPAT_enable; ///< SPAT 수신 동작 여부
	bool PVD_enable; ///< PVD 수신 동작 여부
	bool RSA_enable; ///< RSA 수신 동작 여부
	bool RTCM_enable; ///< RTCM 수신 동작 여부
	bool TIM_enable; ///< TIM 수신 동작 여부
} LTEV2XJ2735RxEnableList;


/**
 * @brief V2X-J2735 설정
 */
typedef struct 
{
	bool enable; ///< V2X-J2735 동작 여부

	LTEV2XJ2735TxEnableList tx; ///< J2735 메시지 송신 메시지 리스트 설정
	LTEV2XJ2735RxEnableList rx; ///< J2735 메시지 수신 메시지 리스트 설정

	// J2735 메시지별 라이브러리
	LTEV2XJ273BSMJ29451LibraryConfig j29451;

} LTEV2XJ2735Config;


/**
 * @brief V2X-Dot2 설정
 */
typedef struct 
{
		bool enable; ///< V2X-Dot2 동작 여부
		struct Dot2SecProfile profile;
		char cert_path[256]; ///< 인증서 경로
		char trustedcerts_path[256]; ///< 인증서 경로
		bool cmhf_obu_enable; ///< OBU 인증서 사용 여부
		char cmhf_obu_path[256]; ///< OBU 인증서 경로
		bool cmhf_rsu_enable; ///< RSU 인증서 사용 여부
		char cmhf_rsu_path[256]; ///< RSU 인증서 경로
} LTEV2XDot2Config;


/**
 * @brief V2X 라이브러리 관리정보
 */
typedef struct
{
	bool tx_running;
	char dev_name[128]; ///< LTE-V2X 통신 디바이스 이름

	struct {
		bool wsa_enable;
		LTEV2XDot2Config dot2;
		LTEV2XJ2735Config j2735;
	}config;

} LTEV2X_MIB;

// ltev2x.c
int LTEV2X_InitialV2XLibrary(void);
void LTEV2X_ProcessRxLTEV2XPkt(const uint8_t *msdu, LTEV2XHALMSDUSize msdu_size, struct LTEV2XHALMSDURxParams rx_params);
void LTEV2X_ProcessRxLTEV2XPayload(uint32_t psid, uint8_t *payload, size_t payload_len);
int LTEV2X_SetLTEV2XRegisterTransmitFlow(LTEV2XHALTxFlowIndex flow, J29451BSMTxInterval interval, LTEV2XHALPriority priority);
int LTEV2X_SetParameterMSDU(LTEV2XMSDUParameters *params, LTEV2XHALTxFlowType tx_flow_type, 
																													LTEV2XHALTxFlowIndex tx_flow_index, 
																													LTEV2XHALPriority *priority, 
																													LTEV2XHALL2ID dst_l2_id, 
																													LTEV2XHALPower *tx_power);
// ltev2x_dot2.c
int LTEV2X_Dot2InitialSecurity();
void LTEV2X_Dot2ProcessSPDUCallback(Dot2ResultCode result, void *priv);
int LTEV2X_SetParameterDot2(LTEV2XDot2SPDUParams *params, Dot2SPDUConstructType type,
																													Dot2Time64 *time,
																													Dot2PSID psid,
																													Dot2SignerIdType signer_id_type);
// ltev2x_dot3.c
int LTEV2X_SetParameterDot3(LTEV2XDot3WSMParameters *params,  Dot3ProtocolVersion version,
																															Dot3PSID psid,
																															Dot3ChannelNumber *chan_num,
																															Dot3DataRate *datarate,
																															Dot3Power *transmit_power);
// ltev2x_j29451.c
int LTEV2X_J2735J29451Inlitial(const char *pathhistory_filepath, 
															 J29451LogLevel *loglevel, 
															 J29451BSMTxInterval *tx_interval, 
															 J29451VehicleWidth *width, J29451VehicleLength *length);
void LTEV2X_SetJ2735J29451Config(LTEV2XDot2SPDUParams *dot2, LTEV2XDot3WSMParameters *wsm, LTEV2XMSDUParameters *msdu);

#define LTEV2X_MarcoJ2735RangeCheck(output, value, max, min)  do {\
																																		if ((value + 1 >= min + 1) && value <= max) {\
																																			output = value;\
																																		} else {\
																																			ERR("[%s][%d]Fail to set parameter - %d is out of range (%d, %d)\n", __func__, __LINE__, value, min, max);\
																																			return -1;\
																																		}\
																																	} while (0)

#endif // V2X_HUB_CSM_V2X_H