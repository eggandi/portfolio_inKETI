#include <V2XHO_Agent.h>



int main(int argc, char **argv)
{
    g_time_tick = malloc(sizeof(struct V2XHO_Time_Tick_t));
    pthread_t timer_th;
    pthread_create(&timer_th, NULL, F_th_V2XHO_Epoll_Time_Tick, (void *)g_time_tick);

    struct V2XHO_Agent_Info_t *agent_info = malloc(sizeof(struct V2XHO_Agent_Info_t));
    if (argc < 2)
    {
        agent_info->type = HomeAgent;
    }else if(argc == 2){
        char *now_argv = *(argv + 1);
        if(strcmp("foreign", now_argv) == 0)
        {
            agent_info->type = ForeignAgent;
        }
    }

    agent_info->interface_mobileip = malloc(sizeof(char) *  128);
    sprintf(agent_info->interface_mobileip, "%s", DEFAULT_MOBILEIP_INTERFACE_NAME);
    agent_info->interface_common = malloc(sizeof(char) *  128);
    sprintf(agent_info->interface_common, "%s", DEFAULT_COMMON_INTERFACE_NAME);
    pthread_mutex_init(&agent_info->task_info.mutex_id, NULL);
    pthread_cond_init(&agent_info->task_info.cond_id, NULL);
    printf("[DEBUG][%s][%d]\n", __func__, __LINE__);
    pthread_create(&agent_info->task_info.th_id, NULL, F_th_V2XHO_Agent_Task_Doing, (void *)agent_info);
    printf("[DEBUG][%s][%d]\n", __func__, __LINE__);

    while(1) sleep(1);

    if(agent_info->interface_mobileip)
        free(agent_info->interface_mobileip);
    if(agent_info->interface_common)
        free(agent_info->interface_common);
    return 1;
}