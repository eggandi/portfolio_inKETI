#include "v2x_handover_main.h"

#define V2XHO_ERROR_CODE_FILE_CODE V2XHO_ERROR_CODE_FILE_CODE_RSA_HA

#define V2XHO_PSR_WSA_ID 0
#define V2XHO_DEFAULT_WSA_SEND_NUM 3
#define V2XHO_RSU_HA_RECV_DEFAULT_EPOLL_EVENT_NUM 128


static int f_i_V2XHO_RSU_HA_Send_Registration_Reply(v2xho_obu_ha_registration_info_list_t *send_info, char* recv_buf);
enum v2xho_reg_states_e f_e_V2XHO_Registration_OBU_Add(struct v2xho_rsu_ha_registraion_t *info, const char *add); 
enum v2xho_reg_states_e f_e_V2XHO_Registration_OBU_Del(struct v2xho_rsu_ha_registraion_t *info, const v2xho_obu_ha_registration_info_list_t *del_info); 

struct v2xho_rsu_ha_registraion_t g_rsu_ha_reg_info; 
v2xho_equip_info_t *g_rsu_ha_info; 
static size_t f_s_V2XHO_RSU_HA_WSA_Param_Construct(uint8_t *outbuf, bool security_enable)
{
    struct Dot3ConstructWSAParams wsa_params;
    memset(&wsa_params, 0, sizeof(wsa_params));
    //wsa_params.hdr.msg_id
    //wsa_params.hdr.version = kDot3WSAVersion_Current;
    wsa_params.hdr.wsa_id = V2XHO_PSR_WSA_ID;
    wsa_params.hdr.content_count = 0;
    if(security_enable)
    {}else{
        switch(G_gnss_data->fix.mode)
        {
            default: break;
            case MODE_3D:
            {
                wsa_params.hdr.extensions.threed_location = true;
                wsa_params.hdr.threed_location.latitude = G_gnss_data->lat;
                wsa_params.hdr.threed_location.longitude = G_gnss_data->lon;
                wsa_params.hdr.threed_location.elevation = G_gnss_data->elev;
                wsa_params.hdr.content_count++;
                break;
            }
            case MODE_2D:
            {
                wsa_params.hdr.extensions.twod_location = true;
                wsa_params.hdr.twod_location.latitude = G_gnss_data->lat;
                wsa_params.hdr.twod_location.longitude = G_gnss_data->lon;
                wsa_params.hdr.content_count++;
                break;
            }
            case MODE_NO_FIX:
            {
                wsa_params.hdr.extensions.twod_location = true;
                wsa_params.hdr.twod_location.latitude = G_gnss_data->lat_raw;
                wsa_params.hdr.twod_location.longitude = G_gnss_data->lon_raw;
                wsa_params.hdr.content_count++;
                break;
            }
            case MODE_NOT_SEEN:
            {
                break;
            }     
        }
    }
    
    wsa_params.present.wra = true;
    wsa_params.wra.router_lifetime = 30 * 60;
    memset(&wsa_params.wra.ip_prefix, 0x00, V2XHO_IPV6_ALEN_BIN);
    struct in_addr addr_bin;
    inet_pton(AF_INET, G_v2xho_config.v2xho_interface_IP, &addr_bin);
    memcpy(wsa_params.wra.ip_prefix + (V2XHO_IPV6_ALEN_BIN - V2XHO_IPV4_ALEN_BIN), &addr_bin.s_addr, V2XHO_IPV4_ALEN_BIN);
    wsa_params.wra.ip_prefix_len = V2XHO_IPV6_PREFIX_LEN_COMPATIBLE;
    memset(wsa_params.wra.default_gw, 0x00, IPv6_ALEN);
    memset(wsa_params.wra.primary_dns, 0x00, IPv6_ALEN);
    wsa_params.wra.present.secondary_dns = false;
    wsa_params.wra.present.gateway_mac_addr = true;
    if(wsa_params.wra.present.gateway_mac_addr)
    {
        memset(wsa_params.wra.gateway_mac_addr, 0xff, MAC_ALEN);
        WAL_GetIfMACAddress(0, wsa_params.wra.gateway_mac_addr);
    }
    size_t wsa_size;
    int ret; 
    uint8_t *wsa = Dot3_ConstructWSA(&wsa_params, &wsa_size, &ret);
    if (wsa == NULL) 
    {
        lreturn(V2XHO_EQUIPMENT_RSU_HA_WSA_CONSTRUCT_ERROR);
    }
    memcpy(outbuf, wsa, wsa_size);
    lSafeFree(wsa);
    return wsa_size;
}

extern void *Th_v_V2XHO_RSU_HA_Task_Running_Send(void *d)
{
    d = (void*)d;
    int ret;
    
    int32_t time_fd;
	struct itimerspec itval;
    int msec = 100;
    if(G_gnss_data->status.unavailable == true)
	{
		time_fd = timerfd_create (CLOCK_MONOTONIC, 0);
	}else{
		time_fd = timerfd_create (CLOCK_MONOTONIC, 0);
	}
    itval.it_value.tv_sec = 1;
    itval.it_value.tv_nsec = 0;
    itval.it_interval.tv_sec = 0 + (msec / 1000);
    itval.it_interval.tv_nsec = (msec % 1000) * 1e6;
    timerfd_settime(time_fd, TFD_TIMER_ABSTIME, &itval, NULL);

    uint8_t v2xho_wsa[V2XHO_DEFAULT_WSA_SIZE_MAX] = {0,};
    size_t v2xho_wsa_size = f_s_V2XHO_RSU_HA_WSA_Param_Construct(v2xho_wsa, G_v2xho_config.security_enable);
    bool cert_expired;
    uint8_t v2xho_spdu[2048] = {0,};
    size_t v2xho_spdu_size;

    struct Dot2SPDUConstructParams spdu_params;
    if(G_v2xho_config.security_enable)
    {
        spdu_params.type = kDot2SPDUConstructType_Signed;
        spdu_params.signed_data.psid = 135;
        spdu_params.signed_data.signer_id_type = kDot2SignerId_Profile;
        spdu_params.signed_data.gen_location.elev = 0;
        switch(G_gnss_data->fix.mode)
        {
            default: break;
            case MODE_3D:
            {
                spdu_params.signed_data.gen_location.elev = G_gnss_data->elev;
            }
            case MODE_2D:
            {
                spdu_params.signed_data.gen_location.lat = G_gnss_data->lat;
                spdu_params.signed_data.gen_location.lon = G_gnss_data->lon;
                break;
            }
            case MODE_NO_FIX:
            {
                spdu_params.signed_data.gen_location.lat = G_gnss_data->lat_raw;
                spdu_params.signed_data.gen_location.lon = G_gnss_data->lon_raw;
                break;
            }
        }
    }else{
        spdu_params.type = kDot2SPDUConstructType_Unsecured;
        spdu_params.signed_data.psid = 135;
    }
    struct Dot2SPDUConstructResult spdu_result = Dot2_ConstructSPDU(&spdu_params, v2xho_wsa, v2xho_wsa_size);
    v2xho_spdu_size = spdu_result.ret;
    memcpy(v2xho_spdu, spdu_result.spdu, spdu_result.ret);
    lSafeFree(spdu_result.spdu); 
    cert_expired = spdu_result.cmh_expiry;
    if(cert_expired == true)
    {  
        printf("cert_expired:%d\n", cert_expired);
    }


    struct Dot3MACAndWSMConstructParams dot3_params;
    dot3_params.wsm.psid = 135;
	dot3_params.wsm.chan_num = G_v2xho_config.u.dsrc.chan_0;
	dot3_params.wsm.datarate = G_v2xho_config.u.dsrc.datarate_0;
	dot3_params.wsm.transmit_power = G_v2xho_config.u.dsrc.tx_power_0;
	dot3_params.mac.priority = 7;
	memset(dot3_params.mac.dst_mac_addr, 0xff, MAC_ALEN);
	WAL_GetIfMACAddress(0, dot3_params.mac.src_mac_addr);
    size_t v2xho_mpdu_size;
	uint8_t *v2xho_mpdu = Dot3_ConstructWSMMPDU(&dot3_params, v2xho_spdu, (Dot3WSMPayloadSize)v2xho_spdu_size, &v2xho_mpdu_size, &ret);
    struct WalMPDUTxParams wal_tx_params;
    wal_tx_params.chan_num = G_v2xho_config.u.dsrc.chan_0; // 현재 접속 중인 채널
    wal_tx_params.datarate = dot3_params.wsm.chan_num;
    wal_tx_params.expiry = 0;
    wal_tx_params.tx_power = G_v2xho_config.u.dsrc.tx_power_0;

    printf("Start th_v_V2XHO_RSU_HA_Task_Running_Send.\n");
    uint32_t periodic_tick = 0;
    uint32_t wsa_send_for1sec = 3;
    uint32_t wsa_send_remain = wsa_send_for1sec;
    uint32_t wsa_send_slot;
    uint64_t res;
	while(1)
	{
		/* wait periodic timer (timeout = 100ms) */
        ret = read(time_fd, &res, sizeof(res));
		if (ret > 0)
		{
            periodic_tick++;
            if(periodic_tick == 1000 * 1000)
            {
                periodic_tick = 0;
            }
            wsa_send_slot = (10 / wsa_send_for1sec) + (10 / wsa_send_for1sec) * (wsa_send_for1sec - wsa_send_remain);
            if(wsa_send_slot == periodic_tick % 10)
            {
                srand(time(NULL));
                usleep((rand() % 20 + 1) * 1000);
                ret = WAL_TransmitMPDU(0, v2xho_mpdu, v2xho_mpdu_size, &wal_tx_params);
                wsa_send_remain--;
                if(wsa_send_remain == 0)
                {
                    wsa_send_remain = wsa_send_for1sec;
                }
            }
		}

	}

    close(time_fd);
    return (void*)NULL;

}

extern void *Th_v_V2XHO_RSU_HA_RAW_Receive(void *d)
{
    g_rsu_ha_info = (v2xho_equip_info_t*)d;
        
    for(int  i = 0; i < V2XHO_DEFAULT_RSU_HA_OBU_REGISTRATION_MAX; i++)
    {
        g_rsu_ha_reg_info.reg_obu_ha[i].isempty = true;
    }
    WAL_GetIfMACAddress(0, g_rsu_ha_info->src_mac);
    //RAW 소캣 생성
    g_rsu_ha_info->raw_socket_eth = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    setsockopt(g_rsu_ha_info->raw_socket_eth , SOL_SOCKET, SO_BINDTODEVICE, G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name));
    g_rsu_ha_info->raw_socket_ip = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    //setsockopt(g_rsu_ha_info->raw_socket_ip , SOL_SOCKET, SO_BINDTODEVICE, G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name));

    struct linger solinger = { 1, 0 };
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 500 * 1000;
    setsockopt(g_rsu_ha_info->raw_socket_eth, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger));
    setsockopt(g_rsu_ha_info->raw_socket_eth, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv,sizeof(struct timeval)); 
    setsockopt(g_rsu_ha_info->raw_socket_eth, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv,sizeof(struct timeval));
    setsockopt(g_rsu_ha_info->raw_socket_ip, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger));
    setsockopt(g_rsu_ha_info->raw_socket_ip, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv,sizeof(struct timeval)); 
    setsockopt(g_rsu_ha_info->raw_socket_ip, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv,sizeof(struct timeval));  
    // UDP 소켓 생성
    #if 0
    struct sockaddr_in server_addr;
    g_rsu_ha_info->udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_rsu_ha_info->udp_socket < 0) 
    {
        perror("socket");  
        return (void*)NULL;
    }
    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, G_v2xho_config.v2xho_interface_IP, &server_addr.sin_addr);
    server_addr.sin_port = htons(V2XHO_DEFAULT_ADVERTISEMENT_PORT);
    // 소켓 바인드      
    if (bind(g_rsu_ha_info->udp_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("bind");
        close(g_rsu_ha_info->udp_socket);
    }
    // 소켓 설정, 바로 삭제, Blocking timer 100ms 
    tv.tv_sec = 0;
    tv.tv_usec = 100 * 1000;
    if (setsockopt(g_rsu_ha_info->udp_socket, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger)) == SO_ERROR) 
    {
        perror("setsockopt(SO_LINGER)");
        return (void*)NULL;
    }
    if (setsockopt(g_rsu_ha_info->udp_socket, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv,sizeof(struct timeval)) == SO_ERROR) 
    {
        perror("setsockopt(SO_SNDTIMEO)");
        return (void*)NULL;
    }
    if (setsockopt(g_rsu_ha_info->udp_socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv,sizeof(struct timeval)) == SO_ERROR) 
    {
        perror("setsockopt(SO_RCVTIMEO)");
        return (void*)NULL;
    }
    #endif
    struct epoll_event ep_event, ep_events[V2XHO_RSU_HA_RECV_DEFAULT_EPOLL_EVENT_NUM];
	int ep_fd = epoll_create(V2XHO_RSU_HA_RECV_DEFAULT_EPOLL_EVENT_NUM);
    int ep_num = 0;
    ep_event.events = EPOLLIN;
    ep_event.data.fd = g_rsu_ha_info->raw_socket_eth;
	epoll_ctl(ep_fd, EPOLL_CTL_ADD, g_rsu_ha_info->raw_socket_eth, &ep_event);
    char **g_argv = malloc(12 * sizeof(char *));
    for (int row = 0; row < 12; row++) {
        g_argv[row] = malloc(32 * sizeof(char));
        memset(g_argv[row], 0x00, 32);
    }
    printf("Start th_v_V2XHO_RSU_HA_Task_Running_UDP_Reciver.\n");
    while (1) {
        ep_num = epoll_wait(ep_fd, ep_events, V2XHO_RSU_HA_RECV_DEFAULT_EPOLL_EVENT_NUM, -1);  // -1은 무제한 대기
        if (ep_num < 0) {
            perror("epoll_wait");
            break;
        }
        if(ep_num > 0)
        {
            for (int i = 0; i < V2XHO_RSU_HA_RECV_DEFAULT_EPOLL_EVENT_NUM; i++) {
                if (ep_events[i].data.fd == g_rsu_ha_info->raw_socket_eth) {
                    // 데이터 수신
                    char recv_buf[1024] = {0,};
                    struct sockaddr_ll client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int bytes_received = recvfrom(g_rsu_ha_info->raw_socket_eth, recv_buf, 1024, 0, (struct sockaddr *)&client_addr, &client_len);
                    double start_time = lclock_ms;
                    if (bytes_received < 0) 
                    {
                        perror("recvfrom");
                    } else {
                        switch(client_addr.sll_pkttype)
                        {
                            default:break;
                            case PACKET_HOST:
                            case PACKET_MULTICAST:
                            {
                                struct ethhdr *eth_header;
                                struct iphdr *ip_header;
                                struct udphdr *udp_header;
                                v2xho_registration_request_payload_t *recv_req_payload;
                                eth_header = (struct ethhdr *)recv_buf;
                                ip_header = (struct iphdr *)(recv_buf + sizeof(struct ethhdr));
                                switch(htons(eth_header->h_proto))
                                {
                                    case ETH_P_ARP:
                                    {
                                        #if 0
                                        char recv_if_name[128] = {0x00};
                                        if_indextoname(client_addr.sll_ifindex, recv_if_name);
                                        if(strcmp(recv_if_name, G_v2xho_config.v2xho_interface_name) == 0)
                                        {
                                            arp_header_t *arp = (arp_header_t *)(recv_buf + sizeof(struct ethhdr));
                                            if(htons(arp->oper) == 1)
                                            {
                                                char src_addr_str[V2XHO_IPV4_ALEN_STR];
                                                inet_ntop(AF_INET, arp->spa, src_addr_str, V2XHO_IPV4_ALEN_STR);
                                                int argc = 0;
                                                for (int row = 0; row < 12; row++) {
                                                    memset(g_argv[row], 0x00, 32);
                                                }
                                                memcpy(g_argv[argc], src_addr_str, strlen(src_addr_str)); argc++;
                                                memcpy(g_argv[argc], "via", strlen("via")); argc++;
                                                memcpy(g_argv[argc], src_addr_str, strlen(src_addr_str)); argc++;
                                                memcpy(g_argv[argc], "dev", strlen("dev")); argc++;
                                                memcpy(g_argv[argc], G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name)); argc++;
                                                memcpy(g_argv[argc], "onlink", strlen("onlink")); argc++;
                                                memcpy(g_argv[argc], "metric", strlen("metric")); argc++;
                                                sprintf(g_argv[argc], "%d", V2XHO_METRIC_STATIC_ACTIVE_WITH_GW); argc++;
                                                F_i_V2XHO_Iproute_Route_Modify(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_EXCL, argc, g_argv);
                                            }
                                        }
                                        #endif
                                        break;
                                    }
                                    default:
                                    {
                                        switch(ip_header->protocol)
                                        {
                                            default: break;
                                            case 17://UDP
                                            {
                                                udp_header = (struct udphdr *)(recv_buf + sizeof(struct ethhdr) + sizeof(struct iphdr));
                                                if(htons(udp_header->dest) == (V2XHO_DEFAULT_REGISTRATION_REQUEST_PORT))
                                                {
                                                    recv_req_payload = (v2xho_registration_request_payload_t *)(recv_buf + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr));
                                                    int recv_req_payload_len = bytes_received - (sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr));
                                                    if(sizeof(v2xho_registration_request_payload_t) == recv_req_payload_len)
                                                    {
                                                        int ret = f_e_V2XHO_Registration_OBU_Add(&g_rsu_ha_reg_info, (char*)recv_req_payload);
                                                        switch((enum v2xho_reg_states_e)ret)
                                                        {
                                                            default:
                                                            case V2XHO_REG_OBU_LIST_ADD_FIRST:
                                                            {
                                                                if(ret >= V2XHO_REG_OBU_LIST_ADD_FIRST && ret <= V2XHO_REG_OBU_LIST_ADD_LAST)
                                                                {
                                                                    struct v2xho_rsu_ha_registraion_t *rsu_info = &g_rsu_ha_reg_info;
                                                                    v2xho_obu_ha_registration_info_list_t *obu_ha_reg_info = &rsu_info->reg_obu_ha[ret];
                                                                    v2xho_registration_request_payload_t *req_payload = &obu_ha_reg_info->req_payload;
                                                                    v2xho_registration_reply_payload_t *rep_payload = &obu_ha_reg_info->rep_payload;
                                                                    v2xho_obu_ha_registration_info_t *obu_info = &obu_ha_reg_info->obu_info;
                                                                    if(obu_ha_reg_info->isconnected)
                                                                    {

                                                                    }
                                                                    memcpy(obu_ha_reg_info->dest_mac, eth_header->h_source, 6);
                                                                    struct in_addr addr_bin;
                                                                    inet_pton(AF_INET, G_v2xho_config.v2xho_interface_IP, &addr_bin);
                                                                    obu_info->source_address = (uint32_t)addr_bin.s_addr;
                                                                    obu_info->destination_addess = req_payload->home_address;
                                                                    obu_info->time_to_live = 10;
                                                                    obu_info->source_port = htons(udp_header->dest);
                                                                    obu_info->destination_port = htons(udp_header->source);
                                                                    rep_payload->type = 3;
                                                                    rep_payload->code = V2XHO_REPLY_CODE_REQ_ACCEPTED;
                                                                    rep_payload->lifetime = htonl(V2XHO_DEFAULT_RSU_HA_LIFETIME);
                                                                    rep_payload->home_address = req_payload->home_address;
                                                                    rep_payload->home_agent = req_payload->home_agent;
                                                                    rep_payload->identification = req_payload->identification;
                                                                    int argc = 0;
                                                                    for (int row = 0; row < 12; row++) {
                                                                        memset(g_argv[row], 0x00, 32);
                                                                    }
                                                                    char obu_addr_str[V2XHO_IPV4_ALEN_STR] = {0x00,};
                                                                    char fa_addr_str[V2XHO_IPV4_ALEN_STR] = {0x00,};
                                                                    if(req_payload->home_agent == req_payload->care_of_address)
                                                                    {
                                                                        inet_ntop(AF_INET, &obu_info->destination_addess, obu_addr_str, V2XHO_IPV4_ALEN_STR);
                                                                        inet_ntop(AF_INET, &req_payload->care_of_address, fa_addr_str, V2XHO_IPV4_ALEN_STR);
                                                                        
                                                                        memcpy(g_argv[argc], obu_addr_str, strlen(obu_addr_str)); argc++;
                                                                        memcpy(g_argv[argc], "via", strlen("via")); argc++;
                                                                        memcpy(g_argv[argc], obu_addr_str, strlen(obu_addr_str)); argc++;
                                                                        memcpy(g_argv[argc], "dev", strlen("dev")); argc++;
                                                                        memcpy(g_argv[argc], G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name)); argc++;
                                                                        memcpy(g_argv[argc], "onlink", strlen("onlink")); argc++;
                                                                        memcpy(g_argv[argc], "metric", strlen("metric")); argc++;
                                                                        sprintf(g_argv[argc], "%d", V2XHO_METRIC_STATIC_ACTIVE_WITH_GW); argc++;
                                                                        F_i_V2XHO_Iproute_Route_Modify(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_REPLACE, argc, g_argv);
                                                                        printf("[%d][Router Set][%f][%f]\n", __LINE__, lclock_ms, lclock_ms - start_time);
                                                                    }else{
                                                                        if(G_v2xho_config.tunneling_enable)
                                                                        {
                                                                            argc = 0;
                                                                            for (int row = 0; row < 12; row++) {
                                                                                memset(g_argv[row], 0x00, 32);
                                                                            }
                                                                            inet_ntop(AF_INET, &req_payload->home_address, obu_addr_str, V2XHO_IPV4_ALEN_STR);
                                                                            inet_ntop(AF_INET, &req_payload->care_of_address, fa_addr_str, V2XHO_IPV4_ALEN_STR);
                                                                            
                                                                            memcpy(g_argv[argc], "name", strlen("name")); argc++;
                                                                            sprintf(g_argv[argc], "%08X", req_payload->care_of_address); argc++;
                                                                            memcpy(g_argv[argc], "mode", strlen("mode")); argc++;
                                                                            memcpy(g_argv[argc], "ipip", strlen("ipip")); argc++;
                                                                            memcpy(g_argv[argc], "remote", strlen("remote")); argc++;
                                                                            memcpy(g_argv[argc], fa_addr_str, strlen(fa_addr_str)); argc++;
                                                                            memcpy(g_argv[argc], "local", strlen("local")); argc++;
                                                                            memcpy(g_argv[argc], G_v2xho_config.v2xho_interface_IP, strlen(G_v2xho_config.v2xho_interface_IP)); argc++;
                                                                            ret = F_i_V2XHO_Iproute_Tunnel_Modify(SIOCADDTUNNEL, argc, g_argv);
                                                                            if(ret < 0)
                                                                            {
                                                                                F_i_V2XHO_Iproute_Tunnel_Modify(SIOCCHGTUNNEL, argc, g_argv);
                                                                            }

                                                                            argc = 0;
                                                                            for (int row = 0; row < 12; row++) {
                                                                                memset(g_argv[row], 0x00, 32);
                                                                            }
                                                                            memcpy(g_argv[argc], "name", strlen("name")); argc++;
                                                                            sprintf(g_argv[argc], "%08X", req_payload->care_of_address); argc++;
                                                                            memcpy(g_argv[argc], "up", strlen("up")); argc++;
                                                                            F_i_V2XHO_Iproute_Link_Modify(RTM_NEWLINK, 0, argc, g_argv);
                                                                            printf("[%d][Tunneling Set][%f][%f]\n", __LINE__, lclock_ms, lclock_ms - start_time);
                                                                            for (int row = 0; row < 12; row++) {
                                                                                printf("%s ", g_argv[row]);
                                                                            }
                                                                            printf("\n");
                                                                            
                                                                            argc = 0;
                                                                            for (int row = 0; row < 12; row++) {
                                                                                memset(g_argv[row], 0x00, 32);
                                                                            }
                                                                            memcpy(g_argv[argc], obu_addr_str, strlen(obu_addr_str)); argc++;
                                                                            memcpy(g_argv[argc], "via", strlen("via")); argc++;
                                                                            memcpy(g_argv[argc], fa_addr_str, strlen(fa_addr_str)); argc++;
                                                                            memcpy(g_argv[argc], "dev", strlen("dev")); argc++;
                                                                            sprintf(g_argv[argc], "%08X", req_payload->care_of_address); argc++;
                                                                            memcpy(g_argv[argc], "onlink", strlen("onlink")); argc++;
                                                                            memcpy(g_argv[argc], "metric", strlen("metric")); argc++;
                                                                            sprintf(g_argv[argc], "%d", V2XHO_METRIC_STATIC_ACTIVE_WITH_GW); argc++;
                                                                            F_i_V2XHO_Iproute_Route_Modify(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_REPLACE, argc, g_argv);
                                                                            printf("[%d][Router Set][%f][%f]\n", __LINE__, lclock_ms, lclock_ms - start_time);
                                                                        }else{
                                                                            inet_ntop(AF_INET, &obu_info->destination_addess, obu_addr_str, V2XHO_IPV4_ALEN_STR);
                                                                            inet_ntop(AF_INET, &ip_header->saddr, fa_addr_str, V2XHO_IPV4_ALEN_STR);

                                                                            memcpy(g_argv[argc], obu_addr_str, strlen(obu_addr_str)); argc++;
                                                                            memcpy(g_argv[argc], "via", strlen("via")); argc++;
                                                                            memcpy(g_argv[argc], fa_addr_str, strlen(fa_addr_str)); argc++;
                                                                            memcpy(g_argv[argc], "dev", strlen("dev")); argc++;
                                                                            memcpy(g_argv[argc], G_v2xho_config.onlyIP_interface_name[0], strlen(G_v2xho_config.onlyIP_interface_name[0])); argc++;
                                                                            memcpy(g_argv[argc], "onlink", strlen("onlink")); argc++;
                                                                            memcpy(g_argv[argc], "metric", strlen("metric")); argc++;
                                                                            sprintf(g_argv[argc], "%d", V2XHO_METRIC_STATIC_ACTIVE_WITH_GW); argc++;
                                                                            F_i_V2XHO_Iproute_Route_Modify(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_REPLACE, argc, g_argv);
                                                                            printf("[%d][Router Set][%f][%f]\n", __LINE__, lclock_ms, lclock_ms - start_time);
                                                                        }
                                                                        
#if 0
                                                                        //char cmd[256];
                                                                        //sprintf(cmd, "ip route show | grep \"%s\" | grep -v \"%s\" | while read -r route; do ip route del $route; done", obu_addr_str, fa_addr_str);
                                                                        //system(cmd);
#endif
                                                                    }
                                                                    
                                                                    for (int row = 0; row < 12; row++) {
                                                                        printf("%s ", g_argv[row]);
                                                                    }
                                                                    printf("\n");
                                                                    int rep_ret;
                                                                    rep_ret = f_i_V2XHO_RSU_HA_Send_Registration_Reply(obu_ha_reg_info, recv_buf);
                                                                    printf("[%d][Send Reply][%f][%f]\n", __LINE__, lclock_ms, lclock_ms - start_time);
                                                                    if(rep_ret < 0)
                                                                    {
                                                                        printf("error\n");
                                                                    }else{
                                                                        if(req_payload->home_agent == req_payload->care_of_address)
                                                                        {
                                                                            obu_ha_reg_info->isconnected = true;
                                                                            obu_ha_reg_info->iswithfa = false;
                                                                        }else{
                                                                            obu_ha_reg_info->isconnected = true;
                                                                            obu_ha_reg_info->iswithfa = true;
                                                                        }
                                                                    }
                                                                    
                                                                }
                                                                break;
                                                            }                                
                                                            case V2XHO_REG_OBU_LIST_ADD_ISFULL:
                                                            case V2XHO_REG_OBU_LIST_ADD_ERROR_INPUT:
                                                            case V2XHO_REG_OBU_LIST_DEL_ISEMTPY:
                                                            {
                                                                break;
                                                            }

                                                        }

                                                    }
                                                }
                                            }
                                                break;
                                            }
                                }

                                        break;
                                    }
                                }
                                
                                
                        }
                    }
                }
            }
        }
    }

    // 자원 해제
    close(g_rsu_ha_info->raw_socket_eth);
    close(g_rsu_ha_info->raw_socket_ip);
    for (int row = 0; row < 12; row++) {
        lSafeFree(g_argv[row])
    }
    lSafeFree(*g_argv);
    return (void*)NULL;

}


static int f_i_V2XHO_RSU_HA_Send_Registration_Reply(v2xho_obu_ha_registration_info_list_t *send_info, char* recv_buf)
{
    int ret;
    v2xho_obu_ha_registration_info_list_t *obu_ha_reg_info = send_info;
    v2xho_registration_reply_payload_t *rep_payload = &obu_ha_reg_info->rep_payload;
    v2xho_obu_ha_registration_info_t *obu_info = &obu_ha_reg_info->obu_info;

    char buffer[V2XHO_DEFAULT_RAW_SOCKET_BUFFER_SIZE];
    //struct sockaddr_ll socket_address;
    struct sockaddr_in dest;
    //struct ethhdr *eth_header;
    struct iphdr *ip_header, *recv_ip_header;
    struct udphdr *udp_header;

    memset(buffer, 0, V2XHO_DEFAULT_RAW_SOCKET_BUFFER_SIZE);
    //eth_header = (struct ethhdr *)buffer;
    int eth_hdr_len = 0;
    ip_header = (struct iphdr *)(buffer + eth_hdr_len);
    recv_ip_header  = (struct iphdr *)(recv_buf + sizeof(struct ethhdr));
    udp_header = (struct udphdr *)(buffer + eth_hdr_len + sizeof(struct iphdr));
    v2xho_registration_reply_payload_t *udp_payload = (v2xho_registration_reply_payload_t*)(buffer + eth_hdr_len + sizeof(struct iphdr) + sizeof(struct udphdr));
    memcpy(udp_payload, rep_payload, sizeof(v2xho_registration_reply_payload_t));
    //memcpy(eth_header->h_source, g_rsu_ha_info->src_mac, 6);
    //memcpy(eth_header->h_dest, obu_ha_reg_info->dest_mac, 6);
    //eth_header->h_proto = htons(ETH_P_IP);

    ip_header->version  = 4;
    ip_header->ihl      = 5;
    ip_header->tos      = 0; // normal
    ip_header->tot_len  = 0;
    ip_header->id       = 0;
    //ip->frag_off = 0b010;
    ip_header->ttl      = obu_info->time_to_live; // hops
    ip_header->protocol = IPPROTO_UDP; // UDP
    ip_header->saddr = obu_info->source_address;
    ip_header->daddr = recv_ip_header->saddr;
    printf("[%f] ip_header->saddr:%08X, ip_header->daddr:%08X\n", lclock_ms, ip_header->saddr, ip_header->daddr);
    udp_header->source = htons(obu_info->source_port);
    udp_header->dest = htons(obu_info->destination_port);
    udp_header->len = htons(sizeof(struct udphdr) + sizeof(v2xho_registration_reply_payload_t));
    ip_header->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(v2xho_registration_reply_payload_t));
    ip_header->check = 0x0000;
    ip_header->check = F_u16_V2XHO_IP_Checksum((unsigned short *) ip_header, sizeof(struct iphdr) / 2);
    v2xho_udp_checksum_t *udp_checksum_field = malloc(sizeof(v2xho_udp_checksum_t) + sizeof(v2xho_registration_reply_payload_t));
    memset(udp_checksum_field, 0, sizeof(v2xho_udp_checksum_t) + sizeof(v2xho_registration_reply_payload_t));
    udp_checksum_field->saddr = ip_header->saddr;
    udp_checksum_field->daddr = ip_header->daddr;
    udp_checksum_field->ttl = 0; // 가상 헤더용 패딩
    udp_checksum_field->protocol = ip_header->protocol;
    udp_checksum_field->udp_len = udp_header->len;
    udp_checksum_field->source = udp_header->source;
    udp_checksum_field->dest = udp_header->dest;
    udp_checksum_field->length = udp_header->len;

    memcpy(udp_checksum_field->data, (uint8_t *)udp_payload, sizeof(v2xho_registration_reply_payload_t));
    udp_header->check = F_u16_V2XHO_UDP_Checksum((unsigned short *)udp_checksum_field, (sizeof(v2xho_udp_checksum_t) + sizeof(v2xho_registration_reply_payload_t)) / 2);
    lSafeFree(udp_checksum_field);
    
    dest.sin_family = AF_INET;
    dest.sin_port = udp_header->dest;
    dest.sin_addr.s_addr = ip_header->daddr;

    //memcpy(&socket_address, (struct sockaddr_ll*)client_addr, sizeof(socket_address));
    //while( 1)
    
    ret = sendto(g_rsu_ha_info->raw_socket_ip, buffer, eth_hdr_len + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(v2xho_registration_reply_payload_t), 0, (struct sockaddr *)&dest, sizeof(dest));
    if ( ret < 0) 
    {
        perror("패킷 전송 실패");
        close(g_rsu_ha_info->raw_socket_ip);
        lreturn(V2XHO_OBU_REGISTRATION_REQUEST_SEND_FAILURE);
    }else{
    }
 
    return ret;
}

enum v2xho_reg_states_e f_e_V2XHO_Registration_OBU_Del(struct v2xho_rsu_ha_registraion_t *info, const v2xho_obu_ha_registration_info_list_t *del_info)
{
    enum v2xho_reg_states_e ret;
    struct v2xho_rsu_ha_registraion_t *rsu_info;
    if(info)
    {
        rsu_info = info;
    }else{
        rsu_info = &g_rsu_ha_reg_info;
    }
    if(rsu_info->registered_obu_num == 0)
    {
        ret = V2XHO_REG_OBU_LIST_DEL_ISEMTPY;
        return ret;
    }

    if(del_info)
    {
        for(int i = 0; i < rsu_info->registered_obu_num; i++)
        {
            if(rsu_info->reg_obu_ha[i].req_payload.home_address == del_info->rep_payload.home_address)
            {
                memset(&rsu_info->reg_obu_ha[i].req_payload, 0x00, sizeof(v2xho_obu_ha_registration_info_list_t));
                if(rsu_info->reg_obu_ha[i + 1].isempty == false)
                {
                    memmove(&rsu_info->reg_obu_ha[i], &rsu_info->reg_obu_ha[i + 1], sizeof(v2xho_obu_ha_registration_info_list_t) * (V2XHO_DEFAULT_RSU_HA_OBU_REGISTRATION_MAX - i - 1));
                }
                rsu_info->registered_obu_num -= 1;
                ret = rsu_info->registered_obu_num;
            }
            break;
        }
    }else{
        if(rsu_info->reg_obu_ha[rsu_info->registered_obu_num - 1].isempty == false)
        {
            memset(&rsu_info->reg_obu_ha[rsu_info->registered_obu_num - 1].req_payload, 0x00, sizeof(v2xho_obu_ha_registration_info_list_t));
            rsu_info->registered_obu_num -= 1;
            ret = rsu_info->registered_obu_num;
        }else{
            for(int i = 0; i < rsu_info->registered_obu_num; i++)
            {
                if(rsu_info->reg_obu_ha[i].isempty == true)
                {
                    memset(&rsu_info->reg_obu_ha[i].req_payload, 0x00, sizeof(v2xho_obu_ha_registration_info_list_t));
                    if(rsu_info->reg_obu_ha[i + 1].isempty == false)
                    {
                        memmove(&rsu_info->reg_obu_ha[i], &rsu_info->reg_obu_ha[i + 1], sizeof(v2xho_obu_ha_registration_info_list_t) * (V2XHO_DEFAULT_RSU_HA_OBU_REGISTRATION_MAX - i - 1));
                    }
                    rsu_info->registered_obu_num -= 1;
                    ret = rsu_info->registered_obu_num;
                }
            }
        }
    }
    return ret;
}


enum v2xho_reg_states_e f_e_V2XHO_Registration_OBU_Add(struct v2xho_rsu_ha_registraion_t *info, const char *add)
{
    enum v2xho_reg_states_e ret;
    struct v2xho_rsu_ha_registraion_t *rsu_info = info;
    if(info)
    {
    }else{
        rsu_info = &g_rsu_ha_reg_info;
    }
    v2xho_registration_request_payload_t *add_req_payload = (v2xho_registration_request_payload_t *)add;
    switch(rsu_info->registered_obu_num)
    {
        case V2XHO_DEFAULT_RSU_HA_OBU_REGISTRATION_MAX:
        {
            ret = V2XHO_REG_OBU_LIST_ADD_ISFULL;
            break;
        }
        default:
        {   
            int now;
            for(now = 0; now < V2XHO_DEFAULT_RSU_HA_OBU_REGISTRATION_MAX; now++)
            {
                if(rsu_info->reg_obu_ha[now].isempty == false)
                {
                    if(rsu_info->reg_obu_ha[now].req_payload.home_address == add_req_payload->home_address)
                    {
                        memcpy(&rsu_info->reg_obu_ha[now].req_payload, add_req_payload, sizeof(v2xho_registration_request_payload_t));
                        ret = now;
                        return ret;
                    }
                }
            }
            if(now == V2XHO_DEFAULT_RSU_HA_OBU_REGISTRATION_MAX)
            {
                for(now = 0; now < V2XHO_DEFAULT_RSU_HA_OBU_REGISTRATION_MAX; now++)
                { 
                    if(rsu_info->reg_obu_ha[now].isempty == true) 
                    {
                        rsu_info->reg_obu_ha[now].isempty = false;
                        memcpy(&rsu_info->reg_obu_ha[now].req_payload, add_req_payload, sizeof(v2xho_registration_request_payload_t));
                        ret = now;
                        rsu_info->registered_obu_num += 1;
                        return ret;
                    }
                }
            }
            break;
        }
    }
    return ret;
}


#if 0
extern int F_i_V2XHO_RSU_HA_Start(pthread_t *th_id, struct v2xho_rsu_ha_registraion_t *info)
{
    int ret;
    
    lreturn(V2XHO_NO_ERROR);
}
#endif

