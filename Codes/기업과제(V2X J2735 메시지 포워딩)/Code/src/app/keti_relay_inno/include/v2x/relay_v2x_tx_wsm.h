#include "relay-internal-system.h"
#include "relay-extern-defines.h"

#ifndef _D_HEADER_RELAY_INNO_V2X_TX_WSM
#define _D_HEADER_RELAY_INNO_V2X_TX_WSM

#include "dot2-2016/dot2.h"
#include "dot3-2016/dot3.h"

#include "ltev2x-hal/ltev2x-hal.h"
#include "ffasn1c/ffasn1-dot2-2021.h"
#include "ffasn1c/ffasn1-dot3-2016.h"
#include "ffasn1c/ffasn1-j2735-2016.h"

#ifndef _D_HEADER_RELAY_INNO_V2X
#endif

enum relay_inno_wsm_ext_type_e{
	RELAY_INNO_WSM_EXT_ID_TX_POWER = 0,
	RELAY_INNO_WSM_EXT_ID_CHANNEL,
	RELAY_INNO_WSM_EXT_ID_DATERATE,
};

struct realy_inno_wsm_header_ext_data_t
{
	int tx_power;
	unsigned int tx_channel_num;
	unsigned int tx_datarate;
};

struct relay_inno_msdu_t
{
	bool isused;
	bool isfilled;
	uint8_t *msdu;
	asn1_ssize_t msdu_size;
	struct LTEV2XHALMSDUTxParams tx_params;
};

#define RELAY_INNO_Fill_TxPrams(str, ...) _D_F_RELAY_INNO_Fill_TxPrams(str, ##__VA_ARGS__, 0xFFFFFFFF)

EXTERN_API asn1_ssize_t RELAY_INNO_WSM_Fill_MSDU(dot3ShortMsgNpdu *wsm, const dot3ShortMsgData *wsm_body, uint8_t **msdu_in);
EXTERN_API void RELAY_INNO_WSM_Fill_Header(dot3ShortMsgNpdu **wsm_in, unsigned int tx_psid, struct realy_inno_wsm_header_ext_data_t *ext_data);
EXTERN_API void RELAY_INNO_WSM_Free_Header(dot3ShortMsgNpdu *wsm);
EXTERN_API int RELAY_INNO_V2X_MSDU_Transmit(const dot3ShortMsgData *wsm_body, dot3ShortMsgNpdu *wsm, struct LTEV2XHALMSDUTxParams *tx_params);

#endif //?_D_HEADER_RELAY_INNO_V2X_TX_WSM


