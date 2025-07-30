#include "pro_pcap.h"
#include "utils.h"
#include "config.h"

// 준-전역변수
#ifdef _D_PCAP_USED
pcap_t *g_pcap_dead = NULL;
pcap_dumper_t *g_pd   = NULL;
#else
char *g_pcap_dead = NULL;
char *g_pd = NULL;
#endif
void **g_pd_ptr = NULL;
void **g_fpfn_ptr = NULL;
bool g_pcap_add_used;

static void PRO_Pcap_File_Set_Name(char *filename, size_t len);
#if 0
static void PRO_Pcap_SwitchFile(char *fpfn);
static long PRO_Pcap_FILE_Size_Check(char *fpfn_ptr);
#endif
// 메인 코드
extern int PRO_Pcap_Init(void **pd_ptr, const char *filename, const char *path)
{
    char fn[256] = {0};
    char *fpfn = malloc(sizeof(char) * 1024);

    if(path)
    {
        PRO_Utils_File_Path_Create(path);
        if(filename == NULL)
        {
            PRO_Pcap_File_Set_Name(fn, 256);
            snprintf(fpfn, 1024, "%s/%s", path, fn);
        }else{
            snprintf(fpfn, 1024, "%s/%s", path, filename);
        }
    }else{
        if(G_pro_config.pcap.pcap_file_sub_path_enable)
        { 
            struct timeval tv;
            struct tm *tm_info;
            gettimeofday(&tv, NULL);
            time_t now = tv.tv_sec;
            tm_info = localtime(&now);
            char sub_path[512] = {0};
            snprintf(sub_path, 512, "%s/%02d%02d%02d", G_pro_config.pcap.pcap_file_path, tm_info->tm_year % 100, tm_info->tm_mon + 1, tm_info->tm_mday);
            PRO_Utils_File_Path_Create(sub_path);
            if(filename == NULL)
            {
                PRO_Pcap_File_Set_Name(fn, 256);
                snprintf(fpfn, 1024, "%s/%s", sub_path, fn);
            }else{
                snprintf(fpfn, 1024, "%s/%s", sub_path, filename);
            }
            memset(sub_path, 0x00, 256);
        }else{
            PRO_Utils_File_Path_Create(G_pro_config.pcap.pcap_file_path);
            if(filename == NULL)
            {
                PRO_Pcap_File_Set_Name(fn, 256);
                snprintf(fpfn, 512, "%s/%s", G_pro_config.pcap.pcap_file_path, fn);
            }else{
                snprintf(fpfn, 512, "%s/%s", G_pro_config.pcap.pcap_file_path, filename);
            }
        }
    }
    if(g_fpfn_ptr == NULL)
    {
        g_fpfn_ptr = malloc(sizeof(char*));
        *g_fpfn_ptr = (char*)fpfn;
    }

    int ret = PRO_Pcap_Open(pd_ptr, fpfn);

    g_pcap_add_used = 0;
    memset(fn, 0x00, 256);
    return ret;
}

extern int PRO_Pcap_Open(void **pd_ptr, const char *fpfn)
{
    pd_ptr = pd_ptr;
    fpfn = fpfn;
    if(g_pcap_dead != NULL)
    {
        PRO_Pcap_Close();
    }
#ifdef _D_PCAP_USED
    g_pcap_dead = pcap_open_dead(DLT_USER0, 65535);
    if (!g_pcap_dead) {
        fprintf(stderr, "pcap_open_dead() failed\n");
        return -1 * __LINE__;
    }

    if(pd_ptr != NULL)
    {
        *pd_ptr = (pcap_dumper_t *)pcap_dump_open(g_pcap_dead, fpfn);
        if (!*pd_ptr) {
            fprintf(stderr, "pcap_dump_open() failed: %s\n", pcap_geterr(g_pcap_dead));
            pcap_close(g_pcap_dead);
            return -1 * __LINE__;
        }else{
            g_pd_ptr = pd_ptr;
            g_pd = (pcap_dumper_t *)*pd_ptr;
            printf("pd_ptr : %p\n", *pd_ptr);
        }
    }else{
        g_pd_ptr = malloc(sizeof(pcap_dumper_t*));
        *g_pd_ptr = (pcap_dumper_t *)pcap_dump_open(g_pcap_dead, fpfn);
        if (!*g_pd_ptr) {
            fprintf(stderr, "pcap_dump_open() failed: %s\n", pcap_geterr(g_pcap_dead));
            pcap_close(g_pcap_dead);
            return -1 * __LINE__;
        }else{
            g_pd = (pcap_dumper_t *)*g_pd_ptr;
            printf("pd_ptr : %p\n", *g_pd_ptr);
        }
    }
#endif
    return 0;
}
/**
 * @brief PCAP 파일 닫기
 * 
 */
extern void PRO_Pcap_Close(void)
{


    if(g_pd == NULL)
    {
    }else{
#ifdef _D_PCAP_USED
        pcap_dump_close(g_pd);
#endif
    }
#ifdef _D_PCAP_USED
    if(g_pcap_dead == NULL)
    {
        return;
    }
    pcap_close(g_pcap_dead);
    if (*g_fpfn_ptr != NULL)  // 기존 메모리 해제
    {
        free(*g_fpfn_ptr);
    }
    if(g_fpfn_ptr != NULL)
    {
        free(g_fpfn_ptr);
    }
#endif
    return;
}

/**
 * @brief PCAP 파일에 패킷 추가
 * @param pd pcap_dumper_t 핸들
 * @param data 패킷 데이터
 * @param len 패킷 길이
 * @return 성공시 0, 실패시 -1 반환 
 */
extern int PRO_Pcap_Add(void *pd, const unsigned char *data, size_t len)
{
    pd = pd;
    data = data;
    len = len;
    g_pcap_add_used = true;
#ifdef _D_PCAP_USED
    struct pcap_pkthdr header;
    memset(&header, 0, sizeof(header));

    // 3-1) 타임스탬프 설정 (현재 시간)
    struct timeval tv;
    gettimeofday(&tv, NULL);
    header.ts = tv; // pcap_pkthdr 구조체에 현재 시각 저장

    // 3-2) 패킷 길이(저장할 데이터 길이)
    header.caplen = len;
    header.len    = len;

    // 3-3) pcap_dump() 호출
    if(pd == NULL)
    {
        pcap_dump((u_char *)g_pd, &header, data);
        pcap_dump_flush(g_pd);
    }else{
        pcap_dump((u_char *)pd, &header, data);
    }
#endif
    g_pcap_add_used = false;
    return 0;
}

/**
 * @brief PCAP 파일 이름 설정
 * 
 */
static void PRO_Pcap_File_Set_Name(char *filename, size_t len) 
{
    struct timeval tv;
    struct tm *tm_info;
    
    gettimeofday(&tv, NULL);
    time_t now = tv.tv_sec;
    tm_info = localtime(&now);
    if(filename == NULL)
    {
        return;
    }
    snprintf(filename, len, PRO_PCAP_DEFAULT_FILENAME_FORMAT,   G_pro_config.pcap.pcap_file_name_header_0,
                                                                G_pro_config.pcap.pcap_file_name_header_1,
                                                                tm_info->tm_year % 100,  // YY (2자리)
                                                                tm_info->tm_mon + 1,     // MM (1~12)
                                                                tm_info->tm_mday,        // DD (1~31)
                                                                tv.tv_usec);             // 마이크로초 단위 (6자리)
   
    return; 
}

#if 0 
/**
 * @brief PCAP 파일 크기 확인
 * @param d 사용하지 않음
 * @return 성공시 저장된 데이터 개수, 실패시 -1 반환
 */
long PRO_Pcap_FILE_Size_Check(char *fpfn)
{
    char *fpfn_ptr;
    if (fpfn == NULL)
    {
        if (g_fpfn_ptr != NULL)
            fpfn_ptr = *(char **)g_fpfn_ptr;
        else
            return -1;
    }else{
        fpfn_ptr = fpfn;
    }
    struct stat st;
    if (stat(fpfn_ptr, &st) == 0) {
        return st.st_size;
    }
    return -1; // 파일 크기 확인 실패
}

/**
 * @brief PCAP 파일 변경
 * 
 */
void PRO_Pcap_SwitchFile(char *fpfn)
{

    if (g_pd) {
        pcap_dump_close(g_pd);
        g_pd = NULL;
    }
    
    g_pd = pcap_dump_open(g_pcap_dead, fpfn);
    if (!g_pd) {
        fprintf(stderr, "pcap_dump_open() failed for new file: %s\n", fpfn);
        exit(1);
    }
    if(g_pd_ptr != NULL)
    {
        *g_pd_ptr = (pcap_dumper_t *)g_pd;
    }
    printf("Switched to new PCAP file: %s\n", fpfn);
}

#endif
