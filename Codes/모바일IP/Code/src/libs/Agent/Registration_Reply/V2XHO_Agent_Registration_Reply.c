#include <V2XHO_Agent_Registration_Reply.h>

static int f_i_V2XHO_Agent_Registration_Reply_Set_Packet(struct V2XHO_Agent_Info_t *agent_info, struct V2XHO_Agent_Registed_Node_Info_t *node_info, struct V2XHO_IP_Header_Info_t ip_hdr_info, struct V2XHO_extention_3_t *extention_3, char *out_buf);

int F_i_V2XHO_Agent_Registration_Reply_Do(struct V2XHO_Agent_Info_t *agent_info, int node_num, struct V2XHO_IP_Header_Info_t ip_hdr_info)
{

    struct V2XHO_Agent_Registed_Node_Info_t *udp_info = (struct V2XHO_Agent_Registed_Node_Info_t *)&(agent_info->registed_node_list[node_num]);

    char *packet = malloc(sizeof(char) * REPLY_PACKET_LEGNTH);
    memset(packet, 0x00, REPLY_PACKET_LEGNTH);
    int packet_length = f_i_V2XHO_Agent_Registration_Reply_Set_Packet(agent_info, udp_info, ip_hdr_info, NULL, packet);
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = udp_info->dest_port;
    sock_addr.sin_addr.s_addr = inet_addr(ip_hdr_info.ip_dst_addr);
    memset(&sock_addr.sin_zero, 0, sizeof (sock_addr.sin_zero));
    
    sendto(agent_info->send_socket, packet, packet_length, 0, (struct sockaddr*) &sock_addr, sizeof (sock_addr));
    V2XHO_safefree(packet, REPLY_PACKET_LEGNTH);
    return 0;
}


int f_i_V2XHO_Agent_Registration_Reply_Set_Packet( struct V2XHO_Agent_Info_t *agent_info, \
                                                 struct V2XHO_Agent_Registed_Node_Info_t *node_info, \
                                                 struct V2XHO_IP_Header_Info_t ip_hdr_info, \
                                                 struct V2XHO_extention_3_t *extention_3, \
                                                 char *out_buf)
{  
    char *packet = out_buf;
    int packet_position_now = 0 ;
    struct iphdr *ip = (struct iphdr *) packet;
    ip->version  = 4;
    ip->ihl      = 5;
    ip->tos      = ip_hdr_info.ip_tos; // normal
    ip->tot_len  = 0;
    ip->id       = ip_hdr_info.ip_id;
    //ip->frag_off = 0b010;
    ip->ttl      = ip_hdr_info.ip_ttl; // hops
    ip->protocol = 17; // UDP
    ip->check = 0;
    ip->saddr = inet_addr(ip_hdr_info.ip_src_addr);
    ip->daddr = inet_addr(ip_hdr_info.ip_dst_addr);

    packet_position_now = packet_position_now + sizeof(struct iphdr);
    struct udphdr *udp = (struct udphdr *) (packet + packet_position_now);
    udp->source = node_info->src_port;
    udp->dest = node_info->dest_port;
    udp->len = packet_position_now;
    udp->check = 0;

    packet_position_now = packet_position_now + sizeof(struct udphdr);
    struct V2XHO_1_UDP_Payload_Header_t *udp_payload_hdr = (struct V2XHO_1_UDP_Payload_Header_t *) (packet + packet_position_now);
    int16_t t = 1800;
    udp_payload_hdr->type = 3;
    udp_payload_hdr->code = (uint8_t)node_info->reply_code;
    if(node_info->home_agent.s_addr == 0x00000000)
    {
        switch(agent_info->type)
        {
            case HomeAgent:
            {
                udp_payload_hdr->home_agent = ip->saddr;
                break;
            }
            case ForeignAgent:
            {
                for(int count_agent = 1; count_agent > *agent_info->num_agent - 1; count_agent++)
                {
                    if(agent_info->agent_info_list[count_agent].type == HomeAgent)
                    {
                        V2XHO_DEBUG_1
                        printf("agent_info->agent_info_list[count_agent].agent_addr.s_addr:%08X\n", agent_info->agent_info_list[count_agent].agent_addr.s_addr);
                        udp_payload_hdr->home_agent = agent_info->agent_info_list[count_agent].agent_addr.s_addr;
                        break;
                    }
                }
                break;
            }
            default: break;
        }
    }else{
        udp_payload_hdr->home_agent = node_info->home_agent.s_addr;
    }
    
    udp_payload_hdr->lifetime  = htons(t);
    udp_payload_hdr->home_address = node_info->node_address.s_addr;
    
    udp_payload_hdr->identification = node_info->udp_payload.identification;
    packet_position_now = packet_position_now + sizeof(struct V2XHO_1_UDP_Payload_Header_t);
    if(extention_3)
    {
        struct V2XHO_extention_3_t *udp_extention_3 = (struct V2XHO_extention_3_t *) (packet + packet_position_now);
        memcpy(udp_extention_3, extention_3, udp_extention_3->Length);
        packet_position_now = packet_position_now + (sizeof(struct V2XHO_extention_3_t) - 6);
        
        if(extention_3->Authenticator)
        {
            udp_extention_3->Authenticator = (uint8_t *) (packet + packet_position_now);
            memcpy(udp_extention_3->Authenticator, extention_3->Authenticator, extention_3->Authenticator_length);
            packet_position_now = packet_position_now + extention_3->Authenticator_length;
        }   
    }
    ip->tot_len = packet_position_now;
    udp->len = htons(ip->tot_len - udp->len);
    ip->check = F_u16_V2XHO_Checksum((unsigned short *) packet, sizeof(struct iphdr) / 2);
    struct v2xho_udp_checksum_t *udp_checksum_field = malloc(sizeof(struct v2xho_udp_checksum_t));
    udp_checksum_field->saddr = ip->saddr;
    udp_checksum_field->daddr = ip->daddr;
    udp_checksum_field->ttl = 0x00;
    udp_checksum_field->protocol = ip->protocol;
    udp_checksum_field->udp_len = udp->len;
    udp_checksum_field->source = udp->source;
    udp_checksum_field->dest = udp->dest;
    udp_checksum_field->length = udp->len;
    memcpy(udp_checksum_field->data, (uint8_t *)udp_payload_hdr, sizeof(struct V2XHO_0_UDP_Payload_Header_t));
    udp->check = F_u16_V2XHO_Checksum((unsigned short *)(udp_checksum_field), (18 + sizeof(struct V2XHO_0_UDP_Payload_Header_t)) / 2);
    V2XHO_safefree(udp_checksum_field, sizeof(struct v2xho_udp_checksum_t));
    return ip->tot_len;
}