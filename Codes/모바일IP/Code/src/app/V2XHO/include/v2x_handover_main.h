
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/types.h>
#include <dirent.h>
#include <getopt.h>
#include <stdarg.h>

#include <time.h>
#include <sys/timerfd.h>

#include <poll.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <sys/ioctl.h>
#include <syslog.h>


#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <linux/if_ether.h>
#include <linux/udp.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "v2x-sw.h"
#include "gps.h"
#include <math.h>
#include <linux/pps.h>

#include "dot2/dot2.h"
#include "dot3/dot3.h"
#include "ffasn1c/ffasn1-dot3-2016.h"
#include "wlanaccess/wlanaccess.h"

#include "j29451/j29451-types.h"

#include "v2x_handover_util_router.h"
#include "v2x_handover_util_gnss.h"
#include "v2x_handover_util_uds.h"
#include "v2x_handover_debug_pingtest.h"
#include "tunnel.h"

#include <V2XHO_Router.h>

#ifndef _V2XHO_MAIN_H_
#define _V2XHO_MAIN_H_

#define V2XHO_INITAIL_SETUP_CONFIGURAION_FILE_PATH "./config/"
#define V2XHO_INITAIL_SETUP_CONFIGURAION_FILE_NAME "v2xho_config.conf"

#define V2XHO_IPV4_ALEN_BIN 4
#define V2XHO_IPV4_ALEN_STR INET_ADDRSTRLEN
#define V2XHO_IPV6_ALEN_BIN 16
#define V2XHO_IPV6_ALEN_STR INET6_ADDRSTRLEN

#define V2XHO_IPV6_PREFIX_LEN_COMPATIBLE 96

#define V2XHO_INTERFACE_NAME_SIZE 64
#define V2XHO_INTERFACE_MAC_STRING_SIZE 17
#define V2XHO_INTERFACE_IPv4_ADDRESS_SIZE V2XHO_IPV4_ALEN_STR
#define V2XHO_INTERFACE_IPv6_ADDRESS_SIZE V2XHO_IPV6_ALEN_STR
#define V2XHO_INTERFACE_PREFIX_SIZE 3
#define V2XHO_ONLY_IP_INTERFACE_SIZE 3

#define V2XHO_DEFAULT_DSRC_CHANNEL_0 182
#define V2XHO_DEFAULT_DSRC_CHANNEL_1 184
#define V2XHO_DEFAULT_DSRC_TX_POWER_0 20
#define V2XHO_DEFAULT_DSRC_TX_POWER_1 20

#define V2XHO_DEFAULT_LTEV2X_CHANNEL 183
#define V2XHO_DEFAULT_LTEV2X_TX_POWER 20

#define V2XHO_DEFAULT_ADVERTISEMENT_PORT 434
#define V2XHO_DEFAULT_REGISTRATION_REQUEST_PORT 434
#define V2XHO_DEFAULT_REGISTRATION_REPLY_PORT 434

#define V2XHO_DEFAULT_WSA_SIZE_MAX 2330

#define V2XHO_DEFAULT_COMMUNICATIONS_PATH_ROOT "./"
#define V2XHO_DEFAULT_COMMUNICATIONS_PATH V2XHO_DEFAULT_COMMUNICATIONS_PATH_ROOT"communications/"
#define V2XHO_DEFAULT_COMMUNICATIONS_UDS_PATH "uds/"
#define V2XHO_DEFAULT_RSU_HA_UDS_SERVER_NAME "server_rsu_ha"
#define V2XHO_DEFAULT_OBU_UDS_SERVER_NAME "server_obu"

#define V2XHO_DEFAULT_FILEDB_PATH_ROOT "./"
#define V2XHO_DEFAULT_FILEDB_PATH V2XHO_DEFAULT_FILEDB_PATH_ROOT"fileDB/"

#define V2XHO_DEFULAT_RAMDISK_DEFAULT_SIZE 1024 * 1000 * 4 // 4M
#define V2XHO_DEFULAT_RAMDISK_FILEDB_SIZE 1024 * 1000 * 64 // 64M
#define V2XHO_DEFAULT_RAW_SOCKET_BUFFER_SIZE 65536

#define V2XHO_ERROR_CODE_FILE_CODE_MAIN 0x10
#define V2XHO_ERROR_CODE_FILE_CODE_RSA_HA 0x40
#define V2XHO_ERROR_CODE_FILE_CODE_RSA_FA 0x41
#define V2XHO_ERROR_CODE_FILE_CODE_OBU 0x42
#define V2XHO_ERROR_CODE_FILE_CODE_WAVE_IPV6 0x11
#define V2XHO_ERROR_CODE_FILE_CODE_WAVE_RX 0x12
#define V2XHO_ERROR_CODE_FILE_CODE_UTILS 0x20
#define V2XHO_ERROR_CODE_FILE_CODE_UTIL_DASHBOARD 0x21
#define V2XHO_ERROR_CODE_FILE_CODE_UTIL_GNSS 0x22
#define V2XHO_ERROR_CODE_FILE_CODE_UTIL_TICK_TIMER 0x23
#define V2XHO_ERROR_CODE_FILE_CODE_UTIL_UDS 0x24

#define V2XHO_ROUTE_LIST_NUM_MAX 128



#ifndef USED_ROUTER_API
#define USED_ROUTER_API 1
	int preferred_family;
	int show_stats;
	int show_details;
	int timestamp;
	int compress_vlans;
	int json;
	struct rtnl_handle rth;
#endif

#ifndef _V2XHO_MSG_DEFINED
#define _V2XHO_MSG_DEFINED
typedef struct
{    
    uint8_t type;
    struct{
        uint8_t x:1; // Mobility agent supports Registration Revocation as specified in [28]
        uint8_t T:1; // Foreign agent supports reverse tunneling as specified in [12]
        uint8_t r:1; // Sent as zero; ignored on reception.
        uint8_t G:1; // Generic Routing Encapsulation.
        uint8_t M:1; // Minimal encapsulation.  /* M:0, G:0 IPIP tunnel*/
        uint8_t B:1;  // registering with a co-located careof address
                        // Set 1: decapsulate any datagrams tunneled to this careof address
                        // Set 0: the foreign agent will thus decapsulate arriving datagrams before forwarding them to the mobile node.
        uint8_t D:1; // request its home agent toforward to it a copy of broadcast datagrams received by its home agent from the home network.
                        /*Example If B is seted 1, D MUST SET 1*/
        uint8_t S:1; //the home agent maintain prior mobility binding 
        
    } flag;                                                  
    uint16_t lifetime; //MAX 1800[sec], Set 0, delete a particular mobility binding
    uint32_t home_address;
    uint32_t home_agent;
    uint32_t care_of_address;
    int64_t identification;
} __attribute__((__packed__)) v2xho_registration_request_payload_t;
typedef struct
{    
    uint8_t type;
    uint8_t code;                                               
    uint16_t lifetime; //MAX 1800[sec], Set 0, delete a particular mobility binding
    uint32_t home_address;
    uint32_t home_agent;
    int64_t identification;
} __attribute__((__packed__)) v2xho_registration_reply_payload_t;

enum v2xho_reg_reply_code_e
{
    V2XHO_REPLY_CODE_REQ_ACCEPTED = 0,                              // V2X handover request is accepted.
    V2XHO_REPLY_CODE_REQ_ACCEPTED_NO_MULTI_COA = 1,                 // Request accepted; OBU cannot use multiple CoAs.

    V2XHO_REPLY_CODE_FA_REQ_DENIED_UNKNOWN = 64,                    // Request denied by RSU (FA): Unknown reason.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_ADMIN = 65,                      // Request denied by RSU (FA): Administrative reasons.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_RESOURCE = 66,                   // Request denied by RSU (FA): Insufficient resources.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_OBU_AUTH_FAIL = 67,              // Request denied by RSU (FA): OBU authentication failed.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_RSU_AUTH_FAIL = 68,              // Request denied by RSU (FA): RSU (HA) authentication failed.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_LIFETIME_TOO_LONG = 69,          // Request denied by RSU (FA): Requested lifetime too long.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_INVALID_REQ_FORMAT = 70,         // Request denied by RSU (FA): Invalid Registration Request format.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_INVALID_REPLY_FORMAT = 71,       // Request denied by RSU (FA): Invalid Registration Reply format.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_UNSUPPORTED_TUNNEL = 72,         // Request denied by RSU (FA): Unsupported tunneling method.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_RESERVED = 73,                   // Request denied by RSU (FA): Reserved and unavailable.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_INVALID_COA = 77,                // Request denied by RSU (FA): Invalid CoA address.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_TIMEOUT = 78,                    // Request denied by RSU (FA): Registration timeout.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_NET_INACCESSIBLE = 80,           // Request denied by RSU (HA): Network inaccessible.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_HOST_INACCESSIBLE = 81,          // Request denied by RSU (HA): Host inaccessible.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_PORT_INACCESSIBLE = 82,          // Request denied by RSU (HA): Port inaccessible.
    V2XHO_REPLY_CODE_FA_REQ_DENIED_RSU_INACCESSIBLE = 88,           // Request denied by RSU (HA): RSU (HA) inaccessible.

    V2XHO_REPLY_CODE_FA_REQ_DENIED_INVALID_HOME_ADDR = 194,         // Request denied by RSU (FA): Invalid Home Address.

    V2XHO_REPLY_CODE_HA_REQ_DENIED_UNKNOWN = 128,                   // Request denied by RSU (HA): Unknown reason.
    V2XHO_REPLY_CODE_HA_REQ_DENIED_ADMIN = 129,                     // Request denied by RSU (HA): Administrative reasons.
    V2XHO_REPLY_CODE_HA_REQ_DENIED_RESOURCE = 130,                  // Request denied by RSU (HA): Insufficient resources.
    V2XHO_REPLY_CODE_HA_REQ_DENIED_OBU_AUTH_FAIL = 131,             // Request denied by RSU (HA): OBU authentication failed.
    V2XHO_REPLY_CODE_HA_REQ_DENIED_FA_AUTH_FAIL = 132,              // Request denied by RSU (HA): RSU (FA) authentication failed.
    V2XHO_REPLY_CODE_HA_REQ_DENIED_ID_MISMATCH = 133,               // Request denied by RSU (HA): Identification field mismatch.
    V2XHO_REPLY_CODE_HA_REQ_DENIED_INVALID_REQ_FORMAT = 134,        // Request denied by RSU (HA): Invalid Registration Request format.
    V2XHO_REPLY_CODE_HA_REQ_DENIED_TOO_MANY_MULTI_COA = 135,        // Request denied by RSU (HA): Excessive Multi-CoA usage.
    V2XHO_REPLY_CODE_HA_REQ_DENIED_UNKNOWN_HOME_ADDR = 136    
};

#include "v2x_handover_rsu_ha.h"
#include "v2x_handover_rsu_fa.h"
#include "v2x_handover_obu.h"

#endif


enum v2xho_error_code_e
{
    V2XHO_NO_ERROR = 0,

    V2XHO_MAIN = 10,
    V2XHO_MAIN_INITIAL_SETUP_CONFIGURATION_NO_CONFIG_FILE,
    V2XHO_MAIN_INITIAL_SETUP_CONFIGURATION_NO_ENABLE,
    V2XHO_MAIN_INITIAL_SETUP_HANDOVER_INTERFACE_NO_IPv4_ADDRESS,
    V2XHO_MAIN_INITIAL_SETUP_ONLYIP_INTERFACE_NO_IPv4_ADDRESS,
    V2XHO_MAIN_INITIAL_V2X_INTITAL_FUNC_NOT_WORKING,

    V2XHO_WAVE_RX = 40,
    V2XHO_WAVE_RX_DSRC_PACKET_PARSING_NULL,
    V2XHO_WAVE_RX_NOT_INTEREST_PSID,
    
    V2XHO_WAVE_IPV6 = 60,
    V2XHO_WAVE_IPV6_NOT_SETTING_ADDRESS,
    V2XHO_WAVE_IPV6_NOT_SETTING_AUTO_LINK,

    V2XHO_UTILS = 100,
    V2XHO_UTIL_UDS = V2XHO_UTILS + 10,
    V2XHO_UTIL_UDS_SOCKET_FILE_NOT_OPEN,
    V2XHO_UTIL_UDS_RMDIR_ERROR_ISDIR,
    V2XHO_UTIL_UDS_RMDIR_ERROR_ISREG,
    V2XHO_UTIL_DASHBOARD = V2XHO_UTILS + 40,
    V2XHO_UTIL_DASHBOARD_WINDOW_FUNC_NOT_WORKING,
    V2XHO_UTIL_DASHBOARD_WINDOW_SIZE_SMALLER,

    V2XHO_LIBRARY_V2X_DOT3_FUNC_ERR = 120,

    V2XHO_OBU = 140,
    V2XHO_OBU_UDS_SOCKET_PATH_NOT_ACCESS,
    V2XHO_OBU_UDS_SOCKET_NOT_CREATE,
    V2XHO_OBU_REGISTRATION_REQUEST_SEND_FAILURE,

    V2XHO_EQUIPMENT_RSU_HA = 150,
    V2XHO_EQUIPMENT_RSU_HA_Task_RECV_SOCKET_BIND,
    V2XHO_EQUIPMENT_RSU_HA_WSA_CONSTRUCT_ERROR,

    V2XHO_EQUIPMENT_RSU_FA = 160,
    V2XHO_EQUIPMENT_RSU_FA_Task_RECV_SOCKET_BIND,
    V2XHO_EQUIPMENT_RSU_FA_WSA_CONSTRUCT_ERROR,
    
};

enum v2xho_equiptype_e
{
    DSRC_RSU_HA = 10,
    DSRC_RSU_FA,
    DSRC_RSU_HAFA,
    DSRC_OBU,
    DSRC_OBU_FA,

    LTEV2X_RSU_HA = 20,
    LTEV2X_RSU_FA,
    LTEV2X_RSU_HAFA,
    LTEV2X_OBU,
    LTEV2X_OBU_FA,
};

struct v2xho_equip_states_t
{
    enum v2xho_equiptype_e equip_type;
    union { 
        v2xho_equip_state_obu_t stats_obu;
        v2xho_equip_state_rsu_ha_t stats_rsu_ha;
        v2xho_equip_state_rsu_fa_t stats_rsu_fa;
    } u;
};

typedef struct 
{
    struct v2xho_equip_states_t status;

    uint32_t uds_socket_num;
    struct v2xho_util_uds_info_t uds_socket_list[4];

    uint8_t src_mac[6];
    int raw_socket_ip;
    uint8_t empty_0;
    int raw_socket_eth;
    uint8_t empty_1;
    int udp_socket;
} __attribute__((__packed__)) v2xho_equip_info_t;

typedef struct
{
    uint32_t chan_0;
    uint32_t chan_1;
    uint32_t datarate_0;
    uint32_t datarate_1;
    int tx_power_0;
    int tx_power_1;
} __attribute__((__packed__)) v2xho_dsrc_t;

typedef struct
{
    uint32_t chan;
    uint32_t datarate;
    int tx_power;
} __attribute__((__packed__)) v2xho_ltev2x_t;

typedef struct 
{
    uint32_t saddr;
    uint32_t daddr;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t udp_len;
    uint16_t source;
    uint16_t dest;
    uint16_t length;
    uint8_t data[128];
}__attribute__((__packed__)) v2xho_udp_checksum_t;
typedef struct 
{
    uint32_t saddr;
    uint32_t daddr;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t udp_len;
}__attribute__((__packed__)) v2xho_udp_pesudo_header_t;
typedef struct 
{
    int config_enable;
    enum v2xho_equiptype_e equipment_type;

    char v2xho_interface_name[V2XHO_INTERFACE_NAME_SIZE];
    char v2xho_interface_MAC_S[V2XHO_INTERFACE_MAC_STRING_SIZE + 1];
    char v2xho_interface_IP[V2XHO_INTERFACE_IPv4_ADDRESS_SIZE + 1];
    int v2xho_interface_prefix;

    int count_onlyIP_interface;
    char onlyIP_interface_name[V2XHO_ONLY_IP_INTERFACE_SIZE][V2XHO_INTERFACE_NAME_SIZE];
    char onlyIP_interface_IP[V2XHO_ONLY_IP_INTERFACE_SIZE][V2XHO_INTERFACE_IPv4_ADDRESS_SIZE + 1];
    int onlyIP_interface_prefix[V2XHO_ONLY_IP_INTERFACE_SIZE];

    bool tunneling_enable;
    
    bool security_enable; 
    union {
        v2xho_dsrc_t dsrc;
        v2xho_ltev2x_t ltev2x;
    }u;

    bool debug_dashboard_enable;
    int debug_dashboard_repeat_time;
    
    bool debug_pingtest_enable;
    bool debug_pingtest_logging_enable;
    bool debug_pingtest_pcap_enable;

    struct v2xho_debug_pingtest_info_t ping_info;
    
} __attribute__((__packed__)) v2xho_config_t;

/*v2x_handover 매크로 정의*/
#define V2XHO_MAC_PAD(x) x[0], x[1], x[2], x[3], x[4], x[5]

/*v2x_handover 매크로 콜백*/
#define lclock_ms F_d_V2XHO_Utils_Clockms()

/*v2x_handover 전처리 함수*/
#define _DEBUG_LOG printf("[DEBUG][%d][%s]\n", __LINE__, __func__);
#define TEXT_RED(TEXT)    "\033[1;31m"TEXT"\033[0m"
#define TEXT_GREEN(TEXT)  "\033[1;32m"TEXT"\033[0m"
#define lsystem(command, ret) ret = system(command);memset(command, 0x00, strlen(command));if(WIFEXITED(ret)){}else if(WIFSIGNALED(ret)){printf("Command was terminated by signal: %d\n", WTERMSIG(ret));}printf("\b")
#define lmac_printf(x) printf("MAC:%02X:%02X:%02X:%02X:%02X:%02X\n", x[0], x[1], x[2], x[3], x[4], x[5])
#define lSafeFree(ptr) if(ptr){free(ptr);}else{printf("[%d][%s]Not Allocation!:%p\n", __LINE__, __func__, ptr);}
#define lreturn(code_value) enum v2xho_error_code_e c=code_value;return (c * 0x1000000) + (V2XHO_ERROR_CODE_FILE_CODE * 0x010000) + (__LINE__ % 0xFFFF)

#endif

/*v2x_handover_main*/
extern struct V2XHO_IProuter_Route_List_info_IPv4_t G_route_list_info[DEFAULT_ROUTE_LIST_NUM_MAX];
extern int G_route_list_num;
extern v2xho_config_t G_v2xho_config;

/*v2x_handover_rsu_ha*/
extern void *Th_v_V2XHO_RSU_HA_RAW_Receive(void *d);
extern void *Th_v_V2XHO_RSU_HA_Task_Running_Send(void *d);

/*v2x_handover_rsu_fa*/
extern void *Th_v_V2XHO_RSU_FA_RAW_Receive(void *d);
extern void *Th_v_V2XHO_RSU_FA_Task_Running_Send(void *d);

/*v2x_handover_obu*/
extern void *Th_v_V2XHO_OBU_Task_Running_Do(void *d);
extern int F_i_V2XHO_OBU_UDS_Send_WSMP_Data(v2xho_wsmp_t *wsmp);

/*v2x_handover_wave_rx*/
extern void F_v_V2XHO_WAVE_Rx_DOT2_SPDU_Callback(Dot2ResultCode dot2_result, void *priv);
extern void F_v_V2XHO_WAVE_Rx_MPDU_Callback(const uint8_t *mpdu, WalMPDUSize mpdu_size, const struct WalMPDURxParams *rx_params);

/*v2x_handover_wave_ipv6*/
extern int F_i_V2XHO_WAVE_IPv6_Setup(void);

/*v2x_handover_utils*/
extern int F_i_V2XHO_Initial_Setup_Configuration_Read(v2xho_config_t *G_v2xho_config);
extern void F_v_V2XHO_Print(const char *text, const char *format, ...);
extern void F_v_V2XHO_IPv4_Address_Get(const char *iface, char *ipv4_addr, int *prefix);
extern int F_i_V2XHO_Utils_Rmdirs(const char *path, int force);
extern uint16_t F_u16_V2XHO_IP_Checksum(unsigned short *buf, int nwords);
extern uint16_t F_u16_V2XHO_UDP_Checksum(unsigned short *buf, int nwords);
extern void F_v_V2XHO_MAC_ADDR_TRANS_BIN_to_STR(char *src, char *dst);
extern void F_v_V2XHO_Utils_Eth_Device_Flush(char *dev);
extern double F_d_V2XHO_Utils_Clockms();


/*v2x_handover_util_uds*/
extern int F_i_V2XHO_Util_UDS_Create_Socket_File(char *path, int force);
extern int F_i_V2XHO_Util_UDS_Socket_Bind(char* file_path);

/*v2x_handover_util_gnss*/
extern int F_i_V2XHO_Gnss_Init_Gnssata(pthread_t *gnss_thread_t);
extern struct v2xho_gnss_data_t *G_gnss_data;

/*v2x_handover_util_pingtest*/
extern void *Th_v_V2XHO_Util_Pingtest_Task_Running(void *d);

/*v2x_handover_util_tick_timer*/
extern void *Th_v_V2XHO_Util_Task_Tick_Timer(void *d);
extern uint32_t G_tick_10ms;
extern uint32_t G_tick_100ms;
extern uint32_t G_tick_1000ms;
