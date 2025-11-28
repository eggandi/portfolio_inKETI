/**
 * @file
 * @brief MB(Message Broker) I/F 기능 구현
 * @date 2025-08-07
 * @author donggyu
 */

#include <dscm.h>


/**
 * @brief DSCM의 MB(Message Broker)는 아래 기능을 구현합니다.
 * 차량 내부 장치는 디지털 센터와 직접(TCP, LTE) 연결된 DSCM 모듈(Layer-7 기반 GW)을 통해 디지털 센터와 통신을 해야 합니다. 
 * DSCM은 MB 기능을 통해 차량 내부 장치와 디지털 센터 간 메시지 교환을 중계 합니다.
 * MB는 아래와 같은 순서로 동작합니다.
 * 1. 장치 별 MB 기능 활성화
 * 1.1. DSCM은 차량 내부 장치로부터 주행전량 API 중 메시지 전달 요청 Request를 수신 처리합니다.
 * 1.2. DSCM은 메시지 전달 요청 Request에 포함된 장치 종류와 메시지 종류를 확인합니다.
 * 1.3. DSCM은 메시지 전달 요청 한 장치로 Response를 송신합니다.
 * 1.3.1. 회신할 때 주행전략 프토토콜 헤더에 DC와의 로그인 할 때 수신한 auth_info는 전달하지 않습니다. 
 * 2. MB 기능이 활성화된 장치는 V2XHUB가 DC와 주행전략 API를 교환해 줍니다. 
 */

 /**
	* @brief DC로부터 수신 받은 주행전략 메시지를 브로커가 활성화된 장치에게 전송
  */
int DSCM_ProcessMBToInVehicleTCPPkt(DSCM_MIB *mib, KVHDSCMMessageFramePtr frame, KVHDSCMEquipmentList equiptype, const int client_fd)
{
	KVHDSCMMessageHeader *header = (KVHDSCMMessageHeader*)&frame->header;
	memset(header->auth_info, 0x00, DSCMDC_PKT_LOGIN_AUTHFICATION_TOKEN_LENGTH);
	KVHIFDataLen pkt_len = DSCMDC_PKT_STX_LENGTH + sizeof(KVHDSCMMessageHeader) + header->packet_length + DSCMDC_PKT_TAIL_LENGTH;

	// 헤더 정보 출력
	LOG(kKVHLOGLevel_InOut, "Sending MB to equiptype: %d, packet_type: %d\n", equiptype, (int)header->packet_type);

	// DC로 패킷 전송
	if (KVHTCP_SendToClient(mib->dc_if.tcpinfo.h, client_fd, (KVHTCPClientType)equiptype, (uint8_t *)frame, pkt_len) < 0) {
		ERR("Failed to send MB to DC\n");
		return -1;
	}

	return 0;
}


 /**
	* @brief 차내장치의 메시지를 DC로 전송
  */
int DSCM_ProcessMBToDCTCPPkt(DSCM_MIB *mib, KVHDSCMMessageFramePtr frame)
{

	KVHDSCMMessageHeader *header = (KVHDSCMMessageHeader*)&frame->header;
	memcpy(header->auth_info, mib->dc_if.dc_login_info.auth_token, DSCMDC_PKT_LOGIN_AUTHFICATION_TOKEN_LENGTH);
	KVHIFDataLen pkt_len = DSCMDC_PKT_STX_LENGTH + sizeof(KVHDSCMMessageHeader) + header->packet_length + DSCMDC_PKT_TAIL_LENGTH;

	// 헤더 정보 출력
	LOG(kKVHLOGLevel_InOut, "Sending MB to DC - packet_type: %d\n", (int)header->packet_type);

	// DC로 패킷 전송
	if (KVHTCP_SendToServer(mib->dc_if.tcpinfo.h, (uint8_t *)frame, pkt_len) < 0) {
		ERR("Failed to send MB to DC\n");
		return -1;
	}

	return 0;
}


 /**
	* @brief MB 기능을 수행하기 위한 API
	* @param[in] mib 모듈 관리정보
	* @param[in] frame 수신된 메시지 프레임
	* @param[in] client_fd 메시지 전달 요청을 송신한 클라이언트 소켓
	* @retval 0(성공), -1(실패)
  */
 int DSCM_PushMBDrivingStrategyDoJob(DSCM_MIB *mib, KVHDSCMMessageFramePtr frame, KVHDSCMEquipmentList equiptype, const int client_fd, int flag)
 {
	int ret = -1;
	if (!mib || !frame || !client_fd || client_fd < 0) {
		ERR("Invalid parameters. mib:%p, frame:%p, client_fd:%p(%d)\n", mib, frame, client_fd, client_fd ? client_fd : -1);
		return ret;
	}
	
	KVHDSCMMessageHeader *header = &frame->header;

	switch ((KVHDSCMDrivingStrategyPktType)header->packet_type) {
		case kKVHDSCM_DrivingStrategyPktType_None:                          ///<	없음										
		case kKVHDSCM_DrivingStrategyPktType_StartReport:                   ///<	시작보고
		case kKVHDSCM_DrivingStrategyPktType_EndReport:                     ///<	종료보고
		case kKVHDSCM_DrivingStrategyPktType_VehicleStatusDiagnosis:        ///<	차량상태진단(운행중)
		case kKVHDSCM_DrivingStrategyPktType_DrivingStrategyAPIListRequest: ///<	주행전략API목록요청
		case kKVHDSCM_DrivingStrategyPktType_DrivingStrategyAPISetting:     ///<	주행전략API설정
		case kKVHDSCM_DrivingStrategyPktType_DrivingStrategyPush:           ///<	주행전략(Push)
		case kKVHDSCM_DrivingStrategyPktType_AllRouteRequest:               ///<	전구간경로요청
		case kKVHDSCM_DrivingStrategyPktType_PreDriveStatusDiagnosis:       ///<	상태진단정보(운행전)
		case kKVHDSCM_DrivingStrategyPktType_DispatchInfoListRequest:       ///<	배차정보리스트요청
		case kKVHDSCM_DrivingStrategyPktType_DispatchSelection:             ///<	배차선택
		case kKVHDSCM_DrivingStrategyPktType_CargoLoadingStart:             ///<	화물적재시작
		case kKVHDSCM_DrivingStrategyPktType_CargoLoadingComplete:          ///<	화물적재완료
		case kKVHDSCM_DrivingStrategyPktType_UnloadingStart:                ///<	하차시작
		case kKVHDSCM_DrivingStrategyPktType_UnloadingComplete:             ///<	하차완료
		case kKVHDSCM_DrivingStrategyPktType_DrivingStart:                  ///<	운행시작
		case kKVHDSCM_DrivingStrategyPktType_DrivingComplete:               ///<	운행완료
		case kKVHDSCM_DrivingStrategyPktType_VehicleWeightReport:{           ///<	차량중량보고
			if (flag == 0) { // flag가 0이면 DC로 송신하는 메시지 SendToServer
				LOG(kKVHLOGLevel_InOut, "Forwarding pkt %d from equiptype %d to Data Center\n", (uint32_t)header->packet_type, (int)equiptype);
				ret = DSCM_ProcessMBToDCTCPPkt(mib, frame); // Data Center로 전송
			} else {
				LOG(kKVHLOGLevel_InOut, "Forwarding pkt %d from Data Center to Equiptype:%d\n", (uint32_t)header->packet_type, (int)equiptype);
				ret = DSCM_ProcessMBToInVehicleTCPPkt(mib, frame, equiptype, client_fd); // Data Center로 전송
			}
			
			break;
		}
		case kKVHDSCM_DrivingStrategyPktType_RemoteControl:                 ///<	원격제어
		case kKVHDSCM_DrivingStrategyPktType_MessageDeliveryRequest:        ///<	메시지전달요청
		case kKVHDSCM_DrivingStrategyPktType_Login:                         ///<	Login
		case kKVHDSCM_DrivingStrategyPktType_Logout:                        ///<	Logout
		case kKVHDSCM_DrivingStrategyPktType_LogoutAbnormal:                ///<	로그아웃(비정상)
		case kKVHDSCM_DrivingStrategyPktType_InVehicleEquipmentInfo:        ///<	자율주행상용차 수집정보
		case kKVHDSCM_DrivingStrategyPktType_InVehicleEquipmentStatus:      ///<	접속장치상태
		default:
			ERR("Unknown packet type in the TCP pkt from HMI(type: %d)\n", (uint32_t)header->packet_type);
			return ret;
			break;
	}

	return ret;
 }