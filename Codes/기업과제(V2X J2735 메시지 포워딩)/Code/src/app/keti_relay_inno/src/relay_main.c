#include "relay_main.h"
#include "relay_config.h"
#include "relay_v2x.h"
#include "relay_v2x_j2735_bsm.h"
#include "relay_v2x_dot2.h"

int G_relay_v2x_tx_socket;
int G_relay_v2x_rx_socket;
struct sockaddr_in G_relay_v2x_tx_addr;
struct sockaddr_in G_relay_v2x_rx_addr;
bool G_power_off = false;

bool G_BSM_TX_RUNNING = false;

static void RELAY_INNO_Main_Signal_Set();
static int RELAY_INNO_Main_Socket_Init();

/**
 * @brief 시그널 핸들러
 * @details 시그널 핸들러
 * @param signo 시그널 번호
 * @return void
 */
static void RELAY_INNO_Main_Signal_Handler(int signo);

pthread_t g_thread_gnss;

int main()
{
  int ret;
	// 
  ret = RELAY_INNO_Config_Setup_Configuration_Read(&G_relay_inno_config);
  if(ret < 0)
  {
    _DEBUG_PRINT("Configuration read failed.\n");
    return -1;
  } else{
    _DEBUG_PRINT("Configuration read success.\n");
  }
  RELAY_INNO_Main_Signal_Set();

  ret = RELAY_INNO_V2X_Init();
  if(ret < 0)
  {
    _DEBUG_PRINT("V2X initialization failed.\n");
    goto out;
  }else{
    _DEBUG_PRINT("V2X initialization success.\n");
  }
	if(G_relay_inno_config.v2x.dot2.enable == true)
	{
		ret = RELAY_INNO_V2X_Dot2_Security_Init();
		if(ret < 0)
		{
			_DEBUG_PRINT("V2X security initialization failed.\n");
			goto out;
		}else{
			_DEBUG_PRINT("V2X security initialization success.\n");
		}
	}
	ret = RELAY_INNO_Gnss_Init_Gnssata(&g_thread_gnss);
	if(ret < 0)
	{
		_DEBUG_PRINT("Gnss initialization failed.\n");
		goto out;
	}else{
		_DEBUG_PRINT("Gnss initialization success.\n");
	}

	LTEV2XHAL_RegisterCallbackProcessMSDU(RELAY_INNO_V2X_RxMSDUCallback);
	_DEBUG_PRINT("V2X RxMSDU callback registered.\n");

	struct itimerspec itval;
  int msec = 10;
  
	int32_t time_fd = timerfd_create (CLOCK_REALTIME, 0);
  itval.it_value.tv_sec = 1;
  itval.it_value.tv_nsec = 0;
  itval.it_interval.tv_sec = 0 + (msec / 1000);
  itval.it_interval.tv_nsec = (msec % 1000) * 1e6;
  timerfd_settime(time_fd, TFD_TIMER_ABSTIME, &itval, NULL);

  uint64_t res;
	uint32_t time_tick_10ms = 0;
	G_relay_inno_config.v2x.tx_running = true;
	ret = RELAY_INNO_Main_Socket_Init();
	if(ret < 0)
	{
		_DEBUG_PRINT("Socket initialization failed.\n");
		goto out;
	}else{
		_DEBUG_PRINT("Socket initialization success.\n");
	}
	/*
	 * J29451 라이브러리 사용에 따라 BSM 송신 방식이 달라진다.
	 * J29451 라이브러리 사용 시, BSM 자체 생성 방식은 사용하지 않는다.
	*/
	if(G_relay_inno_config.v2x.j2735.bsm.j29451_enable == true)
	{
		ret = RELAY_INNO_J2736_J29451_Initial();
		if(ret < 0)
		{
			_DEBUG_PRINT("BSM initialization failed.\n");
			goto out;
		}else{
			_DEBUG_PRINT("BSM initialization success.\n");
		}
		ret = J29451_StartBSMTransmit(G_relay_inno_config.v2x.j2735.bsm.interval);
		if (ret < 0) {
			_DEBUG_PRINT("Fail to start BSM transmit - J29451_StartBSMTransmit() failed: %d\n", ret);
			return -1;
		}
	}
	time_t bsm_check_prev = time(NULL);
	int reboot_timer = 20;
	G_BSM_TX_RUNNING = false;
  while(1)
  {
    ret = read(time_fd, &res, sizeof(res));
		time_tick_10ms = (time_tick_10ms + 1) % 0x10000000;
    if(ret < 0)
    {
      _DEBUG_PRINT("timerfd read failed.\n");
			close(time_fd);
			goto out;
      break;
    }
		if(time_tick_10ms % 100 == 0)
		{
			time_t current_time = time(NULL);
			if(G_BSM_TX_RUNNING == true)
			{
				bsm_check_prev = current_time;
				G_BSM_TX_RUNNING = false;
			}
			if (current_time < bsm_check_prev) {
			// 시스템 시간이 역전된 경우, 시간을 리셋
				bsm_check_prev = current_time;
			} else {
				if (current_time - bsm_check_prev > reboot_timer) {
						_DEBUG_PRINT("BSM transmission timeout.\n");
						RELAY_INNO_Main_Signal_Handler(SIGINT);
				}
			}
			_DEBUG_PRINT("BSM transmission check - %ld/%d\n", current_time - bsm_check_prev, reboot_timer);
		}

		if(G_relay_inno_config.v2x.j2735.bsm.j29451_enable == false)
		{
			if(time_tick_10ms % (G_relay_inno_config.v2x.j2735.bsm.interval / 10) == 0)
			{
				ret = RELAY_INNO_V2X_Tx_J2735_BSM(NULL);
				if(ret < 0)
				{
					_DEBUG_PRINT("V2X Tx BSM failed.\n");
				}else{
					_DEBUG_PRINT("V2X Tx BSM success.\n");
				}
			}
		}
  }

out:
  RELAY_INNO_Main_Signal_Handler(SIGINT);
  return 0;
}

static void RELAY_INNO_Main_Signal_Set()
{
  /*
  * 종료 시에 반드시 LAL_Close()가 호출되어야 하므로, 종료 시그널 핸들러를 등록한다.
  */
  struct sigaction sig_action;
  sig_action.sa_handler = RELAY_INNO_Main_Signal_Handler;
  sigemptyset(&sig_action.sa_mask);
  sig_action.sa_flags = 0;

  sigaction(SIGINT, &sig_action, NULL);
  sigaction(SIGHUP, &sig_action, NULL);
  sigaction(SIGTERM, &sig_action, NULL);
  sigaction(SIGSEGV, &sig_action, NULL);
  return;
}

static void RELAY_INNO_Main_Signal_Handler(int signo)
{
	G_power_off = true;
	sleep(1);
  switch(signo)
  {
    case SIGINT:
    case SIGTERM:
    case SIGSEGV:
    case SIGHUP:
    {
      _DEBUG_PRINT("Signal %d received. Exit.\n", signo);
      (void)signo;
      G_relay_inno_config.v2x.tx_running = false;
			
			close(G_relay_v2x_tx_socket);
			close(G_relay_v2x_rx_socket);
      exit(0);
      system("killall "PROJECT_NAME);
      break;
    }
    default:
    {
			close(G_relay_v2x_tx_socket);
			close(G_relay_v2x_rx_socket);
      exit(0);
      system("killall "PROJECT_NAME);
      break;
    }
  }
  return;
}

static int RELAY_INNO_Main_Socket_Init()
{
	G_relay_v2x_tx_socket = socket(AF_INET, SOCK_DGRAM, 0);
	G_relay_v2x_rx_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (G_relay_v2x_tx_socket < 0) {
			return -1;
	}else{
		int flags = fcntl(G_relay_v2x_tx_socket, F_GETFL, 0);
		fcntl(G_relay_v2x_tx_socket, F_SETFL, flags | O_NONBLOCK);
		struct linger solinger = { 1, 0 };
		setsockopt(G_relay_v2x_tx_socket, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger));
	}
	if (G_relay_v2x_rx_socket < 0) {
		return -2;
	}else{
		int flags = fcntl(G_relay_v2x_rx_socket, F_GETFL, 0);
		fcntl(G_relay_v2x_rx_socket, F_SETFL, flags | O_NONBLOCK);
		struct linger solinger = { 1, 0 };
		setsockopt(G_relay_v2x_rx_socket, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger));
	}
	memset(&G_relay_v2x_tx_addr, 0, sizeof(struct sockaddr_in));
	G_relay_v2x_tx_addr.sin_family = AF_INET;
	G_relay_v2x_tx_addr.sin_port = htons(G_relay_inno_config.relay.port_v2x_tx);
	G_relay_v2x_tx_addr.sin_addr.s_addr = inet_addr(G_relay_inno_config.relay.gatewayip);

	memset(&G_relay_v2x_rx_addr, 0, sizeof(struct sockaddr_in));
	G_relay_v2x_rx_addr.sin_family = AF_INET;
	G_relay_v2x_rx_addr.sin_port = htons(G_relay_inno_config.relay.port_v2x_rx);
	G_relay_v2x_rx_addr.sin_addr.s_addr = inet_addr(G_relay_inno_config.relay.gatewayip);
	G_relay_inno_config.relay.enable = true;

	_DEBUG_PRINT("Success to create UDP T/Rx socket\n");
	_DEBUG_PRINT("Tx socket: %d, Rx socket: %d\n", G_relay_v2x_tx_socket, G_relay_v2x_rx_socket);
	_DEBUG_PRINT("Tx port: %d, Rx port: %d\n", G_relay_inno_config.relay.port_v2x_tx, G_relay_inno_config.relay.port_v2x_rx);
	_DEBUG_PRINT("Tx IP: %s, Rx IP: %s\n", G_relay_inno_config.relay.gatewayip, G_relay_inno_config.relay.gatewayip);
	_DEBUG_PRINT("Tx socket addr: %s:%d, Rx socket addr: %s:%d\n", inet_ntoa(G_relay_v2x_tx_addr.sin_addr), ntohs(G_relay_v2x_tx_addr.sin_port), inet_ntoa(G_relay_v2x_rx_addr.sin_addr), ntohs(G_relay_v2x_rx_addr.sin_port));
	
	return 0;
}
