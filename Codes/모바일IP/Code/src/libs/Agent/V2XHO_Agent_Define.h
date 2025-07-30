#include <V2XHO_Agent_Include.h>

#define NODE_LIST_SIZE 128
#define NODE_PREFERENCE_LEVEL 3

enum V2XHO_Agent_Type_e
{
    UnknownAgent,
    HomeAgent,
    ForeignAgent
};
enum Agent_Work_List_e
{
    Advertisements,
    Registration_Reply
};
enum V2XHO_Node_State_e
{
    Not_Used_Node,
    Solicitation_To_Agent,
    Registration_Request_To_Agent,
    Registed_And_Route_Agent,
};
struct V2XHO_Agent_Registed_Node_Info_t 
{
    //struct V2XHO_IP_Header_Info_t ip_hdr_info;
    //struct V2XHO_UDP_Header_Info_t udp_hdr_info;
    enum V2XHO_Node_State_e state;
    uint16_t src_port;
    uint16_t dest_port;
    struct in_addr node_address;
    struct in_addr home_agent;
    struct in_addr co_coa_agent;
    enum V2XHO_Registration_Reply_Code_e reply_code;

    struct V2XHO_Agent_Registering_Info_t regist_node_info;
    //received for identification check
    struct V2XHO_0_UDP_Payload_Header_t udp_payload;
};

enum V2XHO_Agent_State_e
{
    Sleep_No_Operation,
    Waiting_Message_From_Any,
    Waiting_Advertisement_From_Home,
    Waiting_Registration_Request_From_Node,
    Waiting_Registration_Request_From_Agent,
    
    Operation_Advertisement_To_Node,
    Operation_Advertisement_To_Agent,
    Operation_Solicitation_Reply_To_Node,
    Operation_Registration_Reply_To_Node,
    Operation_Registration_Reply_To_Agent,

    Connected_With_Node,

    Finish_Agent_State,
};

enum V2XHO_Agent_Communication_State_e
{
    Waiting_Communication,
    Receiver_Mode,
    Sender_Mode,
};
enum V2XHO_Agent_Receiver_State_e
{
    Sleep_Receiver,
    Solicitation_From_Node,
    Advertisement_From_Home,
    Advertisement_From_Foreign,
    Advertisement_From_Agent,
    Registration_Request_From_Node,
    Registration_Request_From_Agent,
};
enum V2XHO_Agent_Sender_State_e
{
    Sleep_Sender,
    Advertisement_To_Node,
    Advertisement_To_Agent,
    Solicitation_Reply_To_Node,
    Registration_Reply_To_Node,
    Registration_Reply_To_Agent,
};
enum V2XHO_Agent_Node_State_e
{
    Waiting_Agent,
    Connection_Directly,
    Connection_With_Foreign,
};


struct V2XHO_Agent_State_t
{
    enum V2XHO_Agent_State_e now;

    enum V2XHO_Agent_Communication_State_e trx;
    enum V2XHO_Agent_Receiver_State_e recv;
    enum V2XHO_Agent_Sender_State_e send;

    int hop_num;
    enum V2XHO_Agent_Node_State_e node;
};

struct V2XHO_Agent_Agent_Info_List_t
{
    enum V2XHO_Agent_Type_e type;
    struct in_addr agent_addr;
    uint16_t *num_addrs;
    struct V2XHO_Router_Addr_es_t *router_addrs[ROUTER_ADDR_ES_SIZE];

    struct V2XHO_ICMP_0_Header_Info_t adv_icmp_info;
    struct V2XHO_1_UDP_Payload_Header_t requset_reply_payload;

    

    uint16_t num_coa_addrs;

};

struct V2XHO_Agent_Info_t
{
    enum V2XHO_Agent_Type_e type;
    char *interface_mobileip;
    int interface_v2xho_index;
    struct sockaddr_in interface_addr_mobileip;
    char *interface_common;
    int interface_common_index;
    struct sockaddr_in interface_addr_common;
    int send_socket;
    int send_socket_common;
    int recv_socket;

    struct V2XHO_ICMP_0_Header_Info_t icmp_adv_info;//TBD:변경 예정

    struct V2XHO_Agent_State_t *agent_state;

    uint16_t *num_agent;
    struct V2XHO_Agent_Agent_Info_List_t agent_info_list[64];
    
    uint16_t *num_nodes;
    struct V2XHO_Agent_Registed_Node_Info_t registed_node_list[NODE_LIST_SIZE];


    struct V2XHO_Th_Task_Info_t task_info;
};
