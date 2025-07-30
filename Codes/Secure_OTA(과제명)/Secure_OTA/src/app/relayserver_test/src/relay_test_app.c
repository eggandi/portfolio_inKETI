#include <librelayserver.h>

#define DEBUG_1 //printf("[DEBUG][%s][%d]\n", __func__, __LINE__);
#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))


enum debug_lever_e G_Debug_Level;
struct ticktimer_t G_TickTimer;
struct clients_info_t G_Clients_Info;

struct Memory_Used_Info_t G_Memory_Used_Info;
struct Memory_Used_Data_Info_t G_Data_Info;

uint8_t *G_HTTP_Request_Info_Program;
uint8_t *G_HTTP_Request_Info_Fireware;

int main(int argc, char* argv[])
{
    int ret_err;
    G_Clients_Info.connected_client_num = malloc(sizeof(int));
    G_Memory_Used_Info = F_s_Memory_Initial(1024 * 10);
    G_Data_Info = F_s_Memory_Data_Distributtion(G_Memory_Used_Info, 1024 * 7, &ret_err);
    G_Data_Info.type = OCTET_STRING;

    pthread_create(&(G_TickTimer.Th_TickTimer), NULL, Th_i_RelayServer_TickTimer, NULL);
    pthread_detach(G_TickTimer.Th_TickTimer);

    if(argc > 2 && atoi(argv[1]) == 1)
    {
        struct http_info_t http_info;
        http_info.Request_Line.Method = "POST";
        http_info.Request_Line.To = DEFAULT_HTTP_SERVER_PROGRAM_URL;
        http_info.Request_Line.What = "HTTP";
        http_info.Request_Line.Version = "1.0";
        http_info.HOST = DEFAULT_HTTP_HOST;
        http_info.PORT = "80";
        http_info.ACCEPT = "*/*";
        http_info.CONTENT_TYPE = "Content-Type: multipart/form-data; boundary=" DEFAULT_HTTP_FROMDATA_BOUNDARY;

        G_HTTP_Request_Info_Program = malloc(sizeof(uint8_t) * DEFAULT_HTTP_INFO_SIZE);
        G_HTTP_Request_Info_Fireware = malloc(sizeof(uint8_t) * DEFAULT_HTTP_INFO_SIZE);

        F_i_RelayServer_HTTP_Initial(G_HTTP_Request_Info_Program, &http_info);
        http_info.Request_Line.To = DEFAULT_HTTP_SERVER_PROGRAM_URL;
        F_i_RelayServer_HTTP_Initial(G_HTTP_Request_Info_Fireware, &http_info);

        int port = 50000;
        struct socket_info_t Server_Info = F_s_RelayServer_TcpIp_Initial_Server("eth1", &port, &ret_err);
        F_i_RelayServer_TcpIp_Task_Run(&Server_Info);

        pthread_t Task_ID_Job;
        pthread_create(&Task_ID_Job, NULL, Th_RelayServer_Job_Task, (void*)&G_Data_Info);
        pthread_detach(Task_ID_Job);
    }
    pthread_t Task_ID_NOBO;
    struct NUVO_recv_task_info_t nubo_task;

    if(argc > 3 && atoi(argv[2]) == 1)
    {
        clear();
        gotoxy(1,0);
        printf("[DRIVING HISTORY] START NUVO Job Task.\n");
        pthread_create(&Task_ID_NOBO, NULL, Th_RelayServer_NUVO_Client_Task, (void*)&nubo_task);
        pthread_detach(Task_ID_NOBO);
    }

    if(argc > 3 && atoi(argv[3]) == 1)
    {
        pthread_t Task_ID_V2X;
        pthread_create(&Task_ID_V2X, NULL, Th_RelayServer_V2X_UDP_Task, (void*)NULL);
        pthread_detach(Task_ID_V2X);
    }

    while(1)
    {
        if(pthread_kill(Task_ID_NOBO, 0) == 0)
        {
            if(nubo_task.task_info_state == NULL)
            {
                pthread_cancel(Task_ID_NOBO);
                pthread_create(&Task_ID_NOBO, NULL, Th_RelayServer_NUVO_Client_Task, (void*)&nubo_task);
                pthread_detach(Task_ID_NOBO);
            }
        }else{
            if(nubo_task.task_info_state == NULL)
            {
                pthread_cancel(Task_ID_NOBO);
                pthread_create(&Task_ID_NOBO, NULL, Th_RelayServer_NUVO_Client_Task, (void*)&nubo_task);
                pthread_detach(Task_ID_NOBO);
            }
        }
        sleep(1);
        
        //ret = sendto(nubo_info->sock , send_buf, 11, 0, (struct sockaddr*)&nubo_info->serv_adr, sizeof(nubo_info->serv_adr));
        sleep(1);
    }


    return 0;
}
