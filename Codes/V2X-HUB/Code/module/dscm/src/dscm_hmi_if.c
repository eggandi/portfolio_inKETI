/**
 * @file
 * @brief  HMI와의 I/F 기능 구현(TCPIP over A-Ethernet)
 * @date 2025-09-17
 * @author donggyu
 */

#include <dscm.h>

/**
 * @brief HMI의 주행전략 메시지 전달 요청 수신 처리
 * @details
 * 주행전략 API 중 V2XHUB가 수행해야 하는 HMI의 메시지 전달 요청(Message Broker 기능) 수신에 대한 응답 수행한다. 
 * @param[in] mib 모듈 관리정보
 * @param[in] rx_frame 수신된 메시지 프레임
 * @param[in] DrivingStrategyMessageDeliveryRequest를 송신한 client_fd 클라이언트 소켓
 * @retval 0(성공), -1(실패)
 */
int DSCM_ProcessRxHMIDrivingStrategyMessageDeliveryResponse(DSCM_MIB *mib, KVHDSCMMessageFramePtr rx_frame, const int client_fd)
{
	if (mib || rx_frame || client_fd < 0) {
		ERR("Invalid parameters. mib:%p, rx_frame:%p, client_fd:%d\n", mib, rx_frame, client_fd);
		return -1;
	}
	///< 수신된 패킷의 페이로드에서 push_msg_list를 확인한다.
	KVHDSCMMessageHeader rx_header = rx_frame->header;
	uint32_t rx_payload_len = ntohl(rx_header.packet_length);
	KVHDSCMMessageDeliveryRequest *rx_msg = (KVHDSCMMessageDeliveryRequest *)rx_frame->payload_data;
	int push_msg_list_count = (rx_payload_len - sizeof(KVHDSCMMessageDeliveryRequest)) / sizeof(KVHDSCMDrivingStrategyPktType);

	///< TCP 패킷을 생성한다.
	KVHDSCMMessageFrameTx *frame; // 주행전략 Tx  패킷 프레임
	size_t payload_len = sizeof(KVHDSCMMessageDeliveryRequest) + push_msg_list_count * sizeof(KVHDSCMDrivingStrategyPktType);
	size_t pkt_len = DSCMDC_PKT_STX_LENGTH + sizeof(KVHDSCMMessageHeader) + payload_len +  DSCMDC_PKT_TAIL_LENGTH;
	frame = (KVHDSCMMessageFrameTx *)malloc(pkt_len);
	if (!frame) {
		return -1;
	}
	memset(frame, 0, pkt_len);

	/// STX 설정
	memcpy(&frame->header, &rx_frame->header, sizeof(KVHDSCMMessageHeader));
	/// 헤더 설정
	frame->header.packet_seqno = htonl(mib->trx_cnt.hmi.tx);
	frame->header.timestamp = HTONLL(DSCM_EPOCHTIME_MS);
	frame->header.version = 1;
	memset(frame->header.auth_info, 0x00, DSCMDC_PKT_LOGIN_AUTHFICATION_TOKEN_LENGTH);
	frame->header.packet_type = htons((uint16_t)kKVHDSCM_MessageDeliveryRequest);
	frame->header.packet_flag.type = 1;
	frame->header.packet_flag.isres = 0;
	frame->header.packet_flag.result = 0;
	frame->header.packet_flag.reserved = 0;
	frame->header.packet_flag.restimer = 0;

	/// 페이로드 설정
	KVHDSCMMessageDeliveryResponse *tx_msg = (KVHDSCMMessageDeliveryResponse *)frame->payload_data;
	tx_msg->timestamp = rx_msg->timestamp;
	for (int equip = 0; equip < DSCMMB_MAX_EQUIPMENTS; equip++) {
		if (mib->mb_if.req_equip_list[equip].isactive && mib->mb_if.req_equip_list[equip].client_fd == client_fd) {
				for (int i = 0; i < push_msg_list_count; i++) {
					tx_msg->push_msg_list[i] = mib->mb_if.req_equip_list[equip].push_msg_list[rx_msg->push_msg_list[i]];
				}
			break;
		}
	}


	/// 테일(CRC, ETX) 설정
	uint16_t *crc = (uint16_t *)(frame->payload_data + payload_len);
	uint32_t *etx = (uint32_t *)(crc + DSCMDC_PKT_CRC_LENGTH);
	uint16_t calc_crc = htons(DSCM_GetCRC16((uint8_t *)&frame->header, 0, sizeof(KVHDSCMMessageHeader) + payload_len));
	uint32_t calc_etx = htonl(DSCMDC_PKT_ETX);
	memcpy(crc, &calc_crc, DSCMDC_PKT_CRC_LENGTH);
	memcpy(etx, &calc_etx, DSCMDC_PKT_EXT_LENGTH);

	int ret = KVHTCP_SendToClient(&mib->hmi_if.tcpinfo.h, client_fd, kKVHTCPClientType_HMI, (uint8_t *)frame, pkt_len);
	if (ret < 0) {
		free(frame);
		return -1;
	} else {//
		LOG(kKVHLOGLevel_InOut, "Sent HMI driving strategy login pkt(len: %d)\n", pkt_len);
		mib->trx_cnt.hmi.tx++;
	}

	return ret;
}


/**
 * @brief 수신된 주행전략 정보를 처리한다. 
 * @details 
 * DSCM_ProcessTxHMITCPPkt 함수에서 호출되며, 주행전략 TCP 패킷의 헤더에서 타입을 확인하고 
 * @param[in] mib 모듈 관리정보
 * @param[in] data 수신 데이터 (본 데이터는 동적할당되었으므로, 어플리케이션은 데이터 처리 후 free() 해 주어야 한다)
 * @param[in] len 수신 데이터 길이
 * @retval 0(성공), -1(실패)
 */
int DSCM_ProcessTxHMIDrivingStrategyDoJob(DSCM_MIB *mib, KVHDSCMMessageFramePtr frame, const int client_fd)
{
	KVHDSCMMessageHeader header = frame->header;
	///< 작업리스트에 따라 처리
	switch ((KVHDSCMJobList)ntohs(header.packet_type)) {
		case kKVHDSCM_MessageDeliveryRequest: // 메시지 전달 요청
			if (mib->dc_if.dc_login_info.login_state) { // 메시지 전달 요청은 응답이 필요한 메시지 이므로 로그인 상태여야 함.
				DSCM_ProcessRxHMIDrivingStrategyMessageDeliveryResponse(mib, frame, client_fd);
			} else {
				ERR("DSCM is not connected to Data Center.\n");
				return -1;
			}
			break;
		default:
			if (mib->hmi_if.mb_enable) {
				if(DSCM_PushMBDrivingStrategyDoJob(mib, frame, kKVHDSCM_EquipmentStatusHMI, client_fd, 0)) { // 메시지 브로커 기능 수행
					
					///< TODO::massage broker 기능 수행 시 요청시 Enable된 API 리스트만 Push 하는 기능 추가 필요
					for (int i = 0; i < DSCMMB_MAX_EQUIPMENTS; i++) {
						if (mib->mb_if.req_equip_list[i].isactive && mib->mb_if.req_equip_list[i].client_fd == client_fd) { // 요청 장비에 대해서만 메시지 브로커 기능 수행, 다수 장비가 요청할 때 다수에게 전달
							
						}
					}
				}
			} else {
				LOG(kKVHLOGLevel_Err, "Unknown packet type in the TCP pkt from HMI (type: %d)\n", (uint32_t)header.packet_type);
			}
			return -1;
			break;
	}
  return 0;
}


/**
 * @brief HMI의 주행전략 메시지 전달 요청 수신 처리
 * @details
 * 주행전략 API 중 V2XHUB가 수행해야 하는 HMI의 메시지 전달 요청(Message Broker 기능) 수신에 대한 응답 수행한다. 
 * @param[in] mib 모듈 관리정보
 * @retval 0(성공), -1(실패)
 */
int DSCM_ProcessRxHMIDrivingStrategyMessageDeliveryRequest(DSCM_MIB *mib, KVHDSCMMessageFramePtr frame, const int client_fd)
{
	KVHDSCMMessageHeader header = frame->header;
	KVHDSCMTCP_CTX *ctx = (KVHDSCMTCP_CTX *)mib->hmi_if.tcpinfo.h;

	if (header.packet_flag.type == 0 && header.packet_flag.result == 1) { // 로그인에 대한 응답 플레그 확인
		KVHDSCMMessageDeliveryRequest *msg = (KVHDSCMMessageDeliveryRequest *)frame->payload_data;
		LOG(kKVHLOGLevel_InOut, "Received message delivery request from HMI equip_type: %d, system_id: %.16s\n", msg->equip_type, msg->equip_system_id);
		uint32_t payload_len = ntohl(header.packet_length);
		///< 메시지 전달 요청 장비 처리
		switch (msg->equip_type) {
			case kKVHDSCM_EquipmentStatusALL:
			case kKVHDSCM_EquipmentStatusIVDCT:
			case kKVHDSCM_EquipmentStatusADS:
			case kKVHDSCM_EquipmentStatusV2XHUB:
			case kKVHDSCM_EquipmentStatusDMS:
			case kKVHDSCM_EquipmentStatusDTG:
				LOG(kKVHLOGLevel_Err, "No support equipment type in the message delivery request from Invehicle Equipment(equip_type: %d)\n", (uint32_t)msg->equip_type);
				break;
			case kKVHDSCM_EquipmentStatusHMI: // 현재 HMI만 지원
				for (int i = 0; i < DSCMMB_MAX_EQUIPMENTS; i++) {
					if (mib->mb_if.req_equip_list[i].isactive == false) {
						mib->mb_if.req_equip_list[i].isactive = true;
						mib->mb_if.req_equip_list[i].equip_type = msg->equip_type;
						mib->mb_if.req_equip_list[i].client_fd = client_fd; // 요청 장치
						ctx->sock.client_sock[client_fd] = msg->equip_type; // 클라이언트 소켓 종류 설정

						///< 메시지 유형 처리
						for (uint32_t j = 0; j < (payload_len - sizeof(KVHDSCMMessageDeliveryRequest)) / sizeof(KVHDSCMDrivingStrategyPktType); j++) {
						if (msg->push_msg_list[j] != kKVHDSCM_DrivingStrategyPktType_None) {
								LOG(kKVHLOGLevel_InOut, " - push_msg_list[%d]: %d\n", j, msg->push_msg_list[j]);
								///< TODO:: 주행전략 API 중 DC가 응답가능한 API 유형만 true로 설정해야 하므로 향 후에 DC와 사용 가능 API 리스트 받는 작업 필요.
								/// 현재는 모든 유형을 true로 설정
								mib->mb_if.req_equip_list[i].push_msg_list[msg->push_msg_list[j]] = true; // 요청 메시지 유형
							}
						}
						break;
					} else {
						if (mib->mb_if.req_equip_list[i].client_fd == client_fd) { // 이미 등록된 클라이언트 소켓이면 메시지 유형만 갱신
							break;
						}
					}
				}
				break;
			default:
				LOG(kKVHLOGLevel_Err, "Unknown equipment type in the message delivery request from Invehicle Equipment(equip_type: %d)\n", (uint32_t)msg->equip_type);
				return -1;
		}

		///< 메시지 전달 요청 Request에 대한 Response 수행
		if (DSCM_ProcessTxHMIDrivingStrategyDoJob(mib, frame, client_fd) < 0) { // 응답 전송
			ERR("Fail to process tx HMI driving strategy do job\n");
			return -1;
		}
	} else{
		ERR("Fail to process rx HMI driving strategy message delivery request\n");
		return -1;
	}

	return 0;
	
}

/**
 * @brief 수신된 주행전략 정보를 처리한다. 
 * @details 
 * DSCM_ProcessTxHMITCPPkt 함수에서 호출되며, 주행전략 TCP 패킷의 헤더에서 타입을 확인하고 
 * 그에 따라 페이로드를 처리한다.
 * V2XHUB와 HMI가 직접적으로 통신하는 패킷 종류는 아래와 같다.
 * - 메시지 전달 요청
 * @param[in] mib 모듈 관리정보
 * @param[in] data 수신 데이터 (본 데이터는 동적할당되었으므로, 어플리케이션은 데이터 처리 후 free() 해 주어야 한다)
 * @param[in] len 수신 데이터 길이
 * @retval 0(성공), -1(실패)
 */
int DSCM_ProcessTxHMIDoJob(DSCM_MIB *mib, KVHDSCMMessageFramePtr frame, const int client_fd)
{
	KVHDSCMMessageHeader header = frame->header;
		//작업리스트에 따라 처리
	switch ((KVHDSCMJobList)ntohs(header.packet_type)) {
		case kKVHDSCM_MessageDeliveryRequest: // 메시지 전달 요청
			if (mib->dc_if.dc_login_info.login_state) { // 메시지 전달 요청은 응답이 필요한 메시지 이므로 로그인 상태여야 함.
				DSCM_ProcessRxHMIDrivingStrategyMessageDeliveryRequest(mib, frame, client_fd);
			} else {
				ERR("DSCM is not connected to Data Center.\n");
				return -1;
			}
			break;
		default:
			LOG(kKVHLOGLevel_Err, "Unknown packet type in the TCP pkt from HMI(type: %d)\n", (uint32_t)header.packet_type);
			return -1;
			break;
	}
  return 0;
}


/**
 * @brief HMI로부터 수신된 주행전략 패킷의 무결성을 검사한다.
 * @details
 * STX(0x7E 0xA5 0x7E 0xA5), ETX(0x0D 0x0A 0x0D 0x0A)을 확인
 * CRC16(HEADER+PAYLOAD) 체크섬을 검사
 * @param[in] frame 수신된 주행전략 패킷 프레임
 * @retval
 */
int DSCM_ProcessTxHMIPktCheck(KVHDSCMMessageFramePtr frame)
{
	// STX 검사
	if (frame->STX != htonl(DSCMDC_PKT_STX)) {
		LOG(kKVHLOGLevel_Err, "Fail to check rx HMI pkt - invalid STX(0x%08X)\n", htonl(frame->STX));
		return -1;
	}
	// ETX 검사
	if (frame->ETX != htonl(DSCMDC_PKT_ETX)) {
		LOG(kKVHLOGLevel_Err, "Fail to check rx HMI pkt - invalid ETX(0x%08X)\n", htonl(frame->ETX));
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
void DSCM_ProcessTxHMITCPPkt(KVHTCPHANDLE h, const uint8_t *data, KVHIFDataLen len, void *priv)
{
  LOG(kKVHLOGLevel_Event, "Process rx HMI data TCP pkt(len: %d)\n", len);
	int ret;
  if (!h || !data || (len == 0) || !priv) {
    ERR("Fail to process rx HMI data TCP pkt - null (h: %p, data: %p, len: %d, priv: %p)\n", h, data, len, priv);
    return;
  }
	KVHDSCMTCP_CTX *ctx = (KVHDSCMTCP_CTX *)h;
	DSCM_MIB *mib = (DSCM_MIB *)ctx->app_priv;
	//패킷 길이를 확인
	//페이로드가 가변이기 때문에 페이로드 길이와 메시지 프레임 길이를 합산해서 비교
	size_t payload_len = ((KVHDSCMMessageFramePtr)data)->header.packet_length;
	size_t pkt_len = sizeof(KVHDSCMMessageFramePtr) + payload_len;
  if (len < pkt_len) {
    ERR("Fail to process rx HMI status data TCP pkt - too short %d < %d\n", len, pkt_len);
    return;
  }

	KVHDSCMMessageFramePtr frame = (KVHDSCMMessageFramePtr)malloc(pkt_len);
  if (!frame) {
		return; 
	}
	memcpy(frame, data, pkt_len);

	if (DSCM_ProcessTxHMIPktCheck(frame) == 0){
		LOG(kKVHLOGLevel_InOut, "Sucess to check rx HMI data TCP pkt - type: %d, len: %d\n", ntohs(frame->header.packet_type), ntohl(frame->header.packet_length));
		mib->trx_cnt.hmi.rx++; // HMI 수신 카운트 증가
	} else {
		ERR("Fail to check rx HMI data TCP pkt - type: %d, len: %d\n", ntohs(frame->header.packet_type), ntohl(frame->header.packet_length));
	}

	/*
   * 수신된 데이터를 처리한다.
   */
	int now_client_fd;
	memcpy(&now_client_fd, priv, sizeof(int));
	free(priv); // 더 이상 필요없으므로 해제
  ret = DSCM_ProcessTxHMIDoJob(mib, frame, now_client_fd);
	if (ret == 0) {
		LOG(kKVHLOGLevel_InOut, "Sucess to process rx HMI data TCP pkt - type: %d, len: %d\n", ntohs(frame->header.packet_type), ntohl(frame->header.packet_length));
	} else {
		ERR("Fail to process rx HMI data TCP pkt - type: %d, len: %d\n", ntohs(frame->header.packet_type), ntohl(frame->header.packet_length));
	}
  
	free(frame);
	return;
}


/**
 * @brief HMI와의 I/F 초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int DSCM_InitHMIIF(DSCM_MIB *mib)
{
	// TCP I/F 초기화
	// 초기화 시 Server와 Connection을 시도하고 성공해야 동작하기 때문에 Server와 IP 통신이 가능한 상태여야 한다.  
	mib->hmi_if.tcpinfo.h = KVHTCP_InitServer((char *)mib->hmi_if.hmi_server_addr,
																						mib->hmi_if.hmi_server_port,
																						DSCM_ProcessTxHMITCPPkt,
																						(void *)mib);


	return 0;
}