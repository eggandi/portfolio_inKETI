#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <libudev.h>
#include <sys/epoll.h>

#include <sys/stat.h>  // stat(), mkdir() (Linux/macOS)
#include <sys/types.h> // stat(), mkdir()

#ifndef _D_HEADER_UTILS
#define _D_HEADER_UTILS

struct pro_config_utils_t
{
    struct mount_t
    {
        bool enable;
        int type;
        bool active_mode;
        char device[256];
        char path[256];
    } mount;

};

// 패킹 옵션: 구조체의 멤버들을 패딩 없이 연속 배치
#pragma pack(push, 1)
typedef struct {
    uint8_t  boot_indicator;   // 부트 가능 여부: 0x80 부트 가능, 0x00 비부트 가능
    uint8_t  start_head;
    uint8_t  start_sector;     // 상위 6비트: 섹터, 하위 2비트: 시작 실린더 상위 비트
    uint8_t  start_cylinder;
    uint8_t  partition_type;   // 파티션 타입 코드 (예: 0x83: Linux, 0x07: NTFS 등)
    uint8_t  end_head;
    uint8_t  end_sector;
    uint8_t  end_cylinder;
    uint32_t starting_lba;     // 파티션 시작 LBA
    uint32_t size_in_sectors;  // 파티션 크기 (섹터 수)
} PartitionEntry;
#pragma pack(pop)

#endif //?_D_HEADER_UTILS

extern void PRO_Utils_File_Path_Create(const char *path);
extern int PRO_Utils_File_Path_Exists(const char *path);
extern int PRO_Utils_Mount_Device_Check(const char* device);
extern void* PRO_Utils_Mount_Active_Mode(void *d);

