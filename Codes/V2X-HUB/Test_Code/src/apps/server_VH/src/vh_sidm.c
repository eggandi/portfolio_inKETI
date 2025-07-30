#include "vh_main.h"
#include "vh_sidm.h"

#define DEBUG_1 printf("[DEBUG][%s][%d]\n", __func__, __LINE__);
static void *th_v_VH_SIDM_UDS_Sender_Task(void* d);

extern void *Th_v_VH_SIDM_UDS_Task(void *d)
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
        info->server_type = UDS_SERVER_SIDM;DEBUG_1

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

    pthread_t th_uds_send[MAX_CLIENT_NUM];DEBUG_1
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
                    case RESPONES_TO_CLIENT:
                    {
                        uds_info->clients->client->state = WAITING_SENDER_TASK;DEBUG_1
                        break;DEBUG_1
                    }
                    case WAITING_SENDER_TASK:
                    {
                        pthread_create(&th_uds_send[th_num], NULL, th_v_VH_SIDM_UDS_Sender_Task, (void*)uds_info->clients->client);
                        pthread_detach(th_uds_send[th_num]);DEBUG_1
                        th_num++;DEBUG_1
                        break;DEBUG_1
                    }
                    case WORKING_SENDER_TASK:
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


static void *th_v_VH_SIDM_UDS_Sender_Task(void* d)
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
    

    int Connection_Check = 0;DEBUG_1
    time_t prev_send_time = time(NULL);DEBUG_1
    int no_connect_timer_check_5s = 0;DEBUG_1
    while(1)
    {
        client->state = WORKING_SENDER_TASK;DEBUG_1
        struct VH_SHM_Data_Send_Protocol_t pop_data;DEBUG_1
        if(*part->part_state == SHM_PART_UNUSE)
        {
            *part->part_state = SHM_PART_INUSE;DEBUG_1
            bool only_read = false;DEBUG_1
            (*part->Data_Pop)(part, (void*)&pop_data, &only_read);DEBUG_1
            
            *part->part_state = SHM_PART_UNUSE;DEBUG_1
        }
        no_connect_timer_check_5s = time(NULL) - prev_send_time;DEBUG_1
        printf("no_connect_timer_check_5s:%d\n", no_connect_timer_check_5s);
        if(pop_data.data_len > 0 && pop_data.payload_ptr != NULL)
        {
            ret = F_i_VH_Communication_UDS_Sendto((char*)pop_data.payload_ptr, pop_data.data_len, 0, client->downstream_sockfd);
            free(pop_data.payload_ptr);DEBUG_1
        }
        if(ret > 0)
        {
            printf("ret:%d\n", ret);
            no_connect_timer_check_5s = 0;DEBUG_1
            prev_send_time = time(NULL);DEBUG_1
        }

        if(no_connect_timer_check_5s > 5)
        {
            break;DEBUG_1
        }
        
    }
    return NULL;DEBUG_1
}