#include "vh_main.h"
#include "vh_sifm.h"

#define DEBUG_1 printf("[DEBUG][%s][%d]\n", __func__, __LINE__);

static void *th_v_VH_SIFM_UDS_Receiver_Task(void* d);

extern void *Th_v_VH_SIFM_UDS_Task(void *d)
{
    int ret;DEBUG_1
    struct VH_UDS_Info_t *uds_info;DEBUG_1
    if(d)
    {
        uds_info = (struct VH_UDS_Info_t*)d;DEBUG_1
    }else{
        uds_info = (struct VH_UDS_Info_t*)malloc(sizeof(struct VH_UDS_Info_t));DEBUG_1
        uds_info->list_used = (bool*)malloc(sizeof(bool));DEBUG_1
        uds_info->clients = (struct VH_UDS_Client_list_t *)malloc(sizeof(struct VH_UDS_Client_list_t));DEBUG_1
        uds_info->clients->client_num = (uint32_t*)malloc(sizeof(uint32_t));DEBUG_1
        struct VH_UDS_Info_Setup_t *info = (struct VH_UDS_Info_Setup_t*)&(uds_info->info_setup);DEBUG_1

        info->path_type = UDS_PATH_TYPE_RAMDISK;DEBUG_1
        info->server_type = UDS_SERVER_SIFM;DEBUG_1

    #ifndef _D_USING_DEFAULT_VALUE
        info->socket_path;DEBUG_1
        info->request_socket_name;DEBUG_1
        info->request_socket_buff_size;DEBUG_1
        info->response_socket_name;DEBUG_1
        info->response_socket_buff_size;DEBUG_1
    #endif

        ret = F_i_VH_Communication_UDS_Server_Setup(info);DEBUG_1
        if(ret < 0)
        {
            printf("Error Code : 0x%08X\n", ret);DEBUG_1
        }
    }
    
    pthread_t th_uds_respone;DEBUG_1
    pthread_create(&th_uds_respone, NULL, Th_v_VH_Communication_UDS_Respone, (void*)uds_info);DEBUG_1
    pthread_detach(th_uds_respone);DEBUG_1

    pthread_t th_uds_recv[MAX_CLIENT_NUM];DEBUG_1
    uint32_t th_num = 0;DEBUG_1

    while(1)    
    {
        uint32_t while_block_counter = 0;DEBUG_1
        if(*uds_info->clients->client_num > 0)
        {
            printf("*uds_info->clients->client_num:%d\n", *uds_info->clients->client_num);DEBUG_1
            while(uds_info->clients->prev)
            {
                uds_info->clients = uds_info->clients->prev;DEBUG_1
                if(while_block_counter > *uds_info->clients->client_num)
                {
                    break;DEBUG_1
                }
                while_block_counter++;DEBUG_1
            }  
            while_block_counter = 0;DEBUG_1
            
            for(uint32_t k = 0; k < *uds_info->clients->client_num; k++)
            {
                switch(uds_info->clients->client->state)
                {
                    default: break;DEBUG_1
                    case REQUEST_FROM_CLIENT:
                    {
                        uds_info->clients->client->state = WAITING_RECEIVER_TASK;DEBUG_1
                        break;DEBUG_1
                    }
                    case WAITING_RECEIVER_TASK:
                    {
                        pthread_create(&th_uds_recv[th_num], NULL, th_v_VH_SIFM_UDS_Receiver_Task, (void*)uds_info->clients->client);DEBUG_1
                        pthread_detach(th_uds_recv[th_num]);DEBUG_1
                        th_num++;DEBUG_1
                        break;DEBUG_1
                    }
                    case WORKING_RECEIVER_TASK:
                    {
                        break;DEBUG_1
                    }
                }
                if(uds_info->clients->next)
                    uds_info->clients = uds_info->clients->next;DEBUG_1

                if(while_block_counter > *uds_info->clients->client_num)
                {
                    break;DEBUG_1
                }
                while_block_counter++;DEBUG_1
            }
        }
        usleep(500 * 1000);DEBUG_1
    }
    return  0;DEBUG_1
}

static void *th_v_VH_SIFM_UDS_Receiver_Task(void* d)
{
    struct VH_UDS_Client_info_t *client = (struct VH_UDS_Client_info_t*)d;DEBUG_1

    int ret = 0;DEBUG_1
   
    int shmid = shmget((key_t)G_shm_key, 0, 0);    
    struct VH_SHM_Info_t *shm_info = (struct VH_SHM_Info_t*)shmat(shmid, 0, 0);DEBUG_1
    struct VH_SHM_Part_Info_t *part;DEBUG_1
    enum VH_SHM_Part_Type_e part_type =  F_e_VH_SHM_Part_Type_Select((char*)client->client_name);DEBUG_1
    
    switch(0 <= part_type ? (part_type <= SHM_PART_TYPE_MODULE_NAME_9 ? true : false) : false)
    {
        default:break;
        case true:
        {
            part = (struct VH_SHM_Part_Info_t*)*shm_info->part_list[part_type]->part;DEBUG_1
            *part->part_type = part_type;DEBUG_1
            break;DEBUG_1
        }
        case false:
        {
            DEBUG_1
            //error//
            return NULL;DEBUG_1
        }
    }
    
    int sock;DEBUG_1
    printf("client->client_name:%s\n", client->client_name);DEBUG_1
    printf("client->direction:%d\n", client->direction);DEBUG_1
    printf("client->state:%d\n", client->state);DEBUG_1
    if(client->upstream_sockfd != NULL)
    {
        printf("client->upstream_sockfd:%s\n", client->upstream_sockfd);DEBUG_1
        printf("client->upstream_sockfd_buff_size:%d\n", client->upstream_sockfd_buff_size);DEBUG_1
        sock = F_i_VH_Communication_UDS_Socket_Bind(client->upstream_sockfd);DEBUG_1
        if(sock > 0)
        {
            struct timeval sock_tv;                  /* Socket Send/Recv Block Timer */      
            sock_tv.tv_sec = (int)(1000 / 1000);DEBUG_1
            sock_tv.tv_usec = (1000 % 1000) * 1000; 
            if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&sock_tv, sizeof(struct timeval)) == SO_ERROR) {
                perror("setsockopt(SO_RCVTIMEO)");DEBUG_1
            }
        }
    }
    if(client->downstream_sockfd != NULL){
        printf("client->downstream_sockfd:%s\n", client->downstream_sockfd);DEBUG_1
        printf("client->downstream_sockfd_buff_size:%d\n", client->downstream_sockfd_buff_size);DEBUG_1
    }
    int Connection_Check = 0;DEBUG_1
    time_t prev_recv_time = time(NULL);DEBUG_1
    int no_connect_timer_check_5s = 0;DEBUG_1
    while(1)
    {
        char recv_buf[10240] = {0,};DEBUG_1
        client->state = WORKING_RECEIVER_TASK;DEBUG_1
        ret = recvfrom(sock , recv_buf, 10240, 0, NULL, 0);DEBUG_1
        no_connect_timer_check_5s = time(NULL) - prev_recv_time;DEBUG_1
        printf("no_connect_timer_check_5s:%d\n", no_connect_timer_check_5s);
        if(ret > 0)
        {
            printf("ret:%d\n", ret);
            no_connect_timer_check_5s = 0;DEBUG_1
            prev_recv_time = time(NULL);DEBUG_1

            switch(part_type)
            {
                default:
                { 
                    if(*part->part_state == SHM_PART_UNUSE)
                    {
                        *part->part_state = SHM_PART_INUSE;DEBUG_1
                        bool foraced = true;DEBUG_1
                        struct VH_SHM_Data_Send_Protocol_t push_data;DEBUG_1
                        push_data.data_type = SHM_PART_TYPE_SAVE_TO_BUFF;DEBUG_1
                        push_data.data_state = SHM_DATA_UNUSE;DEBUG_1
                        push_data.data_len = 0;DEBUG_1
                        push_data.payload_ptr = malloc(sizeof(char) * ret);DEBUG_1
                        memcpy(push_data.payload_ptr, recv_buf, ret);DEBUG_1
                        (*part->Data_Push)(part, (void*)&push_data, &foraced);DEBUG_1
                        free(push_data.payload_ptr);DEBUG_1
                        *part->part_state = SHM_PART_UNUSE;DEBUG_1
                    }
                    break;
                }
                case SHM_PART_TYPE_MODULE_DTG_0:
                {
                    for(int j = 0; j <= ret; j++)
                    {
                        printf("%02X ", ((char*)recv_buf)[j]);
                    }
                    printf("\n");
                    if(recv_buf[0] == 0x02 && recv_buf[1] == 0x50)
                    {
                        char *hdr_send_signal = "HDR!";
                        F_i_VH_Communication_UDS_Sendto(hdr_send_signal, 4, 0, client->downstream_sockfd);
                        printf("client->upstream_sockfd:%s\n", client->downstream_sockfd);
                    }else{
                        if(*part->part_state == SHM_PART_UNUSE)
                        {
                            *part->part_state = SHM_PART_INUSE;DEBUG_1
                            bool foraced = true;DEBUG_1
                            struct VH_SHM_Data_Send_Protocol_t push_data;DEBUG_1
                            push_data.data_type = SHM_PART_TYPE_SAVE_TO_BUFF;DEBUG_1
                            push_data.data_state = SHM_DATA_UNUSE;DEBUG_1
                            push_data.data_len = 0;DEBUG_1
                            push_data.payload_ptr = malloc(sizeof(char) * ret);DEBUG_1
                            memcpy(push_data.payload_ptr, recv_buf, ret);DEBUG_1
                            (*part->Data_Push)(part, (void*)&push_data, &foraced);DEBUG_1
                            free(push_data.payload_ptr);DEBUG_1
                            *part->part_state = SHM_PART_UNUSE;DEBUG_1
                        }
                    }
                    break;
                }
            }
            
        }
        if(no_connect_timer_check_5s > 5)
        {
            break;DEBUG_1
        }
        
    }
    close(sock);DEBUG_1
    return NULL;DEBUG_1
}

extern enum VH_SHM_Part_Type_e F_e_VH_SHM_Part_Type_Select(char *client_name)
{
    int ret;
    int ptr_now = 0;
    enum VH_SHM_Part_Type_e part_type;
    bool ptr_check_is = false;
    while(1) 
    {
ptr_char_check_ok:
        char *ptr_char = (char*)client_name + ptr_now;
        ptr_now++;
        switch(*ptr_char)
        {
            default:
            {
                if(ptr_check_is == true)
                {
                    printf("[DEBUG][Error]The client name is not support. %s\n", client_name);
                    goto ptr_char_check_error;
                }
                break;
            }
            case 'D':
            {
                switch(part_type)
                {
                    default:break;
                    case SHM_PART_TYPE_MODULE_DTG_0:
                    {
                        part_type = SHM_PART_TYPE_MODULE_DTG_0;
                        ptr_check_is = true;
                        goto ptr_char_check_ok;
                        break;
                    }
                    case SHM_PART_TYPE_MODULE_ADS_0:
                    {
                        part_type = part_type;
                        ptr_check_is = true;
                        goto ptr_char_check_ok;
                        break;
                    }
                }
                break;
            }
            case 'T':
            {
                part_type = SHM_PART_TYPE_MODULE_DTG_0;
                ptr_check_is = true;
                goto ptr_char_check_ok;
            }
            case 'G':
            {
                part_type = SHM_PART_TYPE_MODULE_DTG_0;
                ptr_check_is = true;
                break;
            }
            case 'M':
            {
                switch(part_type)
                {
                    default:break;
                    case SHM_PART_TYPE_MODULE_DMS_0:
                    case SHM_PART_TYPE_MODULE_HMI_0:
                    {
                        part_type = part_type;
                        ptr_check_is = true;
                        goto ptr_char_check_ok;
                        break;
                    }
                }
                break;
            }  
            case 'S':
            {
                switch(part_type)
                {
                    default:break;
                    case SHM_PART_TYPE_MODULE_DMS_0:
                    {
                        part_type = part_type;
                        ptr_check_is = true;
                        break;
                    }
                    case SHM_PART_TYPE_MODULE_ADS_0:
                    {
                        part_type = part_type;
                        ptr_check_is = true;
                        break;
                    }
                }
                break;
            }
            case 'A':
            {
                part_type = SHM_PART_TYPE_MODULE_ADS_0;
                ptr_check_is = true;
                goto ptr_char_check_ok;
            }
            case 'H':
            {
                part_type = SHM_PART_TYPE_MODULE_HMI_0;
                ptr_check_is = true;
                goto ptr_char_check_ok;
            }

                case 'I':
                {
                    part_type = SHM_PART_TYPE_MODULE_HMI_0;
                    ptr_check_is = true;
                    break;
                }
        }
        if(strlen(client_name) <= ptr_now)
        {
            break;
        }
    }

    return part_type;
ptr_char_check_error:
    return -1;
}