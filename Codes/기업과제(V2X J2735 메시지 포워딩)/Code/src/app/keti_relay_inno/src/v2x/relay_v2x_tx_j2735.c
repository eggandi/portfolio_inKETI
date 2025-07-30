/**
 * @file
 * @brief
 * @date 2025-04-09
 * @author dong
 */
#include "relay_v2x_tx_j2735.h"
#include "relay_config.h"
/**
 * @brief BSM을 전송한다.
 * @retval 0: 성공
 * @retval -1: 실패
 */
EXTERN_API int RELAY_INNO_V2X_Tx_J2735_BSM(dot3ShortMsgNpdu *wsm_in)
{
	uint8_t *spdu_payload_bsm = NULL;
	size_t spdu_payload_bsm_size = 0;
	if(wsm_in == NULL)
	{
		//_DEBUG_PRINT("Check to transmit BSM - wsm_in is NULL. And Will make wsm.\n");
	}
	spdu_payload_bsm = RELAY_INNO_J2735_Construct_BSM(&spdu_payload_bsm_size);
	if(spdu_payload_bsm == NULL)
	{
		_DEBUG_PRINT("Fail to construct BSM - RELAY_INNO_J2735_Construct_BSM() failed\n");
		return -1;
	}
	/*
   * Signed SPDU를 생성한다.
	 */
	struct Dot2SPDUConstructParams params;
	struct Dot2SPDUConstructResult res;
	memset(&params, 0, sizeof(params));
if(G_relay_inno_config.v2x.dot2.enable == true)
{
	params.type = kDot2SPDUConstructType_Unsecured;
	params.signed_data.psid = G_relay_inno_config.v2x.j2735.bsm.psid;
	params.signed_data.signer_id_type = kDot2SignerId_Profile;
	params.signed_data.cmh_change = false;
re_ContstructSPDU:
	res = Dot2_ConstructSPDU(&params, spdu_payload_bsm, spdu_payload_bsm_size);
	if (res.ret < 0) {
		_DEBUG_PRINT("BSM tx callback - Dot2_ConstructSPDU() failed: %d\n", res.ret);
		return -2;
	}
	if(res.cmh_expiry == true)
	{
		goto re_ContstructSPDU;
		params.signed_data.signer_id_type = kDot2SignerId_Certificate;
		params.signed_data.cmh_change = true;
	}
}else{
	
	params.type = kDot2SPDUConstructType_Signed;
	params.signed_data.psid = G_relay_inno_config.v2x.j2735.bsm.psid;
	params.signed_data.signer_id_type = kDot2SignerId_Self;
	params.signed_data.cmh_change = false;
	
	res.ret = spdu_payload_bsm_size;
	res.spdu = malloc(res.ret * sizeof(char));
	memcpy(res.spdu, spdu_payload_bsm, res.ret);
}

#ifdef _USED_DOT3_LIB
#else
	struct LTEV2XHALMSDUTxParams tx_params;
	memset(&tx_params, 0x00, sizeof(struct LTEV2XHALMSDUTxParams));
	tx_params.tx_flow_type = G_relay_inno_config.v2x.j2735.bsm.tx_type;	
	tx_params.tx_flow_index = 0;
	tx_params.priority = G_relay_inno_config.v2x.j2735.bsm.priority;
	if(G_relay_inno_config.v2x.j2735.bsm.tx_power == 0)
	{
		tx_params.tx_power = G_relay_inno_config.v2x.tx_power;
	}else{
		tx_params.tx_power = G_relay_inno_config.v2x.j2735.bsm.tx_power;
	}
	tx_params.dst_l2_id = G_relay_inno_config.v2x.j2735.bsm.psid;
	const dot3ShortMsgData wsm_body = {
		.buf = res.spdu,
		.len = res.ret,
	}; 
	int ret = RELAY_INNO_V2X_MSDU_Transmit(&wsm_body, NULL, &tx_params);
	if (ret < 0) {
		_DEBUG_PRINT("Fail to transmit BSM - RELAY_INNO_V2X_MSDU_Transmit() failed: %d\n", ret);
		return -3;
	}
	switch(G_relay_inno_config.relay.relay_data_type)
		{
			case RELAY_DATA_TYPE_V2X_SSDU:
			{
				ret = sendto(G_relay_v2x_tx_socket, spdu_payload_bsm, spdu_payload_bsm_size, 0, (struct sockaddr *)&G_relay_v2x_tx_addr, sizeof(G_relay_v2x_tx_addr));
				if(ret < 0)
				{
					_DEBUG_PRINT("Fail to sendto - %d sendto() failed: %d\n",G_relay_inno_config.relay.relay_data_type, ret);
				}else{
					_DEBUG_PRINT("Success to sendto - %d sendto() success: %d\n",G_relay_inno_config.relay.relay_data_type, ret);
				}
				break;
			}	
			default:
				break;
		}

	free(spdu_payload_bsm);
#endif
	return 0;
}

