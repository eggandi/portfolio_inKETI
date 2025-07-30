#include "vh_blender.h"

#define DEBUG_1 //printf("[DEBUG][%s][%d]\n", __func__, __LINE__);

#define _D_USING_DEFAULT_VALUE
#define MAX_CLIENT_NUM 5

void *th_v_VH_Application_Blender_UDS_Receiver_task(void* d);

int main()
{
    int ret;DEBUG_1
    struct VH_UDS_Info_t *uds_info = (struct VH_UDS_Info_t*)malloc(sizeof(struct VH_UDS_Info_t));DEBUG_1
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
    pthread_t th_uds_respone;DEBUG_1
    pthread_create(&th_uds_respone, NULL, Th_v_VH_Communication_UDS_Respone, (void*)uds_info);DEBUG_1
    pthread_detach(th_uds_respone);DEBUG_1

    pthread_t th_uds_recv[MAX_CLIENT_NUM];DEBUG_1
    uint32_t th_num = 0;DEBUG_1

    while(1)    
    {
        uint32_t while_block_counter = 0;DEBUG_1
        if(1)//(*uds_info->clients->client_num > 0)
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
            
            for(int k = 0; k <*uds_info->clients->client_num; k++)
            {
                switch(uds_info->clients->client->state)
                {
                    default: break;DEBUG_1
                    case REQUEST_FROM_CLIENT:
                    {
                        uds_info->clients->client->state = WAITING_RECEIVER_TASK;DEBUG_1
                        break;
                    }
                    case WAITING_RECEIVER_TASK:
                    {
                        pthread_create(&th_uds_recv[th_num], NULL, th_v_VH_Application_Blender_UDS_Receiver_task, (void*)uds_info);DEBUG_1
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

void *th_v_VH_Application_Blender_UDS_Receiver_task(void* d)
{
    struct VH_UDS_Info_t *uds_info  = (struct VH_UDS_Info_t*)d;DEBUG_1
    struct VH_UDS_Client_info_t *client = uds_info->clients->client;DEBUG_1
    key_t key_value = 0;DEBUG_1
    int ret = 0;DEBUG_1
    struct VH_SHM_Info_t *shm_info = (struct VH_SHM_Info_t*)F_i_VH_Data_SHM_Initial(&key_value);DEBUG_1
    if(!shm_info)
    {
        return 0;DEBUG_1
    }else{

    }
    shmdt(shm_info);DEBUG_1
    int shmid = shmget((key_t)key_value, 0, 0);    
    struct VH_SHM_Info_t *shm_info_1 = (struct VH_SHM_Info_t*)shmat(shmid, 0, 0);DEBUG_1
    struct VH_SHM_Part_Info_t *part = (struct VH_SHM_Part_Info_t*)*shm_info_1->part_list[4]->part;DEBUG_1
    *part->part_type = SHM_PART_TYPE_MODULE_NAME_0;    
    
    int sock;

    printf("client->client_name:%s\n", client->client_name);DEBUG_1
    printf("client->direction:%d\n", client->direction);DEBUG_1
    printf("client->state:%d\n", client->state);DEBUG_1
    if(client->upstream_sockfd != NULL)
    {
        printf("client->upstream_sockfd:%s\n", client->upstream_sockfd);DEBUG_1
        printf("client->upstream_sockfd_buff_size:%d\n", client->upstream_sockfd_buff_size);DEBUG_1
        sock = F_i_VH_Communication_UDS_Socket_Bind(client->upstream_sockfd);
    }else if(client->downstream_sockfd != NULL){
        printf("client->downstream_sockfd:%s\n", client->downstream_sockfd);DEBUG_1
        printf("client->downstream_sockfd_buff_size:%d\n", client->downstream_sockfd_buff_size);DEBUG_1
    }
    while(1)
    {
        char buf[10240];
        uds_info->clients->client->state = WORKING_RECEIVER_TASK;DEBUG_1
        ret = recvfrom(sock , &buf, 10240, 0, NULL, 0);
        if(ret > 0)
        {
            for(int  i = 0; i < ret; i++)
            {
                printf("%02X", buf[i]);
                if(i % 64 == 63)
                {
                    printf("\n");
                }
            }
            if(*part->part_state == SHM_PART_UNUSE)
            {
                bool foraced = true;
                struct VH_SHM_Data_Send_Protocol_t push_data;
                push_data.data_type = SHM_PART_TYPE_SAVE_TO_BUFF;
                push_data.data_state = SHM_DATA_INUSE;
                push_data.data_len = 0;
                push_data.payload_ptr = malloc(sizeof(char) * ret);
                memcpy(push_data.payload_ptr, buf, ret);
                (*part->Data_Push)(part, (void*)&push_data, &foraced);
                free(push_data.payload_ptr);
            }
        }
        sleep(1);DEBUG_1
    }
    return NULL;DEBUG_1
}