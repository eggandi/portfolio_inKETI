/**
 * @file
 * @brief
 * @date 2025-02-06
 * @author user
 */


// 의존 헤더 파일
#include "sed_buffer.h"
#include "sed_json.h"
#include "sed_azure.h"
//#define  printf("[DEBUG][%d][%s]\n", __LINE__, __func__);

struct sed_buffer_message_t g_sed_buffer_message;
int g_buffer_expired_time = 0;
bool g_sed_buffer_signal_expired_active = false;

static int SED_Buffer_Check_Expired();
static void SED_Buffer_Signal_Expired_Handler_Timer_Set();
#ifdef _WIN32
static void CALLBACK SED_Buffer_Signal_Expired_Handler(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
#else
static void SED_Buffer_Signal_Expired_Handler(union sigval sv);
#endif

/**
 * @brief 버퍼 초기화로 사용자가 저장하는 버퍼(전역변수)를 초기화
 * @param expired_time 만료시간
 * @return 없음
 */
EXPORT void SED_Buffer_Init(int *expired_time)
{
    memset(&g_sed_buffer_message.msg, 0, sizeof(struct sed_buffer_message_main_t));
    if(g_sed_buffer_message.isempty == false) {
        memset(&g_sed_buffer_message.msg.vehicle_info, 0, sizeof(struct sed_buffer_message_vehicle_info_t));
        memset(&g_sed_buffer_message.msg.risky_drv_s, 0, sizeof(struct sed_buffer_messages_count_t));
        memset(&g_sed_buffer_message.msg.road_event_s, 0, sizeof(struct sed_buffer_messages_count_t));
        memset(&g_sed_buffer_message.msg.dms_event_s, 0, sizeof(struct sed_buffer_messages_count_t));
    }
    g_sed_buffer_message.isempty = true;
    g_sed_buffer_message.timestamp = 0;

    if(expired_time != NULL) {
        g_buffer_expired_time = *expired_time;
    }else{
        g_buffer_expired_time =SED_BUFFER_MESSAGE_DEFAILT_EXPIRED_TIEM;
    }
    return (void)NULL;
}

/**
 * @brief 버퍼에 저장된 데이터 만료시간 설정
 * @return 없음
 */
EXPORT void SED_Buffer_Expired_Time_Set(int time)
{
    g_buffer_expired_time = time;
    return (void)NULL;
}

/**
 * @brief 버퍼에 저장된 데이터가 만료시간이 지났는지 확인
 * @return 만료시간이 지났으면 -1, 아니면 0
 */
static int SED_Buffer_Check_Expired()
{
    if(g_sed_buffer_message.isempty == false) {
        if(time(NULL) - g_sed_buffer_message.timestamp > g_buffer_expired_time) {
            SED_Buffer_Init(&g_buffer_expired_time);
            SED_JSON_Release_Object();
        }
        return -1;
    }
    return 0;
}

/**
 * @brief 클라우드 서버로 보낼 raw 데이터를 저장한다.
 * @param msg_type 메시지 타입
 * @param data 데이터
 * @param count 여러개의 데이터를 동시에 넣을 때 사용
 * @return 성공시 0, 없는 메시지 타입 -1, 데이터 오류 -2, 만료시간 지남 -3
 */
EXPORT int SED_Buffer_Push_Data(enum sed_buffer_message_type_e msg_type, void *data, int count)
{
    fflush(stdout); 
    // 만료시간 체크
    if(SED_Buffer_Check_Expired() < 0)
    {
        return -3;
    }
    
    // 메시지 타입에 따라 데이터 저장
    msg_type = (enum sed_buffer_message_type_e) msg_type;
    switch(msg_type) {
        case SED_BUFFER_MESSAGE_MSGID:
        {
            g_sed_buffer_message.msg.msgid = *(int *)data;
            SED_JSON_Write_Object((enum sed_JSON_object_type_e)SED_BUFFER_MESSAGE_MSGID, data);
            if(g_sed_buffer_signal_expired_active == true)
            {
                SED_Buffer_Signal_Expired_Handler_Timer_Set();
            }            
            break;
        }
        case SED_BUFFER_MESSAGE_TIMESTAMP:
        {
            memcpy(g_sed_buffer_message.msg.timestamp, (char *)data, SED_BUFFER_MESSAGE_DEFAULT_TIMESTAMP_SIZE);
            SED_JSON_Write_Object((enum sed_JSON_object_type_e)SED_BUFFER_MESSAGE_TIMESTAMP, (void*)g_sed_buffer_message.msg.timestamp);

            break;
        }
        case SED_BUFFER_MESSAGE_VEHICLE_INFO:
        {
            g_sed_buffer_message.msg.vehicle_info = *(struct sed_buffer_message_vehicle_info_t *)data;
            memcpy(g_sed_buffer_message.msg.vehicle_info.vin, (char *)data, SED_BUFFER_MESSAGE_DEFAULT_VIN_SIZE - 1);
            SED_JSON_Write_Object((enum sed_JSON_object_type_e)SED_BUFFER_MESSAGE_VEHICLE_INFO, (void*)data);
            break;
        }
        case SED_BUFFER_MESSAGE_RISKY_DRV_S:
        {
            if(count > 0) {
                g_sed_buffer_message.msg.risky_drv_s.risky_drv[g_sed_buffer_message.msg.risky_drv_s.count] = *(struct sed_buffer_message_risky_driving_behaviors_t *)data;
                SED_JSON_Write_Object((enum sed_JSON_object_type_e)SED_BUFFER_MESSAGE_RISKY_DRV_S, data);
            }else{
                return -2;
            }
            g_sed_buffer_message.msg.risky_drv_s.count += count;
            break;
        }
        case SED_BUFFER_MESSAGE_ROAD_EVENT_S:
        {
            if(count > 0) {
                g_sed_buffer_message.msg.road_event_s.road_event[g_sed_buffer_message.msg.road_event_s.count] = *(struct sed_buffer_message_road_condition_t *)data;
                SED_JSON_Write_Object((enum sed_JSON_object_type_e)SED_BUFFER_MESSAGE_ROAD_EVENT_S, data);
            }else{
                return -2;
            }
            g_sed_buffer_message.msg.road_event_s.count += count;
            break;
        }        
        case SED_BUFFER_MESSAGE_DMS_EVENT_S:
        {
            if(count > 0) {
                g_sed_buffer_message.msg.dms_event_s.dms_event[g_sed_buffer_message.msg.dms_event_s.count] = *(struct sed_buffer_message_driver_status_monitoring_t *)data;
                SED_JSON_Write_Object((enum sed_JSON_object_type_e)SED_BUFFER_MESSAGE_DMS_EVENT_S, data);
            }else{
                return -2;
            }
            g_sed_buffer_message.msg.dms_event_s.count += count;
            break;
        }       
        default:
            return -1;
            break;
    }

    return 0;
}
/**
 * @brief 만료시간이 지난 데이터를 클라우드 서버로 업로드하는 시그널 핸들러 동작을 활성화
 *        만료시간은 buffer msg의 timestamp를 시작으로 g_buffer_expired_time(초)만큼 지난 후 동작
 * @param true 활성화 여부
 * @return 없음
 */
EXPORT void SED_Buffer_Signal_Expired_Active(bool enable)
{
    if(enable == true)
    {
        g_sed_buffer_signal_expired_active = true;
    }else{
        g_sed_buffer_signal_expired_active = false;
    }
    return (void)NULL;
}
/**
 * @brief 만료시간이 지난 데이터를 클라우드 서버로 업로드
 * @return 없음
 */
#ifdef _WIN32
static void CALLBACK SED_Buffer_Signal_Expired_Handler(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
#else
static void SED_Buffer_Signal_Expired_Handler(union sigval sv)
#endif
#ifdef _WIN32
{
    int timer_id = *(int*)lpParameter;
    timer_id = timer_id;
    TimerOrWaitFired = TimerOrWaitFired;

    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    printf("[%lld] Timer expired. Timer_ID:%d\n", *(long long*)&ft, timer_id);
    SED_Azure_Blob_Upload(NULL, NULL, NULL);
    SED_Buffer_Check_Expired();  // 버퍼 만료 처리
}
#else
{
    sv.sival_int = sv.sival_int;
#ifdef _DEBUG_LOG_BUFFER
    if(_DEBUG_LOG_BUFFER)
        printf("[%ld]timer expired. Timer_ID:%d\n", time(NULL), sv.sival_int);
#endif
    SED_Azure_Blob_Upload(NULL, NULL, NULL);
    SED_Buffer_Check_Expired();  // 버퍼 만료 처리
}
#endif

/**
 * @brief 만료시간이 지난 데이터를 클라우드 서버로 업로드하는 시그널 핸들러 동작을 설정
 * @return 없음
 */
static void SED_Buffer_Signal_Expired_Handler_Timer_Set()
#ifdef _WIN32
{
    static int timer_id = 0;
    HANDLE hTimer = NULL;
    HANDLE hTimerQueue = CreateTimerQueue();
    
    if (!hTimerQueue)
    {
        perror("타이머 큐 생성 실패");
        exit(EXIT_FAILURE);
    }

    timer_id = (int)time(NULL);  // 현재 시간을 Timer ID로 사용

    if (!CreateTimerQueueTimer(
        &hTimer, 
        hTimerQueue, 
        (WAITORTIMERCALLBACK)SED_Buffer_Signal_Expired_Handler, 
        &timer_id, 
        g_buffer_expired_time * 1000, // 밀리초 단위
        0, 
        WT_EXECUTEDEFAULT
    ))
    {
        perror("타이머 생성 실패");
        exit(EXIT_FAILURE);
    }
}
#else
{
    timer_t timer_id;
    int id = time(NULL);
    struct sigevent sev;
    struct itimerspec its;

    // 타이머 이벤트 설정
    memset(&sev, 0, sizeof(struct sigevent));  
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = SED_Buffer_Signal_Expired_Handler;
    sev.sigev_value.sival_int = id;

    // 타이머 생성
    if (timer_create(CLOCK_REALTIME, &sev, &timer_id) == -1)
    {
        perror("timer_create 실패");
        exit(EXIT_FAILURE);
    }

    // 타이머 시간 설정
    its.it_value.tv_sec = g_buffer_expired_time;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;

    if (timer_settime(timer_id, 0, &its, NULL) == -1)
    {
        perror("timer_settime 실패");
        exit(EXIT_FAILURE);
    }
}
#endif