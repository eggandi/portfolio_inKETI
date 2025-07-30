#ifndef _V2XHO_OBU_H_
#define _V2XHO_OBU_H_
#include <stdint.h>
#include "dot2/dot2.h"
#include "dot3/dot3.h"


/**
 * @brief 장치 상태로 값이 일부 중첩 가능
 * 
 */
enum v2xho_equip_state_obu_e
{
    V2XHO_EQUIP_STATES_OBU_HANDOVER_IDEL = 0,           //0000000000000000
    V2XHO_EQUIP_STATES_OBU_HANDOVER_ACTIVE = 1,         //0000000000000001
    V2XHO_EQUIP_STATES_OBU_CONNECTING_HA = 2,           //0000000000000010
    V2XHO_EQUIP_STATES_OBU_CONNECTING_FA = 6,           //0000000000000110
    V2XHO_EQUIP_STATES_OBU_USING_coCoA = 8,             //0000000000001000
    V2XHO_EQUIP_STATES_OBU_USING_DeCAP = 16,            //0000000000010000
    V2XHO_EQUIP_STATES_OBU_SEND_SOLICITATION = 32,      //0000000000100000
    V2XHO_EQUIP_STATES_OBU_SEND_REG_REQUEST_HA = 64,    //0000000001000000
    V2XHO_EQUIP_STATES_OBU_SEND_REG_REQUEST_FA = 192,   //0000000011000000
    V2XHO_EQUIP_STATES_OBU_RECV_WSA_HA = 256,           //0000000100000000
    V2XHO_EQUIP_STATES_OBU_RECV_WSA_FA = 768,           //0000001100000000
    V2XHO_EQUIP_STATES_OBU_RECV_REG_REPLY_HA = 1024,    //0000010000000000
    V2XHO_EQUIP_STATES_OBU_RECV_REG_REPLY_FA = 3072,    //0000110000000000
    V2XHO_EQUIP_STATES_OBU_WAITING_WSA = 4096,          //0001000000000000
    V2XHO_EQUIP_STATES_OBU_WAITING_REG_REPLY_HA = 8192, //0010000000000000
    V2XHO_EQUIP_STATES_OBU_WAITING_REG_REPLY_FA = 24576,//0110000000000000
};

/**
 * @brief enum v2xho_equip_state_obu_e으로 확인 v2xho_equip_state_obu_t s_b = 0b01; (enum v2xho_equip_state_obu_e)(s_b.connecting * 2) == V2XHO_EQUIP_STATES_OBU_CONNECTING_HA
 * 
 */
typedef struct
{
    uint32_t active:1;
    uint32_t connecting:2;
    uint32_t coCoA:1;
    uint32_t DeCAP:1;
    uint32_t send_solicitation:1;
    uint32_t send_regrequest:2;
    uint32_t recv_wsa:2;
    uint32_t recv_regreply:2;
    uint32_t waiting_wsa:1;
    uint32_t waiting_regreply:2;
    uint32_t resolved:16;
} __attribute__((__packed__)) v2xho_equip_state_obu_t;

typedef struct
{
    uint8_t dest_mac[6];
    uint32_t source_address;
    uint32_t destination_addess;
    uint8_t time_to_live;
    uint32_t source_port;
    uint32_t destination_port;
    
    v2xho_registration_request_payload_t req_payload;
    v2xho_registration_reply_payload_t rep_payload;
    enum v2xho_util_router_metric_state_e metric;
} __attribute__((__packed__)) v2xho_rsu_ha_registration_info_t;

typedef struct {
    uint8_t src_mac[6];
    WalPower rx_power;
    Dot2ContentType dot2_type;
    union{
        struct Dot2ThreeDLocation dot2_location;
        struct Dot3WSAThreeDLocation wsa_location;
    } u;
    Dot3WSAIdentifier wsa_id;
    struct Dot3WRA wra;
} __attribute__((__packed__)) v2xho_wsmp_t;

enum v2xho_agent_type_e{
    V2XHO_OBU_UNKNOWN_AGENT = 0,
    V2XHO_OBU_FOREIGN_AGENT = 1,
    V2XHO_OBU_HOME_AGENT    = 3,
};
struct v2xho_rsu_ha_registration_sub_info_t
{
    bool isempty;
    enum v2xho_agent_type_e agent_type;
    v2xho_wsmp_t wsmp; // last received
    uint32_t recv_count;
    uint32_t recv_count_during_3s;
    double recv_time;
    double recv_time_list[128];
    WalPower rxpower_list[128]; 
    WalPower rxpower_avg;
    double distance;
    double last_reg_req_send_time;
    double last_reg_rep_recv_time;
};

struct v2xho_obu_registration_t
{
    int home_agent_num;
    int connecting_now;
    int registration_request_send_num;

    int registed_num;
    v2xho_rsu_ha_registration_info_t reg_list[16];
    struct v2xho_rsu_ha_registration_sub_info_t reg_sub_list[16];
};

typedef struct
{
    uint8_t src_MAC[6];
    char ip_src_add_str[V2XHO_IPV4_ALEN_STR];
    char ip_dst_add_str[V2XHO_IPV4_ALEN_STR];
    uint16_t icmp_seq_num;
    double recv_time;
    int lat; ///< 위도
	int lon; ///< 경도
    double distance_1;
    double distance_2;
    double distance_3;
} __attribute__((__packed__)) v2xho_debug_log_t;

struct ping_time_check_t
{
    uint16_t icmp_send_seq;
    double send_timing;
};

#define ICMP_ECHOREPLY           0        /* Echo Reply                        */
#define ICMP_ECHO                8        /* Echo Request   */
struct icmphdr
{
  u_int8_t icmp_type;                /* message type */
  u_int8_t icmp_code;                /* type sub-code */
  u_int16_t icmp_cksum;
  union
  {
    struct
    {
      u_int16_t        icmp_id;
      u_int16_t        icmp_seq_num;
    } echo;                        /* echo datagram */
    u_int32_t        gateway;        /* gateway address */
    struct
    {
      u_int16_t        __unused;
      u_int16_t        mtu;
    } frag;                        /* path mtu discovery */
  } un;
  #define icmp_id un.echo.icmp_id
  #define icmp_seq un.echo.icmp_seq_num
};

#endif

