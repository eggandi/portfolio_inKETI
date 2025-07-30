#include <V2XHO_Agent_Advertisements.h>

inline static int f_i_V2XHO_Agent_Advertisement_Set_Packet(struct V2XHO_IP_Header_Info_t ip_hdr_info, struct V2XHO_Agent_Info_t *agent_info, struct V2XHO_extention_0_t *extention_0, struct V2XHO_extention_1_t *extention_1, struct V2XHO_extention_2_t *extention_2, char *out_buf);

int F_i_V2XHO_Agent_Advertisement_Do(struct V2XHO_Agent_Info_t *agent_info, struct V2XHO_IP_Header_Info_t ip_hdr_info)
{
    int ret;
    int packet_length;
    char *packet = malloc(sizeof(char) * ADVERTISEMENT_PACKET_LEGNTH);
    memset(packet, 0x00, ADVERTISEMENT_PACKET_LEGNTH);
    struct V2XHO_IProuter_Route_List_info_IPv4_t route_list[MAX_ROUTE_LIST_NUM];
    int route_list_num = 0;
    ret = F_i_V2XHO_IProuter_Route_Get_List(agent_info->interface_mobileip, route_list, &route_list_num);
    for(int count_addrs = 0; count_addrs < *agent_info->agent_info_list[0].num_addrs; count_addrs++)
    {
        for(int count_router = 0; count_router < route_list_num; count_router++)
        {          
            if(route_list[count_router].dest_addr.s_addr == agent_info->agent_info_list[0].router_addrs[count_addrs]->Route_Address
            || route_list[count_router].gateway_addr.s_addr == agent_info->agent_info_list[0].router_addrs[count_addrs]->Route_Address)
            {
                break;
            }else{
                if(count_router == route_list_num - 1) 
                {
                }
            }
        }
    }

    if(agent_info->agent_info_list[0].adv_icmp_info.extention_0)
    {
        packet_length = f_i_V2XHO_Agent_Advertisement_Set_Packet(ip_hdr_info, agent_info, agent_info->agent_info_list[0].adv_icmp_info.extention_0, NULL, NULL, packet);
    }else{
        packet_length = f_i_V2XHO_Agent_Advertisement_Set_Packet(ip_hdr_info, agent_info, NULL, NULL, NULL, packet);
    }
    
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(ADVERTISEMENT_PORT);
    sock_addr.sin_addr.s_addr = inet_addr(ip_hdr_info.ip_dst_addr);
    memset(&sock_addr.sin_zero, 0, sizeof (sock_addr.sin_zero));

    if(strcmp(agent_info->interface_mobileip, agent_info->interface_common) != 0)
    {
        ret = sendto(agent_info->send_socket, packet, packet_length, 0, (struct sockaddr*) &sock_addr, sizeof (sock_addr));

        struct iphdr *ip = (struct iphdr *) packet;
        ip->saddr = agent_info->interface_v2xho_addr.sin_addr.s_addr;
        ret = sendto(agent_info->send_socket_common, packet, packet_length, 0, (struct sockaddr*) &sock_addr, sizeof (sock_addr));
    }
   
    if(ret < 0)
    {
        return ret;
    }
    V2XHO_safefree(packet, ADVERTISEMENT_PACKET_LEGNTH);
    return 0;
}

int f_i_V2XHO_Agent_Advertisement_Set_Packet( struct V2XHO_IP_Header_Info_t ip_hdr_info, \
                                            struct V2XHO_Agent_Info_t *agent_info, \
                                            struct V2XHO_extention_0_t *extention_0, \
                                            struct V2XHO_extention_1_t *extention_1, \
                                            struct V2XHO_extention_2_t *extention_2, \
                                            char *out_buf)
{  
    char *packet = out_buf;
    uint32_t packet_position_now = 0 ;
    struct iphdr *ip = (struct iphdr *) packet;
    ip->version  = 4;
    ip->ihl      = 5;
    ip->tos      = ip_hdr_info.ip_tos; // normal
    ip->tot_len  = 0;
    ip->id       = ip_hdr_info.ip_id;
    //ip->frag_off = 0b010;
    ip->ttl      = ip_hdr_info.ip_ttl; // hops
    ip->protocol = 1; // ICMP
    ip->check = 0;
    //
    switch(ip_hdr_info.ip_dst_type)
    {
        case unicast:
            ip->daddr = inet_addr(ip_hdr_info.ip_dst_addr);
            break;
        case multicast:
            ip->protocol = 2; // IGMP
            //ip->saddr = inet_addr("224.0.0.2");
            ip->daddr = inet_addr("224.0.0.1");
            break;
        case broadcast:
            ip->daddr = inet_addr(BROADCAST_ADDRESS);
            break;
    }
    packet_position_now = packet_position_now + sizeof(struct iphdr);
    ip->check = F_u16_V2XHO_Checksum((unsigned short *)packet, sizeof(struct iphdr) / 2);

    struct icmphdr *icmp = (struct icmphdr *) (packet + packet_position_now);
    icmp->type = ICMP_NET_ANO; //9
    icmp->code = agent_info->icmp_adv_info.code;
    icmp->checksum = 0;
    packet_position_now = packet_position_now + 4;
    struct V2XHO_0_ICMP_Payload_Header_t *icmp_payload_hdr = (struct V2XHO_0_ICMP_Payload_Header_t *) (packet + packet_position_now);
    icmp_payload_hdr->Num_Addrs = *agent_info->agent_info_list[0].num_addrs;
    icmp_payload_hdr->Addr_Entry_Size = 2;
    icmp_payload_hdr->LifeTime  = htons(agent_info->icmp_adv_info.lifetime);
    packet_position_now = packet_position_now + 4;
    
    for(int i = 0; i < icmp_payload_hdr->Num_Addrs; i++)
    {
        struct V2XHO_Router_Addr_es_t *icmp_payload_data = (struct V2XHO_Router_Addr_es_t *) (packet + packet_position_now);
        icmp_payload_data->Route_Address = agent_info->agent_info_list[0].router_addrs[i]->Route_Address;
        icmp_payload_data->Preference_Level = htonl(agent_info->agent_info_list[0].router_addrs[i]->Preference_Level);
        packet_position_now = packet_position_now + (sizeof(struct V2XHO_Router_Addr_es_t) - sizeof(uint32_t));
    }
    if(extention_0)
    {
        struct V2XHO_extention_0_t *icmp_extention_0 = (struct V2XHO_extention_0_t *) (packet + packet_position_now);
        memcpy(icmp_extention_0, extention_0, sizeof(struct V2XHO_extention_0_t) - (sizeof(uint32_t) * 128));
        packet_position_now = packet_position_now + (sizeof(struct V2XHO_extention_0_t) - (sizeof(uint32_t) * 128));
        uint8_t CoA_Num =  (icmp_extention_0->Length - 16)/4;
        for(uint8_t i = 0; i < CoA_Num; i++)
        {
            uint32_t *extention_0_CoA = (uint32_t *) (packet + packet_position_now);
            memcpy(extention_0_CoA, &extention_0->Care_Of_Addresses[i], sizeof(uint32_t)); 
            packet_position_now = packet_position_now + sizeof(uint32_t);
        }
    }
    if(extention_1)
    {
        struct V2XHO_extention_1_t *icmp_extention_1 = (struct V2XHO_extention_1_t *) (packet + packet_position_now);
        memcpy(icmp_extention_1, extention_1, sizeof(struct V2XHO_extention_1_t));
        packet_position_now = packet_position_now + (sizeof(struct V2XHO_extention_1_t));
        int *prefix_length = malloc(4);
        uint32_t Prefix_Num =  (icmp_extention_1->Length);
        for(uint32_t i = 0; i < Prefix_Num; i++)
        {
            uint8_t *extention_1_Prefix = (uint8_t *) (packet + packet_position_now);
            memcpy(extention_1_Prefix, prefix_length + i*(sizeof(uint8_t)), sizeof(uint8_t)); 
            packet_position_now = packet_position_now + sizeof(uint8_t);
        }
        V2XHO_safefree(prefix_length, 4);
    }
    if(extention_2)
    {
        struct V2XHO_extention_2_t *icmp_extention_2 = (struct V2XHO_extention_2_t *) (packet + packet_position_now);
        memcpy(icmp_extention_2, extention_2, sizeof(struct V2XHO_extention_2_t));
        packet_position_now = packet_position_now + (sizeof(struct V2XHO_extention_2_t));
    }
    ip->tot_len = packet_position_now;
    icmp->checksum = F_u16_V2XHO_Checksum((uint16_t *)(icmp), (packet_position_now - sizeof(struct iphdr))/2);
    return ip->tot_len;
}

int F_i_V2XHO_Agent_Advertisement_Set_ICMP_Code(struct V2XHO_Agent_Info_t *agent_info, enum V2XHO_ICMP_Header_Code_e code)
{
    switch(code)
    {
        case 0:
        case 16:
            agent_info->icmp_adv_info.code = code;
            break;
        default:
            return -1;
    }
    return 0;
}
int F_i_V2XHO_Agent_Advertisement_Set_ICMP_LifeTime(struct V2XHO_Agent_Info_t *agent_info, uint16_t *lifetime)
{
    if(lifetime)
    {
        agent_info->icmp_adv_info.lifetime = *lifetime;
    }else
    {
        return -1;
    }
    return 0;
}

struct V2XHO_extention_0_t* F_s_V2XHO_Agent_Advertisements_Initial_Extention_0(struct V2XHO_Agent_Info_t *agent_info, struct V2XHO_extention_0_t *extention_0)
{
    struct V2XHO_extention_0_t *adv_extention_0;
    if(!extention_0)
    {
        adv_extention_0 = malloc(sizeof(struct V2XHO_extention_0_t));
    }else{
        adv_extention_0 = extention_0;
    }
        
    adv_extention_0->Type = 16;
    adv_extention_0->Length = 16;
    adv_extention_0->Sequence_Number = 0;
    adv_extention_0->Registration_LifeTime = DEFAULT_RESTRATION_LIFETIME;
    adv_extention_0->flag.R = 0;
    adv_extention_0->flag.B = 0;
    switch(agent_info->type)
    {
        case HomeAgent:
            adv_extention_0->flag.H = 1;
            adv_extention_0->flag.F = 0;
            break;
        case ForeignAgent:
            adv_extention_0->flag.H = 0;
            adv_extention_0->flag.F = 1;
            adv_extention_0->Care_Of_Addresses[0] = agent_info->agent_info_list[0].agent_addr.s_addr;
            adv_extention_0->Length = adv_extention_0->Length + 4;
            agent_info->agent_info_list[0].num_coa_addrs++;
            break;
        default: break;
    }
    adv_extention_0->flag.M = 0;
    adv_extention_0->flag.G = 0;
    adv_extention_0->flag.r = 0;
    adv_extention_0->flag.T = 0;
    adv_extention_0->flag.U = 0;
    adv_extention_0->flag.X = 0;
    adv_extention_0->flag.I = 0;
    
    return adv_extention_0;
}

