#include <stdio.h>     // 사용된 함수: printf, fprintf, perror, snprintf
#include <stdlib.h>    // 사용된 함수: malloc, free, exit, strdup
#include <stdint.h>    // 사용된 타입: uint8_t, uint16_t 등
#include <string.h>    // 사용된 함수: memset, memcpy, strlen, strdup
#include <sys/stat.h>  // stat(), mkdir() (Linux/macOS)
#include <unistd.h>    // 사용된 함수: access, mkdir
#include <dirent.h>    // 사용된 함수: opendir, readdir, closedir
#include <assert.h>    // 사용된 매크로: assert
#include <stdbool.h>   // 사용된 타입: bool, true, false
#include <fcntl.h>     // 사용된 함수: open, O_RDWR, O_NOCTTY, O_SYNC
#include <arpa/inet.h> // 사용된 함수: inet_pton, htons
#include <sys/socket.h> // 사용된 함수: socket, connect
#include <netinet/in.h> // 사용된 타입: sockaddr_in
#include <pthread.h>   // 사용된 타입: pthread_t, pthread_create, pthread_join
#include <sys/timerfd.h> // 사용된 함수: timerfd_create, timerfd_settime
#include <signal.h>    // 사용된 함수: sigaction, SIGINT, SIGTERM
#include <time.h>      // 사용된 타입: struct timespec, clock_gettime
#include <ifaddrs.h> // 사용된 함수: getifaddrs, freeifaddrs
#include <getopt.h> // 사용된 함수: getopt_long, optarg, optind, 타입: option
#include <stdarg.h> // 사용된 함수: va_list, va_start, va_end, vsnprintf, 타입: va_list
#include <malloc.h> // 사용된 함수: va_list, va_start, va_end, vsnprintf, 타입: va_list