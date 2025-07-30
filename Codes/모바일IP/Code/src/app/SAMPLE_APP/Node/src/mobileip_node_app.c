#include <V2XHO_Node.h>

int main()
{
    g_time_tick = malloc(sizeof(struct V2XHO_Time_Tick_t));
    pthread_t timer_th;
    pthread_create(&timer_th, NULL, F_th_V2XHO_Epoll_Time_Tick, (void *)g_time_tick);

    struct V2XHO_Node_Info_t *node_info = malloc(sizeof(struct V2XHO_Node_Info_t));
    node_info->interface = malloc(sizeof(char) *  128);
    sprintf(node_info->interface, "%s","eth0");
    pthread_mutex_init(&node_info->task_info.mutex_id, NULL);
    pthread_cond_init(&node_info->task_info.cond_id, NULL);
    pthread_create(&node_info->task_info.th_id, NULL, F_th_V2XHO_Node_Task_Doing, (void *)node_info);

    while(1) sleep(1);

    return 1;
}