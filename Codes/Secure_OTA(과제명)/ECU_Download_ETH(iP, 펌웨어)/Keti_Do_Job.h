

#include <stdio.h>
#include <stdint.h>

#include "board.h"
#include <lwip/sys.h>
#include "pin_mux.h"
#include "lwip/init.h"
#include "lwip/inet_chksum.h"
#include "lwip/ip_addr.h"
#include "lwip/ip.h"
#include "lwip/udp.h"
#include "lwip/raw.h"
#include "netif/ethernet.h"
#include "ethernetif.h"
#include "fsl_enet.h"
#include "fsl_gpio.h"
#include "fsl_common.h"

#ifndef configMAC_ADDR
#include "fsl_silicon_id.h"
#endif

#include "fsl_phyksz8081.h"
#include "fsl_phyrtl8211f.h"

#ifndef KETI_TYPE_DEF
#define KETI_TYPE_DEF 1
/* Keti */
typedef uint8_t   u8_t;
typedef int8_t    s8_t;
typedef uint16_t  u16_t;
typedef int16_t   s16_t;
typedef uint32_t  u32_t;
typedef int32_t   s32_t;

typedef enum Keti_Err{
    ERROR_NONE = 0,

} Keti_Err_e;

enum Keti_Eth_Type_e{
    Ethernet_100M = 0,
    Ethernet_1G,
};

#define MASK_Enum_Fireware 1000
#define MASK_Enum_Model 2000

enum Keti_Recver_State_e{
    Fireware_Ethernet_Device_Initial = 1 + MASK_Enum_Model,
	Fireware_Wait_Indication_From_ECU = 10 + MASK_Enum_Fireware,
	Fireware_Response_Info_From_ECU = 20 + MASK_Enum_Fireware,
	Fireware_Waiting_Download_From_ECU = 21 + MASK_Enum_Fireware,
    Fireware_Waiting_Download_Divied_Data_From_ECU = 22 + MASK_Enum_Fireware,
	Fireware_Finish_Download_From_ECU = 30 + MASK_Enum_Fireware,
	Fireware_Check_Download_Payload = 31 + MASK_Enum_Fireware,
	Fireware_Error_Download_From_ECU = 39 + MASK_Enum_Fireware,
	Fireware_Flashing_Download_Payload = 40 + MASK_Enum_Fireware,
	Fireware_Check_Flashing_Download_Payload = 41 + MASK_Enum_Fireware,
	Fireware_Error_Flashing_Download_Payload = 49 + MASK_Enum_Fireware,
	Fireware_No_Response_From_ECU = 60 + MASK_Enum_Fireware,
    Fireware_Download_Task_Start = 70 + MASK_Enum_Fireware,
    Fireware_Error_Repeat_Download_Task_Start = 71 + MASK_Enum_Fireware,
    Fireware_Error_Download_Task_Start = 72 + MASK_Enum_Fireware,
    Fireware_Ethernet_Device_No_Initial = 90 + MASK_Enum_Fireware,

    Model_Ethernet_Device_Initial = 1 + MASK_Enum_Model,
	Model_Wait_Indication_From_ECU = 10 + MASK_Enum_Model,
	Model_Response_Info_From_ECU = 20 + MASK_Enum_Model,
	Model_Waiting_Download_From_ECU = 21 + MASK_Enum_Model,
    Model_Waiting_Download_Divied_Data_From_ECU = 22 + MASK_Enum_Model,
	Model_Finish_Download_From_ECU = 30 + MASK_Enum_Model,
	Model_Check_Download_Payload = 31 + MASK_Enum_Model,
	Model_Error_Download_From_ECU = 39 + MASK_Enum_Model,
	Model_Flashing_Download_Payload = 40 + MASK_Enum_Model,
	Model_Check_Flashing_Download_Payload = 41 + MASK_Enum_Model,
	Model_Error_Flashing_Download_Payload = 49 + MASK_Enum_Model,
    Model_No_Response_From_ECU = 60 + MASK_Enum_Model,
    Model_Download_Task_Start = 70 + MASK_Enum_Model,
    Model_Error_Repeat_Download_Task_Start = 71 + MASK_Enum_Model,
    Model_Error_Download_Task_Start = 72 + MASK_Enum_Model,
    Model_Ethernet_Device_No_Initial = 90 + MASK_Enum_Model,
};

struct Keti_data_p_hdr_t{
    uint32_t div_num;
    uint32_t payload_len;
};

struct Eth_Addr_Info_t{
    uint8_t mac_addr[6U];
    ip4_addr_t src_addr;
    ip4_addr_t dst_addr;
    ip4_addr_t gw_addr;

    ethernetif_config_t enet_config;
    struct netif netif;
    struct udp_pcb *udp_pcb;
    struct tcp_pcb *tcp_pcb;
    struct raw_pcb *raw_pcb;

    u8_t *send_buf;
    u32_t send_buf_len;
    u32_t send_timer;

    enum Keti_Recver_State_e rs;

    u8_t onair_time_ms[2];
    u32_t *time_check;
};

struct Eth_Info_t{
    bool Eth_100M_On;
    struct Eth_Addr_Info_t *eth_100M;

    bool Eth_1G_On;
    struct Eth_Addr_Info_t *eth_1G;
};

struct Keti_UDP_Data_Recv_Length_Info_t
{
	uint32_t totla_len;
	uint32_t now_len;
};

struct Keti_UDP_Data_Recv_Buf_t
{
	int isempty;
	char *recv_data;
	struct Keti_UDP_Data_Recv_Length_Info_t recv_length;
};

struct Keti_UDP_Header_t
{
    u8_t STX;
    u8_t type[2];
    u8_t div_len[2];
    u32_t total_data_len;
    u8_t div_num[2];
    u8_t ETX;
};
#endif

extern phy_ksz8081_resource_t g_phy_resource;
extern phy_rtl8211f_resource_t g_phy_resource_1G;

extern void Keti_Do_Job();
static Keti_Err_e Keit_Eth_UDP_Sender(struct udp_pcb *pcb, struct raw_pcb *raw_pcb, u8_t *data, u16_t data_len);
static struct Keti_UDP_Header_t Keti_UDP_Header_Parser(char *data);
