#include <V2XHO_Agent.h>

static uint8_t f_u8_V2XHO_Receiver_Parser_ICMP_Solicitation(char *buf, struct V2XHO_Agent_Info_t *agent_info);
static enum V2XHO_Agent_Type_e f_e_V2XHO_Receiver_Parser_ICMP_Advertisement(char *buf, struct V2XHO_Agent_Info_t *agent_info);
static struct udphdr *f_s_V2XHO_Receiver_Parser_UDP(char *buf);
static int f_i_V2XHO_Receiver_Parser_UDP_Registration_Request(char *buf, struct V2XHO_Agent_Info_t *agent_info);
static struct iphdr *f_s_V2XHO_Receiver_Parser_IPHDR(char *buf);
static struct icmphdr *f_s_V2XHO_Receiver_Parser_ICMP(char *buf);
static int f_i_V2XHO_Agent_Receive_Do(struct V2XHO_Agent_Info_t *agent_info, int timer);
static int f_i_V2XHO_Agent_Send_Do(struct V2XHO_Agent_Info_t *agent_info);

/* 
Brief:
Parameter[In]
    void *data: The data type is struct V2XHO_Agent_Info_t. Its type, interface fields must be filled.
Parameter[Out]
    NULL
 */
void* F_th_V2XHO_Agent_Task_Doing(void *data)
{
    struct V2XHO_Agent_Info_t *agent_info = (struct V2XHO_Agent_Info_t*)data;
    int ret;
    ret = F_i_V2XHO_Router_Hendler_Open(&rth);
    agent_info->send_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    agent_info->send_socket_common = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    agent_info->recv_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    int on = 1;
    setsockopt(agent_info->send_socket , IPPROTO_IP, IP_HDRINCL, (const char*)&on, sizeof (on));
    setsockopt(agent_info->send_socket , SOL_SOCKET, SO_BINDTODEVICE, agent_info->interface_mobileip, strlen(agent_info->interface_mobileip));
    setsockopt(agent_info->send_socket_common , SOL_SOCKET, SO_BINDTODEVICE, agent_info->interface_common, strlen(agent_info->interface_common));
    setsockopt(agent_info->recv_socket , IPPROTO_IP, IP_HDRINCL, (const char*)&on, sizeof (on));

    //setsockopt(agent_info->recv_socket , SOL_SOCKET, SO_BINDTODEVICE, agent_info->interface_mobileip, strlen(agent_info->interface_mobileip));
    int optval[2] = {1, 0};
    setsockopt (agent_info->send_socket , SOL_SOCKET, SO_LINGER, &optval, sizeof(optval));
    setsockopt (agent_info->recv_socket , SOL_SOCKET, SO_LINGER, &optval, sizeof(optval));
    struct ifreq ifr;
    //setsockopt(socket, SOL_SOCKET, (const char*)&on, sizeof(on));
    //setsockopt(socket, SOL_SOCKET, SO_BINDTODEVICE, device, sizeof(device))
    //setsockopt (agent_info->broadcast_socket.socket, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof (on));
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    memcpy(ifr.ifr_name, agent_info->interface_mobileip, strlen(agent_info->interface_mobileip));
    ifr.ifr_name[strlen(agent_info->interface_mobileip)] = 0;
    ioctl(sock, SIOCGIFINDEX, &ifr);
    agent_info->interface_v2xho_index = ifr.ifr_ifindex;
    memcpy(ifr.ifr_name, agent_info->interface_common, strlen(agent_info->interface_common));
    ifr.ifr_name[strlen(agent_info->interface_common)] = 0;
    ioctl(agent_info->send_socket_common, SIOCGIFINDEX, &ifr);
    agent_info->interface_v2xho_index = ifr.ifr_ifindex;
    struct sockaddr_ll saddrll = {.sll_family = AF_PACKET, .sll_ifindex = agent_info->interface_common_index};
    bind(agent_info->send_socket_common, (struct sockaddr*)&saddrll, sizeof(saddrll));
    close(sock);
    /*기본 라우트 테이블 설정*/
    /*dest                  gateway         dev         metric  */
    /*default               agent_addr      mobileip    1024       */
    /*common_subnet/24      common_addr     common      1024    */
    /*common_subnet/32      common_addr     common      4       */

    if(agent_info->agent_info_list[0].num_addrs == NULL)
    {
        agent_info->agent_info_list[0].num_addrs = malloc(sizeof(uint16_t));
        *agent_info->agent_info_list[0].num_addrs = 0;
        char *interface_ip_0 = F_p_V2XHO_Get_InterfaceIP(agent_info->interface_mobileip);
        char *interface_ip_1 = F_p_V2XHO_Get_InterfaceIP(agent_info->interface_common);
        printf("[MOBILEIP]interface name:%s, interface ifip:%s\n", agent_info->interface_mobileip, interface_ip_0);
        printf("[COMMONIP]interface name:%s, interface ifip:%s\n", agent_info->interface_common, interface_ip_1);
        agent_info->interface_v2xho_addr.sin_addr.s_addr = inet_addr(interface_ip_0);
        agent_info->interface_common_addr.sin_addr.s_addr = inet_addr(interface_ip_1);
        char cmd[128];
        memset(cmd, 0x00, 128);
        sprintf(cmd, "ip route flush dev %s", agent_info->interface_mobileip);
        system(cmd);
        memset(cmd, 0x00, 128);
        sprintf(cmd, "ip route flush dev %s", agent_info->interface_common);
        system(cmd);
        sleep(1);
        memset(cmd, 0x00, 128);
        sprintf(cmd, "ip route add default via %s dev %s metric 1024", interface_ip_0, agent_info->interface_mobileip);
        system(cmd);
        memset(cmd, 0x00, 128);
        struct in_addr subnet;
        subnet.s_addr = agent_info->interface_common_addr.sin_addr.s_addr & 0x00FFFFFF;
        sprintf(cmd, "ip route add %s/24 via %s dev %s metric 1024", inet_ntoa(subnet), interface_ip_1, agent_info->interface_common);
        system(cmd);
        sleep(1);
        memset(cmd, 0x00, 128);
        sprintf(cmd, "ip route add 192.168.0.250/32 via %s dev eth1 metric 4", inet_ntoa(agent_info->interface_common_addr.sin_addr));
        system(cmd); 
        V2XHO_safefree(interface_ip_0, 0);
        V2XHO_safefree(interface_ip_1, 0);
    }
    agent_info->num_nodes = malloc(sizeof(uint16_t));
    *agent_info->num_nodes = 0;
    ret = F_i_V2XHO_Agent_Route_Add(agent_info, -1, NULL, 0, NULL, true, 1024, NULL, false);
    agent_info->agent_state = malloc(sizeof(struct V2XHO_Agent_State_t));
    memset(agent_info->agent_state ,0x00, sizeof(struct V2XHO_Agent_State_t));
    struct V2XHO_Agent_State_t *state = agent_info->agent_state;
    agent_info->num_agent = calloc(1, sizeof(uint16_t));
    
     if(agent_info->interface_mobileip)
    {
        struct V2XHO_IProuter_Route_List_info_IPv4_t route_list[MAX_ROUTE_LIST_NUM];
        int route_list_num = 0;
        ret = F_i_V2XHO_IProuter_Route_Get_List(agent_info->interface_mobileip, route_list, &route_list_num);
        agent_info->agent_info_list[0].num_addrs = calloc(1,sizeof(uint16_t));
        for(int i = 0; i < route_list_num; i++)
        {
            if((route_list[i].gateway_addr.s_addr) == (agent_info->interface_v2xho_addr.sin_addr.s_addr))
            {
                struct V2XHO_Router_Addr_es_t router_addrs;
                router_addrs.Route_Address = route_list[i].gateway_addr.s_addr;
                switch(agent_info->type)
                {
                    case HomeAgent:
                    {
                        router_addrs.Preference_Level = (uint32_t)((enum V2XHO_Preference_Level_e) PREFERENCE_LEVEL_AGENT_HOME_ADDRESS);
                        break;
                    }
                    case ForeignAgent:
                    {
                        router_addrs.Preference_Level = (uint32_t)((enum V2XHO_Preference_Level_e) PREFERENCE_LEVEL_AGENT_FOREIGN_ADDRESS);
                        break;
                    }
                    default: break;
                }
                ret = F_i_V2XHO_Agent_ICMP_Router_addr_Add(agent_info->agent_info_list[0].router_addrs, agent_info->agent_info_list[0].num_addrs, &router_addrs);
                if((route_list[i].dest_addr.s_addr & 0x00FFFFFF) == (agent_info->interface_v2xho_addr.sin_addr.s_addr & 0x00FFFFFF))
                {
                    if((route_list[i].dest_addr.s_addr & 0xFF000000) == 0)
                    {
                        switch(agent_info->type)
                        {
                            case HomeAgent:
                            {
                                router_addrs.Preference_Level = (uint32_t)((enum V2XHO_Preference_Level_e) PREFERENCE_LEVEL_AGENT_HOME_SUBNET);
                                break;
                            }
                            case ForeignAgent:
                            {
                                router_addrs.Preference_Level = (uint32_t)((enum V2XHO_Preference_Level_e) PREFERENCE_LEVEL_AGENT_FOREIGN_SUBNET);
                                break;
                            }
                            default: break;
                        }
                        router_addrs.Route_Address = route_list[i].dest_addr.s_addr;
                        ret = F_i_V2XHO_Agent_ICMP_Router_addr_Add(agent_info->agent_info_list[0].router_addrs, agent_info->agent_info_list[0].num_addrs, &router_addrs);
                    }
                    
                }
            }else if(agent_info->interface_common && strcmp(agent_info->interface_mobileip, agent_info->interface_common) != 0)
            {       
                if((route_list[i].gateway_addr.s_addr ) == (agent_info->interface_common_addr.sin_addr.s_addr))
                {
                    struct V2XHO_Router_Addr_es_t router_addrs;
                    router_addrs.Route_Address = route_list[i].gateway_addr.s_addr;
                    router_addrs.Preference_Level = (uint32_t)((enum V2XHO_Preference_Level_e) PREFERENCE_LEVEL_CORRESPHOND_ADDRESS);
                    ret = F_i_V2XHO_Agent_ICMP_Router_addr_Add(agent_info->agent_info_list[0].router_addrs, agent_info->agent_info_list[0].num_addrs, &router_addrs);    
                }
                if((route_list[i].dest_addr.s_addr & 0x00FFFFFF) == (agent_info->interface_common_addr.sin_addr.s_addr & 0x00FFFFFF))
                {
                    if((route_list[i].dest_addr.s_addr & 0xFF000000) == 0)
                    {
                        struct V2XHO_Router_Addr_es_t router_addrs;
                        router_addrs.Preference_Level = (uint32_t)((enum V2XHO_Preference_Level_e) PREFERENCE_LEVEL_CORRESPHOND_SUBNET);
                        router_addrs.Route_Address = route_list[i].dest_addr.s_addr;
                        ret = F_i_V2XHO_Agent_ICMP_Router_addr_Add(agent_info->agent_info_list[0].router_addrs, agent_info->agent_info_list[0].num_addrs, &router_addrs);
                    }

                }
            }
        }
        agent_info->agent_info_list[0].agent_addr = agent_info->interface_v2xho_addr.sin_addr;
        agent_info->agent_info_list[0].adv_icmp_info.extention_0 = malloc(sizeof(struct V2XHO_extention_0_t));
        switch(agent_info->type)
        {
            case HomeAgent:
            {
                state->now = Waiting_Message_From_Any;
                agent_info->agent_info_list[0].adv_icmp_info.extention_0->flag.H = 1;
                agent_info->agent_info_list[0].adv_icmp_info.extention_0->flag.F = 0;
                break;
            }
            case ForeignAgent:
            {
                state->now = Waiting_Advertisement_From_Home;
                agent_info->agent_info_list[0].adv_icmp_info.extention_0->flag.F = 1;
                agent_info->agent_info_list[0].adv_icmp_info.extention_0->flag.H = 0;
                agent_info->agent_info_list[0].adv_icmp_info.extention_0->Care_Of_Addresses[0] = agent_info->agent_info_list[0].agent_addr.s_addr;
                agent_info->agent_info_list[0].num_coa_addrs++;
                break;
            }
            default: 
            {
                state->now = Waiting_Message_From_Any;
                break;
            }
        }
        (*agent_info->num_agent)++;
    }
  
    printf("agent_info->type:%d\n", agent_info->type);
    agent_info->agent_info_list[0].adv_icmp_info.code = 0;
    agent_info->agent_info_list[0].adv_icmp_info.lifetime = 60;
    agent_info->agent_info_list[0].adv_icmp_info.extention_0 = F_s_V2XHO_Agent_Advertisements_Initial_Extention_0(agent_info, agent_info->agent_info_list[0].adv_icmp_info.extention_0);

    bool wait_send = true;
    while(1)
    {
        if(state->now != Waiting_Message_From_Any)
        {
            //printf("[%s][%d] Node State:%d\n", __func__,__LINE__, state->now);
        }
        
        switch(state->now)
        {
            case Waiting_Message_From_Any:
            {
                state->trx = Receiver_Mode;
                state->send = Sleep_Sender;
                state->recv = Sleep_Receiver;
                ret = f_i_V2XHO_Agent_Receive_Do(agent_info, 25);
                break;
            }
            case Waiting_Advertisement_From_Home:
            {
                state->trx = Receiver_Mode;
                state->send = Sleep_Sender;
                ret = f_i_V2XHO_Agent_Receive_Do(agent_info, 25);
                break;
            }
            case Operation_Solicitation_Reply_To_Node:
            {
                state->trx = Sender_Mode;
                state->recv = Sleep_Receiver;
                state->send = Solicitation_Reply_To_Node;
                ret = f_i_V2XHO_Agent_Send_Do(agent_info);
                if(ret >= 0)
                    state->now = Waiting_Message_From_Any; 
                break;
            }
            case Operation_Advertisement_To_Node:
            {
                state->trx = Sender_Mode;
                state->recv = Sleep_Receiver;
                state->send = Advertisement_To_Node;
                ret = f_i_V2XHO_Agent_Send_Do(agent_info);
                if(ret >= 0)
                    state->now = Waiting_Message_From_Any; 
                break;
            }
            case Operation_Advertisement_To_Agent:
            {
                state->trx = Sender_Mode;
                state->recv = Registration_Request_From_Node;
                state->send = Advertisement_To_Agent;
                ret = f_i_V2XHO_Agent_Send_Do(agent_info);
                if(ret >= 0)
                    state->now = Waiting_Message_From_Any; 
                break;
            }
            case Operation_Registration_Reply_To_Node:
            {
                state->trx = Sender_Mode;
                state->recv = Sleep_Receiver;
                state->send = Registration_Reply_To_Node;
                ret = f_i_V2XHO_Agent_Send_Do(agent_info);
                if(ret >= 0)
                    state->now = Waiting_Message_From_Any; 
                break;
            }
            default:
                break;        
        }
        if(ret >= 0)
        {
            switch(state->recv)
            {
                case Sleep_Receiver:
                    break;
                case Solicitation_From_Node:
                    state->now = Operation_Solicitation_Reply_To_Node;
                    break;
                case Advertisement_From_Home:
                    state->now = Operation_Advertisement_To_Node;
                case Registration_Request_From_Node:
                    if(state->now != Operation_Advertisement_To_Agent)
                        state->now = Operation_Registration_Reply_To_Node;
                    break;
                case Registration_Request_From_Agent:
                default:
                    break;
            }
        }else{
            switch(wait_send)
            {
                case false:
                {
                    if(g_time_tick->tick.ms100 == 1)
                    {
                        if(state->now != Waiting_Advertisement_From_Home)
                        {
                            state->now = Operation_Advertisement_To_Node;
                        }
                       //printf("%02d %02d %02d\n", g_time_tick->tick.ms1000, g_time_tick->tick.ms100, g_time_tick->tick.ms10);
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
    close(agent_info->send_socket);
    F_v_V2XHO_Router_Hendler_Close(&rth);
}

int f_i_V2XHO_Agent_Send_Do(struct V2XHO_Agent_Info_t *agent_info)
{
    struct V2XHO_Agent_State_t *state = agent_info->agent_state;
    struct V2XHO_IP_Header_Info_t ip_hdr_info;
    int ret;
    switch(state->now)
    {
        case Operation_Advertisement_To_Node:
        {
            ip_hdr_info.ip_dst_type = multicast;
            ip_hdr_info.ip_tos = 0; 
            ip_hdr_info.ip_id = 0;
            ip_hdr_info.ip_src_addr = "0.0.0.0";
            ip_hdr_info.ip_dst_addr = "224.0.0.1";
            ip_hdr_info.ip_ttl = 1;
            ret = F_i_V2XHO_Agent_Advertisement_Do(agent_info, ip_hdr_info);

            break;
        }
        case Operation_Advertisement_To_Agent:
        {
            ip_hdr_info.ip_dst_type = multicast;
            ip_hdr_info.ip_tos = 0; 
            ip_hdr_info.ip_id = 0;
            ip_hdr_info.ip_src_addr = calloc(64, sizeof(uint8_t));
            memcpy(ip_hdr_info.ip_src_addr, inet_ntoa(agent_info->interface_common_addr.sin_addr), 64);
            ip_hdr_info.ip_dst_addr = "224.0.0.11";
            ip_hdr_info.ip_ttl = 1;
            ret = F_i_V2XHO_Agent_Advertisement_Do(agent_info, ip_hdr_info);
            V2XHO_safefree(ip_hdr_info.ip_src_addr, 64);
            break;
        }
        case Operation_Solicitation_Reply_To_Node:
        {
            for(int i = *agent_info->num_nodes; i >= 0; i--)
            {
                if(agent_info->registed_node_list[i].state == Solicitation_To_Agent)
                {
                    ip_hdr_info.ip_dst_type = unicast;
                    ip_hdr_info.ip_tos = 0; 
                    ip_hdr_info.ip_id = 0;
                    ip_hdr_info.ip_src_addr = calloc(64, sizeof(uint8_t));
                    ip_hdr_info.ip_dst_addr = calloc(64, sizeof(uint8_t));
                    memcpy(ip_hdr_info.ip_src_addr, inet_ntoa(agent_info->interface_v2xho_addr.sin_addr), 64);
                    memcpy(ip_hdr_info.ip_dst_addr, inet_ntoa(agent_info->registed_node_list[i].node_address), 64);
                    ret = F_i_V2XHO_Agent_Advertisement_Do(agent_info, ip_hdr_info);
                    V2XHO_safefree(ip_hdr_info.ip_src_addr, 64);
                    V2XHO_safefree(ip_hdr_info.ip_dst_addr, 64);
                }
            }
            break;
        }
        case Operation_Registration_Reply_To_Node:
        {
            for(int i = *agent_info->num_nodes - 1; i >= 0; i--)
            {
                if(agent_info->registed_node_list[i].state == Registration_Request_To_Agent)
                {
                    ip_hdr_info.ip_dst_type = unicast;
                    ip_hdr_info.ip_tos = 0; 
                    ip_hdr_info.ip_id = 0;
                    ip_hdr_info.ip_ttl = 1;
                    ip_hdr_info.ip_src_addr = calloc(64, sizeof(uint8_t));
                    ip_hdr_info.ip_dst_addr = calloc(64, sizeof(uint8_t));
                    memcpy(ip_hdr_info.ip_src_addr, inet_ntoa(agent_info->interface_v2xho_addr.sin_addr), 64);
                    memcpy(ip_hdr_info.ip_dst_addr, inet_ntoa(agent_info->registed_node_list[i].node_address), 64);
                    ret = F_i_V2XHO_Agent_Registration_Reply_Do(agent_info, i, ip_hdr_info);
                    if(ret < 0)
                    {
                     
                    }
                    V2XHO_safefree(ip_hdr_info.ip_src_addr, 64);
                    V2XHO_safefree(ip_hdr_info.ip_dst_addr, 64);
                }
            }
            break;
        }
        default: break;
    }
    return ret;
}

int f_i_V2XHO_Agent_Receive_Do(struct V2XHO_Agent_Info_t *agent_info, int recv_timer)
{
    ssize_t recv_len = 0;
    char buf[1024];
    struct V2XHO_Agent_State_t *state = agent_info->agent_state;
    switch(state->now)
    {
        case Waiting_Message_From_Any:
        {
            struct sockaddr_ll saddrll;
            socklen_t src_addr_len = sizeof(saddrll);
            struct timeval tv_timeo = { (int)(recv_timer/1e6), (recv_timer % 1000) * 1000}; 
            setsockopt(agent_info->recv_socket, SOL_SOCKET, SO_RCVTIMEO, &tv_timeo, sizeof(tv_timeo));
            recv_len = recvfrom(agent_info->recv_socket, buf, 1024, 0, (struct sockaddr *)&(saddrll), &src_addr_len);
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
                            if(ip->daddr == inet_addr("224.0.0.11") || ip->daddr == inet_addr("224.0.0.1") || ip->daddr == agent_info->interface_v2xho_addr.sin_addr.s_addr)
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
                                                case ICMP_HOST_ANO: //10
                                                {
                                                    if(f_u8_V2XHO_Receiver_Parser_ICMP_Solicitation(buf, agent_info) > 0)
                                                    {
                                                        state->recv = Solicitation_From_Node;
                                                        return 0;
                                                    }
                                                    break;
                                                }
                                                case ICMP_NET_ANO: //9
                                                {
                                                    int ret = (int)f_e_V2XHO_Receiver_Parser_ICMP_Advertisement(buf, agent_info);
                                                    if(ret == 0 || ret == 1 || ret == 2)
                                                    {
                                                        enum V2XHO_Agent_Type_e type = (enum V2XHO_Agent_Type_e)ret;
                                                        
                                                        switch(type)
                                                        {
                                                            case HomeAgent:
                                                            {
                                                                state->recv = Advertisement_From_Home;
                                                                break;
                                                            }
                                                            case ForeignAgent:
                                                            {
                                                                state->recv = Advertisement_From_Foreign;
                                                                break;
                                                            }
                                                            default: break;
                                                        }
                                                        
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
                                            if(f_i_V2XHO_Receiver_Parser_UDP_Registration_Request(buf, agent_info) == 0)
                                            {
                                                state->recv = Registration_Request_From_Node;
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
                    }

                    default: 
                        break;
                }
            }
            return -1;
            break;
        }
        case Waiting_Advertisement_From_Home:
        {
            struct sockaddr_ll saddrll;
            socklen_t src_addr_len = sizeof(saddrll);
            struct timeval tv_timeo = { (int)(recv_timer/1e6), (recv_timer % 1000) * 1000}; 
            setsockopt(agent_info->recv_socket, SOL_SOCKET, SO_RCVTIMEO, &tv_timeo, sizeof(tv_timeo));
            recv_len = recvfrom(agent_info->recv_socket, buf, 1024, 0, (struct sockaddr *)&(saddrll), &src_addr_len);
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
                            struct in_addr home_agent;
                            if(agent_info->type == HomeAgent)
                            {
                                home_agent = agent_info->agent_info_list[0].agent_addr;
                            }else{
                                for(int n = 1; n < *agent_info->num_agent; n++)
                                {
                                    if(agent_info->agent_info_list[n].type == HomeAgent)
                                    {
                                        home_agent = agent_info->agent_info_list[n].agent_addr;
                                    }
                                }
                            }

                            if(ip->daddr == inet_addr("224.0.0.1") || ip->saddr == home_agent.s_addr)
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
                                                    int ret = (int)f_e_V2XHO_Receiver_Parser_ICMP_Advertisement(buf, agent_info);
                                                    if(ret == 0 || ret == 1 || ret == 2)
                                                    {
                                                        enum V2XHO_Agent_Type_e type = (enum V2XHO_Agent_Type_e)ret;
                                                        
                                                        switch(type)
                                                        {
                                                            case HomeAgent:
                                                            {
                                                            
                                                                state->recv = Advertisement_From_Home;
                                                                break;
                                                            }
                                                            case ForeignAgent:
                                                            {
                                                                state->recv = Advertisement_From_Foreign;
                                                                break;
                                                            }
                                                            default: break;
                                                        }
                                                        
                                                        return 0;
                                                    }
                                                    break;
                                                }
                                            default:break;
                                            }
                                        }
                                        break;
                                    }
                                    default:
                                        break;
                                }
                            }

                        }
                    }
                }
            }
            break;
        }
        default:
            break;
    }
    return -1;
}

 enum V2XHO_Agent_Type_e f_e_V2XHO_Receiver_Parser_ICMP_Advertisement(char *buf, struct V2XHO_Agent_Info_t *agent_info)
{
    if(buf)
    {
        enum V2XHO_Agent_Type_e ret_type, ret;
        uint32_t pointer_now = sizeof(struct ether_header); //14 + 0
        struct iphdr *ip = (struct iphdr *)(buf + pointer_now);
        pointer_now += sizeof(struct iphdr); 
        struct icmphdr *icmp = (struct icmphdr *) (buf + pointer_now);
        pointer_now += sizeof(struct icmphdr) - 4;
        struct V2XHO_0_ICMP_Payload_Header_t *icmp_payload_hdr = (struct V2XHO_0_ICMP_Payload_Header_t *)(buf + pointer_now);
        pointer_now += sizeof(struct V2XHO_0_ICMP_Payload_Header_t); //14 + (24 + 4) = 14 + 28
        struct V2XHO_Router_Addr_es_t router_addrs[icmp_payload_hdr->Num_Addrs];
        for(int i = 0; i < icmp_payload_hdr->Num_Addrs; i++)
        {
            struct V2XHO_Router_Addr_es_t *icmp_payload_data = (struct V2XHO_Router_Addr_es_t *)(buf + pointer_now);
            memcpy(&(router_addrs[i]), icmp_payload_data, sizeof(struct V2XHO_Router_Addr_es_t) - sizeof(uint32_t));
            pointer_now += ((sizeof(struct V2XHO_Router_Addr_es_t) - sizeof(uint32_t))); //14 + (28 + 8 * 2) = 14 + 44
        }
        uint16_t ip_len = htons(ip->tot_len);
        if(ip_len >  pointer_now - sizeof(struct ether_header))
        {
            uint8_t *type = (uint8_t *)(buf + pointer_now);
            uint16_t now_agent;
            if(*type == 16)
            {
                struct V2XHO_extention_0_t *extention_0 = (struct V2XHO_extention_0_t *)(buf + pointer_now);
                uint16_t *num_agent = agent_info->num_agent;
                for(int count_agent = 1; count_agent <= *num_agent; count_agent++)
                {
                     
                    if((agent_info->agent_info_list[count_agent].agent_addr.s_addr == ip->saddr && agent_info->agent_info_list[count_agent].type == HomeAgent) \
                        || count_agent == *num_agent)
                    {
                       now_agent = count_agent;
                        if(count_agent == *num_agent)
                        {
                            agent_info->agent_info_list[count_agent].type = HomeAgent;
                            agent_info->agent_info_list[count_agent].agent_addr.s_addr = ip->saddr;
                            agent_info->agent_info_list[count_agent].num_addrs = calloc(1, sizeof(uint16_t));
                            agent_info->agent_info_list[count_agent].adv_icmp_info.extention_0 = calloc(1, sizeof(struct V2XHO_extention_0_t));
                            (*num_agent)++;
                        }else{
                            F_i_V2XHO_Agent_ICMP_Router_addr_Flush(agent_info->agent_info_list[count_agent].router_addrs, agent_info->agent_info_list[count_agent].num_addrs);
                        }
                        for(int count_router = 0; count_router < icmp_payload_hdr->Num_Addrs; count_router++)
                        {
                            router_addrs[count_router].Preference_Level = htonl(router_addrs[count_router].Preference_Level);
                            F_i_V2XHO_Agent_ICMP_Router_addr_Add(agent_info->agent_info_list[count_agent].router_addrs, agent_info->agent_info_list[count_agent].num_addrs, &router_addrs[count_router]);
                        }
                        agent_info->agent_info_list[count_agent].adv_icmp_info.code = icmp->code;
                        agent_info->agent_info_list[count_agent].adv_icmp_info.lifetime = icmp_payload_hdr->LifeTime;
                        agent_info->agent_info_list[count_agent].num_coa_addrs = (extention_0->Length - 16)/4;
                        memcpy(agent_info->agent_info_list[count_agent].adv_icmp_info.extention_0, extention_0, sizeof(struct V2XHO_extention_0_t));
                        break;
                    }
                }
                uint16_t *num_addrs = agent_info->agent_info_list[now_agent].num_addrs;
                struct V2XHO_IProuter_Route_List_info_IPv4_t route_list[MAX_ROUTE_LIST_NUM];
                int route_list_num = 0;
                ret = F_i_V2XHO_IProuter_Route_Get_List(agent_info->interface_mobileip, route_list, &route_list_num);
                char *ip_address = calloc(IPV4_ADDR_SIZE, sizeof(char));
                char *gateway = calloc(IPV4_ADDR_SIZE, sizeof(char)); 
                printf("*agent_info->agent_info_list[0].num_addrs:%d\n", *agent_info->agent_info_list[0].num_addrs);
                for(int test = 0; test < *agent_info->agent_info_list[0].num_addrs; test++)
                {
                    printf("agent_info->agent_info_list[0].router_addrs[%d]->Route_Address:%08X\n", test, agent_info->agent_info_list[0].router_addrs[test]->Route_Address);
                }
                for(int count_addrs = 0; count_addrs < *num_addrs; count_addrs++)
                {
                    
                    char *interface;
                    char *table = NULL;
                    bool local_gateway, non_forecd;
                    enum v2xho_util_router_metric_state_e metric;
                    int prefix;
                    struct in_addr ip_addr, gateway_addr;
                    printf("agent_info->agent_info_list[now_agent].router_addrs[%d]->Route_Address:%08X\n", count_addrs, agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address);
                    printf("agent_info->agent_info_list[now_agent].router_addrs[%d]->Preference_Level:%d\n", count_addrs, agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Preference_Level);
                    switch(agent_info->type)
                    {
                        default: break;
                        case HomeAgent:
                        {
                            if(extention_0->flag.H == 1)
                            {
                                //printf("[HomeAgent] Home Agent!\n");
                                ret_type = HomeAgent;
                                break;
                            }
                            if(extention_0->flag.F == 1)
                            {
                                //printf("[HomeAgent] Foreign Agent!\n");
                                ret_type = ForeignAgent;
                                switch(agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Preference_Level)
                                {
                                    default:break;
                                    case PREFERENCE_LEVEL_AGENT_FOREIGN_ADDRESS:
                                    case PREFERENCE_LEVEL_AGENT_FOREIGN_SUBNET:
                                    {   
                                        for(int count_router = 0; count_router < route_list_num; count_router++)
                                        {
                                            if(route_list[count_router].dest_addr.s_addr == agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address
                                            || route_list[count_router].gateway_addr.s_addr == agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address)
                                            {
                                                break;
                                            }else{
                                                if(count_router == route_list_num - 1) 
                                                {
                                                    ip_addr.s_addr = (agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address & 0x00FFFFFF);
                                                    memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                                                    memcpy(ip_address, inet_ntoa(ip_addr), IPV4_ADDR_SIZE);
                                                    prefix = 24;
                                                    gateway_addr.s_addr = (agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address);
                                                    memset(gateway, 0x00, IPV4_ADDR_SIZE);
                                                    memcpy(gateway, inet_ntoa(gateway_addr), IPV4_ADDR_SIZE);
                                                    local_gateway = false;
                                                    interface = agent_info->interface_common;
                                                    metric = (int)agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Preference_Level;
                                                    non_forecd = false;
                                                    ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, interface, &metric, table, non_forecd);
                                                    if(ret >= 0)
                                                    {
                                                       agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Preference_Level = PREFERENCE_LEVEL_TUNNEL_EoT_ADDRESS;
                                                        F_i_V2XHO_Agent_ICMP_Router_addr_Add(agent_info->agent_info_list[0].router_addrs, agent_info->agent_info_list[0].num_addrs, agent_info->agent_info_list[now_agent].router_addrs[count_addrs]);
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    case PREFERENCE_LEVEL_CoCoA_ADDRESS:
                                    case PREFERENCE_LEVEL_CoCoA_SUBNET:
                                    {
                                        break;
                                    }
                                    case PREFERENCE_LEVEL_NODE_ADDRESS:
                                    {
                                        for(int count_router = 0; count_router < route_list_num; count_router++)
                                        {
                                            if(route_list[count_router].dest_addr.s_addr == agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address
                                            || route_list[count_router].gateway_addr.s_addr == agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address)
                                            {
                                                if(strncmp(route_list[count_router].dev_name, "ipip", 4) == 0)
                                                { 

                                                }else{
                                                    struct in_addr del_addr = {.s_addr = agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address};
                                                    F_i_V2XHO_Agent_ICMP_Router_addr_Del(agent_info->agent_info_list[0].router_addrs, agent_info->agent_info_list[0].num_addrs, &agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address);
                                                    char cmd[128];
                                                    memset(cmd, 0x00, 128);
                                                    sprintf(cmd, "ip route flush %s dev %s", inet_ntoa(del_addr), agent_info->interface_mobileip);
                                                    system(cmd);                                             
                                                }
                                            }else{
                                                if(count_router == route_list_num - 1) 
                                                {
                                                    char cmd[128];
                                                    memset(cmd, 0x00, 128);
                                                    struct in_addr subnet = {.s_addr = (agent_info->interface_v2xho_addr.sin_addr.s_addr & 0x0000FFFF) + ((agent_info->interface_v2xho_addr.sin_addr.s_addr & 0x00FF0000) * (0x100)) + 0x00F00000 };                                                                memcpy(ip_address, inet_ntoa(agent_info->interface_v2xho_addr.sin_addr), IPV4_ADDR_SIZE);
                                                    memcpy(ip_address, inet_ntoa(agent_info->interface_v2xho_addr.sin_addr), IPV4_ADDR_SIZE);
                                                    memcpy(gateway, inet_ntoa(subnet), IPV4_ADDR_SIZE);
                                                    subnet.s_addr = (ip->saddr);
                                                    sprintf(cmd, "ip link add name ipip%d type ipip local %s remote %s", 0, ip_address, inet_ntoa(subnet));
                                                    system(cmd);
                                                    usleep(10000);
                                                    memset(cmd, 0x00, 128);
                                                    sprintf(cmd, "ip address add %s/32 dev ipip%d", gateway, 0);
                                                    system(cmd);
                                                    memset(cmd, 0x00, 128);
                                                    sprintf(cmd, "ip link set up ipip%d", 0);
                                                    system(cmd);
                                                    subnet.s_addr = agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address;
                                                    memset(cmd, 0x00, 128);
                                                    memcpy(ip_address, inet_ntoa(subnet), IPV4_ADDR_SIZE);
                                                    prefix = 32;
                                                    local_gateway = true;
                                                    memset(cmd, 0x00, 128);
                                                    sprintf(cmd,"ipip%d", 0);
                                                    agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Preference_Level = PREFERENCE_LEVEL_TUNNEL_DEST_ADDRESS;
                                                    metric = (int)agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Preference_Level;
                                                    non_forecd = false;
                                                    ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, cmd, &metric, table, non_forecd);
                                                    if(ret >= 0)
                                                        F_i_V2XHO_Agent_ICMP_Router_addr_Add(agent_info->agent_info_list[0].router_addrs, agent_info->agent_info_list[0].num_addrs, agent_info->agent_info_list[now_agent].router_addrs[count_addrs]);
                                                }
                                            }
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        case ForeignAgent:
                        {
                            if(extention_0->flag.F == 1)
                            {
                                //printf("[ForeignAgent] Foreign Agent!\n");
                                ret_type = ForeignAgent;
                                break;
                            }
                            if(extention_0->flag.H == 1)
                            {
                                //printf("[ForeignAgent] Home Agent!\n");
                                ret_type = HomeAgent;
                                switch(agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Preference_Level)
                                {
                                    default:break;
                                    case PREFERENCE_LEVEL_CORRESPHOND_ADDRESS:
                                    case PREFERENCE_LEVEL_CORRESPHOND_SUBNET:
                                    case PREFERENCE_LEVEL_HOST_UNKNOW_ADDRESS:
                                    case PREFERENCE_LEVEL_HOST_UNKNOW_SUBNET:
                                    case PREFERENCE_LEVEL_AGENT_HOME_ADDRESS:
                                    case PREFERENCE_LEVEL_AGENT_HOME_SUBNET:
                                    {   
                                        for(int count_router = 0; count_router < route_list_num; count_router++)
                                        {
                                            if(route_list[count_router].dest_addr.s_addr == agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address
                                            || route_list[count_router].gateway_addr.s_addr == agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address)
                                            {
                                                break;
                                            }else{
                                                if(count_router == route_list_num - 1) 
                                                {
                                                    struct in_addr subnet = {.s_addr = (agent_info->interface_v2xho_addr.sin_addr.s_addr)};                                                                
                                                    memcpy(gateway, inet_ntoa(subnet), IPV4_ADDR_SIZE);
                                                    memcpy(ip_address, inet_ntoa(subnet), IPV4_ADDR_SIZE);
                                                    prefix = 32;
                                                    local_gateway = false;
                                                    metric = (int)agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Preference_Level;
                                                    non_forecd = false;
                                                    ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, agent_info->interface_common, &metric, table, non_forecd);
                                                    if(ret >= 0)
                                                    {
                                                        F_i_V2XHO_Agent_ICMP_Router_addr_Add(agent_info->agent_info_list[0].router_addrs, agent_info->agent_info_list[0].num_addrs, agent_info->agent_info_list[now_agent].router_addrs[count_addrs]);
                                                        char cmd[128];
                                                        subnet.s_addr = ((agent_info->interface_v2xho_addr.sin_addr.s_addr & 0x0000FFFF) + ((agent_info->interface_v2xho_addr.sin_addr.s_addr & 0x00FF0000) * (0x100)) + 0x00F00000);                                                                
                                                        memcpy(gateway, inet_ntoa(subnet), IPV4_ADDR_SIZE);
                                                        subnet.s_addr = (agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address & 0x00FFFFFF);
                                                        memcpy(ip_address, inet_ntoa(subnet), IPV4_ADDR_SIZE);
                                                        prefix = 24;
                                                        local_gateway = true;
                                                        memset(cmd, 0x00, 128);
                                                        sprintf(cmd,"ipip%d", 0);
                                                        agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address = (agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address & 0x00FFFFFF);
                                                        agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Preference_Level = PREFERENCE_LEVEL_AGENT_HOME_SUBNET;
                                                        metric = (int)agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Preference_Level;
                                                        non_forecd = false;
                                                        ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, cmd, &metric, table, non_forecd);
                                                        if(ret >= 0)
                                                            F_i_V2XHO_Agent_ICMP_Router_addr_Add(agent_info->agent_info_list[0].router_addrs, agent_info->agent_info_list[0].num_addrs, agent_info->agent_info_list[now_agent].router_addrs[count_addrs]);
                                                    }
                                                }
                                            }
                                            
                                        }
                                        break;
                                    }
                                    case PREFERENCE_LEVEL_TUNNEL_EoT_ADDRESS:
                                    case PREFERENCE_LEVEL_TUNNEL_EoT_SUBNET:
                                    {
                                        for(int count_router = 0; count_router < route_list_num; count_router++)
                                        {
                                            if(route_list[count_router].dest_addr.s_addr == agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address)
                                            {   
                                                break;
                                            }else{
                                                if(count_router == route_list_num - 1) 
                                                {
                                                    char cmd[128];
                                                    struct in_addr subnet = {.s_addr = (agent_info->interface_v2xho_addr.sin_addr.s_addr & 0x0000FFFF) + ((agent_info->interface_v2xho_addr.sin_addr.s_addr & 0x00FF0000) * (0x100)) + 0x00F00000 };                                                               
                                                    memset(cmd, 0x00, 128);
                                                    memcpy(ip_address, inet_ntoa(agent_info->interface_v2xho_addr.sin_addr), IPV4_ADDR_SIZE);
                                                    memcpy(gateway, inet_ntoa(subnet), IPV4_ADDR_SIZE);
                                                    subnet.s_addr = (ip->saddr);
                                                    sprintf(cmd, "ip link add name ipip%d type ipip local %s remote %s", 0, ip_address, inet_ntoa(subnet));
                                                    system(cmd);
                                                    usleep(10000);
                                                    memset(cmd, 0x00, 128);
                                                    sprintf(cmd, "ip address add %s/32 dev ipip%d", gateway, 0);
                                                    system(cmd);
                                                    memset(cmd, 0x00, 128);
                                                    sprintf(cmd, "ip link set up ipip%d", 0);
                                                    system(cmd);
                                                    memcpy(gateway, inet_ntoa(subnet), IPV4_ADDR_SIZE);
                                                    subnet.s_addr = (agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address);
                                                    memcpy(ip_address, inet_ntoa(subnet), IPV4_ADDR_SIZE);
                                                    prefix = 32;
                                                    local_gateway = false;
                                                    metric = (int)agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Preference_Level;
                                                    non_forecd = false;
                                                    ret = F_i_V2XHO_IProuter_Route_Add(gateway, &prefix, gateway, local_gateway, agent_info->interface_common, &metric, table, non_forecd);
                                                    if(ret >= 0)
                                                        F_i_V2XHO_Agent_ICMP_Router_addr_Add(agent_info->agent_info_list[0].router_addrs, agent_info->agent_info_list[0].num_addrs, agent_info->agent_info_list[now_agent].router_addrs[count_addrs]);
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    }
                                    case PREFERENCE_LEVEL_NODE_ADDRESS:
                                    case PREFERENCE_LEVEL_TUNNEL_DEST_ADDRESS:
                                    {
                                        for(int count_router = 0; count_router < route_list_num; count_router++)
                                        {
                                            if(route_list[count_router].dest_addr.s_addr == agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address
                                            || route_list[count_router].gateway_addr.s_addr == agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address)
                                            {  
                                                struct in_addr del_addr = {.s_addr = agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address};
                                                F_i_V2XHO_Agent_ICMP_Router_addr_Del(agent_info->agent_info_list[0].router_addrs, agent_info->agent_info_list[0].num_addrs, &agent_info->agent_info_list[now_agent].router_addrs[count_addrs]->Route_Address);
                                                char cmd[128];
                                                memset(cmd, 0x00, 128);
                                                sprintf(cmd, "ip route flush %s", inet_ntoa(del_addr));
                                                system(cmd);
                                            }
                                        }
                                        break;
                                    }
                                }
                                break;
                            }  
                            break;
                        }
                    }
                }    
                V2XHO_safefree(ip_address, IPV4_ADDR_SIZE);
                V2XHO_safefree(gateway, IPV4_ADDR_SIZE);
                for(int count_coa_addrs = 0; count_coa_addrs < agent_info->agent_info_list[now_agent].num_coa_addrs;)
                {
                    for(int count_router = 0; count_router < route_list_num; count_router++)
                    {   
                        if(agent_info->agent_info_list[now_agent].adv_icmp_info.extention_0->Care_Of_Addresses[count_coa_addrs] == route_list[count_router].gateway_addr.s_addr)
                        {
                            count_coa_addrs++;
                        }
                        if(agent_info->agent_info_list[now_agent].adv_icmp_info.extention_0->Care_Of_Addresses[count_coa_addrs] == route_list[count_router].dest_addr.s_addr)
                        {
                            count_coa_addrs++;
                        }
                    }
                    if(count_coa_addrs < agent_info->agent_info_list[now_agent].num_coa_addrs)
                    {                
                     
                        count_coa_addrs++;
                    }
                    if(count_coa_addrs >= *num_addrs)
                    {
                        break;
                    }
                }
            }
        }
        if(ret < 0)
            printf("ret:%d\n", ret);
        return ret_type;
    }else{
        return -1;
    }
}     

int F_i_V2XHO_Agent_Route_Add(struct V2XHO_Agent_Info_t *agent_info, enum V2XHO_Ip_Dst_Type_e dst_type, char *address, int prefix, char *gateway_addr, bool local_gateway, enum v2xho_util_router_metric_state_e metric, char *table, bool NonFored)
{
    int ret;
    char *gateway;

    if(local_gateway)
    {
        gateway = inet_ntoa(agent_info->interface_v2xho_addr.sin_addr);
    }else{
        gateway = gateway_addr;
    }

    switch(dst_type)
    {
        case unicast:
        {
            char *ip_address = address;
            ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, agent_info->interface_mobileip, &metric, table, NonFored); 
            break;
        }
        case multicast:
        {
            char *adv_ip_address = "224.0.0.0";
            prefix = 24;
            ret = F_i_V2XHO_IProuter_Route_Add(adv_ip_address, &prefix, gateway, local_gateway, agent_info->interface_mobileip, &metric, table, NonFored); 
            printf("[%s][%d]%d\n", __func__, __LINE__, ret);
            break;
        }
        case broadcast:
            break;
        default:
        {            
            char *ip_address = "0.0.0.0";
            prefix = 0;//0
            metric = 1024;
            ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, gateway, local_gateway, agent_info->interface_mobileip, &metric, table, NonFored); 
            break;
        }

    }
    return ret;
}

int F_i_V2XHO_Agent_Interface_Addr_Add(struct V2XHO_Agent_Info_t *agent_info, enum V2XHO_Ip_Dst_Type_e dst_type, char *address, int prefix)
{
    int ret;
    switch(dst_type)
    {
        case unicast:
        {
            char *ip_address = address;
            ret = F_i_V2XHO_IProuter_Address_Set(agent_info->interface_mobileip, ip_address, &prefix);
            break;
        }
        case multicast:
        {
            char *adv_ip_address = "224.0.0.0";
            ret = F_i_V2XHO_IProuter_Address_Set(agent_info->interface_mobileip, adv_ip_address, &prefix);
            break;
        }
        case broadcast:
            break;
    }
    return ret;
}

int F_i_V2XHO_Agent_ICMP_Router_addr_Add(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs, struct V2XHO_Router_Addr_es_t *router_addrs)
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
        (*num_addrs)++;
    }else
    {

        return -1;
    }
    return 0;   
}

int F_i_V2XHO_Agent_ICMP_Router_addr_Del(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs,  uint32_t *router_addrs)
{
    if(router_addrs)
    {
        for(int i = 0; i < *num_addrs; i++)
        {
            if(router_es[i]->Route_Address - *router_addrs == 0)
            {
                printf("router_es[%d]->Route_Address:%08X\n", i, router_es[i]->Route_Address);
                router_es[i]->Route_Address = 0;
                router_es[i]->Preference_Level = 0;
                if(i == *num_addrs - 1)
                {
                    V2XHO_safefree(router_es[i], sizeof(struct V2XHO_Router_Addr_es_t ));
                    (*num_addrs)--;
                    return 0;
                }else{
                    for(int j = i; j < *num_addrs - 1; j++)
                    {
                        router_es[j]->Route_Address =  router_es[j + 1]->Route_Address;
                        router_es[j]->Preference_Level =  router_es[j + 1]->Preference_Level;
                    }
                    router_es[*num_addrs - 1]->Route_Address =  0;
                    router_es[*num_addrs - 1]->Preference_Level =  0;
                    V2XHO_safefree(router_es[*num_addrs - 1], sizeof(struct V2XHO_Router_Addr_es_t ));
                    (*num_addrs)--;
                    return 0;
                }
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

int F_i_V2XHO_Agent_ICMP_Router_addr_Flush(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs)
{
    if(1)
    {
        while(*num_addrs > 0)
        {
            (*num_addrs)--;
            router_es[*num_addrs]->used = true;
            router_es[*num_addrs]->Route_Address = 0;
            router_es[*num_addrs]->Preference_Level = 0;
            V2XHO_safefree(router_es[*num_addrs], sizeof(struct V2XHO_Router_Addr_es_t ));
        }
    
    }else
    {
        return -1;
    }
    return 0;   
}

int F_i_V2XHO_Agent_Set_ICMP_Preference_Level(struct V2XHO_Agent_Info_t *agent_info,  uint32_t *router_addrs, int Preference_Level)
{
    if(router_addrs)
    {
        for(int i = 0; i < *agent_info->agent_info_list[0].num_addrs; i++)
        {
            if(agent_info->agent_info_list[0].router_addrs[i]->Route_Address - *router_addrs == 0)
            {
                agent_info->agent_info_list[0].router_addrs[i]->used = true;
                agent_info->agent_info_list[0].router_addrs[i]->Preference_Level = Preference_Level;
                agent_info->agent_info_list[0].router_addrs[i]->used = false;
                break;
            }
        }
    }else
    {
        return -1;
    }
    return 0;   
}


int f_i_V2XHO_Receiver_Parser_UDP_Registration_Request(char *buf, struct V2XHO_Agent_Info_t *agent_info)
{
    if(buf)
    {
        uint32_t pointer_now = sizeof(struct ether_header);
        struct iphdr *ip = (struct iphdr *)(buf + pointer_now);
        pointer_now = pointer_now + sizeof(struct iphdr);
        struct udphdr *udp = (struct udphdr *)(buf + pointer_now);
        uint16_t ip_len = htons(ip->tot_len);
        uint16_t udp_len = htons(udp->len);
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
        struct V2XHO_0_UDP_Payload_Header_t *udp_payload = (struct V2XHO_0_UDP_Payload_Header_t *)(buf + pointer_now);
        uint16_t now_node;
        if(udp_payload->type == 1)
        {
            for(int count_node = 0; count_node <= *agent_info->num_nodes; count_node++)
            {   
                now_node = count_node;
                if(count_node == *agent_info->num_nodes)
                {
                    memset(&agent_info->registed_node_list[*agent_info->num_nodes], 0x00, sizeof(struct V2XHO_Agent_Registed_Node_Info_t));
                    agent_info->registed_node_list[*agent_info->num_nodes].state = Registration_Request_To_Agent;
                    agent_info->registed_node_list[*agent_info->num_nodes].node_address.s_addr = (ip->saddr);
                    agent_info->registed_node_list[*agent_info->num_nodes].src_port = udp->dest;
                    agent_info->registed_node_list[*agent_info->num_nodes].dest_port = udp->source;
                    agent_info->registed_node_list[*agent_info->num_nodes].reply_code = Registration_Accepted;
                    memcpy(&(agent_info->registed_node_list[*agent_info->num_nodes].udp_payload), udp_payload, sizeof(struct V2XHO_0_UDP_Payload_Header_t)); 
                    *agent_info->num_nodes = *agent_info->num_nodes + 1;
                    break;
                }
                if(agent_info->registed_node_list[count_node].node_address.s_addr == (ip->saddr))
                {
                    break;
                }
                if(agent_info->registed_node_list[count_node].state == Solicitation_To_Agent)
                {
                    if(agent_info->registed_node_list[count_node].node_address.s_addr == (ip->saddr))
                    {

                    }
                    break;
                }else if(agent_info->registed_node_list[count_node].state == Registration_Request_To_Agent)
                {
                    if(agent_info->registed_node_list[count_node].node_address.s_addr == ip->saddr)
                    {

                    }
                    break;
                }
            }
            struct V2XHO_IProuter_Route_List_info_IPv4_t route_list[MAX_ROUTE_LIST_NUM];
            int route_list_num = 0;
            int ret;
            ret = F_i_V2XHO_IProuter_Route_Get_List(agent_info->interface_mobileip, route_list, &route_list_num);
            struct in_addr ip_addr;        
            ip_addr.s_addr = (agent_info->registed_node_list[now_node].udp_payload.home_address);
            if(ip_addr.s_addr != 0x00000000)
            {
                for(int count_router = 0; count_router < route_list_num; count_router++)
                {
                    if(route_list[count_router].dest_addr.s_addr == ip_addr.s_addr
                    || route_list[count_router].gateway_addr.s_addr == ip_addr.s_addr)
                    {
                        if(strncmp(route_list[count_router].dev_name, "ipip", 4) == 0)
                        { 
                            printf("route_list[count_router].dev_name:%s\n", route_list[count_router].dev_name);
                            printf("node_addresss:%s\n", inet_ntoa(ip_addr));
                            char cmd[128];
                            memset(cmd, 0x00, 128);                        
                            sprintf(cmd, "ip route flush %s", inet_ntoa(ip_addr));
                            system(cmd);
                            if(agent_info->type == HomeAgent)
                            {
                                memset(cmd, 0x00, 128);                        
                                sprintf(cmd, "ip link del %s", route_list[count_router].dev_name);
                                system(cmd);
                            }
                        }else{
                            break;
                        }
                    }else{
                        if(count_router == route_list_num - 1) 
                        {
                            char *ip_address = calloc(IPV4_ADDR_SIZE, sizeof(char));
                            char *interface = agent_info->interface_mobileip;
                            char *table = NULL;
                            bool local_gateway, non_forecd;
                            enum v2xho_util_router_metric_state_e metric;
                            int prefix;
                            memset(ip_address, 0x00, IPV4_ADDR_SIZE);
                            memcpy(ip_address, inet_ntoa(ip_addr), IPV4_ADDR_SIZE);
                            prefix = 32;
                            local_gateway = true;
                            metric = (int)((enum V2XHO_Preference_Level_e) PREFERENCE_LEVEL_NODE_ADDRESS);
                            non_forecd = false;
                            printf("ip_address:%s\n", ip_address);
                            printf("gateway_address%s\n",  inet_ntoa(agent_info->interface_v2xho_addr.sin_addr));
                            printf("interface%s\n", interface);
                            ret = F_i_V2XHO_IProuter_Route_Add(ip_address, &prefix, inet_ntoa(agent_info->interface_v2xho_addr.sin_addr), local_gateway, interface, &metric, table, non_forecd);
                            if(ret >= 0)
                            {
                                struct V2XHO_Router_Addr_es_t router_addrs;
                                router_addrs.Route_Address = ip_addr.s_addr;
                                router_addrs.Preference_Level = metric;
                                F_i_V2XHO_Agent_ICMP_Router_addr_Add(agent_info->agent_info_list[0].router_addrs, agent_info->agent_info_list[0].num_addrs, &router_addrs);
                            }
                            V2XHO_safefree(ip_address, IPV4_ADDR_SIZE);
                            agent_info->agent_state->now = Operation_Advertisement_To_Agent;
                        }
                    }
                }
            }
        }else
        {
            return -1;
        }
    }
    return 0;
}     

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
            if(ip->tot_len == pointer_now + *iphdr_option_len + udp_len)
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

uint8_t f_u8_V2XHO_Receiver_Parser_ICMP_Solicitation(char *buf, struct V2XHO_Agent_Info_t *agent_info)
{
    if(buf)
    {
        uint32_t pointer_now = sizeof(struct ether_header);
        struct iphdr *ip = (struct iphdr *)(buf + pointer_now);
        agent_info->registed_node_list[*agent_info->num_nodes].state = Solicitation_To_Agent;
        agent_info->registed_node_list[*agent_info->num_nodes].node_address.s_addr = ip->saddr;
        *agent_info->num_nodes =  *agent_info->num_nodes + 1;
        return *agent_info->num_nodes;
    }else{
        return -1;
    }
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
