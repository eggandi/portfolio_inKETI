#include <V2XHO_Common_Include.h>

/*Preprocessor(macro) Function */
#define V2XHO_safefree(ptr, len) if(len > 0){memset(ptr, 0x00, len);} do { free((ptr)); (ptr) = NULL;} while(0)

#define V2XHO_DEBUG_0 printf("[%s][%d]",__func__,__LINE__);
#define V2XHO_DEBUG_1 printf("[%s][%d]\n",__func__,__LINE__);

#define BROADCAST_ADDRESS "255.255.255.255"
#define ROUTER_ADDR_ES_SIZE 128
#define MAX_RECEIVE_BUF 1024 * 4
#define IPV4_ADDR_SIZE 64

enum V2XHO_Preference_Level_e
{
    PREFERENCE_LEVEL_CORRESPHOND_ADDRESS = 1,
    PREFERENCE_LEVEL_CORRESPHOND_SUBNET,
    
    PREFERENCE_LEVEL_HOST_UNKNOW_ADDRESS = 10,
    PREFERENCE_LEVEL_HOST_UNKNOW_SUBNET,
    PREFERENCE_LEVEL_AGENT_HOME_ADDRESS,
    PREFERENCE_LEVEL_AGENT_HOME_SUBNET,
    PREFERENCE_LEVEL_AGENT_FOREIGN_ADDRESS,
    PREFERENCE_LEVEL_AGENT_FOREIGN_SUBNET,
    PREFERENCE_LEVEL_CoCoA_ADDRESS,
    PREFERENCE_LEVEL_CoCoA_SUBNET,
    PREFERENCE_LEVEL_NODE_ADDRESS,
    PREFERENCE_LEVEL_NODE_SUBNET,
    PREFERENCE_LEVEL_TUNNEL_DEST_ADDRESS = 30,
    PREFERENCE_LEVEL_TUNNEL_DEST_SUBNET,
    PREFERENCE_LEVEL_TUNNEL_EoT_ADDRESS,
    PREFERENCE_LEVEL_TUNNEL_EoT_SUBNET,

    PREFERENCE_LEVEL_DEFAULT_ROUTER_INTERFACE_0 = 100,
    PREFERENCE_LEVEL_DEFAULT_ROUTER_INTERFACE_1,

    PREFERENCE_LEVEL_MONITORING_ADDRESS = 0xFFFFFF01,
};

extern struct V2XHO_Time_Tick_t *g_time_tick;
struct	ether_header 
{
	uint8_t ether_dhost[6];
	uint8_t ether_shost[6];
	uint16_t ether_type;
};
struct V2XHO_Socket_Info_t
{
    int socket;
    struct sockaddr_in saddr;
    struct sockaddr_in daddr;
};

enum V2XHO_Ip_Dst_Type_e{
    unicast,
    multicast,
    broadcast,
};

struct V2XHO_Th_Task_Info_t
{
    pthread_mutex_t mutex_id;
    pthread_cond_t cond_id;
    pthread_t th_id;
};
struct V2XHO_IP_Header_Info_t
{
    enum V2XHO_Ip_Dst_Type_e ip_dst_type;
    char *ip_dst_addr;
    char *ip_src_addr;
    uint8_t ip_tos;
    uint16_t ip_id;
    uint16_t ip_frag_off; 
    uint8_t ip_ttl;
    uint8_t ip_protocol;
};

enum V2XHO_ICMP_Header_Code_e
{
    traffic_common = 0,
    traffic_mobileip = 16,
};

struct V2XHO_0_ICMP_Payload_Header_t{
    uint8_t Num_Addrs;
    uint8_t Addr_Entry_Size;
    uint16_t LifeTime;
};

struct V2XHO_extention_0_t//Mobile Agent Advertisement Extention
{
    uint8_t Type;
    uint8_t Length;
    uint16_t Sequence_Number;
    uint16_t Registration_LifeTime;
    struct{
        //uint16_t R:1, B:1, H:1, F:1, M:1, G:1, r:1, T:1, U:1, X:1, I:1, reserved:5;
        uint16_t reserved:5;
        uint16_t I:1;
        uint16_t X:1;
        uint16_t U:1;
        uint16_t T:1;
        uint16_t r:1;
        uint16_t G:1;
        uint16_t M:1;
        uint16_t F:1;
        uint16_t H:1;
        uint16_t B:1;
        uint16_t R:1;
    } flag;  
    uint32_t Care_Of_Addresses[128]; 
};

struct V2XHO_extention_1_t//Prefix-Lengths Extention
{
    uint8_t Type;
    uint8_t Length;
};
struct V2XHO_extention_2_t// One-Byte Padding Extension
{
    uint8_t Type;
};

struct V2XHO_extention_3_t// (32)Mobile-Home Authentication Extension, (33)Mobile-Foreign Authentication Extension, (34)Foreign-Home Authentication Extension
{
    uint8_t Type; // (32 or 33 or 34)
    uint8_t Length;
    uint16_t SPI;
    uint16_t SPI_cont;
    uint32_t Authenticator_length;
    uint8_t *Authenticator;
};

struct V2XHO_ICMP_Checksum_t
{
    uint32_t saddr;
    uint32_t daddr;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t icmp_len;
    uint8_t type;
    uint8_t code;
    uint16_t length;
    uint8_t data[128];
};
struct V2XHO_ICMP_0_Header_Info_t
{
    uint8_t code;
    uint16_t lifetime;
    struct V2XHO_extention_0_t *extention_0;
    struct V2XHO_extention_1_t *extention_1;
    struct V2XHO_extention_1_t *extention_2;
};
struct V2XHO_Router_Addr_es_t
{
    uint32_t Route_Address;
    uint32_t Preference_Level;
    bool used;
};
struct V2XHO_0_UDP_Payload_Header_t
{
    uint8_t type;
    struct{
        uint8_t x:1; //the home agent maintain prior mobility binding 
        uint8_t T:1; // request its home agent toforward to it a copy of broadcast datagrams received by its home agent from the home network.
                        /*Example If B is seted 1, D MUST SET 1*/
        uint8_t r:1; // registering with a co-located careof address
                        // Set 1: decapsulate any datagrams tunneled to this careof address
                        // Set 0: the foreign agent will thus decapsulate arriving datagrams before forwarding them to the mobile node.
        uint8_t G:1; // Minimal encapsulation.
        uint8_t M:1; // Generic Routing Encapsulation.
                        /* M:0, G:0 IPIP tunnel*/
        uint8_t B:1; // Sent as zero; ignored on reception.
        uint8_t D:1; // Foreign agent supports reverse tunneling as specified in [12]
        uint8_t S:1; // Mobility agent supports Registration Revocation as specified in [28]
        
    } flag;                                                  
    uint16_t lifetime; //MAX 1800[sec], Set 0, delete a particular mobility binding
    uint32_t home_address;
    uint32_t home_agent;
    uint32_t care_of_address;
    int64_t identification;
};

enum V2XHO_Registration_Reply_Code_e
{
    Registration_Accepted = 0,
    Registration_Accepted_No_Biding = 1,

    From_Foreign_Reason_Unspecified = 64,
    From_Foreign_Administratively_Prohibited,
    From_Foreign_Insufficient_Resources,
    From_Foreign_Mobile_Node_Failed_Authentication,
    From_Foreign_Home_Agent_Failed_Authentication,
    From_Foreign_Requested_LifeTime_Too_Long,
    From_Foreign_Poorly_Formed_Request,
    From_Foreign_Poorly_Formed_Reply,
    From_Foreign_Requested_Encapsulation_Unavailable,
    From_Foreign_Reserved_And_Unavailable,
    From_Foreign_Invalid_Care_Of_Address,
    From_Foreign_Registration_Timeout,
    From_Foreign_Home_Network_Unreachable_ICMP_error_received,
    From_Foreign_Home_Agent_Host_Unreachable_ICMP_error_received,
    From_Foreign_Home_Agent_Port_Unreachable_ICMP_error_received,
    From_Foreign_Home_Agent_Unreachable_Other_ICMP_error_received,
    From_Foreign_Invalid_Home_Agent_Address,

    From_Home_Reason_Unspecified = 128,
    From_Home_Administratively_Prohibited,
    From_Home_Insufficient_Resources,
    From_Home_Mobile_Node_Failed_Authentication,
    From_Home_Foreign_Agent_Failed_Authentication,
    From_Home_Registration_Identification_Mismatch,
    From_Home_Poorly_Formed_Request,
    From_Home_Too_Many_Simultaneous_Mobility_Bindings,
    From_Home_Unknown_Home_Agent_Addres,
};
struct V2XHO_1_UDP_Payload_Header_t
{
    uint8_t type;
    uint8_t code;                                               
    uint16_t lifetime; //MAX 1800[sec], Set 0, delete a particular mobility binding
    uint32_t home_address;
    uint32_t home_agent;
    int64_t identification;
};
struct V2XHO_UDP_Header_Info_t
{
    uint16_t source;
    uint16_t dest;
    union
    {
        struct
        {
            uint16_t len;
            uint16_t check;
        }normal;
        struct
        {
            uint8_t type;
            uint8_t code;
            uint16_t lifetime;
        }mobileip;    
    }un;
};
struct V2XHO_tick_t{ 
    uint32_t ms1000:8;
    uint32_t ms100:8;
    uint32_t ms10:8;
    uint32_t ms1:8;
};

enum V2XHO_Time_Tick_Precision_e
{
    ms100,
    ms1000,
    ms10,
    ms1,
};

struct V2XHO_Time_Tick_t
{
    enum V2XHO_Time_Tick_Precision_e precision;
    struct itimerspec ts;
    struct V2XHO_tick_t tick;
};

enum V2XHO_DEBUG_Level_e
{
    ADD_DEBUG_MESSAGE,
    ADD_ERROR_MESSAGE,
    ADD_INFO_MESSAGE,
    ADD_LINE,
    ADD_FUNC,
    ADD_TIME,
};

struct V2XHO_IP_fields_t
{
    uint32_t source_address;
    uint32_t destination_address;
    int time_to_live;
};

struct V2XHO_UDP_fields_t
{
    uint32_t source_port;
    uint32_t destination_port;
};

struct V2XHO_Registration_Request_fields_t
{
    int type;
    struct{
        uint8_t x:1; //the home agent maintain prior mobility binding 
        uint8_t T:1; // request its home agent toforward to it a copy of broadcast datagrams received by its home agent from the home network.
                        /*Example If B is seted 1, D MUST SET 1*/
        uint8_t r:1; // registering with a co-located careof address
                        // Set 1: decapsulate any datagrams tunneled to this careof address
                        // Set 0: the foreign agent will thus decapsulate arriving datagrams before forwarding them to the mobile node.
        uint8_t G:1; // Minimal encapsulation.
        uint8_t M:1; // Generic Routing Encapsulation.
                        /* M:0, G:0 IPIP tunnel*/
        uint8_t B:1; // Sent as zero; ignored on reception.
        uint8_t D:1; // Foreign agent supports reverse tunneling as specified in [12]
        uint8_t S:1; // Mobility agent supports Registration Revocation as specified in [28]
        
    } flag;   
    int lifetime;
    uint32_t home_address;
    uint32_t home_agent;
    uint32_t careof_address;
    uint64_t identification;
};

struct V2XHO_Extensions_t
{                                                                                                                                                                                                                                                                                                                                                                                            
    void *extention;
};

struct V2XHO_Agent_Registering_Info_t
{
    struct V2XHO_IP_fields_t IP_fields;
    struct V2XHO_UDP_fields_t UDP_fields;
    struct V2XHO_Registration_Request_fields_t Registration_Request_fields;
    struct V2XHO_Extensions_t Extensions;
};