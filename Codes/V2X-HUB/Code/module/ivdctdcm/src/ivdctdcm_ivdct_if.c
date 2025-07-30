/**
 * @file
 * @brief IVDCT와의 I/F 기능 구현(RS232)
 * @date 2025-07-03
 * @author dong
 */

// 모듈 헤더파일
#include "ivdctdcm.h"
#include <arpa/inet.h>

#define IVDCTDCM_MAX_RX_DATA (3) ///< IVDCT에서 동시에 수신하는 최대 데이터 개수

IVDCTDCM_IvdctBasicData g_ivdctdcm_rx_data[IVDCTDCM_MAX_RX_DATA]; ///< IVDCT에서 수신된 데이터

static void * IVDCTDCM_IvdctBasicDataRxThread(void *arg);
static int IVDCTDCM_ConfigureIVDCTIFSerialPort(int sock, long baudrate);
static void IVDCTDCM_PrintfIVDCTCTypeData(KVHIVDCTData* d);


#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  #define BE32_TO_HOST(x) __builtin_bswap32(x)
  #define BE16_TO_HOST(x) __builtin_bswap16(x)
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define BE32_TO_HOST(x) (x)
  #define BE16_TO_HOST(x) (x)
#else
	#include <arpa/inet.h>
  #define BE32_TO_HOST(x) ntohl(x)
  #define BE16_TO_HOST(x) ntohs(x)
#endif

/**
 * @brief IVDCT I/F 시리얼포트를 설정한다.
 * @param[in] sock IVDCT I/F 시리얼포트 소켓
 * @param[in] baudrate 설정할 baudrate
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int IVDCTDCM_ConfigureIVDCTIFSerialPort(int sock, long baudrate)
{
  LOG(kKVHLOGLevel_Init, "Configure IVDCT I/F serial port\n");

  /*
   * 시리얼포트를 설정한다.
   */
  struct termios newtio;
  memset(&newtio, 0, sizeof(newtio));
  newtio.c_iflag &= ~(IGNCR | INLCR | ICRNL);
  newtio.c_lflag &= ~ICANON;
  newtio.c_lflag &= ~(ECHO | ECHOCTL | ECHONL);
  newtio.c_cc[VMIN] = 255;
  newtio.c_cc[VTIME] = 0; // 0.1초 타임아웃
  newtio.c_cflag |=  CREAD | CLOCAL;

  switch (baudrate) {
    case 9600:
      newtio.c_cflag |= B9600;
      break;
    case 19200:
      newtio.c_cflag |= B19200;
      break;
    case 38400:
      newtio.c_cflag |= B38400;
      break;
    case 57600:
      newtio.c_cflag |= B57600;
      break;
    case 115200:
      newtio.c_cflag |= B115200;
      break;
    case 230400:
      newtio.c_cflag |= B230400;
      break;
			    case 921600 :
      newtio.c_cflag |= B921600 ;
      break;
    default:
      ERR("Fail to configure IVDCT I/F serial port - invalid baudrate %ld\n", baudrate);
      return -1;

  }
  tcflush(sock, TCIFLUSH);
  tcsetattr(sock, TCSANOW, &newtio);

  LOG(kKVHLOGLevel_Init, "Success to configure IVDCT I/F serial port\n");
  return 0;
}
int IVDCTDCM_ProcessIVDCTData(IVDCTDCM_IvdctBasicData *pkt, uint16_t payload_len) 
{
	/*
	 * IVDCT 패킷을 처리
	 */
	if(pkt == NULL || pkt->payload_len == NULL || payload_len < 2) { // IVDCT 패킷이  유효한지 확인
		LOG(kKVHLOGLevel_Err, "Invalid IVDCT data packet\n");
		return -1;
	}
	/*
	 * IVDCT 프로토콜 버퍼 직렬화 데이터 파싱
	 *  NULL 입력 시 코드 내 전역변수가 핸들러(인스턴스)로 사용된다.
	 */
	int ret_pb = IVDCTDCM_ProcessPBParseFromArray(NULL ,pkt->payload, payload_len);
	if (ret_pb == 0) {
		LOG(kKVHLOGLevel_InOut, "Parsed Ivdct instance successfully.\n");	
		KVHIVDCTData *ivdct_data = NULL; // IVDCT 패킷의 payload(C++)를 저장할 c 타입 포인터 
		ret_pb = IVDCTDCM_GetPBHandleData(NULL, &ivdct_data);
		if (ret_pb == 0) {
			LOG(kKVHLOGLevel_InOut, "IVDCT data handle retrieved successfully.\n");
			IVDCTDCM_PrintfIVDCTCTypeData(ivdct_data);
			if (mib->dgm_if.uds_init == false) {
				continue; // UDS 초기화가 안된 경우, IVDCT 데이터를 DGM으로 전송하지 않는다.
			}
			/*
			 * IVDCT 프로토콜 버퍼 인스턴스의 데이터를 C 구조체로 변환하여 핸들러를 통해 가져온다. 
			 *  NULL 입력 시 코드 내 전역변수가 핸들러로 사용된다.
			 */ 
			IVDCTDCM_TransmitIvdctDataToDGM(mib, ivdct_data);
		} else {
			LOG(kKVHLOGLevel_Err, "Failed to get IVDCT data handle. Error code: %d\n", ret_pb);
			return -2;
		}
	} else {
		LOG(kKVHLOGLevel_Err, "Failed to parse Ivdct instance from string. Error code: %d\n", ret_pb);
		return -3;
	}
	return 1;
}
/**
 * @brief 시리얼포트를 통해 IVDCT에서 데이터를 수신하는 쓰레드
 * @param[in] arg 라이브러리 관리정보(MIB)
 * @return NULL(프로그램 종료되기 전까지는 리턴되지 않음)
 */
static void * IVDCTDCM_IvdctBasicDataRxThread(void *arg)
{
  LOG(kKVHLOGLevel_Init, "Start IVDCT data rx thread\n");

  IVDCTDCM_MIB *mib = (IVDCTDCM_MIB *)arg;
  mib->ivdct_if.rx_th_running = true;

	char rx_buf[256];
  while (1) {
    // 시리얼포트에서 IVDCT 데이터를 수신한다.
    memset(rx_buf, 0, sizeof(rx_buf));
		// IVDCT 패킷 수신을 확인하기 위해 STX와 payload 길이 4byte씩 수신
    int ret = read(mib->ivdct_if.sock, rx_buf, 4);
		if(ret > 0) {
			// STX 확인
			uint16_t stx = BE16_TO_HOST(*(uint16_t*)(rx_buf));
			if(stx == IVDCTDCMPOROBUFPKT_STX)
			{
				/*
				 * IVDCT 패킷 처리
				 */
				IVDCTDCM_IvdctBasicData *pkt = (IVDCTDCM_IvdctBasicData *)rx_buf;
				uint16_t payload_len = BE16_TO_HOST(*(uint16_t*)((pkt->payload_len)));
				int recv_count = 0;
				// IVDCT 프로토콜 버퍼 직렬화 데이터 수신
				double start_time = clock() / CLOCKS_PER_SEC;
				while (recv_count < payload_len + 2) {
					recv_count +=  read(mib->ivdct_if.sock, rx_buf + 4 + recv_count, payload_len + 2 - recv_count);
					// 수신 시간 초과 체크 무한 루프 방지
					if ((clock() / CLOCKS_PER_SEC) - start_time > 0.5) { // 0.5초 타임아웃
						ERR("Fail to read IVDCT data - timeout\n");
						break;
					}
				}
				// 수신된 데이터 길이 확인 (무한 루프 방지 코드에 의해서 나왔을 때 처리)
				if (recv_count < payload_len + 2) {
					ERR("Fail to read IVDCT data - received %d bytes, expected %d bytes\n", recv_count, payload_len + 2);
					continue;
				}
				uint16_t etx = BE16_TO_HOST(*(uint16_t*)(pkt->payload + payload_len));
				if (etx != IVDCTDCMPOROBUFPKT_ETX) {
					ERR("Fail to read IVDCT data - ETX mismatch: %04X\n", etx);
					continue;
				}
				// IVDCT 프로토콜 버퍼 직렬화 데이터 파싱
				if (IVDCTDCM_ProcessIVDCTData(pkt, payload_len)) {
					mib->io_cnt.ivdctdcm_rx++ % 0XFFFFFFFF;
					LOG(kKVHLOGLevel_InOut, "Read IVDCT data - recv len: %d, payload len: %d, cnt: %u\n", recv_count + 4, payload_len, mib->io_cnt.ivdctdcm_rx);
				}
			} else {
				ERR("Fail to read IVDCT data - STX mismatch: %04X\n", stx);
			}
		} else {
			ERR("Fail to read(sock) - %s(ret: %d)\n", strerror(errno), ret);
			usleep(1 * 100); // 무한로그출력을 방지하기 위한 지연 루틴
    }
    // 쓰레드 종료
    if (mib->ivdct_if.rx_th_running == false) {
      break;
    }
  }

  LOG(kKVHLOGLevel_Init, "Exit IVDCT data rx thread\n");
  return NULL;
}

/**
 * @brief IVDCT와의 I/F를 초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int IVDCTDCM_InitIVDCTIF(IVDCTDCM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize IVDCT I/F\n");

  /*
   * 시리얼포트를 초기화한다.
   */
  mib->ivdct_if.sock = open(mib->ivdct_if.dev_name, O_RDWR);
  if (mib->ivdct_if.sock < 0) {
    ERR("Fail to initialize IVDCT I/F - open(%s)\n", mib->ivdct_if.dev_name);
    return -1;
  }

  /*
   * 시리얼포트를 설정한다.
   */
  if (IVDCTDCM_ConfigureIVDCTIFSerialPort(mib->ivdct_if.sock, mib->ivdct_if.baudrate) < 0) {
    close(mib->ivdct_if.sock);
    return -1;
  }

  /*
   * 시리얼포트 데이터 수신 쓰레드를 생성한다.
   */
  if (pthread_create(&(mib->ivdct_if.rx_th), NULL, IVDCTDCM_IvdctBasicDataRxThread, mib) != 0) {
    close(mib->ivdct_if.sock);
    return -1;
  }

	/*
	 * IVDCT 프로토콜 버퍼 인스턴스를 생성하고 초기화한다. 핸들러 변수를 통해 Protobuf 인스턴스를 관리할 수 있다. 
	 * NULL 입력 시 코드 내 전역변수가 핸들러로 사용된다. 
	 */
	IVDCTDCM_CreatePB(NULL);

  struct timespec req = { .tv_sec = 0, .tv_nsec = 10000000 }, rem; // 쓰레드가 생성될 때까지 대기한다.
  while(mib->ivdct_if.rx_th_running == false) {
    nanosleep(&req, &rem);
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize IVDCT I/F\n");
  return 0;
}


/**
 * @brief IVDCT 프로토콜 버퍼 핸들러를 생성한다.
 */
static void IVDCTDCM_PrintfIVDCTCTypeData(KVHIVDCTData* d)
{
	if (d == NULL) {
		ERR("IVDCT data is NULL\n");
		return;
	}
	printf("IVDCT->system.status_ct: %lu\n", d->system.status_ct);
	printf("IVDCT->system.status: %lu\n", d->system.status);

	printf("IVDCT->driving.speed.wheel_speed_ct: %lu\n", d->driving.speed.wheel_speed_ct);
	printf("IVDCT->driving.speed.wheel_speed: %ld\n", d->driving.speed.wheel_speed);
	printf("IVDCT->driving.speed.yawrate_ct: %lu\n", d->driving.speed.yawrate_ct);
	printf("IVDCT->driving.speed.yawrate: %ld\n", d->driving.speed.yawrate);
	printf("IVDCT->driving.speed.accel_pedal_pos_ct: %lu\n", d->driving.speed.accel_pedal_pos_ct);
	printf("IVDCT->driving.speed.accel_pedal_pos: %ld\n", d->driving.speed.accel_pedal_pos);
	
	printf("IVDCT->driving.engine.torque_1_ct: %lu\n", d->driving.engine.torque_1_ct);
	printf("IVDCT->driving.engine.torque_1: %ld\n", d->driving.engine.torque_1);
	printf("IVDCT->driving.engine.torque_2_ct: %lu\n", d->driving.engine.torque_2_ct);
	printf("IVDCT->driving.engine.torque_2: %ld\n", d->driving.engine.torque_2);
	printf("IVDCT->driving.engine.rpm_ct: %lu\n", d->driving.engine.rpm_ct);
	printf("IVDCT->driving.engine.rpm: %ld\n", d->driving.engine.rpm);
	
	printf("IVDCT->driving.gear.ratio_ct: %lu\n", d->driving.gear.ratio_ct);
	printf("IVDCT->driving.gear.ratio: %ld\n", d->driving.gear.ratio);
	printf("IVDCT->driving.gear.current_ct: %lu\n", d->driving.gear.current_ct);
	printf("IVDCT->driving.gear.current: %ld\n", d->driving.gear.current);
	
	printf("IVDCT->driving.steering_wheel.angle_ct: %lu\n", d->driving.steering_wheel.angle_ct);
	printf("IVDCT->driving.steering_wheel.angle: %ld\n", d->driving.steering_wheel.angle);
	
	printf("IVDCT->driving.brake.pedal_pos_ct: %lu\n", d->driving.brake.pedal_pos_ct);
	printf("IVDCT->driving.brake.pedal_pos: %ld\n", d->driving.brake.pedal_pos);
	printf("IVDCT->driving.brake.temp_warning_ct: %lu\n", d->driving.brake.temp_warning_ct);
	printf("IVDCT->driving.brake.temp_warning: %lu\n", d->driving.brake.temp_warning);
	printf("IVDCT->driving.brake.tcs_ct: %lu\n", d->driving.brake.tcs_ct);
	printf("IVDCT->driving.brake.tcs: %lu\n", d->driving.brake.tcs);
	printf("IVDCT->driving.brake.abs_ct: %lu\n", d->driving.brake.abs_ct);
	printf("IVDCT->driving.brake.abs: %lu\n", d->driving.brake.abs);

	printf("IVDCT->diag.battery.voltage_ct: %lu\n", d->diag.battery.voltage_ct);
	printf("IVDCT->diag.battery.voltage: %ld\n", d->diag.battery.voltage);
	printf("IVDCT->diag.engine.coolant_temp_ct: %lu\n", d->diag.engine.coolant_temp_ct);
	printf("IVDCT->diag.engine.coolant_temp: %ld\n", d->diag.engine.coolant_temp);
	printf("IVDCT->diag.fuel.economy_ct: %lu\n", d->diag.fuel.economy_ct);
	printf("IVDCT->diag.fuel.economy: %ld\n", d->diag.fuel.economy);
	
	printf("IVDCT->lights.hazard_sig_ct: %lu\n", d->lights.hazard_sig_ct);
	printf("IVDCT->lights.hazard_sig: %lu\n", d->lights.hazard_sig);

	for (int i = 1; i < 2; ++i) {
		if(d->weight.axle_weight == NULL)
			break;
		printf("IVDCT->weight.axle_weight[%d].location_ct: %lu\n", i, d->weight.axle_weight[i].location_ct);
		printf("IVDCT->weight.axle_weight[%d].location: %ld\n", i, d->weight.axle_weight[i].location);
		printf("IVDCT->weight.axle_weight[%d].weight_ct: %lu\n", i, d->weight.axle_weight[i].weight_ct);
		printf("IVDCT->weight.axle_weight[%d].weight: %ld\n", i, d->weight.axle_weight[i].weight);
	}
}