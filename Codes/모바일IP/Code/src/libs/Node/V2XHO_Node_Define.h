#include <V2XHO_Node_Include.h>

#define MAX_AGENT_INFO_LIST 256

enum V2XHO_Node_Type_e
{
    MobileNode,
    CorresponedentNode,
    PeerNode,
    HostNode,
    EtcNode,
};

enum Node_Receiver_Job_List_e
{
    Advertisement,
    Reply,
    Datagram
};

enum V2XHO_Node_Receive_Message_Info_List_e
{
    Message_Error,
    Advertisement_Message_Received_NO,
    Advertisement_Message_Received_OK,
};

enum V2XHO_Node_Communication_State_e
{
    Waiting_Communication,
    Receiver_Mode,
    Sender_Mode,
    Router_Mode,
};
enum V2XHO_Node_Receiver_State_e
{
    Sleep_Receiver,
    Advertisement_From_Agent,
    Advertisement_From_Home,
    Advertisement_From_Foreign,
    Registration_Reply_From_Home,
    Registration_Reply_From_Forein,
};
enum V2XHO_Node_Sender_State_e
{
    Sleep_Sender,
    Solicitation_To_Agent,
    Registration_Request_to_Home,
    Registration_Request_to_Foreign,
    Registration_Request_to_Agent,
};
enum V2XHO_Node_Home_Agent_State_e
{
    Waiting_Agent,
    Connection_Directly_Home,
    Connection_Through_Foreign,
};

enum V2XHO_Node_Foreign_Agent_State_e
{
    No_Connection_With_Foreign,
    Connection_With_Foreign,
    Connected_Foreign_Alone,
};
enum V2XHO_Node_State_e
{
    Sleep_No_Operation,
    Waiting_Advertisement_From_Agent,
    Waiting_Solicitation_Reply_From_Agent,
    Waiting_Registration_Reply_From_Agent,
    Waiting_Registration_Reply_From_Home,
    Waiting_Registration_Reply_From_Foreign,
    
    Operation_Solicitation_Request_To_Agent,
    Operation_Registration_Request_To_Agent,
    Operation_Registration_Request_To_Home,
    Operation_Registration_Request_To_Foreign,

    Accepted_Registration_From_Home,
    Accepted_Registration_From_Foreign,

    Connected_With_Home,
    Connected_With_Foreign,

    Finish_Node_State,
};
struct V2XHO_Node_State_t
{
    enum V2XHO_Node_State_e now;

    enum V2XHO_Node_Communication_State_e trx;
    enum V2XHO_Node_Receiver_State_e recv;
    enum V2XHO_Node_Sender_State_e send;

    enum V2XHO_Node_Home_Agent_State_e home;
    enum V2XHO_Node_Foreign_Agent_State_e foreign;
};
struct V2XHO_Node_Home_Address_Info_t
{
    struct in_addr home_agent_addr;
    struct in_addr home_addr;
    uint32_t *adv_timer_left;
};
struct V2XHO_Node_CoA_Address_Info_t
{
    struct in_addr foreign_agent_addr;
    struct in_addr co_coa_addr;
    uint32_t *adv_timer_left;
};
struct V2XHO_Node_Advertised_Agent_Info_t
{
    bool home_agent;
    bool foreign_agent;
    
    struct in_addr agent_addr;
    bool router_applied;
    bool home_address_applied;
    bool co_coa_address_applied;

    struct in_addr home_addr;
    struct in_addr co_coa_addr;

    uint64_t last_recv_adv_time;
    uint16_t num_addrs;
    struct V2XHO_0_ICMP_Payload_Header_t icmp_payload;
    struct V2XHO_Router_Addr_es_t *router_addrs[ROUTER_ADDR_ES_SIZE];
    struct V2XHO_extention_0_t extention_0;
    struct V2XHO_extention_1_t extention_1;

    //sended for identification check
    struct V2XHO_0_UDP_Payload_Header_t udp_send_payload;
    //receiver for identification check
    struct V2XHO_1_UDP_Payload_Header_t udp_recv_payload;
};

struct V2XHO_Node_Info_t
{
    enum V2XHO_Node_Type_e type;
    char *interface;
    struct sockaddr_in interface_addr;
    int send_socket;
    int recv_socket;

    struct V2XHO_Node_State_t *node_state;

    struct V2XHO_Node_Home_Address_Info_t home_address_info;
    struct V2XHO_Node_CoA_Address_Info_t coa_address_info;

    struct V2XHO_ICMP_0_Header_Info_t icmp_adv_info;

    uint8_t *num_agent;
    struct V2XHO_Node_Advertised_Agent_Info_t agent_list[MAX_AGENT_INFO_LIST];

    uint16_t *num_addrs;
    struct V2XHO_Router_Addr_es_t *router_addrs[ROUTER_ADDR_ES_SIZE];

    int wait_adv_sec;
    struct V2XHO_Th_Task_Info_t task_info;
};

