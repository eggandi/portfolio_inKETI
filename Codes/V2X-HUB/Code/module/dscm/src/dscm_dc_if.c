/**
 * @file
 * @brief DC(Data Center)와의 I/F 기능 구현(TCPIP over LTE)
 * @date 2025-08-21
 * @author dong
 */

// 모듈 헤더파일
#include "dscm.h"


/**
 * @brief 지정된 네트워크 인터페이스 이름으로 IPv4 주소를 가져온다.
 * @param[in] iface_name 네트워크 인터페이스 이름 (예: "eth0", "wlan0")
 * @param[out] ip_buffer IPv4 주소 문자열을 저장할 버퍼
 * @return int 0: 성공, -1: 실패
 */
int DSCM_GetDCIPv4AddressStrFromIfacename(const char *iface_name, char *ip_buffer)
{
    struct ifaddrs *ifaddr, *ifa;
    int ret = -1;

    if (getifaddrs(&ifaddr) == -1) {
			perror("getifaddrs");
			return -1;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr == NULL)
				continue;
			// IPv4 주소만 확인
			if (ifa->ifa_addr->sa_family == AF_INET) {
				if (strcmp(ifa->ifa_name, iface_name) == 0) {
					struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
					const char *ip = inet_ntop(AF_INET, &addr->sin_addr, ip_buffer, INET_ADDRSTRLEN);
					if (ip) ret = 0;
					break;
				}
			}
    }
    freeifaddrs(ifaddr);
    return ret;
}



/**
 * @brief DC 주행전략 상태정보 전송 처리
 * @details
 * 주행전략 API 중 V2XHUB가 수행해야 하는 DC와의 상태정보 전송 요청을 수행한다. 
 * @param[in] mib 모듈 관리정보
 * @retval 0(성공), -1(실패)
 */
int DSCM_ProcessTxDCDrivingStrategy_V2cVehicleData(DSCM_MIB *mib, const uint8_t *encoded_payload, size_t payload_len)
{
	// 주행전략 Tx  패킷 프레임
	size_t pkt_len = DSCMDC_PKT_STX_LENGTH + sizeof(KVHDSCMMessageHeader) + payload_len +  DSCMDC_PKT_TAIL_LENGTH;
	KVHDSCMMessageFrameTx *frame = (KVHDSCMMessageFrameTx *)malloc(pkt_len);
	if (!frame) {
		return -1;
	}
	
	// DC로 전송할 패킷 생성
	memset(frame, 0, pkt_len);
	/// STX 설정
	frame->STX = htonl(DSCMDC_PKT_STX);
	
	/// 헤더 설정
	frame->header.packet_seqno = htonl(mib->trx_cnt.dc.drive_strategy_data_tx[kKVHDSCM_InVehicleEquipmentCollectionData]);
	frame->header.timestamp = HTONLL(DSCM_EPOCHTIME_MS);
	frame->header.version = 1;
	memcpy(frame->header.auth_info, mib->dc_if.dc_login_info.auth_info, 16);
	frame->header.packet_type = htons((uint16_t)kKVHDSCM_InVehicleEquipmentCollectionData);
	frame->header.packet_flag.type = 0;
	frame->header.packet_flag.isres = 1;
	frame->header.packet_flag.result = 0;
	frame->header.packet_flag.reserved = 0;
	frame->header.packet_flag.restimer = 0;
	frame->header.packet_length = htonl(payload_len);
	/// 페이로드 설정
	uint8_t *payload = frame->payload_data;
	memcpy(payload, encoded_payload, payload_len);

	/// 테일(CRC, ETX) 설정
	uint16_t *crc = (uint16_t *)(payload + payload_len);
	uint16_t calc_crc = htons(DSCM_GetCRC16((uint8_t *)&frame->header, 0, sizeof(KVHDSCMMessageHeader) + payload_len));
	memcpy(crc, &calc_crc, DSCMDC_PKT_CRC_LENGTH);

	uint32_t *etx = (uint32_t *)(crc + DSCMDC_PKT_CRC_LENGTH / sizeof(uint16_t));
	uint32_t calc_etx = htonl(DSCMDC_PKT_ETX);
	memcpy(etx, &calc_etx, DSCMDC_PKT_EXT_LENGTH);

	// DC로 데이터 전송
	int ret = KVHTCP_SendToServer((mib->dc_if.tcpinfo.h), (uint8_t *)frame, pkt_len);
	if (ret < 0) {
		free(frame);
		return -1;
	} else {
		LOG(kKVHLOGLevel_InOut, "Sent DC driving strategy V2cVehicleData pkt(len: %d)\n", pkt_len);
		mib->trx_cnt.dc.drive_strategy_data_tx_t++;
		mib->trx_cnt.dc.drive_strategy_data_tx[kKVHDSCM_InVehicleEquipmentCollectionData]++;
		mib->trx_cnt.dc.timer.login = time(NULL);
	}
	free(frame);
	return 0;	
}


/**
 * @brief DC 주행전략 로그인 처리
 * @details
 * 주행전략 API 중 V2XHUB가 수행해야 하는 DC와의 로그인 요청을 수행한다. 
 * @param[in] mib 모듈 관리정보
 * @retval 0(성공), -1(실패)
 */
int DSCM_ProcessTxDCDrivingStrategyLogin(DSCM_MIB *mib)
{
	KVHDSCMMessageFrameTx *frame; // 주행전략 Tx  패킷 프레임

	// Payload Protobuf 직렬화
	DSCM_PBHandler_Ptr v2cloginreq = DSCM_CreatePBHandler_V2cLoginReq();
	if (!v2cloginreq) {
		return -1;
	}
	if (DSCM_SetPBHandleV2cLoginReqFrom_Str(v2cloginreq, (const char *)mib->dc_if.dc_login_info.auth_info) < 0) {
		return -1;
	}
	char *encoded_payload = NULL;
	size_t payload_len = DSCM_ProcessPBSerializeToString_V2cLoginReq(v2cloginreq, &encoded_payload);
	DSCM_DestroyPBHandler_V2cLoginReq(v2cloginreq);
	if (payload_len == 0) {
		return -1;
	}

	size_t pkt_len = DSCMDC_PKT_STX_LENGTH + sizeof(KVHDSCMMessageHeader) + payload_len +  DSCMDC_PKT_TAIL_LENGTH;
	frame = (KVHDSCMMessageFrameTx *)malloc(pkt_len);
	if (!frame) {
		return -1;
	}
	
	// DC로 전송할 패킷 생성
	
	memset(frame, 0, pkt_len);
	/// STX 설정
	frame->STX = htonl(DSCMDC_PKT_STX);
	
	/// 헤더 설정
	frame->header.packet_seqno = htonl(mib->trx_cnt.dc.drive_strategy_data_tx[kKVHDSCM_CenterLogin]);
	frame->header.timestamp = HTONLL(DSCM_EPOCHTIME_MS);
	frame->header.version = 1;
	memset(frame->header.auth_info, 0x00, 16);
	frame->header.packet_type = htons((uint16_t)kKVHDSCM_CenterLogin);
	frame->header.packet_flag.type = 0;
	frame->header.packet_flag.isres = 0;
	frame->header.packet_flag.result = 0;
	frame->header.packet_flag.reserved = 0;
	frame->header.packet_flag.restimer = 1;
	frame->header.packet_length = htonl(payload_len);
	/// 페이로드 설정
	uint8_t *payload = frame->payload_data;
	memcpy(payload, encoded_payload, payload_len);
	free(encoded_payload);

	/// 테일(CRC, ETX) 설정
	uint16_t *crc = (uint16_t *)(payload + payload_len);
	uint16_t calc_crc = htons(DSCM_GetCRC16((uint8_t *)&frame->header, 0, sizeof(KVHDSCMMessageHeader) + payload_len));
	memcpy(crc, &calc_crc, DSCMDC_PKT_CRC_LENGTH);

	uint32_t *etx = (uint32_t *)(crc + DSCMDC_PKT_CRC_LENGTH / sizeof(uint16_t));
	uint32_t calc_etx = htonl(DSCMDC_PKT_ETX);
	memcpy(etx, &calc_etx, DSCMDC_PKT_EXT_LENGTH);

	// DC로 데이터 전송
	int ret = KVHTCP_SendToServer((mib->dc_if.tcpinfo.h), (uint8_t *)frame, pkt_len);
	if (ret < 0) {
		free(frame);
		return -1;
	} else {
		LOG(kKVHLOGLevel_InOut, "Sent DC driving strategy login pkt(len: %d)\n", pkt_len);
		mib->trx_cnt.dc.drive_strategy_data_tx_t++;
		mib->trx_cnt.dc.drive_strategy_data_tx[kKVHDSCM_CenterLogin]++;
		mib->trx_cnt.dc.timer.login = time(NULL);
	}
	free(frame);
	return 0;	
}


/**
 * @brief 주행전략 시나리오에서 V2X-HUB가 수행해야 하는 작업리스트와 수행
 * @details 
 * @param[in] mib 모듈 관리정보
 * @param[in] dojob 수행할 작업 리스트
 * @retval 0(성공), -1(실패)
 */
int DSCM_ProcessTxDCDrivingStrategyDoJob(DSCM_MIB *mib, KVHDSCMJobList dojob)
{
	//작업리스트에 따라 처리
	switch ((KVHDSCMJobList)dojob) {
		case kKVHDSCM_CenterLogin: // 센터 로그인
			if (mib->dc_if.dc_login_info.login_state) {
				LOG(kKVHLOGLevel_Event, "Already logged in to Data Center\n");
				return -1;
			} else {
				DSCM_ProcessTxDCDrivingStrategyLogin(mib);
			}
			break;
		case kKVHDSCM_InVehicleEquipmentState: // 차량 내 장비 상태 보고
			///< TODO::DSCM_ProcessTxDCDrivingStrategyInVehicleEquipmentState(mib);
			break;
		case kKVHDSCM_InVehicleEquipmentCollectionData: // 차량 내 장비 수집 데이터 보고
			///<  TODO::DSCM_ProcessTxDCDrivingStrategyInVehicleEquipmentCollectionData(mib);
			break;
		default:
			LOG(kKVHLOGLevel_Max, "Unknown packet type in the TCP pkt from Data Center(type: %d)\n", (uint32_t)dojob);
			return -1;
			break;
	}
  return 0;
}


/**
 * @brief 메시지 브로커에 DC 주행전략 로그인 상태 갱신
 * @details
 * 주행전략 API 중 V2XHUB가 수행해야 하는 DC와의 로그인 송신에 대한 응답을 처리한 후,
 * 로그인 토큰 정보를 갱신한다. 
 * *payload가 프로토버퍼지만 라인에서 직접 디코딩)
 */
int DSCM_UpdateDCDrivingStrategyAuthInfo(DSCM_MIB *mib, KVHDSCMMessageFramePtr frame)
{
	// 메시지 브로커에 DC 주행전략 로그인 상태 갱신
	// Protobuf 지만 필드가 한개라 직접 디코딩 (playload[0]: type, payload[1]: length, payload[2~]: auth_token)
	if (frame->payload_data[1] == DSCMDC_PKT_LOGIN_AUTHFICATION_TOKEN_LENGTH) {
		memcpy(mib->dc_if.dc_login_info.auth_token, frame->payload_data + 2, DSCMDC_PKT_LOGIN_AUTHFICATION_TOKEN_LENGTH);
		LOG(kKVHLOGLevel_Event, "Updated DC driving strategy auth info in the message broker\n");
	} else {
		ERR("Failed to update DC driving strategy auth info in the message broker\n");
		return -1;
	}
	
	return 0;
}


/**
 * @brief DC 주행전략 로그인 수신 처리
 * @details
 * 주행전략 API 중 V2XHUB가 수행해야 하는 DC와의 로그인 송신에 대한 응답 수행한다. 
 * 로그인 송신 후 DC로부터 timer(1초) 이내 응답이 없으면 타임아웃으로 간주되지만 응답이 정상일 경우 로그로 보고한다. 
 * @param[in] mib 모듈 관리정보
 * @retval 0(성공), -1(실패)
 */
int DSCM_ProcessRxDCDrivingStrategyLogin(DSCM_MIB *mib, KVHDSCMMessageFramePtr frame)
{
	KVHDSCMMessageHeader header = frame->header;
	uint8_t *payload = frame->payload_data;
	if (header.packet_flag.type == 1 && header.packet_flag.result == 0) { // 로그인에 대한 응답 플레그 확인
		memcpy(mib->dc_if.dc_login_info.auth_token, payload, DSCMDC_PKT_LOGIN_AUTHFICATION_TOKEN_LENGTH);
		mib->dc_if.dc_login_info.login_state = true;
		if ((time(NULL) - mib->trx_cnt.dc.timer.login) > header.packet_flag.restimer) {
			LOG(kKVHLOGLevel_Max, "Login response timeout. (response time: %d sec)\n", (int)(time(NULL) - mib->trx_cnt.dc.timer.login));
		}
		LOG(kKVHLOGLevel_InOut, "Sucess to login to Data Center\n");
		if(DSCM_UpdateDCDrivingStrategyAuthInfo(mib, frame) == 0) { // 메시지 브로커에 로그인 상태 갱신
			DUMP(kKVHLOGLevel_Dump, "Auth Token: \n", (uint8_t*) mib->dc_if.dc_login_info.auth_token, DSCMDC_PKT_LOGIN_AUTHFICATION_TOKEN_LENGTH);
		}
	} else {
		mib->dc_if.dc_login_info.login_state = false;
		ERR("Failed to login to Data Center\n");
		return -1;
	}
	return 0;	
}


/**
 * @brief DC로부터 수신된 주행전략 정보를 처리한다. 
 * @details 
 * DSCM_ProcessRxDCDrivingStrategyTCPPkt 함수에서 호출되며, 주행전략 TCP 패킷의 헤더에서 타입을 확인하고 
 * 그에 따라 페이로드를 처리한다.
 * V2XHUB와 DC가 직접적으로 통신하는 패킷 종류는 아래와 같다.
 * - 센터 로그인/로그아웃(비주기, IF 초기화 시 1회)
 * - 차량 내 장비 상태 보고(비주기, 요청시)
 * - 차내장치 수집정보 보고(주기)
 * @param[in] mib 모듈 관리정보
 * @param[in] data 수신 데이터 (본 데이터는 동적할당되었으므로, 어플리케이션은 데이터 처리 후 free() 해 주어야 한다)
 * @param[in] len 수신 데이터 길이
 * @retval 0(성공), -1(실패)
 */
int DSCM_ProcessRxDCDrivingStrategyDoJob(DSCM_MIB *mib, KVHDSCMMessageFramePtr frame)
{
	KVHDSCMMessageHeader header = frame->header;
		//작업리스트에 따라 처리
		printf("Received DC driving strategy pkt - type: %d, seqno: %d, length: %d\n", ntohs(header.packet_type), ntohl(header.packet_seqno), ntohl(header.packet_length));
	switch ((KVHDSCMJobList)ntohs(header.packet_type)) {
		case kKVHDSCM_CenterLogin: // 센터 로그인
			if (mib->trx_cnt.dc.drive_strategy_data_tx[kKVHDSCM_CenterLogin] - mib->trx_cnt.dc.drive_strategy_data_rx[kKVHDSCM_CenterLogin] == 1) { // login은 응답이 필요한 메시지 이므로 tx 카운트가 rx 카운트 보다 1 커야함.
				DSCM_ProcessRxDCDrivingStrategyLogin(mib, frame);
			} else {
				ERR("No Tx Login Msg or Response timeout.\n");
				return -1;
			}
			break;
		case kKVHDSCM_InVehicleEquipmentState: // 차량 내 장비 상태 보고
			///< TODO::DSCM_ProcessRxDCDrivingStrategyInVehicleEquipmentState(mib, frame);
			break;
		case kKVHDSCM_InVehicleEquipmentCollectionData: // 차량 내 장비 수집 데이터 보고
			///< TODO::DSCM_ProcessRxDCDrivingStrategyInVehicleEquipmentCollectionData(mib);
			break;
		default:
			if (mib->dc_if.mb_enable) {
				// 메시지 브로커 기능 수행
				for (int i = 0; i < DSCMMB_MAX_EQUIPMENTS; i++) {
					if (mib->mb_if.req_equip_list[i].isactive && mib->mb_if.req_equip_list[i].isreq[header.packet_type]) { // 요청 장비에 대해서만 메시지 브로커 기능 수행
						DSCM_PushMBDrivingStrategyDoJob(mib, frame, mib->mb_if.req_equip_list[i].equip_type, mib->mb_if.req_equip_list[i].client_fd, 1); // 메시지 브로커 기능 수행
					}
				}
			} else {
				LOG(kKVHLOGLevel_Err, "Unknown packet type in the TCP pkt from Data Center(type: %d)\n", (uint32_t)header.packet_type);
			}
			return -1;
			break;
	}
  return 0;
}


/**
 * @brief 수신된 주행전략 패킷의 무결성을 검사한다.
 * @details
 * STX(0x7E 0xA5 0x7E 0xA5), ETX(0x0D 0x0A 0x0D 0x0A)을 확인
 * @param[in] frame 수신된 주행전략 패킷 프레임
 * @retval
 */
int DSCM_ProcessRxDCDrivingStrategyPktCheckSTXETX(KVHDSCMMessageFramePtr frame)
{
	// STX 검사
	if (frame->STX != htonl(DSCMDC_PKT_STX)) {
		LOG(kKVHLOGLevel_Err, "Fail to check rx DC driving strategy pkt - invalid STX(0x%08X)\n", htonl(frame->STX));
		return -1;
	}
	// ETX 검사
	if (frame->ETX != htonl(DSCMDC_PKT_ETX)) {
		LOG(kKVHLOGLevel_Err, "Fail to check rx DC driving strategy pkt - invalid ETX(0x%08X)\n", htonl(frame->ETX));
		return -1;
	}


	return 0;
}


/**
 * @brief 수신된 주행전략 패킷의 무결성을 검사한다.
 * @details
 * CRC16(HEADER+PAYLOAD) 체크섬을 검사
 * @param[in] frame 수신된 주행전략 패킷 프레임
 * @retval
 */
int DSCM_ProcessRxDCDrivingStrategyPktCheckCRC(const uint8_t *data, KVHDSCMMessageFramePtr frame)
{
	// CRC16 검사
	size_t payload_len = ntohl(frame->header.packet_length);
	uint16_t calc_crc = DSCM_GetCRC16(data, DSCMDC_PKT_STX_LENGTH, sizeof(KVHDSCMMessageHeader) + payload_len);
	if (ntohs(frame->CRC) != calc_crc) {
		LOG(kKVHLOGLevel_Err, "Fail to check rx DC driving strategy pkt - invalid CRC16(0x%04X != 0x%04X)\n", ntohs(frame->CRC), calc_crc);
		return -1;
	}

	return 0;
}


/**
 * @brief DC로부터 주행전략데이터 TCP 패킷을 수신하면 호출되는 콜백함수 (kvshudp 내 수신 쓰레드에서 호출된다)
 * @param[in] h TCP 라이브러리 기능 핸들 (초기화 API에서 리턴된 핸들)
 * @param[in] data 수신 데이터 (본 데이터는 동적할당되었으므로, 어플리케이션은 데이터 처리 후 free() 해 주어야 한다)
 * @param[in] len 수신 데이터 길이
 * @param[in] priv 어플리케이션 전용 정보
 * @retval 없음
 */
void DSCM_ProcessRxDCDrivingStrategyTCPPkt(KVHTCPHANDLE h, const uint8_t *data, KVHIFDataLen len, void *priv)
{
  LOG(kKVHLOGLevel_Event, "Process rx DC driving strategy data TCP pkt(len: %d)\n", len);
	int ret;
  if (!h || !data || (len == 0) || !priv) {
    ERR("Fail to process rx DC driving strategy data TCP pkt - null (h: %p, data: %p, len: %d, priv: %p)\n", h, data, len, priv);
    return;
  }
	//패킷 길이를 확인
	//페이로드가 가변이기 때문에 페이로드 길이와 메시지 프레임 길이를 합산해서 비교
	size_t payload_len = ((KVHDSCMMessageFramePtr)data)->header.packet_length;
	size_t pkt_len =  sizeof(KVHDSCMMessageFrame) + ntohl(payload_len);
  if (len < pkt_len) {
    ERR("Fail to process rx DC driving strategy status data TCP pkt - too short %d < %d\n", len, pkt_len);
    return;
  }
	KVHDSCMMessageFramePtr frame = (KVHDSCMMessageFramePtr)malloc(pkt_len);
  if (!frame) {
		return; 
	}
	int frame_ptr_pos = 0;
	memcpy(frame, data + frame_ptr_pos, sizeof(KVHDSCMMessageHeader) + DSCMDC_PKT_STX_LENGTH);
	frame_ptr_pos += sizeof(KVHDSCMMessageHeader) + DSCMDC_PKT_STX_LENGTH;
	memcpy(frame->payload_data, data + frame_ptr_pos, ntohl(payload_len));
	frame_ptr_pos = (pkt_len - DSCMDC_PKT_TAIL_LENGTH);
	memcpy(&frame->CRC, data + frame_ptr_pos, DSCMDC_PKT_TAIL_LENGTH);
	
	printf("	Raw Dump:");
	for (size_t i=0; i < pkt_len; i++) {
		printf("%02X", ((uint8_t*)frame)[i]);
	}
	printf("\n");	

	if (DSCM_ProcessRxDCDrivingStrategyPktCheckSTXETX(frame) == 0){
		LOG(kKVHLOGLevel_InOut, "Sucess to check rx DC driving strategy data TCP pkt - type: %d, len: %d\n", ntohs(frame->header.packet_type), ntohl(frame->header.packet_length));
	} else {
		ERR("Fail to check rx DC driving strategy data TCP pkt - type: %d, len: %d\n", ntohs(frame->header.packet_type), ntohl(frame->header.packet_length));
	}
	if(DSCM_ProcessRxDCDrivingStrategyPktCheckCRC(data, frame) == 0){
		LOG(kKVHLOGLevel_InOut, "Sucess to check crc rx DC driving strategy data TCP pkt - type: %d, len: %d\n", ntohs(frame->header.packet_type), ntohl(frame->header.packet_length));
	} else {
		ERR("Fail to check crc rx DC driving strategy data TCP pkt - type: %d, len: %d\n", ntohs(frame->header.packet_type), ntohl(frame->header.packet_length));
	}

	/*
   * 수신된 데이터를 처리한다.
   */
  ret = DSCM_ProcessRxDCDrivingStrategyDoJob((DSCM_MIB *)priv, frame);
	if (ret == 0) {
		LOG(kKVHLOGLevel_InOut, "Sucess to process rx DC driving strategy data TCP pkt - type: %d, len: %d\n", ntohs(frame->header.packet_type), ntohl(frame->header.packet_length));
	} else {
		ERR("Fail to process rx DC driving strategy data TCP pkt - type: %d, len: %d\n", ntohs(frame->header.packet_type), ntohl(frame->header.packet_length));
	}
  
	free(frame);
	return;
}


/**
 * @brief DC와의 주행전략 데이터 송수신 TCP I/F를 초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int DSCM_InitDCIF_DrivingStrategyTrx(DSCM_MIB *mib)
{
	// TCP I/F 초기화
	// 초기화 시 Server와 Connection을 시도하고 성공해야 동작하기 때문에 Server와 IP 통신이 가능한 상태여야 한다.  
	if (strlen(mib->dc_if.dc_eth_if) > 0) {
		DSCM_GetDCIPv4AddressStrFromIfacename(mib->dc_if.dc_eth_if, mib->dc_if.dc_client_addr);
	}
	mib->dc_if.tcpinfo.h = KVHTCP_InitClient( (char *)mib->dc_if.dc_client_addr,
																						&mib->dc_if.dc_client_port,
																						(char *)mib->dc_if.dc_server_addr,
																						mib->dc_if.dc_server_port,
																						DSCM_ProcessRxDCDrivingStrategyTCPPkt,
																						(void *)mib);

	if (!mib->dc_if.tcpinfo.h) {
		ERR("Fail to init DC driving strategy TCP I/F - KVHTCP_Init()\n");
		return -1;
	}
	pthread_detach(((KVHDSCMTCP_CTX *)mib->dc_if.tcpinfo.h)->rx_thread.th);
	LOG(kKVHLOGLevel_Init, "Success to initialize DC driving strategy TCP I/F\n");
	return 0;
}


/**
 * @brief DC와의 I/F 초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int DSCM_InitDCIF(DSCM_MIB *mib)
{
	/// DC와 TCP I/F 초기화
	if (DSCM_InitDCIF_DrivingStrategyTrx(mib) < 0) {
		return -1;
	}

	/// DC와 로그인 요청
	sleep(1);
	KVHDSCMTCP_CTX *dc_ctx = (KVHDSCMTCP_CTX *)mib->dc_if.tcpinfo.h;
	if (dc_ctx->sock.session_established) { // Login Action Do
		if (DSCM_ProcessTxDCDrivingStrategyDoJob(mib, kKVHDSCM_CenterLogin) == 0) {
			LOG(kKVHLOGLevel_Event, "Success Send to login to Data Center\n");
		} 
	}

	return 0;
}


#if 0 /// not used
/**
 * @brief 외부망에 통신하기 위한 라우팅 테이블, IPTALBES 설정
 * @details
 * 외부망과 통신하기 위해서는 외부망(PN, Public Network)과 통신하는 Ethernet 장치가 필요하다.
 * IPTABLES MASQUERADE 설정을 통해 외부망으로 나가는 패킷의 출발지 IP주소를 외부망과 연결된 장치의 IP주소로 변경한다.
 * 이것은, 내부망 IP로 송신하게 되면 수신받은 장치가 반송할 때, 목적지 IP를 설정할 수 없는 문제를 해결하는
 * NAT(Network Address Translation) 방법이다. 
 * 외부망과 통신하고자 하는 내부망의 장치(IP, Port)는 목적지를 특정해야만 목적지로부터 응답을 받을 수 있다.
 * (외부망으로 변환된 후 수신한 패킷을 각 내부망으로 전달하기 위해서는 외부망을 필터링 해야한다. 
 * 이렇게 필터링된 외부망(IP, Port)는 다른 내부망에 연결될 수 없다. 
 * 본 설정은 DSCM 모듈이 부팅될 때 한번만 설정하면 된다.  
 * @param[in] mib 모듈 통합관리정보(MIB)
 * @return 0(성공), -1(실패)
 */
 int DSCM_SetRoutingAndIptablesForPN(DSCM_MIB *mib)
 {
	//외부망과 통신하는 Ethernet 장치명
	char *ipv4_src_addr_str = mib->dc_if.if_ipv4_src_addr_str;
	char *if_pn_dev_name = mib->dc_if.if_pn_dev_name;
	/**
	 * @brief IPTABLES에 MASQUERADE 설정
	 */

	 char cmd[256] = {0,};
	 snprintf(cmd, sizeof(cmd), "iptables -t nat -A POSTROUTING -s %s -o %s -j MASQUERADE", ipv4_src_addr_str, if_pn_dev_name);
	 system (cmd);
	 LOG(kKVHLOGLevel_Init, "Set IPTABLES MASQUERADE - %s\n", cmd);

	 return 0;
 }
	
 #endif



 