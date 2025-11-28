/**
 * @file
 * @brief DMS와의 I/F 기능 구현
 * @date 2025-07-29
 * @author user
 */


// 모듈 헤더파일
#include "dmsdcm.h"


/**
 * @brief DMS I/F 시리얼포트를 설정한다.
 * @param[in] sock DMS I/F 시리얼포트 소켓
 * @param[in] baudrate 설정할 baudrate
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int DMSDCM_ConfigureDMSIFSerialPort(int sock, long baudrate)
{
  LOG(kKVHLOGLevel_Init, "Configure DMS I/F serial port\n");

  /*
   * 시리얼포트를 설정한다.
   */
  struct termios newtio;
  memset(&newtio, 0, sizeof(newtio));
  newtio.c_iflag &= ~(IGNCR | INLCR | ICRNL);
  newtio.c_lflag &= ~ICANON;
  newtio.c_lflag &= ~(ECHO | ECHOCTL | ECHONL);
  newtio.c_cc[VMIN] = DMSDCM_BASIC_TX_MSG_LEN;
  newtio.c_cc[VTIME] = 0;
  newtio.c_cflag |=  B115200 | CREAD | CLOCAL;

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
    default:
      ERR("Fail to configure DMS I/F serial port - invalid baudrate %ld\n", baudrate);
      return -1;

  }
  tcflush(sock, TCIFLUSH);
  tcsetattr(sock, TCSANOW, &newtio);

  LOG(kKVHLOGLevel_Init, "Success to configure DMS I/F serial port\n");
  return 0;
}


/**
 * @brief MDSM-7에서 출력된 기본 데이터가 유효한지 확인한다.
 * @param[in] hdr 프르토콜 헤더
 * @return 유효한지 여부
 */
static bool DMSDCM_CheckMDSM7BasicDataValidity(DMSDCM_MDSM7BasicData *hdr)
{
  int ret = true;
  if ((hdr->stx_1 != DMSDCM_MDSM7_BASIC_DATA_STX_1) ||
      (hdr->hdr_1 != DMSDCM_MDSM7_BASIC_DATA_HDR_1) ||
      (hdr->hdr_2 != DMSDCM_MDSM7_BASIC_DATA_HDR_2)) {
    ERR("Fail to check MDSM-7 Basic data validity - STX1:%02X, HDR1:%02X, HDR2:%02X\n",
        hdr->stx_1, hdr->hdr_1, hdr->hdr_2);
    ret = false;
  }
  return ret;
}


/**
 * @brief MDSM-7에서 출력된 이벤트 정보를 DriverStatus 정보로 변환한다.
 * @param[in] event 이벤트 정보
 * @return 변환된 DriverStatus 정보
 *
 * @note MDSM-7에서 출력되는 데이터 형식과 DriverStatus 정보형식은 동일하므로 그대로 반환한다.
 */
static inline KVHDMSDriverStatus DMSDCM_ConvertMDSM7EventToDrvierStatus(uint8_t event)
{
  return event;
}


/**
 * @brief 시리얼포트를 통해 MDSM-7 데이터를 수신하는 쓰레드
 * @param[in] arg 라이브러리 관리정보(MIB)
 * @return NULL(프로그램 종료되기 전까지는 리턴되지 않음)
 */
static void * DMSDCM_MDSM7DataRxThread(void *arg)
{
  LOG(kKVHLOGLevel_Init, "Start DMS data rx thread\n");

  DMSDCM_MIB *mib = (DMSDCM_MIB *)arg;
  mib->dms_if.rx_th_running = true;

  char rx_buf[1024];
  while (1)
  {
    // 시리얼포트에서 DMS 데이터를 수신한다.
    memset(rx_buf, 0, sizeof(rx_buf));
    int ret = read(mib->dms_if.sock, rx_buf, sizeof(rx_buf)-1);
    if (ret > 0) {

      // 수신된 데이터가 유효한지 확인한다.
      DMSDCM_MDSM7BasicData *hdr = (DMSDCM_MDSM7BasicData *)rx_buf;
      mib->io_cnt.dms_rx++;
      KVHLOGLevel lv = ((mib->io_cnt.dms_rx % 10) == 1) ? kKVHLOGLevel_InOut : kKVHLOGLevel_Event;
      LOG(lv, "Read DMS data - recv len: %d, payload len: %d, cnt: %u\n", ret, hdr->len - 0x30, mib->io_cnt.dms_rx);
      DUMP(kKVHLOGLevel_Dump, "DMS Data", (uint8_t *)hdr, ret);
      if (DMSDCM_CheckMDSM7BasicDataValidity(hdr)) {

        // 이벤트 데이터를 운전자상태정보로 변환 후 DGM으로 전달한다.
        KVHDMSDriverStatus status = DMSDCM_ConvertMDSM7EventToDrvierStatus(hdr->dsm_event);
        DMSDCM_TransmitDriverStatusToDGM(mib, status);
      }
    } else {
      ERR("Fail to read(sock) - %s(ret: %d)\n", strerror(errno), ret);
      sleep(1); // 무한로그출력을 방지하기 위한 지연 루틴

      // TODO:: 에러의 종류에 따라 소켓 재생성 루틴 호출
    }

    // 쓰레드 종료
    if (mib->dms_if.rx_th_running == false) {
      break;
    }
  }

  LOG(kKVHLOGLevel_Init, "Exit DMS data rx thread\n");
  return NULL;
}


/**
 * @brief DMS와의 I/F를 초기화한다.
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int DMSDCM_InitDMSIF(DMSDCM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize DMS I/F\n");

  /*
   * 시리얼포트를 초기화한다.
   */
  mib->dms_if.sock = open(mib->dms_if.dev_name, O_RDWR);
  if (mib->dms_if.sock < 0) {
    ERR("Fail to initialize DMS I/F - open(%s)\n", mib->dms_if.dev_name);
    return -1;
  }

  /*
   * 시리얼포트를 설정한다.
   */
  if (DMSDCM_ConfigureDMSIFSerialPort(mib->dms_if.sock, mib->dms_if.baudrate) < 0) {
    close(mib->dms_if.sock);
    return -1;
  }

  /*
   * 시리얼포트 데이터 수신 쓰레드를 생성한다.
   */
  if (pthread_create(&(mib->dms_if.rx_th), NULL, DMSDCM_MDSM7DataRxThread, mib) != 0) {
    close(mib->dms_if.sock);
    return -1;
  }
  struct timespec req = { .tv_sec = 0, .tv_nsec = 10000000 }, rem; // 쓰레드가 생성될 때까지 대기한다.
  while (mib->dms_if.rx_th_running == false) {
    nanosleep(&req, &rem);
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize DMS I/F\n");
  return 0;
}
