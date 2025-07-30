#include "vh_main.h"
#include "vh_sifm.h"
#include "vh_sidm.h"

#define DEBUG_1 printf("[DEBUG][%s][%d]\n", __func__, __LINE__);

static void f_v_VH_Main_Exit(void *shm_addr);
static struct VH_UDS_Info_t *f_s_VH_Main_UdsInfo_Initial(struct VH_UDS_Info_Setup_t *Setup_Info);

key_t G_shm_key = 0;

int main()
{
    int ret = 0;
    struct VH_SHM_Info_t *shm_info = (struct VH_SHM_Info_t*)F_i_VH_Data_SHM_Initial(&G_shm_key);
    if(!shm_info)
    {
        return 0;
    }else{

    }
    shmdt(shm_info);
    sleep(1);
    int shmid = shmget((key_t)G_shm_key, 0, 0);    
    struct VH_SHM_Info_t *shm_info_1 = (struct VH_SHM_Info_t*)shmat(shmid, 0, 0);

    for(int i = 0; i < 0; i++)
    {
        //printf(":%p, ", shm_info_1->part_list[i]->part);
        //printf(":%p\n", *shm_info_1->part_list[i]->part);
        struct VH_SHM_Part_Info_t *part = (struct VH_SHM_Part_Info_t*)*shm_info_1->part_list[i]->part;
        printf(":%d, ", *shm_info_1->part_state[i]);
        printf(":%p, ", shm_info_1->part_state[i]);
        printf(":%p\n", part->part_state);
        printf(":%ld, ", *shm_info_1->part_inuse_size[i]);
        printf(":%p, ", shm_info_1->part_inuse_size[i]);
        printf(":%p\n", part->inused_data_size);
    }
    struct VH_UDS_Info_t *uds_info = f_s_VH_Main_UdsInfo_Initial(NULL);
    pthread_t SIMF_TASK_ID;
    pthread_create(&SIMF_TASK_ID, NULL, Th_v_VH_SIFM_UDS_Task, (void*)uds_info);
    pthread_detach(SIMF_TASK_ID);     

    pthread_create(&SIMF_TASK_ID, NULL, Th_v_VH_SIDM_UDS_Task, (void*)uds_info);
    pthread_detach(SIMF_TASK_ID);     
    while(1)
    {
        usleep(1000 * 1000);
    }

    return ret;
}

static struct VH_UDS_Info_t *f_s_VH_Main_UdsInfo_Initial(struct VH_UDS_Info_Setup_t *Setup_Info)
{
    int ret;
    struct VH_UDS_Info_t *uds_info = (struct VH_UDS_Info_t*)malloc(sizeof(struct VH_UDS_Info_t));
    uds_info->list_used = (bool*)malloc(sizeof(bool));
    uds_info->clients = (struct VH_UDS_Client_list_t *)malloc(sizeof(struct VH_UDS_Client_list_t));
    uds_info->clients->client_num = (uint32_t*)malloc(sizeof(uint32_t));
    struct VH_UDS_Info_Setup_t *info = (struct VH_UDS_Info_Setup_t*)&(uds_info->info_setup);

    if(Setup_Info)
    {
        info = Setup_Info;
    }else{
        info = (struct VH_UDS_Info_Setup_t*)&(uds_info->info_setup);
        info->path_type = UDS_PATH_TYPE_RAMDISK;
        info->server_type = UDS_SERVER_SIFM;

    #ifndef _D_USING_DEFAULT_VALUE
        info->socket_path;
        info->request_socket_name;
        info->request_socket_buff_size;
        info->response_socket_name;
        info->response_socket_buff_size;
    #endif
    }

    ret = F_i_VH_Communication_UDS_Server_Setup(info);
    if(ret < 0)
    {
        printf("Error Code : 0x%08X\n", ret);
    }
    return uds_info;
}

static void f_v_VH_Main_Exit(void *shm_addr)
{   
    int ret;

    int shm_id = shmget((key_t)G_shm_key, 0, 0);    
    ret = shmdt(shm_addr);
    ret = shmctl(shm_id, IPC_RMID, 0);
    
    if(ret < 0)
    {
        struct shmid_ds shm_info;
        shmctl(shm_id, IPC_STAT, &shm_info);
        printf("[DEBUG][Error]shm_info.shm_nattch:%ld\n", shm_info.shm_nattch);
        return;
    }

    return;
}