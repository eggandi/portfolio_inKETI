#ifndef _D_HEADER_RELAY_INNO_EXTERN_DEFINES
#define _D_HEADER_RELAY_INNO_EXTERN_DEFINES

#define PROJECT_NAME "RELAY_INNO"
#define EXTERN_API extern __attribute__((visibility("default")))

#define _DEBUG_LINE _DEBUG_PRINT("\n");
#ifdef _D_DEBUG_LOG
	#define _DEBUG_PRINT(fmt, ...) printf("[%s][%d]",__func__, __LINE__); printf(fmt, ##__VA_ARGS__)
	#else
		#define _DEBUG_PRINT(fmt, ...) 
#endif // _D_DEBUG_LOG

extern int G_relay_v2x_tx_socket;
extern int G_relay_v2x_rx_socket;
extern struct sockaddr_in G_relay_v2x_tx_addr;
extern struct sockaddr_in G_relay_v2x_rx_addr;
extern bool G_power_off;
extern bool G_BSM_TX_RUNNING;
#endif // _D_HEADER_RELAY_INNO_EXTERN_DEFINES
