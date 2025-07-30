#include <stdio.h>     // Standard I/O functions: printf, fprintf, etc.
#include <stdlib.h>    // Standard library functions: malloc, free, exit, etc.
#include <string.h>    // String manipulation functions: strcpy, strlen, memcpy, etc.
#include <time.h>      // Time functions: time, localtime, etc.
#include <sys/time.h>  // Time functions: gettimeofday, struct timeval, etc.
#include <stdbool.h>   // Boolean type support: bool, true, false.
#ifdef _D_PCAP_USED
#include <pcap/pcap.h>      // Packet capture library functions: pcap_open, pcap_close, pcap_dump, etc.
#endif
#ifndef _D_HEADER_PRO_PCAP
#define _D_HEADER_PRO_PCAP

#define PRO_PCAP_DEFAULT_PROJECT_NAME "KETI"
#define PRO_PCAP_DEFAULT_DATA_SET  "V2XnGNSS"
#define PRO_PCAP_DEFAULT_FILENAME_FORMAT "%s_%s_%02d%02d%02d_%06ld.pcap"
#define PRO_PCAP_DEFAULT_FILE_PATH "./f_db/pcap/"

struct pro_config_pcap_t{
    bool pcap_enable;
    char pcap_file_path[256];
    bool pcap_file_sub_path_enable;
    char pcap_file_name_header_0[256];
    char pcap_file_name_header_1[256];
    bool pcap_file_name_yymmdd_enable;
    int pcap_file_saving_type;
    int pcap_file_saving_split_size;
    int pcap_file_saving_rolling_time;
};
// 함수

#endif //?_D_HEADER_PRO_PCAP

extern int PRO_Pcap_Open(void **pd_ptr, const char *filename);
extern int PRO_Pcap_Init(void **pd_ptr, const char *filename, const char *path);
extern void PRO_Pcap_Close(void);
extern int PRO_Pcap_Add(void *pd, const unsigned char *data, size_t len);
