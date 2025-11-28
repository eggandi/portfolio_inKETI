/**
 * @file
 * @brief TCP 통신 기능 라이브러리 구현
 * @date 2025-10-11
 * @author user
 */

#ifndef V2X_HUB_KVHTCP_INTERNAL_H
#define V2X_HUB_KVHTCP_INTERNAL_H


// 시스템 헤더파일
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

// 라이브러리 헤더파일
#include "kvhtcp.h"


/*
 * 함수 속성 정의 매크로
 */
/// 공개 API 함수임을 나타내기 위한 매크로
#define OPEN_API __attribute__((visibility("default")))
/// 공개 API 가 아닌 내부함수로 지정 (라이브러리 외부에서 호출 불가)
#define INTERNAL __attribute__((visibility("hidden")))


/**
 * @brief 라이브러리 기능 컨텍스트 정보
 */
typedef struct
{
  struct {
    int fd; ///< 소켓 파일 디스크립터
    struct sockaddr_in client_addr; ///< 내 주소 정보
    int client_port; ///< 클라이언트 포트
    struct sockaddr_in server_addr; ///< 서버 주소 정보
    int server_port; ///< 서버 포트
    bool session_established; ///< 연결 상태. 클라이언트는 connect() 성공 시 true, 서버는 listen() 성공 시 true
		int send_flags;
		int recv_flags;
		KVHTCPClientType client_sock[KVHTCP_ServerConnectedClientsMax]; ///< 서버동작시 연결된 클라이언트 리스트
  } sock; ///< 소켓 관리정보

  struct {
    pthread_t th; ///< 쓰레드 식별자
    volatile bool running; ///< 쓰레드가 동작 중인지 여부
  } rx_thread; ///< TCP 데이터 수신 쓰레드 관리정보

  uint8_t rx_buf[kKVHIFDataLen_Max]; ///< 수신된 데이터가 저장되는 버퍼
  KVHTCP_ProcessRxCallback rx_cb; ///< 어플리케이션 콜백함수
  void *app_priv; ///< 어플리케이션 전용 정보 (콜백함수 호출시 어플리케이션으로 함께 전달된다)
} KVHTCP_CTX;


#endif //V2X_HUB_KVHTCP_INTERNAL_H
