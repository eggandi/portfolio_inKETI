#include "relay_config.h"

struct relay_inno_config_t G_relay_inno_config;

static char *g_config_path = NULL;

static void RELAY_INNO_Config_Setup_Configuration_Value_Input(char type, char *value, size_t value_len, void* out_value);
static int RELAY_INNO_Config_ParsedOption(int option);

/**
 * @brief config 파일을 읽어서 V2X 핸드오버 장치의 초기설정 
 * 
 * @param  
 * @return int 0 | error code
 */
extern int RELAY_INNO_Config_Setup_Configuration_Read(struct relay_inno_config_t *relay_inno_config)
{
    int ret;
    // config 경로가 존재하는지 확인
    char *config_path;
    if(g_config_path == NULL)    
    {
        config_path = RELAY_INNO_INITAIL_SETUP_CONFIGURAION_FILE_PATH;
    }else{
        config_path = g_config_path;
    }
    ret = access(config_path, F_OK);
    if(ret < 0)
    {
        char sys_cmd[256] = {0}; 
        // mkdir 명령어에 대한 결과값 체크
        snprintf(sys_cmd, sizeof(sys_cmd), "mkdir -m 777 -p %s", config_path);
        ret = system(sys_cmd);
        if(ret != 0)
        {
            printf("Failed to create config directory\n");
            return -1;
        }
    }
    
    // config 파일 경로 생성 (경로 + "/" + 파일명 + 널 종료 문자)
    char *config_file_name = RELAY_INNO_INITAIL_SETUP_CONFIGURAION_FILE_NAME;
    size_t path_len = strlen(config_path);
    size_t name_len = strlen(config_file_name);
    size_t total_len = path_len + 1 + name_len + 1;
    char *config_file = (char*)malloc(total_len);
    if(!config_file)
    {
        printf("Memory allocation failed\n");
        return -1;
    }
    snprintf(config_file, total_len, "%s/%s", config_path, config_file_name);
    
    ret = access(config_file, F_OK);
    if(ret < 0)
    {
        // config 파일이 없으면 새로 생성
        FILE *config_fp = fopen(config_file, "w+");
        if(!config_fp)
        {
            printf("Failed to open config file for writing\n");
            free(config_file);
						config_file = NULL;
            return -1;
        }

        fputs("Configuration_Enable=0x01;\n", config_fp);
				fputs("Configuration_Path=\"./\";\n", config_fp);
				fputs("Configuration_File_Name=\"kRelay.conf\";\n", config_fp);
				fputs("Configuration_Log_Level=0;\t\t\t(0:No, 1:Error, 2:Event, 3:All)(TBA)\n", config_fp);
        fputs("\n", config_fp);

				fputs("----------------------------------------Relay-----------------------------------------;\n", config_fp);
				fputs("----------------------------------------Ethernet\n", config_fp);
				fputs("Relay_Eth_Interface_Name=\"eth0\";\n", config_fp);
				fputs("Relay_Destination_IP_Address=\"192.168.1.100\"\n", config_fp);
				fputs("Relay_Port_V2X_Rx_Data=27353;\n", config_fp);
				fputs("Relay_Port_V2X_Tx_Data=27354;\n", config_fp);
				fputs("Relay_V2X_Data_Type=3;\t\t\t\t(0:MPDU, 1:WSM, 2:SPDU, 3:MessageFrame(J2735, WSA)\n", config_fp);
				fputs("----------------------------------------GNSS\n", config_fp);
				fputs("GNSS_Enable=1\n", config_fp);
				fputs("GNSS_Interval=100;\n", config_fp);
				fputs("\n", config_fp);

        fputs("----------------------------------------V2X-----------------------------------------;\n", config_fp);
        fputs("V2X_Operation_Type=1;\t\t\t\t(0:rx only, 1:trx)\n", config_fp);
        fputs("V2X_Device_Name=\"/dev/spidev1.0\";\n", config_fp);
        fputs("V2X_Dbg_Msg_Level=0;\t\t\t\t(0:nothing, 1:err, 2:init, 3:event, 4:message hexdump)\n", config_fp);
        fputs("V2X_Lib_Dbg_Msg_Level=0;\t\t\t(0:nothing, 1:err, 2:init, 3:event, 4:message hexdump)\n", config_fp);
				fputs("----------------------------------------V2X-Vechicle(TBA)", config_fp);
				fputs("----------------------------------------V2X-Tx\n", config_fp);
        fputs("V2X_Tx_Type=0;\t\t\t\t(0:SPS, 1:Event)\n", config_fp);
				fputs("V2X_Tx_SPS_Interval=100;\n", config_fp);
        fputs("V2X_Tx_Channel_Num=183;\n", config_fp);
        fputs("V2X_Tx_DataRate=12;\n", config_fp);
        fputs("V2X_Tx_Power=20; \n", config_fp);
        fputs("V2X_Tx_Priority=7;\n", config_fp);
				fputs("----------------------------------------V2X-Tx-WSA(TBA)\n", config_fp);
				
				fputs("----------------------------------------V2X-Tx-J2735-BSM\n", config_fp);
				fputs("V2X_Tx_J2735_BSM_Enable=1;\n", config_fp);
				fputs("V2X_Tx_J2735_BSM_J29451_Enable=1;\n", config_fp);
				fputs("V2X_Tx_J2735_BSM_Forced=1;\t\t\t(0:3D-FIX Only, 1:All)\n", config_fp);
				fputs("V2X_Tx_J2735_BSM_PSID=32;\n", config_fp);
				fputs("V2X_Tx_J2735_BSM_Temporary_ID=0x01020304;\n", config_fp);
				fputs("V2X_Tx_J2735_BSM_Tx_Type=0;\t\t\t(0:Default, 0:Ad-Hoc, 1: SPS)\n", config_fp);
				fputs("V2X_Tx_J2735_BSM_Interval=100;\n", config_fp);
				fputs("V2X_Tx_J2735_BSM_Priority=7;\n", config_fp);
				//fputs("V2X_Tx_J2735_BSM_Tx_Power=;\n", config_fp);
				//fputs("----------------------------------------V2X-Tx-J2735-WSA(TBA)\n", config_fp);
				//fputs("----------------------------------------V2X-Tx-J2735-PVD(TBA)\n", config_fp);

				fputs("----------------------------------------V2X-Rx\n", config_fp);
				fputs("V2X_Rx_Enable=0;\n", config_fp);
				fputs("V2X_Rx_Dot2_Forced_Enable=0;\n", config_fp);
				//fputs("V2X_Rx_WSM=0;\n\t\t\t\t(0:Standard WSM, 1:Non-Standard Print Hexdump)", config_fp);
				fputs("V2X_Rx_WSA_Enable=1;\n", config_fp);
				fputs("----------------------------------------V2X-Rx-J2735-MSGs\n", config_fp);
        fputs("V2X_Rx_J2735_BSM=1;\n", config_fp);
				fputs("V2X_Rx_J2735_SPAT=1;\n", config_fp);
				fputs("V2X_Rx_J2735_MAP=1;\n", config_fp);
				fputs("V2X_Rx_J2735_RTCM=1;\n", config_fp);
				fputs("V2X_Rx_J2735_TIM=1;\n", config_fp);
				fputs("V2X_Rx_J2735_RSA=1;\n", config_fp);
				//fputs("V2X_Rx_J2735_PVD=0;\n", config_fp);

				fputs("----------------------------------------V2X-Dot2\n", config_fp);
        fputs("V2X_Dot2_Enable=0;\n", config_fp);
        fputs("V2X_Dot2_Certificates_Path=\"./certificates\";\n", config_fp);
				fputs("V2X_Dot2_Trustedcerts_File_Path=\"trustedcerts\";\n", config_fp);
				fputs("V2X_Dot2_CMHF_OBU_Enable=1;\n", config_fp);
				fputs("V2X_Dot2_CMHF_OBU_File_Path=\"obu\";\n", config_fp);
				//fputs("V2X_Dot2_CMHF_RSU_Enable=1;\n", config_fp);
				//fputs("V2X_Dot2_CMHF_RSU_File_Path=\"rsu\";\n", config_fp);
        fputs("\n", config_fp);

        fclose(config_fp);
        free(config_file);
				config_file = NULL;
        printf("Fill the config_file in %s/relay_inno_config.conf\n", g_config_path);
        return 0;
    }
    else
    {
        FILE *config_fp = fopen(config_file, "rw");
        if(!config_fp)
        {
            printf("Failed to open config file for reading\n");
            free(config_file);
						config_file = NULL;
            return -1;
        }
        char str[256] = {0};
        while(fgets(str, sizeof(str), config_fp))
        {
            // 개행 문자 제거
            str[strcspn(str, "\n")] = 0;
            char *ptr_name = strtok(str, "=");
            if(ptr_name)
            {
                char *ptr_value = strtok(NULL, ";");
                if(ptr_value && strlen(ptr_value) >= 1)
                {
                    // 값의 형식에 따라 타입 결정
                    char type;
                    size_t value_len = strlen(ptr_value);
                    if(ptr_value[0] == '"')
                    {
                        type = '"';
                    }
                    else if(value_len >= 2 && ptr_value[0]=='0' && (ptr_value[1]=='x' || ptr_value[1]=='X'))
                    {
                        type = 'x';
                    }
                    else if(value_len >= 2 && ptr_value[0]=='0' && (ptr_value[1]=='b' || ptr_value[1]=='B'))
                    {
                        type = 'b';
                    }
                    else
                    {
                        type = 'd'; // default: 정수형으로 처리
                    }
                    
                    if(strcmp(ptr_name, "Configuration_Enable") == 0)
                    {
                        RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->config_enable);
                        if(relay_inno_config->config_enable == 0x00)
                        {
                            printf("Configuration not enable.\n");
                            fclose(config_fp);
                            free(config_file);
														config_file = NULL;
                            return -1 * __LINE__;
                        }
                    }else if(strcmp(ptr_name, "Relay_Eth_Interface_Name") == 0)
                    {
                        RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)relay_inno_config->relay.dev_name);
                    }else if(strcmp(ptr_name, "Relay_Destination_IP_Address") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)relay_inno_config->relay.gatewayip);
										}else if(strcmp(ptr_name, "Relay_V2X_Data_Type") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->relay.relay_data_type);
										}else if(strcmp(ptr_name, "Relay_Port_V2X_Rx_Data") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->relay.port_v2x_rx);
										}else if(strcmp(ptr_name, "Relay_Port_V2X_Tx_Data") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->relay.port_v2x_tx);
										}else if(strcmp(ptr_name, "GNSS_Enable") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->relay.gnss_enable);
										}else if(strcmp(ptr_name, "GNSS_Interval") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->relay.gnss_interval);
										}else if(strcmp(ptr_name, "V2X_Operation_Type") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.op);
										}else if(strcmp(ptr_name, "V2X_Device_Name") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)relay_inno_config->v2x.dev_name);
										}else if(strcmp(ptr_name, "V2X_Dbg_Msg_Level") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.dbg);
										}else if(strcmp(ptr_name, "V2X_Lib_Dbg_Msg_Level") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.lib_dbg);
										}else if(strcmp(ptr_name, "V2X_Tx_Type") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.tx_type);
										}else if(strcmp(ptr_name, "V2X_Tx_Channel_Num") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.chan_num);
										}else if(strcmp(ptr_name, "V2X_Tx_DataRate") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.tx_datarate);
										}else if(strcmp(ptr_name, "V2X_Tx_Power") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.tx_power);
										}else if(strcmp(ptr_name, "V2X_Tx_Priority") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.tx_priority);
										}else if(strcmp(ptr_name, "V2X_Tx_Interval") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.tx_interval);
										}else if(strcmp(ptr_name, "V2X_Tx_J2735_BSM_Enable") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.j2735.bsm.enable);
										}else if(strcmp(ptr_name, "V2X_Tx_J2735_BSM_J29451_Enable") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.j2735.bsm.j29451_enable);
										}else if(strcmp(ptr_name, "V2X_Tx_J2735_BSM_Forced") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.j2735.bsm.tx_forced);
										}else if(strcmp(ptr_name, "V2X_Tx_J2735_BSM_PSID") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.j2735.bsm.psid);
										}else if(strcmp(ptr_name, "V2X_Tx_J2735_BSM_Temporary_ID") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)relay_inno_config->v2x.j2735.bsm.temporary_id);
										}else if(strcmp(ptr_name, "V2X_Tx_J2735_BSM_Tx_Type") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.j2735.bsm.tx_type);	
										}else if(strcmp(ptr_name, "V2X_Tx_J2735_BSM_Interval") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.j2735.bsm.interval);
										}else if(strcmp(ptr_name, "V2X_Tx_J2735_BSM_Priority") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.j2735.bsm.priority);
										}else if(strcmp(ptr_name, "V2X_Dot2_Enable") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.dot2.enable);
										}else if(strcmp(ptr_name, "V2X_Dot2_Certificates_Path") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)relay_inno_config->v2x.dot2.cert_path);
										}else if(strcmp(ptr_name, "V2X_Dot2_Trustedcerts_File_Path") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)relay_inno_config->v2x.dot2.trustedcerts_path);
										}else if(strcmp(ptr_name, "V2X_Dot2_CMHF_OBU_Enable") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.dot2.cmhf_obu_enable);
										}else if(strcmp(ptr_name, "V2X_Dot2_CMHF_OBU_File_Path") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)relay_inno_config->v2x.dot2.cmhf_obu_path);
										}else if(strcmp(ptr_name, "V2X_Dot2_CMHF_RSU_Enable") == 0)	
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.dot2.cmhf_rsu_enable);
										}else if(strcmp(ptr_name, "V2X_Dot2_CMHF_RSU_File_Path") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)relay_inno_config->v2x.dot2.cmhf_rsu_path);
										}else if(strcmp(ptr_name, "V2X_Rx_Enable") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.rx.enable);
										}else if(strcmp(ptr_name, "V2X_Rx_WSA_Enable") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.rx.wsa_enable);
										}else if(strcmp(ptr_name, "V2X_Rx_Dot2_Forced_Enable") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.rx.dot2_forced_enable);
										}else if(strcmp(ptr_name, "V2X_Rx_J2735_BSM") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.rx.j2735.BSM_enable);
										}else if(strcmp(ptr_name, "V2X_Rx_J2735_SPAT") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.rx.j2735.SPAT_enable);
										}else if(strcmp(ptr_name, "V2X_Rx_J2735_MAP") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.rx.j2735.MAP_enable);
										}else if(strcmp(ptr_name, "V2X_Rx_J2735_RTCM") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.rx.j2735.RTCM_enable);
										}else if(strcmp(ptr_name, "V2X_Rx_J2735_TIM") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.rx.j2735.TIM_enable);
										}else if(strcmp(ptr_name, "V2X_Rx_J2735_RSA") == 0)
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.rx.j2735.RSA_enable);
										}
										else if(strcmp(ptr_name, "V2X_Rx_J2735_PVD") == 0)	
										{
												RELAY_INNO_Config_Setup_Configuration_Value_Input(type, ptr_value, value_len, (void*)&relay_inno_config->v2x.rx.j2735.PVD_enable);
										}
                }
            }
        }
        fclose(config_fp);
    }
    if(g_config_path != NULL)
		{
      free(g_config_path);
			g_config_path = NULL;
		}

    free(config_file);
		config_file = NULL;
    return 0;
}


/**
 * @brief Config 파일을 읽어서 out_value에 값 입력
 * 
 * @param type value 종류: 문자열("), 16진수(x), 2진수(b), 또는 default(정수, 'd')
 * @param value 입력 문자열
 * @param value_len 문자열 길이
 * @param out_value 출력값 (메모리 공간에 결과 복사)
 * @return void 
 */
static void RELAY_INNO_Config_Setup_Configuration_Value_Input(char type, char *value, size_t value_len, void* out_value)
{
    switch(type)
    {
        case 'b':   // 2진수 처리 ("0b" 또는 "0B"로 시작)
        {
            int decimal = 0; 
            for(size_t i = 2; i < value_len; i++)
            {
                char bin_char = value[i];
                decimal = decimal * 2 + (bin_char == '1' ? 1 : 0);
            }
            memcpy(out_value, &decimal, sizeof(int));
            break;
        }
        case 'x':   // 16진수 처리 ("0x" 또는 "0X"로 시작)
        {
            int decimal = 0; 
            for(size_t i = 2; i < value_len; i++)
            {
                char hex_char = value[i];
                int digit = 0;
                if (hex_char >= '0' && hex_char <= '9')
                { 
                    digit = hex_char - '0';
                } else if (hex_char >= 'A' && hex_char <= 'F')
                {
                    digit = hex_char - 'A' + 10;
                }else if (hex_char >= 'a' && hex_char <= 'f')  
                {
                    digit = hex_char - 'a' + 10;
                }
                decimal = decimal * 16 + digit;
            }
            memcpy(out_value, &decimal, sizeof(int));
            break;
        }
        case '"':   // 문자열 처리: 따옴표 제거 후 복사
        {
            size_t copy_len = value_len;
            if(value[0] == '"' && value[value_len - 1] == '"')
            {
                copy_len = value_len - 2;
                memcpy(out_value, value + 1, copy_len);
                ((char*)out_value)[copy_len] = '\0';
            }
            else
            {
                memcpy(out_value, value, copy_len);
                ((char*)out_value)[copy_len] = '\0';
            }
            break;
        }
        default:    // 기본: 정수값으로 처리 (한 자리 숫자 포함)
        {
            int v = atoi(value);
            memcpy(out_value, &v, sizeof(int));
            break;
        }
    }
}


/**
 * @brief 어플리케이션 실행 시 함께 입력된 파라미터들을 파싱하여 관리정보에 저장한다.
 * @param[in] argc 유틸리티 실행 시 입력되는 명령줄 내 파라미터들의 개수 (유틸리티 실행파일명 포함)
 * @param[in] argv 유틸리티 실행 시 입력되는 명령줄 내 파라미터들의 문자열 집합 (유틸리티 실행파일명 포함)
 * @retval 0: 성공
 * @retval -1: 실패
 */
extern int RELAY_INNO_Config_Pasrsing_Argument(int argc, char *argv[])
{
  int c, option_idx = 0;
  struct option options[] = {
    {"config_path", required_argument, 0, 1 /*=getopt_long() 호출 시 option_idx 에 반환되는 값*/},
    {0, 0, 0, 0} // 옵션 배열은 {0,0,0,0} 센티넬에 의해 만료된다.
  };

  while(1) {

    /*
     * 옵션 파싱
     */
    c = getopt_long(argc, argv, "", options, &option_idx);
    if (c == -1) {  // 모든 파라미터 파싱 완료
      break;
    }

    /*
     * 파싱된 옵션 처리
     */
    int ret = RELAY_INNO_Config_ParsedOption(c);
    if (ret < 0) {
      return ret;
    }
  }

  return 0;
}


/**
 * @brief 옵션값에 따라 각 옵션을 처리한다.
 * @param[in] option 옵션값 (struct option 의 4번째 멤버변수)
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int RELAY_INNO_Config_ParsedOption(int option)
{
  switch (option) {
    case 1:
    {
        g_config_path = malloc(sizeof(char) * strlen(optarg));
        strncpy(g_config_path , optarg, strlen(optarg));
        break;
    }
    default: 
    {
      printf("Invalid option\n");
      return -1;
    }
  }
  return 0;
}

