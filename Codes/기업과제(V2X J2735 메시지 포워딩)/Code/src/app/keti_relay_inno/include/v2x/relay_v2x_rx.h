#include "relay-internal-system.h"
#include "relay-extern-defines.h"

#ifndef _D_HEADER_RELAY_INNO_V2X_RX
#define _D_HEADER_RELAY_INNO_V2X_RX
#include "dot2-2016/dot2.h"
#include "dot3-2016/dot3.h"

#include "ltev2x-hal/ltev2x-hal.h"
#include "ffasn1c/ffasn1-dot2-2021.h"
#include "ffasn1c/ffasn1-dot3-2016.h"
#include "ffasn1c/ffasn1-j2735-2016.h"
struct relay_inno_config_v2x_rx_j2735_t
{
		bool BSM_enable;
		bool SPAT_enable;
		bool MAP_enable;
		bool RTCM_enable;
		bool TIM_enable;
		bool PVD_enable;
		bool RSA_enable;
};
struct relay_inno_config_v2x_rx_t
{
	bool enable;
	bool dot2_forced_enable; ///< V2X-Dot2 강제 수신 여부
	bool wsa_enable; ///< WSA 수신 여부
	struct relay_inno_config_v2x_rx_j2735_t j2735;
};
#endif //?_D_HEADER_RELAY_INNO_V2X_RX

extern void RELAY_INNO_V2X_RxMSDUCallback(const uint8_t *msdu, unsigned int msdu_size, struct LTEV2XHALMSDURxParams rx_params);
