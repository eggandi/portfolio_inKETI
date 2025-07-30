/*
 * V2XHO_Node.c
 *
 *  Created on: 2023. 10. 1.
 *      Author: Sung Dong-Gyu
 */

#include <V2XHO_node.h>

static enum V2XHO_Registration_Reply_Code_e f_e_V2XHO_Receiver_Parser_UDP_Registration_Reply(char *buf, struct V2XHO_Node_Info_t *node_info);
static uint32_t* f_u32_V2XHO_Receiver_Parser_ICMP_Advertisement(char *buf, struct V2XHO_Node_Info_t *node_info);
static struct iphdr *f_s_V2XHO_Receiver_Parser_IPHDR(char *buf);
static struct icmphdr *f_s_V2XHO_Receiver_Parser_ICMP(char *buf);
static struct udphdr *f_s_V2XHO_Receiver_Parser_UDP(char *buf);

static int f_i_V2XHO_Node_Receive_Do(struct V2XHO_Node_Info_t *node_info, int timer);
static int f_i_V2XHO_Node_Send_Do(struct V2XHO_Node_Info_t *node_info);

/* 
Brief: Node의 기능을 수행하는 테스크
Parameter[In]
    void *data:struct V2XHO_Node_Info_t를 void로 타입캐스팅해 전달 받음.
Parameter[Out]
    NULL
 */
void* F_th_V2XHO_Node_Task_Doing(void *data)
{
    struct V2XHO_Node_Info_t *node_info = (struct V2XHO_Node_Info_t*)data;
    int ret;
    ret = F_i_V2XHO_Router_Hendler_Open(&rth);
    
    if(ret<0)
    {
        printf("\n");
    }
    /*Send/Receive Socket Initial*/
    node_info->send_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    node_info->recv_socket = socket (AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    int on = 1;
    setsockopt(node_info->send_socket , IPPROTO_IP, IP_HDRINCL, (const char*)&on, sizeof (on));
    setsockopt(node_info->send_socket , SOL_SOCKET, SO_BINDTODEVICE, node_info->interface, strlen(node_info->interface));
    setsockopt(node_info->recv_socket , IPPROTO_IP, IP_HDRINCL, (const char*)&on, sizeof (on));
    setsockopt(node_info->recv_socket , SOL_SOCKET, SO_BINDTODEVICE, node_info->interface, strlen(node_info->interface));
    int optval[2] = {1, 0};
    setsockopt (node_info->send_socket , SOL_SOCKET, SO_LINGER, &optval, sizeof(optval));
    setsockopt (node_info->recv_socket , SOL_SOCKET, SO_LINGER, &optval, sizeof(optval));

    struct ifreq ifr;
    struct sockaddr_ll saddrll;
    
    memcpy(ifr.ifr_name, node_info->interface, strlen(node_info->interface));
    ifr.ifr_name[strlen(node_info->interface)] = 0;
    ioctl(node_info->recv_socket, SIOCGIFINDEX, &ifr);

    memset((void*)&saddrll, 0, sizeof(saddrll));
    saddrll.sll_family = AF_PACKET; 
    saddrll.sll_protocol = htons(ETH_P_IP);  
    saddrll.sll_ifindex = ifr.ifr_ifindex;
    bind(node_info->recv_socket, (struct sockaddr*)&saddrll, sizeof(saddrll));

    char *interface_ip = F_p_V2XHO_Get_InterfaceIP(node_info->interface);
    node_info->interface_addr.sin_addr.s_addr = inet_addr(interface_ip);
    printf("interface name:%s, interface ifip:%s\n", node_info->interface, interface_ip);
    char cmd[128];
    memset(cmd, 0x00, 128);
    sprintf(cmd, "ip address flush dev %s", node_info->interface);
    system(cmd);
    sleep(1);   
    memset(cmd, 0x00, 128);
    sprintf(cmd, "ip address add %s/32 dev %s", interface_ip, node_info->interface);
    system(cmd);
    sleep(1);  
    memset(cmd, 0x00, 128);
    sprintf(cmd, "ip route flush dev %s", node_info->interface);
    system(cmd);
    sleep(1);
    memset(cmd, 0x00, 128);
    sprintf(cmd, "ip route add default via %s dev %s metric 1024", interface_ip, node_info->interface);
    system(cmd);
    sleep(1);
    V2XHO_safefree(interface_ip, 0);

    struct V2XHO_IProuter_Route_List_info_IPv4_t route_list[64];
    node_info->num_addrs = malloc(sizeof(uint16_t));
    int route_list_num = 0;
    ret = F_i_V2XHO_IProuter_Route_Get_List(node_info->interface, route_list, &route_list_num);
    for(int i = 0; i < route_list_num; i++)
    {
        struct V2XHO_Router_Addr_es_t router_addrs;
        router_addrs.Route_Address = route_list[i].gateway_addr.s_addr;
        router_addrs.Preference_Level = 0;
        ret = F_i_V2XHO_Node_ICMP_Router_addr_Add(node_info->router_addrs, node_info->num_addrs, &router_addrs);
    }

    node_info->num_agent = malloc(sizeof(int8_t));
    *node_info->num_agent = 0;
    //node_info->wait_adv_sec = 5;//when node mode start, it wating the advertisement message from agent while 5 second.
    node_info->node_state = malloc(sizeof(struct V2XHO_Node_State_t));
    memset(node_info->node_state ,0x00, sizeof(struct V2XHO_Node_State_t));
    struct V2XHO_Node_State_t *state = node_info->node_state;
    state->now = Waiting_Advertisement_From_Agent;
    bool wait_send = true;

    while(1)
    {
        if(state->now != Waiting_Advertisement_From_Agent)
        {
            //printf("[%s][%d] Node State:%d\n", __func__,__LINE__, state->now);
        }
        switch(state->now)
        {
            case Waiting_Advertisement_From_Agent:
            {
                state->trx = Receiver_Mode;
                state->send = Sleep_Sender;
                ret = f_i_V2XHO_Node_Receive_Do(node_info, 25);
                break;
            }
            case Waiting_Solicitation_Reply_From_Agent:
            {
                break;
            }
            case Waiting_Registration_Reply_From_Agent:
            {
                state->trx = Receiver_Mode;
                state->send = Sleep_Sender;
                ret = f_i_V2XHO_Node_Receive_Do(node_info, 25);
                break;
            }
            case Waiting_Registration_Reply_From_Home:
            {
                state->trx = Receiver_Mode;
                state->send = Sleep_Sender;
                ret = f_i_V2XHO_Node_Receive_Do(node_info, 25);
                break;
            }
            case Waiting_Registration_Reply_From_Foreign:
            {
                state->trx = Receiver_Mode;
                state->send = Sleep_Sender;
                ret = f_i_V2XHO_Node_Receive_Do(node_info, 25);
                break;
            }
            case Operation_Solicitation_Request_To_Agent:
            {
                state->trx = Sender_Mode;
                state->recv = Sleep_Receiver;
                state->send = Solicitation_To_Agent;
                ret = f_i_V2XHO_Node_Send_Do(node_info);
                if(ret >= 0)
                    state->now = Waiting_Advertisement_From_Agent;
                break;
            }
            case Operation_Registration_Request_To_Home:
            {
                state->trx = Sender_Mode;
                state->recv = Sleep_Receiver;
                state->send = Registration_Request_to_Home;
                ret = f_i_V2XHO_Node_Send_Do(node_info);
                if(ret >= 0)
                    state->now = Waiting_Registration_Reply_From_Home;
                break;
            }
            case Operation_Registration_Request_To_Foreign:
            {
                state->trx = Sender_Mode;
                state->recv = Sleep_Receiver;
                state->send = Registration_Request_to_Foreign;
                ret = f_i_V2XHO_Node_Send_Do(node_info);
                if(ret >= 0)
                    state->now = Waiting_Registration_Reply_From_Foreign;
                break;
            }
            case Accepted_Registration_From_Home:
            {
                state->trx = Router_Mode;
                state->recv = Sleep_Receiver;
                state->send = Sleep_Sender;
                switch(state->home)
                {
                    case Connection_Directly_Home:  
                        state->now = Connected_With_Home;
                        state->foreign = Connection_With_Foreign;
                        break;
                    case Connection_Through_Foreign:
                        state->now = Connected_With_Foreign;
                        break;
                    default:
                        break;
                }
                break; 
            }
            case Accepted_Registration_From_Foreign:
            {
                state->trx = Router_Mode;
                state->recv = Sleep_Receiver;
                state->send = Sleep_Sender;
                switch(state->foreign)
                {
                    case Connection_With_Foreign:
                        if(state->home == Waiting_Agent || state->home == Connection_Through_Foreign)
                        {
                            state->foreign = Connected_Foreign_Alone;
                            state->now = Connected_With_Foreign;
                            break;
                        }else{
                            state->now = Connected_With_Foreign;
                        }
                        break;
                    default:
                        break;
                } 
                break;
            }
            case Connected_With_Home:
            case Connected_With_Foreign:
            {
                state->trx = Router_Mode;
                state->send = Sleep_Sender;
                state->recv = Sleep_Receiver;
                ret = f_i_V2XHO_Node_Receive_Do(node_info, 25);
                break;
            }
            default:
                break;        
        }
        
        if(ret >= 0)
        {
            if(state->now != Connected_With_Home || state->now != Connected_With_Foreign)
            {
                switch(state->recv)
                {
                    case Sleep_Receiver:
                        break;
                    case Advertisement_From_Agent:
                    {
                        state->now = Operation_Registration_Request_To_Home;
                        break;
                    }
                    case Advertisement_From_Home:
                    {
                        state->now = Operation_Registration_Request_To_Home;
                        break;
                    }
                    case Advertisement_From_Foreign:
                    {
                        state->now = Operation_Registration_Request_To_Foreign;
                        break;
                    }
                    case Registration_Reply_From_Home:
                    case Registration_Reply_From_Forein:
                    default:
                        break;
                }

            }
            
        }else{
            switch(state->now)
            {
                case Waiting_Solicitation_Reply_From_Agent:
                case Waiting_Registration_Reply_From_Agent:
                case Waiting_Registration_Reply_From_Home:
                case Waiting_Registration_Reply_From_Foreign:
                case Connected_With_Home:
                case Connected_With_Foreign:
                    break;
                case Waiting_Advertisement_From_Agent:
                default:
                {
                    switch(wait_send)
                    {
                        case false:
                        {
                            if(g_time_tick->tick.ms100 == 1)
                            {
                                state->now = Operation_Solicitation_Request_To_Agent;
                                wait_send = true;
                            }
                            break;
                        }
                        case true:
                        {
                            if(g_time_tick->tick.ms100 != 1)
                            {
                                wait_send = false;
                            }
                            break;
                        }
                    }
                }
            }
            
        }
    }
    close(node_info->send_socket);
    F_v_V2XHO_Router_Hendler_Close(&rth);

}
/* 
Brief: Node의 Tx 기능을 수행하는 Function
Parameter[In]
    struct V2XHO_Node_Info_t *node_info:Node의 정보를 저장한 구조체
Parameter[Out]
    int error_code: 0 정상
                   <0 에러
 */
int f_i_V2XHO_Node_Send_Do(struct V2XHO_Node_Info_t *node_info)
{
    struct V2XHO_Node_State_t *state = node_info->node_state;
    struct V2XHO_IP_Header_Info_t ip_hdr_info;
    int ret;
    switch(state->send)
    {
        case Solicitation_To_Agent:
        {
            ip_hdr_info.ip_dst_type = multicast;
            ip_hdr_info.ip_tos = 0; 
            ip_hdr_info.ip_id = 0;
            ip_hdr_info.ip_src_addr = "0.0.0.0";
            ip_hdr_info.ip_dst_addr = "224.0.0.11";
            ip_hdr_info.ip_ttl = 1;
            ret = F_i_V2XHO_Node_Solicitation_Do(node_info, ip_hdr_info);

            break;
        }
        case Registration_Request_to_Agent:
        {
            ip_hdr_info.ip_dst_type = multicast;
            ip_hdr_info.ip_tos = 0; 
            ip_hdr_info.ip_id = 0;
            ip_hdr_info.ip_src_addr = "0.0.0.0";
            ip_hdr_info.ip_dst_addr = "224.0.0.11";
            ret = F_i_V2XHO_Node_Registration_Request_Do(node_info, ip_hdr_info);
            break;
        }
        case Registration_Request_to_Home:
        {
            ip_hdr_info.ip_dst_type = unicast;
            ip_hdr_info.ip_tos = 0; 
            ip_hdr_info.ip_id = 0;

            char *ip_address = calloc(IPV4_ADDR_SIZE, sizeof(char));
            int prefix;
            char *gateway = calloc(IPV4_ADDR_SIZE, sizeof(char)); 
            bool local_gateway = false;
            char *interface = node_info->interface;
            enum v2xho_util_router_metric_state_e metric;
            char *table = NULL;
            bool non_forecd = false;
            struct in_addr subnet;
            prefix = 24;
            metric = 100; 
            uint8_t *num_agent = node_info->num_agent;
            for(int i = 0; i < *num_agent; i++)
            {
                if(node_info->agent_list[i].home_agent == true)
                {
                    if(node_info->agent_list[i].router_applied == false)
                    {
                        subnet.s_addr = node_info->agent_list[i].agent_addr.s_addr & 0x00FFFFFF;
                        memcpy(ip_address, inet_ntoa(subnet), IPV4_ADDR_SIZE);
                        memcpy(gateway, inet_ntoa(node_info->agent_list[i].agent_addr), IPV4_ADDR_SIZE);
                        ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, interface, &metric, table, non_forecd);                                                             
                        if(ret == -1  || ret == 0)
                            node_info->agent_list[i].router_applied = true;
                    }  
                    if(node_info->agent_list[i].home_address_applied == false)
                    {   
                        memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                        if(subnet.s_addr - node_info->interface_addr.sin_addr.s_addr == 0)
                        {
                            node_info->home_address_info.home_addr = node_info->interface_addr.sin_addr;
                            node_info->agent_list[i].home_addr = node_info->interface_addr.sin_addr;
                        }else
                        {
                            node_info->home_address_info.home_addr.s_addr = subnet.s_addr + 0x01000000;
                            node_info->agent_list[i].home_addr = node_info->home_address_info.home_addr;
                        }
                        memcpy(ip_address, inet_ntoa(node_info->home_address_info.home_addr), IPV4_ADDR_SIZE);
                        prefix = 32;
                        ret = F_i_V2XHO_IProuter_Address_Set(node_info->interface, ip_address, &prefix);
                        printf("ret:%d\n", ret);
                        if(ret == -1 || ret == 0)
                            node_info->agent_list[i].home_address_applied = true;
                    }
                    V2XHO_safefree(ip_address, IPV4_ADDR_SIZE);
                    V2XHO_safefree(gateway, IPV4_ADDR_SIZE);
                    ip_hdr_info.ip_src_addr = calloc(64, sizeof(uint8_t));
                    ip_hdr_info.ip_dst_addr = calloc(64, sizeof(uint8_t));
                    memcpy(ip_hdr_info.ip_src_addr, inet_ntoa(node_info->home_address_info.home_addr), 64);
                    memcpy(ip_hdr_info.ip_dst_addr, inet_ntoa(node_info->agent_list[i].agent_addr), 64);
                    ret = F_i_V2XHO_Node_Registration_Request_Do(node_info, ip_hdr_info);
                    V2XHO_safefree(ip_hdr_info.ip_src_addr, 64);
                    V2XHO_safefree(ip_hdr_info.ip_dst_addr, 64);
                }
            }
            break;
        }
        case Registration_Request_to_Foreign:
        {
            
            ip_hdr_info.ip_dst_type = unicast;
            ip_hdr_info.ip_tos = 0; 
            ip_hdr_info.ip_id = 0;
            ip_hdr_info.ip_src_addr = calloc(64, sizeof(uint8_t));
            char *ip_address = calloc(IPV4_ADDR_SIZE, sizeof(char));
            int prefix;
            char *gateway = calloc(IPV4_ADDR_SIZE, sizeof(char)); 
            bool local_gateway = false;
            char *interface = node_info->interface;
            enum v2xho_util_router_metric_state_e metric;
            char *table = NULL;
            bool non_forecd = false;
            struct in_addr subnet;
            prefix = 24;
            metric = 50; 
            uint8_t *num_agent = node_info->num_agent;
            for(int i = 0; i < *num_agent; i++)
            {
                if(node_info->agent_list[i].foreign_agent  == true)
                {
                    if(node_info->agent_list[i].router_applied == false)
                    {
                        subnet.s_addr = node_info->agent_list[i].agent_addr.s_addr & 0x00FFFFFF;
                        memcpy(ip_address, inet_ntoa(subnet), IPV4_ADDR_SIZE);
                        memcpy(gateway, inet_ntoa(node_info->agent_list[i].agent_addr), IPV4_ADDR_SIZE);
                        ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, interface, &metric, table, non_forecd);                                                             
                        if(ret == -1  || ret == 0)
                            node_info->agent_list[i].router_applied = true;
                    }  
                    if(node_info->agent_list[i].co_coa_address_applied == false)
                    {   
                        memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                        if(subnet.s_addr - node_info->interface_addr.sin_addr.s_addr == 0)
                        {
                            node_info->coa_address_info.foreign_agent_addr = node_info->interface_addr.sin_addr;
                            node_info->agent_list[i].co_coa_addr = node_info->interface_addr.sin_addr;
                        }else
                        {
                            node_info->coa_address_info.co_coa_addr.s_addr = subnet.s_addr + 0x01000000;
                            node_info->agent_list[i].co_coa_addr = node_info->coa_address_info.co_coa_addr;
                        }
                        memcpy(ip_address, inet_ntoa(node_info->agent_list[i].co_coa_addr), IPV4_ADDR_SIZE);
                        prefix = 32;
                        ret = F_i_V2XHO_IProuter_Address_Set(node_info->interface, ip_address, &prefix);
                        printf("ret:%d\n", ret);
                        if(ret == -1 || ret == 0)
                            node_info->agent_list[i].co_coa_address_applied = true;
                    }
                    V2XHO_safefree(ip_address, IPV4_ADDR_SIZE);
                    V2XHO_safefree(gateway, IPV4_ADDR_SIZE);
                    ip_hdr_info.ip_src_addr = calloc(64, sizeof(uint8_t));
                    memcpy(ip_hdr_info.ip_src_addr, inet_ntoa(node_info->agent_list[i].co_coa_addr), 64);
                    ip_hdr_info.ip_dst_addr = inet_ntoa(node_info->agent_list[i].agent_addr);
                    ret = F_i_V2XHO_Node_Registration_Request_Do(node_info, ip_hdr_info);
                    V2XHO_safefree(ip_hdr_info.ip_src_addr, 64);
                }
            }
            break;
        }
        default: break;
    }
    return ret;
}

/* 
Brief: Node의 Rx 기능을 수행하는 Function
Parameter[In]
    struct V2XHO_Node_Info_t *node_info:Node의 정보를 저장한 구조체
    int recv_timer:socket의 Block 제한 시간 
Parameter[Out]
    int error_code: 0 정상
                   <0 에러
 */
int f_i_V2XHO_Node_Receive_Do(struct V2XHO_Node_Info_t *node_info, int recv_timer)
{
    ssize_t recv_len = 0;
    char buf[1024];
    struct V2XHO_Node_State_t *state = node_info->node_state;
    switch(state->now)
    {
        case Connected_With_Home:
        case Connected_With_Foreign:
        case Waiting_Advertisement_From_Agent:
        case Waiting_Registration_Reply_From_Home:
        case Waiting_Registration_Reply_From_Foreign:
        {
            struct sockaddr_ll saddrll;
            socklen_t src_addr_len = sizeof(saddrll);
            struct timeval tv_timeo = { (int)(recv_timer/1e6), (recv_timer % 1000) * 1000}; 
            setsockopt(node_info->recv_socket, SOL_SOCKET, SO_RCVTIMEO, &tv_timeo, sizeof(tv_timeo));
            recv_len = recvfrom(node_info->recv_socket, buf, 1024, 0, (struct sockaddr *)&(saddrll), &src_addr_len);
            if(recv_len > 0)
            {
                switch(saddrll.sll_pkttype)
                {
                    case PACKET_HOST:
                    case PACKET_MULTICAST:
                    {
                        struct iphdr *ip = f_s_V2XHO_Receiver_Parser_IPHDR(buf);
                        if(ip)
                        {
                            switch(ip->protocol)
                            {
                                case 1://ICMP
                                case 2://IGMP
                                {
                                    struct icmphdr *icmp = f_s_V2XHO_Receiver_Parser_ICMP(buf);
                                    if(icmp)
                                    {
                                        switch(icmp->type)
                                        {
                                            case ICMP_NET_ANO: //9
                                            { 
                                                struct V2XHO_extention_0_t *ret = (struct V2XHO_extention_0_t *)f_u32_V2XHO_Receiver_Parser_ICMP_Advertisement(buf, node_info);
                                                if(ret)
                                                {   
                                                    if(ret->flag.H == 1)
                                                            state->recv = Advertisement_From_Home;
                                                    if(ret->flag.F == 1)
                                                            state->recv = Advertisement_From_Foreign;                                          
                                                    return 0;
                                                }
                                                break;
                                            }
                                        default:break;
                                        }
                                    }
                                    break;
                                }
                                case 17://UDP
                                {
                                    struct udphdr *udp = f_s_V2XHO_Receiver_Parser_UDP(buf);
                                    if(udp)
                                    {
                                        enum V2XHO_Registration_Reply_Code_e reply_code = f_e_V2XHO_Receiver_Parser_UDP_Registration_Reply(buf, node_info);
                                        if(reply_code >= 0)
                                        {
                                            switch(reply_code)
                                            {
                                                case Registration_Accepted:
                                                case Registration_Accepted_No_Biding:
                                                {         
                                                    break;
                                                }
                                                default: break;
                                            }
                                            return 0;
                                        }
                                    }
                                    break;
                                }
                                default:
                                    break;
                            }                              
                        }
                    }
                    default: 
                        break;
                }
            }
            return -1;
            break;
        }
        default:
            break;
    }
    return -1;
}
/* 
Brief: f_i_V2XHO_Node_Receive_Do을 통해 수신한 메시지 중 Registration Reply 메시지를 파싱
Parameter[In]
    char *buf:수신한 메시지 패킷(이더넷 로우 패킷)
    struct V2XHO_Node_Info_t *node_info:Node의 정보를 저장한 구조체
Parameter[Out]
    enum V2XHO_Registration_Reply_Code_e : Agent로부터 수신한 Registration Reply 메시지의 code
 */
enum V2XHO_Registration_Reply_Code_e f_e_V2XHO_Receiver_Parser_UDP_Registration_Reply(char *buf, struct V2XHO_Node_Info_t *node_info)
{
    if(buf)
    {
        uint32_t pointer_now = sizeof(struct ether_header);
        struct iphdr *ip = (struct iphdr *)(buf + pointer_now);
        pointer_now = pointer_now + sizeof(struct iphdr);
        struct udphdr *udp = (struct udphdr *)(buf + pointer_now);
        uint16_t ip_len = htons(ip->tot_len);
        uint16_t udp_len = htons(udp->len);
        int ret;
        if(ip_len - sizeof(struct iphdr) > udp_len)
        {
            uint8_t *iphdr_option_len = (uint8_t *)(buf + pointer_now + sizeof(uint8_t));
            //printf("iphdr heve extenion field:len:%d\n", *iphdr_option_len);
            pointer_now += *iphdr_option_len;
            udp = (struct udphdr *)(buf + pointer_now);
            if(ip->tot_len == pointer_now + *iphdr_option_len + udp_len)
            {
                printf("packet_error!\n");
                return -1;
            }
        }
        pointer_now += sizeof(struct udphdr);
        struct V2XHO_1_UDP_Payload_Header_t *udp_payload = (struct V2XHO_1_UDP_Payload_Header_t *)(buf + pointer_now);
        if(udp_payload->type == 3)
        {
            switch((enum V2XHO_Registration_Reply_Code_e)udp_payload->code)
            {
                case Registration_Accepted:
                case Registration_Accepted_No_Biding:
                {
                    if(node_info->home_address_info.home_agent_addr.s_addr == 0)
                    {
                        node_info->home_address_info.home_agent_addr.s_addr = udp_payload->home_agent;
                        node_info->home_address_info.home_addr.s_addr = (node_info->home_address_info.home_agent_addr.s_addr & 0x00FFFFFF) + 0x01000000;
                        char *home_address = malloc(IPV4_ADDR_SIZE);
                        memcpy(home_address, inet_ntoa(node_info->home_address_info.home_addr), IPV4_ADDR_SIZE);
                        printf("node_info->home_address_info.home_addr.s_addr:%08X\n", node_info->home_address_info.home_addr.s_addr);
                        int prefix = 32;
                        ret = F_i_V2XHO_IProuter_Address_Set(node_info->interface, home_address, &prefix);
                        V2XHO_safefree(home_address, IPV4_ADDR_SIZE);
                    }
                    struct V2XHO_Node_State_t *state = (struct V2XHO_Node_State_t *)node_info->node_state;
                    switch(state->now)
                    {
                        case Waiting_Registration_Reply_From_Home:
                        case Waiting_Registration_Reply_From_Foreign:
                        case Connected_With_Home:
                        case Connected_With_Foreign:
                        {
                            uint8_t *num_agent = node_info->num_agent;
                            for(int i = 0; i < *num_agent; i++)
                            {
                                if(node_info->agent_list[i].agent_addr.s_addr == ip->saddr)
                                {
                                    if(node_info->agent_list[i].home_agent)
                                    {
                                        state->now = Accepted_Registration_From_Home;
                                        state->home = Connection_Directly_Home;
                                        state->recv = Registration_Reply_From_Home;
                                        
                                        char *ip_address = calloc(IPV4_ADDR_SIZE, sizeof(char));
                                        int prefix;
                                        char *gateway = calloc(IPV4_ADDR_SIZE, sizeof(char)); 
                                        bool local_gateway = false;
                                        char *interface = node_info->interface;
                                        enum v2xho_util_router_metric_state_e metric;
                                        char *table = NULL;
                                        bool non_forecd = false;
                                        struct in_addr gateway_addr, dst_addr;
                                        struct V2XHO_IProuter_Route_List_info_IPv4_t route_list[MAX_ROUTE_LIST_NUM];
                                        int route_list_num = 0;
                                        ret = F_i_V2XHO_IProuter_Route_Get_List(node_info->interface, route_list, &route_list_num);
                                        struct V2XHO_extention_0_t *extention_0 = &(node_info->agent_list[i].extention_0);
                                        uint16_t coa_num = (extention_0->Length - 16)/4;
                                        
                                        for(uint16_t j = 0; j < node_info->agent_list[i].num_addrs; j++)
                                        {
                                            for(int k = 0; k < route_list_num; k++)
                                            { 
                                                if((node_info->agent_list[i].router_addrs[j]->Route_Address) == route_list[k].gateway_addr.s_addr \
                                                || node_info->agent_list[i].router_addrs[j]->Route_Address == node_info->agent_list[i].home_addr.s_addr \
                                                || node_info->agent_list[i].router_addrs[j]->Route_Address == 0)
                                                {
                                                    break;
                                                }else if(k == route_list_num - 1)
                                                {   
                                                    if(coa_num > 0)
                                                    {
                                                        for(uint16_t t = 0; t < coa_num; t++)
                                                        {   
                                                            if(extention_0->Care_Of_Addresses[t] == node_info->agent_list[i].router_addrs[j]->Route_Address \
                                                            || (extention_0->Care_Of_Addresses[t] & 0x00FFFFFF) == (node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF))
                                                            {
                                                                if(((node_info->agent_list[i].router_addrs[j]->Route_Address) & 0xFF000000) == 0x00000000)
                                                                {
                                                                    prefix = 24;
                                                                }else{
                                                                    prefix = 32;
                                                                }
                                                                dst_addr.s_addr = node_info->agent_list[i].router_addrs[j]->Route_Address;
                                                                gateway_addr.s_addr = extention_0->Care_Of_Addresses[t];
                                                                memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                                                                memset(gateway, 0x00, IPV4_ADDR_SIZE);
                                                                memcpy(ip_address, inet_ntoa(dst_addr), IPV4_ADDR_SIZE);
                                                                memcpy(gateway, inet_ntoa(gateway_addr), IPV4_ADDR_SIZE);
                                                                ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, interface, &metric, table, non_forecd); 
                                                                break;
                                                            }else{
                                                                if(((node_info->agent_list[i].router_addrs[j]->Route_Address) & 0xFF000000) == 0x00000000)
                                                                {
                                                                    gateway_addr.s_addr = node_info->home_address_info.home_agent_addr.s_addr;
                                                                    dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address;
                                                                    prefix = 24;
                                                                    metric = 100; 
                                                                }else if((node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF) == (node_info->home_address_info.home_agent_addr.s_addr & 0x00FFFFFF))
                                                                {
                                                                    gateway_addr.s_addr = node_info->home_address_info.home_agent_addr.s_addr;
                                                                    dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address;
                                                                    prefix = 32;
                                                                    metric = 99; 
                                                                }else{
                                                                    gateway_addr.s_addr = ip->saddr;
                                                                    dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF;
                                                                    prefix = 24;
                                                                    metric = 98; 
                                                                }
                                                                memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                                                                memset(gateway, 0x00, IPV4_ADDR_SIZE);
                                                                memcpy(ip_address, inet_ntoa(dst_addr), IPV4_ADDR_SIZE);
                                                                memcpy(gateway, inet_ntoa(gateway_addr), IPV4_ADDR_SIZE);
                                                                
                                                                ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, interface, &metric, table, non_forecd); 
                                                            }
                                                        }
                                                    }else{
                                                        
                                                        if(((node_info->agent_list[i].router_addrs[j]->Route_Address) & 0xFF000000) == 0x00000000)
                                                        {
                                                            local_gateway = false;
                                                            non_forecd = false;
                                                            gateway_addr.s_addr = node_info->home_address_info.home_agent_addr.s_addr;
                                                            dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address;
                                                            prefix = 24;
                                                            metric = 97; 
                                                            memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                                                            memset(gateway, 0x00, IPV4_ADDR_SIZE);
                                                            memcpy(ip_address, inet_ntoa(dst_addr), IPV4_ADDR_SIZE);
                                                            memcpy(gateway, inet_ntoa(gateway_addr), IPV4_ADDR_SIZE);
                                                            ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, interface, &metric, table, non_forecd); 
                                                        }else if((node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF) == (node_info->home_address_info.home_agent_addr.s_addr & 0x00FFFFFF))
                                                        {
                                                            gateway_addr.s_addr = node_info->home_address_info.home_agent_addr.s_addr;
                                                            dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address;
                                                            prefix = 32;
                                                            metric = 96; 
                                                        }else{
                                                            if((ip->saddr & 0x00FFFFFF) == (node_info->home_address_info.home_agent_addr.s_addr & 0x00FFFFFF))
                                                            {

                                                            }else if((ip->saddr & 0x00FFFFFF) == (node_info->coa_address_info.foreign_agent_addr.s_addr & 0x00FFFFFF)){
                                                            }else{
                                                                gateway_addr.s_addr = ip->saddr;
                                                                dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF;
                                                                prefix = 24;
                                                                metric = 95; 
                                                                memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                                                                memset(gateway, 0x00, IPV4_ADDR_SIZE);
                                                                memcpy(ip_address, inet_ntoa(dst_addr), IPV4_ADDR_SIZE);
                                                                memcpy(gateway, inet_ntoa(gateway_addr), IPV4_ADDR_SIZE);
                                                                ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, interface, &metric, table, non_forecd); 
                                                            }
                                                        }
                                                        
                                                        if(ret < 0)
                                                            printf("%d\n", ret);
                                                    }
                                                    
                                                }
                                            }
                                        }
                                        
                                    }else if(node_info->agent_list[i].foreign_agent)
                                    {
                                        state->now = Accepted_Registration_From_Foreign;
                                        state->foreign = Connection_With_Foreign;
                                        state->recv = Registration_Reply_From_Forein;

                                        char *ip_address = calloc(IPV4_ADDR_SIZE, sizeof(char));
                                        int prefix;
                                        char *gateway = calloc(IPV4_ADDR_SIZE, sizeof(char)); 
                                        bool local_gateway = false;
                                        char *interface = node_info->interface;
                                        enum v2xho_util_router_metric_state_e metric;
                                        char *table = NULL;
                                        bool non_forecd = false;
                                        struct in_addr gateway_addr, dst_addr;
                                        struct V2XHO_IProuter_Route_List_info_IPv4_t route_list[MAX_ROUTE_LIST_NUM];
                                        int route_list_num = 0;
                                        ret = F_i_V2XHO_IProuter_Route_Get_List(node_info->interface, route_list, &route_list_num);
                                        struct V2XHO_extention_0_t *extention_0 = &(node_info->agent_list[i].extention_0);
                                        uint16_t coa_num = (extention_0->Length - 16)/4;
                                        
                                        for(uint16_t j = 0; j < node_info->agent_list[i].num_addrs; j++)
                                        {
                                            for(int k = 0; k < route_list_num; k++)
                                            { 
                                                if((node_info->agent_list[i].router_addrs[j]->Route_Address) == route_list[k].gateway_addr.s_addr \
                                                || node_info->agent_list[i].router_addrs[j]->Route_Address == node_info->agent_list[i].home_addr.s_addr \
                                                || node_info->agent_list[i].router_addrs[j]->Route_Address == 0)
                                                {
                                                    break;
                                                }else if(k == route_list_num - 1)
                                                {   
                                                    if(coa_num > 0)
                                                    {
                                                        for(uint16_t t = 0; t < coa_num; t++)
                                                        {   
                                                            if(extention_0->Care_Of_Addresses[t] == node_info->agent_list[i].router_addrs[j]->Route_Address \
                                                            || (extention_0->Care_Of_Addresses[t] & 0x00FFFFFF) == (node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF))
                                                            {
                                                                if(((node_info->agent_list[i].router_addrs[j]->Route_Address) & 0xFF000000) == 0x00000000)
                                                                {
                                                                    prefix = 24;
                                                                    metric = 50; 
                                                                }else{
                                                                    prefix = 32;
                                                                    metric = 40; 
                                                                }
                                                                dst_addr.s_addr = node_info->agent_list[i].router_addrs[j]->Route_Address;
                                                                gateway_addr.s_addr = extention_0->Care_Of_Addresses[t];
                                                                memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                                                                memset(gateway, 0x00, IPV4_ADDR_SIZE);
                                                                memcpy(ip_address, inet_ntoa(dst_addr), IPV4_ADDR_SIZE);
                                                                memcpy(gateway, inet_ntoa(gateway_addr), IPV4_ADDR_SIZE);
                                                                ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, interface, &metric, table, non_forecd); 
                                                                break;
                                                            }else{
                                                                if(((node_info->agent_list[i].router_addrs[j]->Route_Address) & 0xFF000000) == 0x00000000)
                                                                {
                                                                    
                                                                    gateway_addr.s_addr = node_info->home_address_info.home_agent_addr.s_addr;
                                                                    dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address;
                                                                    prefix = 24;
                                                                    metric = 50; 
                                                                    
                                                                }else if((node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF) == (node_info->home_address_info.home_agent_addr.s_addr & 0x00FFFFFF))
                                                                {
                                                                    gateway_addr.s_addr = node_info->home_address_info.home_agent_addr.s_addr;
                                                                    dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF;
                                                                    prefix = 24;
                                                                    metric = 49;
                                                                    if((enum V2XHO_Preference_Level_e)node_info->agent_list[i].router_addrs[j]->Preference_Level == PREFERENCE_LEVEL_AGENT_HOME_ADDRESS)
                                                                    {
                                                                        struct in_addr subnet ={.s_addr = (node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF)};
                                                                        if(node_info->agent_list[i].home_address_applied == false)
                                                                        {   
                                                                            memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                                                                            if(subnet.s_addr - node_info->interface_addr.sin_addr.s_addr == 0)
                                                                            {
                                                                                node_info->home_address_info.home_addr = node_info->interface_addr.sin_addr;
                                                                                node_info->agent_list[i].home_addr = node_info->interface_addr.sin_addr;
                                                                            }else
                                                                            {
                                                                                node_info->home_address_info.home_addr.s_addr = subnet.s_addr + 0x01000000;
                                                                                node_info->agent_list[i].home_addr = node_info->home_address_info.home_addr;
                                                                            }
                                                                            memcpy(ip_address, inet_ntoa(node_info->home_address_info.home_addr), IPV4_ADDR_SIZE);
                                                                            prefix = 32;
                                                                            ret = F_i_V2XHO_IProuter_Address_Set(node_info->interface, ip_address, &prefix);
                                                                            if(ret == -1 || ret == 0)
                                                                                node_info->agent_list[i].home_address_applied = true;
                                                                        }
                                                                        dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF;
                                                                        memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                                                                        memset(gateway, 0x00, IPV4_ADDR_SIZE);
                                                                        memcpy(ip_address, inet_ntoa(dst_addr), IPV4_ADDR_SIZE);
                                                                        memcpy(gateway, inet_ntoa(gateway_addr), IPV4_ADDR_SIZE);
                                                                        ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, interface, &metric, table, non_forecd); 
                                                                    }
 
                                                                }else{
                                                                    
                                                                    gateway_addr.s_addr = ip->saddr;
                                                                    dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF;
                                                                    prefix = 24;
                                                                    metric = 48; 
                                                                    memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                                                                    memset(gateway, 0x00, IPV4_ADDR_SIZE);
                                                                    memcpy(ip_address, inet_ntoa(dst_addr), IPV4_ADDR_SIZE);
                                                                    memcpy(gateway, inet_ntoa(gateway_addr), IPV4_ADDR_SIZE);
                                                                    ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, interface, &metric, table, non_forecd); 
                                                                }

                                                            }
                                                        }
                                                    }else{
                                                        if(((node_info->agent_list[i].router_addrs[j]->Route_Address) & 0xFF000000) == 0x00000000)
                                                        {
                                                            gateway_addr.s_addr = node_info->home_address_info.home_agent_addr.s_addr;
                                                            dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address;
                                                            prefix = 24;
                                                            metric = 47; 
                                                            memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                                                            memset(gateway, 0x00, IPV4_ADDR_SIZE);
                                                            memcpy(ip_address, inet_ntoa(dst_addr), IPV4_ADDR_SIZE);
                                                            memcpy(gateway, inet_ntoa(gateway_addr), IPV4_ADDR_SIZE);
                                                            ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, interface, &metric, table, non_forecd); 
                                                        }else if((node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF) == (node_info->home_address_info.home_agent_addr.s_addr & 0x00FFFFFF))
                                                        {
                                                            
                                                            gateway_addr.s_addr = node_info->home_address_info.home_agent_addr.s_addr;
                                                            dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address;
                                                            prefix = 32;
                                                            metric = 46; 
                                                        }else{
                                                            
                                                             if((ip->saddr & 0x00FFFFFF) == (node_info->home_address_info.home_agent_addr.s_addr & 0x00FFFFFF))
                                                            {

                                                            }else if((ip->saddr & 0x00FFFFFF) == (node_info->coa_address_info.foreign_agent_addr.s_addr & 0x00FFFFFF)){
                                                                
                                                            }else{
                                                                gateway_addr.s_addr = ip->saddr;
                                                                dst_addr.s_addr  = node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF;

                                                                prefix = 24;
                                                                metric = 45; 
                                                                memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                                                                memset(gateway, 0x00, IPV4_ADDR_SIZE);
                                                                memcpy(ip_address, inet_ntoa(dst_addr), IPV4_ADDR_SIZE);
                                                                memcpy(gateway, inet_ntoa(gateway_addr), IPV4_ADDR_SIZE);
                                                                ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, interface, &metric, table, non_forecd); 
                                                            }


                                                        }
                                                        
                                                        if(ret < 0)
                                                            printf("%d\n", ret);
                                                    }
                                                    
                                                }
                                            }
                                        }
                                    }

                                    memcpy(&node_info->agent_list[i].udp_recv_payload, udp_payload, sizeof(struct V2XHO_1_UDP_Payload_Header_t));
                                    return ((enum V2XHO_Registration_Reply_Code_e)udp_payload->code);
                                }
                            } 
                        }
                        default:
                            break;   
                    }
                    break;
                }
                default: break;
            }
        }else
        {
            return 0;
        }
    }
    return 0;
}     
/* 
Brief: f_i_V2XHO_Node_Receive_Do을 통해 수신한 메시지 중 UDP 메시지를 파싱
Parameter[In]
    char *buf:수신한 메시지 패킷(이더넷 로우 패킷)
Parameter[Out]
    struct *udphd:메시지가 정상이면 UDP 헤더의 포인터를 반환
 */
struct udphdr *f_s_V2XHO_Receiver_Parser_UDP(char *buf)
{
    if(buf)
    {

        struct iphdr *ip = (struct iphdr *)(buf + sizeof(struct ether_header));
        uint32_t pointer_now = sizeof(struct ether_header) + sizeof(struct iphdr);
        struct udphdr *udp = (struct udphdr *)(buf + pointer_now);
        uint16_t ip_len = htons(ip->tot_len);
        uint16_t udp_len = htons(udp->len);
        if(ip_len - sizeof(struct iphdr) > udp_len)
        {
            uint8_t *iphdr_option_len = (uint8_t *)(buf + pointer_now + sizeof(uint8_t));
            //printf("iphdr heve extenion field:len:%d\n", *iphdr_option_len);
            pointer_now += *iphdr_option_len;
            udp = (struct udphdr *)(buf + pointer_now);
            if(ip_len == pointer_now + *iphdr_option_len + udp_len)
            {
                printf("packet_error!\n");
                return NULL;
            }
        }
        return udp;
    }else{
        return NULL;
    }
}
uint32_t* f_u32_V2XHO_Receiver_Parser_ICMP_Advertisement(char *buf, struct V2XHO_Node_Info_t *node_info)
{
    if(buf)
    {
        uint32_t pointer_now = sizeof(struct ether_header);
        struct iphdr *ip = (struct iphdr *)(buf + pointer_now);
        pointer_now += sizeof(struct iphdr) + sizeof(struct icmphdr) - 4;
        struct V2XHO_0_ICMP_Payload_Header_t *icmp_payload_hdr = (struct V2XHO_0_ICMP_Payload_Header_t *)(buf + pointer_now);
        pointer_now += sizeof(struct V2XHO_0_ICMP_Payload_Header_t);
        struct V2XHO_Router_Addr_es_t router_addrs[icmp_payload_hdr->Num_Addrs];
        for(int i = 0; i < icmp_payload_hdr->Num_Addrs; i++)
        {
            struct V2XHO_Router_Addr_es_t *icmp_payload_data = (struct V2XHO_Router_Addr_es_t *)(buf + pointer_now);
            memcpy(&(router_addrs[i]), icmp_payload_data, sizeof(struct V2XHO_Router_Addr_es_t) - sizeof(uint32_t));
            pointer_now += ((sizeof(struct V2XHO_Router_Addr_es_t) - sizeof(uint32_t))); //14 + (28 + 8 * 2) = 14 + 44
        }
        uint16_t ip_len = htons(ip->tot_len);
        for(int i = 0; i < ip_len; i++)
        {
            //printf("%02X",buf[i]);
        }
        printf("\n");
        if(ip_len > pointer_now - sizeof(struct ether_header))
        {
            uint8_t *type = (uint8_t *)(buf + pointer_now);
            if(*type == 16)
            {
                struct V2XHO_extention_0_t *extention_0 = (struct V2XHO_extention_0_t *)(buf + pointer_now);
                for(int i = pointer_now; i < ip_len; i++)
                {
                    printf("%02X",buf[i]);
                }
                printf("\n");
                uint8_t *num_agent = node_info->num_agent;
                if(extention_0->flag.H == 1)
                {
                    printf("Home Agent!\n");
                    node_info->agent_list[*num_agent].home_agent = true;
                    node_info->agent_list[*num_agent].foreign_agent = false;
                }
                if(extention_0->flag.F == 1)
                {
                    printf("Foreign Agent!\n");
                    node_info->agent_list[*num_agent].home_agent = false;
                    node_info->agent_list[*num_agent].foreign_agent = true;
                }
                if(*num_agent == 0)
                {
                    node_info->agent_list[*num_agent].agent_addr.s_addr = ip->saddr;
                    node_info->agent_list[*num_agent].last_recv_adv_time= time(NULL);
                    for(int i = 0; i < icmp_payload_hdr->Num_Addrs; i++)
                    {
                        router_addrs[i].Preference_Level = htonl(router_addrs[i].Preference_Level);
                        F_i_V2XHO_Node_ICMP_Router_addr_Add(node_info->agent_list[*num_agent].router_addrs, &node_info->agent_list[*num_agent].num_addrs, &(router_addrs[i]));
                    }
                    memcpy(&node_info->agent_list[*num_agent].icmp_payload, icmp_payload_hdr, sizeof(struct V2XHO_0_ICMP_Payload_Header_t));
                    memcpy(&node_info->agent_list[*num_agent].extention_0, extention_0, sizeof(struct V2XHO_extention_0_t) - (sizeof(uint32_t) * 128) + (extention_0->Length - 16)/4);
                    (*num_agent)++;
                }else {
                    for(int i = 0; i < *num_agent; i++)
                    {
                        if(node_info->agent_list[i].agent_addr.s_addr == ip->saddr)
                        {
                            node_info->agent_list[i].last_recv_adv_time = time(NULL);
                            for(int j = 0; j < icmp_payload_hdr->Num_Addrs; j++)
                            {
                                router_addrs[j].Preference_Level = htonl(router_addrs[j].Preference_Level);
                                F_i_V2XHO_Node_ICMP_Router_addr_Add(node_info->agent_list[i].router_addrs, &node_info->agent_list[i].num_addrs, &(router_addrs[j]));
                            }
                            memcpy(&node_info->agent_list[i].icmp_payload, icmp_payload_hdr, sizeof(struct V2XHO_0_ICMP_Payload_Header_t));
                            memcpy(&node_info->agent_list[*num_agent].extention_0, extention_0, sizeof(struct V2XHO_extention_0_t) - (sizeof(uint32_t) * 128) + (extention_0->Length - 16)/4);
                            break;
                        }else if(node_info->agent_list[i].agent_addr.s_addr != ip->saddr && i == *num_agent - 1)
                        {
                            
                            node_info->agent_list[*num_agent].agent_addr.s_addr = ip->saddr;
                            node_info->agent_list[*num_agent].last_recv_adv_time = time(NULL);
                            for(int j = 0; j < icmp_payload_hdr->Num_Addrs; j++)
                            {
                                router_addrs[j].Preference_Level = htonl(router_addrs[j].Preference_Level);
                                F_i_V2XHO_Node_ICMP_Router_addr_Add(node_info->agent_list[*num_agent].router_addrs, &node_info->agent_list[*num_agent].num_addrs, &(router_addrs[j]));
                            }
                            memcpy(&node_info->agent_list[*num_agent].icmp_payload, icmp_payload_hdr, sizeof(struct V2XHO_0_ICMP_Payload_Header_t));
                            memcpy(&node_info->agent_list[*num_agent].extention_0, extention_0, sizeof(struct V2XHO_extention_0_t) - (sizeof(uint32_t) * 128) + (extention_0->Length - 16)/4);
                            (*num_agent)++;
                        }
                    }
                }
                for(int i = 0; i < *num_agent; i++)
                {
                    if(node_info->agent_list[i].foreign_agent == true)
                    {
                        uint16_t num_addr = node_info->agent_list[i].num_addrs;
                        for(int j = 0; j < num_addr; j++)
                        {
                            if((enum V2XHO_Preference_Level_e)node_info->agent_list[i].router_addrs[j]->Preference_Level == PREFERENCE_LEVEL_AGENT_HOME_ADDRESS)
                            {
                                struct in_addr subnet ={.s_addr = (node_info->agent_list[i].router_addrs[j]->Route_Address & 0x00FFFFFF)};
                                if(node_info->agent_list[i].home_address_applied == false)
                                {   
                                    char *ip_address = malloc(IPV4_ADDR_SIZE * sizeof(char));
                                    memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                                    if(subnet.s_addr - node_info->interface_addr.sin_addr.s_addr == 0)
                                    {
                                        node_info->home_address_info.home_addr = node_info->interface_addr.sin_addr;
                                        node_info->agent_list[i].home_addr = node_info->interface_addr.sin_addr;
                                    }else
                                    {
                                        node_info->home_address_info.home_addr.s_addr = subnet.s_addr + 0x01000000;
                                        node_info->agent_list[i].home_addr = node_info->home_address_info.home_addr;
                                    }
                                    memcpy(ip_address, inet_ntoa(node_info->home_address_info.home_addr), IPV4_ADDR_SIZE);
                                    int prefix = 32;
                                    int ret = F_i_V2XHO_IProuter_Address_Set(node_info->interface, ip_address, &prefix);
                                    printf("ret:%d\n", ret);
                                    if(ret == -1 || ret == 0)
                                        node_info->agent_list[i].home_address_applied = true;
                                }
                            }
                        }
                    }
                }
                return (uint32_t *)extention_0;
            }
        }
        
    }else{
    }
    return NULL;
}     

struct iphdr *f_s_V2XHO_Receiver_Parser_IPHDR(char *buf)
{
    if(buf)
    {
        uint32_t pointer_now = sizeof(struct ether_header);
        struct iphdr *ip = (struct iphdr *)(buf + pointer_now);
        return ip;
    }else{
        return NULL;
    }
}
                
struct icmphdr *f_s_V2XHO_Receiver_Parser_ICMP(char *buf)
{
    if(buf)
    {
        uint32_t pointer_now = sizeof(struct ether_header) + sizeof(struct iphdr);
        struct icmphdr *icmp = (struct icmphdr *)(buf + pointer_now);
        return icmp;
    }else{
        return NULL;
    }
}

int F_i_V2XHO_Node_Route_Add(struct V2XHO_Node_Info_t *node_info, enum V2XHO_Ip_Dst_Type_e dst_type, char *address, int prefix, char *gateway_addr, bool local_gateway, enum v2xho_util_router_metric_state_e metric, char *table, bool NonForeced)
{
    int ret;
    char *gateway;
    if(local_gateway)
    {
        gateway = inet_ntoa(node_info->interface_addr.sin_addr);
    }else{
        gateway = gateway_addr;
    }

    switch(dst_type)
    {
        case unicast:
        {
            char *ip_address = address;
            ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, node_info->interface, &metric, table, NonForeced); 
            break;
        }
        case multicast:
        {
            char *adv_ip_address = "224.0.0.0";
            prefix = 24;
            ret = F_i_V2XHO_IProuter_Route_Add(adv_ip_address, &prefix, gateway, local_gateway, node_info->interface, &metric, table, NonForeced); 
            break;
        }
        case broadcast:
            break;
        default:
        {
            char *ip_address = "0.0.0.0";
            prefix = 0;//0
            metric = 1024;
            ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, node_info->interface, &metric, table, NonForeced); 
            break;
        }
    }
    return ret;
}

int F_i_V2XHO_Node_Interface_Addr_Add(struct V2XHO_Node_Info_t *node_info, enum V2XHO_Ip_Dst_Type_e dst_type, char *address, int prefix)
{
    int ret;
    switch(dst_type)
    {
        case unicast:
        {
            char *ip_address = address;
            ret = F_i_V2XHO_IProuter_Address_Set(node_info->interface, ip_address, &prefix);
            break;
        }
        case multicast:
        {
            char *adv_ip_address = "224.0.0.0";
            ret = F_i_V2XHO_IProuter_Address_Set(node_info->interface, adv_ip_address, &prefix);
            break;
        }
        case broadcast:
            break;
    }
    return ret;
}

int F_i_V2XHO_Node_ICMP_Router_addr_Add(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs, struct V2XHO_Router_Addr_es_t *router_addrs)
{
    if(router_addrs)
    {
        for(int i = 0; i < *num_addrs; i++)
        {
            if(router_es[i]->Route_Address - router_addrs->Route_Address == 0)
            {
                return i;
            }
        }
        router_es[*num_addrs] = malloc(sizeof(struct V2XHO_Router_Addr_es_t));
        router_es[*num_addrs]->used = true;
        router_es[*num_addrs]->Route_Address = router_addrs->Route_Address;
        router_es[*num_addrs]->Preference_Level = router_addrs->Preference_Level;
        router_es[*num_addrs]->used = false;
         (*num_addrs)++;
    }else
    {

        return -1;
    }
    return 0;   
}

int F_i_V2XHO_Node_ICMP_Router_addr_Del(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs, uint32_t *router_addrs)
{
    if(router_addrs)
    {
        for(int i = 0; i < *num_addrs; i++)
        {
            if(router_es[i]->Route_Address - *router_addrs == 0)
            {
                int ret;
                while(router_es[i]->used == false)
                {
                    usleep(1000);
                } 
                router_es[i]->used = false;
                router_es[i]->Route_Address = 0;
                router_es[i]->Preference_Level = 0;
                V2XHO_safefree(router_es[i], sizeof(struct V2XHO_Router_Addr_es_t ));
                if(i == *num_addrs - 1)
                {
                }else
                {
                    for(int j = i; j < *num_addrs; j++)
                    {
                        router_es[j] =  router_es[j + 1];
                    }
                    ret = F_i_V2XHO_Node_ICMP_Router_addr_Del(router_es, num_addrs, &router_es[*num_addrs]->Route_Address);
                    if(ret < 0)
                        return -3;
                }
                (*num_addrs)--;
            }else if(i == *num_addrs - 1)
            {
                return -2;
            }
        }
    }else
    {
        return -1;
    }
    return 0;   
}

int F_i_V2XHO_Node_ICMP_Router_addr_Flush(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs, bool Not_all)
{
    if(1)
    {
        for(int i =  *num_addrs - 1; i > 0 + Not_all; i++)
        {
            while(router_es[i]->used == false)
            {
                usleep(1000);
            }
            router_es[i]->used = true;
            router_es[i]->Route_Address = 0;
            router_es[i]->Preference_Level = 0;
            V2XHO_safefree(router_es[i], 1);
            (num_addrs)--;
        }
    }else
    {
        return -1;
    }
    return 0;   
}

int F_i_V2XHO_Node_Set_ICMP_Preference_Level(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs,  uint32_t *router_addrs, int Preference_Level)
{
    if(router_addrs)
    {
        for(int i = 0; i < *num_addrs; i++)
        {
            if(router_es[i]->Route_Address - *router_addrs == 0)
            {
                while(router_es[i]->used == false)
                {
                    usleep(1000);
                }
                router_es[i]->used = true;
                router_es[i]->Preference_Level = Preference_Level;
                router_es[i]->used = false;
                break;
            }
        }
    }else
    {
        return -1;
    }
    return 0;   
}

int F_i_V2XHO_Node_Extention_Set(struct V2XHO_Node_Info_t *node_info, uint8_t extention_num, void *data);
int F_i_V2XHO_Node_Extention_Get(struct V2XHO_Node_Info_t *node_info, uint8_t extention_num, void *out_data);