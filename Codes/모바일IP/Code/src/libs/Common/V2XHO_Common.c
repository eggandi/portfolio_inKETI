/*
 * V2XHO_Common.c
 *
 *  Created on: 2023. 10. 1.
 *      Author: Sung Dong-Gyu
 */
#include <V2XHO_Common.h>

struct V2XHO_Time_Tick_t *g_time_tick;

/* 
Brief:Checksum 계산
Parameter[In]
    unsigned short *buf:계산할 데이터의 포인터
    int nwords:데이터 포인터로부터 몇개를 계산할지에 대한 수
Parameter[Out]
    uint16_t checksum:계산된 checksum 값
 */
uint16_t F_u16_V2XHO_Checksum(unsigned short *buf, int nwords)
{
    unsigned long sum = 0;
	int tot_len = nwords;
    for(int i = 0; i < tot_len; i++)
    {
        sum += (*buf++);
    }
	if (tot_len % 2 == 1 )
	{
		sum += *(unsigned char*)buf++ & 0x00ff;
	}
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

/* 
Brief:인터페이스 이름을 이용해 할당된 주소값을 반환받음
Parameter[In]
    char* interface:주소값을 반환받을 인터페이스 장치
Parameter[Out]
    char *ip_address:인터페이스에 할당된 주소값을 반환(모든 주소가 아닌 ifconfig app을 사용했을 때 프린트 되는 장치에 할당된 주소)
 */
char* F_p_V2XHO_Get_InterfaceIP(char* interface)
{	
    struct ifreq ifr;
	int s;
    char *ipstr = malloc(sizeof(char) * 40);
	s = socket(AF_INET, SOCK_DGRAM, 0);
	strncpy(ifr.ifr_name, interface, IFNAMSIZ);
	if (ioctl(s, SIOCGIFADDR, &ifr) < 0) {
		printf("Error");
	} else {
		inet_ntop(AF_INET, ifr.ifr_addr.sa_data+2, ipstr, sizeof(struct sockaddr));
		//printf("%s IP Address is %s\n", interface, ipstr);
	}
	struct linger solinger = { 1, 0 };
	if (setsockopt(s, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger)) == -1) {
		perror("setsockopt(SO_LINGER)");
	}
	close(s);
    return ipstr;
}

/* 
Brief:틱 타이머 테스크
Parameter[In]
    void *d:struct V2XHO_Time_Tick_t *time_tick를 void로 타입캐스팅해 전달받음
Parameter[Out]
    NULL
 */
void *F_th_V2XHO_Epoll_Time_Tick(void *d)
{
    struct V2XHO_Time_Tick_t *time_tick = (struct V2XHO_Time_Tick_t *)d;
    
    int timer_fd;
    int ret;
    int msec;
    uint64_t res;

	switch(time_tick->precision)
    {
        case ms1000:
            msec = 1000;
            break;
        case ms100:
            msec = 100;
            break;
        case ms10:
            msec = 10;
            break;
        case ms1:
            msec = 1;
            break;
        default: 
            msec = 100;
            break;
    }

    struct timeval tv;
    gettimeofday(&tv, NULL); 
    time_tick->ts.it_interval.tv_sec = (int)(msec / 1000);
	time_tick->ts.it_interval.tv_nsec = ((msec * 1000) % 1000000) * 1e3;
    time_tick->ts.it_value.tv_sec = tv.tv_sec + 1;
	time_tick->ts.it_value.tv_nsec = 0;

    timer_fd = timerfd_create(CLOCK_REALTIME, 0);
    if(timer_fd < 0)
    {
        printf("[%s][%d]error\n", __func__, __LINE__);
    }
    ret = timerfd_settime(timer_fd, TFD_TIMER_ABSTIME, &(time_tick->ts), NULL);
    if(ret < 0)
    {
        printf("[%s][%d]error\n", __func__, __LINE__);
    }

    time_tick->tick.ms1 = 0;
    time_tick->tick.ms10 = 0;
    time_tick->tick.ms100 = 0;
    time_tick->tick.ms1000 = 0;
    while(1)
    {
        ret = read(timer_fd, &res, sizeof(res));
        if(ret > 0)
        {
            switch(g_time_tick->precision)
            {
                case ms1:
                    g_time_tick->tick.ms1 = (g_time_tick->tick.ms1 + 1) % 10;
                case ms10:
                    if(g_time_tick->tick.ms1 == 0)
                        g_time_tick->tick.ms10 = (g_time_tick->tick.ms10 + 1) % 10;
                case ms100:
                    if(g_time_tick->tick.ms10 == 0)
                        g_time_tick->tick.ms100 = (g_time_tick->tick.ms100 + 1) % 10;
                case ms1000:
                    if(g_time_tick->tick.ms100 == 0)
                        g_time_tick->tick.ms1000 = (g_time_tick->tick.ms1000 + 1) % 10;
                    break;
                default:
                    break;
            }
        }
    }

    close(timer_fd);
}

#if 0
/* 
Brief:According to the input debug level to print out a message in compare with the Global Debug Level.
Parameter[In]
    Debug_Lever_e:Debug Level
    String:A message to be print
    format:The Message Format
Parameter[Out]
    NULL
 */
void F_v_V2XHO_Print_Debug(enum V2XHO_DEBUG_Level_e Level, const char *format, ...)
{

    if(format)
    {
        switch(g_debug_level)
        {
            case 3:
            {
                #ifdef(G_DEBUG_PRINT_LINE_OFF)
                    fprintf(stderr, "[%d]", __LINE__);
                #endif
            }
            case 4:
            {   
                #ifdef(G_DEBUG_PRINT_FUNC_OFF)
                    fprintf(stderr, "[%s]", __func__);
                #endif
            }
            case 5:
            {
                #ifdef(G_DEBUG_PRINT_TIME_OFF)
                    struct timespec ts;
                    struct tm tm_now;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    localtime_r((time_t *)&ts.tv_sec, &tm_now);
                    fprintf(stderr, "[%04u%02u%02u.%02u%02u%02u.%06ld]", tm_now.tm_year+1900, tm_now.tm_mon+1, tm_now.tm_mday, \
                    tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec, ts.tv_nsec / 1000);
                #endif
            }
            default:
            {
                if(g_debug_level > Level)
                va_list arg;
                va_start(arg, format);
                vprintf(format, arg);
                va_end(arg);
                break;
            }
        }
    }else{
        return;
    }
}
#endif