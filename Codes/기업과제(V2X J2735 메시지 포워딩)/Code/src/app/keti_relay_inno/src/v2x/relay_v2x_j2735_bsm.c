#include "relay_v2x_j2735_bsm.h"
#include "relay_config.h"

extern struct relay_inno_gnss_data_t *G_gnss_data;
extern struct relay_inno_gnss_data_bsm_t *G_gnss_bsm_data;

uint8_t G_j29451_mac_addr_arry[6];

static struct j2735BSMcoreData *g_core; 
static bool g_bsm_core_data_installed = false; 

static uint8_t g_msg_bsm_tx_cnt = 0; 
static uint8_t g_temporary_id[RELAY_INNO_TEMPORARY_ID_LEN] = { 0x00, 0x01, 0x02, 0x03 }; 
static int g_transmission = 7; // neutral
static int g_angle = 127; 
static int g_traction = 0; // unavailable
static int g_abs = 0; // off
static int g_scs = 0; // on
static int g_brake_boost = 0; // on
static int g_aux_brakes = 0; // on
static int g_wheel_brakes_unavailable = 1; // true
static int g_wheel_brakes_left_front = 0; // false
static int g_wheel_brakes_left_rear = 0; // true
static int g_wheel_brakes_right_front = 0; // false
static int g_wheel_brakes_right_rear = 0; // true
static int g_vehicle_length = 1000; 
static int g_vehicle_width = 1001; 

#define RELAY_INNO_MAX_PATHHISTORYPOINT 23
struct relay_inno_PathHistoryPointList_t
{
	j2735PathHistoryPoint tab[RELAY_INNO_MAX_PATHHISTORYPOINT];
  size_t count;
};

static struct relay_inno_PathHistoryPointList_t g_pathhistorypointlistlist = {.count = 0};
static void RELAY_INNO_J2736_J29451_Tx_Callback(const uint8_t *bsm, size_t bsm_size, bool event, bool cert_sign, bool id_change, uint8_t *addr);
static int RELAY_INNO_J2735_J29451_RegisterTransmitFlow(J29451BSMTxInterval interval, LTEV2XHALPriority priority);

static int RELAY_INNO_J2735_BSM_SecMark(); 
static int RELAY_INNO_J2735_BSM_Fill_CoreData(struct j2735BSMcoreData *core_ptr); 
static int RELAY_INNO_J2735_BSM_Fill_PartII(struct j2735PartIIcontent_1 *partII_ptr);
static size_t RELAY_INNO_J2735_BSM_Push_Pathhistroty();
static size_t RELAY_INNO_J2735_BSM_Move_Pathhistroty();

/**
 * @brief J29451 라이브러리를 초기화한다.
 * @details J29451 라이브러리를 초기화하고 BSM 송신 콜백함수를 등록한다.
 * @retval 0: 성공
 * @retval -1: 실패
 */
EXTERN_API int RELAY_INNO_J2736_J29451_Initial()
{
	int ret;
	ret = J29451_Init(G_relay_inno_config.v2x.lib_dbg, G_j29451_mac_addr_arry);
  if (ret < 0) {
    _DEBUG_PRINT("Fail to initialize j29451 library - J29451_Init() failed: %d\n", ret);
    return -1;
  }
  J29451_RegisterBSMTransmitCallback(RELAY_INNO_J2736_J29451_Tx_Callback);
  J29451_LoadPathInfoBackupFile("./bsm_path.history"); // 백업된 Path 정보 로딩
  _DEBUG_PRINT("Success to initialize j29451 libraries\n");
	
	ret = RELAY_INNO_J2735_J29451_RegisterTransmitFlow(G_relay_inno_config.v2x.j2735.bsm.interval, G_relay_inno_config.v2x.j2735.bsm.priority);
	if (ret < 0) {
    return -2;
  }
	/*
   * BSM 필수정보를 설정한다.
   */
  ret = J29451_SetVehicleSize(150, 250);
  if (ret < 0) 
	{
    _DEBUG_PRINT("Fail to start BSM transmit - J29451_SetVehicleSize() failed: %d\n", ret);
    return -3;
  }

	return 0;
}

/**
 * @brief LTE-V2X 송신 플로우를 등록한다.
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int RELAY_INNO_J2735_J29451_RegisterTransmitFlow(J29451BSMTxInterval interval, LTEV2XHALPriority priority)
{
  _DEBUG_PRINT("Preapare LTE-V2X transmit\n");

  struct LTEV2XHALTxFlowParams params;
  memset(&params, 0x00, sizeof(struct LTEV2XHALTxFlowParams));
  params.index = kLTEV2XHALTxFLowIndex_SPS1;
  params.interval = interval;
  params.priority = priority;
  params.size = kLTEV2XHALMSDUSize_Max;

  int ret = 0;//= LTEV2XHAL_RegisterTransmitFlow(params);
  if (ret < 0) {
    _DEBUG_PRINT("Fail to prepare LTE-V2X transmit - LTEV2XHAL_RegisterTransmitFlow() failed: %d\n", ret);
    return -1;
  }

  _DEBUG_PRINT("Success to prepare LTE-V2X transmit\n");
  return 0;
}
/**
 * @brief j29451 라이브러리가 호출하는 BSM 송신 콜백함수
 * @param[in] bsm BSM 메시지 UPER 인코딩 바이트열
 * @param[in] bsm_size BSM 메시지의 길이
 * @param[in] event 이벤트 발생 여부
 * @param[in] cert_sign 인증서로 서명해야 하는지 여부
 * @param[in] id_change ID/인증서 변경 필요 여부
 * @param[in] addr 랜덤하게 생성된 MAC 주소. id_change=true일 경우 본 MAC 주소를 장치에 설정해야 한다.
 */
static void RELAY_INNO_J2736_J29451_Tx_Callback(const uint8_t *bsm, size_t bsm_size, bool event, bool cert_sign, bool id_change, uint8_t *addr)
{
  _DEBUG_PRINT("BSM tx callback - event: %u, cert_sign: %u, id_change: %u\n", event, cert_sign, id_change);
	
	ASN1Error ans1_err;
	j2735MessageFrame *frame = NULL;
	j2735BasicSafetyMessage *bsm_tmp = NULL;
	j2735PartIIcontent_1 *tab = NULL;
	// BSM 메시지 UPER 인코딩 바이트열을 j2735MessageFrame 구조체로 디코딩한다.
	asn1_uper_decode((void **)&frame, asn1_type_j2735MessageFrame, bsm, bsm_size, &ans1_err);
	if (frame == NULL) {
		_DEBUG_PRINT("Fail to Asn1 UPER Decode[%s]\n", ans1_err.msg);
	}else{
			
		// BSM 메시지 PartII에 SupplementalVehicleExtensions을 추가한다.
		bsm_tmp = (j2735BasicSafetyMessage *)frame->value.u.data;
		bsm_tmp->partII.count = 2;
		j2735PartIIcontent_1 *content_tmp = (j2735PartIIcontent_1 *)(bsm_tmp->partII.tab);
		tab = asn1_mallocz(asn1_get_size(asn1_type_j2735PartIIcontent_1) * bsm_tmp->partII.count);
		asn1_copy_value(asn1_type_j2735PartIIcontent_1, tab, bsm_tmp->partII.tab);
		bsm_tmp->partII.tab = tab;
		asn1_free_value(asn1_type_j2735PartIIcontent_1, content_tmp);
		struct j2735PartIIcontent_1 *tab_now = tab + 1;
		if(tab_now != NULL)
		{
			tab_now->partII_Id = 0x02;//SupplementalVehicleExtensions
			RELAY_INNO_J2735_BSM_Fill_PartII(tab_now);
		}	
	}
	uint8_t *encoded_bsm = NULL;
		// BSM 메시지 구조체를 다시 UPER 인코딩한다
	size_t encoded_bsm_size = (size_t)asn1_uper_encode(&encoded_bsm, asn1_type_j2735MessageFrame, frame);
	asn1_free_value(asn1_type_j2735MessageFrame, frame);
	if (encoded_bsm == NULL) {
		_DEBUG_PRINT("Fail to encode BSM - asn1_uper_encode() failed\n");
		return;
	}


  /*
   * 필요 시 MAC 주소를 변경한다.
   */
  if (id_change == true) {
    _DEBUG_PRINT("BSM tx callback - id changed - new MAC addr: "MAC_ADDR_FMT"\n", MAC_ADDR_FMT_ARGS(addr));
    memcpy(G_j29451_mac_addr_arry, addr, MAC_ALEN);
  }

	const uint8_t *wsdu = NULL;
	size_t wsdu_size = 0;
	struct Dot2SPDUConstructResult res;
	if(G_relay_inno_config.v2x.dot2.enable == true)
	{
		/*
		* Signed SPDU를 생성한다.
		*/

		Dot2SignerIdType signer_id;
		if (cert_sign == true) {
			_DEBUG_PRINT("BSM tx callback - Force to sign with certificate\n");
			signer_id = kDot2SignerId_Certificate;
		} else {
			signer_id = kDot2SignerId_Profile;
		}
		struct Dot2SPDUConstructParams params;

		memset(&params, 0, sizeof(params));
		params.type = kDot2SPDUConstructType_Unsecured;
		params.time = 0; // SPDU 생성 시각 (어플리케이션이 0으로 설정할 경우, API 내부에서 현재시각으로 설정된다)
		params.signed_data.psid = 32; // BSM_PSID;
		params.signed_data.signer_id_type = signer_id;
		params.signed_data.cmh_change = id_change;
		
		res = Dot2_ConstructSPDU(&params, encoded_bsm, encoded_bsm_size);
		if (res.ret < 0) {
			free(encoded_bsm);
			_DEBUG_PRINT("BSM tx callback - Dot2_ConstructSPDU() failed: %d\n", res.ret);
			return;
		}
		
		wsdu = res.spdu;
		wsdu_size = (size_t)res.ret;
	}else{
		wsdu = bsm;
		wsdu_size = bsm_size;
	}

	/*
	 * WSM을 생성한다.
	 */
	_DEBUG_PRINT("Success to encode BSM - %ld bytes\n", wsdu_size);
	LTEV2XHALPriority priority = (event == true) ? 7 : G_relay_inno_config.v2x.j2735.bsm.priority;
  struct Dot3WSMConstructParams wsm_params;
  memset(&wsm_params, 0, sizeof(wsm_params));
  wsm_params.chan_num = G_relay_inno_config.v2x.chan_num;
  wsm_params.datarate =  G_relay_inno_config.v2x.tx_datarate;
  wsm_params.transmit_power =  G_relay_inno_config.v2x.tx_power;
  wsm_params.psid = 32; // BSM_PSID;
  size_t wsm_size;
	int ret = 0;
  uint8_t *wsm = Dot3_ConstructWSM(&wsm_params, wsdu, wsdu_size, &wsm_size, &ret);
  if (wsm == NULL) 
	{
		free(encoded_bsm);
		encoded_bsm = NULL;
		//free(res.spdu);
    _DEBUG_PRINT("Fail to construct LTE-V2X WSM - Dot3_ConstructWSM() failed - %d\n", ret);
    return;
  }
  _DEBUG_PRINT("Success to construct %ld-bytes LTE-V2X WSM\n", wsm_size);

  /*
   * WSM을 전송한다.
   */
  struct LTEV2XHALMSDUTxParams tx_params;
  memset(&tx_params, 0x00, sizeof(struct LTEV2XHALMSDUTxParams));
  tx_params.tx_flow_type = G_relay_inno_config.v2x.j2735.bsm.tx_type;
  tx_params.tx_flow_index = kLTEV2XHALTxFLowIndex_SPS1;
  tx_params.priority = priority;
  tx_params.tx_power = G_relay_inno_config.v2x.j2735.bsm.tx_power;
  tx_params.dst_l2_id = kLTEV2XHALL2ID_Broadcast;

  ret = LTEV2XHAL_TransmitMSDU(wsm, wsm_size, tx_params);
	if (ret != 0 && ret != -7)
	{
		_DEBUG_PRINT("Fail to transmit LTE-V2X WSM - LTEV2XHAL_TransmitMSDU() failed: %d\n", ret);
		free(encoded_bsm);
		encoded_bsm = NULL;
		free(wsm);
		wsm = NULL;
		return;
	}
	G_BSM_TX_RUNNING = true;
	_DEBUG_PRINT("Success to transmit LTE-V2X WSM\n");
	if(ret == 0 || ret == -7)
	{
		switch(G_relay_inno_config.relay.relay_data_type)
		{
			case RELAY_DATA_TYPE_V2X_SSDU:
			{
				int udp_ret = sendto(G_relay_v2x_tx_socket, encoded_bsm, encoded_bsm_size, 0, (struct sockaddr *)&G_relay_v2x_tx_addr, sizeof(G_relay_v2x_tx_addr));
				if(udp_ret < 0)
				{
					_DEBUG_PRINT("Fail to sendto - %d sendto() failed: %d\n",G_relay_inno_config.relay.relay_data_type, udp_ret);
				}else{
					_DEBUG_PRINT("Success to sendto - %d sendto() success: %d\n",G_relay_inno_config.relay.relay_data_type, udp_ret);
				}
				break;
			}	
			default:
			{
				break;
			}
		}
	}
	if(encoded_bsm != NULL)
	{
		free(encoded_bsm);
		encoded_bsm = NULL;
	}
	if(wsm != NULL)
	{
		free(wsm);
		wsm = NULL;
	}	
	if(G_relay_inno_config.v2x.dot2.enable == true)
	{
		// SPDU 메모리 해제
		free(res.spdu);
		// 현재 인증서가 이미 만기되었거나 다음번 BSM 서명 시에 만기될 경우에는, BSM ID 변경을 요청한다.
		// 다음번 BSM 콜백함수가 호출될 때, id_change=true, cert_sign=true 가 전달된다.
		if (res.cmh_expiry == true) {
			_DEBUG_PRINT("BSM tx callback - Certificate will be expired. Request to change BSM ID\n");
			J29451_RequestBSMIDChange();
		}
	}
  
  /*
   * Power off가 된 상태이면,
   * BSM 전송을 중지하고 Path 정보를 백업한다.
   */
  if (G_power_off == true) {
    _DEBUG_PRINT("Power off detected - stop BSM transmit and backup path info\n");
    J29451_StopBSMTransmit();
    J29451_SavePathInfoBackupFile("./bsm_path.history");
		system("sync"); // 낸드 플래시 싱크 명령 강제 입력 (=전원이 꺼지기 전에 낸드플래시에 즉각 저장되도록 한다)
  }
	
}


/** 
 * @brief BSM을 생성한다.([j2736 Frame[BSM Data]])
 * @param[out] bsm_size BSM 크기
 * @retval BSM 인코딩정보 포인터
*/
EXTERN_API uint8_t *RELAY_INNO_J2735_Construct_BSM(size_t *bsm_size)
{
	uint8_t *buf = NULL;
  struct j2735MessageFrame *frame = NULL;
  struct j2735BasicSafetyMessage *bsm = NULL;

	if (((frame = (struct j2735MessageFrame *)asn1_mallocz_value(asn1_type_j2735MessageFrame)) == NULL) ||
      ((bsm = (struct j2735BasicSafetyMessage *)asn1_mallocz_value(asn1_type_j2735BasicSafetyMessage)) == NULL)) {
    _DEBUG_PRINT("Fail to encode BSM - asn1_mallocz_value() failed\n");
    goto out;
  }
	
	if(RELAY_INNO_J2735_Fill_BSM(bsm) < 0)
	{
		return NULL;
	}

  frame->messageId = 20; // BasicSafetyMessage (per SAE j2735)
  frame->value.type = (ASN1CType *)asn1_type_j2735BasicSafetyMessage;
  frame->value.u.data = bsm;

  // BSM을 인코딩한다.
  *bsm_size = (size_t)asn1_uper_encode(&buf, asn1_type_j2735MessageFrame, frame);
  if (buf == NULL) {
    _DEBUG_PRINT("Fail to encode BSM - asn1_uper_encode() failed\n");
    goto out;
  }else{
		if(1)
		{
			g_pathhistorypointlistlist.count = RELAY_INNO_J2735_BSM_Push_Pathhistroty();	
		}
		
		g_msg_bsm_tx_cnt = (g_msg_bsm_tx_cnt + 1) % 128;
	}
out:
  if(frame) 
	{ 
		asn1_free_value(asn1_type_j2735MessageFrame, frame); 
	}
	
  return buf;
}
/**
 * @brief BSM 인코딩정보에 값을 채운다.
 * @param[out] bsm 정보를 채울 BSM 인코딩정보 구조체 포인터
 * @retval 0: 성공
 * @retval -1: 실패
 */
EXTERN_API int RELAY_INNO_J2735_Fill_BSM(struct j2735BasicSafetyMessage *bsm)
{
	int ret;

	if(g_bsm_core_data_installed == false)
	{
		struct j2735BSMcoreData *core = NULL;
		ret = RELAY_INNO_J2735_BSM_Gnss_Info_Ptr_Instrall(&core);
		if(ret < 0)
		{
			_DEBUG_PRINT("Fail to install BSM core data\n");
			return ret;
		}else{
			_DEBUG_PRINT("BSM core data installed\n");
			return -1;
		}
	}

	ret = RELAY_INNO_J2735_BSM_Fill_CoreData(&bsm->coreData);
	if(ret < 0)
	{
		_DEBUG_PRINT("Fail to fill BSM core data\n");
		return ret;
	}
	bsm->partII_option = false;
	if(bsm->partII_option == true)
	{
		bsm->partII.count = 3;
		bsm->partII.tab = asn1_mallocz(asn1_get_size(asn1_type_j2735PartIIcontent_1) * bsm->partII.count);
		for(size_t count_num = 0; count_num < bsm->partII.count; count_num++)
		{
			struct j2735PartIIcontent_1 *tab_now = bsm->partII.tab + count_num;
			if(tab_now != NULL)
			{
				tab_now->partII_Id = count_num;
				ret = RELAY_INNO_J2735_BSM_Fill_PartII(tab_now);
			}
		}
	}

	bsm->regional_option = false;
	if(bsm->regional_option == true)
	{
		bsm->regional.count = 1;
		bsm->regional.tab = asn1_mallocz(asn1_get_size(asn1_type_j2735RegionalExtension_1) * bsm->regional.count);
		for(size_t count_num = 0; count_num < bsm->regional.count; count_num++)
		{
  		j2735RegionalExtension_1 *tab_now = bsm->regional.tab + count_num;
			if(tab_now != NULL)
			{
				tab_now->regionId = 4;
				tab_now->regExtValue.type = NULL;
				tab_now->regExtValue.u.octet_string.len = 6;
				tab_now->regExtValue.u.octet_string.buf = asn1_mallocz(tab_now->regExtValue.u.octet_string.len);
				if(tab_now->regExtValue.u.octet_string.buf == NULL)
				{
					return -1;
				}
				memcpy(tab_now->regExtValue.u.octet_string.buf, (uint8_t []){0x1C, 0x00, 0x00, 0x00, 0x00, 0x00}, tab_now->regExtValue.u.octet_string.len);	
			}

			
		}
	}
	return 0;
}

/**
 * @brief BSM Core 데이터 중 Gnss관련 정보를 포인터로 연결한다.
 * @param[in] core BSM Core 데이터 주소 포인터
 * @retval void
 * @note BSM Core 데이터의 포인터를 전달하지 않으면 내부적으로 생성된 구조체를 사용한다.
 */
EXTERN_API int RELAY_INNO_J2735_BSM_Gnss_Info_Ptr_Instrall(struct j2735BSMcoreData **core_ptr)
{
	struct j2735BSMcoreData *core;
	if(*core_ptr == NULL)
	{
		g_core = malloc(sizeof(struct j2735BSMcoreData));
		memset(g_core, 0, sizeof(struct j2735BSMcoreData));
	}else{
		
		g_core = *core_ptr;
	}
	core = g_core;	
	G_gnss_bsm_data = malloc(sizeof(struct relay_inno_gnss_data_bsm_t));
	memset(G_gnss_bsm_data, 0, sizeof(struct relay_inno_gnss_data_bsm_t));
	if(G_gnss_bsm_data == NULL)
	{
		return -1;
	}

	G_gnss_bsm_data->lat = (int*)&core->lat;
	G_gnss_bsm_data->lon = (int*)&core->Long;
	G_gnss_bsm_data->elev = (int*)&core->elev;
	G_gnss_bsm_data->speed = (uint32_t*)&core->speed;
	G_gnss_bsm_data->heading = (uint32_t*)&core->heading;
	G_gnss_bsm_data->acceleration_set.lat = (int*)&core->accelSet.lat;
	G_gnss_bsm_data->acceleration_set.lon = (int*)&core->accelSet.Long;
	G_gnss_bsm_data->acceleration_set.vert = (int*)&core->accelSet.vert;
	G_gnss_bsm_data->acceleration_set.yaw = (int*)&core->accelSet.yaw;
	G_gnss_bsm_data->pos_accuracy.semi_major = (unsigned int*)&core->accuracy.semiMajor;
	G_gnss_bsm_data->pos_accuracy.semi_minor = (unsigned int*)&core->accuracy.semiMinor;
	G_gnss_bsm_data->pos_accuracy.orientation = (unsigned int*)&core->accuracy.orientation;

	G_gnss_bsm_data->isused = true;
	g_bsm_core_data_installed = true;
	return 0;
}

/**
 * @brief BSMCoreData 인코딩정보에 값을 채운다.
 * @param[out] core 값을 채울 BSMCoreData 인코딩정보 구조체 포인터
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int RELAY_INNO_J2735_BSM_Fill_CoreData(struct j2735BSMcoreData *core_ptr)
{
  int ret = -1;
	struct j2735BSMcoreData *core = core_ptr;
	if(G_gnss_bsm_data->isused == true)
	{
		if(g_core != NULL)
		{
			asn1_copy_value(asn1_type_j2735BSMcoreData, core, g_core);
		}
	}
  if(G_gnss_data->status.unavailable == FALSE || G_relay_inno_config.v2x.j2735.bsm.tx_forced == true) 	
  {
    core->msgCnt = RELAY_INNO_INCREASE_BSM_MSG_CNT(g_msg_bsm_tx_cnt);
    core->id.len = RELAY_INNO_TEMPORARY_ID_LEN;
		if(core->id.buf == NULL){
    	core->id.buf = asn1_mallocz(core->id.len);
		}else{
			memset(core->id.buf, 0, core->id.len);
		}
    if (core->id.buf) {
      memcpy(core->id.buf, g_temporary_id, RELAY_INNO_TEMPORARY_ID_LEN);
		}
		core->secMark = RELAY_INNO_J2735_BSM_SecMark();
		g_core->secMark = core->secMark;
		core->transmission = g_transmission;
		core->angle = g_angle;
		core->brakes.traction = g_traction;
		core->brakes.albs = g_abs;
		core->brakes.scs = g_scs;
		core->brakes.brakeBoost = g_brake_boost;
		core->brakes.auxBrakes = g_aux_brakes;
		if(core->brakes.wheelBrakes.buf == NULL)
		{
			core->brakes.wheelBrakes.buf = asn1_mallocz(1);
		}else{
			memset(core->brakes.wheelBrakes.buf, 0, 1);
		}
		if (core->brakes.wheelBrakes.buf) 
		{
			*(core->brakes.wheelBrakes.buf) = (g_wheel_brakes_unavailable << 7) |
																				(g_wheel_brakes_left_front << 6) |
																				(g_wheel_brakes_left_rear << 5) |
																				(g_wheel_brakes_right_front << 4) |
																				(g_wheel_brakes_right_rear << 3);
			core->brakes.wheelBrakes.len = 5;
		}
		core->size.length = g_vehicle_length;
		core->size.width = g_vehicle_width;
		ret = 0;
  }

  return ret;
}



/**
 * @brief BSM Part II를 채운다.
 * @param[out] partII_ptr 값을 채울 BSM Part II 인코딩정보 구조체 포인터
 * @retval 0: 성공
 */
static int RELAY_INNO_J2735_BSM_Fill_PartII(struct j2735PartIIcontent_1 *partII_ptr)
{
	switch(partII_ptr->partII_Id)
	{
		case 0: // VehicleSafetyExtensions
		{
			partII_ptr->partII_Value.type = (ASN1CType *)asn1_type_j2735VehicleSafetyExtensions;
			partII_ptr->partII_Value.u.data = asn1_mallocz_value(asn1_type_j2735VehicleSafetyExtensions);
			struct j2735VehicleSafetyExtensions *data_ptr = partII_ptr->partII_Value.u.data;
			if(g_pathhistorypointlistlist.count > 0)
			{
				data_ptr->pathHistory_option = true;
				if(data_ptr->pathHistory_option == true)
				{
					memset(&data_ptr->pathHistory, 0x00, sizeof(struct j2735PathHistory));
					data_ptr->pathHistory.crumbData.count = g_pathhistorypointlistlist.count;
					data_ptr->pathHistory.crumbData.tab = asn1_mallocz(asn1_get_size(asn1_type_j2735PathHistoryPoint) * data_ptr->pathHistory.crumbData.count);
					for(size_t count_num = 0; count_num < g_pathhistorypointlistlist.count; count_num++)
					{
						j2735PathHistoryPoint *pathhistorypoint = &g_pathhistorypointlistlist.tab[count_num];
						j2735PathHistoryPoint *pathhistorypoint_ptr = data_ptr->pathHistory.crumbData.tab + count_num;
						asn1_copy_value(asn1_type_j2735PathHistoryPoint, pathhistorypoint_ptr, pathhistorypoint);
						pathhistorypoint_ptr->latOffset = g_core->lat - pathhistorypoint->latOffset;
						pathhistorypoint_ptr->lonOffset = g_core->Long - pathhistorypoint->lonOffset;
						pathhistorypoint_ptr->elevationOffset = g_core->elev - pathhistorypoint->elevationOffset;
						pathhistorypoint_ptr->timeOffset = g_core->secMark - pathhistorypoint->timeOffset;
						if(pathhistorypoint_ptr->timeOffset < 0)
						{
							pathhistorypoint_ptr->timeOffset += 65535;
						}
					}
					
				}
			}
			if(data_ptr->pathPrediction_option == true)
			{
				data_ptr->pathPrediction.radiusOfCurve = 32767; // straight path
				data_ptr->pathPrediction.confidence = 4095; // unavailable
			}
			break;
		}
		case 1: // SpecialVehicleExtensions
		{
			partII_ptr->partII_Value.type = (ASN1CType *)asn1_type_j2735SpecialVehicleExtensions;
			partII_ptr->partII_Value.u.data = asn1_mallocz_value(asn1_type_j2735SpecialVehicleExtensions);
			struct j2735SpecialVehicleExtensions *data_ptr = partII_ptr->partII_Value.u.data;
			data_ptr->description_option = true; // unavailable
			memset(&data_ptr->description, 0x00, sizeof(struct j2735EventDescription));
			data_ptr->description.typeEvent = 0; // ITIScodes
			data_ptr->trailers_option = false;
			break;
		}
		case 2: // SupplementalVehicleExtensions
		{
			partII_ptr->partII_Value.type = (ASN1CType *)asn1_type_j2735SupplementalVehicleExtensions;
			partII_ptr->partII_Value.u.data = asn1_mallocz_value(asn1_type_j2735SupplementalVehicleExtensions);
			struct j2735SupplementalVehicleExtensions *data_ptr = partII_ptr->partII_Value.u.data;
			data_ptr->classDetails_option = true; // unavailable
			memset(&data_ptr->classDetails, 0x00, sizeof(struct j2735VehicleClassification));
			data_ptr->classDetails.role_option = true;
			data_ptr->classDetails.role = j2735BasicVehicleRole_basicVehicle;
			data_ptr->classDetails.vehicleType_option = true; 
			data_ptr->classDetails.vehicleType = j2735VehicleGroupAffected_cars;
			break;
		}
	}
	return 0;
}

/**
 * @brief PathHistoryPoint를 이동한다.
 * @retval PathHistoryPoint 개수(디버깅용)
 */
static size_t RELAY_INNO_J2735_BSM_Move_Pathhistroty()
{
	if(g_pathhistorypointlistlist.count == RELAY_INNO_MAX_PATHHISTORYPOINT)
	{
		// PathHistoryPoint의 개수가 최대 개수에 도달했을 경우, 가장 오래된 PathHistoryPoint를 삭제한다.
		g_pathhistorypointlistlist.count--;
		memset(&g_pathhistorypointlistlist.tab[g_pathhistorypointlistlist.count], 0x00, sizeof(j2735PathHistoryPoint));
		// PathHistoryPoint 개수가 0이면 PathHistoryPoint를 이동할 필요가 없으므로 return 한다.
		if(g_pathhistorypointlistlist.count == 0)
		{
			return 0;
		}
	}
	// PathHistoryPoint를 이동한다.
	for(size_t count_num = g_pathhistorypointlistlist.count; 0 < count_num; count_num--)
	{
		memcpy(&g_pathhistorypointlistlist.tab[count_num], &g_pathhistorypointlistlist.tab[count_num - 1] , sizeof(j2735PathHistoryPoint));
	}
	memset(&g_pathhistorypointlistlist.tab[0], 0x00, sizeof(j2735PathHistoryPoint));
	return g_pathhistorypointlistlist.count;
}

/**
 * @brief PathHistoryPoint를 추가한다.
 * @retval PathHistoryPoint 개수
 */
static size_t RELAY_INNO_J2735_BSM_Push_Pathhistroty()
{
	RELAY_INNO_J2735_BSM_Move_Pathhistroty();
	j2735PathHistoryPoint *pathhistorypoint = &g_pathhistorypointlistlist.tab[0];
	pathhistorypoint->latOffset = g_core->lat; // unavailable
	pathhistorypoint->lonOffset = g_core->Long; // unavailable
	pathhistorypoint->elevationOffset = g_core->elev; // unavailable
	pathhistorypoint->timeOffset = g_core->secMark;

	ASN1Error *error = NULL;
	pathhistorypoint->speed_option = asn1_check_constraints(asn1_type_j2735Speed, G_gnss_bsm_data->speed, error);
	if(pathhistorypoint->speed_option)
	{
		pathhistorypoint->speed = (j2735Speed)*G_gnss_bsm_data->speed; // Units of 0.02 m/s
	}else{
		pathhistorypoint->speed = 0;
	}
	pathhistorypoint->heading_option = asn1_check_constraints(asn1_type_j2735Heading, G_gnss_bsm_data->heading, error);
	if(pathhistorypoint->heading_option)
	{
		pathhistorypoint->heading = (j2735Speed)*G_gnss_bsm_data->heading; // Units of 0.02 m/s
	}else{
		pathhistorypoint->heading = 0;
	}
	g_pathhistorypointlistlist.count++;
	return g_pathhistorypointlistlist.count;
}


/**
 * @brief BSM 메시지 카운트를 증가시킨다.
 * @param[in] msg_cnt BSM 메시지 카운트
 * @retval BSM 메시지 카운트
 */
static int RELAY_INNO_J2735_BSM_SecMark()
{
	struct timespec tv;
	clock_gettime(CLOCK_REALTIME, &tv); 
	struct tm *tm;
	tm = localtime(&tv.tv_sec);
	int ret = 0;
	ret += tm->tm_sec * 1000;
	ret += (int)((tv.tv_nsec)/1000000);
	
	return ret % 65535;
}

