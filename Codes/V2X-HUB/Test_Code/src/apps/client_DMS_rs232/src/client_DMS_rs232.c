#include "client_DMS_rs232.h"

#define DEBUG_1 printf("[DEBUG][%s][%d]\n", __func__, __LINE__);

static int F_i_VH_Client_DMS_RS232_Initial(char *device, int *socket_fd);
static int f_i_VH_DMS_Snapshot_Retransmission_Parser(const int rs232_fd, char *recv_buf, int recv_buf_len, char *upstream_buf, int *upstream_buf_len);
static int f_i_VH_DMS_Snapshot_JPEG_Pkt_Parser(char *recv_JPEG_buf, int recv_JPEG_buf_len, char *upstream_buf, int *upstream_buf_len);
static char f_c_VH_DMS_Checksum(char *data, size_t check_sum_len);
static void f_v_VH_DMS_Print_Hex(char *data, int data_len, int len);
static void f_v_VH_DMS_Config_Initial(struct VH_DMS_Config_Setting_t *dms_config);
static int f_i_VH_DMS_Config_Input_Value(char type, char *value, size_t value_len, int *Max_value);

const char Request_Snapshot[4] = {0x5A, 0x79, 0x42, 0x5D};
const char Request_Snapshot_ACK[4] = {0x1A, 0x01, 0x79, 0x5D};

int main()
{   
    int ret;
    int sock;
    struct VH_UDS_Info_t *uds_info = (struct VH_UDS_Info_t*)malloc(sizeof(struct VH_UDS_Info_t));
    struct VH_UDS_Info_Setup_t *info = (struct VH_UDS_Info_Setup_t*)&(uds_info->info_setup);

    info->path_type = UDS_PATH_TYPE_NOUSE;
    info->server_type = UDS_NOSERVER_CLIENT;

    ret = F_i_VH_Communication_UDS_Server_Setup(info);
    enum VH_UDS_Socket_Triffic_Direction_e sock_direction = TRIFFIC_TO_SERVER;
    char *equip_name = "DMS";
    struct VH_UDS_Response_t res_data;
    ret = F_i_VH_Communication_UDS_Request(NULL, NULL, &sock_direction, equip_name, sizeof(equip_name), &res_data);
    if(ret < 0)
    {
        for(int repeat = 0; repeat < 0; repeat++)
        {
            if(F_i_VH_Communication_UDS_Request(NULL, NULL, &sock_direction, equip_name, sizeof(equip_name), &res_data) > 0)
            {
                break;
            }else{
                sleep(1);
            }
        }
    }

    int rs232_fd;
    char *device_name = "/dev/ttyLP2";
    ret = F_i_VH_Client_DMS_RS232_Initial(device_name, &rs232_fd);

    struct linger solinger = { 1, 0 };
	if (setsockopt(res_data.sock, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger)) == -1) {
		perror("setsockopt(SO_LINGER)");
	} 

    struct VH_DMS_Config_Setting_t *dms_config = malloc(sizeof(struct VH_DMS_Config_Setting_t));
    f_v_VH_DMS_Config_Initial(dms_config);

    for(int repeat = 0; repeat < 1; repeat++)
    {
        ret = write(rs232_fd, (char*)dms_config, sizeof(struct VH_DMS_Config_Setting_t));
        //f_v_VH_DMS_Print_Hex((char*)dms_config, sizeof(struct VH_DMS_Config_Setting_t), 128);
        sleep(1);
    }

    bool repeat_snap_packet_enable = false;
    bool dms_evnet_enable = false;
    char *snapshot_data_prev = NULL;

    while(1)
    {
        char recv_buf[2048] = {0,};
        ret = read(rs232_fd, recv_buf, 2048);
        f_v_VH_DMS_Print_Hex((char*)recv_buf, ret, 128);
Received_Double_Packet:
        if(ret > 0 && (recv_buf[0] == 0x5B && recv_buf[1] == 0x79 && recv_buf[2] == 0x41) && recv_buf[8] != 0x00)
        {
            dms_evnet_enable = true;
            #if 0
            struct DSM_Event_Bitfield *dsm_event = malloc(1);
            memcpy(dsm_event, &recv_buf[8], 1);
            printf("dsm_event->Seat_Belt:%d\n", dsm_event->Seat_Belt);
            printf("dsm_event->Mask:%d\n", dsm_event->Mask);
            printf("dsm_event->Driver_Absence:%d\n", dsm_event->Driver_Absence);
            printf("dsm_event->Smoking:%d\n", dsm_event->Smoking);
            printf("dsm_event->Phone:%d\n", dsm_event->Phone);
            printf("dsm_event->Yawning:%d\n", dsm_event->Yawning);
            printf("dsm_event->Distraction:%d\n", dsm_event->Distraction);
            printf("dsm_event->Drowsiness:%d\n", dsm_event->Drowsiness);
            free(dsm_event);
            #endif
            if(ret > (recv_buf[3] - 0x30 + 6))
            {
                printf("ret:%d/%d\n", ret, (recv_buf[3] - 0x30 + 6));
                memmove(recv_buf, recv_buf + (recv_buf[3] - 0x30 + 6), 2048 - (recv_buf[3] - 0x30 + 6));
                goto Received_Double_Packet;
            }
        }
        if(dms_evnet_enable && (ret > 0 && (recv_buf[0] == 0x5A && recv_buf[1] == 0x79 && recv_buf[2] == 0x02)))
        {
            struct VH_DMS_Snapshot_Hdr_t *snapshot_hdr = (struct VH_DMS_Snapshot_Retransmission_Hdr_t*)recv_buf;  
            uint16_t total_recv_snatshot_data_size = 0;
            uint16_t Total_Picture_Size_In_Byte = snapshot_hdr->Total_Picture_Size_In_Byte[0] * 0xFF + snapshot_hdr->Total_Picture_Size_In_Byte[1];
            uint8_t Total_Picture_Packages = snapshot_hdr->Total_Picture_Packages;
            uint16_t Current_Package_Data_Size = snapshot_hdr->Current_Package_Data_Size[0] * 0xFF + snapshot_hdr->Current_Package_Data_Size[1];
            uint8_t Current_Package_Number = snapshot_hdr->Current_Package_Number;
            
            size_t snapshot_data_len_now = 0;
            size_t snapshot_data_len_prev = 0;
            snapshot_data_len_now = sizeof(struct VH_DMS_Snapshot_Hdr_t) + (sizeof(char) * (Current_Package_Data_Size + 2));
            total_recv_snatshot_data_size += snapshot_data_len_now;
            char *snapshot_data = malloc(Total_Picture_Size_In_Byte + (sizeof(struct VH_DMS_Snapshot_Hdr_t) + 2) * Total_Picture_Packages);
            memcpy(snapshot_data, recv_buf, ret);
            printf("ret:%d/%d\n", ret, snapshot_data_len_now);
            for(int  repeat = 2; repeat <= Total_Picture_Packages; repeat++)
            {
                ret = write(rs232_fd, Request_Snapshot_ACK, 4);
                ret = read(rs232_fd, recv_buf, 2048);
Received_Double_Snapshot_Packet:
                if(ret > 0 && (recv_buf[0] == 0x5A && recv_buf[1] == 0x79 && recv_buf[2] == 0x02))
                {
                    struct VH_DMS_Snapshot_Hdr_t *snapshot_hdr = (struct VH_DMS_Snapshot_Retransmission_Hdr_t*)recv_buf;  
                    
                    Current_Package_Data_Size = snapshot_hdr->Current_Package_Data_Size[0] * 0xFF + snapshot_hdr->Current_Package_Data_Size[1];
                    Current_Package_Number = snapshot_hdr->Current_Package_Number;  
                    snapshot_data_len_prev = snapshot_data_len_now;
                    snapshot_data_len_now = sizeof(struct VH_DMS_Snapshot_Hdr_t) + (sizeof(char) * (Current_Package_Data_Size + 2));
                    total_recv_snatshot_data_size += snapshot_data_len_now;
                    memcpy(snapshot_data + snapshot_data_len_prev, recv_buf, ret);

                }else if(ret > 0 && (recv_buf[0] == 0x5B && recv_buf[1] == 0x79 && recv_buf[2] == 0x41))
                {
                   
                    if(ret > (recv_buf[3] - 0x30 + 6))
                    {
                        printf("ret:%d/%d\n", ret, (recv_buf[3] - 0x30 + 6));
                        memmove(recv_buf, recv_buf + (recv_buf[3] - 0x30 + 6), 2048 - (recv_buf[3] - 0x30 + 6));
                        goto Received_Double_Snapshot_Packet;
                    }else{
                         repeat--;
                    }
                }else{
                    printf("Error\n");
                }
            }
            if(total_recv_snatshot_data_size == Total_Picture_Size_In_Byte + (sizeof(struct VH_DMS_Snapshot_Hdr_t) + 2) * Total_Picture_Packages)
            {
                F_i_VH_Communication_UDS_Sendto((char*)snapshot_data, total_recv_snatshot_data_size + 1, 0, res_data.socketfd);
                printf("snapshot_data_len:%d/%d\n", total_recv_snatshot_data_size,Total_Picture_Size_In_Byte + (sizeof(struct VH_DMS_Snapshot_Hdr_t) + 2) * Total_Picture_Packages);
            }
            memset(snapshot_data, 0x00, Total_Picture_Size_In_Byte + (sizeof(struct VH_DMS_Snapshot_Hdr_t) + 2) * Total_Picture_Packages);
            free(snapshot_data);
            dms_evnet_enable == false;
        /* DMS에 event가 발생했지만 snapshot data transmission 데이터가 전송되지 않으면 스냅샷을 요청해 수신한다. */
        /* When the event occurred in the DMS, DMS sended an snapshot message to the FMS equip. However, if it do not send the message, the FMS equip send a request message to receive snapshot message.*/
        }else if(0)//if(dms_evnet_enable && (ret > 0 && (recv_buf[0] != 0x5A || recv_buf[1] != 0x79 || recv_buf[2] != 0x02)))
        {
Repeat_Snap_Retrans_Packet:
            if(repeat_snap_packet_enable)
            {
                ret = write(rs232_fd, Request_Snapshot_ACK, 4);DEBUG_1
            }else{
                ret = write(rs232_fd, Request_Snapshot, 4);DEBUG_1
            }
            repeat_snap_packet_enable = false;DEBUG_1
        }if(ret > 0 && (recv_buf[0] == 0x5A && recv_buf[1] == 0x79 && recv_buf[2] == 0x04))
        {
            if(recv_buf[ret - 1] == 0x5D)
            { 
                char *upstream_buf = malloc(1);DEBUG_1
                int upstream_buf_len = 0;DEBUG_1
                switch(recv_buf[2])
                {
                    default:break;               
                    case 0x02://Snapshot
                    {
                        //struct VH_DMS_Snapshot_Hdr_t *snapshot_hdr = (struct VH_DMS_Snapshot_Hdr_t*)recv_buf;DEBUG_1
                        break;DEBUG_1
                    }
                    case 0x04:
                    {
                        struct VH_DMS_Snapshot_Retransmission_Hdr_t *snapshot_hdr = (struct VH_DMS_Snapshot_Retransmission_Hdr_t*)recv_buf;DEBUG_1
                        uint16_t Snap_Total_Index = snapshot_hdr->Snap_Total_Index[1] * 0xFF + snapshot_hdr->Snap_Total_Index[0];DEBUG_1
                        if(Snap_Total_Index != 0)
                        {
                            ret = f_i_VH_DMS_Snapshot_Retransmission_Parser(rs232_fd, (char*)recv_buf, ret, upstream_buf, &upstream_buf_len);DEBUG_1
                            if(ret >= 0)
                            {
                                uint16_t Snap_Current_Index_prev = snapshot_hdr->Snap_Current_Index[1] * 0xFF + snapshot_hdr->Snap_Current_Index[0];DEBUG_1
                                printf("Snap_Total_Index/Snap_Current_Index_prev:%d/%d\n", Snap_Total_Index, Snap_Current_Index_prev);DEBUG_1
                                if(Snap_Total_Index > Snap_Current_Index_prev)
                                {
                                    repeat_snap_packet_enable = true;DEBUG_1
                                    goto Repeat_Snap_Retrans_Packet;DEBUG_1
                                }
                            }
                        }else{
                            printf("No Snap Shot. Snap_Total_Index:%d\n", Snap_Total_Index);DEBUG_1
                            repeat_snap_packet_enable = false;DEBUG_1
                        }
                        break;DEBUG_1
                    }
                }
                free(upstream_buf);DEBUG_1
            }else{
                int recv_one_len = 0;DEBUG_1
                while(1)
                {
                    if(recv_buf[recv_one_len] == 0x5D)
                        break;DEBUG_1
                    recv_one_len++;DEBUG_1
                    if(recv_one_len >= ret)
                        break;DEBUG_1
                }
                recv_one_len++;DEBUG_1
                char *upstream_buf = malloc(1);DEBUG_1
                int upstream_buf_len = 0;DEBUG_1
                switch(recv_buf[2])
                {
                    default:break;               
                    case 0x02://Snapshot
                    {
                        //struct VH_DMS_Snapshot_Hdr_t *snapshot_hdr = (struct VH_DMS_Snapshot_Hdr_t*)recv_buf;DEBUG_1
                        break;DEBUG_1
                    }
                    case 0x04:
                    {
                        struct VH_DMS_Snapshot_Retransmission_Hdr_t *snapshot_hdr = (struct VH_DMS_Snapshot_Retransmission_Hdr_t*)recv_buf;DEBUG_1
                        uint16_t Snap_Total_Index = snapshot_hdr->Snap_Total_Index[1] * 0xFF + snapshot_hdr->Snap_Total_Index[0];DEBUG_1
                        if(Snap_Total_Index != 0)
                        {
                            ret = f_i_VH_DMS_Snapshot_Retransmission_Parser(rs232_fd, (char*)recv_buf, ret, upstream_buf, &upstream_buf_len);DEBUG_1
                            if(ret >= 0)
                            {
                                uint16_t Snap_Current_Index_prev = snapshot_hdr->Snap_Current_Index[1] * 0xFF + snapshot_hdr->Snap_Current_Index[0];DEBUG_1
                                printf("Snap_Total_Index/Snap_Current_Index_prev:%d/%d\n", Snap_Total_Index, Snap_Current_Index_prev);DEBUG_1
                                if(Snap_Total_Index > Snap_Current_Index_prev)
                                {
                                    repeat_snap_packet_enable = true;DEBUG_1
                                    goto Repeat_Snap_Retrans_Packet;DEBUG_1
                                }
                            }
                        }else{
                            repeat_snap_packet_enable = false;DEBUG_1
                            printf("No Snap Shot. Snap_Total_Index:%d\n", Snap_Total_Index);DEBUG_1
                        }
                        break;DEBUG_1
                    }
                }
                free(upstream_buf);
                ret -= (recv_one_len);
                memmove(recv_buf, recv_buf + recv_one_len, 2048 - recv_one_len);
                goto Received_Double_Packet; 
            }
            dms_evnet_enable == false;
        }
    }
    free(dms_config);
    close(rs232_fd);
    close(res_data.sock);
    close(sock);
    return 0;
}

extern int F_i_VH_Client_DMS_RS232_Initial(char *device, int *socket_fd)
{
	struct termios  oldtio, newtio;
	int ret;

	/* Open GPS interface */
	*socket_fd	=	open(device,  O_RDWR);
    
    //flock(*socket_fd, LOCK_EX | LOCK_NB);
	/* Configuring GPS interface */
	{	
		ret = tcgetattr(*socket_fd, &oldtio);					// save current serial port setting
        if(ret < 0)
            return -1;
		memset(&newtio, 0, sizeof(newtio));
		
		/*
		 * BAUDRATE	:	���ۼӵ� 115200
		 * CS8		:	8N1(8bit, no parity, 1 stop bit)
		 * CLOCAL	: 	Local connection(�� ��� ���� ����)
		 * CREAD	:	���� ������ �����ϰ� ��
		 */
        newtio.c_iflag &= ~(IGNCR | INLCR | ICRNL);
	    //newtio.c_lflag &= ~ISIG;
        newtio.c_lflag &= ~ICANON;
        newtio.c_lflag &= ~(ECHO | ECHOCTL | ECHONL);
        newtio.c_cflag |=  B115200 | CREAD | CLOCAL;
        //newtio.c_iflag |= IGNBRK;
        newtio.c_cc[VMIN] = 1;
        newtio.c_cc[VTIME] = 0;
        //cfsetospeed(&newtio, B115200);
        //cfsetispeed(&newtio, B115200);
		/* Modem init and port configuration */
		tcflush(*socket_fd, TCIFLUSH);
		tcsetattr(*socket_fd, TCSANOW, &newtio);
	}
	
	return	0;
}


static int f_i_VH_DMS_Snapshot_Retransmission_Parser(const int rs232_fd, char *recv_buf, int recv_buf_len, char *upstream_buf, int *upstream_buf_len)
{
    int ret;

    struct VH_DMS_Snapshot_Retransmission_Hdr_t *snapshot_hdr = (struct VH_DMS_Snapshot_Retransmission_Hdr_t*)recv_buf;
    uint8_t JPEG_Pkt_Total_Binary_Num = snapshot_hdr->JPEG_Pkt_Total_Binary_Num;
    uint8_t JPEG_Pkt_Current_Binary_Num_prev = snapshot_hdr->JPEG_Pkt_Current_Binary_Num;
    printf("JPEG_Pkt_Total_Binary_Num/JPEG_Pkt_Current_Binary_Num_prev:%d/%d\n", JPEG_Pkt_Total_Binary_Num, JPEG_Pkt_Current_Binary_Num_prev);
    if(snapshot_hdr->STX == 0x5A && snapshot_hdr->Type1 == 0x79 && snapshot_hdr->Type2 == 0x04)
    {
        *upstream_buf_len += recv_buf_len;
        if(upstream_buf)
        {
            char *ptr_backup = upstream_buf_len;
            if(NULL == (upstream_buf_len = (char*)realloc(upstream_buf, *upstream_buf_len)))
            {
                upstream_buf_len = ptr_backup;
                fprintf(stderr, "Memory allocation is failed");
                return -1;
            }
        }else{
            return -1;
        }
        memcpy(upstream_buf - (*upstream_buf_len - recv_buf_len), recv_buf, recv_buf_len);
    }else{
        return -1;
    }
    if(JPEG_Pkt_Total_Binary_Num >= JPEG_Pkt_Current_Binary_Num_prev)
    {
        for(uint8_t JPEG_Pkt_Num = JPEG_Pkt_Current_Binary_Num_prev + 1; JPEG_Pkt_Num <= JPEG_Pkt_Total_Binary_Num; JPEG_Pkt_Num++)
        {
            ret = write(rs232_fd, Request_Snapshot_ACK, 4);
            
            for(int for_block_count; for_block_count < 5; for_block_count++)
            {
                char recv_JPEG_buf[2048] = {0,};
                int recv_JPEG_buf_len = read(rs232_fd, recv_JPEG_buf, 2048);
                ret = f_i_VH_DMS_Snapshot_JPEG_Pkt_Parser((char*)recv_JPEG_buf, recv_JPEG_buf_len, upstream_buf, upstream_buf_len);
                if(ret == 0)
                {
                    break;
                }
            }
        }
    }
    return 0;
}

static int f_i_VH_DMS_Snapshot_JPEG_Pkt_Parser(char *recv_JPEG_buf, int recv_JPEG_buf_len, char *upstream_buf, int *upstream_buf_len)
{
    struct VH_DMS_Snapshot_Retransmission_Hdr_t *snapshot_hdr = (struct VH_DMS_Snapshot_Retransmission_Hdr_t*)recv_JPEG_buf;
    if(snapshot_hdr->STX == 0x5A && snapshot_hdr->Type1 == 0x79 && snapshot_hdr->Type2 == 0x04)
    {
        *upstream_buf_len += recv_JPEG_buf_len;
        if(upstream_buf)
        {
            char *ptr_backup = upstream_buf_len;
            if(NULL == (upstream_buf_len = (char*)realloc(upstream_buf, *upstream_buf_len)))
            {
                upstream_buf_len = ptr_backup;
                fprintf(stderr, "Memory allocation is failed");
                return -1;
            }
        }else{
            return -1;
        }
        memcpy(upstream_buf - (*upstream_buf_len - recv_JPEG_buf_len), recv_JPEG_buf, recv_JPEG_buf_len);
    }else{
        return -1;
    }
    return 0;
}

static char f_c_VH_DMS_Checksum(char *data, size_t check_sum_len)
{
    char checksum = 0x00;
    for (size_t i = 0; i <= check_sum_len; i++)
    {
        checksum += data[i];
    }

    return ~checksum + 0x1;
}

static void f_v_VH_DMS_Print_Hex(char *data, int data_len, int len)
{
    for(int  i = 0; i < data_len; i++)
    {
        printf("%02X", data[i]);
        if(i % len == len - 1)
        {
            printf("\n");
        }
    }
    printf("\n");
}

static void f_v_VH_DMS_Config_Initial(struct VH_DMS_Config_Setting_t *dms_config)
{
    int ret;
    ret = access(DMS_CONFIG_FILE_DEFAULT_PATH, F_OK);
    if(ret < 0)
    {
        char sys_cmd[128] = {0,}; 
        sprintf(sys_cmd, "mkdir -m 777 -p %s", DMS_CONFIG_FILE_DEFAULT_PATH);
        system(sys_cmd);
    }
    char *config_file_name = "mdms7.config";
    char *config_file = (char*)malloc(strlen(DMS_CONFIG_FILE_DEFAULT_PATH "/") + strlen(config_file_name));
    sprintf(config_file, "%s/%s", DMS_CONFIG_FILE_DEFAULT_PATH, config_file_name);
    ret = access(config_file, F_OK);
    if(ret < 0)
    {
        FILE *config_fp = fopen(config_file, "w");
        fputs("Change_Config_Now=0x00;\n", config_fp);
        fputs("Type1=0x79;\n", config_fp);
        fputs("Type2=0x41;\n", config_fp);
        fputs("Data_Length=0x4A;\n", config_fp);
        fputs("Drowsiness_Setting.OnOff=1;\n", config_fp);
        fputs("Drowsiness_Setting.Sound=0b00;\n", config_fp);
        fputs("Drowsiness_Setting.Vibration=0;\n", config_fp);
        fputs("Drowsiness_Setting.Sensitivity=0b00;\n", config_fp);
        fputs("Drowsiness_Setting.Dummy=0b11;\n", config_fp);
        fputs("Distration_Setting.OnOff=1;\n", config_fp);
        fputs("Distration_Setting.Sound=0b00;\n", config_fp);
        fputs("Distration_Setting.Vibration=0;\n", config_fp);
        fputs("Distration_Setting.Sensitivity=0b00;\n", config_fp);
        fputs("Distration_Setting.Dummy=0b11;\n", config_fp);
        fputs("Phone_Setting.OnOff=1;\n", config_fp);
        fputs("Phone_Setting.Sound=0b00;\n", config_fp);
        fputs("Phone_Setting.Vibration=0;\n", config_fp);
        fputs("Phone_Setting.Sensitivity=0b00;\n", config_fp);
        fputs("Phone_Setting.Dummy=0b11;\n", config_fp);
        fputs("Smoking_Setting.OnOff=1;\n", config_fp);
        fputs("Smoking_Setting.Sound=0b00;\n", config_fp);
        fputs("Smoking_Setting.Vibration=0;\n", config_fp);
        fputs("Smoking_Setting.Sensitivity=0b00;\n", config_fp);
        fputs("Smoking_Setting.Dummy=0b11;\n", config_fp);
        fputs("Yawning_Setting.OnOff=1;\n", config_fp);
        fputs("Yawning_Setting.Sound=0b00;\n", config_fp);
        fputs("Yawning_Setting.Vibration=0;\n", config_fp);
        fputs("Yawning_Setting.Sensitivity=0b00;\n", config_fp);
        fputs("Yawning_Setting.Dummy=0b11;\n", config_fp);
        fputs("bit_filed_0.Kmpermile=0;\n", config_fp);
        fputs("bit_filed_0.SeatBelt_Setting_OnOff=0;\n", config_fp);
        fputs("bit_filed_0.SeatBelt_Setting_Sound=0b00;\n", config_fp);
        fputs("bit_filed_0.SeatBelt_Setting_Driver_Pos=1;\n", config_fp);
        fputs("bit_filed_0.SeatBelt_Setting_Dummy=0b111;\n", config_fp);
        fputs("Volume_Level=0x01;\n", config_fp);
        fputs("Working_Speed=0x28;\n", config_fp);
        fputs("DVR_Setting1.Voice=0b0;\n", config_fp);
        fputs("DVR_Setting1.G_Sensor=0b100;\n", config_fp);
        fputs("DVR_Setting1.log=0b1;\n", config_fp);
        fputs("DVR_Setting1.Dummy=0b000;\n", config_fp);
        fputs("DVR_Setting2=0x07|0b11110000;\n", config_fp);
        fputs("Miscellaneous_Setting.Video_Format=0;\n", config_fp);
        fputs("Miscellaneous_Setting.Point_Overlay_on_preview=1;\n", config_fp);
        fputs("Miscellaneous_Setting.Transmit_Drowsiness=1;\n", config_fp);
        fputs("Miscellaneous_Setting.Camera_block_repeat_alarm=1;\n", config_fp);
        fputs("Miscellaneous_Setting.Transmit_Not_Listed=1;\n", config_fp);
        fputs("Miscellaneous_Setting.Transmit_Distraction=1;\n", config_fp);
        fputs("Miscellaneous_Setting.Transmit_Phone=1;\n", config_fp);
        fputs("Miscellaneous_Setting.Transmit_Smoking=1;\n", config_fp);
        fputs("Reboot=0x56+0x01;\n", config_fp);
        fputs("Year[0]=0xE8;\n", config_fp);
        fputs("Year[1]=0x07;\n", config_fp);
        fputs("Mon=0x04;\n", config_fp);
        fputs("Day=0x1D;\n", config_fp);
        fputs("Hour=0x10;\n", config_fp);
        fputs("Min=0x0A;\n", config_fp);
        fputs("Sec=0x28;\n", config_fp);
        fputs("Dip_Switch.Hardware_Switch=0b0;\n", config_fp);
        fputs("Dip_Switch.Mirror_Display=0b0;\n", config_fp);
        fputs("Dip_Switch.Test_Mode=0b1;\n", config_fp);
        fputs("Dip_Switch.Type_of_transmit=0b0;\n", config_fp);
        fputs("Dip_Switch.Timeout_feature=0b0;\n", config_fp);
        fputs("Dip_Switch.Dummy=0b000;\n", config_fp);
        fputs("Snapshot_Request=0x00;\n", config_fp);
        fputs("Working_Speed_Drowsiness=0x00;\n", config_fp);
        fputs("Working_Speed_Distraction=0x00;\n", config_fp);
        fputs("Working_Speed_Yawning=0x00;\n", config_fp);
        fputs("Working_Speed_Phone=0x00;\n", config_fp);
        fputs("Working_Speed_Smoking=0x00;\n", config_fp);
        fclose(config_fp);
    }

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
        char *ptr = strtok(str, "=");
        if(strcmp(ptr, "Type1") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Type1 = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Type2") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Type2 = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Data_Length") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Data_Length = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Drowsiness_Setting.OnOff") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Drowsiness_Setting.OnOff = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Drowsiness_Setting.Sound") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Drowsiness_Setting.Sound = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Drowsiness_Setting.Vibration") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Drowsiness_Setting.Vibration = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Drowsiness_Setting.Sensitivity") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Drowsiness_Setting.Sensitivity = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Drowsiness_Setting.Dummy") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Drowsiness_Setting.Dummy = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Distration_Setting.OnOff") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Distration_Setting.OnOff = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Distration_Setting.Sound") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Distration_Setting.Sound = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Distration_Setting.Vibration") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Distration_Setting.Vibration = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Distration_Setting.Sensitivity") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Distration_Setting.Sensitivity = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Distration_Setting.Dummy") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Distration_Setting.Dummy = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Phone_Setting.OnOff") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Phone_Setting.OnOff = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Phone_Setting.Sound") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Phone_Setting.Sound = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Phone_Setting.Vibration") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Phone_Setting.Vibration = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Phone_Setting.Sensitivity") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Phone_Setting.Sensitivity = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Phone_Setting.Dummy") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Phone_Setting.Dummy = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Smoking_Setting.OnOff") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Smoking_Setting.OnOff = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Smoking_Setting.Sound") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Smoking_Setting.Sound = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Smoking_Setting.Vibration") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Smoking_Setting.Vibration = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Smoking_Setting.Sensitivity") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Smoking_Setting.Sensitivity = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Smoking_Setting.Dummy") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Smoking_Setting.Dummy = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Yawning_Setting.OnOff") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Yawning_Setting.OnOff = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Yawning_Setting.Sound") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Yawning_Setting.Sound = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Yawning_Setting.Vibration") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Yawning_Setting.Vibration = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Yawning_Setting.Sensitivity") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Yawning_Setting.Sensitivity = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Yawning_Setting.Dummy") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Yawning_Setting.Dummy = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "bit_filed_0.Kmpermile") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->bit_filed_0.Kmpermile = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "bit_filed_0.SeatBelt_Setting_OnOff") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->bit_filed_0.SeatBelt_Setting_OnOff = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "bit_filed_0.SeatBelt_Setting_Sound") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->bit_filed_0.SeatBelt_Setting_Sound = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "bit_filed_0.SeatBelt_Setting_Driver_Pos") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->bit_filed_0.SeatBelt_Setting_Driver_Pos = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "bit_filed_0.SeatBelt_Setting_Dummy") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->bit_filed_0.SeatBelt_Setting_Dummy = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Volume_Level") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Volume_Level = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Working_Speed") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Working_Speed = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "DVR_Setting1.Voice") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->DVR_Setting1.Voice = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "DVR_Setting1.G_Sensor") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->DVR_Setting1.G_Sensor = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "DVR_Setting1.log") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->DVR_Setting1.log = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "DVR_Setting1.Dummy") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->DVR_Setting1.Dummy = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "DVR_Setting2") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->DVR_Setting2 = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Miscellaneous_Setting.Video_Format") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Miscellaneous_Setting.Video_Format = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Miscellaneous_Setting.Point_Overlay_on_preview") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Miscellaneous_Setting.Point_Overlay_on_preview = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Miscellaneous_Setting.Transmit_Drowsiness") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Miscellaneous_Setting.Transmit_Drowsiness = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Miscellaneous_Setting.Camera_block_repeat_alarm") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Miscellaneous_Setting.Camera_block_repeat_alarm = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Miscellaneous_Setting.Transmit_Not_Listed") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Miscellaneous_Setting.Transmit_Not_Listed = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Miscellaneous_Setting.Transmit_Distraction") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Miscellaneous_Setting.Transmit_Distraction = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Miscellaneous_Setting.Transmit_Phone") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Miscellaneous_Setting.Transmit_Phone = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Miscellaneous_Setting.Transmit_Smoking") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Miscellaneous_Setting.Transmit_Smoking = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Reboot") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Reboot = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Year[0]") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Year[0] = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Year[1]") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Year[1] = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Mon") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Mon = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Day") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Day = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Hour") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Hour = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Min") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Min = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Sec") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Sec = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Dip_Switch.Hardware_Switch") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Dip_Switch.Hardware_Switch = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Dip_Switch.Mirror_Display") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Dip_Switch.Mirror_Display = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Dip_Switch.Test_Mode") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Dip_Switch.Test_Mode = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Dip_Switch.Type_of_transmit") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Dip_Switch.Type_of_transmit = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Dip_Switch.Timeout_feature") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Dip_Switch.Timeout_feature = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Dip_Switch.Dummy") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Dip_Switch.Dummy = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Snapshot_Request") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Snapshot_Request = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Working_Speed_Drowsiness") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Working_Speed_Drowsiness = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Working_Speed_Distraction") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Working_Speed_Distraction = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Working_Speed_Yawning") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Working_Speed_Yawning = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Working_Speed_Phone") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Working_Speed_Phone = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }else if(strcmp(ptr, "Working_Speed_Smoking") == 0){
            ptr = strtok(NULL, ";");
            char type = ptr[1];
            dms_config->Working_Speed_Smoking = f_i_VH_DMS_Config_Input_Value(type, ptr, strlen(ptr), NULL);
        }
    }
    fclose(config_fp);

    dms_config->STX = 0x51;
    dms_config->Checksum = f_c_VH_DMS_Checksum((char*)dms_config + 1, sizeof(struct VH_DMS_Config_Setting_t) - 4); 
    dms_config->EXT = 0x5F;

    free(config_file);
    
    return;
}

static int f_i_VH_DMS_Config_Input_Value(char type, char *value, size_t value_len, int *Max_value)
{
    int ret;
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
                ret = decimal;
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
            ret = decimal;
            break;
        }
        default:
        {
            ret = atoi(value);
            break;
        }
    }
    if(Max_value && ret > Max_value)
    {
        ret = Max_value;
    }
    return ret;
}