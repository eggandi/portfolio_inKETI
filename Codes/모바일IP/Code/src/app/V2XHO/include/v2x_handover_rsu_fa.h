#ifndef _V2XHO_RSU_FA_H_
#define _V2XHO_RSU_FA_H_

#define V2XHO_DEFAULT_RSU_FA_OBU_REGISTRATION_MAX 128
#define V2XHO_DEFAULT_RSU_FA_LIFETIME 1800
/**
 * @brief 장치 상태로 값이 일부 중첩 가능
 * 
 */
enum v2xho_equip_state_rsu_fa_e
{
    V2XHO_EQUIP_STATES_RSU_FA_HANDOVER_IDEL = 0,         //0000000000000000
    V2XHO_EQUIP_STATES_RSU_FA_HANDOVER_ACTIVE = 1,       //0000000000000001
    V2XHO_EQUIP_STATES_RSU_FA_RECV_REQ_OBU = 2,          //0000000000000010
    V2XHO_EQUIP_STATES_RSU_FA_RECV_REP_RSU_HA = 4,       //0000000000000100
    // V2XHO_EQUIP_STATES_RSU_HA_RECV_REQ_FA = 16,            //0000000000010000
    // V2XHO_EQUIP_STATES_RSU_HA_RESOLVED  = 32,              //0000000000100000
    // V2XHO_EQUIP_STATES_RSU_HA_ = 64,    //0000000001000000
    // V2XHO_EQUIP_STATES_RSU_HA_ = 192,   //0000000011000000
    // V2XHO_EQUIP_STATES_RSU_HA_ = 256,   //0000000100000000
    // V2XHO_EQUIP_STATES_RSU_HA_ = 768,   //0000001100000000
    // V2XHO_EQUIP_STATES_RSU_HA_ = 1024,  //0000010000000000
    // V2XHO_EQUIP_STATES_RSU_HA_ = 3072,  //0000110000000000
    // V2XHO_EQUIP_STATES_RSU_HA_ = 4096,  //0001000000000000
    // V2XHO_EQUIP_STATES_RSU_HA_ = 8192,  //0010000000000000
    // V2XHO_EQUIP_STATES_RSU_HA_ = 24576, //0110000000000000
};

/**
 * @brief enum v2xho_equip_state_obu_e으로 확인 v2xho_equip_state_obu_t s_b = 0b01; (enum v2xho_equip_state_obu_e)(s_b.connecting * 2) == V2XHO_EQUIP_STATES_RSU_HA_CONNECTING_HA
 * 
 */
typedef struct
{
    uint32_t active:1;
    uint32_t recv_req:1;
    uint32_t recv_rep:1;
    uint32_t resolved:29;
} __attribute__((__packed__)) v2xho_equip_state_rsu_fa_t;

typedef struct
{
    uint32_t source_address;//home_address or co-located address
    uint32_t destination_addess;//home agent address
    uint32_t source_port;
    uint32_t destination_port;
    int64_t identification;
    uint16_t lifetime;
    uint16_t remaining_lifetime;
} __attribute__((__packed__)) v2xho_obu_fa_registration_info_t;

typedef struct
{
    bool isconnected;
    bool isrouted;
    uint8_t obu_mac[6];

    v2xho_obu_fa_registration_info_t obu_info;
    v2xho_registration_request_payload_t req_payload;
    v2xho_registration_reply_payload_t rep_payload;
} __attribute__((__packed__)) v2xho_obu_fa_registration_info_list_t;

struct v2xho_rsu_fa_registraion_t
{
    int registered_obu_num;
    v2xho_obu_fa_registration_info_list_t reg_obu_fa[V2XHO_DEFAULT_RSU_FA_OBU_REGISTRATION_MAX];
};


#endif

