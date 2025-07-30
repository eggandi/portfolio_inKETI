#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _DSED_LIB_BUILD_JSON 
#define _DSED_LIB_BUILD_JSON 1

#ifdef _WIN32  // Windows 환경
    #define EXPORT __declspec(dllexport)
#else  // POSIX 환경 (Linux/macOS)
    #define EXPORT EXPORT
#endif

// 모듈 헤더파일
#include "./cJSON/cJSON.h"

#define SED_JSON_DATA_PTR(ptr_name) if(g_sed_JSON_data.isempty == false){cJSON *ptr_name = g_sed_JSON_data.sed;}
#define SED_JSON_PRINT SED_JSON_DATA_PTR(ptr) printf("%s\n", cJSON_Print(ptr));

struct sed_JSON_key_0_list_t
{
    cJSON *msgid;
    cJSON *timestamp;
    cJSON *vehicle_info;
    cJSON *risky_drv_s;
    cJSON *road_event_s;
    cJSON *dms_event_s;
};

struct g_sed_JSON_data_t{
    bool isempty;
    cJSON *sed;
    struct sed_JSON_key_0_list_t key_0_list;
};

enum sed_JSON_object_type_e {
    SED_JSON_OBJECT_MSGID = 0,
    SED_JSON_OBJECT_TIMESTAMP,
    SED_JSON_OBJECT_VEHICLE_INFO,
    SED_JSON_OBJECT_RISKY_DRV_S,
    SED_JSON_OBJECT_ROAD_EVENT_S,
    SED_JSON_OBJECT_DMS_EVENT_S,
};

#endif // !_DSED_LIB_BUILD_JSON

EXPORT void *SED_JSON_Create_Main_Object();
EXPORT int SED_JSON_Write_Object(enum sed_JSON_object_type_e object_type, void *data);
EXPORT void SED_JSON_Release_Object();
EXPORT int SED_JSON_Print(char **out_buffer);

#ifdef __cplusplus
}
#endif