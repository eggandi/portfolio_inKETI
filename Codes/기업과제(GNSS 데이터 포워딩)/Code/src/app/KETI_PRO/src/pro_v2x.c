#include "pro_v2x.h"
#include "config.h"
#include "buffer.h"

uint32_t g_recv_count;

int g_v2x_id = 1;

/**
 * @brief LTE-V2X MSDU 수신처리 콜백함수. lteaccess 라이브러리에서 호출된다.
 * @param[in] msdu 수신된 MSDU (= WSM 헤더 + WSM body)
 * @param[in] msdu_size 수신된 MSDU의 크기
 */
void PRO_V2X_RxMSDUCallback(const uint8_t *msdu, LTEV2XHALMSDUSize msdu_size, struct LTEV2XHALMSDURxParams rx_params);

/**
 * @brief V2X 라이브러리들을 초기화한다.
 * @retval 0: 성공
 * @retval -1: 실패
 */
extern int PRO_V2X_Init(void)
{
  LTEV2XHALLogLevel hal_log_level = (LTEV2XHALLogLevel ) G_pro_config.v2x.lib_dbg;

  // LTEV2X 접속계층 라이브러리 초기화하고 패킷수신콜백함수를 등록한다.
  int ret = LTEV2XHAL_Init(hal_log_level, G_pro_config.v2x.dev_name);
  if (ret < 0) {
    printf("Fail to initialize ltev2x-hal library - LTEV2XHAL_Init() failed: %d\n", ret);
    return -1;
  }
  LTEV2XHAL_RegisterCallbackProcessMSDU(PRO_V2X_RxMSDUCallback);


  if(G_pro_config.udp_enable)
  {
    g_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_sockfd < 0) {
        exit(EXIT_FAILURE);
    }
    int flags = fcntl(g_sockfd, F_GETFL, 0);
    fcntl(g_sockfd, F_SETFL, flags | O_NONBLOCK);

    struct linger solinger = { 1, 0 };
    setsockopt(g_sockfd, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger));

    memset(&g_v2x_addr, 0, sizeof(g_v2x_addr));
    g_v2x_addr.sin_family = AF_INET;
    g_v2x_addr.sin_port = htons(G_pro_config.udp_port_v2x);
    g_v2x_addr.sin_addr.s_addr = inet_addr(G_pro_config.udp_ip_str);
  }

  printf("Success to initialize V2X library\n");

  return 0;
}

/**
 * @brief LTE-V2X MSDU 수신처리 콜백함수. lteaccess 라이브러리에서 호출된다.
 * @param[in] msdu 수신된 MSDU (= WSM 헤더 + WSM body)
 * @param[in] msdu_size 수신된 MSDU의 크기
 */
void PRO_V2X_RxMSDUCallback(const uint8_t *msdu, LTEV2XHALMSDUSize msdu_size, struct LTEV2XHALMSDURxParams rx_params)
{
  if(msdu_size > 0)
  {
    g_recv_count++;
    struct pro_waytis_header_t waytis_header;
    memset(&waytis_header, 0x00, sizeof(struct pro_waytis_header_t));
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    waytis_header.Seconds = ts.tv_sec;
    waytis_header.Nanoseconds = ts.tv_nsec * 1000;
    uint8_t src_id[4] = {0,};
    memcpy(src_id, &rx_params.src_l2_id, 4);
    waytis_header.Source_Address[0] = src_id[2];
    waytis_header.Source_Address[1] = src_id[1];
    waytis_header.Source_Address[2] = src_id[0];
    waytis_header.Sequence_Number = g_recv_count;
    uint8_t dst_id[4] = {0,};
    memcpy(dst_id, &rx_params.dst_l2_id, 4);
    waytis_header.Destination_Address[0] = dst_id[2];
    waytis_header.Destination_Address[1] = dst_id[1];
    waytis_header.Destination_Address[2] = dst_id[0];
    waytis_header.Priority = rx_params.rx_priority;
    //waytis_header.Transceiver_ID;
    //waytis_header.Channel_Busy_Ratio;
    waytis_header.Channel_Number = G_pro_config.v2x.rx_chan_num;
    //waytis_header.RSSI_A;
    //waytis_header.RSSI_B;
    if(g_recv_count >= 0xFFFFFFFF)
    {
      g_recv_count = 0;
    }
    uint8_t payload[4028] = {0,};
    memcpy(payload, &waytis_header, sizeof(struct pro_waytis_header_t));
    memcpy(payload + sizeof(struct pro_waytis_header_t), msdu, msdu_size);
    if(G_pro_config.pcap.pcap_enable)
    {
      Pro_Buffer_Array_Push(payload, sizeof(struct pro_waytis_header_t) + msdu_size, &g_v2x_id, NULL);
    }
    if(G_pro_config.udp_enable)
    {
      int ret;
      ret = sendto(g_sockfd, payload, sizeof(struct pro_waytis_header_t) + msdu_size, 0, (struct sockaddr *)&g_v2x_addr, sizeof(g_v2x_addr));
      if(ret > 0)
      {
          //printf("Send Sucess. : %d\n", ret);
      }
    }
    
    memset(payload, 0x00, sizeof(payload));
    free(wsdu)
  }
  // 수신 파라미터 출력
  #if 0
  printf("rx_params: \n");
  printf("rx_time: %"PRIu64", rx_power: %ddBm, rx_priority: %d, src_l2_id: %08X, dst_l2_id: %08X\n",
      rx_params.rx_time,
      rx_params.rx_power,
      rx_params.rx_priority,
      rx_params.src_l2_id,
      rx_params.dst_l2_id);
  #endif
  if
  
  return;
}

