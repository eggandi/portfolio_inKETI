#include "v2x_handover_main.h"

#include <netinet/ip_icmp.h>

uint16_t f_u16_V2XHO_Util_PingTest_Checksum(void *b, uint32_t len) {
    uint16_t *buf = b;
    uint32_t sum = 0;
    uint16_t result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

long f_l_V2XHO_Util_PingTest_Get_Time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000) + (tv.tv_usec);
}

extern void *Th_v_V2XHO_Util_Pingtest_Task_Running(void *d)
{   
    struct v2xho_debug_pingtest_info_t *ping_info = (struct v2xho_debug_pingtest_info_t *)d;

    const char *target_ip = ping_info->dest_addr;
    int sockfd;
    struct sockaddr_in target_addr;
    struct timeval timeout = {1, 0};  // 1초 타임아웃

    uint32_t echo_timestamp_len = 0;
    uint32_t icmp_payload_len = ping_info->packet_size - echo_timestamp_len;
    uint32_t icmp_pkt_len = sizeof(struct icmphdr) + echo_timestamp_len + icmp_payload_len;
    char *icmp_pkt = malloc(sizeof(char) * icmp_pkt_len);
    struct icmphdr *icmp_hdr = (struct icmphdr*)icmp_pkt;
    char *icmp_timestamp = icmp_pkt + sizeof(struct icmphdr);
    char *icmp_payload = icmp_pkt + sizeof(struct icmphdr) + echo_timestamp_len;

    int32_t  time_fd;
	struct itimerspec itval;
    if(G_gnss_data->status.unavailable == true)
	{
        if ((time_fd = timerfd_create(CLOCK_MONOTONIC, 0)) < 0) 
        {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }
	}else{
        if ((time_fd = timerfd_create(CLOCK_MONOTONIC, 0)) < 0) 
        {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }
	}
    itval.it_value.tv_sec = 1;
    itval.it_value.tv_nsec = 0;
    itval.it_interval.tv_sec = (ping_info->interval_ms / 1000);
    itval.it_interval.tv_nsec = (ping_info->interval_ms % 1000) * 1e6;

    timerfd_settime(time_fd, TFD_TIMER_ABSTIME, &itval, NULL);
    // 소켓 생성
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 타임아웃 설정
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    // 대상 주소 설정
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    inet_pton(AF_INET, target_ip, &target_addr.sin_addr);

    // ICMP Echo Request 패킷 생성
    memset(icmp_hdr, 0, sizeof(struct icmphdr));
    //icmp_hdr.icmp_type = ICMP_ECHO;
    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->code = 0;
    icmp_hdr->un.echo.id = getpid() & 0xFFFF;
    icmp_hdr->un.echo.sequence = 1;
    icmp_hdr->checksum  = 0;
    memset(icmp_timestamp, 0, echo_timestamp_len);
    memset(icmp_payload, 0x00, icmp_payload_len);
    if(icmp_payload_len > 0)
    {
        unsigned char temp_input = 0x00;
        for(uint32_t  i = 0; i < icmp_payload_len; i++)
        {
            icmp_payload[i] = temp_input;
            temp_input++; 
        }
    }
    uint16_t payload_checksum = f_u16_V2XHO_Util_PingTest_Checksum((void*)icmp_payload, icmp_payload_len);
    uint64_t res;
    int ret;
    int icmp_seq_num = 0;
    char *recv_buf = malloc(sizeof(char) * (icmp_pkt_len + 128));

    while(1)
    {
        ret = read(time_fd, &res, sizeof(res));
        if(ret > 0)
        {
            icmp_hdr->un.echo.sequence = icmp_seq_num + 1;
            icmp_seq_num++;
            icmp_hdr->checksum  = 0;
            memset(icmp_timestamp, 0, echo_timestamp_len);

            icmp_hdr->checksum  = f_u16_V2XHO_Util_PingTest_Checksum((void*)icmp_pkt, icmp_pkt_len);
            long start_time = f_l_V2XHO_Util_PingTest_Get_Time_ms();

            // ICMP 패킷 전송
            if (sendto(sockfd, icmp_pkt, icmp_pkt_len, 0, (struct sockaddr *)&target_addr, sizeof(target_addr)) <= 0) 
            {
                perror("Send failed");
                continue;
            }
            
            // 응답 대기
            socklen_t addr_len = sizeof(target_addr);
            memset(recv_buf, 0x00, (icmp_pkt_len + 128));
            ssize_t n = recvfrom(sockfd, recv_buf, (icmp_pkt_len + 128), 0, (struct sockaddr *)&target_addr, &addr_len);
            long end_time = f_l_V2XHO_Util_PingTest_Get_Time_ms();
            if (n > 0) 
            {
                struct iphdr *ip_header = (struct iphdr *)recv_buf;
                struct icmp *icmp_resp = (struct icmp *)(recv_buf + sizeof(struct iphdr));

                if (icmp_resp->icmp_type == ICMP_ECHOREPLY && icmp_resp->icmp_id == icmp_hdr->un.echo.id) 
                //if (icmp_resp->icmp_type == ICMP_TIMESTAMPREPLY && icmp_resp->icmp_id == icmp_hdr->un.echo.id) 
                {
                    unsigned char *icmp_recv_payload = (unsigned char *)(ip_header + sizeof(struct iphdr) + sizeof(struct icmphdr));
                    #if 0
                    uint32_t originate, receive, transmit;

                    memcpy(&originate, icmp_recv_payload, 4);
                    memcpy(&receive, icmp_recv_payload + 4, 4);
                    memcpy(&transmit, icmp_recv_payload + 8, 4);

                    printf("Originate Timestamp: %u\n", ntohl(originate));
                    printf("Receive Timestamp: %u\n", ntohl(receive));
                    printf("Transmit Timestamp: %u\n", ntohl(transmit));
                    #endif
                    printf("n:%d\n",n);
                    for(int k = 0; k < (int)(n - sizeof(struct iphdr) - sizeof(struct icmphdr) - echo_timestamp_len); k++)
                    {
                        printf("%02X", icmp_recv_payload[k]);
                        if(k % 64 == 63)
                        {
                            printf("\n");
                        }
                    }
                    printf("\n");
                    uint16_t recv_payload_checksum  = f_u16_V2XHO_Util_PingTest_Checksum((void*)icmp_recv_payload + echo_timestamp_len, n - sizeof(struct iphdr) - sizeof(struct icmphdr) - echo_timestamp_len);
                    uint32_t lat = 0;
                    uint32_t lon = 0;
                    uint32_t elev = 0;
                    switch(G_gnss_data->fix.mode)
                    {
                        default: break;
                        case MODE_3D:
                        {
                             elev = G_gnss_data->elev;
                        }
                        case MODE_2D:
                        {
                            lat = G_gnss_data->lat;
                            lon = G_gnss_data->lon;
                            break;
                        }
                        case MODE_NO_FIX:
                        {
                            lat = G_gnss_data->lat_raw;
                            lon = G_gnss_data->lon_raw;
                            break;
                        }
                    }
                    printf( "Reply from %s: seq=%08d RTT=%06ld[us]" " ," "Location: lat=%u, lon:%u, elev:%u" " ," "PCRC: %04X/%04X" "\n", 
                            target_ip, icmp_resp->icmp_seq, end_time - start_time,
                            lat, lon, elev,
                            recv_payload_checksum, payload_checksum);
                }
            } else {
                printf("Request timed out.\n");
            }
        }

    }
    lSafeFree(recv_buf);
    lSafeFree(icmp_payload);
    close(time_fd);
    close(sockfd);
    return 0;
}

#if 0
char *f_c_V2XHO_Util_PingTest_Parsing_Argument(const char *argv)
{

    char p;
    return NULL;
}

void *th_v_V2XHO_Util_PingTest_Task_Get_Input(void *intput_value)
{
    char a[128];
    char *ret_c;
    while(1)
    {
        printf("V2XHO>");
        ret_c = fgets(a, sizeof(a), stdin);
        if(ret_c != NULL)
        {
            while(a =="\n")
            {
                a = f_c_V2XHO_Util_PingTest_Parsing_Argument(a);

            }
        }
    }
}
#endif