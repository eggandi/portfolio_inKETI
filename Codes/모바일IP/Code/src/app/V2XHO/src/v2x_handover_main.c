#include "v2x_handover_main.h"

/* DEFINE */
#define V2XHO_ERROR_CODE_FILE_CODE V2XHO_ERROR_CODE_FILE_CODE_MAIN
/* Functions */
static void f_v_V2XHO_Router_Initial(void); 
static int f_i_V2XHO_V2X_Initial(void); 

/* Variables */
struct V2XHO_IProuter_Route_List_info_IPv4_t G_route_list_info[DEFAULT_ROUTE_LIST_NUM_MAX]; 
int G_route_list_num; 
v2xho_config_t G_v2xho_config; 

/* Main Line */
int main(int argc, char *argv[])
{  
    int ret;
    enum v2xho_error_code_e code = V2XHO_NO_ERROR;
    
    ret = F_i_V2XHO_Initial_Setup_Configuration_Read(&G_v2xho_config);
    if(ret != 0)
    {
        lreturn(V2XHO_MAIN_INITIAL_SETUP_HANDOVER_INTERFACE_NO_IPv4_ADDRESS);
    }
    pthread_t th_gnss;
    ret = F_i_V2XHO_Gnss_Init_Gnssata(&th_gnss);
    ret = f_i_V2XHO_V2X_Initial();
    //라우팅 테이블 저장
    f_v_V2XHO_Router_Initial();
    if(ret < 0)
    {
        code = V2XHO_MAIN_INITIAL_SETUP_HANDOVER_INTERFACE_NO_IPv4_ADDRESS;
        return (code * 0x1000000) + (V2XHO_ERROR_CODE_FILE_CODE * 0x010000) + (__LINE__ % 0xFFFF);
    }

    v2xho_equip_info_t equip_state;
    memset(&equip_state, 0x00, sizeof(v2xho_equip_info_t));
    equip_state.status.equip_type = G_v2xho_config.equipment_type;
    //Config 파일에 IP를 공란으로 했을 때, 인터페이스 명을 가지고 IPv4 주소를 가져옴. 
    char empty_addr_ipv4[INET_ADDRSTRLEN] = {0,};
    int prefix = 0;
    //enum v2xho_util_router_metric_state_e metric = V2XHO_METRIC_INITIAL;
    pthread_t th_main_equip_send;
    pthread_t th_main_equip_recv;
    
    ret = F_i_V2XHO_WAVE_IPv6_Setup();

    char **g_argv = malloc(12 * sizeof(char *));
    for (int row = 0; row < 12; row++) {
        g_argv[row] = malloc(32 * sizeof(char));
        memset(g_argv[row], 0x00, 32);
    }

    switch(G_v2xho_config.equipment_type)
    {
        default:
        {
            break;
        }
        case DSRC_RSU_FA:
        {
            printf("Equiptype:%d - \033[1;32mRSU(ForeanAgent)!\033[0m\n", G_v2xho_config.equipment_type);
            printf("V2X Handover Ethernet Device:%s\n", G_v2xho_config.v2xho_interface_name);
            printf("V2X Handover IP Address:"TEXT_GREEN("%s/%d")"\n", G_v2xho_config.v2xho_interface_IP, G_v2xho_config.v2xho_interface_prefix);
            printf("V2X Handover MAC Address:%s\n", G_v2xho_config.v2xho_interface_MAC_S);
            F_i_V2XHO_IProuter_Route_Flush();
            prefix = (G_v2xho_config.v2xho_interface_prefix);
            F_i_V2XHO_IProuter_Address_Set(G_v2xho_config.v2xho_interface_name, G_v2xho_config.v2xho_interface_IP, &prefix, 1);
            
            printf("G_v2xho_config.count_onlyIP_interface:%d\n", G_v2xho_config.count_onlyIP_interface);
            for(int i = 0; i < G_v2xho_config.count_onlyIP_interface; i++)
            {

                int route_argc = 0;
                for (int row = 0; row < 12; row++) {
                    memset(g_argv[row], 0x00, 32);
                }
                if(i == 0)
                {
                    printf("Only IP Ethernet Device:%s\n", G_v2xho_config.onlyIP_interface_name[i]);
                    printf("Only IP IP Address:"TEXT_GREEN("%s/%d")"\n", G_v2xho_config.onlyIP_interface_IP[i], G_v2xho_config.onlyIP_interface_prefix[i]);
                    if(strncmp(G_v2xho_config.onlyIP_interface_IP[i], empty_addr_ipv4, V2XHO_IPV4_ALEN_STR) == 0)
                    {
                        F_v_V2XHO_IPv4_Address_Get(G_v2xho_config.onlyIP_interface_name[i], G_v2xho_config.onlyIP_interface_IP[i], &G_v2xho_config.onlyIP_interface_prefix[i]);
                    }
                    if(strncmp(G_v2xho_config.onlyIP_interface_IP[i], empty_addr_ipv4, V2XHO_IPV4_ALEN_STR) == 0)
                    {
                        lreturn(V2XHO_MAIN_INITIAL_SETUP_ONLYIP_INTERFACE_NO_IPv4_ADDRESS);
                    }
                    //TBA
                    if(1){
                        char cmd_tmp[128] = {0x00,};
                        sprintf(cmd_tmp, "ip route flush %s", G_v2xho_config.onlyIP_interface_name[i]);
                        system(cmd_tmp);
                    }                
                    
                    F_i_V2XHO_IProuter_Address_Set(G_v2xho_config.onlyIP_interface_name[i], G_v2xho_config.onlyIP_interface_IP[i], &G_v2xho_config.onlyIP_interface_prefix[i], 1);

                }else{
                    struct in_addr sub_addr_bin = {.s_addr = ((inet_addr(G_v2xho_config.onlyIP_interface_IP[i]) & 0x00FFFFFF))};
                    char sub_addr_str[V2XHO_IPV4_ALEN_STR];
                    inet_ntop(AF_INET, &sub_addr_bin.s_addr, sub_addr_str, V2XHO_IPV4_ALEN_STR);
                    memcpy(g_argv[route_argc], sub_addr_str, strlen(sub_addr_str));
                    memcpy(g_argv[route_argc] + strlen(sub_addr_str), "/24", strlen("/24")); route_argc++;
                    memcpy(g_argv[route_argc], "via", strlen("via")); route_argc++;
                    memcpy(g_argv[route_argc], G_v2xho_config.onlyIP_interface_name[i], strlen(G_v2xho_config.onlyIP_interface_name[i])); route_argc++;
                    memcpy(g_argv[route_argc], "metric", strlen("metric")); route_argc++;
                    sprintf(g_argv[route_argc], "%d", V2XHO_METRIC_DYNAMIC_ACTIVE_WITH_GW); route_argc++;
                    F_i_V2XHO_Iproute_Route_Modify(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_REPLACE, route_argc, g_argv);
                }
            }
            equip_state.status.u.stats_rsu_fa.active = 1; 
            pthread_create(&th_main_equip_send, NULL, Th_v_V2XHO_RSU_FA_Task_Running_Send, (void*)&equip_state);
            pthread_detach(th_main_equip_send);
            pthread_create(&th_main_equip_recv, NULL, Th_v_V2XHO_RSU_FA_RAW_Receive, (void*)&equip_state);
            pthread_detach(th_main_equip_recv);

            break;
        }
        case DSRC_RSU_HA:
        {
            printf("Equiptype:%d - \033[1;32mRSU(HomeAgent)!\033[0m\n", G_v2xho_config.equipment_type);
            printf("V2X Handover Ethernet Device:%s\n", G_v2xho_config.v2xho_interface_name);
            printf("V2X Handover IP Address:"TEXT_GREEN("%s/%d")"\n", G_v2xho_config.v2xho_interface_IP, G_v2xho_config.v2xho_interface_prefix);
            F_i_V2XHO_IProuter_Route_Flush();
            prefix = (G_v2xho_config.v2xho_interface_prefix);
            F_i_V2XHO_IProuter_Address_Set(G_v2xho_config.v2xho_interface_name, G_v2xho_config.v2xho_interface_IP, &prefix, 1);
            
            printf("G_v2xho_config.count_onlyIP_interface:%d\n", G_v2xho_config.count_onlyIP_interface);
            for(int i = 0; i < G_v2xho_config.count_onlyIP_interface; i++)
            {
                int route_argc = 0;
                for (int row = 0; row < 12; row++) {
                    memset(g_argv[row], 0x00, 32);
                }
                if(i == 0)
                {
                    printf("Only IP Ethernet Device:%s\n", G_v2xho_config.onlyIP_interface_name[i]);
                    printf("Only IP IP Address:"TEXT_GREEN("%s/%d")"\n", G_v2xho_config.onlyIP_interface_IP[i], G_v2xho_config.onlyIP_interface_prefix[i]);
                    if(strncmp(G_v2xho_config.onlyIP_interface_IP[i], empty_addr_ipv4, V2XHO_IPV4_ALEN_STR) == 0)
                    {
                        F_v_V2XHO_IPv4_Address_Get(G_v2xho_config.onlyIP_interface_name[i], G_v2xho_config.onlyIP_interface_IP[i], &G_v2xho_config.onlyIP_interface_prefix[i]);
                    }
                    if(strncmp(G_v2xho_config.onlyIP_interface_IP[i], empty_addr_ipv4, V2XHO_IPV4_ALEN_STR) == 0)
                    {
                        lreturn(V2XHO_MAIN_INITIAL_SETUP_ONLYIP_INTERFACE_NO_IPv4_ADDRESS);
                    }
                    if(1){
                        char cmd_tmp[128] = {0x00,};
                        sprintf(cmd_tmp, "ip route flush %s", G_v2xho_config.onlyIP_interface_name[i]);
                        system(cmd_tmp);
                    }     
                    F_i_V2XHO_IProuter_Address_Set(G_v2xho_config.onlyIP_interface_name[i], G_v2xho_config.onlyIP_interface_IP[i], &G_v2xho_config.onlyIP_interface_prefix[i], 1);
                }else{
                    struct in_addr sub_addr_bin = {.s_addr = ((inet_addr(G_v2xho_config.onlyIP_interface_IP[i]) & 0x00FFFFFF))};
                    char sub_addr_str[V2XHO_IPV4_ALEN_STR];
                    inet_ntop(AF_INET, &sub_addr_bin.s_addr, sub_addr_str, V2XHO_IPV4_ALEN_STR);
                    memcpy(g_argv[route_argc], sub_addr_str, strlen(sub_addr_str));
                    memcpy(g_argv[route_argc] + strlen(sub_addr_str), "/24", strlen("/24")); route_argc++;
                    memcpy(g_argv[route_argc], "via", strlen("via")); route_argc++;
                    memcpy(g_argv[route_argc], G_v2xho_config.onlyIP_interface_name[i], strlen(G_v2xho_config.onlyIP_interface_name[i])); route_argc++;
                    memcpy(g_argv[route_argc], "metric", strlen("metric")); route_argc++;
                    sprintf(g_argv[route_argc], "%d", V2XHO_METRIC_DYNAMIC_ACTIVE_WITH_GW); route_argc++;
                    F_i_V2XHO_Iproute_Route_Modify(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_REPLACE, route_argc, g_argv);
                }
            }
            equip_state.status.u.stats_rsu_ha.active = 1; 
            pthread_create(&th_main_equip_send, NULL, Th_v_V2XHO_RSU_HA_Task_Running_Send, (void*)&equip_state);
            pthread_detach(th_main_equip_send);
            pthread_create(&th_main_equip_recv, NULL, Th_v_V2XHO_RSU_HA_RAW_Receive, (void*)&equip_state);
            pthread_detach(th_main_equip_recv);
            break;
        }
        case DSRC_OBU:
        {
            printf("Equiptype:%d - \033[1;32mOBU\033[0m)!.\n", G_v2xho_config.equipment_type);
            printf("V2X Handover Ethernet Device:%s\n", G_v2xho_config.v2xho_interface_name);
            F_i_V2XHO_IProuter_Route_Flush();
            #if 0
            int route_argc = 0;
            memcpy(g_argv[route_argc], "default", strlen("default")); route_argc++;
            memcpy(g_argv[route_argc], "dev", strlen("dev")); route_argc++;
            memcpy(g_argv[route_argc], G_v2xho_config.v2xho_interface_name, strlen(G_v2xho_config.v2xho_interface_name)); route_argc++;
            memcpy(g_argv[route_argc], "metric", strlen("metric")); route_argc++;
            sprintf(g_argv[route_argc], "%d", V2XHO_METRIC_DEFAULT_ACTIVE); route_argc++;
            F_i_V2XHO_Iproute_Route_Modify(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_REPLACE, route_argc, g_argv);
            #endif
            printf("G_v2xho_config.count_onlyIP_interface:%d\n", G_v2xho_config.count_onlyIP_interface);
            for(int i = 0; i < G_v2xho_config.count_onlyIP_interface; i++)
            {
                printf("Only IP Ethernet Device:%s\n", G_v2xho_config.onlyIP_interface_name[i]);
                printf("Only IP IP Address:%s/%d\n", G_v2xho_config.onlyIP_interface_IP[i], G_v2xho_config.onlyIP_interface_prefix[i]);
                if(strncmp(G_v2xho_config.onlyIP_interface_IP[i], empty_addr_ipv4, V2XHO_IPV4_ALEN_STR) == 0)
                {
                    F_v_V2XHO_IPv4_Address_Get(G_v2xho_config.onlyIP_interface_name[i], G_v2xho_config.onlyIP_interface_IP[i], &G_v2xho_config.onlyIP_interface_prefix[i]);
                }
                if(strncmp(G_v2xho_config.onlyIP_interface_IP[i], empty_addr_ipv4, V2XHO_IPV4_ALEN_STR) == 0)
                {
                    lreturn(V2XHO_MAIN_INITIAL_SETUP_ONLYIP_INTERFACE_NO_IPv4_ADDRESS);
                }
                prefix = 24;
                F_i_V2XHO_IProuter_Address_Set(G_v2xho_config.v2xho_interface_name, G_v2xho_config.v2xho_interface_name, &prefix, 1);

            }
            pthread_create(&th_main_equip_send, NULL, Th_v_V2XHO_OBU_Task_Running_Do, (void*)&equip_state);
            pthread_detach(th_main_equip_send);
            break;
        }
        case LTEV2X_OBU:
        case DSRC_OBU_FA:
        case LTEV2X_OBU_FA:
        {
            printf("Equiptype:%d - OBU(MobileNode)!.\n", G_v2xho_config.equipment_type);
            break;
        }
    }

    while(1)
    {
        sleep(1);
    }

    F_v_V2XHO_Router_Hendler_Close(&rth);
    //라우팅 테이블 복구
    F_i_V2XHO_IProuter_Route_Load_File(NULL);
    if(argc > 1)
    {
        printf("%s\n,",argv[1]);
    }

    return 0;
}

/**
 * @brief 핸드오버 어플리케이션 동작 전 IPv6 라우팅 테이블 저장, 라우터 API 토커 소켓 &rth 오픈
 * 
 * @return int we
 */
static void  f_v_V2XHO_Router_Initial(void)
{
    int ret;

    ret =F_i_V2XHO_IProuter_Route_Save_File(NULL);
    
    if (ret == -1) {
        perror("system");
    } else {
        // 정상적으로 종료되었는지 확인
        if (WIFEXITED(ret)) {
        }
        // 비정상 종료(신호로 인해 종료) 여부 확인
        else if (WIFSIGNALED(ret)) {
            printf("Command was terminated by signal: %d\n", WTERMSIG(ret));
        }
    }

    ret = F_i_V2XHO_Router_Hendler_Open(&rth);
}

/**
 * @brief V2X 기능 초기화
 * 
 * @return int 
 */
static int f_i_V2XHO_V2X_Initial(void)
{
    int ret;
    ret = WAL_Init(kWalLogLevel_Err);
    enum v2xho_error_code_e code = V2XHO_NO_ERROR;
    if (ret < 0) {
        printf("Fail to init v2X - WAL_Init() failed - %d\n", ret);
        code = V2XHO_MAIN_INITIAL_V2X_INTITAL_FUNC_NOT_WORKING;
        return (code * 0x1000000) + (V2XHO_ERROR_CODE_FILE_CODE * 0x010000) + (__LINE__ % 0xFFFF);
    }
    ret = Dot2_Init(kDot2LogLevel_Err, kDot2SigningParamsPrecomputeInterval_Default, "/dev/random", kDot2LeapSeconds_Default);
    if (ret < 0) {
        printf("Fail to init v2X - Dot2_Init() failed - %d\n", ret);
        code = V2XHO_MAIN_INITIAL_V2X_INTITAL_FUNC_NOT_WORKING;
        return (code * 0x1000000) + (V2XHO_ERROR_CODE_FILE_CODE * 0x010000) + (__LINE__ % 0xFFFF);
    }

    ret = Dot3_Init(kDot3LogLevel_Err);
    if (ret < 0) {
        printf("Fail to init v2X - Dot3_Init() failed - %d\n", ret);
        code = V2XHO_MAIN_INITIAL_V2X_INTITAL_FUNC_NOT_WORKING;
        return (code * 0x1000000) + (V2XHO_ERROR_CODE_FILE_CODE * 0x010000) + (__LINE__ % 0xFFFF);
    }
    Dot3_SetWSMMaxLength(2048);
    Dot3_DeleteAllPSRs();
    Dot2_RegisterProcessSPDUCallback(F_v_V2XHO_WAVE_Rx_DOT2_SPDU_Callback);

    WAL_AccessChannel(0, G_v2xho_config.u.dsrc.chan_0, G_v2xho_config.u.dsrc.chan_0);
    WAL_AccessChannel(1, G_v2xho_config.u.dsrc.chan_1, G_v2xho_config.u.dsrc.chan_1);
    char mac_addr_bin[6];
    char mac_addr_str[V2XHO_INTERFACE_MAC_STRING_SIZE] = {0,};
    memcpy(mac_addr_str,  G_v2xho_config.v2xho_interface_MAC_S, V2XHO_INTERFACE_MAC_STRING_SIZE);
    F_v_V2XHO_MAC_ADDR_TRANS_BIN_to_STR(mac_addr_str, mac_addr_bin);
    WAL_SetIfMACAddress(0, (uint8_t*)mac_addr_bin);
    srand(time(NULL));
    mac_addr_bin[5] = (mac_addr_bin[5] + (rand() % 20 + 20)) % 255;
    WAL_SetIfMACAddress(1, (uint8_t*)mac_addr_bin);

    struct Dot2SecProfile profile;
	profile.tx.gen_time_hdr = true;
	profile.tx.gen_location_hdr = true;
	profile.tx.exp_time_hdr = true;
	profile.tx.spdu_lifetime = 30 * 1000 * 1000;
	profile.tx.min_inter_cert_time = 495;
	profile.tx.sign_type = kDot2SecProfileSign_Compressed;
	profile.tx.ecp_format = kDot2SecProfileEcPointFormat_Compressed;
    profile.psid = 135;
    Dot2_AddSecProfile(&profile);

    return code;
}

