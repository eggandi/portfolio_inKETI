#include "v2x_handover_main.h"

#define V2XHO_ERROR_CODE_FILE_CODE V2XHO_ERROR_CODE_FILE_CODE_WAVE_RX
#define MAC_PRINT(x) x[0], x[1], x[2], x[3], x[4], x[5]

static int f_i_V2XHO_Rx_Dot2_Psid_Filter(Dot3PSID psid); 

extern void F_v_V2XHO_WAVE_Rx_MPDU_Callback(const uint8_t *mpdu, WalMPDUSize mpdu_size, const struct WalMPDURxParams *rx_params)
{
    int ret;
    struct V2XPacketParseData *v2x_parsed = V2X_AllocateDSRCPacketParseData(mpdu, mpdu_size, rx_params);
    if (v2x_parsed == NULL) {
        //lreturn(V2XHO_WAVE_RX_DSRC_PACKET_PARSING_NULL);r
        return;
    }
    v2x_parsed->wsdu = Dot3_ParseWSMMPDU(    (v2x_parsed->pkt), v2x_parsed->pkt_size,
                                            &(v2x_parsed->mac_wsm),
                                            &(v2x_parsed->wsdu_size),
                                            &(v2x_parsed->interested_psid),
                                            &(ret));
    if(ret < 0)
    {
        char msg[128] = {"\0",};
        sprintf(msg, "Fail to dot3 Parsing MPDU. Error Code:%d", ret);
        perror(msg);
        return;
    }
    if(v2x_parsed->wsdu_size <= 0) 
    {
        char msg[128] = {"\0",};
        sprintf(msg, "No WSDU Data in MPDU. WSDU size:%d", v2x_parsed->wsdu_size);
        perror(msg);
        V2X_FreePacketParseData(v2x_parsed);
        return;
    }

    struct Dot2SPDUProcessParams params = {.rx_time = 0, .rx_psid = ret};
    ret = Dot2_ProcessSPDU(v2x_parsed->wsdu, v2x_parsed->wsdu_size, &params, v2x_parsed);
    if(ret < 0)
    {
        V2X_FreePacketParseData(v2x_parsed);
    }
   

    return;
}

extern void F_v_V2XHO_WAVE_Rx_DOT2_SPDU_Callback(Dot2ResultCode dot2_result, void *priv)
{
    struct V2XPacketParseData *v2x_parsed = (struct V2XPacketParseData *)priv;
    
    switch(dot2_result)
    {
        default:
        {
            char msg[128] = {"\0",};
            sprintf(msg, "Fail to process dot2 msg. Error Code:%d", dot2_result);
            perror(msg);
            V2X_FreePacketParseData(v2x_parsed);
            return;
        }
        case kDot2Result_Success:
        {
            break;
        }
    }
    
    struct Dot2SPDUParseData *dot2_parsed = &(v2x_parsed->spdu);
    struct Dot3ParseWSAParams wsa;
    v2xho_wsmp_t wsmp_data;
    int ret;

    wsmp_data.dot2_type = dot2_parsed->content_type;
    ret = f_i_V2XHO_Rx_Dot2_Psid_Filter(v2x_parsed->mac_wsm.wsm.psid);
    if(ret < 0)
    {
        V2X_FreePacketParseData(v2x_parsed);
        return;
    }
    memcpy(wsmp_data.src_mac, v2x_parsed->mac_wsm.mac.src_mac_addr, MAC_ALEN);
    wsmp_data.rx_power = v2x_parsed->rx_params.rx_power;
    Dot3_ParseWSA(v2x_parsed->ssdu, v2x_parsed->ssdu_size, &wsa);
    wsmp_data.wsa_id = wsa.hdr.wsa_id;
    if(wsa.present.wra)
    {   
        memcpy(&wsmp_data.wra, &wsa.wra, sizeof(struct Dot3WRA));
    }else{
        printf("No WRA Data.");
        printf("Src_MAC-%02X:%02X:%02X:%02X:%02X:%02X\n", MAC_PRINT(wsmp_data.src_mac));
        return;
    }

    switch(dot2_parsed->content_type)
    {
        default:
        {
            break;
        }
        case kDot2Content_UnsecuredData:
        {
            if(wsa.hdr.extensions.twod_location)
            {
                wsmp_data.u.wsa_location.latitude = wsa.hdr.twod_location.latitude;
                wsmp_data.u.wsa_location.longitude = wsa.hdr.twod_location.longitude;
                wsmp_data.u.wsa_location.elevation = 0;
            }else if(wsa.hdr.extensions.threed_location)
            {
                wsmp_data.u.wsa_location.latitude = wsa.hdr.threed_location.latitude;
                wsmp_data.u.wsa_location.longitude = wsa.hdr.threed_location.longitude;
                wsmp_data.u.wsa_location.elevation = wsa.hdr.threed_location.elevation;
            }
            break;
        }
        case kDot2Content_SignedData:
        {
            if(dot2_parsed->signed_data.gen_location_present)
            {
                wsmp_data.u.dot2_location.lat = dot2_parsed->signed_data.gen_location.lat;
                wsmp_data.u.dot2_location.lon = dot2_parsed->signed_data.gen_location.lon;
                wsmp_data.u.dot2_location.elev = dot2_parsed->signed_data.gen_location.elev;
            }
            break;
        }
    }
    switch(G_v2xho_config.equipment_type)
    {
        default:
        {
            break;
        }
        case DSRC_OBU:
        case DSRC_OBU_FA:
        {
            F_i_V2XHO_OBU_UDS_Send_WSMP_Data(&wsmp_data);
            break;
        }
    }
    
}

static int f_i_V2XHO_Rx_Dot2_Psid_Filter(Dot3PSID psid)
{
    int ret;
	switch(psid)
	{
		case 135:{      ret = 135;    goto PSID_Accept;	break;}//WSA
		case 82056:{	ret = 82056;  goto PSID_Refuse;	break;}//MAP
		case 82055:{	ret = 82055;  goto PSID_Refuse;	break;}//SPAT
		case 82051:{	ret = 82051;  goto PSID_Refuse;	break;}//PVD
		case 82053:{	ret = 82053;  goto PSID_Refuse;	break;}//RSA
		case 82057:{	ret = 82057;  goto PSID_Refuse;	break;}//RTCM
		case 82054:{	ret = 82054;  goto PSID_Refuse;	break;}//TIM
		default :
		{
PSID_Accept:
			return ret;
		}
	}		

PSID_Refuse:
	return -1;

}
