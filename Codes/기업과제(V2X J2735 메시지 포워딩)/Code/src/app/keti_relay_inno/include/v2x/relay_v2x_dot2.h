#include "relay-internal-system.h"
#include "relay-extern-defines.h"

#ifndef _D_HEADER_RELAY_INNO_V2X_DOT2
#define _D_HEADER_RELAY_INNO_V2X_DOT2

#include "dot2-2016/dot2.h"
#include "dot3-2016/dot3.h"

#include "ltev2x-hal/ltev2x-hal.h"
#include "ffasn1c/ffasn1-dot2-2021.h"
#include "ffasn1c/ffasn1-dot3-2016.h"
#include "ffasn1c/ffasn1-j2735-2016.h"

struct relay_inno_config_v2x_dot2_t
{
		bool enable; ///< V2X-Dot2 동작 여부
		char cert_path[256]; ///< 인증서 경로
		char trustedcerts_path[256]; ///< 인증서 경로
		bool cmhf_obu_enable; ///< OBU 인증서 사용 여부
		char cmhf_obu_path[256]; ///< OBU 인증서 경로
		bool cmhf_rsu_enable; ///< RSU 인증서 사용 여부
		char cmhf_rsu_path[256]; ///< RSU 인증서 경로
};

#endif //?_D_HEADER_RELAY_INNO_V2X_DOT2

extern int RELAY_INNO_V2X_Dot2_Security_Init();
