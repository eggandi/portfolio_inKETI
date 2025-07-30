#include "v2x_handover_main.h"

#define V2XHO_ERROR_CODE_FILE_CODE V2XHO_ERROR_CODE_FILE_CODE_OBU

#define V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL 120
#define V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW 20

#define V2XHO_DASHBOARD_COMMAND_NAME "V2XHO_KETI>"
#define V2XHO_DASHBOARD_COMMAND_SCRIPT_INPUT "input_command."
#define V2XHO_DASHBOARD_COMMAND_SCRIPT_ERROR "error_command:"
#define V2XHO_DASHBOARD_COMMAND_0 "handover"

#define V2XHO_DEFAULT_OBU_TASK_TIMER_MS 50

#define AGENT_LIST_PRINT_PAD "\033[%d;5H""%02d" "\033[9G""%02X:%02X:%02X:%02X:%02X:%02X" "\033[29G""%s" "\033[46G""%06.6f""\033[61G""%06d""\033[70G""%s""\033[78G""%04.3f""\033[88G""%06.3f""\033[101G""%s"
#define lcmove(x, y) printf("\033[%d;%dH", y + 1, x + 1)

#define EARTH_RADIUS 6371008.8

static int f_i_V2XHO_OBU_Add_Registration_List(struct v2xho_obu_registration_t *reg_info, int list_num, v2xho_wsmp_t *recv_wsmp); 
static int f_i_V2XHO_OBU_Send_Registration_Request(v2xho_rsu_ha_registration_info_t *send_info); 
static int f_i_V2XHO_Check_Agent_Priority(struct v2xho_obu_registration_t *reg_info); 
static void *th_v_V2XHO_OBU_Menual_Handover_Task(void *d); 
static double f_d_V2XHO_OBU_Int_To_Radians(int coord); 
static void *th_v_V2XHO_OBU_Menual_Handover_Command_Prompt(void *d); 

static void *th_v_V2XHO_Util_Pingtest_Task_Send_Running(void *d);
static void *th_v_V2XHO_Util_Pingtest_Task_Recv_Running(void *d);
static int f_i_V2XHO_OBU_UDS_Logging_FileDB_Setup(const char *path);


struct v2xho_util_uds_info_t *g_obu_uds_socket_info; 
v2xho_equip_info_t *g_obu_info; 
struct v2xho_obu_registration_t *g_reg_info; 
struct iphdr *g_ping_ip_hdr;
struct icmphdr *g_ping_icmp_hdr;
bool g_ping_isrun;
bool g_ping_recv_isnow;
char **g_argv;
double g_distance_logging[3];
int g_agent_num;
int g_logging_fd;
uint32_t g_icmp_recv_num;
int f_i_V2XHO_OBU_UDS_Server_Setup(struct v2xho_util_uds_info_t *setup_info)
{
    int ret;
    uint32_t path_len = strlen(V2XHO_DEFAULT_COMMUNICATIONS_PATH) + strlen(V2XHO_DEFAULT_COMMUNICATIONS_UDS_PATH);
    uint32_t name_len = strlen(V2XHO_DEFAULT_OBU_UDS_SERVER_NAME);

    setup_info->socket_type = V2XHO_TCP_OBU_RECEIVE_WSMP_DATA;
    sprintf(setup_info->socket_path, "%s%s", V2XHO_DEFAULT_COMMUNICATIONS_PATH, V2XHO_DEFAULT_COMMUNICATIONS_UDS_PATH);
    sprintf(setup_info->socket_name, "%s", V2XHO_DEFAULT_OBU_UDS_SERVER_NAME);  

    ret = access(V2XHO_DEFAULT_COMMUNICATIONS_PATH, F_OK);
    if(ret >= 0)
    {
        unlink(setup_info->socket_path);
        ret = F_i_V2XHO_Utils_Rmdirs(V2XHO_DEFAULT_COMMUNICATIONS_PATH, true);
        if(ret == -1)
        {
            char sys_cmd[256] = {0x00,};
            sprintf(sys_cmd, "umount %s", V2XHO_DEFAULT_COMMUNICATIONS_PATH);
            lsystem(sys_cmd, ret); 
            memset(sys_cmd, 0x00, 256);
            sprintf(sys_cmd, "rm -r %s", COMUNICATIONS_UDS_DEFAULT_PATH);
            lsystem(sys_cmd, ret);  
        }
    }

    if(1)
    {
        char sys_cmd[256] = {0x00,};
        sprintf(sys_cmd, "mkdir -m 777 -p %s", setup_info->socket_path);
        lsystem(sys_cmd, ret); 
        memset(sys_cmd, 0x00, 256);
        sprintf(sys_cmd, "umount %s", V2XHO_DEFAULT_COMMUNICATIONS_PATH);
        lsystem(sys_cmd, ret); 
        memset(sys_cmd, 0x00, 256);
        sprintf(sys_cmd, "mount -t tmpfs %s %s -o size=%dM", V2XHO_DEFAULT_COMMUNICATIONS_PATH, setup_info->socket_path, V2XHO_DEFULAT_RAMDISK_DEFAULT_SIZE);
       //printf("%s\n", sys_cmd);
        lsystem(sys_cmd, ret); 
        memset(sys_cmd, 0x00, 256);
        ret = access(setup_info->socket_path, F_OK);
        if(ret >= 0)
        {
            unlink(setup_info->socket_path);
        }else{
            lreturn(V2XHO_OBU_UDS_SOCKET_PATH_NOT_ACCESS);
        }
    }

    {
        char *tmp_sockfd_path = malloc(sizeof(char) * (name_len + path_len + 5));
        memset(tmp_sockfd_path, 0x00,  (name_len + path_len + 5));
        memcpy(tmp_sockfd_path, setup_info->socket_path, path_len);
        memcpy(tmp_sockfd_path + path_len, setup_info->socket_name, name_len);

        ret = F_i_V2XHO_Util_UDS_Create_Socket_File(tmp_sockfd_path, 0777);
        
        if(ret == -1)
        {
            lreturn(V2XHO_OBU_UDS_SOCKET_NOT_CREATE);
        }else{

        }
        ret = access(tmp_sockfd_path, F_OK);
        if(ret >= 0)
            unlink(tmp_sockfd_path);

        setup_info->socketfd = socket(PF_FILE, SOCK_DGRAM, 0);
        memset(&setup_info->addr, 0, sizeof(setup_info->addr));
        setup_info->addr.sun_family = AF_UNIX;
        memcpy(setup_info->addr.sun_path, tmp_sockfd_path, strlen(tmp_sockfd_path));
        if( -1 == bind(setup_info->socketfd , (struct sockaddr*)&setup_info->addr, sizeof(setup_info->addr)) )
        {
            return -1;
        }
        setup_info->socketfd_path = malloc(strlen(tmp_sockfd_path));
        memcpy(setup_info->socketfd_path, tmp_sockfd_path, strlen(tmp_sockfd_path));
        lSafeFree(tmp_sockfd_path);
        struct linger solinger = { 1, 0 };
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 50 * 1000;
        if (setsockopt(setup_info->socketfd, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger)) == SO_ERROR) 
        {
            perror("setsockopt(SO_LINGER)");
        }
        if (setsockopt(setup_info->socketfd, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv,sizeof(struct timeval)) == SO_ERROR) 
        {
            perror("setsockopt(SO_SNDTIMEO)");
        }
        if (setsockopt(setup_info->socketfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv,sizeof(struct timeval)) == SO_ERROR) 
        {
            perror("setsockopt(SO_RCVTIMEO)");
        }
    }

    return 0;
}

extern void *Th_v_V2XHO_OBU_Task_Running_Do(void *d)
{
    g_obu_info = (v2xho_equip_info_t*)d; 
    v2xho_equip_state_obu_t *status = (v2xho_equip_state_obu_t*)&(g_obu_info->status.u.stats_obu);
    struct v2xho_obu_registration_t *reg_info = malloc(sizeof(struct v2xho_obu_registration_t));
    g_reg_info = reg_info;

    memset(reg_info, 0x00, sizeof(struct v2xho_obu_registration_t));
    for(int i = 0; i  < 16; i++)
    {
        struct v2xho_rsu_ha_registration_sub_info_t *reg_sub_info = (struct v2xho_rsu_ha_registration_sub_info_t *)&(reg_info->reg_sub_list[i]);
        reg_sub_info->isempty = true;
    }

    if(status == NULL)
    {
        status = malloc(sizeof(v2xho_equip_state_obu_t));
    }
    ((uint32_t *)status)[0] = 0x00000000;
    struct v2xho_util_uds_info_t *uds_socket_info; 
    int ret;
    int uds_socket_now = g_obu_info->uds_socket_num;
    
    reg_info->home_agent_num = 0xFF;
    reg_info->connecting_now = 0xFF;
    if(uds_socket_now < 4)
    {
        uds_socket_info = &(g_obu_info->uds_socket_list[uds_socket_now]);
        ret = f_i_V2XHO_OBU_UDS_Server_Setup(uds_socket_info);
        if(ret)
        {
           //printf("error code:ret\n");
            return (void*)NULL;
        }else{
            g_obu_info->uds_socket_num++;
            g_obu_uds_socket_info = uds_socket_info;
        }
    }

    WAL_RegisterCallbackRxMPDU(F_v_V2XHO_WAVE_Rx_MPDU_Callback);

    g_obu_info->raw_socket_eth = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    setsockopt(g_obu_info->raw_socket_eth , SOL_SOCKET, SO_BINDTODEVICE, G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name));
    g_obu_info->raw_socket_ip = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    setsockopt(g_obu_info->raw_socket_ip , SOL_SOCKET, SO_BINDTODEVICE, G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name));

    struct linger solinger = { 1, 0 };
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 50 * 1000;
    setsockopt(g_obu_info->raw_socket_eth, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger));
    setsockopt(g_obu_info->raw_socket_eth, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv,sizeof(struct timeval)); 
    setsockopt(g_obu_info->raw_socket_eth, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv,sizeof(struct timeval));
    setsockopt(g_obu_info->raw_socket_ip, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger));
    setsockopt(g_obu_info->raw_socket_ip, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv,sizeof(struct timeval)); 
    setsockopt(g_obu_info->raw_socket_ip, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv,sizeof(struct timeval));  
    WAL_GetIfMACAddress(0, g_obu_info->src_mac);
    int32_t time_fd;
	struct itimerspec itval;
    int msec = V2XHO_DEFAULT_OBU_TASK_TIMER_MS;
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
    uint64_t res;
    status->active = 1;
    status->waiting_wsa = 1;
    status->connecting = 0;
    status->waiting_regreply = 0;
    status->recv_wsa = 0;
    reg_info->registration_request_send_num = 0xFF;

    if(1)
    {
        pthread_t th_dashboard;
        pthread_create(&th_dashboard, NULL, th_v_V2XHO_OBU_Menual_Handover_Task, NULL);
        pthread_detach(th_dashboard);
    }

    if(G_v2xho_config.debug_pingtest_enable == 1)
    {
        printf("[DEBUG_MODE] Ping Test Start!\n");
        printf("[DEBUG_MODE] Destination IP Addresss:%s\n", G_v2xho_config.ping_info.dest_addr);
        printf("[DEBUG_MODE] Inverval:%03d[ms]\n", G_v2xho_config.ping_info.interval_ms);
        printf("[DEBUG_MODE] Packet Size:%d[byte]\n", G_v2xho_config.ping_info.packet_size);
        pthread_t th_ping;
        pthread_create(&th_ping, NULL, th_v_V2XHO_Util_Pingtest_Task_Send_Running, (void*)&G_v2xho_config.ping_info);
        pthread_detach(th_ping);
    }
    g_argv = malloc(12 * sizeof(char *));
    for (int row = 0; row < 12; row++) {
        g_argv[row] = malloc(32 * sizeof(char));
        memset(g_argv[row], 0x00, 32);
    }
    int prev_send = 0; 
    double prev_send_time = 0;

    while(1)
    {
        ret = read(time_fd, &res, sizeof(res));
        //printf("\33[34;1H[DEBUG][%f]""status:""connecting:%d"";recv_wsa:%d"";recv_regreply:%d"";waiting_wsa:%d"";waiting_regreply:%d", lclock_ms, status->connecting, status->recv_wsa, status->recv_regreply, status->waiting_wsa, status->waiting_regreply);
        //printf("[DEBUG][%f]""status:""connecting:%d"";recv_wsa:%d"";recv_regreply:%d"";waiting_wsa:%d"";waiting_regreply:%d\n", lclock_ms, status->connecting, status->recv_wsa, status->recv_regreply, status->waiting_wsa, status->waiting_regreply);
        if(ret >= 0)
        {
            
            if(status->recv_wsa == 1)
            {   
                v2xho_wsmp_t recv_wsmp;
                while((ret = recvfrom(uds_socket_info->socketfd, &recv_wsmp, sizeof(v2xho_wsmp_t), 0, NULL, NULL)) <= 0)
                {
                    
                }
                status->recv_wsa = 0;
                status->waiting_wsa = 1;
                
                if(status->waiting_regreply != 1)
                {
                    if(reg_info->registed_num == 0)
                    {
                        reg_info->registed_num++;
                        ret = f_i_V2XHO_OBU_Add_Registration_List(reg_info, 0, &recv_wsmp);
                        if(ret == 0)
                        {
                            if(status->waiting_regreply == 0)
                            {   
                                reg_info->registration_request_send_num = 0;
                                struct v2xho_rsu_ha_registration_sub_info_t *reg_sub_info = (struct v2xho_rsu_ha_registration_sub_info_t *)&(reg_info->reg_sub_list[reg_info->registration_request_send_num]);
                                if(reg_sub_info->agent_type == V2XHO_OBU_UNKNOWN_AGENT)
                                {
                                    ret = f_i_V2XHO_OBU_Send_Registration_Request(&reg_info->reg_list[reg_info->registration_request_send_num]);
                                    if(ret > 0)
                                    {
                                        reg_sub_info->last_reg_req_send_time = reg_info->reg_list[reg_info->registration_request_send_num].rep_payload.identification;
                                        status->waiting_regreply = 1;
                                    }else{
                                        status->waiting_regreply = 0;
                                        reg_info->registration_request_send_num = 0xFF;
                                    }
                                }else{
                                    status->waiting_regreply = 0;
                                    reg_info->registration_request_send_num = 0xFF;
                                }
                            }
                        }
                    }else if(reg_info->registed_num < 16)
                    {
                        int now;
                        for(now = 0; now  < reg_info->registed_num; now++)
                        {
                            struct v2xho_rsu_ha_registration_sub_info_t *reg_sub_info = (struct v2xho_rsu_ha_registration_sub_info_t *)&(reg_info->reg_sub_list[now]);
                            int mac_comp;
                            if(!reg_sub_info->isempty)
                            {
                                for(mac_comp = 0; mac_comp < 6; mac_comp++)
                                {
                                    if(reg_sub_info->wsmp.src_mac[mac_comp] != recv_wsmp.src_mac[mac_comp])
                                    {
                                        break;
                                    }
                                }
                            }
                            if(mac_comp == 6)
                            {
                                ret = f_i_V2XHO_OBU_Add_Registration_List(reg_info, now, &recv_wsmp);
                                now = 17;
                                break;
                            }
                        }
                        if(now == reg_info->registed_num)
                        {
                            for(now = reg_info->registed_num; now < 16; now++)
                            {
                                struct v2xho_rsu_ha_registration_sub_info_t *reg_sub_info = (struct v2xho_rsu_ha_registration_sub_info_t *)&(reg_info->reg_sub_list[now]);
                                if(reg_sub_info->isempty)
                                {
                                    ret = f_i_V2XHO_OBU_Add_Registration_List(reg_info, now, &recv_wsmp);
                                    reg_info->registed_num++;
                                    if(status->waiting_regreply == 0 && reg_info->home_agent_num == 0xFF)
                                    {   
                                        reg_info->registration_request_send_num = now;
                                        if(reg_sub_info->agent_type == V2XHO_OBU_UNKNOWN_AGENT)
                                        {
                                            ret = f_i_V2XHO_OBU_Send_Registration_Request(&reg_info->reg_list[reg_info->registration_request_send_num]);
                                            if(ret > 0)
                                            {                                   
                                                reg_sub_info->last_reg_req_send_time = reg_info->reg_list[reg_info->registration_request_send_num].rep_payload.identification;         
                                                status->waiting_regreply = 1;
                                            }else{
                                                status->waiting_regreply = 0;
                                                reg_info->registration_request_send_num = 0xFF;
                                            }
                                        }else{
                                            status->waiting_regreply = 0;
                                            reg_info->registration_request_send_num = 0xFF;
                                        }
                                    }
                                    break;
                                }
                            }
                        }

                    }
                }
            }
            
            if((status->waiting_regreply != 1 && reg_info->registration_request_send_num != 0xFF) || g_agent_num == 99)
            {
                if(reg_info->registration_request_send_num < reg_info->registed_num)
                {
                    v2xho_rsu_ha_registration_info_t *reg_list = &(reg_info->reg_list[reg_info->registration_request_send_num]);
                    char gw_addr_str[V2XHO_IPV4_ALEN_STR];
                    inet_ntop(AF_INET, &reg_list->destination_addess, gw_addr_str, V2XHO_IPV4_ALEN_STR);
                    int argc = 0;
                    for (int row = 0; row < 12; row++) {
                        memset(g_argv[row], 0x00, 32);
                    }
                    memcpy(g_argv[argc], "default", strlen("default")); argc++;
                    memcpy(g_argv[argc], "via", strlen("via")); argc++;
                    memcpy(g_argv[argc], gw_addr_str, strlen(gw_addr_str)); argc++;
                    memcpy(g_argv[argc], "dev", strlen("dev")); argc++;
                    memcpy(g_argv[argc], G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name)); argc++;
                    memcpy(g_argv[argc], "onlink", strlen("onlink")); argc++;
                    memcpy(g_argv[argc], "metric", strlen("metric")); argc++;
                    sprintf(g_argv[argc], "%d", V2XHO_METRIC_DEFAULT_ACTIVE_WITH_GW); argc++;
                    ret = F_i_V2XHO_Iproute_Route_Modify(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_REPLACE, argc, g_argv);
                    ret = f_i_V2XHO_OBU_Send_Registration_Request(&reg_info->reg_list[reg_info->registration_request_send_num]);
                    if(ret > 0)
                    {
                        struct v2xho_rsu_ha_registration_sub_info_t *reg_sub_info = (struct v2xho_rsu_ha_registration_sub_info_t *)&(reg_info->reg_sub_list[reg_info->registration_request_send_num]);
                        reg_sub_info->last_reg_req_send_time = reg_info->reg_list[reg_info->registration_request_send_num].rep_payload.identification;
                        status->waiting_regreply = 1;
                    }
                }else if(g_agent_num == 99)
                {
                    if(lclock_ms - prev_send_time > 5)
                    {
                        while(status->waiting_regreply == 1)
                        {
                        }
                        prev_send_time = lclock_ms;
                        srand(time(NULL));
                        prev_send = (prev_send + (rand() % reg_info->registed_num - 1) + 1) % reg_info->registed_num;
                        printf("\033[%d;1H""Action Auto Handover; Registraion Request To Agent[%d]           ", V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 8, prev_send);
                        
                        reg_info->registration_request_send_num = prev_send;
                        v2xho_rsu_ha_registration_info_t *reg_list = &(reg_info->reg_list[reg_info->registration_request_send_num]);
                        char gw_addr_str[V2XHO_IPV4_ALEN_STR];
                        inet_ntop(AF_INET, &reg_list->destination_addess, gw_addr_str, V2XHO_IPV4_ALEN_STR);
                        int argc = 0;
                        for (int row = 0; row < 12; row++) {
                            memset(g_argv[row], 0x00, 32);
                        }
                        memcpy(g_argv[argc], "default", strlen("default")); argc++;
                        memcpy(g_argv[argc], "via", strlen("via")); argc++;
                        memcpy(g_argv[argc], gw_addr_str, strlen(gw_addr_str)); argc++;
                        memcpy(g_argv[argc], "dev", strlen("dev")); argc++;
                        memcpy(g_argv[argc], G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name)); argc++;
                        memcpy(g_argv[argc], "onlink", strlen("onlink")); argc++;
                        memcpy(g_argv[argc], "metric", strlen("metric")); argc++;
                        sprintf(g_argv[argc], "%d", V2XHO_METRIC_DEFAULT_ACTIVE_WITH_GW); argc++;
                        ret = F_i_V2XHO_Iproute_Route_Modify(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_REPLACE, argc, g_argv);
                        ret = f_i_V2XHO_OBU_Send_Registration_Request(&reg_info->reg_list[reg_info->registration_request_send_num]);
                        if(ret > 0)
                        {
                            struct v2xho_rsu_ha_registration_sub_info_t *reg_sub_info = (struct v2xho_rsu_ha_registration_sub_info_t *)&(reg_info->reg_sub_list[reg_info->registration_request_send_num]);
                            reg_sub_info->last_reg_req_send_time = reg_info->reg_list[reg_info->registration_request_send_num].rep_payload.identification;
                            status->waiting_regreply = 1;
                        }
                    }
                }
            }
            status->waiting_wsa = 1;
            if(status->waiting_regreply)
            {
                char recv_buf[V2XHO_DEFAULT_RAW_SOCKET_BUFFER_SIZE];
                struct sockaddr_ll saddrll;
                socklen_t src_addr_len = sizeof(saddrll);
                double waiting_timer = lclock_ms;
                while(status->waiting_regreply == 1)
                {
                    ret = recvfrom(g_obu_info->raw_socket_eth, recv_buf, V2XHO_DEFAULT_RAW_SOCKET_BUFFER_SIZE, 0, (struct sockaddr *)&(saddrll), &src_addr_len);
                    double recv_start_time = lclock_ms;
                    if( lclock_ms - waiting_timer >= 1)
                    {
                        //printf("[DEBUG][%f]""%f\n", lclock_ms, lclock_ms - waiting_timer);
                        status->waiting_regreply = 0;
                        //printf("reg_info->registration_request_send_num:%d\n", reg_info->registration_request_send_num);
                        struct v2xho_rsu_ha_registration_sub_info_t *rsu_ha_reg_sub_info = &reg_info->reg_sub_list[reg_info->registration_request_send_num];
                        rsu_ha_reg_sub_info->agent_type = V2XHO_OBU_FOREIGN_AGENT;
                        reg_info->registration_request_send_num = 0xFF;
                        break;
                    }
                    if(ret <= 0)
                    {
                        //printf("Receive Fail! %f %d %d\n", lclock_ms - recv_time_0, ret, waiting_timer);
                    }else{
                        switch(saddrll.sll_pkttype)
                        {
                            default:break;
                            case PACKET_HOST:
                            case PACKET_MULTICAST:
                            case PACKET_BROADCAST:
                            {
                                struct ethhdr *eth_header;
                                struct iphdr *ip_header;
                                struct udphdr *udp_header;
                                eth_header = (struct ethhdr *)recv_buf;
                                ip_header = (struct iphdr *)(recv_buf + sizeof(struct ethhdr));
                                switch(ip_header->protocol)
                                {
                                    default: break;
                                    case 17://UDP
                                    {
                                        udp_header = (struct udphdr *)(recv_buf + sizeof(struct ethhdr) + sizeof(struct iphdr));
                                        if(htons(udp_header->dest) == (V2XHO_DEFAULT_REGISTRATION_REPLY_PORT))
                                        {
                                            status->waiting_regreply = 0;
                                            //printf("\33[37;1H""[DEBUG][%f]saddr:%08X,daddr:%08X", lclock_ms, ip_header->saddr, ip_header->daddr);
                                            for(int now = 0; now < reg_info->registed_num; now++)
                                            {
                                                v2xho_rsu_ha_registration_info_t *rsu_ha_reg_info = &reg_info->reg_list[now];
                                                struct v2xho_rsu_ha_registration_sub_info_t *rsu_ha_reg_sub_info = &reg_info->reg_sub_list[now];
                                                if(ip_header->saddr == rsu_ha_reg_info->destination_addess && ip_header->daddr == rsu_ha_reg_info->source_address)
                                                {
                                                    v2xho_registration_reply_payload_t *recv_rep_payload = (v2xho_registration_reply_payload_t *)(recv_buf + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr));
                                                    size_t recv_rep_payload_len = ret - (sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr));
                                                    if(sizeof(v2xho_registration_reply_payload_t) == recv_rep_payload_len)
                                                    {
                                                        v2xho_registration_reply_payload_t *rep_payload = &rsu_ha_reg_info->rep_payload;
                                                        memcpy(rep_payload, recv_rep_payload, recv_rep_payload_len);            
                                                        double router_set_time;
                                                        switch((enum v2xho_reg_reply_code_e)rep_payload->code)
                                                        {
                                                            case V2XHO_REPLY_CODE_REQ_ACCEPTED:
                                                            case V2XHO_REPLY_CODE_REQ_ACCEPTED_NO_MULTI_COA:
                                                            {
                                                                rsu_ha_reg_sub_info->last_reg_rep_recv_time = lclock_ms;
                                                                int check_mac = 0;
                                                                for(check_mac = 0; check_mac < 6; check_mac++)
                                                                {
                                                                    if(eth_header->h_source[check_mac] != rsu_ha_reg_sub_info->wsmp.src_mac[check_mac])
                                                                    {
                                                                        check_mac = 10;
                                                                        break;
                                                                    }
                                                                }
                                                                int argc = 0;
                                                                for (int row = 0; row < 12; row++) {
                                                                    memset(g_argv[row], 0x00, 32);
                                                                }
                                                                memcpy(g_argv[argc], "default", strlen("default")); argc++;
                                                                memcpy(g_argv[argc], "via", strlen("via")); argc++;
                                                                char addr_str[V2XHO_IPV4_ALEN_STR] = {0x00,};
                                                                switch(check_mac)
                                                                {
                                                                    case 6:
                                                                    {
                                                                        if(reg_info->home_agent_num == 0xFF)
                                                                        {
                                                                            status->connecting = 0b01;
                                                                            reg_info->home_agent_num = now;
                                                                            rsu_ha_reg_sub_info->agent_type = V2XHO_OBU_HOME_AGENT;
                                                                            inet_ntop(AF_INET, &rsu_ha_reg_info->req_payload.home_agent, addr_str, V2XHO_IPV4_ALEN_STR);
                                                                            inet_ntop(AF_INET, &rsu_ha_reg_info->req_payload.home_address, G_v2xho_config.v2xho_interface_IP, V2XHO_IPV4_ALEN_STR);
                                                                            int prefix = 24;
                                                                            F_i_V2XHO_IProuter_Address_Set(G_v2xho_config.v2xho_interface_name, G_v2xho_config.v2xho_interface_IP, &prefix, 1);
                                                                        }else if(rsu_ha_reg_info->rep_payload.home_agent == ip_header->saddr)
                                                                        {
                                                                            status->connecting = 0b01;
                                                                            reg_info->home_agent_num = now;
                                                                            inet_ntop(AF_INET, &rsu_ha_reg_info->req_payload.home_agent, addr_str, V2XHO_IPV4_ALEN_STR);
                                                                            inet_ntop(AF_INET, &rsu_ha_reg_info->req_payload.home_address, G_v2xho_config.v2xho_interface_IP, V2XHO_IPV4_ALEN_STR);
                                                                            int prefix = 24;
                                                                            F_i_V2XHO_IProuter_Address_Set(G_v2xho_config.v2xho_interface_name, G_v2xho_config.v2xho_interface_IP, &prefix, 0);

                                                                        }else if(rsu_ha_reg_info->req_payload.care_of_address  == ip_header->saddr){
                                                                            status->connecting = 0b11;
                                                                            inet_ntop(AF_INET, &rsu_ha_reg_info->req_payload.care_of_address, addr_str, V2XHO_IPV4_ALEN_STR);
                                                                        }
                                                                        memcpy(g_argv[argc], addr_str, strlen(addr_str)); argc++;
                                                                        memcpy(g_argv[argc], "dev", strlen("dev")); argc++;
                                                                        memcpy(g_argv[argc], G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name)); argc++;
                                                                        memcpy(g_argv[argc], "onlink", strlen("onlink")); argc++;
                                                                        memcpy(g_argv[argc], "metric", strlen("metric")); argc++;
                                                                        sprintf(g_argv[argc], "%d", V2XHO_METRIC_DEFAULT_ACTIVE_WITH_GW); argc++;
                                                                        F_i_V2XHO_Iproute_Route_Modify(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_EXCL, argc, g_argv);
                                                                        router_set_time = lclock_ms;
                                                                        break;
                                                                    }
                                                                    case 10:
                                                                    {
                                                                        if(rsu_ha_reg_info->rep_payload.home_agent == ip_header->saddr)
                                                                        {
                                                                            status->connecting = 0b01;
                                                                            reg_info->home_agent_num = now;
                                                                            inet_ntop(AF_INET, &rsu_ha_reg_info->req_payload.home_agent, addr_str, V2XHO_IPV4_ALEN_STR);
                                                                            inet_ntop(AF_INET, &rsu_ha_reg_info->req_payload.home_address, G_v2xho_config.v2xho_interface_IP, V2XHO_IPV4_ALEN_STR);
                                                                            int prefix = 24;
                                                                            F_i_V2XHO_IProuter_Address_Set(G_v2xho_config.v2xho_interface_name, G_v2xho_config.v2xho_interface_IP, &prefix, 0);
                                                                        }else if(rsu_ha_reg_info->req_payload.care_of_address  == ip_header->saddr){
                                                                            status->connecting = 0b11;
                                                                            inet_ntop(AF_INET, &rsu_ha_reg_info->req_payload.care_of_address, addr_str, V2XHO_IPV4_ALEN_STR);
                                                                        }
                                                                        break;
                                                                    }
                                                                    default:break;
                                                                }
                                                                reg_info->connecting_now = now;
                                                                printf("\33[%d;1H", V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 7);
                                                                for (int row = 0; row < 12; row++) {
                                                                    printf("%s ", g_argv[row]);
                                                                }
                                                                printf("\33[%d;1H""[DEBUG]P_t:%06.3f, Route Set:%06.3f, RTT(Req->Rep):%06.3f[s]", V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 6,\
                                                                lclock_ms - recv_start_time, router_set_time - recv_start_time,\
                                                                lclock_ms - rsu_ha_reg_info->rep_payload.identification);
                                                            }
                                                            default:
                                                            {
                                                                //printf("[V2X HANDOVER] Request DENIED! Reply Code:%d\n", rep_payload->code);
                                                                break;
                                                            }
                                                        }
                                                    }
                                                }   
                                            }
                                        }
                                        break;
                                    }
                                    #if 0
                                    case IPPROTO_ICMP:
                                    {                                
                                        struct icmphdr *icmp_resp =  (struct icmphdr *)(recv_buf + sizeof(struct ethhdr) + sizeof(struct iphdr));
                                        if (icmp_resp->icmp_type == ICMP_ECHOREPLY)
                                        {
                                            if(g_ping_icmp_hdr->icmp_id == icmp_resp->icmp_id)
                                            {
                                                if(g_logging_fd != 0)//g_ping_timing.icmp_send_seq == icmp_resp->icmp_seq)
                                                {
                                                    g_ping_recv_isnow = true;
                                                    g_icmp_recv_num++;
                                                    if(G_v2xho_config.debug_pingtest_logging_enable)
                                                    {
                                                        #if 0 
                                                        memcpy(log_data.src_MAC, eth_header->h_source, 6);
                                                        inet_ntop(AF_INET, &ip_header->saddr, log_data.ip_src_add_str, V2XHO_IPV4_ALEN_STR);
                                                        inet_ntop(AF_INET, &ip_header->daddr, log_data.ip_dst_add_str, V2XHO_IPV4_ALEN_STR);
                                                        log_data.icmp_seq_num = icmp_resp->icmp_seq;
                                                        log_data.recv_time = recv_timing;
                                                        log_data.lat = G_gnss_data->lat;
                                                        log_data.lon = G_gnss_data->lon;
                                                        log_data.distance_1 = g_distance_logging[0];
                                                        log_data.distance_2 = g_distance_logging[1];
                                                        log_data.distance_3 = g_distance_logging[2];
                                                        log_data.ext[0] = ';';
                                                        log_data.ext[1] = '\n';
                                                        char log_buf[256] = {'\0'};
                                                        sprintf(log_buf,"%02X:%02X:%02X:%02X:%02X\t%s\t%s\t%05d\t%06.3f\t%09d\t%09d\t%06.3f\t%06.3f\t%06.3f;\n",V2XHO_MAC_PAD(eth_header->h_source),
                                                                                                                                                                log_data.ip_src_add_str,
                                                                                                                                                                log_data.ip_dst_add_str,
                                                                                                                                                                log_data.icmp_seq_num,
                                                                                                                                                                log_data.recv_time,
                                                                                                                                                                log_data.lat,
                                                                                                                                                                log_data.lon,
                                                                                                                                                                log_data.distance_1,
                                                                                                                                                                log_data.distance_2,
                                                                                                                                                                log_data.distance_3);
                                                                                                                                                                inet_ntop(AF_INET, &ip_header->saddr, ip_src_add_str, V2XHO_IPV4_ALEN_STR);
                                                        #endif
                                                        char log_buf[256] = {'\0'};
                                                        char ip_src_add_str[V2XHO_IPV4_ALEN_STR];
                                                        char ip_dst_add_str[V2XHO_IPV4_ALEN_STR];
                                                        inet_ntop(AF_INET, &ip_header->daddr, ip_src_add_str, V2XHO_IPV4_ALEN_STR);
                                                        inet_ntop(AF_INET, &ip_header->daddr, ip_dst_add_str, V2XHO_IPV4_ALEN_STR);
                                                        sprintf(log_buf,"%02X:%02X:%02X:%02X:%02X:%02X\t%s\t%s\t%05d\t%06.3f\t%09d\t%09d\t%06.3f\t%06.3f\t%06.3f;\n",   V2XHO_MAC_PAD(eth_header->h_source),
                                                                                                                                                                        ip_src_add_str,
                                                                                                                                                                        ip_dst_add_str,
                                                                                                                                                                        icmp_resp->icmp_seq,
                                                                                                                                                                        recv_start_time,
                                                                                                                                                                        G_gnss_data->lat,
                                                                                                                                                                        G_gnss_data->lon,
                                                                                                                                                                        g_distance_logging[0],
                                                                                                                                                                        g_distance_logging[1],
                                                                                                                                                                        g_distance_logging[2]);
                                                        write(g_logging_fd, log_buf, strlen(log_buf));
                                                    }
                                                    #if 0 
                                                    int ii;
                                                    for(ii = 0; ii < 6; ii++)
                                                    {
                                                        if(prev_mac[ii] != eth_header->h_source[ii])
                                                        {
                                                            ii = 10;
                                                            printf("\33[%d;1H""[DEBUG][%f]prev src:%02X:%02X:%02X:%02X:%02X:%02X, new src::%02X:%02X:%02X:%02X:%02X:%02X, seq:%d/%d RTT:%06.3f[s] Latancy:%06.3f[s]", V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 4, lclock_ms,
                                                            V2XHO_MAC_PAD(prev_mac), V2XHO_MAC_PAD(eth_header->h_source), prev_seq, icmp_resp->icmp_seq, recv_timing - g_ping_timing.send_timing, recv_timing - prev_timing);
                                                            break;
                                                        }
                                                    }
                                                    if(ii == 10)
                                                    {                    
                                                        for(ii = 0; ii < 6; ii++)
                                                        {
                                                            prev_mac[ii] = eth_header->h_source[ii];
                                                        }
                                                    }
                                                    prev_timing = recv_timing;
                                                    prev_seq = icmp_resp->icmp_seq;
                                                    #endif
                                                }
                                            }
                                        }
                                        break;
                                    }
                                    #endif
                                }
                                break;
                            }
                                
                        }
                    }
                }
            }
            
            int nearest_agent = f_i_V2XHO_Check_Agent_Priority(reg_info);
            if(reg_info->connecting_now != nearest_agent && reg_info->connecting_now != 0xFF && nearest_agent != 0xFF )
            {
                double distance_connected = reg_info->reg_sub_list[reg_info->connecting_now].distance;
                double distance_nearest = reg_info->reg_sub_list[nearest_agent].distance;
                if(distance_connected - distance_nearest >= 100) // 현재 접속된 Agent보다 가까운 Agent의 거리가 100m 이상 더 가까울 때
                {
                    //Trying to change The Agent.
                }
            }
            if(status->connecting != 0)
            {
                if(reg_info->home_agent_num != 0xFF)
                {
                    if(reg_info->home_agent_num == reg_info->connecting_now)
                    {
                        //v2xho_rsu_ha_registration_info_t *reg_list_HA = (v2xho_rsu_ha_registration_info_t*)&(reg_info->reg_list[reg_info->home_agent_num]);
                        //v2xho_registration_reply_payload_t *rep_payload = (v2xho_registration_reply_payload_t*)&reg_list_HA->rep_payload;
                        //struct v2xho_rsu_ha_registration_sub_info_t *reg_sub_info = (struct v2xho_rsu_ha_registration_sub_info_t *)&(reg_info->reg_sub_list[reg_info->home_agent_num]);
                       //printf("[Connected][HomeAgent][Addr] HAAddr:%08X, HomeAddr:%08X\n", rep_payload->home_agent, rep_payload->home_address);
                       //printf("[Connected][HomeAgent][SubInfo] WSA_Last_Recv_Time:%f, WSA_g_icmp_recv_num:%08X, Distance:%f\n", reg_sub_info->last_reg_req_send_time, reg_sub_info->recv_count, reg_sub_info->distance);
                    }
                }else{
                   //printf("[Wating]HomeAgent\n");
                }
            }else{
                if(0)//nearest_agent != 0xFF)
                {   
                    if(status->connecting == 0 && status->waiting_regreply == 0)
                    {   
                        reg_info->registration_request_send_num = nearest_agent;
                        struct v2xho_rsu_ha_registration_sub_info_t *reg_sub_info = (struct v2xho_rsu_ha_registration_sub_info_t *)&(reg_info->reg_sub_list[reg_info->registration_request_send_num]);
                        if(reg_sub_info->agent_type != V2XHO_OBU_FOREIGN_AGENT)
                        {
                            ret = f_i_V2XHO_OBU_Send_Registration_Request(&reg_info->reg_list[reg_info->registration_request_send_num ]);
                            if(ret > 0)
                            {
                                reg_sub_info->last_reg_req_send_time = reg_info->reg_list[reg_info->registration_request_send_num].rep_payload.identification;
                                status->waiting_regreply = 1;
                            }else{
                                status->waiting_regreply = 0;
                                reg_info->registration_request_send_num = 0xFF;
                            }
                        }else{
                                status->waiting_regreply = 0;
                                reg_info->registration_request_send_num = 0xFF;
                        }
                    }
                }
            }
        }
        reg_info->registration_request_send_num = 0xFF;
    }
    close(g_obu_uds_socket_info->socketfd);
    close(g_obu_info->raw_socket_ip);
    close(g_obu_info->raw_socket_eth);

    for (int i = 0; i < 8; i++) {
        lSafeFree(g_argv[i])
    }
    lSafeFree(*g_argv);
    lSafeFree(reg_info);
    lSafeFree(status);
}

extern int F_i_V2XHO_OBU_UDS_Send_WSMP_Data(v2xho_wsmp_t *wsmp)
{
    int ret = 0;
    v2xho_equip_state_obu_t *status = (v2xho_equip_state_obu_t*)&(g_obu_info->status.u.stats_obu);
    if(status->waiting_wsa == 1)
    {
        status->waiting_wsa = 0;
        ret = sendto(g_obu_uds_socket_info->socketfd, wsmp, sizeof(v2xho_wsmp_t), 0, (struct sockaddr *)&g_obu_uds_socket_info->addr, sizeof(g_obu_uds_socket_info->addr));
        if(ret > 0)
        {
            status->recv_wsa = 1;
        }
    }else{

    }
    return ret;
}

static int f_i_V2XHO_OBU_Send_Registration_Request(v2xho_rsu_ha_registration_info_t *send_info)
{
    int ret;
    char buffer[V2XHO_DEFAULT_RAW_SOCKET_BUFFER_SIZE];
    //struct sockaddr_ll socket_address;
    struct sockaddr_in dest;
    //struct ethhdr *eth_header;
    struct iphdr *ip_header;
    struct udphdr *udp_header;
    v2xho_registration_request_payload_t *udp_payload;

    int eth_hdr_len = 0;
    memset(buffer, 0, V2XHO_DEFAULT_RAW_SOCKET_BUFFER_SIZE);
    //eth_header = (struct ethhdr *)buffer;
    ip_header = (struct iphdr *)(buffer + eth_hdr_len);
    udp_header = (struct udphdr *)(buffer + eth_hdr_len+ sizeof(struct iphdr));
    udp_payload = (v2xho_registration_request_payload_t*)(buffer + eth_hdr_len + sizeof(struct iphdr) + sizeof(struct udphdr));

    //memcpy(eth_header->h_source, g_obu_info->src_mac, 6);
    //memcpy(eth_header->h_dest, send_info->dest_mac, 6);
    //eth_header->h_proto = htons(ETH_P_IP);
    
    ip_header->version  = 4;
    ip_header->ihl      = 5;
    ip_header->tos      = 0; // normal
    ip_header->tot_len  = 0;
    ip_header->id       = 0;
    //ip->frag_off = 0b010;
    ip_header->ttl      = send_info->time_to_live; // hops
    ip_header->protocol = 17; // UDP
    ip_header->saddr = send_info->source_address;
    ip_header->daddr = send_info->destination_addess;
    udp_header->source = htons(send_info->source_port);
    udp_header->dest = htons(send_info->destination_port);
    send_info->req_payload.identification = lclock_ms;
    memcpy(udp_payload, &send_info->req_payload, sizeof(v2xho_registration_request_payload_t));
    udp_header->len = htons(sizeof(struct udphdr) + sizeof(v2xho_registration_request_payload_t));
    ip_header->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(v2xho_registration_request_payload_t));
    ip_header->check = 0x0000;
    ip_header->check = F_u16_V2XHO_IP_Checksum((unsigned short *) ip_header, sizeof(struct iphdr) / 2);
    v2xho_udp_checksum_t *udp_checksum_field = malloc(sizeof(v2xho_udp_checksum_t) + sizeof(v2xho_registration_request_payload_t));
    memset(udp_checksum_field, 0, sizeof(v2xho_udp_checksum_t) + sizeof(v2xho_registration_request_payload_t));
    udp_checksum_field->saddr = ip_header->saddr;
    udp_checksum_field->daddr = ip_header->daddr;
    udp_checksum_field->ttl = 0; // 가상 헤더용 패딩
    udp_checksum_field->protocol = ip_header->protocol;
    udp_checksum_field->udp_len = udp_header->len;
    udp_checksum_field->source = udp_header->source;
    udp_checksum_field->dest = udp_header->dest;
    udp_checksum_field->length = udp_header->len;

    memcpy(udp_checksum_field->data, (uint8_t *)udp_payload, sizeof(v2xho_registration_request_payload_t));
    udp_header->check = F_u16_V2XHO_UDP_Checksum((unsigned short *)udp_checksum_field, (sizeof(v2xho_udp_checksum_t) + sizeof(v2xho_registration_request_payload_t)) / 2);
    lSafeFree(udp_checksum_field);
    dest.sin_family = AF_INET;
    dest.sin_port = udp_header->dest;
    dest.sin_addr.s_addr = ip_header->daddr;
    while(g_ping_recv_isnow)
    {

    } 

    ret = sendto(g_obu_info->raw_socket_ip, buffer, eth_hdr_len + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(v2xho_registration_request_payload_t), 0, (struct sockaddr *)&dest, sizeof(dest));
    if ( ret < 0) 
    {
        perror("패킷 전송 실패");
        close(g_obu_info->raw_socket_ip);
        lreturn(V2XHO_OBU_REGISTRATION_REQUEST_SEND_FAILURE); 
    }else{
       //printf("Send Success! %d\n", ret);
    }
    return ret;
}

// 정수 GPS 좌표를 라디안으로 변환하는 함수
static double f_d_V2XHO_OBU_Int_To_Radians(int coord) 
{
    // GPS 좌표는 1e6 스케일로 입력되므로 이를 다시 소수점 좌표로 변환
    return (coord / 1e7) * M_PI / 180.0;
}

double f_d_V2XHO_OBU_Calcuration_Distance(int now_num)
{
    struct v2xho_obu_registration_t *reg_info = g_reg_info;
    struct v2xho_rsu_ha_registration_sub_info_t *reg_sub_info = &(reg_info->reg_sub_list[now_num]);
    v2xho_wsmp_t *wsmp = &(reg_sub_info->wsmp);
    double lat1_rad, lon1_rad;
    double lat2_rad, lon2_rad;

    if(wsmp->dot2_type == kDot2Content_UnsecuredData)
    {
        lat1_rad =f_d_V2XHO_OBU_Int_To_Radians(wsmp->u.wsa_location.latitude);
        lon1_rad =f_d_V2XHO_OBU_Int_To_Radians(wsmp->u.wsa_location.longitude);

    }else if(wsmp->dot2_type == kDot2Content_SignedData){
        lat1_rad =f_d_V2XHO_OBU_Int_To_Radians(wsmp->u.dot2_location.lat);
        lon1_rad =f_d_V2XHO_OBU_Int_To_Radians(wsmp->u.dot2_location.lon);
    }else{

    }

    switch(G_gnss_data->fix.mode)
    {
        
        case MODE_3D:
        case MODE_2D:
        {
            lat2_rad =f_d_V2XHO_OBU_Int_To_Radians(G_gnss_data->lat);
            lon2_rad =f_d_V2XHO_OBU_Int_To_Radians(G_gnss_data->lon);
            break;
        }
        case MODE_NO_FIX:
        {
            lat2_rad =f_d_V2XHO_OBU_Int_To_Radians(G_gnss_data->lat_raw);
            lon2_rad =f_d_V2XHO_OBU_Int_To_Radians(G_gnss_data->lon_raw);
            break;
        }
        default: 
        {
            break;
        }
    }    
    double delta_lat = lat2_rad - lat1_rad;
    double delta_lon = lon2_rad - lon1_rad;

    //double a = sin(delta_lat / 2) * sin(delta_lat / 2) + cos(lat1_rad) * cos(lat2_rad) * sin(delta_lon / 2) * sin(delta_lon / 2);
    double a = fma(sin(delta_lat / 2), sin(delta_lat / 2), cos(lat1_rad) * cos(lat2_rad) * sin(delta_lon / 2) * sin(delta_lon / 2));
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;   
}

double f_d_V2XHO_OBU_Calcuration_Distance_AB(v2xho_wsmp_t *wsmp_A, v2xho_wsmp_t *wsmp_B)
{
    double lat1_rad, lon1_rad;
    double lat2_rad, lon2_rad;
    
    if(wsmp_A->dot2_type == kDot2Content_UnsecuredData)
    {
        lat1_rad =f_d_V2XHO_OBU_Int_To_Radians(wsmp_A->u.wsa_location.latitude);
        lon1_rad =f_d_V2XHO_OBU_Int_To_Radians(wsmp_A->u.wsa_location.longitude);

    }else if(wsmp_A->dot2_type == kDot2Content_SignedData){
        lat1_rad =f_d_V2XHO_OBU_Int_To_Radians(wsmp_A->u.dot2_location.lat);
        lon1_rad =f_d_V2XHO_OBU_Int_To_Radians(wsmp_A->u.dot2_location.lon);
    }else{

    }

    if(wsmp_B->dot2_type == kDot2Content_UnsecuredData)
    {
        lat2_rad =f_d_V2XHO_OBU_Int_To_Radians(wsmp_B->u.wsa_location.latitude);
        lon2_rad =f_d_V2XHO_OBU_Int_To_Radians(wsmp_B->u.wsa_location.longitude);

    }else if(wsmp_B->dot2_type == kDot2Content_SignedData){
        lat2_rad =f_d_V2XHO_OBU_Int_To_Radians(wsmp_B->u.dot2_location.lat);
        lon2_rad =f_d_V2XHO_OBU_Int_To_Radians(wsmp_B->u.dot2_location.lon);
    }else{

    }

    double delta_lat = lat2_rad - lat1_rad;
    double delta_lon = lon2_rad - lon1_rad;

    double a = sin(delta_lat / 2) * sin(delta_lat / 2) + cos(lat1_rad) * cos(lat2_rad) * sin(delta_lon / 2) * sin(delta_lon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS * c;   
}

static int f_i_V2XHO_OBU_Add_Registration_List(struct v2xho_obu_registration_t *reg_info, int list_num, v2xho_wsmp_t *recv_wsmp)
{
    v2xho_rsu_ha_registration_info_t *reg_list = &(reg_info->reg_list[list_num]);
    struct v2xho_rsu_ha_registration_sub_info_t *reg_sub_info = &(reg_info->reg_sub_list[list_num]);

    if(reg_sub_info->isempty)
    {
        reg_sub_info->isempty = false;
        memcpy(&reg_sub_info->wsmp, recv_wsmp, sizeof(v2xho_wsmp_t));
        memcpy(&reg_list->destination_addess, recv_wsmp->wra.ip_prefix + (V2XHO_IPV6_PREFIX_LEN_COMPATIBLE / 8), V2XHO_IPV4_ALEN_BIN);
        reg_list->time_to_live = 1;
        reg_list->source_port = 434;
        reg_list->destination_port = 434;
        int16_t t = 1800;
        reg_list->req_payload.type = 1;
        reg_list->req_payload.flag.S = 1;
        reg_list->req_payload.flag.D = 0;
        reg_list->req_payload.flag.B = 0;
        reg_list->req_payload.flag.M = 0;
        reg_list->req_payload.flag.G = 0;
        reg_list->req_payload.flag.r = 0;
        reg_list->req_payload.flag.T = 0;
        reg_list->req_payload.flag.x = 0;
        reg_list->req_payload.lifetime  = htons(t);
        if(reg_info->home_agent_num == 0xFF)
        {
            switch(reg_sub_info->agent_type)
            {
                case V2XHO_OBU_FOREIGN_AGENT:
                {
                    if(reg_list->req_payload.home_agent == reg_list->destination_addess)
                    {

                    }
                    break;
                }
                case V2XHO_OBU_UNKNOWN_AGENT:
                {
                    memcpy(reg_list->dest_mac, recv_wsmp->src_mac, 6);
                    srand(time(NULL));
                    struct in_addr addr_bin = {.s_addr = ((htonl(reg_list->destination_addess) & 0xFFFFFF00) + 28)};//(rand() % 20 + 20))};
                    addr_bin.s_addr = htonl(addr_bin.s_addr);
                    inet_ntop(AF_INET, &addr_bin, G_v2xho_config.v2xho_interface_IP, V2XHO_IPV4_ALEN_STR);
                    reg_list->source_address = addr_bin.s_addr;
                    int prefix = 32;
                    F_i_V2XHO_IProuter_Address_Set(G_v2xho_config.v2xho_interface_name, G_v2xho_config.v2xho_interface_IP, &prefix, 0);
                    memcpy(&reg_list->req_payload.home_address, &addr_bin.s_addr, V2XHO_IPV4_ALEN_BIN);
                    reg_list->req_payload.home_agent = reg_list->destination_addess;
                    reg_list->req_payload.care_of_address = reg_list->destination_addess;
                    reg_list->req_payload.identification =  lclock_ms;
                    reg_list->metric = V2XHO_METRIC_STATIC_ACTIVE_WITH_GW;
                    break;
                }
                case V2XHO_OBU_HOME_AGENT:
                {
                    break;
                }
                default:break;
            } 
        }else{
            if(list_num == reg_info->home_agent_num)
            {

            }else{
                reg_list->time_to_live = 1;
                v2xho_rsu_ha_registration_info_t *ha_reg_list = &(reg_info->reg_list[reg_info->home_agent_num]);
                memcpy(reg_list->dest_mac, ha_reg_list->dest_mac, 6);
                reg_list->source_address = ha_reg_list->source_address;  
                reg_list->req_payload.home_address = ha_reg_list->req_payload.home_address;
                reg_list->req_payload.home_agent = ha_reg_list->req_payload.home_agent;
                reg_list->req_payload.care_of_address = reg_list->destination_addess;
                reg_list->req_payload.identification =  lclock_ms;
            }
        }
        char addr_str[V2XHO_IPV4_ALEN_STR] = {0x00,};
        inet_ntop(AF_INET, &reg_list->destination_addess, addr_str, V2XHO_IPV4_ALEN_STR);
        int argc = 0;
        for (int row = 0; row < 12; row++) {
            memset(g_argv[row], 0x00, 32);
        }
        memcpy(g_argv[argc], addr_str, strlen(addr_str)); argc++;
        memcpy(g_argv[argc], "via", strlen("via")); argc++;
        memcpy(g_argv[argc], addr_str, strlen(addr_str)); argc++;
        memcpy(g_argv[argc], "dev", strlen("dev")); argc++;
        memcpy(g_argv[argc], G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name)); argc++;
        memcpy(g_argv[argc], "onlink", strlen("onlink")); argc++;
        memcpy(g_argv[argc], "metric", strlen("metric")); argc++;
        reg_list->metric = V2XHO_METRIC_STATIC_ACTIVE_WITH_GW;
        sprintf(g_argv[argc], "%d", V2XHO_METRIC_STATIC_ACTIVE_WITH_GW); argc++;
        F_i_V2XHO_Iproute_Route_Modify(RTM_NEWROUTE, NLM_F_CREATE, argc, g_argv);

        reg_sub_info->recv_count = 1;
        reg_sub_info->recv_count_during_3s = 1;
        reg_sub_info->recv_time = lclock_ms;
        memset(reg_sub_info->recv_time_list, 0x00, sizeof(double) * 128);
        reg_sub_info->recv_time_list[0] = lclock_ms;
        memset(reg_sub_info->rxpower_list, 0x00, sizeof(WalPower) * 128);
        reg_sub_info->rxpower_list[0] = lclock_ms;
        reg_sub_info->distance = f_d_V2XHO_OBU_Calcuration_Distance(list_num);
    }else{
        if(reg_info->home_agent_num == 0xFF)
        {
            switch(reg_sub_info->agent_type)
            {
                case V2XHO_OBU_FOREIGN_AGENT:
                {
                    if(reg_list->req_payload.home_agent == reg_list->destination_addess)
                    {
                    }
                    break;
                }
                case V2XHO_OBU_UNKNOWN_AGENT:
                {
                    srand(time(NULL));
                    struct in_addr addr_bin = {.s_addr = ((htonl(reg_list->destination_addess) & 0xFFFFFF00) + (rand() % 20 + 20))};
                    inet_ntop(AF_INET, &addr_bin, G_v2xho_config.v2xho_interface_IP, V2XHO_IPV4_ALEN_STR);
                    int prefix = 32;
                    F_i_V2XHO_IProuter_Address_Set(G_v2xho_config.v2xho_interface_name, G_v2xho_config.v2xho_interface_IP, &prefix, 0);
                    memcpy(&reg_list->req_payload.home_address, &addr_bin.s_addr, V2XHO_IPV4_ALEN_BIN);
                    reg_list->req_payload.home_agent = reg_list->destination_addess;
                    reg_list->req_payload.care_of_address = reg_list->destination_addess;
                    reg_list->req_payload.identification =  lclock_ms;
                    break;
                }
                case V2XHO_OBU_HOME_AGENT:
                {
                    break;
                }
                default:break;
            } 
        }else{
            v2xho_rsu_ha_registration_info_t *ha_reg_list = &(reg_info->reg_list[reg_info->home_agent_num]);
            memcpy(reg_list->dest_mac, ha_reg_list->dest_mac, 6);
            reg_list->source_address = ha_reg_list->source_address;  
            reg_list->req_payload.home_address = ha_reg_list->req_payload.home_address;
            reg_list->req_payload.home_agent = ha_reg_list->req_payload.home_agent;
            reg_list->req_payload.care_of_address = reg_list->destination_addess;
        }

        reg_sub_info->recv_count++;
        reg_sub_info->recv_time = lclock_ms;
        for(int i = 0; i < 128; i++)
        {
            reg_sub_info->recv_time_list[i + 1] = reg_sub_info->recv_time_list[i];
            reg_sub_info->rxpower_list[i + 1] = reg_sub_info->rxpower_list[i];
        }
        reg_sub_info->recv_time_list[0]  = reg_sub_info->recv_time;
        reg_sub_info->rxpower_list[0] = reg_sub_info->wsmp.rx_power;
        for(int i = 0; i < 128; i++)
        {
            if(reg_sub_info->recv_time - reg_sub_info->recv_time_list[i] >= 3)
            {
                reg_sub_info->recv_time_list[i] = 0;
                reg_sub_info->rxpower_list[i] = 0;
                reg_sub_info->recv_count_during_3s = i;
            }
        }

        reg_sub_info->distance = f_d_V2XHO_OBU_Calcuration_Distance(list_num);

    }
    return 0;
}

static int f_i_V2XHO_Check_Agent_Priority(struct v2xho_obu_registration_t *reg_info)
{
    int ret = 0xFF;
    double distance_min = 1000 * 1000;;
    
    for(int now = 0; now  < 16; now++)
    {
        v2xho_rsu_ha_registration_info_t *reg_list = &(reg_info->reg_list[now]);
        struct v2xho_rsu_ha_registration_sub_info_t *reg_sub_info = &(reg_info->reg_sub_list[now]);
        if(reg_sub_info->isempty)
        {

       }else{
            switch(reg_list->destination_addess)
            {
                case 0x0A0A000A:
                {
                    g_distance_logging[0] = reg_sub_info->distance;
                    break;
                }
                case 0x1414000A:
                {
                    g_distance_logging[1] = reg_sub_info->distance;
                    break;
                }
                case 0x1E1E000A:
                {
                    g_distance_logging[2] = reg_sub_info->distance;
                    break;
                }
                default:break;
            }
            if(distance_min > reg_sub_info->distance)
            {
                distance_min = reg_sub_info->distance;
                ret = now;
            }
        }
    }
    return ret;
}

static void *th_v_V2XHO_OBU_Menual_Handover_Task(void *d)
{
    d = (void*)d;

    int ret;
    struct winsize frame_main;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &frame_main) == -1) {
        perror("ioctl 실패");
    }
    frame_main.ws_col = V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL;
    if(frame_main.ws_row < V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW) 
    {
        frame_main.ws_row = V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 2;
    }

    if (ioctl(STDOUT_FILENO, TIOCSWINSZ, &frame_main) == -1) {
        perror("터미널 크기 변경 실패");
    }
    char frame_main_background[V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 2][V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL + 2];
    
    memset(&frame_main_background[0][0], '*', V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL);
    memset(&frame_main_background[V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW][0], '*', V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL);
    for(int i = 1; i <= V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW - 1; i++)
    {
        memset(&frame_main_background[i][0], ' ', V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL + 1);
        memset(&frame_main_background[i][0], '*', 1);
        memset(&frame_main_background[i][V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL - 1], '*', 1);
    }

    memcpy(&frame_main_background[1][4], "DASHBOARD", 9);
    memcpy(&frame_main_background[3][4], "AGENT_LIST_NUMBER:", 18);
    memcpy(&frame_main_background[4][4], "AGENT_LIST_INFO.  ", 18);
    memcpy(&frame_main_background[5][4], "N", 1);
    memcpy(&frame_main_background[5][8], "A_M", 3);
    memcpy(&frame_main_background[5][28], "A_A", 3);
    memcpy(&frame_main_background[5][45], "W_R_T_S", 7);//WSA Receive Time 
    memcpy(&frame_main_background[5][60], "W_R_C", 5);//WSA Receive Count
    memcpy(&frame_main_background[5][69], "A_T", 3);
    memcpy(&frame_main_background[5][77], "D_R", 3);//Duration Replying
    memcpy(&frame_main_background[5][87], "A_D", 3);
    memcpy(&frame_main_background[5][100], "A_S", 3);
#if 0
    memcpy(&frame_main_background[17][4], "OBU_ACTION_LIST.", 16);
    memcpy(&frame_main_background[18][4], "Time", 4);
    memcpy(&frame_main_background[18][19], "To", 2);
    memcpy(&frame_main_background[18][23], "Action", 6);
    memcpy(&frame_main_background[18][43], "State", 5);
    memcpy(&frame_main_background[18][57], "ReplyCode", 9);
#endif
    int32_t time_fd;
	struct itimerspec itval;
    int msec = 500;
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
    uint64_t res;

    system("clear");
    lcmove(0,0);
    for(int m = 0; m <= V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW; m++)
    {
        if(1)
        {
            for(int j = 0; j <= V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL; j++)
            {
                printf("%c", frame_main_background[m][j]);
            }
        }else{
            printf("%s", frame_main_background[m]);
        }
        printf("\n");
    }
    int agent_list_row = 7;
    //int obu_action_row = 19;
    //int command_prompt_row = V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW;

    struct v2xho_obu_registration_t *reg_info = g_reg_info;
    int registed_num = 0;
    printf("\033[4;23H""%02d", registed_num);

    pthread_t prompt;
    pthread_create(&prompt, NULL, th_v_V2XHO_OBU_Menual_Handover_Command_Prompt, NULL);
    pthread_detach(prompt);
    while(1)
    {
        ret = read(time_fd, &res, sizeof(res));
        if(ret >= 0)
        {
            if(reg_info->registed_num != registed_num)
            {
                registed_num = reg_info->registed_num;
                printf("\033[4;0H""%s", frame_main_background[3]);
                printf("\033[4;23H""%02d", registed_num);
            }
            
            if(reg_info->registed_num > 0)
            {
                int now;
                for(now = 0; now < reg_info->registed_num; now++)
                {
                    if(now > 10)
                    {
                        break;
                    }
                    v2xho_rsu_ha_registration_info_t *reg_list = (v2xho_rsu_ha_registration_info_t*)&(reg_info->reg_list[now]);
                    struct v2xho_rsu_ha_registration_sub_info_t *reg_sub_info = (struct v2xho_rsu_ha_registration_sub_info_t *)&(reg_info->reg_sub_list[now]);
                    char addr_str[V2XHO_IPV4_ALEN_STR];
                    char agent_type[6];
                    char agent_state[17];
                    double wrts_time;
                    memset(addr_str, ' ',  V2XHO_IPV4_ALEN_STR);
                    inet_ntop(AF_INET, &reg_list->destination_addess, addr_str, V2XHO_IPV4_ALEN_STR);
                    if(now == reg_info->home_agent_num)
                    {   
                        memcpy(agent_type, "HOME  ", 6);
                    }else{
                        memcpy(agent_type, "FOREAN", 6);
                    }
                    if(now == reg_info->connecting_now)
                    {   
                        memcpy(agent_state, "Connected        ", 17);
                    }else{
                        if(now == reg_info->home_agent_num)
                        {
                            memcpy(agent_state, "Connected_with_FA", 17);
                        }
                        memcpy(agent_state, "Stand_by         ", 17);
                    }
                    wrts_time = lclock_ms - reg_sub_info->recv_time_list[0];
                    if(wrts_time < 0 && wrts_time > 999999)
                    {
                        wrts_time = -1;
                    }
                    printf(AGENT_LIST_PRINT_PAD, agent_list_row + now, now, V2XHO_MAC_PAD(reg_sub_info->wsmp.src_mac), addr_str, wrts_time, reg_sub_info->recv_count, agent_type, reg_sub_info->last_reg_rep_recv_time - reg_sub_info->last_reg_rep_recv_time, reg_sub_info->distance, agent_state);
                }   
                for(now = now; now < 10; now++)
                {
                    //lcmove(0, agent_list_row + now);
                    //for(int j = 0; j <= V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL; j++)
                    {
                        printf("\033[%d;1H""%s", agent_list_row + now, frame_main_background[agent_list_row + now]);
                    }
                }
            }      
        }
        lcmove(0,V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 3);
    }
    close(time_fd);
    return (void*)NULL;
    //lSafeFree(command_list[0]);
}


void f_v_V2xHO_OBU_EnableRawMode() 
{
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);           // 현재 터미널 설정 가져오기
    t.c_lflag &= ~(ICANON | ECHO);         // Canonical 모드와 에코 비활성화
    tcsetattr(STDIN_FILENO, TCSANOW, &t);  // 새로운 설정 적용
}

void f_v_V2xHO_OBU_DisableRawMode(struct termios *original)
{
    tcsetattr(STDIN_FILENO, TCSANOW, original);  // 원래 설정 복원
}

void f_v_V2XHO_OBU_Menual_Handover_Command_Prompt_Action(char *cmd, int cmd_len)
{
    cmd_len = (int)cmd_len;
    switch(cmd[0])
    {
        case 'h':
        {
            char *ptr = strtok(cmd, " ");
            if(strncmp(ptr, "handover", 8) == 0)
            {
                ptr = strtok(NULL, " ");
                int agent_num = atoi(ptr);
                printf("\033[%d;1H""Action Munual Handover; Registraion Request To Agent[%d]           ", V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 9, agent_num);
                struct v2xho_obu_registration_t *reg_info = g_reg_info;
                if(agent_num >= 0 && agent_num < reg_info->registed_num)
                {
                    v2xho_equip_state_obu_t *status = (v2xho_equip_state_obu_t*)&(g_obu_info->status.u.stats_obu);
                    if(status->waiting_regreply == 0)
                    {
                        reg_info->registration_request_send_num = agent_num;
                    }
                }else if(agent_num == 99)
                {
                    printf("\033[%d;1H""Action Auto Handover; Registraion Request To Agent[%d]           ", V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 9, agent_num);
                    g_agent_num = agent_num;
                }else{
                    printf("\033[%d;1H""Action Munual Handover; Error Agent Num [%d/%d]          ", V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 10, agent_num, reg_info->registed_num);
                }

            }
            break;
        }
        default:
        {
            printf("\033[%d;1H""Wrong Command:%s", 37, cmd);
            break;
        }
    }
    return;
}

static void *th_v_V2XHO_OBU_Menual_Handover_Command_Prompt(void *d)
{
    d = (void*)d;

    char command_prompt_main[V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL + 1];
    char command_prompt_main_with_command[V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL + 1];
    char command_prompt_main_with_error[V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL + 1];
    memset(command_prompt_main, ' ', V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL);
    memset(command_prompt_main_with_command, ' ', V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL);
    memset(command_prompt_main_with_error, ' ', V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_COL);

    int promft_p_main = strlen(V2XHO_DASHBOARD_COMMAND_NAME) + strlen(V2XHO_DASHBOARD_COMMAND_SCRIPT_INPUT) + 1;
    memcpy(command_prompt_main, V2XHO_DASHBOARD_COMMAND_NAME " " V2XHO_DASHBOARD_COMMAND_SCRIPT_INPUT, promft_p_main);
    int promft_p_input = strlen(V2XHO_DASHBOARD_COMMAND_NAME) + 2;
    memcpy(command_prompt_main_with_command, V2XHO_DASHBOARD_COMMAND_NAME, promft_p_input);
    int promft_p_error = strlen(V2XHO_DASHBOARD_COMMAND_NAME) + strlen(V2XHO_DASHBOARD_COMMAND_SCRIPT_ERROR) + 1;
    memcpy(command_prompt_main_with_error, V2XHO_DASHBOARD_COMMAND_NAME " " V2XHO_DASHBOARD_COMMAND_SCRIPT_ERROR, promft_p_error);
    
    int command_prompt_row = V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 2;

    printf("\033[%d;1H""%s""\033[%dG", command_prompt_row, command_prompt_main, promft_p_input);
    char command[12][128];
    struct termios old_ter;
    tcgetattr(STDIN_FILENO, &old_ter);
    
    f_v_V2xHO_OBU_EnableRawMode();
    char c;
    int cmd_str_len = 0;
    //int cmd_list_num = 0;
    int cur_ptr = promft_p_input;
    while(1)
    {   
        c = getchar();
        switch(c)
        {
            case '\n':
            {  
                f_v_V2XHO_OBU_Menual_Handover_Command_Prompt_Action(command[0], 128);
                //printf("\033[%d;1H""[DEBUG]INPUT:%s", command_prompt_row + 3, command_ptr);
                cmd_str_len = 0;
                cur_ptr = promft_p_input;
                memset(command[0], 0x00, 128);
                printf("\033[%d;1H""%s""\033[%dG", command_prompt_row, command_prompt_main, promft_p_input);
                break;
            }
            case '\t':
            {
                switch(command[0][0])
                {
                    case 'h':
                    {
                        memcpy(command, "handover ", 9);
                        cur_ptr = promft_p_input + 9;
                        cmd_str_len = 9;
                        printf("\033[%d;%dH""%s", command_prompt_row, promft_p_input, command[0]);
                        printf("\033[%d;%dH", command_prompt_row, cur_ptr);
                        break;
                    }
                    default:break;
                }
                break;
            }
            case '\b':
            {
                if(cur_ptr == promft_p_input)
                {

                }else{
                    printf("\033[%d;%dH""\b \b", command_prompt_row, cur_ptr);
                    cur_ptr--;
                    cmd_str_len--;
                    if(cur_ptr == promft_p_input + cmd_str_len)
                    {
                        command[0][cmd_str_len] = ' '; 
                    }else{
                        int del_p = cur_ptr - promft_p_input;
                        for(int i = del_p; i < cmd_str_len; i++)
                        {
                            command[0][i] = command[0][i + 1];
                            printf("\033[%d;%dH""%c", command_prompt_row, promft_p_input + i, command[0][i]);
                        }
                        command[0][cmd_str_len] = ' '; 
                        printf("\033[%d;%dH""%c", command_prompt_row, promft_p_input + cmd_str_len, command[0][cmd_str_len]);
                        printf("\033[%d;%dH", command_prompt_row, cur_ptr);
                    }
                }
                
                break;
            }
            case '\033':
            {
                char escseq = getchar();
                if (escseq == '[') {
                    escseq = getchar(); // 세 번째 문자
                    switch (escseq) {
                        case 'A':
                            //printf("위쪽 키 입력됨\n");
                            break;
                        case 'B':
                            //printf("아래쪽 키 입력됨\n");
                            break;
                        case 'C':
                        {
                            if(cur_ptr == promft_p_input + cmd_str_len)
                            {
                                printf("\033[%d;%dH", command_prompt_row, cur_ptr);
                            }else{
                                printf("\033[%d;%dH""\033[C", command_prompt_row, cur_ptr);
                                cur_ptr++;
                            }
                            //printf("오른쪽 키 입력됨\n");
                            break;
                        }
                        case 'D':
                        {
                            if(cur_ptr == promft_p_input)
                            {
                                printf("\033[%d;%dH", command_prompt_row, cur_ptr);
                            }else{
                                printf("\033[%d;%dH""\033[D", command_prompt_row, cur_ptr);
                                cur_ptr--;
                            }
                            //printf("왼쪽 키 입력됨\n");
                            break;
                        }
                        case '3':
                        {
                            escseq = getchar(); // 세 번째 문자
                            if(escseq == '~')
                            {
                                cmd_str_len--;
                            }
                            break;
                        }
                        default:
                            //printf("알 수 없는 키 입력: %c\n", seq[2]);
                            break;
                    }
                }
                break;
            }
            default:
            {
                if(c >=32 && c <=126)
                {
                    if(cmd_str_len == 0)
                    {
                        printf("\033[%d;%dH" "                 " "\033[%dG", command_prompt_row, promft_p_input, promft_p_input);
                    }
                    printf("\033[%d;%dH""%c", command_prompt_row, cur_ptr, c);
                    if(cur_ptr == promft_p_input + cmd_str_len)
                    {
                        command[0][cmd_str_len] = c;
                    }else{
                        int add_p = cur_ptr - promft_p_input;
                        command[0][add_p] = c;
                    }
                    cmd_str_len++;
                    cur_ptr++;
                }else{
                    printf("\033[%d;1H" "%s""\033[%dG:Wrong input value:%d", command_prompt_row, command_prompt_main_with_error, promft_p_error, c);
                    cmd_str_len = 0;
                    cur_ptr = promft_p_input;
                    memset(command[0], 0x00, 128);
                }
                break;
            }
        }
        printf("\033[%d;%dH", command_prompt_row, cur_ptr);
    }
    f_v_V2xHO_OBU_DisableRawMode(&old_ter);
    return (void*)NULL;
}

struct ping_time_check_t g_ping_timing;

static void *th_v_V2XHO_Util_Pingtest_Task_Send_Running(void *d)
{   
    struct v2xho_debug_pingtest_info_t *ping_info = (struct v2xho_debug_pingtest_info_t *)d;
    g_ping_isrun = 0;
    const char *target_ip = ping_info->dest_addr;
    int sockfd;
    char icmp_buf[526];
    struct sockaddr_ll dest_addr;
    struct ifreq ifr;
    uint32_t packet_size;
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("Socket creation failed");
    }

    #if 0
    // Construct Ethernet header
    struct ethhdr *eth = (struct ethhdr *)icmp_buf;
    memset(eth->h_dest, 0xff, ETH_ALEN); // Broadcast MAC address
    WAL_GetIfMACAddress(0, eth->h_source);
    eth->h_proto = htons(ETH_P_IP);
    #endif

    // Construct IP header
    struct iphdr *ip = (struct iphdr *)(icmp_buf);
    g_ping_ip_hdr = ip;
    ip->version = 4;
    ip->ihl = 5;
    ip->tos = 0;
    ip->tot_len = htons(ping_info->packet_size);
    ip->id = htons(1);
    ip->frag_off = 0;
    ip->ttl = 64;
    ip->protocol = IPPROTO_ICMP;
    ip->daddr = inet_addr(target_ip); // Replace with destination IP
    struct v2xho_obu_registration_t *reg_info = g_reg_info;
    while(reg_info->home_agent_num == 0xFF)
    {
        sleep(1);
    }
    ip->saddr = inet_addr(G_v2xho_config.v2xho_interface_IP);
    ip->check = 0;
    ip->check = F_u16_V2XHO_IP_Checksum((unsigned short *)ip, sizeof(struct iphdr));

    setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name));

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, G_v2xho_config.v2xho_interface_name, IFNAMSIZ - 1); // Replace "eth0" with your interface
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        perror("Interface index fetch failed");
        close(sockfd);
    }
    // Prepare destination address
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sll_ifindex = ifr.ifr_ifindex;
    dest_addr.sll_halen = ETH_ALEN;
    memset(dest_addr.sll_addr, 0xff, ETH_ALEN); // Broadcast MAC address

    struct icmphdr *icmp = (struct icmphdr *)(icmp_buf + sizeof(struct iphdr));
    g_ping_icmp_hdr = icmp;
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_id = htons(getpid() & 0xFFFF);
    icmp->icmp_cksum = 0;

    packet_size = ping_info->packet_size - sizeof(struct iphdr) - sizeof(struct icmphdr);
    memset(icmp_buf + sizeof(struct iphdr) + sizeof(struct icmphdr), 0xa5, packet_size);

    int32_t  time_fd;
	struct itimerspec itval;
    if ((time_fd = timerfd_create(CLOCK_MONOTONIC, 0)) < 0) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    itval.it_value.tv_sec = 1;
    itval.it_value.tv_nsec = 0;
    itval.it_interval.tv_sec = (ping_info->interval_ms / 1000);
    itval.it_interval.tv_nsec = (ping_info->interval_ms % 1000) * 1e6;

    timerfd_settime(time_fd, TFD_TIMER_ABSTIME, &itval, NULL);

    // 타임아웃 설정
    struct timeval tv;
    tv.tv_sec = ping_info->interval_ms / 1000000;
    tv.tv_usec = (10 % 1000000);
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    tv.tv_usec = 500 * 1000;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    uint64_t res;
    int ret;
    int icmp_seq_num = 1;

    if(G_v2xho_config.debug_pingtest_pcap_enable)
    {
        while(g_reg_info->home_agent_num == 0xFF)
        {
            usleep(100 * 1000);
        }
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char cmd[128] = {'\0'};
        sprintf(cmd, "mkdir -m 777 -p ~/pcap_log/%04d_%02d_%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
        lsystem(cmd, ret);
        sprintf(cmd, "tcpdump -i %s -nnvv -w ~/pcap_log/%04d_%02d_%02d/%04d_%02d_%02d.pcap &", G_v2xho_config.v2xho_interface_name, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
        lsystem(cmd, ret);
    }else{
        pthread_t th_ping_recv;
        pthread_create(&th_ping_recv, NULL, th_v_V2XHO_Util_Pingtest_Task_Recv_Running, NULL);
        pthread_detach(th_ping_recv);
    }


    while(1)
    {
        g_ping_recv_isnow = false;
        ret = read(time_fd, &res, sizeof(res));
        if(ret > 0)
        {
            icmp->icmp_seq = icmp_seq_num;
            icmp_seq_num++;
            icmp->icmp_cksum  = 0;
            icmp->icmp_cksum  = F_u16_V2XHO_UDP_Checksum((unsigned short *)icmp, ping_info->packet_size - sizeof(struct iphdr));
            //double start_time = lclock_ms;

            // ICMP 패킷 전송
            if(ip->saddr != 0x00000000)
            {
                ret = sendto(sockfd, icmp_buf, ping_info->packet_size + sizeof(struct ethhdr), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                g_ping_isrun = 1;
            }
            if(ret < 0 )
            {
                perror("Send failed");
                continue;
            }else{
                g_ping_timing.icmp_send_seq = icmp->icmp_seq;
                g_ping_timing.send_timing = lclock_ms;
            }
        }
    }
    close(time_fd);
    close(sockfd);
    return 0;
}

static void *th_v_V2XHO_Util_Pingtest_Task_Recv_Running(void *d)
{
    d = (void*)d;

    int sockfd;
    struct ifreq ifr;
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) 
    {
        perror("Socket creation failed");
    }  

    //setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name));
    int buf_size = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &buf_size, sizeof(buf_size)) < 0) {
        perror("setsockopt");
        close(sockfd);
    }
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, G_v2xho_config.v2xho_interface_name, IFNAMSIZ - 1); // Replace "llc-cch-ipv6" with your interface
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) 
    {
        perror("Interface index fetch failed");
        close(sockfd);
    }
    int ret;
    char recv_buf[512];
    struct sockaddr_ll saddrll;
    socklen_t src_addr_len = sizeof(saddrll);
    double recv_timing;
    //double prev_timing;
    //uint16_t prev_seq;
    //uint8_t prev_mac[6];
    g_logging_fd = 0;
    g_icmp_recv_num = 0;
    if(G_v2xho_config.debug_pingtest_logging_enable)
    {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char folder_name[16] = {'\0'};
        snprintf(folder_name, sizeof(folder_name), "%04d_%02d_%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
        f_i_V2XHO_OBU_UDS_Logging_FileDB_Setup(folder_name);
        char folder_path[128] = {'\0'};
        sprintf(folder_path, "%s%s",V2XHO_DEFAULT_FILEDB_PATH, folder_name);
        
        ret = access(folder_path, F_OK);
        if(ret >= 0)
        {
            unlink(folder_path);
            char file_name[100];
            sprintf(file_name, "%s/%02d_%02d_%02d.log", folder_path, t->tm_hour, t->tm_min, t->tm_sec);
            g_logging_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        }
    }

    while(1)
    {
        if(g_ping_isrun)
        {
            while(1)
            { 
                g_ping_recv_isnow = false;
                memset(recv_buf, 0x00, 512);
                ret = recvfrom(sockfd, recv_buf, 512, 0, (struct sockaddr *)&(saddrll), &src_addr_len);
                recv_timing = lclock_ms;
                if(ret <= 0)
                {
                    //printf("Receive Fail! %f %d %d\n", lclock_ms - recv_time_0, ret, waiting_timer);
                }else{
                    struct ethhdr *eth_header;
                    struct iphdr *ip_header;
                    eth_header = (struct ethhdr *)recv_buf;
                    ip_header = (struct iphdr *)(recv_buf + sizeof(struct ethhdr));
                    switch(saddrll.sll_pkttype)
                    {
                        default:break;
                        case PACKET_HOST:
                        case PACKET_MULTICAST:
                        case PACKET_BROADCAST:
                        {
                            switch(ip_header->protocol)
                            {
                                default: break;
                                case IPPROTO_ICMP:
                                {                                
                                    struct icmphdr *icmp_resp =  (struct icmphdr *)(recv_buf + sizeof(struct ethhdr) + sizeof(struct iphdr));
                                    if (icmp_resp->icmp_type == ICMP_ECHOREPLY)
                                    {
                                        if(g_ping_icmp_hdr->icmp_id == icmp_resp->icmp_id)
                                        {
                                            if(1)//g_ping_timing.icmp_send_seq == icmp_resp->icmp_seq)
                                            {
                                                g_ping_recv_isnow = true;
                                                g_icmp_recv_num++;
                                                printf("\33[%d;1H""[DEBUG][%f]src:%02X:%02X:%02X:%02X:%02X:%02X RTT:%06.3f[s] ", V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 5, lclock_ms, V2XHO_MAC_PAD(eth_header->h_source), recv_timing - g_ping_timing.send_timing);                                            
                                                if(G_v2xho_config.debug_pingtest_logging_enable)
                                                {
                                                    #if 0 
                                                    memcpy(log_data.src_MAC, eth_header->h_source, 6);
                                                    inet_ntop(AF_INET, &ip_header->saddr, log_data.ip_src_add_str, V2XHO_IPV4_ALEN_STR);
                                                    inet_ntop(AF_INET, &ip_header->daddr, log_data.ip_dst_add_str, V2XHO_IPV4_ALEN_STR);
                                                    log_data.icmp_seq_num = icmp_resp->icmp_seq;
                                                    log_data.recv_time = recv_timing;
                                                    log_data.lat = G_gnss_data->lat;
                                                    log_data.lon = G_gnss_data->lon;
                                                    log_data.distance_1 = g_distance_logging[0];
                                                    log_data.distance_2 = g_distance_logging[1];
                                                    log_data.distance_3 = g_distance_logging[2];
                                                    log_data.ext[0] = ';';
                                                    log_data.ext[1] = '\n';
                                                    char log_buf[256] = {'\0'};
                                                    sprintf(log_buf,"%02X:%02X:%02X:%02X:%02X\t%s\t%s\t%05d\t%06.3f\t%09d\t%09d\t%06.3f\t%06.3f\t%06.3f;\n",V2XHO_MAC_PAD(eth_header->h_source),
                                                                                                                                                            log_data.ip_src_add_str,
                                                                                                                                                            log_data.ip_dst_add_str,
                                                                                                                                                            log_data.icmp_seq_num,
                                                                                                                                                            log_data.recv_time,
                                                                                                                                                            log_data.lat,
                                                                                                                                                            log_data.lon,
                                                                                                                                                            log_data.distance_1,
                                                                                                                                                            log_data.distance_2,
                                                                                                                                                            log_data.distance_3);
                                                                                                                                                            inet_ntop(AF_INET, &ip_header->saddr, ip_src_add_str, V2XHO_IPV4_ALEN_STR);
                                                    #endif
                                                    char log_buf[256] = {'\0'};
                                                    char ip_src_add_str[V2XHO_IPV4_ALEN_STR];
                                                    char ip_dst_add_str[V2XHO_IPV4_ALEN_STR];
                                                    inet_ntop(AF_INET, &ip_header->daddr, ip_src_add_str, V2XHO_IPV4_ALEN_STR);
                                                    inet_ntop(AF_INET, &ip_header->daddr, ip_dst_add_str, V2XHO_IPV4_ALEN_STR);
                                                    sprintf(log_buf,"%02X:%02X:%02X:%02X:%02X:%02X\t%s\t%s\t%05d\t%06.3f\t%09d\t%09d\t%06.3f\t%06.3f\t%06.3f;\n",   V2XHO_MAC_PAD(eth_header->h_source),
                                                                                                                                                                    ip_src_add_str,
                                                                                                                                                                    ip_dst_add_str,
                                                                                                                                                                    icmp_resp->icmp_seq,
                                                                                                                                                                    lclock_ms,
                                                                                                                                                                    G_gnss_data->lat,
                                                                                                                                                                    G_gnss_data->lon,
                                                                                                                                                                    g_distance_logging[0],
                                                                                                                                                                    g_distance_logging[1],
                                                                                                                                                                    g_distance_logging[2]);
                                                    write(g_logging_fd, log_buf, strlen(log_buf));
                                                }
                                                #if 0
                                                int ii;
                                                for(ii = 0; ii < 6; ii++)
                                                {
                                                    if(prev_mac[ii] != eth_header->h_source[ii])
                                                    {
                                                        ii = 10;
                                                        printf("\33[%d;1H""[DEBUG][%f]prev src:%02X:%02X:%02X:%02X:%02X:%02X, new src::%02X:%02X:%02X:%02X:%02X:%02X, seq:%d/%d RTT:%06.3f[s] Latancy:%06.3f[s]", V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 4, lclock_ms,
                                                        V2XHO_MAC_PAD(prev_mac), V2XHO_MAC_PAD(eth_header->h_source), prev_seq, icmp_resp->icmp_seq, recv_timing - g_ping_timing.send_timing, recv_timing - prev_timing);
                                                        break;
                                                    }
                                                }
                                                if(ii == 10)
                                                {                    
                                                    for(ii = 0; ii < 6; ii++)
                                                    {
                                                        prev_mac[ii] = eth_header->h_source[ii];
                                                    }
                                                }
                                                prev_timing = recv_timing;
                                                prev_seq = icmp_resp->icmp_seq;
                                                #endif
                                            }
                                        }
                                    }
                                    break;
                                }
                                case IPPROTO_UDP:
                                {
                                    #if 0
                                    struct udphdr *udp_resp =  (struct udphdr *)(recv_buf + sizeof(struct ethhdr) + sizeof(struct iphdr));
                                    if (htons(udp_resp->dest) == 11245)
                                    {
                                        int ii;
                                        for(ii = 0; ii < 6; ii++)
                                        {
                                            if(prev_mac[ii] != eth_header->h_source[ii])
                                            {
                                                ii = 10;
                                                printf("\33[%d;1H""[DEBUG][%f]prev src:%02X:%02X:%02X:%02X:%02X:%02X, new src::%02X:%02X:%02X:%02X:%02X:%02X, Latancy:%06.3f[s]", V2XHO_DEFAULT_DASHBOARD_FRAME_MAIN_ROW + 4, lclock_ms,
                                                V2XHO_MAC_PAD(prev_mac), V2XHO_MAC_PAD(eth_header->h_source), recv_timing - prev_timing);
                                                break;
                                            }
                                        }
                                        if(ii == 10)
                                        {                    
                                            for(ii = 0; ii < 6; ii++)
                                            {
                                                prev_mac[ii] = eth_header->h_source[ii];
                                            }
                                        }
                                        prev_timing = recv_timing;
                                            
                                    
                                    }
                                    #endif
                                    break;
                                }
                            }
                        }
                    }
                }
                
            }
        }
        usleep(10 * 1000);
    }

    if(G_v2xho_config.debug_pingtest_logging_enable)
    {
        close(g_logging_fd);
    }
    return 0;
}


static int f_i_V2XHO_OBU_UDS_Logging_FileDB_Setup(const char *path)
{
    int ret;
    uint32_t path_len = strlen(V2XHO_DEFAULT_FILEDB_PATH) + strlen(path);
    
    char *folder_path = malloc(sizeof(uint8_t) * path_len);
    memcpy(folder_path, V2XHO_DEFAULT_FILEDB_PATH, strlen(V2XHO_DEFAULT_FILEDB_PATH));
    memcpy(folder_path + strlen(V2XHO_DEFAULT_FILEDB_PATH), path, strlen(path));

    ret = access(V2XHO_DEFAULT_FILEDB_PATH, F_OK);
    char sys_cmd[256] = {0x00,};
    if(ret >= 0)
    {
        unlink(V2XHO_DEFAULT_FILEDB_PATH);
        ret = access(folder_path, F_OK);
        if(ret >= 0 )
        {
            sprintf(sys_cmd, "mountpoint -qd %s", folder_path);
            lsystem(sys_cmd, ret);
            if(ret == 256)
            {
                sprintf(sys_cmd, "mount -t tmpfs %s %s -o size=%dM", V2XHO_DEFAULT_FILEDB_PATH, folder_path, V2XHO_DEFULAT_RAMDISK_FILEDB_SIZE);
                lsystem(sys_cmd, ret);
            }
        }else{
            sprintf(sys_cmd, "mkdir -m 777 -p %s", folder_path);
            lsystem(sys_cmd, ret); 
            sprintf(sys_cmd, "mount -t tmpfs %s %s -o size=%dM", V2XHO_DEFAULT_FILEDB_PATH, folder_path, V2XHO_DEFULAT_RAMDISK_FILEDB_SIZE);
            lsystem(sys_cmd, ret);
        }
    }else{
        sprintf(sys_cmd, "mkdir -m 777 -p %s", folder_path);
        lsystem(sys_cmd, ret); 
        sprintf(sys_cmd, "mount -t tmpfs %s %s -o size=%dM", V2XHO_DEFAULT_FILEDB_PATH, folder_path, V2XHO_DEFULAT_RAMDISK_FILEDB_SIZE);
        lsystem(sys_cmd, ret); 
    }
    lSafeFree(folder_path);

    return 0;
}