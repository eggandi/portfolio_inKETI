#include "relay-internal-system.h"
#include "relay-extern-defines.h"

#ifndef _D_HEADER_RELAY_INNO_V2X_TX_J2735
#define _D_HEADER_RELAY_INNO_V2X_TX_J2735

#include "dot2-2016/dot2.h"
#include "dot3-2016/dot3.h"
#include "j29451/j29451.h"

#include "ltev2x-hal/ltev2x-hal.h"
#include "ffasn1c/ffasn1-dot2-2021.h"
#include "ffasn1c/ffasn1-dot3-2016.h"
#include "ffasn1c/ffasn1-j2735-2016.h"

#endif //?_D_HEADER_RELAY_INNO_V2X_TX_J2735

EXTERN_API int RELAY_INNO_V2X_Tx_J2735_BSM(dot3ShortMsgNpdu *wsm_in);
