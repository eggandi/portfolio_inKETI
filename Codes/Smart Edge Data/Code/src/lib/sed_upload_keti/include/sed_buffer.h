
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32  // Windows 환경
    #include <windows.h>
#else  // POSIX 환경 (Linux/macOS)
    #include <unistd.h>    // POSIX 시스템 함수 (sleep, usleep)
    #include <sys/time.h>  // gettimeofday, struct timeval
    #include <signal.h>    // 시그널 처리
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _DSED_LIB_BUILD_BUFFER
#define _DSED_LIB_BUILD_BUFFER 1

#ifdef _WIN32  // Windows 환경
    #define EXPORT __declspec(dllexport)
#else  // POSIX 환경 (Linux/macOS)
    #define EXPORT EXPORT
#endif

#define SED_BUFFER_INPUT_INT(x, Min, Max) ((x) < (Min) ? (Min) : (x) > (Max) ? (Max) : (x))

#define SED_BUFFER_MESSAGE_DEFAULT_BOLLEAN_RANGE 0, 1
#define SED_BUFFER_MESSAGE_DEFAULT_INT_RANGE 0, 65535
#define SED_BUFFER_MESSAGE_DEFAULT_FLOAT_RANGE 0, 65535.0
#define SED_BUFFER_MESSAGE_MSGID_RANGE 0, 9999
#define SED_BUFFER_MESSAGE_VEHICLE_TYPE_RANGE 0, 255
#define SED_BUFFER_MESSAGE_ROAD_COND_RANGE 0, 255
#define SED_BUFFER_MESSAGE_DROWSY_DRV_RANGE 0, 255
#define SED_BUFFER_MESSAGE_DRIVER_DISTRACTION_RANGE 0, 255
#define SED_BUFFER_MESSAGE_COUNT_RANGE 0, 32


#define SED_BUFFER_MESSAGE_DEFAULT_COUNT 32
#define SED_BUFFER_MESSAGE_DEFAILT_EXPIRED_TIEM 1 // 1초

#define SED_BUFFER_MESSAGE_DEFAULT_TIMESTAMP_SIZE 12 + 1
#define SED_BUFFER_MESSAGE_DEFAULT_VIN_SIZE 17 + 1

 struct __attribute__((__packed__)) sed_buffer_message_vehicle_info_t {
    char vin[17];
    uint8_t null0;
    uint8_t vehicle_type;
    int vehicle_lat;
    int vehicle_lng;
};

struct __attribute__((__packed__)) sed_buffer_message_risky_driving_behaviors_t {
    uint32_t ex_speed:1;
    uint32_t cont_ex_speed:1;
    uint32_t rapid_acc:1;
    uint32_t rapid_start:1;
    uint32_t rapid_dec:1;
    uint32_t rapid_braking:1;
    uint32_t rapid_lane_change:1;
    uint32_t sharp_passing:1;
    uint32_t sharp_left_turn:1;
    uint32_t sharp_right_turn:1;
    uint32_t sharp_u_turn:1;
    uint32_t reserved:21;
};

struct __attribute__((__packed__)) sed_buffer_message_road_condition_t {
    uint32_t road_cond:8;
    uint32_t reserved:24;
};

struct __attribute__((__packed__)) sed_buffer_message_driver_status_monitoring_t {
    uint32_t drowsy_drv:8;
    uint32_t driver_distraction:8;
    uint32_t reserved:16;
};
struct __attribute__((__packed__)) sed_buffer_messages_count_t{
    union{
        struct sed_buffer_message_risky_driving_behaviors_t risky_drv[SED_BUFFER_MESSAGE_DEFAULT_COUNT];
        struct sed_buffer_message_road_condition_t road_event[SED_BUFFER_MESSAGE_DEFAULT_COUNT];
        struct sed_buffer_message_driver_status_monitoring_t dms_event[SED_BUFFER_MESSAGE_DEFAULT_COUNT];
    }msg_type;
#define risky_drv msg_type.risky_drv
#define road_event msg_type.road_event
#define dms_event msg_type.dms_event
    int count;
 };

struct sed_buffer_message_main_t {
    int msgid;
    char timestamp[12];
    uint8_t null0;
    struct sed_buffer_message_vehicle_info_t vehicle_info;
    struct sed_buffer_messages_count_t risky_drv_s;
    struct sed_buffer_messages_count_t road_event_s;
    struct sed_buffer_messages_count_t dms_event_s;
};

struct sed_buffer_message_t
{
    bool isempty;
    struct sed_buffer_message_main_t msg;
    time_t timestamp;
};

enum sed_buffer_message_type_e {
    SED_BUFFER_MESSAGE_MSGID = 0,
    SED_BUFFER_MESSAGE_TIMESTAMP,
    SED_BUFFER_MESSAGE_VEHICLE_INFO,
    SED_BUFFER_MESSAGE_RISKY_DRV_S,
    SED_BUFFER_MESSAGE_ROAD_EVENT_S,
    SED_BUFFER_MESSAGE_DMS_EVENT_S,
};

#endif // !_DSED_LIB_BUILD_BUFFER

EXPORT void SED_Buffer_Init(int *expired_time);
EXPORT void SED_Buffer_Expired_Time_Set(int time);
EXPORT void SED_Buffer_Signal_Expired_Active(bool enable);
EXPORT int SED_Buffer_Push_Data(enum sed_buffer_message_type_e msg_type, void *data, int count);

#ifdef __cplusplus
}
#endif