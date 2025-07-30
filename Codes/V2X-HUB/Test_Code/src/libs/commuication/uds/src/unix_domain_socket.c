#include "unix_domain_socket.h"

#define DEBUG_1 //printf("[DEBUG][%s][%d]\n", __func__, __LINE__);

static int f_i_VH_Communication_UDS_Create_SockFD(char *path, int force);
static int f_i_VH_Communication_UDS_Rmdirs(const char *path, int force);

int F_i_VH_Communication_UDS_Server_Setup(struct VH_UDS_Info_Setup_t *setup_info)
{
    int ret;
    if(!setup_info->socket_path)
    {
        switch(setup_info->server_type)
        {
            default: break;
            case UDS_SERVER_SIFM:
            {
                setup_info->socket_path = COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/";
                break;
            }
            case UDS_SERVER_SIDM:
            {
                setup_info->socket_path = COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIDM_NAME "/";
            }
        }
        
    }
    if(!setup_info->request_socket_name)
    {
        setup_info->request_socket_name = COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME;
    }
    if(!setup_info->response_socket_name)
    {
        setup_info->response_socket_name = COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME;
    }
    
    ret = access(setup_info->socket_path, F_OK);
    if(ret == 0)
    {
        unlink(setup_info->socket_path);
        ret = f_i_VH_Communication_UDS_Rmdirs(COMUNICATIONS_UDS_DEFAULT_PATH, true);
        if(ret == -1)
        {
            char sys_cmd[256] = {0x00,};
            sprintf(sys_cmd, "umount %s", RAMDISK_DEVICE_PATH);
            system(sys_cmd);
            memset(sys_cmd, 0x00, 256);
            sprintf(sys_cmd, "rm -r %s", COMUNICATIONS_UDS_DEFAULT_PATH);
            system(sys_cmd);
            //printf("[DEBUG][Error][%d][%s] %s\n", __LINE__, __func__, "Socket path clear error.");
        }
    }

    switch(setup_info->path_type)
    {
        default:break;
        case UDS_PATH_TYPE_RAMDISK:
        {
           
            char sys_cmd[256] = {0x00,};
            sprintf(sys_cmd, "mkdir -m 777 -p %s", setup_info->socket_path);
            system(sys_cmd);
            sleep(1);
            memset(sys_cmd, 0x00, 256);
            sprintf(sys_cmd, "umount %s", RAMDISK_DEVICE_PATH);
            system(sys_cmd);
            sleep(1);
            memset(sys_cmd, 0x00, 256);
            sprintf(sys_cmd, "mount -t tmpfs %s %s -o size=%dM", RAMDISK_DEVICE_PATH, setup_info->socket_path, RAMDISK_DEFAULT_SIZE);
            printf("%s\n", sys_cmd);
            system(sys_cmd);
            memset(sys_cmd, 0x00, 256);
            sleep(1);
            ret = access(setup_info->socket_path, F_OK);
            if(ret == -1)
            {
                printf("[DEBUG][Error][%d][%s] %s\n", __LINE__, __func__, "RAMDISK not mount.");
                return (-1) * ((DOMAIN_CODE_CUMMNICATION + CUMMNICATION_LIST_CODE_UDS) + 0x00000001 + 02);
            }else{
                unlink(setup_info->socket_path);
            }
            break;
        }
        case UDS_PATH_TYPE_FLASH:
        {
            char sys_cmd[256] = {0x00,};
            sprintf(sys_cmd, "mkdir -m 777 -p %s", setup_info->socket_path);
            system(sys_cmd);
            #if 0
            ret = mkdir(setup_info->socket_path, 0777);
            if(ret == -1)
            {
                printf("[DEBUG][Error][%d][%s] %s\n", __LINE__, __func__, "Socket file paht not create.");
                
                //return (-1) * ((DOMAIN_CODE_CUMMNICATION + CUMMNICATION_LIST_CODE_UDS) + 01 + 03);
            }
            #endif
            break;
        }
    }
    if(setup_info->server_type != UDS_NOSERVER_CLIENT)
    {
        {
            char tmp_sockfd_path[256] = {0x00,};
            sprintf(tmp_sockfd_path, "%s/%s", setup_info->socket_path, setup_info->request_socket_name);
            ret = f_i_VH_Communication_UDS_Create_SockFD(tmp_sockfd_path, 0777);
            if(ret == -1)
            {
                printf("[DEBUG][Error][%d][%s] %s\n", __LINE__, __func__, "Could not create the socket.");
                return (-1) * ((DOMAIN_CODE_CUMMNICATION + CUMMNICATION_LIST_CODE_UDS) + 01 + 03);
            }
            F_i_VH_Communication_UDS_Socket_Bind(tmp_sockfd_path);

        }
        {
            char tmp_sockfd_path[256] = {0x00,};
            sprintf(tmp_sockfd_path, "%s/%s", setup_info->socket_path, setup_info->response_socket_name);
            ret = f_i_VH_Communication_UDS_Create_SockFD(tmp_sockfd_path, 0777);
            if(ret == -1)
            {
                printf("[DEBUG][Error][%d][%s] %s\n", __LINE__, __func__, "Could not create the socket.");
                return (-1) * ((DOMAIN_CODE_CUMMNICATION + CUMMNICATION_LIST_CODE_UDS) + 01 + 04);
            }
            F_i_VH_Communication_UDS_Socket_Bind(tmp_sockfd_path);
        }
    }
    return 0;
}

extern int F_i_VH_Communication_UDS_Socket_Bind(char* file_path)
{
    int ret;
    int sock;
    struct sockaddr_un target_addr;

    ret = access(file_path, F_OK);
    if(ret == 0)
            unlink(file_path);

    sock  = socket(PF_FILE, SOCK_DGRAM, 0);
    memset(&target_addr, 0, sizeof( target_addr));
    target_addr.sun_family = AF_UNIX;
    strcpy(target_addr.sun_path, file_path);
    if( -1 == bind( sock, (struct sockaddr*)&target_addr, sizeof(target_addr)) )
    {
        printf("[DEBUG][Error][%d][%s] %s\n", __LINE__, __func__, "Error bind()");
        return (-1) * ((DOMAIN_CODE_CUMMNICATION + CUMMNICATION_LIST_CODE_UDS) + 200 + 1);
    }
    return sock;
}

extern int F_i_VH_Communication_UDS_Sendto(char* data, size_t data_len, int flag, char* targetfd)
{
    int ret;
    int sock  = socket(PF_FILE, SOCK_DGRAM, 0);DEBUG_1
    struct sockaddr_un target_addr;DEBUG_1
    int target_addr_len;DEBUG_1
    memset(&target_addr, 0x00, sizeof(target_addr));DEBUG_1
    target_addr.sun_family = AF_UNIX;DEBUG_1
    memcpy(target_addr.sun_path, targetfd, strlen(targetfd));DEBUG_1
    target_addr_len = sizeof(target_addr);DEBUG_1
    struct linger solinger = { 1, 0 };DEBUG_1
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger)) == -1) {
		perror("setsockopt(SO_LINGER)");DEBUG_1
	}
    ret = sendto(sock, data, data_len, flag, ( struct sockaddr*)&target_addr, target_addr_len);
    if(ret < 0)
    {
        printf("[DEBUG][Error][%d][%s] %s\n", __LINE__, __func__, "Not sendto()");DEBUG_1
        return (-1) * ((DOMAIN_CODE_CUMMNICATION + CUMMNICATION_LIST_CODE_UDS) + 0x300 + 0x01);DEBUG_1
    }else{
        char message[128];DEBUG_1
        sprintf(message, "Send success! Size is %d", ret);DEBUG_1
        printf("[DEBUG][Check][%d][%s] %s\n", __LINE__, __func__, message);DEBUG_1
        memset(message, 0x00, 128);DEBUG_1
    }
    close(sock);
    return ret;
}

extern int F_i_VH_Communication_UDS_Th_Create_Recvfrom(void *info)
{ 
    struct VH_UDS_Recv_Info_t *recv_info = (struct VH_UDS_Recv_Info_t*)info;

    void* (*th_fucntion)(void* d) = (void*(*)(void*))recv_info->th_func_ptr;


    struct VH_UDS_Client_info_t *client = (struct VH_UDS_Client_info_t*)recv_info->client;
    switch(client->direction)
    {
        default: break;
        case TRIFFIC_TO_SERVER:
        {
            recv_info->target_sock = F_i_VH_Communication_UDS_Socket_Bind(client->upstream_sockfd);
            break;
        }
        case TRIFFIC_TO_CLIENT:
        {
            recv_info->target_sock = F_i_VH_Communication_UDS_Socket_Bind(client->downstream_sockfd);
            break;
        }
    }
    pthread_t th_id;
    pthread_create(&th_id, NULL, th_fucntion, (void*)recv_info);
    *recv_info->recv_thid = th_id;
    pthread_join(th_id, NULL);
    return 0;    
}

extern int F_i_VH_Communication_UDS_Request(char *request_socket_path, char *response_socket_path, enum VH_UDS_Socket_Triffic_Direction_e *socket_direction, char *equip_name, size_t equip_name_len, struct VH_UDS_Response_t *out_uds_info)
{
    int ret;
    char *req_sock_path;
    char *res_sock_path;
    if(request_socket_path && response_socket_path){
        req_sock_path = request_socket_path;
        res_sock_path = response_socket_path;
    }else{
        if(socket_direction)
        {
            switch(*socket_direction)
            {
                default: break;
                case TRIFFIC_FROM_SIFM:
                case TRIFFIC_TO_SIFM:
                case TRIFFIC_TO_SERVER:
                {
                    req_sock_path = (char*)malloc(strlen(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME));
                    memcpy(req_sock_path, COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME ,sizeof(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME));
                    res_sock_path = (char*)malloc(strlen(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME));
                    memcpy(res_sock_path, COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME,sizeof(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME));
                    break;
                }
                case TRIFFIC_TO_CLIENT:
                case TRIFFIC_TO_SIDM:
                case TRIFFIC_FROM_SIDM:
                {
                    req_sock_path = (char*)malloc(strlen(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIDM_NAME "/" COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME));
                    memcpy(req_sock_path, COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIDM_NAME "/" COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME,sizeof(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME));
                    res_sock_path = (char*)malloc(strlen(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIDM_NAME "/" COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME));
                    memcpy(res_sock_path, COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIDM_NAME "/" COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME,sizeof(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME));
                    break;
                }
            }
        }else{
            printf("[DEBUG][Error][%d][%s] %s\n", __LINE__, __func__, "No Direction Info!");
            return (-1) * ((DOMAIN_CODE_CUMMNICATION + CUMMNICATION_LIST_CODE_UDS) + 9 + 1);
        }
    }
    struct VH_UDS_Request_t *req_data = (struct VH_UDS_Request_t*)malloc(sizeof(struct VH_UDS_Request_t));
    memcpy((char*)req_data->STX, COMUNICATIONS_UDS_DEFAULT_REQUEST_DATA_STX, 4);
    memcpy(req_data->equip_name, equip_name, equip_name_len);
    req_data->direction = *socket_direction;
    req_data->pid = getpid();

    ret = F_i_VH_Communication_UDS_Sendto((char*)req_data, sizeof(struct VH_UDS_Request_t), 0, req_sock_path);
    free(req_data);

    struct VH_UDS_Response_t res_data;
    out_uds_info->sock = F_i_VH_Communication_UDS_Socket_Bind(res_sock_path);
    int recv_timer = 3000 * 1000;
    struct timeval tv_timeo = { (int)(recv_timer/1e6), (recv_timer % 1000) * 1000}; 
    setsockopt( out_uds_info->sock , SOL_SOCKET, SO_RCVTIMEO, &tv_timeo, sizeof(struct timeval));
    ret = recvfrom(out_uds_info->sock , &res_data, sizeof(struct VH_UDS_Response_t), 0, NULL, 0);
    if(ret >= 0)
    {
        memcpy(out_uds_info, &res_data, sizeof(struct VH_UDS_Response_t) - sizeof(int));
    }
    return ret;
}

#ifdef _D_LIB_COMMON_LIST
    extern void *Th_v_VH_Communication_UDS_Respone(void *info)
#else
    extern void *Th_v_VH_Communication_UDS_Respone(void *info)
#endif
{ 
    int ret;
    struct VH_UDS_Info_t *UDS_info = (struct VH_UDS_Info_t*)info;
    struct VH_UDS_Info_Setup_t *UDS_info_setup = (struct VH_UDS_Info_Setup_t*)&UDS_info->info_setup;
    struct VH_UDS_Client_list_t *UDS_client_list = (struct VH_UDS_Client_list_t*)UDS_info->clients;

    int sock;
    char *req_sock_path;
    char *res_sock_path;
    
    switch(UDS_info_setup->server_type)
    {
        default: break;
        case UDS_SERVER_SIFM:
        {
            req_sock_path = (char*)malloc(strlen(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME));
            memcpy(req_sock_path, COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME ,sizeof(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME));
            res_sock_path = (char*)malloc(strlen(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME));
            memcpy(res_sock_path, COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME,sizeof(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME));
            break;
        }
        case UDS_SERVER_SIDM:
        {
            req_sock_path = (char*)malloc(strlen(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIDM_NAME "/" COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME));
            memcpy(req_sock_path, COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIDM_NAME "/" COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME,sizeof(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME));
            res_sock_path = (char*)malloc(strlen(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIDM_NAME "/" COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME));
            memcpy(res_sock_path, COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIDM_NAME "/" COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME,sizeof(COMUNICATIONS_UDS_DEFAULT_PATH "/" COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "/" COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME));
            break;
        }
    }
    sock = F_i_VH_Communication_UDS_Socket_Bind(req_sock_path);
    struct VH_UDS_Request_t *req_data = (struct VH_UDS_Request_t*)malloc(sizeof(struct VH_UDS_Request_t));
    while(1)
    {
        memset(req_data, 0x00, sizeof(struct VH_UDS_Request_t));

        ret = recvfrom(sock, (char*)req_data, sizeof(struct VH_UDS_Request_t), 0, NULL, 0);
        if(ret >= 0)
        {
            for(;;)
            {
                if(strncmp(req_data->STX, "REQs", 4) == 0)
                {
                    //continue;
                }else{
                    break;
                }
                char *client_name = (char*)malloc(sizeof(char) * strlen(req_data->equip_name));
                size_t client_name_len = strlen(req_data->equip_name) + sizeof("_to_server");
                memcpy(client_name, req_data->equip_name, strlen(req_data->equip_name));
                size_t target_sock_path_len = (strlen(req_sock_path) - sizeof(COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME)) + client_name_len;
                char *target_sock_path_temp = (char*)malloc(target_sock_path_len - client_name_len);
                char *target_sock_path = (char*)malloc(target_sock_path_len + client_name_len);
                char *signal_sock_path = (char*)malloc(target_sock_path_len + client_name_len);
                memcpy(target_sock_path_temp, req_sock_path, strlen(req_sock_path) - sizeof(COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME));
                
                switch(req_data->direction)
                {
                    default: break;
                    case TRIFFIC_TO_SERVER:
                    case TRIFFIC_TO_SIDM:
                    case TRIFFIC_TO_SIFM:
                    {
                        sprintf(target_sock_path, "%s/%s_to_server", target_sock_path_temp, client_name);
                        sprintf(signal_sock_path, "%s/server_to_%s", target_sock_path_temp, client_name);
                        break;
                    }
                    case TRIFFIC_TO_CLIENT:
                    case TRIFFIC_FROM_SIDM:
                    case TRIFFIC_FROM_SIFM:
                    {
                        sprintf(target_sock_path, "%s/server_to_%s", target_sock_path_temp, client_name);
                        sprintf(signal_sock_path, "%s/%s_to_server", target_sock_path_temp, client_name);
                        break;
                    }
                }
                free(target_sock_path_temp);
                ret = f_i_VH_Communication_UDS_Create_SockFD(target_sock_path, 0777);
                ret = f_i_VH_Communication_UDS_Create_SockFD(signal_sock_path, 0777);
                if(ret < 0)
                {
                    ret = F_i_VH_Communication_UDS_Socket_Bind(target_sock_path);

                }
                if(ret == 0)
                {
                    struct VH_UDS_Response_t res_data;
                    memcpy(res_data.STX, COMUNICATIONS_UDS_DEFAULT_RESPONSE_DATA_STX, 4);
                    memcpy(res_data.socketfd, target_sock_path, target_sock_path_len);
                    memcpy(res_data.signalfd, signal_sock_path, target_sock_path_len);
                    res_data.pid = getpid();
                    sleep(1);
                    ret = F_i_VH_Communication_UDS_Sendto((char*)&res_data, sizeof(struct VH_UDS_Response_t), 0, res_sock_path);
                    if(ret >= 0)
                    {
                        struct VH_UDS_Client_info_t *client_info = (struct VH_UDS_Client_info_t*)malloc(sizeof(struct VH_UDS_Client_info_t));
                        client_info->client_name = client_name;
                        switch(req_data->direction)
                        {
                            default: break;
                            case TRIFFIC_TO_SERVER:
                            case TRIFFIC_TO_SIDM:
                            case TRIFFIC_TO_SIFM:
                            {
                                client_info->direction = TRIFFIC_TO_SERVER;
                                client_info->state = REQUEST_FROM_CLIENT;
                                client_info->upstream_sockfd = target_sock_path;
                                client_info->upstream_sockfd_buff_size = COMUNICATIONS_UDS_DEFAULT_UPSTREAM_SOCKET_BUFF_SIZE;
                                client_info->downstream_sockfd = signal_sock_path;
                                client_info->downstream_sockfd_buff_size = COMUNICATIONS_UDS_DEFAULT_DOWNSTREAM_SOCKET_BUFF_SIZE/100;
                                
                                //client_info->downstream_sockfd = NULL;
                                break;
                            }
                            case TRIFFIC_TO_CLIENT:
                            case TRIFFIC_FROM_SIDM:
                            case TRIFFIC_FROM_SIFM:
                            {
                                client_info->direction = TRIFFIC_TO_CLIENT;
                                client_info->state = REQUEST_FROM_CLIENT;
                                client_info->downstream_sockfd = target_sock_path;
                                client_info->downstream_sockfd_buff_size = COMUNICATIONS_UDS_DEFAULT_DOWNSTREAM_SOCKET_BUFF_SIZE;
                                client_info->upstream_sockfd = signal_sock_path;
                                client_info->upstream_sockfd_buff_size = COMUNICATIONS_UDS_DEFAULT_UPSTREAM_SOCKET_BUFF_SIZE/100;
                                //client_info->upstream_sockfd  = NULL;
                                break;
                            }
                        }
                        #ifdef _D_LIB_COMMON_LIST
                            F_v_VH_Common_List_Add((struct VH_Common_List_t*)UDS_client_list , (void*)client_info);
                        #else
                            //TBD
                        #endif
                        client_info = NULL;
                        client_name = NULL;
                        target_sock_path = NULL;
                        signal_sock_path = NULL;
                    }else{
                         free(client_name);
                         free(target_sock_path);
                         free(signal_sock_path);
                    }
                }else{
                    free(target_sock_path);
                    free(signal_sock_path);
                }
                
                break;
            }
        }
    }

    close(sock);
    free(req_sock_path);
    free(res_sock_path);

    return NULL;    
}

extern int F_i_VH_Communication_UDS_Release_Client(struct VH_Common_List_t *UDS_client_list , void *client_info)
{
    if(UDS_client_list)
    {
        F_v_VH_Common_List_Del(UDS_client_list, client_info);
    }else{
        //Error Return
    }
    
    return 0;
}

static int f_i_VH_Communication_UDS_Create_SockFD(char *path, int force)
{
    int ret = open(path, O_RDWR | O_CREAT | O_NONBLOCK, force);
    if(ret == -1)
    {
        printf("[DEBUG][Error][%d][%s] %s\n", __LINE__, __func__, "Error open()");
        return (-1) * ((DOMAIN_CODE_CUMMNICATION + CUMMNICATION_LIST_CODE_UDS) + 9 + 01);
    }else{
        close(ret);
        ret = access(path, F_OK);
        if(ret == 0)
            unlink(path);
    }
    
    return 0;
}

static int f_i_VH_Communication_UDS_Rmdirs(const char *path, int force)
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
            if(f_i_VH_Communication_UDS_Rmdirs(filename, force) == -1 && !force) 
            {
                return (-1) * ((DOMAIN_CODE_CUMMNICATION + CUMMNICATION_LIST_CODE_UDS) + 10 + 01);
            }
        } else if(S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) 
        { 
            if(unlink(filename) == -1 && !force) 
            {
                return (-1) * ((DOMAIN_CODE_CUMMNICATION + CUMMNICATION_LIST_CODE_UDS) + 10 + 02);
            }
        }
    }
    closedir(dir_ptr);
    
    return rmdir(path);
}



