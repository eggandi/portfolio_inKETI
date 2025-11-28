/**
 * @file
 * @brief TCP 통신 기능 라이브러리 구현
 * @date 2025-08-25
 * @author user
 */


// 라이브러리 헤더파일
#include "kvhtcp.h"
#include "kvhtcp_internal.h"


/**
 * @brief TCP 데이터를 클라이언트로 전송한다.
 * @parma[in] h 라이브러리 기능 핸들 (초기화 API에서 리턴된 핸들)
 * @param[in] data 전송할 데이터
 * @param[in] len 전송할 데이터의 길이
 * @retval 0: 성공
 * @retval -1: 실패
 *
 * 내가 서버로 동작할 경우, 처음에는 클라이언트의 주소를 모르기 때문에, get_peer_addr가 참이 되기 전까지는 데이터를 전송할 수 없다.
 * 클라이언트로부터 TCP 데이터를 수신하면, 클라이언트의 주소를 획득하게 되고 get_peer_addr가 참이 된다. (TCP 수신 쓰레드 상에서)
 */
int OPEN_API KVHTCP_SendToClient(KVHTCPHANDLE h, int client_sock, KVHTCPClientType client_type, const uint8_t *data, KVHIFDataLen len)
{
  KVHTCP_CTX *ctx = (KVHTCP_CTX *)h;
  int ret = -1;
	if (client_type != ctx->sock.client_sock[client_sock]) {
		return -1; // 클라이언트 소켓 타입 오류
	} else{
		if (ctx && (client_sock > 0) && data && (len <= kKVHIFDataLen_Max)) {
			if ((KVHIFDataLen)send(client_sock, data, len, ctx->sock.send_flags) == len) {
				ret = 0;
			}
		}
	}
  
  return ret;
}


/**
 * @brief TCP 데이터를 전송한다.
 * @parma[in] h 라이브러리 기능 핸들 (초기화 API에서 리턴된 핸들)
 * @param[in] data 전송할 데이터
 * @param[in] len 전송할 데이터의 길이
 * @retval 0: 성공
 * @retval -1: 실패
 *
 * 내가 서버로 동작할 경우, 처음에는 클라이언트의 주소를 모르기 때문에, get_peer_addr가 참이 되기 전까지는 데이터를 전송할 수 없다.
 * 클라이언트로부터 TCP 데이터를 수신하면, 클라이언트의 주소를 획득하게 되고 get_peer_addr가 참이 된다. (TCP 수신 쓰레드 상에서)
 */
int OPEN_API KVHTCP_SendToServer(KVHTCPHANDLE h, const uint8_t *data, KVHIFDataLen len)
{
  KVHTCP_CTX *ctx = (KVHTCP_CTX *)h;
  int ret = -1;
  if (ctx && ctx->sock.session_established && (ctx->sock.fd > 0) && data && (len <= kKVHIFDataLen_Max)) {
    if ((KVHIFDataLen)send(ctx->sock.fd, data, len, ctx->sock.send_flags) == len) {
      ret = 0;
    }
  }
  return ret;
}


/**
 * @brief TCP 클라이언트가 서버에 Connection 요청을 보낸다. 
 * @param[in] h 라이브러리 기능 핸들 (초기화 API에서 리턴된 핸들)
 * @return 0: 성공, -1: 실패, -2: 핸들 오류
 */
int KVHTCP_ConnectionServer(KVHTCPHANDLE *h)
{
  if (!h) {
		return -2;
  }
	int ret = 0;
  KVHTCP_CTX *ctx = (KVHTCP_CTX *)h;
	ret = connect(ctx->sock.fd, (struct sockaddr*)&(ctx->sock.server_addr), sizeof(ctx->sock.server_addr));
	if (ret < 0) { ///< 소켓 연결 실패
		ret = -1;
	} else { ///< 소켓 연결 성공 시 session_established 플래그 설정
		ctx->sock.session_established = true;
	}

	return ret;
}


/**
 * @brief TCP 데이터를 수신하는 클라이언트 쓰레드
 * @param[in] arg 라이브러리 관리정보(MIB)
 * @return NULL(프로그램 종료되기 전까지는 리턴되지 않음)
 */
void * KVHTCP_ClientRxThread(void *arg)
{
  KVHTCP_CTX *ctx = (KVHTCP_CTX *)arg;

  int ret = KVHTCP_ConnectionServer((KVHTCPHANDLE *)ctx);
	if (ret == 0) {
		ctx->sock.session_established = true;	
	} else {
		sleep(KVHTCP_ConnectionServerRetryIntervalSec);
		for (int i = 0; i < KVHTCP_ConnectionServerRetryMax; i++) {
			ret = KVHTCP_ConnectionServer((KVHTCPHANDLE *)ctx);
			if (ret == 0) {
				ctx->sock.session_established = true;
				break;
			}
			sleep(KVHTCP_ConnectionServerRetryIntervalSec);
		}
	}
	if (ctx->sock.session_established == false) {
		// 연결 실패
		return NULL;
	}

  ctx->rx_thread.running = true;
	while (ctx->rx_thread.running)
  {
    /*
     * TCP 데이터를 수신한다. (블로킹함수)
     * ctx->sock.fd 소켓으로 TCP 데이터가 수신되면 본 함수가 리턴되며, ctx->sock.peer_addr에 상대방 정보(IP주소, 포트번호)가 저장/반환된다.
     */
    ssize_t len = recv(ctx->sock.fd, ctx->rx_buf, sizeof(ctx->rx_buf), ctx->sock.recv_flags);

    /*
     * 필요시 쓰레드를 종료한다.
     */
    if (ctx->rx_thread.running == false) {
      return NULL;
    }

    /*
     * 수신된 데이터를 처리한다.
     *  - 콜백함수를 호출하여 수신된 데이터를 어플리케이션으로 전달한다.
     */
    if (len > 0) {
      if (len > kKVHIFDataLen_Max) {
        len = kKVHIFDataLen_Max;
      }
      if (ctx->rx_cb) {
				ctx->rx_cb((KVHTCPHANDLE)ctx, ctx->rx_buf, len, ctx->app_priv);
			}
    } 
  }
  return NULL;
}


/**
 * @brief TCP 클라이언트 기능을 초기화한다.
 * @param[in] bind_addr 클라이언트 바인드 IP 주소
 * @param[in] rx_port 클라이언트 수신 포트번호
 * @param[in] server_ip 서버 IP 주소
 * @param[in] server_port 서버 포트번호
 * @param[in] rx_cb 클라이언트로부터 수신된 데이터를 어플리케이션으로 전달할 콜백함수 포인터 (사용하지 않을 경우, NULL 전달 가능)
 * @parma[in] priv 어플리케이션 전용 정보(콜백함수 호출 시 전달받고자 하는 데이터 포인터) (사용하지 않을 경우, NULL 전달 가능)
 * @return TCP 기능 핸들 (실패 시 NULL)
 *
 * TCP 소켓을 생성(socket())하고 내 주소정보를 바인드(bind()) 한다.
 * TCP 클라이언트로부터 데이터를 수신하는 쓰레드를 생성한다.
 * 본 API를 통해 초기화가 완료되면, 데이터 수신 쓰레드 상에서 TCP 클라이언트로부터 데이터 수신 시마다 어플리케이션 콜백함수가 호출되어 수신데이터가 전달된다.
 */
KVHTCPHANDLE OPEN_API KVHTCP_InitClient(char *bind_addr, uint16_t *rx_port, char *server_ip, uint16_t server_port, KVHTCP_ProcessRxCallback rx_cb, void *priv)
{
  /*
   * 소켓을 생성한다.
   */
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    return NULL;
  }
	
	struct linger solinger = { 1, 0 };
	struct timeval tv = { .tv_sec = 0, .tv_usec = 500 * 1000	};
	setsockopt(sock, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger));
	setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv,sizeof(struct timeval)); 
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv,sizeof(struct timeval));

	/*
	 * TCPIP 통신할 서버 주소정보를 소켓에 입력.
	 */
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	server_addr.sin_port = htons(server_port);

	/*
	 * 내(=클라이언트) 주소정보를 소켓에 바인드(해당 주소로 수신되는 패킷만 처리, TCPIP는 설정 안해도 자동으로 할당)한다.
	 */
	struct sockaddr_in client_addr;
	if (bind_addr) {
		client_addr.sin_family = AF_INET;
		client_addr.sin_addr.s_addr = inet_addr(bind_addr);
		client_addr.sin_port = htons(*rx_port);
		bind(sock, (struct sockaddr*)&client_addr, sizeof(client_addr));
	}

  /*
   * 컨텍스트 정보를 생성한다.
   */
  KVHTCP_CTX *ctx = (KVHTCP_CTX *)calloc(1, sizeof(KVHTCP_CTX));
  if (!ctx) {
    close(sock);
    return NULL;
  }

  ctx->sock.fd = sock;
	ctx->sock.server_port = (server_port);
	ctx->sock.client_port = (*rx_port);
	ctx->sock.session_established = false; // connection 아직 안함
	ctx->sock.send_flags = 0;
	ctx->sock.recv_flags = 0;
	if (bind_addr) {
  	memcpy(&(ctx->sock.client_addr), &client_addr, sizeof(client_addr));
	}
  memcpy(&(ctx->sock.server_addr), &server_addr, sizeof(server_addr));
  ctx->rx_cb = rx_cb;
  ctx->app_priv = priv;

  /*
   * TCP 데이터 수신 쓰레드를 생성한다.
   */
  if (pthread_create(&(ctx->rx_thread.th), NULL, KVHTCP_ClientRxThread, ctx) != 0) {
    close(sock);
    free(ctx);
    return NULL;
  }
  struct timespec req = { .tv_sec = 0, .tv_nsec = 10000000 }, rem; // 쓰레드가 생성될 때까지 대기한다.
  while(ctx->rx_thread.running == false) {
    nanosleep(&req, &rem);
  }

  return (KVHTCPHANDLE)ctx;
}


/**
 * @brief TCP 데이터를 수신하는 Epoll 서버 쓰레드
 * @param[in] arg 라이브러리 관리정보(MIB)
 * @return NULL(프로그램 종료되기 전까지는 리턴되지 않음)
 */
void * KVHTCP_ServerRxThread(void *arg)
{
  KVHTCP_CTX *ctx = (KVHTCP_CTX *)arg;

	int epfd = epoll_create1(0);
	if (epfd < 0) {
		close(ctx->sock.fd);
		return NULL;
	}

	struct epoll_event ev, events[KVHTCP_ServerConnectedClientsMax];
	ev.events = EPOLLIN;
	ev.data.fd = ctx->sock.fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, ctx->sock.fd, &ev) < 0) {
		close(ctx->sock.fd);
		close(epfd);
		return NULL;
	}

  ctx->rx_thread.running = true;

	while (ctx->rx_thread.running)
  {
    int nfds = epoll_wait(epfd, events, KVHTCP_ServerConnectedClientsMax, -1);
		if (nfds < 0) {
			return NULL;
		}
 		/*
     * 필요시 쓰레드를 종료한다.
     */
    if (ctx->rx_thread.running == false) {
      return NULL;
    }
    for (int i = 0; i < nfds; i++) {
			if (events[i].events & EPOLLIN) {
				// 새로운 클라이언트 연결 수락
				if (events[i].data.fd == ctx->sock.fd) {
						struct sockaddr_in client_addr;
						socklen_t client_len = sizeof(client_addr);
						int client_fd = accept(ctx->sock.fd, (struct sockaddr *)&client_addr, &client_len);
						if (client_fd < 0) {
								continue;
						}
						// 클라이언트 소켓을 epoll에 추가
						struct epoll_event ev1;
						ev1.events = EPOLLIN;
						ev1.data.fd = client_fd;
						if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev1) < 0) {
								close(client_fd);
								continue;
						}
						/*
						* 클라이언트 소켓의 종류를 저장
						*/
					if (ctx->sock.client_sock[client_fd] == kKVHTCPClientType_No_Accept) {
						ctx->sock.client_sock[client_fd] = kKVHTCPClientType_Accept; 
					}
				} else {
					ssize_t len = recv(events[i].data.fd, ctx->rx_buf, sizeof(ctx->rx_buf), ctx->sock.recv_flags);
					if (len > 0) {
						if (len > kKVHIFDataLen_Max) {
							len = kKVHIFDataLen_Max;
						}
						if (ctx->rx_cb) {
							int *now_client_fd = malloc(sizeof(int));
							*now_client_fd = events[i].data.fd; // 어플리케이션에서 클라이언트 소켓을 식별할 수 있도록 client_fd를 app_priv로 전달
							uint8_t *data = malloc(len);
							if (data) {
								memcpy(data, ctx->rx_buf, len);
								ctx->rx_cb((KVHTCPHANDLE)ctx, data, len, (void *)now_client_fd);
							}
						}
					}
				}
			}
		}
	}
	close(epfd);
	close(ctx->sock.fd);
   
  return NULL;
}


/**
 * @brief TCP 서버 기능을 초기화한다.
 * @param[in] bind_addr 서버 바인드 IP 주소
 * @param[in] server_ip 서버 IP 주소
 * @param[in] server_port 서버 포트번호
 * @param[in] rx_cb 클라이언트로부터 수신된 데이터를 어플리케이션으로 전달할 콜백함수 포인터 (사용하지 않을 경우, NULL 전달 가능)
 * @parma[in] priv 어플리케이션 전용 정보(콜백함수 호출 시 전달받고자 하는 데이터 포인터) (사용하지 않을 경우, NULL 전달 가능)
 * @return TCP 기능 핸들 (실패 시 NULL)
 *
 * TCP 소켓을 생성(socket())하고 내 주소정보를 바인드(bind()) 한다.
 * Epoll Server로 멀티 클라이언트 접속을 처리한다.
 * TCP 클라이언트로부터 데이터를 수신하는 쓰레드를 생성한다.
 * 본 API를 통해 초기화가 완료되면, 데이터 수신 쓰레드 상에서 TCP 클라이언트로부터 데이터 수신 시마다 어플리케이션 콜백함수가 호출되어 수신데이터가 전달된다.
 */
KVHTCPHANDLE OPEN_API KVHTCP_InitServer(char *server_ip, uint16_t server_port, KVHTCP_ProcessRxCallback rx_cb, void *priv)
{
  /*
   * 소켓을 생성한다.
   */
  int server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0) {
    return NULL;
  }

	/*
	 * TCPIP 통신할 서버 주소정보를 소켓에 입력.
	 */
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	server_addr.sin_port = htons(server_port);

	/*
	 * 내(=서버) 주소정보를 소켓에 바인드(해당 주소로 수신되는 패킷만 처리, TCPIP는 설정 안해도 자동으로 할당)한다.
	 */
	bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

	/*
	 * 내(=서버) 소켓의 send/recv 블록 대기 시간을 설정한다.
	 */
	struct linger solinger = { 1, 0 };
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 500 * 1000;
	setsockopt(server_sock, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger));
	setsockopt(server_sock, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv,sizeof(struct timeval)); 
	setsockopt(server_sock, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv,sizeof(struct timeval));

  /*
   * 컨텍스트 정보를 생성한다.
   */
  KVHTCP_CTX *ctx = (KVHTCP_CTX *)calloc(1, sizeof(KVHTCP_CTX));
  if (!ctx) {
    close(server_sock);
    return NULL;
  }

  ctx->sock.fd = server_sock;
	ctx->sock.server_port = server_port;
	ctx->sock.send_flags = 0;
	ctx->sock.recv_flags = 0;

  memcpy(&(ctx->sock.server_addr), &server_addr, sizeof(server_addr));
  ctx->rx_cb = rx_cb;
  ctx->app_priv = priv;

	/*
	 * 서버 소켓을 수신 대기 상태로 만든다.
	 */
	if (listen(ctx->sock.fd, SOMAXCONN) < 0) {
		perror("listen");
		close(ctx->sock.fd);
		free(ctx);
		return NULL;
	} else {
		ctx->sock.session_established = true; // listen 성공 시 session_established 플래그 설정
	}

  /*
   * TCP 데이터 수신 쓰레드를 생성한다.
   */
  if (pthread_create(&(ctx->rx_thread.th), NULL, KVHTCP_ServerRxThread, ctx) != 0) {
    close(ctx->sock.fd);
    free(ctx);
    return NULL;
  }

  struct timespec req = { .tv_sec = 0, .tv_nsec = 10000000 }, rem; // 쓰레드가 생성될 때까지 대기한다.
  while(ctx->rx_thread.running == false) {
    nanosleep(&req, &rem);
  }

  return (KVHTCPHANDLE)ctx;
}


/**
 * @brief TCP 통신 기능을 종료한다.
 * @parma[in] h 라이브러리 기능 핸들 (초기화 API에서 리턴된 핸들)
 *
 * 소켓이 닫히고 핸들이 삭제된다.
 */
void OPEN_API KVHTCP_Close(KVHTCPHANDLE h)
{
  if (h) {
    KVHTCP_CTX *ctx = (KVHTCP_CTX *)h;
    ctx->rx_thread.running = false;
    close(ctx->sock.fd); // 소켓을 닫으면, 수신 쓰레드의 recv() 함수가 리턴되고 쓰레드가 종료된다.
    ctx->sock.fd = -1;
    pthread_join(ctx->rx_thread.th, NULL); // 쓰레드가 종료될 때까지 대기한다.
    free(ctx);
  }
}

/**
 * @brief TCP 소켓 파일 디스크립터를 통해 정보를 가져온다.
 * @parma[in] sock 클라이언트 소켓 파일 디스크립터
 * @param[out] ip 클라이언트 IP 주소 문자열
 * @param[out] port 클라이언트 포트 번호
 */
void OPEN_API KVHTCP_GetClientInfo(int sock, char *ip, int *port)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

	if (getpeername(sock, (struct sockaddr *)&addr, &len) == -1) {
			perror("getpeername");
			return;
	}
	if (ip == NULL || port == NULL) {
		return;
	}
	inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
	*port = ntohs(addr.sin_port);

	return;
}




#if 0
/**
 * @brief iface할당된 첫번째 Ehternet IPv4 주소를 가져온다.
 * @param[in] iface 네트워크 인터페이스 이름
 * @param[out] out_ipv4_addr_str IPv4 주소 문자열을 저장할 버퍼
 * @param[out] out_prefix 서브넷 프리픽스를 저장할 버퍼
 *
 * 이더넷 인터페이스 이름으로 NIC에 할당된 첫번째 IPv4 주소를 가져온다. 
 */
void INTERNAL KVHTCP_GetIPv4Address(const char *iface, char *out_ipv4_addr_str, int *out_prefix) 
{
    struct ifaddrs *ifaddr, *ifa;

    // 시스템에 등록된 모든 네트워크 인터페이스 정보를 가져옴
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    // 모든 인터페이스를 순회하면서 IPv4 주소 검색
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        // 인터페이스가 지정한 이름과 일치하고 AF_INET (IPv4) 주소인 경우
        if (strcmp(ifa->ifa_name, iface) == 0 && ifa->ifa_addr->sa_family == AF_INET)
         {
            struct sockaddr_in *sockaddr = (struct sockaddr_in *)ifa->ifa_addr;

            // IPv4 주소를 문자열로 변환하여 출력
            if (inet_ntop(AF_INET, &sockaddr->sin_addr, out_ipv4_addr_str, INET_ADDRSTRLEN) != NULL) {
                
            } else {
                perror("inet_ntop");
            }
            int prefix_length = 0;
            unsigned char *ptr = (unsigned char *)&sockaddr->sin_addr;
            for (int i = 0; i < INET_ADDRSTRLEN; i++) 
            {
                unsigned char byte = ptr[i];
                while (byte) {
                    prefix_length += byte & 1;
                    byte >>= 1;2
                }
            }
            *out_prefix = prefix_length;
        }
    }

    freeifaddrs(ifaddr);
}
#endif

