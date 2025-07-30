#include "relay-internal-system.h"
#include "relay-extern-defines.h"

#ifndef _D_HEADER_RELAY_INNO_CONFIG
#define _D_HEADER_RELAY_INNO_CONFIG
#include "relay_v2x.h"

#define RELAY_INNO_INITAIL_SETUP_CONFIGURAION_FILE_PATH "./"
#define RELAY_INNO_INITAIL_SETUP_CONFIGURAION_FILE_NAME "kRelay.conf"

enum relay_inno_relay_data_type_e
{
		RELAY_DATA_TYPE_V2X_MPDU = 0, //MPDU 수신된 패킷 (DSRC의 경우 MPDU, C-V2X의 경우 WSM이 저장된다)(?)
		RELAY_DATA_TYPE_V2X_WSM, ///< mpdu 버퍼 내에서 WSM 패킷의 시작지점을 참조한다.
		RELAY_DATA_TYPE_V2X_WSDU, ///< WSM Service Data Unit. WSM body에 수납된 데이터(=Ieee1609Dot2Data).
		RELAY_DATA_TYPE_V2X_SSDU, //< Secured Service Data Unit. Ieee1609Dot2에 수납된 페이로드(예: J2735 메시지, WSA).
};

struct relay_inno_config_realy_t
{
		bool enable;
		enum relay_inno_relay_data_type_e relay_data_type; ///< 데이터 타입 (V2X, GNSS, V2X+GNSS)
		char dev_name[32];
		char gatewayip[INET_ADDRSTRLEN];
		uint16_t port_v2x_rx;
		uint16_t port_v2x_tx;

		bool gnss_enable; ///< GNSS 사용 여부
		uint32_t gnss_interval; ///< GNSS 수신 주기 (usec 단위)
};

struct relay_inno_config_t {
    bool config_enable;
    char config_path[512];
		char config_file_name[256];
		int config_debug_level;
		
		struct relay_inno_config_realy_t relay;
    struct relay_inno_config_v2x_t v2x;
};

#endif //?_D_HEADER_RELAY_INNO_CONFIG

extern struct relay_inno_config_t G_relay_inno_config;

extern int RELAY_INNO_Config_Setup_Configuration_Read(struct relay_inno_config_t *relay_inno_config);
extern int RELAY_INNO_Config_Pasrsing_Argument(int argc, char *argv[]);
