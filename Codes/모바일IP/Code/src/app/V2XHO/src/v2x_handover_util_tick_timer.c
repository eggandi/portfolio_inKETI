#include "v2x_handover_main.h"

#define V2XHO_ERROR_CODE_FILE_CODE V2XHO_ERROR_CODE_FILE_CODE_UTIL_TICK_TIMER

uint32_t G_tick_10ms = 0;
uint32_t G_tick_100ms = 0;
uint32_t G_tick_1000ms = 0;

extern void *Th_v_V2XHO_Util_Task_Tick_Timer(void *d)
{
    d = (void*)d;
    int ret;
    int32_t  time_fd;
    struct timespec tv;
	struct itimerspec itval;
    int msec = 10;
    if(G_gnss_data->status.unavailable == true)
	{
		time_fd = timerfd_create (CLOCK_MONOTONIC, 0);
		clock_gettime(CLOCK_MONOTONIC, &tv); 
	}else{
		time_fd = timerfd_create (CLOCK_MONOTONIC, 0);
		clock_gettime(CLOCK_MONOTONIC, &tv); 
	}
	itval.it_interval.tv_sec = 0;
	itval.it_interval.tv_nsec = (msec * 1000 % 1000000) * 1e3;
    timerfd_settime(time_fd, TFD_TIMER_ABSTIME, &itval, NULL);
    uint64_t res;
	while(1)
	{
		/* wait periodic timer (timeout = 10ms) */
        ret = read(time_fd, &res, sizeof(res));
		if (ret > 0)
		{
            G_tick_10ms++;
            if(G_tick_10ms == 10)
            {
                G_tick_100ms++;
            }
            if(G_tick_10ms == 100)
            {
                G_tick_1000ms++;
            }

            if(G_tick_10ms == 0xFFFFFFFF)
            {
                G_tick_10ms = 0;
            }
            if(G_tick_100ms == 0xFFFFFFFF)
            {
                G_tick_100ms = 0;
            }
            if(G_tick_1000ms == 0xFFFFFFFF)
            {
                G_tick_1000ms = 0;
            }
		}
	}

    close(time_fd);
}