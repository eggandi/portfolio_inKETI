
#include "relay-internal-system.h"
#include "relay-extern-defines.h"

#ifndef _D_HEADER_RELAY_INNO_V2X_TX
#define _D_HEADER_RELAY_INNO_V2X_TX

// RELAY_INNO 소스 헤더 파일

#ifndef _D_HEADER_RELAY_INNO_V2X_TX_WSM
#include "relay_v2x_tx_wsm.h"
#endif
#ifndef _D_HEADER_RELAY_INNO_V2X_TX_J2735
#include "relay_v2x_tx_j2735.h"
#endif

enum relay_inno_v2x_tx_type_e
{
		RELAY_INNO_V2X_TX_TYPE_SPS = 0, 
		RELAY_INNO_V2X_TX_TYPE_EVENT,
		RELAY_INNO_V2X_TX_TYPE_DEFAULT, ///< Main V2X 설정값 사용
};

#endif //?_D_HEADER_RELAY_INNO_V2X_TX
