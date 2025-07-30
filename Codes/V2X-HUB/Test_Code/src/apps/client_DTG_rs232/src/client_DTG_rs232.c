#include "client_DTG_rs232.h"

#define DEBUG_1 printf("[DEBUG][%s][%d]\n", __func__, __LINE__);

#define DTG_CONFIG_FILE_DEFAULT_PATH "./configs/DTG/"

static int f_i_VH_Client_DTG_RS232_Initial(char *device, int *socket_fd);
static int f_i_VH_DMS_Save_Data_Header(char* hdr_data, size_t hdr_data_len);

int main()
{   
    int ret;DEBUG_1
    int sock;DEBUG_1
    struct VH_UDS_Info_t *uds_info = (struct VH_UDS_Info_t*)malloc(sizeof(struct VH_UDS_Info_t));DEBUG_1
    struct VH_UDS_Info_Setup_t *info = (struct VH_UDS_Info_Setup_t*)&(uds_info->info_setup);DEBUG_1

    info->path_type = UDS_PATH_TYPE_NOUSE;DEBUG_1
    info->server_type = UDS_NOSERVER_CLIENT;DEBUG_1

    ret = F_i_VH_Communication_UDS_Server_Setup(info);DEBUG_1
    enum VH_UDS_Socket_Triffic_Direction_e sock_direction = TRIFFIC_TO_SERVER;DEBUG_1
    char *equip_name = "DTG";DEBUG_1
    struct VH_UDS_Response_t res_data;DEBUG_1
    ret = F_i_VH_Communication_UDS_Request(NULL, NULL, &sock_direction, equip_name, sizeof(equip_name), &res_data);DEBUG_1
    if(ret < 0)
    {
        for(int repeat = 0; repeat < 5; repeat++)
        {
            if(F_i_VH_Communication_UDS_Request(NULL, NULL, &sock_direction, equip_name, sizeof(equip_name), &res_data) >= 0)
            {
                break;DEBUG_1
            }else{
                sleep(1);DEBUG_1
            }
        }
    }
    int rs232_fd;DEBUG_1
    char *device_name = "/dev/ttyLP2";DEBUG_1
    ret = f_i_VH_Client_DTG_RS232_Initial(device_name, &rs232_fd);   

    struct linger solinger = { 1, 0 };DEBUG_1
	if (setsockopt(res_data.sock, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger)) == -1) {
		perror("setsockopt(SO_LINGER)");DEBUG_1
	} 
    if(res_data.sock > 0)
    {
        struct timeval sock_tv;                  /* Socket Send/Recv Block Timer */      
        sock_tv.tv_sec = (int)(800 / 1000);DEBUG_1
        sock_tv.tv_usec = (800 % 1000) * 1000; 
        if (setsockopt(res_data.sock, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&sock_tv, sizeof(struct timeval)) == SO_ERROR) {
            perror("setsockopt(SO_RCVTIMEO)");DEBUG_1
        }
    }

    int dtg_1s_data_size = sizeof(struct VH_DTG_Data_1s_t);DEBUG_1
    int dtg_hdr_data_size = sizeof(struct VH_DTG_Data_Header_t);DEBUG_1
    enum VH_DTG_Data_State_e send_data_state = 1;DEBUG_1
    while(1)
    {
        char recv_buf[2048] = {0,};DEBUG_1
        ret = read(rs232_fd, recv_buf, 2048);DEBUG_1
        switch(send_data_state)
        {
            case DTG_SEND_HEADER:
            {
                struct VH_DTG_Data_Header_t *dtg_data = malloc(dtg_hdr_data_size);DEBUG_1
                if(ret > 0 && (recv_buf[0] == 0x02 && recv_buf[dtg_hdr_data_size - 1] == 0x03))
                {
                    memcpy(dtg_data, &recv_buf[0], dtg_hdr_data_size);DEBUG_1
                    ret = f_i_VH_DMS_Save_Data_Header(&((char*)dtg_data)[0], dtg_hdr_data_size);DEBUG_1
                }else{
                    ret = f_i_VH_DMS_Save_Data_Header((char*)dtg_data, 0);DEBUG_1
                }
                if(ret < 0)
                {
                    printf("[DEBUG][ERROR]... No DTG Header Data.\n");
                    return 0;DEBUG_1
                }
                ret = F_i_VH_Communication_UDS_Sendto((char*)dtg_data, dtg_hdr_data_size, 0, res_data.socketfd);DEBUG_1
                if(ret > 0)
                {   
                    char hdr_recv_signla[4] = {0,};DEBUG_1
                    int sock_signal = F_i_VH_Communication_UDS_Socket_Bind(res_data.signalfd);DEBUG_1
                    ret = recvfrom(sock_signal, hdr_recv_signla, 4, 0, NULL, 0);DEBUG_1

                    if(ret > 0 && strncmp((char*)hdr_recv_signla, "HDR!", 4) == 0)
                    {
                        send_data_state = DTG_RECV_O;DEBUG_1
                        close(sock_signal);DEBUG_1
                    }
                }
                free(dtg_data);DEBUG_1
                break;DEBUG_1
            }
            case DTG_RECV_O:
            {
                ret = write(rs232_fd, "O", 1);DEBUG_1
                if(ret > 0 && (recv_buf[0] == 0x02 && recv_buf[dtg_1s_data_size] == 0x03))
                {
                    send_data_state = DTG_SEND_1sDATA;DEBUG_1
                }else{
                    break;DEBUG_1
                }
            }
            case DTG_SEND_1sDATA:
            {

Received_Double_Patcket:
                if(ret > 0 && (recv_buf[0] == 0x02 && recv_buf[dtg_1s_data_size] == 0x03))
                {
                    struct VH_DTG_Data_1s_t *dtg_data = malloc(dtg_1s_data_size + 1);DEBUG_1
                    memcpy(dtg_data, &recv_buf[0], dtg_1s_data_size + 1);DEBUG_1
                    F_i_VH_Communication_UDS_Sendto((char*)dtg_data, dtg_1s_data_size + 1, 0, res_data.socketfd);DEBUG_1
                    free(dtg_data);DEBUG_1
                }else if(ret > 0 && (recv_buf[0] == 0x02 && ret > dtg_1s_data_size)){
                
                    int recv_one_len = dtg_1s_data_size;DEBUG_1

                    struct VH_DTG_Data_1s_t *dtg_data = malloc(dtg_1s_data_size);DEBUG_1
                    memcpy(dtg_data, &recv_buf[0], dtg_1s_data_size);DEBUG_1
                    F_i_VH_Communication_UDS_Sendto((char*)dtg_data, dtg_1s_data_size, 0, res_data.socketfd);DEBUG_1
                    free(dtg_data);DEBUG_1

                    ret -= (recv_one_len);DEBUG_1
                    if(ret >= dtg_1s_data_size)
                    {
                        memmove(recv_buf, recv_buf + recv_one_len, 2048 - recv_one_len);DEBUG_1
                        goto Received_Double_Patcket;DEBUG_1
                    }
                }
            }
        }
    }

    close(rs232_fd);DEBUG_1
    close(res_data.sock);DEBUG_1
    close(sock);
    return 0;
}

extern int f_i_VH_Client_DTG_RS232_Initial(char *device, int *socket_fd)
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

static int f_i_VH_DMS_Save_Data_Header(char* hdr_data, size_t hdr_data_len)
{
    int ret;DEBUG_1
    char *file_path = DTG_CONFIG_FILE_DEFAULT_PATH;DEBUG_1

    char *hdr_file_name = "DTG_Data.hdr";DEBUG_1
    char *hdr_file = (char*)malloc(strlen(DTG_CONFIG_FILE_DEFAULT_PATH "/") + strlen(hdr_file_name));DEBUG_1
    sprintf(hdr_file, "%s/%s", DTG_CONFIG_FILE_DEFAULT_PATH, hdr_file_name);DEBUG_1
    ret = access(hdr_file, F_OK);DEBUG_1
    if(ret >= 0)
    {
        FILE *config_fp = fopen(hdr_file, "r");DEBUG_1
        char str[256] = {0,};DEBUG_1
        char *rp;DEBUG_1
        if(hdr_data_len != 0 && hdr_data_len == 82)
        {
            for(int i = 0; i < hdr_data_len; i++)
            {
                str[i] = fgetc(config_fp);
                if(str[i] == hdr_data[i])
                {
                }else{
                    fclose(config_fp);
                    unlink(hdr_file);
                    break;
                }
                if(str[hdr_data_len - 1] == 0x03)
                {
                    free(hdr_file);
                    return 2;
                }
            }
        }else if(hdr_data_len == 0 && hdr_data != NULL)
        {
            for(int i = 0; i < 82; i++)
            {
                hdr_data[i] = fgetc(config_fp);
                printf("%02X", hdr_data[i] );
            }
            printf("\n");
            fclose(config_fp);DEBUG_1
            free(hdr_file);DEBUG_1
            return 3;DEBUG_1
        }
    }
   
    ret = access(DTG_CONFIG_FILE_DEFAULT_PATH, F_OK);DEBUG_1
    if(ret < 0)
    {
        char sys_cmd[128] = {0,}; 
        sprintf(sys_cmd, "mkdir -m 777 -p %s", DTG_CONFIG_FILE_DEFAULT_PATH);
        system(sys_cmd);
    }

    FILE *config_fp = fopen(hdr_file, "w");DEBUG_1
    if(hdr_data_len == 82)
    {
        fputs(hdr_data, config_fp);DEBUG_1
        free(hdr_file);DEBUG_1
        return 0;DEBUG_1
    }

    free(hdr_file);DEBUG_1
    return -1;DEBUG_1
}
   