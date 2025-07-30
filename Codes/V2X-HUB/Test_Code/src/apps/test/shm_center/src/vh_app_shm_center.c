#include "vh_app_shm_center.h"
#include <string.h>
#define DEBUG_1 printf("[DEBUG][%s][%d]\n", __func__, __LINE__);
int main()
{
    key_t key_value = 0;
    int ret = 0;
    struct VH_SHM_Info_t *shm_info = (struct VH_SHM_Info_t*)F_i_VH_Data_SHM_Initial(&key_value);
    if(!shm_info)
    {
        return 0;
    }else{

    }

    shmdt(shm_info);
    sleep(1);
    int shmid = shmget((key_t)key_value, 0, 0);    
    struct VH_SHM_Info_t *shm_info_1 = (struct VH_SHM_Info_t*)shmat(shmid, 0, 0);
    struct VH_SHM_Part_Info_t *part = (struct VH_SHM_Part_Info_t*)*shm_info_1->part_list[4]->part;
    *part->part_type = SHM_PART_TYPE_MODULE_NAME_0;    

    while(1)
    {
        if(*part->part_state == SHM_PART_UNUSE)
        {
            bool foraced = false;
            #define INPUT_DUMP "AB"
            char *input_data = malloc(sizeof("AAABBB"));
            struct VH_SHM_Data_Send_Protocol_t push_data;
            push_data.data_type = SHM_PART_TYPE_SAVE_TO_BUFF;
            push_data.data_state = SHM_DATA_INUSE;
            push_data.data_len = 0;
            int rand_num = rand() % 20 + 1;
            push_data.payload_ptr = malloc((sizeof(char) * 2) * (rand_num + 2));
            for(int  i = 0 ; i < rand_num; i++)
            {
                push_data.data_len += 2;
                strcat(push_data.payload_ptr, INPUT_DUMP);
            }
            (*part->Data_Push)(part, (void*)&push_data, &foraced);
            
        }
        usleep(100 * 1000);
    }

    return ret;
}