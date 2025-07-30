#include <stdio.h>     // 사용된 함수: printf, fprintf, perror, snprintf
#include <stdlib.h>    // 사용된 함수: malloc, free, exit, strdup
#include <stdint.h>    // 사용된 타입: uint8_t, uint16_t 등
#include <string.h>    // 사용된 함수: memset, memcpy, strlen, strdup
#include <sys/stat.h>  // stat(), mkdir() (Linux/macOS)
#include <sys/types.h> // stat(), mkdir()
#include <unistd.h>    // 사용된 함수: access, mkdir
#include <stdbool.h>   // 사용된 타입: bool, true, false
#include <fcntl.h>     // 사용된 함수: open, O_RDWR, O_NOCTTY, O_SYNC
#include <termios.h>   // 사용된 매크로: B115200 (시리얼 통신 설정 등)
#include <arpa/inet.h> // 사용된 함수: inet_pton, htons
#include <sys/socket.h> // 사용된 함수: socket, connect
#include <netinet/in.h> // 사용된 타입: sockaddr_in
#include <errno.h>     // 사용된 변수: errno (perror 내부에서 사용)
#include <pthread.h>   // 사용된 타입: pthread_t, pthread_create, pthread_join
#include <sys/timerfd.h> // 사용된 함수: timerfd_create, timerfd_settime
#include <signal.h>    // 사용된 함수: sigaction, SIGINT, SIGTERM
#include <time.h>      // 사용된 타입: struct timespec, clock_gettime

#ifndef _D_HEADER_MAIN
#define _D_HEADER_MAIN

#endif //?_D_HEADER_MAIN

extern void PRO_Main_Signal_Handler(int signo);
extern struct pro_config_t G_pro_config;