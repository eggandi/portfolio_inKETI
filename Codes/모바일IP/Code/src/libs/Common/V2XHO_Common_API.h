#ifndef _CALLED_V2XHO_COMMON_HEADER
#define _CALLED_V2XHO_COMMON_HEADER 1
#include <V2XHO_Common_Define.h>
#endif

extern uint16_t F_u16_V2XHO_Checksum(unsigned short *buf, int nwords);
extern char* F_p_V2XHO_Get_InterfaceIP(char* interface);
extern void *F_th_V2XHO_Epoll_Time_Tick(void *d);

