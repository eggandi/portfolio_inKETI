#include "main.h"
#include "config.h"
#include "buffer.h"
#include "pro_pcap.h"
#include "pro_v2x.h"
#include "gnss.h"

#define _DEBUG_LOG printf("[DEBUG][%d][%s]\n", __LINE__, __func__);

static void PRO_Main_Signal_Set();

int g_sockfd;
struct sockaddr_in g_v2x_addr;
pthread_t g_thread_gnss;
pthread_t g_thread_sdcard_active_mount;
int main(int argc, char *argv[])
{

    int ret;
    ret = PRO_Config_Pasrsing_Argument(argc, argv);

    ret = PRO_Config_Setup_Configuration_Read(&G_pro_config);
    if(ret < 0)
    {
        printf("Configuration read failed.\n");
        return -1;
    }

    PRO_Main_Signal_Set();

    if(G_pro_config.utils.mount.active_mode == true)
    {
        pthread_create(&g_thread_sdcard_active_mount, NULL, PRO_Utils_Mount_Active_Mode, NULL);
        pthread_detach(g_thread_sdcard_active_mount);
        printf("\033[1;32m");
        printf("\nIt's currently in SD card mount active mode.\n");
        printf("If SD card is not inserted, please will do the SD card, thank you...\n");
        printf("If remove the SD card, the application will be terminated.\n\n");
        printf("\033[0m");
        int wait_time = 0;
        while(PRO_Utils_Mount_Device_Check(G_pro_config.utils.mount.device) != 0)
        {
            printf("waiting for SD card mount... %d\n", wait_time++);
            sleep(1);
        }
    }else{
        if(G_pro_config.utils.mount.enable == true)
        {
            PRO_Utils_Mount_Device_Check(G_pro_config.utils.mount.device);
        }
    }

    ret = PRO_V2X_Init();
    if(ret < 0)
    {
        printf("V2X initialization failed.\n");
        goto out;
    }else{
        printf("V2X initialization success.\n");
    }

#ifdef _D_PCAP_USED
    void **pd_ptr
#endif
    if(G_pro_config.pcap.pcap_enable == true)
    {
#ifdef _D_PCAP_USED

        struct buffer_array_setup_t setup;
        setup.buffer_size = 2048;
        setup.buffer_ptr = NULL;

        struct pro_buffer_t *usr_buf_ptr = Pro_Buffer_Array_Init(&setup);
        if (usr_buf_ptr == NULL)
        {
            printf("Buffer initialization failed.\n");
            return -1;
        }else{
            printf("Buffer initialization success. %p\n", usr_buf_ptr);
        }
        pd_ptr = malloc(sizeof(pcap_dumper_t*));

        ret = PRO_Pcap_Init(pd_ptr, NULL, NULL);
        if(ret < 0)
        {
            printf("PCAP initialization failed.\n");
            goto out;
        }
        pthread_create(&g_thread_gnss, NULL, PRO_UBLOX_Gnss_Thread, (void*)usr_buf_ptr);
#endif
    }else{
        pthread_create(&g_thread_gnss, NULL, PRO_UBLOX_Gnss_Thread, (void*)NULL);
    }
    pthread_detach(g_thread_gnss);

	struct itimerspec itval;
    int msec = 1000;
    
	int32_t time_fd = timerfd_create (CLOCK_MONOTONIC, 0);
    itval.it_value.tv_sec = 1;
    itval.it_value.tv_nsec = 0;
    itval.it_interval.tv_sec = 0 + (msec / 1000);
    itval.it_interval.tv_nsec = (msec % 1000) * 1e6;
    timerfd_settime(time_fd, TFD_TIMER_ABSTIME, &itval, NULL);
    uint64_t res;
    uint64_t pcap_num = 0;

    while(1)
    {
        ret = read(time_fd, &res, sizeof(res));
        if(ret < 0)
        {
            perror("read");
            break;
        }
        //20250331 PRO_UBLOX_Gnss_Thread 테스크 종류(세션오류 등으로)되면 재실행
        if(G_gpsd_task_isrun == false)
        {
            pthread_cancel(g_thread_gnss);
            pthread_create(&g_thread_gnss, NULL, PRO_UBLOX_Gnss_Thread, (void*)NULL);
            pthread_detach(g_thread_gnss);
        }

        if(G_pro_config.pcap.pcap_enable == true)
        {
            do{
                struct pro_default_buffer_array_t out_buf = {.data = NULL, .id = 0, .size = 0, .next = NULL, .prev = NULL};
                ret = Pro_Buffer_Array_Pop(&out_buf, NULL);
                if(ret > 0 )
                {   
                    PRO_Pcap_Add(NULL, (const unsigned char *)out_buf.data, (uint32_t)out_buf.size);
                    pcap_num++;
                    printf("[PCAP_ADD], data_id:%d, pcap_num : %ld\n", out_buf.id, pcap_num); 
                    free(out_buf.data);
                }
                usleep(1);
            } while(ret != 0); 
        }
            
    }

out:
//20250331 시그널 오류시 루프 문제로 주석처리
    //LTEV2XHAL_Close();
    if(G_pro_config.pcap.pcap_enable == true)
    {
        do{
            struct pro_default_buffer_array_t out_buf = {.data = NULL, .id = 0, .size = 0, .next = NULL, .prev = NULL};
            ret = Pro_Buffer_Array_Pop(&out_buf, NULL);
            if(ret > 0 )
            {   
                PRO_Pcap_Add(NULL, (const unsigned char *)out_buf.data, (uint32_t)out_buf.size);
                pcap_num++;
                free(out_buf.data);
            }
            usleep(1 * 1000);
        } while(ret != 0); 
#ifdef _D_PCAP_USED
        free(pd_ptr);
#endif
    }
    return 0;
}

static void PRO_Main_Signal_Set()
{
    /*
    * 종료 시에 반드시 LAL_Close()가 호출되어야 하므로, 종료 시그널 핸들러를 등록한다.
    */
    struct sigaction sig_action;
    sig_action.sa_handler = PRO_Main_Signal_Handler;
    sigemptyset(&sig_action.sa_mask);
    sig_action.sa_flags = 0;

    sigaction(SIGINT, &sig_action, NULL);
    sigaction(SIGHUP, &sig_action, NULL);
    sigaction(SIGTERM, &sig_action, NULL);
    sigaction(SIGSEGV, &sig_action, NULL);
    return;
}

extern void PRO_Main_Signal_Handler(int signo)
{
    switch(signo)
    {
        case SIGINT:
        case SIGTERM:
        case SIGSEGV:
        case SIGHUP:
        {
            printf("Signal %d received. Exit.\n", signo);
            (void)signo;
            G_pro_config.v2x.tx_running = false;
//20250331 시그널 오류시 루프 문제로 주석처리
            //LTEV2XHAL_Close();
            if(g_sockfd > 0)
                close(g_sockfd);
            exit(0);
            system("killall pro_pcap");
            break;
        }
        default:
        {
            exit(0);
            system("killall pro_pcap");
            break;
        }
    }
    return;
}