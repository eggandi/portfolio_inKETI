#include "v2x_handover_main.h"

#define V2XHO_ERROR_CODE_FILE_CODE V2XHO_ERROR_CODE_FILE_CODE_UTILS

static void f_v_V2XHO_Initial_Setup_Configuration_Value_Input(char type, char *value, size_t value_len, void* out_value);
int G_dashboard_enable = 0;
int G_log_level = 0;

/**
 * @brief 로그메시지 출력, *Dashboard가 실행중에는 로그파일에 저장한다.
 *
 * @param text 
 * @param format 
 * @param ... 
 */
extern void F_v_V2XHO_Print(const char *text, const char *format, ...)
{
    va_list arg;
    if(G_dashboard_enable)
    {

    }else{
        char log[255] = {0,};
        switch(G_log_level)
        {
            case 4:
            case 3:
            case 2: 
            default:
                break;
        }
        sprintf(log, "%s", text);
        va_start(arg, format);
        vsprintf(log, format, arg);
        va_end(arg);
        syslog(LOG_INFO | LOG_LOCAL0, "%s", log);;
    }
}

/**
 * @brief config 파일을 읽어서 V2X 핸드오버 장치의 초기설정 
 * 
 * @param v2xho_config 
 * @return int 0 | error code
 */
extern int F_i_V2XHO_Initial_Setup_Configuration_Read(v2xho_config_t *v2xho_config)
{
    int ret;
    ret = access(V2XHO_INITAIL_SETUP_CONFIGURAION_FILE_PATH, F_OK);
    if(ret < 0)
    {
        char sys_cmd[128] = {0,}; 
        sprintf(sys_cmd, "mkdir -m 777 -p %s", V2XHO_INITAIL_SETUP_CONFIGURAION_FILE_PATH);
        system(sys_cmd);
    }
    char *config_file_name = V2XHO_INITAIL_SETUP_CONFIGURAION_FILE_NAME;
    char *config_file = (char*)malloc(strlen(V2XHO_INITAIL_SETUP_CONFIGURAION_FILE_PATH "/") + strlen(config_file_name));
    sprintf(config_file, "%s/%s", V2XHO_INITAIL_SETUP_CONFIGURAION_FILE_PATH, config_file_name);
    ret = access(config_file, F_OK);
    if(ret < 0)
    {
        FILE *config_fp = fopen(config_file, "w+");
        fputs("Configuration_Enable=0x00;\n", config_fp);
        fputs("Equipment_Type=;//10:DSRC_RSU(HA),11:DSRC_RSU(FA),13:DSRC_OBU\n", config_fp);

        fputs("V2XHO_Interface_Name=\"\";\n", config_fp);
        fputs("V2XHO_Interface_MAC_S=\":::::\";\n", config_fp);
        fputs("V2XHO_Interface_IP=\"\";\n", config_fp);
        fputs("V2XHO_Interface_Prefix=00;\n", config_fp);

        fputs("Count_OnlyIP_Interface=00;\n", config_fp);
        fputs("OnlyIP_Interface_Name_0=\"\";\n", config_fp);
        fputs("OnlyIP_Interface_IP_0=\"\";\n", config_fp);
        fputs("OnlyIP_Interface_Prefix_0=;\n", config_fp);
        fputs("OnlyIP_Interface_Name_1=\"\";\n", config_fp);
        fputs("OnlyIP_Interface_IP_1=\"\";\n", config_fp);
        fputs("OnlyIP_Interface_Prefix_1=;\n", config_fp);
        fputs("OnlyIP_Interface_Name_2=\"\";\n", config_fp);
        fputs("OnlyIP_Interface_IP_2=\"\";\n", config_fp);
        fputs("OnlyIP_Interface_Prefix_2=;\n", config_fp);
        
        fputs("V2XHO_Tunneling_Enable=0x00;//00:Disable,01:IPinIP,02:GRE,03:Minimal\n", config_fp);

        fputs("V2XHO_Security_Enable=0x00;\n", config_fp);
        
        fputs("V2XHO_DSRC_CHAN_0=182;\n", config_fp);
        fputs("V2XHO_DSRC_CHAN_1=184;\n", config_fp);
        fputs("V2XHO_DSRC_DATARATE_0=12;\n", config_fp);
        fputs("V2XHO_DSRC_DATARATE_1=12;\n", config_fp);
        fputs("V2XHO_DSRC_TX_POWER_0=20;\n", config_fp);
        fputs("V2XHO_DSRC_TX_POWER_1=20;\n", config_fp);

        fputs("V2XHO_LTEV2X_CHAN=183;\n", config_fp);
        fputs("V2XHO_LTEV2X_DATARATE=12;\n", config_fp);
        fputs("V2XHO_LTEV2X_TX_POWER=20;\n", config_fp);

        fputs("Debug_Dashboard_Enable=0x00;\n", config_fp);
        fputs("Debug_Dashboard_Repeat_Time=\"\";\n", config_fp);
        fputs("Debug_PingTest_Enable=0x00;\n", config_fp);
        fputs("Debug_PingTest_Logging_Enable=0x00;//Only for OBU.\n", config_fp);
        fputs("Debug_PingTest_PCAP_Enable=0x00;//Only for OBU.\n", config_fp);
        fputs("Debug_PingTest_Destination_IP=\"\";\n", config_fp);
        fputs("Debug_PingTest_Interval_ms=100;\n", config_fp);
        fputs("Debug_PingTest_Packet_Size=1024;\n", config_fp);


        fputs("*If the IP address field is not filled, the app will find the IP address that matches the interface name..\n", config_fp);

        fclose(config_fp);
        lSafeFree(config_file);
        printf("Fill the config_file in ./config/v2xho_config.conf\n");
        lreturn(V2XHO_MAIN_INITIAL_SETUP_CONFIGURATION_NO_CONFIG_FILE);
    }else{
        FILE *config_fp = fopen(config_file, "r");
        char str[256] = {0,};
        char *rp;
        while(1)
        {
            memset(str, 0x00, 256);
            rp = fgets(str, 256, config_fp);
            if(!rp)
            {
                break;
            }
            char type;
            char *ptr_name = strtok(str, "=");
            if(ptr_name)
            {
                char *ptr_value = strtok(NULL, ";");
                if(ptr_value && strlen(ptr_value) > 1)
                {
                    if(ptr_value[0] == '"')
                    {
                        type = ptr_value[0];
                    }else{
                        type = ptr_value[1];
                    }
                    if(strcmp(ptr_name, "Configuration_Enable") == 0)
                    {
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->config_enable);
                        if(v2xho_config->config_enable == 0x00)
                        {
                            printf("Configuration not enable.\n");
                            lreturn(V2XHO_MAIN_INITIAL_SETUP_CONFIGURATION_NO_ENABLE);
                        }
                    }
                    if(strcmp(ptr_name, "Equipment_Type") == 0){
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->equipment_type);
                    }else if(strcmp(ptr_name, "V2XHO_Interface_Name") == 0){
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)v2xho_config->v2xho_interface_name);
                    }else if(strcmp(ptr_name, "V2XHO_Interface_MAC_S") == 0){
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)v2xho_config->v2xho_interface_MAC_S);
                    }else if(strcmp(ptr_name, "V2XHO_Interface_IP") == 0){
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)v2xho_config->v2xho_interface_IP);
                    }else if(strcmp(ptr_name, "V2XHO_Interface_Prefix") == 0){
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->v2xho_interface_prefix);
                    }else if(strcmp(ptr_name, "Count_OnlyIP_Interface") == 0){
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->count_onlyIP_interface);
                    }else if(strcmp(ptr_name, "V2XHO_Tunneling_Enable") == 0){
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->tunneling_enable);
                    }else if(strcmp(ptr_name, "V2XHO_Security_Enable") == 0){
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->security_enable);
                    }else if(strcmp(ptr_name, "Debug_Dashboard_Enable") == 0){
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->debug_dashboard_enable);
                    }else if(strcmp(ptr_name, "Debug_PingTest_Enable") == 0){
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->debug_pingtest_enable);
                    }else if(strcmp(ptr_name, "Debug_PingTest_Logging_Enable") == 0){
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->debug_pingtest_logging_enable);
                    }else if(strcmp(ptr_name, "Debug_PingTest_PCAP_Enable") == 0){
                        f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->debug_pingtest_pcap_enable);
                    }
                    if(v2xho_config->debug_dashboard_enable == 1)
                    {
                        if(strcmp(ptr_name, "Debug_Dashboard_Repeat_Time") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->debug_dashboard_repeat_time);
                        }
                    }
                    if(v2xho_config->debug_pingtest_enable == 1)
                    {
                        if(strcmp(ptr_name, "Debug_PingTest_Destination_IP") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->ping_info.dest_addr);
                        }else if(strcmp(ptr_name, "Debug_PingTest_Packet_Size") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->ping_info.packet_size);
                        }else if(strcmp(ptr_name, "Debug_PingTest_Interval_ms") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->ping_info.interval_ms);
                        }
                    }
                    if(v2xho_config->count_onlyIP_interface > 0)
                    {
                        if(strcmp(ptr_name, "OnlyIP_Interface_Name_0") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)v2xho_config->onlyIP_interface_name[0]);
                        }else if(strcmp(ptr_name, "OnlyIP_Interface_IP_0") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)v2xho_config->onlyIP_interface_IP[0]);
                        }else if(strcmp(ptr_name, "OnlyIP_Interface_Prefix_0") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->onlyIP_interface_prefix[0]);
                        }else if(strcmp(ptr_name, "OnlyIP_Interface_Name_1") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)v2xho_config->onlyIP_interface_name[1]);
                        }else if(strcmp(ptr_name, "OnlyIP_Interface_IP_1") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)v2xho_config->onlyIP_interface_IP[1]);
                        }else if(strcmp(ptr_name, "OnlyIP_Interface_Prefix_1") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->onlyIP_interface_prefix[0]);
                        }else if(strcmp(ptr_name, "OnlyIP_Interface_Name_2") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)v2xho_config->onlyIP_interface_name[2]);
                        }else if(strcmp(ptr_name, "OnlyIP_Interface_IP_2") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)v2xho_config->onlyIP_interface_IP[2]);
                        }else if(strcmp(ptr_name, "OnlyIP_Interface_Prefix_2") == 0){
                            f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&v2xho_config->onlyIP_interface_prefix[0]);
                        }
                    }

                    switch(v2xho_config->equipment_type)
                    {
                        default:
                        {
                            break;
                        }
                        case 14:
                        case 13:
                        case 12:
                        case 11:
                        case 10:
                        {
                            if(strcmp(ptr_name, "V2XHO_DSRC_CHAN_0") == 0){
                                uint32_t u32_num;
                                f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&u32_num);
                                v2xho_config->u.dsrc.chan_0 = u32_num;
                            }else if(strcmp(ptr_name, "V2XHO_DSRC_CHAN_1") == 0){
                                uint32_t u32_num;
                                f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&u32_num);
                                v2xho_config->u.dsrc.chan_1 = u32_num;
                            }else if(strcmp(ptr_name, "V2XHO_DSRC_DATARATE_0") == 0){
                                uint32_t u32_num;
                                f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&u32_num);
                                v2xho_config->u.dsrc.datarate_0 = u32_num;
                            }else if(strcmp(ptr_name, "V2XHO_DSRC_DATARATE_1") == 0){
                                uint32_t u32_num;
                                f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&u32_num);
                                v2xho_config->u.dsrc.datarate_1 = u32_num;
                            }else if(strcmp(ptr_name, "V2XHO_DSRC_TX_POWER_0") == 0){
                                uint32_t u32_num;
                                f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&u32_num);
                                v2xho_config->u.dsrc.tx_power_0 = u32_num;
                            }else if(strcmp(ptr_name, "V2XHO_DSRC_TX_POWER_1") == 0){
                                uint32_t u32_num;
                                f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&u32_num);
                                v2xho_config->u.dsrc.tx_power_1 = u32_num;
                            }
                            break;
                        }
                        case 24:
                        case 23:
                        case 22:
                        case 21:
                        case 20:
                        {
                            if(strcmp(ptr_name, "V2XHO_LTEV2X_CHAN") == 0){
                                uint32_t u32_num;
                                f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&u32_num);
                                v2xho_config->u.ltev2x.chan = u32_num;
                            }else if(strcmp(ptr_name, "V2XHO_LTEV2X_DATARATE") == 0){
                                uint32_t u32_num;
                                f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&u32_num);
                                v2xho_config->u.ltev2x.datarate = u32_num;
                            }else if(strcmp(ptr_name, "V2XHO_LTEV2X_TX_POWER") == 0){
                                uint32_t u32_num;
                                f_v_V2XHO_Initial_Setup_Configuration_Value_Input(type, ptr_value, strlen(ptr_value), (void*)&u32_num);
                                v2xho_config->u.ltev2x.tx_power = u32_num;
                            }
                            break;
                        }
                    }


                    
                }
            }
         
        }
    }
    return 0;
}

/**
 * @brief Config 파일을 읽어서 out_value에 값 입력
 * 
 * @param type value 종류:string|ascii|bitstring|hexstring|
 * @param value 문자열 입력값
 * @param value_len 입력값 길이
 * @param out_value 출력값
 * @return void 
 */
static void f_v_V2XHO_Initial_Setup_Configuration_Value_Input(char type, char *value, size_t value_len, void* out_value)
{
    switch(type)
    {
        case 'b':
        {
            int decimal = 0; 
            for(size_t i = 0; i < value_len - 2; i++)
            {
                char bin_char = value[i + 2];
                if(bin_char == '1')
                {
                    if(i == value_len - 2 - 1)
                    {
                        decimal += 1;
                    }else{
                        int square_two = 1;
                        for(size_t j = 0; j < (value_len - 2 - i - 1); j++)
                        {
                            square_two = square_two * 2;
                        }
                        decimal += square_two;
                    }
                }
                memcpy(out_value, &decimal, sizeof(int));
            }
            break;
        }
        case 'x':
        {
            int decimal = 0; 
            for(size_t i = 0; i < 2; i++)
            {
                char hex_char = value[i + 2];
                int square_two = 1;
                for(size_t j = 0; j < 4 * (1 - i); j++)
                {
                    square_two = square_two * 2;
                }
                if (hex_char >= 48 && hex_char <= 57) 
                {
                    decimal += (hex_char - 48) * square_two;
                } else if (hex_char >= 65 && hex_char <= 70)
                {
                    decimal += (hex_char - (65 - 10)) * square_two;
                } else if (hex_char >= 97 && hex_char <= 102)   
                {
                    decimal += (hex_char - (97 - 10)) * square_two;
                }
            }
            memcpy(out_value, &decimal, sizeof(int));
            break;
        }
        case '"':
        {
            memcpy(out_value, value + 1, value_len - 2);
            break;
        }
        default:
        {
            int v = atoi(value);
            memcpy(out_value, &v, sizeof(int));
            break;
        }
    }

}
/**
 * @brief 
 * 
 * @param iface 
 * @param ipv4_addr 
 * @param prefix 
 */
extern void F_v_V2XHO_IPv4_Address_Get(const char *iface, char *ipv4_addr, int *prefix) 
{
    struct ifaddrs *ifaddr, *ifa;

    // 시스템에 등록된 모든 네트워크 인터페이스 정보를 가져옴
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    printf("iface:%s\n", iface);
    // 모든 인터페이스를 순회하면서 IPv4 주소 검색
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        // 인터페이스가 지정한 이름과 일치하고 AF_INET (IPv4) 주소인 경우
        if (strcmp(ifa->ifa_name, iface) == 0 && ifa->ifa_addr->sa_family == AF_INET)
         {
            struct sockaddr_in *sockaddr = (struct sockaddr_in *)ifa->ifa_addr;

            // IPv4 주소를 문자열로 변환하여 출력
            if (inet_ntop(AF_INET, &sockaddr->sin_addr, ipv4_addr, INET_ADDRSTRLEN) != NULL) {
                
            } else {
                perror("inet_ntop");
            }
            int prefix_length = 0;
            unsigned char *ptr = (unsigned char *)&sockaddr->sin_addr;
            for (int i = 0; i < INET_ADDRSTRLEN; i++) 
            {
                unsigned char byte = ptr[i];
                while (byte) {
                    prefix_length += byte & 1;
                    byte >>= 1;
                }
            }
            *prefix = prefix_length;
        }
    }

    freeifaddrs(ifaddr);
}

extern void F_v_V2XHO_IPv6_Address_Get(const char *iface, char *ipv6_addr, int *prefix) 
{
    struct ifaddrs *ifaddr, *ifa;

    // 시스템에 등록된 모든 네트워크 인터페이스 정보를 가져옴
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    // 모든 인터페이스를 순회하면서 IPv6 주소 검색
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        // 인터페이스가 지정한 이름과 일치하고 AF_INET6 (IPv6) 주소인 경우
        if (strcmp(ifa->ifa_name, iface) == 0 && ifa->ifa_addr->sa_family == AF_INET6)
         {
            struct sockaddr_in6 *sockaddr = (struct sockaddr_in6 *)ifa->ifa_addr;

            // IPv6 주소를 문자열로 변환하여 출력
            if (inet_ntop(AF_INET6, &sockaddr->sin6_addr, ipv6_addr, INET6_ADDRSTRLEN) != NULL) {
                
            } else {
                perror("inet_ntop");
            }
            int prefix_length = 0;
            unsigned char *ptr = (unsigned char *)&sockaddr->sin6_addr;
            for (int i = 0; i < INET6_ADDRSTRLEN; i++) 
            {
                unsigned char byte = ptr[i];
                while (byte) {
                    prefix_length += byte & 1;
                    byte >>= 1;
                }
            }
            *prefix = prefix_length;
        }
    }

    freeifaddrs(ifaddr);
}

extern int F_i_V2XHO_Utils_Rmdirs(const char *path, int force)
{
    DIR *  dir_ptr      = NULL;
    struct dirent *file = NULL;
    struct stat   buf;
    char   filename[1024];

    if((dir_ptr = opendir(path)) == NULL) 
    {
		return unlink(path);
    }

    while((file = readdir(dir_ptr)) != NULL) 
    {
        if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) 
        {
             continue;
        }

        sprintf(filename, "%s/%s", path, file->d_name);
        if(lstat(filename, &buf) == -1)
        {
            continue;
        }

        if(S_ISDIR(buf.st_mode)) 
        { 
            if(F_i_V2XHO_Utils_Rmdirs(filename, force) == -1 && !force) 
            {
                lreturn(V2XHO_UTIL_UDS_RMDIR_ERROR_ISDIR);
            }
        } else if(S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) 
        { 
            if(unlink(filename) == -1 && !force) 
            {
                lreturn(V2XHO_UTIL_UDS_RMDIR_ERROR_ISREG);
            }
        }
    }
    closedir(dir_ptr);
    
    return rmdir(path);
}


extern uint16_t F_u16_V2XHO_IP_Checksum(uint16_t *buffer, int nwords) {
    unsigned long sum = 0;

    // 모든 16비트 단어를 합산
    for (int i = 0; i < nwords; i++) {
        sum += ntohs(buffer[i]); // 네트워크 바이트 순서로 변환하여 합산
        if (sum & 0xFFFF0000) { // 16비트를 초과하면
            sum = (sum & 0xFFFF) + (sum >> 16); // 오버플로우 처리
        }
    }

    // 1의 보수 계산
    sum = ~sum & 0xFFFF;

    return htons((uint16_t)sum); // 결과를 네트워크 바이트 순서로 반환
}

extern uint16_t F_u16_V2XHO_UDP_Checksum(unsigned short *buf, int nwords)
{
    unsigned long sum = 0;
	int tot_len = nwords;
    for(int i = 0; i < tot_len; i++)
    {
        sum += (*buf++);
    }
	if (tot_len % 2 == 1 )
	{
		sum += *(unsigned char*)buf++ & 0x00ff;
	}
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

extern void F_v_V2XHO_MAC_ADDR_TRANS_BIN_to_STR(char *src, char *dst)
{
    memset(dst, strtol(strtok(src, ":"), NULL, 16) & 0xFF, 1); 
    for(int i = 1; i < 6; i++)
    {
        
        memset(dst + i, strtol(strtok(NULL, ":"), NULL, 16) & 0xFF, 1);
    }
    return;
}

extern void F_v_V2XHO_Utils_Eth_Device_Flush(char *dev)
{
    char cmd[128] = {0,};
    sprintf(cmd, "ip address flush dev %s", dev);
    system(cmd);memset(cmd, 0x00, 128);
    sprintf(cmd, "ip -6 address flush dev %s", dev);
    system(cmd);memset(cmd, 0x00, 128);
    sprintf(cmd, "ip route flush dev %s", dev);
    system(cmd);memset(cmd, 0x00, 128);
    sprintf(cmd, "ip -6 route flush dev %s", dev);
    system(cmd);memset(cmd, 0x00, 128);
}

extern void F_v_V2XHO_Utils_Print_Binary(unsigned int num) {

    // sizeof(unsigned int) * 8은 비트 수 (32비트 또는 64비트)
    for (int i = sizeof(unsigned int) * 8 - 1; i >= 0; i--) {
        printf("%c", (num & (1U << i)) ? '1' : '0'); // 각 비트를 확인
    }
    printf("\n");
}

extern double F_d_V2XHO_Utils_Clockms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // CLOCK_MONOTONIC도 사용 가능
    double sec = ts.tv_sec;
    double msec = (ceil((ts.tv_nsec) * 1e6) / 1e6) / 1e9;
    return sec + msec;
}