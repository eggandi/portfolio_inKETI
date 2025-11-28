/**
 * @file
 * @brief DSCM() 구현
 * @date 2025-06-11
 * @author donggyu
 */

#ifndef V2X_HUB_DSCM_H
#define V2X_HUB_DSCM_H

#ifdef __cplusplus
extern "C"
{
#endif
// 시스템 헤더파일
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/timerfd.h>

// 공용 헤더파일
#include "kvh_common.h"
#include "sudo_queue.h"
#include "ext_data_fmt/kvh_ext_data_fmt.h"
#include "internal_data_fmt/kvh_internal_data_fmt_uds.h"
#include "internal_data_fmt/kvh_internal_data_fmt_ivdct.h" // c Type 구조체
#include "internal_data_fmt/kvh_internal_data_fmt_vhmm.h" // c Type 구조체

// 공용 라이브러리 헤더파일
#include "kvhlog.h"
#include "kvhuds.h"
#include "kvhtcp.h"
#include "kvhredis.h"



/// DSCM 기본 매크로
#define DSCM_BASE64_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define HTONLL(x) (((uint64_t)htonl((uint32_t)((x) & 0xFFFFFFFFULL)) << 32) | htonl((uint32_t)((x) >> 32)))
#define NTOHLL(x) (((uint64_t)ntohl((uint32_t)((x) & 0xFFFFFFFFULL)) << 32) | ntohl((uint32_t)((x) >> 32)))
#else
#define HTONLL(x) (x)
#define NTOHLL(x) (x)
#endif

/// DSCM DC I/F 기본 매크로
#define DSCMDC_TCP_DEFAULT_IF_DEV_NAME "eth0"
#define DSCMDC_TCP_DEFAULT_IF_IPV4_SERVER_ADDR_STR "192.168.0.1"
#define DSCMDC_TCP_DEFAULT_IF_IPV4_CLIENT_ADDR_STR "192.168.0.2"

#define DSCMDC_PKT_STX (0x7EA57EA5)
#define DSCMDC_PKT_ETX (0X0D0A0D0A)
#define DSCMDC_PKT_STX_LENGTH (0x04)
#define DSCMDC_PKT_CRC_LENGTH (0x02)
#define DSCMDC_PKT_EXT_LENGTH (0x04)
#define DSCMDC_PKT_TAIL_LENGTH (DSCMDC_PKT_CRC_LENGTH + DSCMDC_PKT_EXT_LENGTH)

#define DSCMDC_PKT_LOGIN_AUTHFICATION_LENGTH (256)
#define DSCMDC_PKT_LOGIN_AUTHFICATION_TOKEN_LENGTH (16)

#define DSCMDC_PKT_CRC16_CCITTFALSE_POLY 0x1021
#define DSCMDC_PKT_CRC16_CCITTFALSE_INIT 0xFFFF
#define DSCMDC_PKT_CRC16_CCITTFALSE_REFIN 0
#define DSCMDC_PKT_CRC16_CCITTFALSE_REFOUT 0
#define DSCMDC_PKT_CRC16_CCITTFALSE_XOROUT 0x0000

#define DSCMHMI_INVEHICLE_AETH_DEFAULT_IF_DEV_NAME "eth1"
#define DSCMHMI_TCP_DEFAULT_IF_IPV4_SERVER_ADDR_STR "192.168.10.1"
#define DSCMREDIS_TCP_DEFAULT_IF_IPV4_SERVER_ADDR_STR "127.0.0.1"

#define DSCMMB_MAX_EQUIPMENTS (16)

#define DSCMJOB_EVENTLIST_MAX (16)

	/**
	 * @brief Driving Strategy Manager(DSCM) Job List
	 */
	enum eKVHDSCMEquipmentList
	{
		kKVHDSCM_EquipmentStatusALL = 0,		///< 모든 접속 장치 (default)
		kKVHDSCM_EquipmentStatusIVDCT = 1,	///< IVDCT
		kKVHDSCM_EquipmentStatusADS = 2,		///< ADS
		kKVHDSCM_EquipmentStatusV2XHUB = 3, ///< V2XHUB
		kKVHDSCM_EquipmentStatusDMS = 4,		///< DMS
		kKVHDSCM_EquipmentStatusDTG = 5,		///< DTG
		kKVHDSCM_EquipmentStatusHMI = 6,		///< HMI
		kKVHDSCM_EquipmentStatusMAX = kKVHDSCM_EquipmentStatusHMI + 1, ///< 최대값
	};
	typedef long KVHDSCMEquipmentList; ///< @ref eKVHDSCMEquipmentList

	/**
	 * @brief DSCM의 Driving Strategy의 Pkt Type
	 */
	enum eKVHDSCMDrivingStrategyPktType
	{
		kKVHDSCM_DrivingStrategyPktType_None = 0,											     ///<	없음										
		kKVHDSCM_DrivingStrategyPktType_StartReport = 1,									 ///<	시작보고
		kKVHDSCM_DrivingStrategyPktType_EndReport = 2,										 ///<	종료보고
		kKVHDSCM_DrivingStrategyPktType_VehicleStatusDiagnosis = 3,				 ///<	차량상태진단(운행중)
		kKVHDSCM_DrivingStrategyPktType_InVehicleEquipmentInfo = 4,				 ///<	자율주행상용차 수집정보
		kKVHDSCM_DrivingStrategyPktType_InVehicleEquipmentStatus = 5,			 ///<	접속장치상태
		kKVHDSCM_DrivingStrategyPktType_DrivingStrategyAPIListRequest = 6, ///<	주행전략API목록요청
		kKVHDSCM_DrivingStrategyPktType_DrivingStrategyAPISetting = 7,		 ///<	주행전략API설정
		kKVHDSCM_DrivingStrategyPktType_DrivingStrategyPush = 8,					 ///<	주행전략(Push)
		kKVHDSCM_DrivingStrategyPktType_AllRouteRequest = 9,							 ///<	전구간경로요청
		kKVHDSCM_DrivingStrategyPktType_PreDriveStatusDiagnosis = 11,			 ///<	상태진단정보(운행전)
		kKVHDSCM_DrivingStrategyPktType_DispatchInfoListRequest = 12,			 ///<	배차정보리스트요청
		kKVHDSCM_DrivingStrategyPktType_DispatchSelection = 13,						 ///<	배차선택
		kKVHDSCM_DrivingStrategyPktType_CargoLoadingStart = 14,						 ///<	화물적재시작
		kKVHDSCM_DrivingStrategyPktType_CargoLoadingComplete = 15,				 ///<	화물적재완료
		kKVHDSCM_DrivingStrategyPktType_UnloadingStart = 16,							 ///<	하차시작
		kKVHDSCM_DrivingStrategyPktType_UnloadingComplete = 17,						 ///<	하차완료
		kKVHDSCM_DrivingStrategyPktType_DrivingStart = 18,								 ///<	운행시작
		kKVHDSCM_DrivingStrategyPktType_DrivingComplete = 19,							 ///<	운행완료
		kKVHDSCM_DrivingStrategyPktType_VehicleWeightReport = 20,					 ///<	차량중량보고
		kKVHDSCM_DrivingStrategyPktType_RemoteControl = 21,								 ///<	원격제어
		kKVHDSCM_DrivingStrategyPktType_MessageDeliveryRequest = 90,			 ///<	메시지전달요청
		kKVHDSCM_DrivingStrategyPktType_Login = 91,												 ///<	Login
		kKVHDSCM_DrivingStrategyPktType_Logout = 92,											 ///<	Logout
		kKVHDSCM_DrivingStrategyPktType_LogoutAbnormal = 93,							 ///<	로그아웃(비정상)
		kKVHDSCM_DrivingStrategyPktType_MAX = kKVHDSCM_DrivingStrategyPktType_LogoutAbnormal + 1, ///< 최대값
	};
	typedef long KVHDSCMDrivingStrategyPktType; ///< @ref eKVHDSCMDrivingStrategyPktType

	/**
	 * @brief Driving Strategy Manager(DSCM) Job List
	 */
	enum eKVHDSCMJobList
	{
		kKVHDSCM_JobNone = 0,																																								///< Job 없음
		kKVHDSCM_InVehicleEquipmentCollectionData = kKVHDSCM_DrivingStrategyPktType_InVehicleEquipmentInfo, ///< Job 접속장치정보수집
		kKVHDSCM_InVehicleEquipmentState = kKVHDSCM_DrivingStrategyPktType_InVehicleEquipmentStatus,				///< Job 접속장치상태
		kKVHDSCM_RemoteControl = kKVHDSCM_DrivingStrategyPktType_RemoteControl,															///< Job 원격제어
		kKVHDSCM_CenterLogin = kKVHDSCM_DrivingStrategyPktType_Login,																				///< Job 센터로그인
		kKVHDSCM_CenterLogout = kKVHDSCM_DrivingStrategyPktType_Logout,																			///< Job 센터로그아웃
		kKVHDSCM_CenterLogoutAdnormal = kKVHDSCM_DrivingStrategyPktType_LogoutAbnormal,											///< Job 센터로그아웃(비정상)
		kKVHDSCM_MessageDeliveryRequest = kKVHDSCM_DrivingStrategyPktType_MessageDeliveryRequest,						///< Job 메시지전달요청
	};
	typedef long KVHDSCMJobList; ///< @ref eKVHDSCMJobList

	/**
	 * @brief Driving Strategy Manager(DSCM) In-Vehicle Equipment State Job List
	 */
	enum eKVHDSCMDoInVehicleEquipmentState
	{
		kKVHDSCMDoRecv_ReqFromCenterInVehicleEquipmentState = kKVHDSCM_InVehicleEquipmentState + 1, ///< Job 접속장치상태요청
		kKVHDSCMDoSend_ResToCenterInVehicleEquipmentState = kKVHDSCM_InVehicleEquipmentState + 2,		///< Job 접속장치정보응답
	};
	typedef long KVHDSCMDoInVehicleEquipmentState; ///< @ref eKVHDSCM_DoInVehicleEquipmentState

	/**
	 * @brief Driving Strategy Manager(DSCM) Center Login Job List
	 */
	enum eKVHDSCMDoCenterLogin
	{
		kKVHDSCMDoRecv_ReqToCenterCenterLogin = kKVHDSCM_CenterLogin + 1,		///< Job 센터로그인
		kKVHDSCMDoSend_ResFromCenterCenterLogin = kKVHDSCM_CenterLogin + 2, ///< Job 센터로그인
	};
	typedef long KVHDSCMDoCenterLogin; ///< @ref eKVHDSCM_DoCenterLogin

	/**
	 * @brief Driving Strategy Manager(DSCM) Center Logout Job List
	 */
	enum eKVHDSCMDoCenterLogout
	{
		kKVHDSCMDoRecv_ReqToCenterCenterLogout = kKVHDSCM_CenterLogin + 1,	 ///< Job 센터로그아웃
		kKVHDSCMDoSend_ResFromCenterCenterLogout = kKVHDSCM_CenterLogin + 2, ///< Job 센터로그아웃
	};
	typedef long KVHDSCMDoCenterLogout; ///< @ref eKVHDSCM_DoCenterLogout

	enum eKVHDSCMDoCenterLogoutAdnormal
	{
		kKVHDSCMDoRecv_PushToCenterCenterLogoutAdnormal = kKVHDSCM_CenterLogoutAdnormal + 1, ///< Job 센터로그아웃
	};
	typedef long KVHDSCM_DoCenterLogoutAdnormal; ///< @ref eKVHDSCM_DoCenterLogoutAdnormal

	enum eKVHDSCMDoRemoteControl
	{
		kKVHDSCMDoRecv_ReqToCenterRemoteControlStartToWaitingZone = kKVHDSCM_RemoteControl + 1,		///< Job 대기존으로 원격제어 시작
		kKVHDSCMDoRecv_ResFromCenterRemoteControlEndToWaitingZone = kKVHDSCM_RemoteControl + 2,		///< Job 대기존으로 원격제어 종료
		kKVHDSCMDoRecv_ReqToCenterRemoteControlStartToUnloadingZone = kKVHDSCM_RemoteControl + 3, ///< Job 언로딩존으로 원격제어 시작
		kKVHDSCMDoRecv_ResFromCenterRemoteControlEndToUnloadingZone = kKVHDSCM_RemoteControl + 4, ///< Job 언로딩존으로 원격제어 종료
	};
	typedef long KVHDSCMDoRemoteControl; ///< @ref eKVHDSCMDoRemoteControl


	/**
	 * @brief 주행전략 프로토콜 메시지 전달 요청 송신 페이로드
	 */
	typedef struct
	{
		uint64_t timestamp; // epoch time 메시지 생성시간으로 응 메시지에 포함해 메시지 UID로 사용
		KVHDSCMDrivingStrategyPktType push_msg_list[]; // 메시지 유형
	} __attribute__((packed)) KVHDSCMMessageDeliveryResponse;
	

	/**
	 * @brief 주행전략 프로토콜 메시지 전달 요청 수신 페이로드
	 */
	typedef struct
	{
		uint64_t timestamp; // epoch time 메시지 생성시간으로 응 메시지에 포함해 메시지 UID로 사용
		KVHDSCMEquipmentList equip_type; // 요청 장치
		char equip_system_id[16];
		KVHDSCMDrivingStrategyPktType push_msg_list[]; // 메시지 유형
	} __attribute__((packed)) KVHDSCMMessageDeliveryRequest;

	/**
	 * @brief 주행전략 프로토콜 메시지 프레임 헤더의 패킷 플래그 bitfield 정의
	 */
	typedef struct
	{
		uint8_t type : 1;			// 요청:0, 응답:1
		uint8_t isres : 1;		// 응답여부(없음:0,, 있음:1)
		uint8_t result : 1;		// 성공:0, 실패:1
		uint8_t reserved : 1; // 예약
		uint8_t restimer : 4; // 응답대기시간(0~15);
	} __attribute__((packed)) KVHDSCMMessagePacketFlag;

	/**
	 * @brief 주행전략 프로토콜 메시지 프레임 헤더
	 */
	typedef struct
	{
		uint32_t packet_seqno;
		uint64_t timestamp;
		uint8_t version;
		uint8_t auth_info[16];
		uint16_t packet_type;
		KVHDSCMMessagePacketFlag packet_flag;
		uint32_t packet_length;
	} __attribute__((packed)) KVHDSCMMessageHeader;

	/**
	 * @brief 주행전략 프로토콜 Rx 메시지 프레임
	 */
	typedef struct
	{
		uint32_t STX;
		KVHDSCMMessageHeader header;
		uint16_t CRC;
		uint32_t ETX;
		uint8_t payload_data[];
	} __attribute__((packed)) KVHDSCMMessageFrame;
	typedef KVHDSCMMessageFrame *KVHDSCMMessageFramePtr;

	/**
	 * @brief 주행전략 프로토콜 Tx 메시지 프레임
	 */
	typedef struct
	{
		uint32_t STX;
		KVHDSCMMessageHeader header;
		uint8_t payload_data[];
	} __attribute__((packed)) KVHDSCMMessageFrameTx;


	/**
	 * @brief 라이브러리 기능 컨텍스트 정보
	 */
	typedef struct
	{
		struct
		{
			int fd;													///< 소켓 파일 디스크립터
			struct sockaddr_in client_addr; ///< 내 주소 정보
			int client_port;								///< 클라이언트 포트
			struct sockaddr_in server_addr; ///< 서버 주소 정보
			int server_port;								///< 서버 포트
			bool session_established;				///< 연결 상태. 클라이언트는 connect() 성공 시 true, 서버는 listen() 성공 시 true
			int send_flags;
			int recv_flags;
			KVHTCPClientType client_sock[KVHTCP_ServerConnectedClientsMax]; ///< 서버동작시 연결된 클라이언트 리스트
		} sock;																														///< 소켓 관리정보

		struct
		{
			pthread_t th;					 ///< 쓰레드 식별자
			volatile bool running; ///< 쓰레드가 동작 중인지 여부
		} rx_thread;						 ///< TCP 데이터 수신 쓰레드 관리정보

		uint8_t rx_buf[kKVHIFDataLen_Max]; ///< 수신된 데이터가 저장되는 버퍼
		KVHTCP_ProcessRxCallback rx_cb;		 ///< 어플리케이션 콜백함수
		void *app_priv;										 ///< 어플리케이션 전용 정보 (콜백함수 호출시 어플리케이션으로 함께 전달된다)
	} KVHDSCMTCP_CTX;


	enum eDSCMRedis_VehicleDataKeysIdx{
		kDSCMRedis_VehicleDataKeysBuf_Index_0 = 0,
		kDSCMRedis_VehicleDataKeysBuf_Index_1 = 1,
	}; typedef long DSCMRedis_VehicleDataKeysIdx; ///< @ref eDSCMRedis_VehicleDataKeysIdx
	

	enum eDSCMRedis_VehicleDataKeySType{
		kDSCMRedis_VehicleDataKeyS_Type_ADS_HF = 0,
		kDSCMRedis_VehicleDataKeyS_Type_ADS_LF = 1,
		kDSCMRedis_VehicleDataKeyS_Type_DMS = 2,
		kDSCMRedis_VehicleDataKeyS_Type_DTG = 3,
		kDSCMRedis_VehicleDataKeyS_Type_IVDCT = 4,
		kDSCMRedis_VehicleDataKeyS_Type_VH_HF = 5,
		kDSCMRedis_VehicleDataKeyS_Type_VH_LF = 6,
		kDSCMRedis_VehicleDataKeyS_Type_None = 999,
	}; typedef long DSCMRedis_VehicleDataKeySType; ///< @ref eDSCMRedis_VehicleDataKeySType


	typedef struct{
		bool isempty;
		KVHREDISKey key_ADS_HF;
		KVHREDISKey key_ADS_LF;
		KVHREDISKey key_DMS;	
		KVHREDISKey key_DTG;
		KVHREDISKey key_IVDCT;
		KVHREDISKey key_VH_HF;
		KVHREDISKey key_VH_LF;
	} DSCMRedis_VehicleDataKeys;


	typedef struct{
		bool isinitialized;
		DSCMRedis_VehicleDataKeys keys[2];
		DSCMRedis_VehicleDataKeysIdx current_idx;
		DSCMRedis_VehicleDataKeys *key_now_ptr;
	} DSCMRedis_VehicleDataKeysBuf;

	/**
	 * @brief 모듈 통합관리정보(Management Information Base)
	 */
	typedef struct
	{
		char *logfile;			///< 로그 저장 파일경로
		KVHLOGLevel log_lv; ///< 로그 저장 레벨 (기본값 적용 또는 프로그램 실행 인자로 입력된다)


		struct {
			char server_addr_str[INET_ADDRSTRLEN]; ///< Redis 서버 주소 문자열
			uint16_t server_port; ///< Redis 서버 포트번호
			KVHREDISHANDLE h_cmd; ///< Redis 기능 핸들 (PUBLISH 용)
			KVHREDISHANDLE h_psub; ///< Redis 기능 핸들 (PSUBSCRIBE 용), 각종 상태정보 수집
			pthread_t th_pub; ///< Redis PUBLISH 쓰레드 식별자
			volatile bool th_pub_running; ///< Redis PUBLISH 쓰레드가 동작 중인지
			pthread_t th_scheduletask; ///< Redis 스케쥴러 태스크 쓰레드 식별자
			volatile bool th_scheduletask_running; ///< Redis 스케쥴러
		} redis_if;

		struct
		{
			char dc_eth_if[KVH_MAXLINE];					///< 외부망과 연결된 I/F Ethernet 장치명
			char dc_server_addr[INET_ADDRSTRLEN]; ///< DSCM의 IPv4 주소
			char dc_client_addr[INET_ADDRSTRLEN]; ///< Data Center의 IPv4 주소
			uint16_t dc_server_port;							///< DC와 연결될 TCP 포트번호
			uint16_t dc_client_port;							///< DC와 연결될 TCP 포트번호
			struct
			{
				bool login_state; ///< DC 로그인 상태
				char auth_info[DSCMDC_PKT_LOGIN_AUTHFICATION_LENGTH];
				char auth_token[DSCMDC_PKT_LOGIN_AUTHFICATION_TOKEN_LENGTH];
			} dc_login_info;
			KVHTCPINFO tcpinfo;
			bool mb_enable; ///< Message Broker 사용여부, TODO:: 호스트와 타겟이 여러개일 때 장비별 Enable 설정이 필요할 수 있음.
		} dc_if;					///< Data Center과 인터페이스(TCP/IP, LTE)

		struct
		{
			char hmi_eth_if[KVH_MAXLINE];					 ///< 내부망으로 HMI와 연결된 I/F Ethernet 장치명
			char hmi_server_addr[INET_ADDRSTRLEN]; ///< 서버 IPv4 주소
			uint16_t hmi_server_port;							 ///< TCP 서버 포트번호
			KVHTCPINFO tcpinfo;
			bool mb_enable; ///< Message Broker 사용여부, TODO:: 호스트와 타겟이 여러개일 때 장비별 Enable 설정이 필요할 수 있음.
		} hmi_if;					///< HMI와 인터페이스(UDP/IP)

		struct 
		{
			struct{
				bool isactive; // 활성화 여부
				bool isreq[kKVHDSCM_DrivingStrategyPktType_MAX]; // 요청 여부
				int client_fd;
				KVHDSCMEquipmentList equip_type;
				bool push_msg_list[kKVHDSCM_DrivingStrategyPktType_MAX]; // 메시지 유형
		    } req_equip_list[DSCMMB_MAX_EQUIPMENTS];
		} mb_if;
		
		struct
		{
			struct
			{
				uint32_t drive_strategy_data_tx_t;		///< Data Center로 주행전략 데이터 전송 총합
				uint32_t drive_strategy_data_rx_t;		///< Data Center로 주행전략 데이터 수신 총합
				uint32_t drive_strategy_data_tx[255]; ///< Data Center로 주행전략 데이터 전송 누적횟수 정보로 배열숫자가 Job List
				uint32_t drive_strategy_data_rx[255]; ///< Data Center로 주행전략 데이터 수신 누적횟수 정보로 배열숫자가 Job List
				struct
				{
					time_t login;		 ///< 로그인 응답 타이머
					time_t loginout; ///< 로그아웃 응답 타이머
				} timer;
			} dc;

			struct
			{
				unsigned int rx; ///< HMI로부터의 수신 누적횟수
				unsigned int tx; ///< HMI로의 송신 누적횟수
			} hmi;

			struct
			{
				unsigned int rx; ///< DGM으로부터의 수신 누적횟수
				unsigned int tx; ///< DGM으로의 송신 누적횟수
			} dgm;
									 ///< DGM과의 데이터 송수신 누적횟수
			struct
			{
				unsigned int pushtoserver[KVHTCP_ServerConnectedClientsMax]; ///< DGM으로부터의 수신 누적횟수
				unsigned int pushtoclient[KVHTCP_ServerConnectedClientsMax]; ///< DGM으로의 송신 누적횟수
			} mb;	
		} trx_cnt;

	} DSCM_MIB;
	extern DSCM_MIB g_dscm_mib;


	///< dscm_input_params.c
	int DSCM_ParsingInputParameters(int argc, char *argv[], DSCM_MIB *mib);

	///< dscm_mb_if.c
 int DSCM_PushMBDrivingStrategyDoJob(DSCM_MIB *mib, KVHDSCMMessageFramePtr frame, KVHDSCMEquipmentList equiptype, const int client_fd, int flag);
 

	///< dscm_dc_if.c
	int DSCM_InitDCIF(DSCM_MIB *mib);
	int DSCM_ProcessTxDCDrivingStrategy_V2cVehicleData(DSCM_MIB *mib, const uint8_t *encoded_payload, size_t payload_len);

	///< dscm.c
	uint16_t DSCM_GetCRC16(const uint8_t *buf, int off, int len);
	char *DSCM_ProcessBase64Encoder(const unsigned char *input);

	///< dscm_redis_if.c
	int DSCM_InitRedisIF(DSCM_MIB *mib);
	int DSCM_ProcessRedisSETtoKey(DSCM_MIB *mib, KVHREDISKey *key, uint8_t *data, size_t data_size);	

	// dscm_pb.cc
	typedef void* DSCM_PBHandler_Ptr;
	typedef void* DSCM_PBHandler_Ptr_V2cVehicleData;

	int DSCM_InitPB();
	int DSCM_ClosePB();

	// dscm_pb_dc.cc
	DSCM_PBHandler_Ptr DSCM_CreatePBHandler_V2cLoginReq();
	void DSCM_DestroyPBHandler_V2cLoginReq(DSCM_PBHandler_Ptr h);
	int DSCM_SetPBHandleV2cLoginReqFrom_Str(DSCM_PBHandler_Ptr h, const char *data);
	size_t DSCM_ProcessPBSerializeToString_V2cLoginReq(DSCM_PBHandler_Ptr h, char **out_buffer);
	int DSCM_ProcessPBRedisTxCallback(DSCM_MIB *mib, DSCMRedis_VehicleDataKeys *keys_ptr);

	// dscm_pb_ivdct.cc
	void DSCM_SetPBV2cVehicleDataFromIVDCT_CStructure(DSCM_PBHandler_Ptr_V2cVehicleData h, KVHIVDCTData *ivdct_raw);
	// dscm_pb_gnss.cc
	void DSCM_SetPBV2cVehicleDataFromGNSS_CStructure(DSCM_PBHandler_Ptr_V2cVehicleData h, KVHGNSSData *gnss_raw);


	///< dscm.h
	/**
	 * @brief 현재 시간을 epoch milliseconds 형식으로 리턴 받는 Inline 함수
	 * @retval uint64_t epoch milliseconds
	 */
	static inline uint64_t DSCM_GetEpochTime_64bit_ms(void)
	{
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		return (uint64_t)ts.tv_sec * 1000 + (ts.tv_nsec / 1000000);
	}

#define Authentication_Info_Base64(str) DSCM_ProcessBase64Encoder(str)
#define DSCM_EPOCHTIME_MS DSCM_GetEpochTime_64bit_ms()

#ifdef __cplusplus
}
#endif

#endif // V2X_HUB_DSCM_H