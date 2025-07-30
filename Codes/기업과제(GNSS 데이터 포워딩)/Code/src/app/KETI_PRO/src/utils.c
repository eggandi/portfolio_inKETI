#include "utils.h"
#include "config.h"
#include "main.h"

static int PRO_Utils_Mount_Read_Mbr(const char *device, uint8_t mbr[512]);
static char* PRO_Utils_Mount_Parent_Device(const char *dev);

extern int PRO_Utils_File_Path_Exists(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

extern void PRO_Utils_File_Path_Create(const char *path) {
    if (PRO_Utils_File_Path_Exists(path)) {
        printf("Folder '%s' is Exists. \n", path);
    } else {
        printf("Folder '%s' is creating now.\n", path);
        char sys_cmd[256] = {0x00,};
        sprintf(sys_cmd, "mkdir -m 777 -p %s", path);
        system(sys_cmd);
        if(PRO_Utils_File_Path_Exists(path))
        {
            printf("Folder '%s' is created.\n", path);
        }else{
            printf("Folder '%s' is not created.\n", path);
        }
    }
    return ;
}

static int PRO_Utils_Mount_Mounted_Check(const char *target_device, const char *path) 
{
    FILE *fp = fopen("/proc/mounts", "r");
    if (!fp) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    char line[512];
    char mount_point[256] = {0};
    int dev_found = 0;
    int path_found = 100;

    while (fgets(line, sizeof(line), fp)) {
        char device[256], mnt[256], fstype[64];
        if (sscanf(line, "%255s %255s %63s", device, mnt, fstype) == 3) 
        {
            if(target_device != NULL)
            {
                if (strcmp(device, target_device) == 0) {
                    strncpy(mount_point, mnt, sizeof(mount_point) - 1);
                    dev_found += 1;
                    break;
                }
            }
            if(path != NULL)
            {
                if (strcmp(mnt, path) == 0) {
                    strncpy(mount_point, mnt, sizeof(mount_point) - 1);
                    path_found += 1;
                    break;
                }
            }

        }
    }
    fclose(fp);

    return dev_found + path_found;
}

static int PRO_Utils_Mount_Read_Mbr(const char *device, uint8_t mbr[512]) 
{
    int fd = open(device, O_RDONLY);
    if (fd < 0) {
        perror("디바이스 열기 실패");
        return -1;
    }
    ssize_t bytes_read = read(fd, mbr, 512);
    close(fd);
    if (bytes_read != 512) {
        fprintf(stderr, "MBR 읽기 실패 (읽은 바이트: %zd)\n", bytes_read);
        return -1;
    }
    return 0;
}

static char* PRO_Utils_Mount_Parent_Device(const char *dev) 
{
    size_t len = strlen(dev);
    // 복사본 생성 (최대 길이 +1)
    char *parent = malloc(len + 1);
    if (!parent) return NULL;
    strcpy(parent, dev);

    // mmcblk의 경우: 끝에 "p"와 숫자가 붙음.
    // 예: "/dev/mmcblk0p1" -> 찾은 'p' 뒤의 숫자 제거
    char *p = strrchr(parent, 'p');
    if (p && strstr(parent, "mmcblk") != NULL) {
        // p 위치 뒤에 숫자가 있는지 확인
        if (*(p+1) >= '0' && *(p+1) <= '9') {
            *p = '\0'; // "p"를 잘라 부모 디바이스로 만듦
            return parent;
        }
    }
    // 그 외의 경우: 맨 끝의 숫자 제거 (예: "/dev/sda1" -> "/dev/sda")
    // 마지막 문자가 숫자라면 해당 숫자들을 제거
    while (len > 0 && parent[len - 1] >= '0' && parent[len - 1] <= '9') {
        parent[len - 1] = '\0';
        len--;
    }
    return parent;
}

extern int PRO_Utils_Mount_Device_Check(const char* device)
{
    uint8_t mbr[512];
    if (PRO_Utils_Mount_Read_Mbr(device, mbr) != 0) {
        return -1 * __LINE__;
    }
    char *parent_dev = NULL;
    // 부트 시그니처 체크 (0x55 0xAA)
    if (mbr[510] != 0x55 || mbr[511] != 0xAA) {
        // 만약 부트 시그니처가 없으면 파티션 디바이스일 가능성이 있음
        fprintf(stderr, "부모 디바이스로 재시도합니다...\n");
        parent_dev = PRO_Utils_Mount_Parent_Device(device);
        if (!parent_dev) {
            fprintf(stderr, "부모 디바이스 추정 실패\n");
            return -1 * __LINE__;
        }
        printf("추정된 부모 디바이스: %s\n", parent_dev);
        if (PRO_Utils_Mount_Read_Mbr(parent_dev, mbr) != 0) {
            free(parent_dev);
            return -1 * __LINE__;
        }
        if (mbr[510] != 0x55 || mbr[511] != 0xAA) {
            fprintf(stderr, "부모 디바이스 (%s)에서도 유효한 MBR을 찾지 못했습니다.\n", parent_dev);
            free(parent_dev);
            return -1 * __LINE__;
        }
        device = parent_dev;  // 부모 디바이스 경로로 분석 진행
        // 이후 parent_dev는 사용 후 free() 필요
    }

    PartitionEntry *partitions = (PartitionEntry *)(mbr + 446);  // MBR에서 파티션 엔트리 시작은 446바이트 오프셋

    for (int partition_num = 0; partition_num < 4; partition_num++) {
        if (partitions[partition_num].partition_type == 0) {
            continue;
        }else{
            switch(partitions[partition_num].partition_type)
            {
                case 0x83: //Linux
                {
                    char syscmd[512];
                    PRO_Utils_File_Path_Create(G_pro_config.utils.mount.path);
                    if(PRO_Utils_Mount_Mounted_Check(NULL, G_pro_config.utils.mount.path) > 0)
                    {
                        snprintf(syscmd, sizeof(syscmd), "umount %sp%01d", device, partition_num + 1);
                        system(syscmd);
                        memset(syscmd, 0x00, sizeof(syscmd));
                    }
                    snprintf(syscmd, sizeof(syscmd), "mount -o rw %sp%01d %s", device, partition_num + 1, G_pro_config.utils.mount.path);
                    system(syscmd);
                    memset(syscmd, 0x00, sizeof(syscmd));
                    break;
                }
                default:
                    break;
            }
        }
    }
    if(parent_dev != NULL)
    {
        free(parent_dev);
    }
    
    return EXIT_SUCCESS;
}

extern void* PRO_Utils_Mount_Active_Mode(void *d) 
{
    d = (void*)d;

    struct udev *udev;
    struct udev_monitor *mon;
    int mon_fd, epoll_fd;
    int max_events = 10;

    // udev 컨텍스트 생성
    udev = udev_new();
    if (!udev) {
        fprintf(stderr, "udev 컨텍스트 생성 실패\n");
        return NULL;
    }

    // udev 모니터 생성 (netlink을 통해 이벤트 수신)
    mon = udev_monitor_new_from_netlink(udev, "udev");
    if (!mon) {
        fprintf(stderr, "udev 모니터 생성 실패\n");
        udev_unref(udev);
        return NULL;
    }

    // "block" 서브시스템의 이벤트 필터 추가 (mmcblk 관련 대상)
    if (udev_monitor_filter_add_match_subsystem_devtype(mon, "block", NULL) < 0) {
        fprintf(stderr, "필터 추가 실패\n");
        udev_monitor_unref(mon);
        udev_unref(udev);
        return NULL;
    }
    udev_monitor_enable_receiving(mon);

    // udev 모니터의 파일 디스크립터 획득
    mon_fd = udev_monitor_get_fd(mon);

    // epoll 인스턴스 생성
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_create1 실패");
        udev_monitor_unref(mon);
        udev_unref(udev);
        return NULL;
    }

    // udev 모니터 fd를 epoll에 추가
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = mon_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, mon_fd, &ev) < 0) {
        perror("epoll_ctl 실패");
        close(epoll_fd);
        udev_monitor_unref(mon);
        udev_unref(udev);
        return NULL;
    }

    struct epoll_event events[max_events];
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, max_events, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == mon_fd && (events[i].events & EPOLLIN)) {
                struct udev_device *dev = udev_monitor_receive_device(mon);
                if (dev) {
                    const char *action = udev_device_get_action(dev);
                    const char *devnode = udev_device_get_devnode(dev);
                    const char *subsystem = udev_device_get_subsystem(dev);
                    if ( devnode && strcmp(action, "add") == 0 && strstr(devnode, "mmcblk") != NULL) {
                        if(PRO_Utils_Mount_Device_Check(devnode) == 0)
                        {
                            printf("SD 카드 이벤트: action=%s, devnode=%s, subsystem=%s\n",
                                action ? action : "none",
                                devnode ? devnode : "none",
                                subsystem ? subsystem : "none");
                        }
                    }else if(devnode && strcmp(action, "remove") == 0 && strstr(devnode, "mmcblk") != NULL)
                    {
                        printf("SD 카드 이벤트: action=%s, devnode=%s, subsystem=%s\n",
                            action ? action : "none",
                            devnode ? devnode : "none",
                            subsystem ? subsystem : "none");
                        udev_device_unref(dev);
                        close(epoll_fd);
                        udev_monitor_unref(mon);
                        udev_unref(udev);
                        PRO_Main_Signal_Handler(2);
                        break;
                    }
                    udev_device_unref(dev);
                }
            }
        }
    }

    close(epoll_fd);
    udev_monitor_unref(mon);
    udev_unref(udev);
    return EXIT_SUCCESS;
}
