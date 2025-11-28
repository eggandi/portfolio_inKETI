/**
 * @file
 * @brief DGM과의 I/F 기능(=UDS 통신) 구현
 * @date 2024-10-17
 * @author user
 */


// 모듈 헤더파일
#include "csm.h"


/**
 * @brief DGM으로 UDS를 통해 데이터를 전송한다.
 * @param[in] mib 모듈 통합관리정보
 * @param[in] data 전송할 데이터
 * @param[in] len 전송할 데이터의 길이
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int CSM_TransmitUDSDataToDGM(CSM_MIB *mib, uint8_t *data, KVHIFDataLen len)
{
  LOG(kKVHLOGLevel_Event, "Transmit UDS data(len: %d) to DGM\n", len);
  if (len > kKVHIFDataLen_Max) {
    ERR("Fail to transmit UDS data(len: %d) to DGM - too long: %d > %d\n", len, kKVHIFDataLen_Max);
    return -1;
  }
  if (KVHUDS_Send(mib->dgm_if.uds.h, data, len) < 0) {
    ERR("Fail to transmit UDS data(len: %d) to DGM - KVHUDS_Send()\n", len);
    return -1;
  }
  return 0;
}


/**
 * @brief C-ITS 서비스 데이터 전송 타이머를 초기화한다.
 * @param[in] mib 모듈 관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
static int CSM_InitCITSSvcDataTransmitTimer(CSM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize C-ITS svc data transmit timer\n");

  /*
   * 타이머를 생성한다.
   */
  mib->dgm_if.tx.timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
  if (mib->dgm_if.tx.timerfd < 0) {
    ERR("Fail to initialize C-ITS svc data transmit timer - timerfd_create() failed - %m\n");
    return -1;
  }

  /*
   * 타이머 주기를 설정한다.
   */
  struct itimerspec ts;
  long usec = CSM_CITS_SVC_DATA_TX_INTERNVAL;
  ts.it_value.tv_sec = (time_t)(usec / 1000000);
  ts.it_value.tv_nsec = (long)((usec % 1000000) * 1000);
  ts.it_interval.tv_sec = (time_t)(usec / 1000000);
  ts.it_interval.tv_nsec = (long)((usec % 1000000) * 1000);
  int ret = timerfd_settime(mib->dgm_if.tx.timerfd, TFD_TIMER_ABSTIME, &ts, NULL);
  if (ret < 0) {
    ERR("Fail to initialize C-ITS svc data transmit timer - timerfd_settime() failed - %m\n");
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize C-ITS svc data transmit timer\n");
  return 0;
}


/**
 * @brief DGM으로 C-ITS 서비스 데이터를 전송한다.
 * @param[in] mib 모듈 관리정보
 */
static void CSM_TransmitCITSSvcDataToDGM(CSM_MIB *mib)
{
  KVHRetCode rc;

  /*
   * RSDPR로부터 GNSS 정보를 얻는다.
   */
  KVHHFHubData hub_data;
  rc = KVHRSDPR_ReadHFHubData(mib->rsdpr_h, &hub_data);
  if (rc < 0) {
    ERR("Fail to transmit C-ITS svc data to DGM - KVHRSDPR_ReadHFHubData()\n");
    return;
  }

  /*
   * C-ITS 서비스 데이터를 수납한 json 문자열을 생성한다.
   */
  char *json_str;
  rc = KVHCITS_ConstructSvcDataString(mib->cits.h, &(hub_data.gnss), &json_str);
  if ((rc < 0) || !json_str) {
    ERR("Fail to transmit C-ITS svc data to DGM - KVHCITS_ConstructSvcDataString() - rc: %d\n", rc);
    return;
  }
  STR(kKVHLOGLevel_Dump, "C-ITS svc data string to DGM", json_str);
  KVHCITS_FlushSvcData(mib->cits.h);

  /*
   * UDS 데이터를 생성한다.
   */
  size_t payload_len = strlen(json_str);
  size_t uds_data_len = sizeof(KVHUDSDataCITSSvcDataHdr) + payload_len;
  uint8_t *uds_data = malloc(uds_data_len);
  if (!uds_data) {
    ERR("Fail to transmit C-ITS svc data to DGM - malloc()\n");
    goto out;
  }

  KVHUDSDataCITSSvcDataHdr *hdr = (KVHUDSDataCITSSvcDataHdr *)uds_data;
  hdr->common.dst = kKVHUDSEntityType_IVN_HDEM;
  hdr->common.type = kKVHUDSDataType_CITS_MSG;
  hdr->common.subtype = kKVHUDSDataSubtype_CITS_SVC_DATA;
  hdr->common.len = payload_len;
  memcpy(uds_data + sizeof(KVHUDSDataCITSSvcDataHdr), json_str, payload_len);

  DUMP(kKVHLOGLevel_Dump, "C-ITS svc data UDS to HDEM(via DGM)", uds_data, uds_data_len);

  /*
   * UDS 데이터를 DGM으로 전송한다.
   */
  LOG(kKVHLOGLevel_InOut, "Transmit C-ITS svc data to DGM\n");
  CSM_TransmitUDSDataToDGM(mib, uds_data, uds_data_len);
  free(uds_data);

out:
  free(json_str);
}


/**
 * @brief 데이터 전송 주기에 맞춰 DGM으로 C-ITS 서비스 데이터를 전송하는 쓰레드
 * @param[in] arg 모듈 관리정보
 * @return NULL(프로그램 종료되기 전까지는 리턴되지 않음)
 */
static void * CSM_CITSSvcDataTransmitThread(void *arg)
{
  LOG(kKVHLOGLevel_Init, "Start C-ITS svc data transmit thread\n");

  CSM_MIB *mib = (CSM_MIB *)arg;
  mib->dgm_if.tx.th_running = true;

  /*
   * 타이머 주기마다 깨어나서, DGM으로 데이터를 송신한다.
   */
  uint64_t exp;
  while (1)
  {
    // 타이머 만기까지 대기
    read(mib->dgm_if.tx.timerfd, &exp, sizeof(uint64_t));

    // 쓰레드 종료
    if (mib->dgm_if.tx.th_running == false) {
      break;
    }

    // DGM으로 데이터 송신
    CSM_TransmitCITSSvcDataToDGM(mib);
  }

  LOG(kKVHLOGLevel_Event, "Exit C-ITS svc data transmit thread\n");
  return NULL;
}


/**
 * @brief C-ITS 서비스 데이터 송신 기능을 초기화한다.
 * @param[in] mib 모듈 관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 *
 * @note 각 데이터 주기에 맞춰 데이터를 전송하는 기능이다.
 */
static int CSM_InitCITSSvcDataTransmitFunction(CSM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize C-ITS svc data transmit function\n");

  /*
   * 데이터 전송 타이머를 초기화한다.
   */
  if (CSM_InitCITSSvcDataTransmitTimer(mib) < 0) {
    return -1;
  }

  /*
   * 패킷전송 쓰레드를 생성한다.
   */
  if (pthread_create(&(mib->dgm_if.tx.th), NULL, CSM_CITSSvcDataTransmitThread, mib) != 0) {
    ERR("Fail to initialize C-ITS svc data transmit function - pthread_create()\n");
    return -1;
  }
  struct timespec req = { .tv_sec = 0, .tv_nsec = 10000000 }, rem; // 쓰레드가 생성될 때까지 대기한다.
  while (mib->dgm_if.tx.th_running == false) {
    nanosleep(&req, &rem);
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize C-ITS svc data transmit function\n");
  return 0;
}


/**
 * @brief DGM과의 인터페이스 기능을 초기화한다. (DGM은 UDS 서버, CSM은 UDS 클라이언트로 동작한다)
 * @param[in] mib 모듈 통합관리정보
 * @retval 0: 성공
 * @retval -1: 실패
 */
int CSM_InitDGMIF(CSM_MIB *mib)
{
  LOG(kKVHLOGLevel_Init, "Initialize DGM I/F - UDS server(DGM): %s, UDS client(me): %s\n",
      mib->dgm_if.uds.s_uds_file_path, mib->dgm_if.uds.c_uds_file_path);

  /*
   * UDS 클라이언트 라이브러리를 초기화한다.
   */
  mib->dgm_if.uds.h = KVHUDS_InitClient(mib->dgm_if.uds.s_uds_file_path,
                                        mib->dgm_if.uds.c_uds_file_path,
                                        NULL, // DGM으로부터 수신하는 UDS 데이터는 없으므로 콜백함수를 지정하지 않는다.
                                        mib);
  if (mib->dgm_if.uds.h == NULL) {
    ERR("Fail to initialize DGM I/F - UDS server(DGM): %s, UDS client(me): %s\n",
        mib->dgm_if.uds.s_uds_file_path, mib->dgm_if.uds.c_uds_file_path);
    return -1;
  }

  /*
   * C-ITS 서비스 데이터 전송 기능을 초기화한다.
   */
  if (CSM_InitCITSSvcDataTransmitFunction(mib) < 0) {
    return -1;
  }

  LOG(kKVHLOGLevel_Init, "Success to initialize DGM I/F\n");
  return 0;
}
