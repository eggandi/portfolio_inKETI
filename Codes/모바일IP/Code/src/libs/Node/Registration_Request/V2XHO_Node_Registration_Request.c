#include <V2XHO_Node_Registration_Request.h>

inline static int f_i_V2XHO_Node_Registration_Request_Set_Packet(struct V2XHO_IP_Header_Info_t ip_hdr_info, struct V2XHO_Node_Info_t *node_info, char *out_buf);

int F_i_V2XHO_Node_Registration_Request_Do(struct V2XHO_Node_Info_t *node_info, struct V2XHO_IP_Header_Info_t ip_hdr_info)
{
    int ret;
    char *packet = malloc(sizeof(char) * REGISTRATION_PACKET_LEGNTH);
    memset(packet, 0x00, REGISTRATION_PACKET_LEGNTH);
    int packet_length = f_i_V2XHO_Node_Registration_Request_Set_Packet(ip_hdr_info, node_info, packet);

    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(434);
    sock_addr.sin_addr.s_addr =  inet_addr(ip_hdr_info.ip_dst_addr);
    memset(&sock_addr.sin_zero, 0, sizeof (sock_addr.sin_zero));
    printf("%s\n", ip_hdr_info.ip_src_addr);
    printf("%s\n", ip_hdr_info.ip_dst_addr);
    ret = sendto(node_info->send_socket, packet, packet_length, 0, (struct sockaddr*) &sock_addr, sizeof (sock_addr));
    printf("ret:%d\n", ret);
    V2XHO_safefree(packet, REGISTRATION_PACKET_LEGNTH);
    return 0;
}


int f_i_V2XHO_Node_Registration_Request_Set_Packet( struct V2XHO_IP_Header_Info_t ip_hdr_info, \
                                                  struct V2XHO_Node_Info_t *node_info, \
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
    ip->ttl      = 1; // hops
    ip->protocol = 17; // UDP
    ip->check = 0;
    ip->saddr = inet_addr(ip_hdr_info.ip_src_addr);
    ip->daddr = inet_addr(ip_hdr_info.ip_dst_addr);
    packet_position_now = packet_position_now + sizeof(struct iphdr);

    struct udphdr *udp = (struct udphdr *) (packet + packet_position_now);
    udp->source = htons(434);
    udp->dest = htons(434);
    udp->len = packet_position_now;
    udp->check = 0;
    packet_position_now = packet_position_now + sizeof(struct udphdr);
    struct V2XHO_0_UDP_Payload_Header_t *udp_payload_hdr = (struct V2XHO_0_UDP_Payload_Header_t *) (packet + packet_position_now);
    int16_t t = 1800;
    udp_payload_hdr->type = 1;
    udp_payload_hdr->flag.S = 1;
    udp_payload_hdr->flag.D = 0;
    udp_payload_hdr->flag.B = 0;
    udp_payload_hdr->flag.M = 0;
    udp_payload_hdr->flag.G = 0;
    udp_payload_hdr->flag.r = 0;
    udp_payload_hdr->flag.T = 0;
    udp_payload_hdr->flag.x = 0;
    udp_payload_hdr->lifetime  = htons(t);
    V2XHO_DEBUG_1 
    printf("node_info->home_address_info.home_addr.s_addr:%08X\n", node_info->home_address_info.home_addr.s_addr);
    udp_payload_hdr->home_address = node_info->home_address_info.home_addr.s_addr;
    udp_payload_hdr->home_agent = ip->daddr;
    udp_payload_hdr->care_of_address = node_info->coa_address_info.foreign_agent_addr.s_addr;
    udp_payload_hdr->identification =  time(NULL);
    packet_position_now = packet_position_now + sizeof(struct V2XHO_0_UDP_Payload_Header_t);

    uint8_t *num_agent = node_info->num_agent;
    for(int i = 0; i < *num_agent; i++)
    {
        if(node_info->agent_list[i].agent_addr.s_addr == ip->saddr)
        {
            memcpy(&node_info->agent_list[i].udp_recv_payload, udp_payload_hdr, sizeof(struct V2XHO_0_UDP_Payload_Header_t));
        }
    } 

    ip->tot_len = packet_position_now;
    udp->len = htons(ip->tot_len  - udp->len);
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
    udp->check = F_u16_V2XHO_Checksum((unsigned short *)(udp_checksum_field), (18 + sizeof(struct V2XHO_0_UDP_Payload_Header_t))/2);
    V2XHO_safefree(udp_checksum_field, sizeof(struct v2xho_udp_checksum_t));

    return ip->tot_len;
}
