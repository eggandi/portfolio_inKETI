/**
 * @file
 * @brief
 * @date 2025-04-09
 * @author dong
 */
#include "relay_config.h"
#include "relay_v2x_rx.h"
#include "relay_gnss.h"
#include "v2x-sw.h"

#define WSM_EXT_CNT 3
#define WSM_EXT_ID_TX_POWER 4
#define WSM_EXT_ID_CHANNEL 15
#define WSM_EXT_ID_DATERATE 16


/**
 * @brief Dot2_ProccessSPDU() 호출 결과를 전달 받는 콜백함수. dot2 라이브러리에서 호출된다.
 * @param[in] result 처리결과
 * @param[in] priv 패킷파싱데이터
 */
void RELAY_INNO_ProcessSPDUCallback(Dot2ResultCode result, void *priv)
{

	struct V2XPacketParseData *parsed = (struct V2XPacketParseData *)priv;
  struct Dot2SPDUParseData *dot2_parsed = &(parsed->spdu);
	  /*
   * 서명 검증 실패
   */
  if (result != kDot2Result_Success) 
	{
    _DEBUG_PRINT("Fail to process SPDU. result is %d\n", result);
    goto out;
	}
  if (dot2_parsed->content_type == kDot2Content_UnsecuredData) 
	{
    _DEBUG_PRINT("Success to process UNSECURED SPDU. Payload size is %lu\n", parsed->ssdu_size);
  }else if (dot2_parsed->content_type == kDot2Content_SignedData) 
	{
		_DEBUG_PRINT("Success to process/verify SIGNED SPDU. Payload size is %lu\n",
										parsed->ssdu_size);
		_DEBUG_PRINT("    content_type: %u, signer_id_type: %u, PSID: %u\n",
										dot2_parsed->content_type, dot2_parsed->signed_data.signer_id_type, dot2_parsed->signed_data.psid);
		if (dot2_parsed->signed_data.gen_time_present == true) {
			_DEBUG_PRINT("    gen_time: %"PRIu64"\n", dot2_parsed->signed_data.gen_time);
		}
		if (dot2_parsed->signed_data.expiry_time_present == true) {
			_DEBUG_PRINT("    exp_time: %"PRIu64"\n", dot2_parsed->signed_data.expiry_time);
		}
		if (dot2_parsed->signed_data.gen_location_present == true) {
			_DEBUG_PRINT("    gen_lat: %d, gen_lon: %d, gen_elev: %u\n",
											dot2_parsed->signed_data.gen_location.lat, dot2_parsed->signed_data.gen_location.lon,
											dot2_parsed->signed_data.gen_location.elev);
		}
  } else {
		_DEBUG_PRINT("Success to process SPDU(content_type: %u). Payload size is %lu\n",
								dot2_parsed->content_type, parsed->ssdu_size);
  }
	
	int ret = 0;
	switch(G_relay_inno_config.relay.relay_data_type)
	{
		case RELAY_DATA_TYPE_V2X_MPDU:
		{
			ret = sendto(G_relay_v2x_rx_socket, parsed->pkt, parsed->pkt_size, 0, (struct sockaddr *)&G_relay_v2x_rx_addr, sizeof(G_relay_v2x_rx_addr));

			break;
		}
		case RELAY_DATA_TYPE_V2X_WSM:
		{
			ret = sendto(G_relay_v2x_rx_socket, parsed->wsm, parsed->wsm_size, 0, (struct sockaddr *)&G_relay_v2x_rx_addr, sizeof(G_relay_v2x_rx_addr));
			break;
		}	
		case RELAY_DATA_TYPE_V2X_WSDU:
		{
			ret = sendto(G_relay_v2x_rx_socket, parsed->wsdu, parsed->wsdu_size, 0, (struct sockaddr *)&G_relay_v2x_rx_addr, sizeof(G_relay_v2x_rx_addr));
			break;
		}	
		case RELAY_DATA_TYPE_V2X_SSDU:
		{
			ret = sendto(G_relay_v2x_rx_socket, parsed->ssdu, parsed->ssdu_size, 0, (struct sockaddr *)&G_relay_v2x_rx_addr, sizeof(G_relay_v2x_rx_addr));
			break;
		}	
		default:
			break;
	}
	if(ret < 0)
	{
		_DEBUG_PRINT("Fail to sendto - %d sendto() failed: %d\n",G_relay_inno_config.relay.relay_data_type, ret);
	}else{
		_DEBUG_PRINT("Success to sendto - %d sendto() success: %d\n",G_relay_inno_config.relay.relay_data_type, ret);
	}

out:
  V2X_FreePacketParseData(parsed);
}


/**
 * @brief LTE-V2X MSDU 수신처리 콜백함수. lteaccess 라이브러리에서 호출된다.
 * @param[in] msdu 수신된 MSDU (= WSM 헤더 + WSM body)
 * @param[in] msdu_size 수신된 MSDU의 크기
 */
extern void RELAY_INNO_V2X_RxMSDUCallback(const uint8_t *msdu, LTEV2XHALMSDUSize msdu_size, struct LTEV2XHALMSDURxParams rx_params)
{
	rx_params = rx_params;
  if(msdu_size > 0)
  {
		_DEBUG_PRINT("Process rx MSDU - msdu_size: %u\n", msdu_size);
		struct V2XPacketParseData *parsed = V2X_AllocateCV2XPacketParseData(msdu, msdu_size);
		int ret;
		parsed->wsdu = Dot3_ParseWSM(parsed->pkt,
																 parsed->pkt_size,
																 &(parsed->mac_wsm.wsm),
																 &(parsed->wsdu_size),
																 &(parsed->interested_psid),
																 &ret);
		if (parsed->wsdu == NULL) 
		{
			_DEBUG_PRINT("Fail to process rx LTE-V2X MSDU - Dot3_ParseWSM() failed: %d\n", ret);
			V2X_FreePacketParseData(parsed);
			return;
		}
		
		if(G_relay_inno_config.v2x.dot2.enable == true)
		{
			/*
			* SPDU를 처리한다 - 결과는 콜백함수를 통해 전달된다.
			*/
			struct Dot2SPDUProcessParams params;
			memset(&params, 0, sizeof(params));
			params.rx_time = 0;
			params.rx_psid = parsed->mac_wsm.wsm.psid;
			params.rx_pos.lat = G_gnss_data->lat;
			params.rx_pos.lon = G_gnss_data->lon;
			ret = Dot2_ProcessSPDU(parsed->wsdu, parsed->wsdu_size, &params, parsed);
			if (ret < 0) {
				_DEBUG_PRINT("Fail to process rx LTE-V2X MSDU - Dot2_ProcessSPDU() failed: %d\n", ret);
				switch(parsed->mac_wsm.wsm.psid)
				{
					default:break;
					case 32: 
					case 135: 
					case 82051:
					case 82053:
					case 82054:
					case 82055:
					case 82056:
					case 82057:
					{
						switch(G_relay_inno_config.relay.relay_data_type)
						{
							case RELAY_DATA_TYPE_V2X_SSDU:
							{
								ret = sendto(G_relay_v2x_rx_socket, parsed->wsdu, parsed->wsdu_size, 0, (struct sockaddr *)&G_relay_v2x_rx_addr, sizeof(G_relay_v2x_rx_addr));
								break;
							}	
							default:
								break;
						}
						if(ret < 0)
						{
							_DEBUG_PRINT("Fail to sendto - %d sendto() failed: %d\n",G_relay_inno_config.relay.relay_data_type, ret);
						}else{
							_DEBUG_PRINT("Success to sendto - %d sendto() success: %d\n",G_relay_inno_config.relay.relay_data_type, ret);
						}
					}
				}
				V2X_FreePacketParseData(parsed);
				return;
			}
		}else{
			_DEBUG_PRINT("Success to process rx LTE-V2X MSDU - Dot2_ProcessSPDU() Not Used.\n");
			switch(parsed->mac_wsm.wsm.psid)
			{
				default:break;
				case 32: 
				case 135: 
				case 82051:
				case 82053:
				case 82054:
				case 82055:
				case 82056:
				case 82057:
				{
					switch(G_relay_inno_config.relay.relay_data_type)
					{
						case RELAY_DATA_TYPE_V2X_SSDU:
						{
							ret = sendto(G_relay_v2x_rx_socket, parsed->wsdu, parsed->wsdu_size, 0, (struct sockaddr *)&G_relay_v2x_rx_addr, sizeof(G_relay_v2x_rx_addr));
							break;
						}	
						default:
							break;
					}
					if(ret < 0)
					{
						_DEBUG_PRINT("Fail to sendto - %d sendto() failed: %d\n",G_relay_inno_config.relay.relay_data_type, ret);
					}else{
						_DEBUG_PRINT("Success to sendto - %d sendto() success: %d\n",G_relay_inno_config.relay.relay_data_type, ret);
					}
				}
			}
		}
	}
  return;
}
