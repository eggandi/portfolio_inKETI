#include "relay_v2x.h"
#include "relay_config.h"

/**
 * @brief V2X 라이브러리들을 초기화한다.
 * @retval 0: 성공
 * @retval -1: 실패
 */
EXTERN_API int RELAY_INNO_V2X_Init(void)
{
  LTEV2XHALLogLevel hal_log_level = (LTEV2XHALLogLevel ) G_relay_inno_config.v2x.lib_dbg;

  // LTEV2X 접속계층 라이브러리 초기화하고 패킷수신콜백함수를 등록한다.
  int ret = LTEV2XHAL_Init(hal_log_level, G_relay_inno_config.v2x.dev_name);
  if (ret < 0) {
    _DEBUG_PRINT("Fail to initialize ltev2x-hal library - LTEV2XHAL_Init() failed: %d\n", ret);
    return -1;
  }
	if(G_relay_inno_config.v2x.dot2.enable == true)
	{
		ret = Dot2_Init(hal_log_level, 100, NULL, 5);
		if (ret < 0) {
			_DEBUG_PRINT("Fail to initialize dot2 library - Dot2_Init() failed: %d\n", ret);
			return -2;
		}else{
			_DEBUG_PRINT("Success to initialize dot2 library\n");
		}
	}
	ret = Dot3_Init(hal_log_level);
  if (ret < 0) {
    _DEBUG_PRINT("Fail to initialize dot3 library - Dot3_Init() failed: %d\n", ret);
    return -3;
  }else{
		_DEBUG_PRINT("Success to initialize dot3 library\n");
	}
  _DEBUG_PRINT("Success to initialize V2X library\n");

  return 0;
}

extern bool RELAY_INNO_V2X_Psid_Filter(unsigned int psid)
{
	switch(psid)
	{
		case 135:
			if(G_relay_inno_config.v2x.rx.wsa_enable == false)
			{
				goto psid_false;
			}
			break;
    case 32:
			if(G_relay_inno_config.v2x.rx.j2735.BSM_enable == false)
			{	
				goto psid_false;	
			}
			break;//BSM
		case 82056:
			if(G_relay_inno_config.v2x.rx.j2735.MAP_enable == false)
			{	
				goto psid_false;	
			}
			break;//MAP
		case 82055:
			if(G_relay_inno_config.v2x.rx.j2735.SPAT_enable == false)
			{	
				goto psid_false;	
			}
			break;//SPAT
		case 82051:
			if(G_relay_inno_config.v2x.rx.j2735.PVD_enable == false)
			{	
				goto psid_false;	
			}
			break;//PVD
		case 82053:
			if(G_relay_inno_config.v2x.rx.j2735.RSA_enable == false)
			{	
				goto psid_false;	
			}
			break;//RSA
		case 82057:
			if(G_relay_inno_config.v2x.rx.j2735.RTCM_enable == false)
			{	
				goto psid_false;	
			}
			break;//RTCM
		case 82054:
			if(G_relay_inno_config.v2x.rx.j2735.TIM_enable == false)
			{	
				goto psid_false;	
			}
			break;//TIM
		default :
		{
			break;
		}
	}		
	return true;
psid_false:
	return false;

}