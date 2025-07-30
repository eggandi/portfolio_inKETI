#include "relay-internal-system.h"
#include "relay-extern-defines.h"

#ifndef _D_HEADER_RELAY_INNO_V2X_J2735_BSM
#define _D_HEADER_RELAY_INNO_V2X_J2735_BSM	

#ifndef _D_HEADER_RELAY_INNO_V2X_TX
#include "relay_v2x_tx.h"
#endif
#ifndef _D_HEADER_RELAY_INNO_GNSS
#include "relay_gnss.h"
#endif

#define RELAY_INNO_TEMPORARY_ID_LEN (4)
#define RELAY_INNO_INCREASE_BSM_MSG_CNT(cnt) (((cnt) + 1) % 128)

struct relay_inno_config_v2x_j2735_bsm_t
{
		bool enable; ///< BSM 송신 여부
		bool j29451_enable;
		bool rx_enable;
		bool tx_forced; ///< BSM 송신 강제 여부
		unsigned int psid;
		uint8_t msg_cnt; ///< BSM 메시지 카운트
		uint8_t temporary_id[RELAY_INNO_TEMPORARY_ID_LEN]; ///< BSM 임시 ID
		enum relay_inno_v2x_tx_type_e tx_type;
		unsigned int interval;
		unsigned int priority;
		int tx_power;
		struct relay_inno_gnss_data_bsm_t *gnss_data_bsm_ptr; ///< BSM GNSS 데이터
};

#endif //?_D_HEADER_RELAY_INNO_V2X_J2735_BSM
EXTERN_API uint8_t *RELAY_INNO_J2735_Construct_BSM(size_t *bsm_size);
EXTERN_API int RELAY_INNO_J2735_Fill_BSM(struct j2735BasicSafetyMessage *bsm);
EXTERN_API int RELAY_INNO_J2735_BSM_Gnss_Info_Ptr_Instrall(struct j2735BSMcoreData **core_ptr);
EXTERN_API int RELAY_INNO_J2735_BSM_Fill_VarLengthNumber(int psid, dot3VarLengthNumber *to);
EXTERN_API int RELAY_INNO_J2736_J29451_Initial();

