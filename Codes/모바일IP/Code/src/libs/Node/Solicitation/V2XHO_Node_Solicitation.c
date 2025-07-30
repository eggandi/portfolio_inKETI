#include <V2XHO_Node_Solicitation.h>

inline static int f_i_V2XHO_Node_Solicitation_Set_Packet(struct V2XHO_IP_Header_Info_t ip_hdr_info, struct V2XHO_Node_Info_t *node_info, char *out_buf);

int F_i_V2XHO_Node_Solicitation_Do(struct V2XHO_Node_Info_t *node_info, struct V2XHO_IP_Header_Info_t ip_hdr_info)
{
    int ret;
    char *packet = malloc(sizeof(char) * SOLICITATION_PACKET_LEGNTH);
    memset(packet, 0x00, SOLICITATION_PACKET_LEGNTH);
    int packet_length = f_i_V2XHO_Node_Solicitation_Set_Packet(ip_hdr_info, node_info, packet);

    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(SOLICITATION_PORT);
    sock_addr.sin_addr.s_addr = inet_addr(ip_hdr_info.ip_dst_addr);
    memset(&sock_addr.sin_zero, 0, sizeof (sock_addr.sin_zero));
    ret = sendto(node_info->send_socket, packet, packet_length, 0, (struct sockaddr*)&(sock_addr), sizeof(sock_addr));
    if(ret < 0)
    {
        return ret;
    }
    V2XHO_safefree(packet, SOLICITATION_PACKET_LEGNTH);
    return 0;
}

int f_i_V2XHO_Node_Solicitation_Set_Packet( struct V2XHO_IP_Header_Info_t ip_hdr_info, \
                                          struct V2XHO_Node_Info_t *node_info, \
                                          char *out_buf)
{  
    char *packet = out_buf;
    (void )node_info;
    int packet_position_now = 0 ;
    struct iphdr *ip = (struct iphdr *) packet;
    ip->version  = 4;
    ip->ihl      = 5;
    ip->tos      = ip_hdr_info.ip_tos; // normal
    ip->tot_len  = 0;
    ip->id       = ip_hdr_info.ip_id;
    //ip->frag_off = 0b010;
    ip->ttl      = 1; // hops
    ip->protocol = 1; // ICMP
    ip->check = 0;
    //ip->saddr = src_addr;
    switch(ip_hdr_info.ip_dst_type)
    {
        case multicast:
            ip->protocol = 2; // IGMP
            ip->ttl = 1;
            ip->daddr = inet_addr("224.0.0.11"); 
            break;
        case broadcast:
            ip->daddr = inet_addr(BROADCAST_ADDRESS); 
            break;
        default:
            return -1;
            break;
    }
    packet_position_now = packet_position_now + sizeof(struct iphdr);

    struct icmphdr *icmp = (struct icmphdr *) (packet + packet_position_now);
     memset(icmp, 0x00, sizeof(struct icmphdr));
    icmp->type = ICMP_HOST_ANO; //10
    icmp->code = 0;
    icmp->checksum = 0;
    packet_position_now = packet_position_now + sizeof(struct icmphdr);

    ip->tot_len = packet_position_now;
    ip->check = F_u16_V2XHO_Checksum((unsigned short *) packet, sizeof(struct iphdr) / 2);
    icmp->checksum  = F_u16_V2XHO_Checksum((unsigned short *)(packet + sizeof(struct iphdr)), (packet_position_now - sizeof(struct iphdr)) / 2);
    return ip->tot_len;
}
