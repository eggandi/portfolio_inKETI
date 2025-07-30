#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10

int main(void) {
    struct udev *udev;
    struct udev_monitor *mon;
    int mon_fd, epoll_fd;

    // udev 컨텍스트 생성
    udev = udev_new();
    if (!udev) {
        fprintf(stderr, "udev 컨텍스트 생성 실패\n");
        return EXIT_FAILURE;
    }

    // udev 모니터 생성 (netlink을 통해 이벤트 수신)
    mon = udev_monitor_new_from_netlink(udev, "udev");
    if (!mon) {
        fprintf(stderr, "udev 모니터 생성 실패\n");
        udev_unref(udev);
        return EXIT_FAILURE;
    }

    // "block" 서브시스템의 이벤트 필터 추가 (mmcblk 관련 대상)
    if (udev_monitor_filter_add_match_subsystem_devtype(mon, "block", NULL) < 0) {
        fprintf(stderr, "필터 추가 실패\n");
        udev_monitor_unref(mon);
        udev_unref(udev);
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }

    printf("SD 카드 이벤트 감시 시작...\n");

    struct epoll_event events[MAX_EVENTS];
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds < 0) {
            perror("epoll_wait 실패");
            break;
        }

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == mon_fd && (events[i].events & EPOLLIN)) {
                struct udev_device *dev = udev_monitor_receive_device(mon);
                if (dev) {
                    const char *action = udev_device_get_action(dev);
                    const char *devnode = udev_device_get_devnode(dev);
                    const char *subsystem = udev_device_get_subsystem(dev);
                    // mmcblk 관련 디바이스에 대해 이벤트 출력
                    if (devnode && strstr(devnode, "mmcblk") != NULL) {
                        printf("SD 카드 이벤트: action=%s, devnode=%s, subsystem=%s\n",
                               action ? action : "none",
                               devnode ? devnode : "none",
                               subsystem ? subsystem : "none");
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

