#include "gnss.h" 
#include "buffer.h"
#include "config.h"

static void PRO_UBLOX_Trance_STR2BIN(char *hex, uint8_t *out_bin, int *out_len);
static int PRO_UBLOX_Trance_HEX2DEC(char c);

bool G_gpsd_task_isrun = false;

extern void *PRO_UBLOX_Gnss_Thread(void *arg)
{
    G_gpsd_task_isrun = true;
    struct pro_buffer_t *usr_buf_pt = (struct pro_buffer_t *)arg;
    //PRO_UBLOX_Initial_Set();
    // 1) gpsd 서버에 TCP 연결
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return NULL;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(GPSD_PORT);
    if (inet_pton(AF_INET, GPSD_SERVER, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        return NULL;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return NULL;
    }

    printf("Connected to gpsd at %s:%d\n", GPSD_SERVER, GPSD_PORT);

    // 2) RAW UBX 스트림 요청:
    //    JSON=false 이므로 NMEA나 TPV가 아닌 바이너리 UBX가 바로 들어옴
    //    raw=1 -> RAW 모드
    const char *watch_cmd = "?WATCH={\"enable\":true,\"raw\":1,\"json\":false};\n";
    send(sockfd, watch_cmd, strlen(watch_cmd), 0);

    // 3) UBX 프로토콜 파싱 상태 변수
    enum {
        STATE_SYNC1,
        STATE_SYNC2,
        STATE_CLASS,
        STATE_ID,
        STATE_LEN1,
        STATE_LEN2,
        STATE_PAYLOAD,
        STATE_CK_A,
        STATE_CK_B
    } state = STATE_SYNC1;

    uint8_t  ubx_class  = 0;
    uint8_t  ubx_id     = 0;
    uint16_t ubx_length = 0;
    uint8_t  payload[1024];
    uint16_t payload_index = 0;
    uint8_t  ck_a_calc = 0, ck_b_calc = 0;
    uint8_t  ck_a_recv, ck_b_recv;
    int id_gnss = 2;

    int udp_socket;
    struct sockaddr_in gnss_addr;
    struct sockaddr_in daemon_addr;
    if(G_pro_config.udp_enable)
    {
        udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (udp_socket < 0) {
            exit(EXIT_FAILURE);
        }
        int flags = fcntl(udp_socket, F_GETFL, 0);
        fcntl(udp_socket, F_SETFL, flags | O_NONBLOCK);

        struct linger solinger = { 1, 0 };
        setsockopt(udp_socket, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger));
       
        memset(&gnss_addr, 0, sizeof(gnss_addr));
        gnss_addr.sin_family = AF_INET;
        gnss_addr.sin_port = htons(G_pro_config.udp_port_gnss);
        gnss_addr.sin_addr.s_addr = inet_addr(G_pro_config.udp_ip_str);
        memset(&daemon_addr, 0, sizeof(daemon_addr));
        daemon_addr.sin_family = AF_INET;
        daemon_addr.sin_port = htons(G_pro_config.udp_port_gnss_daemon);
        daemon_addr.sin_addr.s_addr = inet_addr(G_pro_config.udp_ip_str);
    }

    while (1) {
        uint8_t buf[2048] = {0};
        uint8_t str_buffer[2048] = {0};
        int n = recv(sockfd, str_buffer, sizeof(buf), 0);
        if (n < 0) {
            G_gpsd_task_isrun = false;
            perror("recv");
            break;
        }
        if(str_buffer[0] == 'b' && str_buffer[1] == '5')
        {
            PRO_UBLOX_Trance_STR2BIN((char *)str_buffer, buf, &n);
        }
        if (n == 0) {
            // gpsd 연결이 끊김
            printf("gpsd connection closed (EOF)\n");
            break;
        }

        // 바이트 스트림 파싱
        for (int i = 0; i < n; i++) {
            uint8_t c = buf[i];
            switch (state)
            {
            case STATE_SYNC1:
                if (c == UBX_SYNC1)
                    state = STATE_SYNC2;
                break;

            case STATE_SYNC2:
                if (c == UBX_SYNC2)
                    state = STATE_CLASS;
                else
                    state = STATE_SYNC1;
                break;

            case STATE_CLASS:
                ubx_class = c;
                ck_a_calc = c;   // 체크섬 초기화
                ck_b_calc = ck_a_calc;
                state = STATE_ID;
                break;

            case STATE_ID:
                ubx_id = c;
                ck_a_calc += c;  
                ck_b_calc += ck_a_calc;
                state = STATE_LEN1;
                break;

            case STATE_LEN1:
                ubx_length = c;  // length low byte
                ck_a_calc += c;
                ck_b_calc += ck_a_calc;
                state = STATE_LEN2;
                break;

            case STATE_LEN2:
            {
                ubx_length |= (c << 8); // length high byte
                ck_a_calc += c;
                ck_b_calc += ck_a_calc;

                // 길이가 너무 크면 버림
                if (ubx_length > sizeof(payload)) {
                    // 상태 재동기화
                    state = STATE_SYNC1;
                } else {
                    payload_index = 0;
                    state = (ubx_length > 0) ? STATE_PAYLOAD : STATE_CK_A;
                }
                break;
            }
            case STATE_PAYLOAD:
                payload[payload_index++] = c;
                ck_a_calc += c;
                ck_b_calc += ck_a_calc;

                if (payload_index >= ubx_length) {
                    state = STATE_CK_A;
                }
                break;

            case STATE_CK_A:
                ck_a_recv = c;
                state = STATE_CK_B;
                break;

            case STATE_CK_B:
                ck_b_recv = c;

                // 체크섬 검증
                if (ck_a_recv == ck_a_calc && ck_b_recv == ck_b_calc) {
                    // NAV-PVT인지 확인 (클래스=0x01, ID=0x07)
                    if (ubx_class == UBX_CLASS_NAV && ubx_id == UBX_ID_PVT) {
                        if (ubx_length == sizeof(ubx_nav_pvt_t)) {
                            //printf("Gnss Data Received %ld\n", sizeof(ubx_nav_pvt_t));
                            if(usr_buf_pt != NULL)
                            {
                                Pro_Buffer_Array_Push(payload, sizeof(ubx_nav_pvt_t), &id_gnss, NULL);

                            }
                            if(G_pro_config.udp_enable)
                            {
                                int ret;
                                if(G_pro_config.udp_gnss_include_ubx_header)
                                {
                                    ret = sendto(udp_socket, buf, sizeof(ubx_nav_pvt_t) + 6, 0, (struct sockaddr *)&gnss_addr, sizeof(gnss_addr));
                                    if(ret > 0)
                                    {
                                        //printf("Send Sucess. : %d\n", ret);
                                    }
                                    usleep(5000);
                                    ret = sendto(udp_socket, buf, sizeof(ubx_nav_pvt_t) + 6, 0, (struct sockaddr *)&daemon_addr, sizeof(daemon_addr));
                                    if(ret > 0)
                                    {
                                        //printf("Send Sucess. : %d\n", ret);
                                    }
                                }else{
                                    ret = sendto(udp_socket, payload, sizeof(ubx_nav_pvt_t), 0, (struct sockaddr *)&gnss_addr, sizeof(gnss_addr));
                                    if(ret > 0)
                                    {
                                        //printf("Send Sucess. : %d\n", ret);
                                    }
                                    usleep(5000);
                                    ret = sendto(udp_socket, payload, sizeof(ubx_nav_pvt_t), 0, (struct sockaddr *)&daemon_addr, sizeof(daemon_addr));
                                    if(ret > 0)
                                    {
                                        //printf("Send Sucess. : %d\n", ret);
                                    }
                                }

                            }
                            #if 0
                            ubx_nav_pvt_t nav;
                            memcpy(&nav, payload, sizeof(nav));

                            // 원하는 정보 출력
                            double lat_deg = nav.lat / 1e7;
                            double lon_deg = nav.lon / 1e7;
                            double alt_m   = nav.hMSL / 1000.0;

                            
                            printf("NAV-PVT Received!\n");
                            printf("  iTOW=%u ms\n", nav.iTOW);
                            printf("  date=%04u-%02u-%02u time=%02u:%02u:%02u\n",
                                nav.year, nav.month, nav.day,
                                nav.hour, nav.min, nav.sec);
                            printf("  lat=%.7f deg, lon=%.7f deg, alt=%.2f m\n",
                                lat_deg, lon_deg, alt_m);
                            printf("  fixType=%u (numSV=%u)\n",
                                nav.fixType, nav.numSV);
                            printf("--------------------------------------\n");
                            printf("Bytes_Dump: ");
                            for(size_t aa = 0; aa < sizeof(ubx_nav_pvt_t); aa++)
                            {
                                printf("%02X", payload[aa]);
                            }  
                            printf("\n");
                            #endif
                        } else {
                            fprintf(stderr, 
                                "NAV-PVT length mismatch (got=%u, expected=%lu)\n",
                                ubx_length, (unsigned long)sizeof(ubx_nav_pvt_t));
                        }
                    }
                } 
                // 다음 메시지 위해 재동기화
                state = STATE_SYNC1;
                break;

            } // switch
        } // for
    } // while

    close(sockfd);
    G_gpsd_task_isrun = false;
    return 0;
}


static int PRO_UBLOX_Trance_HEX2DEC(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return -1;  // 잘못된 문자
}

static void PRO_UBLOX_Trance_STR2BIN(char *hex, uint8_t *out_bin, int *out_len) {
    int hex_len = strlen(hex);
    if (hex_len % 2 != 0) {
        //printf("Invalid hex string length!\n");
        return;
    }

    *out_len = hex_len / 2;
    for (int i = 0; i < *out_len; i++) {
        int high = PRO_UBLOX_Trance_HEX2DEC(hex[i * 2]);
        int low = PRO_UBLOX_Trance_HEX2DEC(hex[i * 2 + 1]);
        if (high == -1 || low == -1) {
            //printf("Invalid hex character found!\n");
            return;
        }
        out_bin[i] = (high << 4) | low;
    }
}

#define SERIAL_PORT "/dev/gps0"
#define BAUDRATE B115200
struct ubx_cfg_header_t{
    uint8_t header[2];
    uint8_t class;
    uint8_t id;
    uint16_t length;
};

const unsigned char UBX_CFG_RATE_10HZ[] = {
    0xB5, 0x62, 0x06, 0x08, 0x06, 0x00,  // UBX 헤더
    0x64, 0x00,  // 측정 주기 (100ms = 10Hz)
    0x01, 0x00,  // navRate (1번 측정마다 NAV 솔루션 업데이트)
    0x01, 0x00,  // timeRef (시간 기준)
    0x7A, 0x12   // 체크섬 (Fletcher-8)
};

extern int PRO_UBLOX_Initial_Set() {
    int fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("Failed to open serial port");
        return 1;
    }

    if (write(fd, UBX_CFG_RATE_10HZ, sizeof(UBX_CFG_RATE_10HZ)) < 0) {
        perror("Failed to send UBX-CFG-RATE message");
        close(fd);
        return 1;
    }

    printf("Sent UBX-CFG-RATE (10Hz) message to u-blox GPS\n");

    close(fd);
    return 0;
}