/**
 * @file
 * @brief
 * @date 2025-02-06
 * @author user
 */

// 의존 헤더 파일
#include "sed_json.h"
#include "sed_buffer.h"
#define  _DEBUG_LOG printf("[DEBUG][%d][%s]\n", __LINE__, __func__);

struct g_sed_JSON_data_t g_sed_JSON_data;
bool g_sed_JSON_data_created = false;


/**
 * @brief JSON 객체(전역변수) 생성
 * @return g_sed_JSON_data.sed
 */
EXPORT void *SED_JSON_Create_Main_Object()
{
    if(g_sed_JSON_data.isempty == false) {
        cJSON_Delete(g_sed_JSON_data.sed);
    }
    g_sed_JSON_data.isempty = false;
    g_sed_JSON_data.sed = cJSON_CreateObject();
    if (g_sed_JSON_data.sed  == NULL) {
        return (void*)NULL;
    }else{
        #if 0
        g_sed_JSON_data.key_0_list.risky_drv_s = cJSON_CreateArray();
        g_sed_JSON_data.key_0_list.road_event_s = cJSON_CreateArray();
        g_sed_JSON_data.key_0_list.dms_event_s = cJSON_CreateArray();
        cJSON_AddItemToObject(g_sed_JSON_data.sed, "risky_drv", g_sed_JSON_data.key_0_list.risky_drv_s);
        cJSON_AddItemToObject(g_sed_JSON_data.sed, "road_event", g_sed_JSON_data.key_0_list.road_event_s);
        cJSON_AddItemToObject(g_sed_JSON_data.sed, "dms_event", g_sed_JSON_data.key_0_list.dms_event_s);
        #endif
    }
    return (void*)g_sed_JSON_data.sed;
}

/**
 * @brief JSON 객체(전역변수) 해제
 * @return 없음
 */
EXPORT void SED_JSON_Release_Object()
{
    if(g_sed_JSON_data.isempty == false) {
        cJSON_Delete(g_sed_JSON_data.sed);
    }
    g_sed_JSON_data.isempty = true;
    g_sed_JSON_data_created = false;
    return (void)NULL;
}

/**
 * @brief JSON 객체(전역변수)에 데이터 추가
 * @param object_type 추가할 데이터 타입
 * @param data 추가할 데이터
 * @return 성공 시 0, 실패 -1
 */
EXPORT int SED_JSON_Write_Object(enum sed_JSON_object_type_e object_type, void *data)
{
    if(data == NULL) {
        return -1;
    }
// JSON 객체가 생성되지 않았다면 생성
    if(g_sed_JSON_data_created == false) {
        SED_JSON_Create_Main_Object();
        g_sed_JSON_data_created = true;
    }

// JSON 객체에 데이터 추가
    switch(object_type) {
        case SED_JSON_OBJECT_MSGID:
        {
            g_sed_JSON_data.key_0_list.msgid = cJSON_CreateNumber(*(int *)data);
            cJSON_AddItemToObject(g_sed_JSON_data.sed, "msgid", g_sed_JSON_data.key_0_list.msgid);
            break;
        }
        case SED_JSON_OBJECT_TIMESTAMP:
        {
            g_sed_JSON_data.key_0_list.timestamp = cJSON_CreateString((char *)data);
            cJSON_AddItemToObject(g_sed_JSON_data.sed, "timestamp", g_sed_JSON_data.key_0_list.timestamp);
            break;
        }
        case SED_JSON_OBJECT_VEHICLE_INFO:
        {
            g_sed_JSON_data.key_0_list.vehicle_info = cJSON_CreateObject();
            cJSON_AddItemToObject(g_sed_JSON_data.sed, "vehicle_info", g_sed_JSON_data.key_0_list.vehicle_info);
            struct sed_buffer_message_vehicle_info_t *vehicle_info = (struct sed_buffer_message_vehicle_info_t *)data;
            cJSON *vin = cJSON_CreateString((char *)vehicle_info->vin);
            cJSON *vehicle_type = cJSON_CreateNumber((int)vehicle_info->vehicle_type);
            cJSON *vehicle_lat = cJSON_CreateNumber((int)vehicle_info->vehicle_lat);
            cJSON *vehicle_lng = cJSON_CreateNumber((int)vehicle_info->vehicle_lng);
            cJSON_AddItemToObject(g_sed_JSON_data.key_0_list.vehicle_info, "vin", vin);
            cJSON_AddItemToObject(g_sed_JSON_data.key_0_list.vehicle_info, "vehicle_type", vehicle_type);
            cJSON_AddItemToObject(g_sed_JSON_data.key_0_list.vehicle_info, "vehicle_lat", vehicle_lat);
            cJSON_AddItemToObject(g_sed_JSON_data.key_0_list.vehicle_info, "vehicle_lng", vehicle_lng);
            break;
        }
        case SED_JSON_OBJECT_RISKY_DRV_S:
        {
            cJSON *risky_drv_s = cJSON_CreateObject();
            cJSON *items[11];
            int bitfiled = *(int*)data;
            int bitptr = 1;
            for(int i = 0; i < 11; i++)
            {
                items[i] = cJSON_CreateNumber(((bitfiled/bitptr) % 2));
                bitptr *= 2;
            }
            //cJSON_AddItemToArray(g_sed_JSON_data.key_0_list.risky_drv_s, risky_drv);
            cJSON_AddItemToObject(g_sed_JSON_data.sed, "risky_drv", risky_drv_s);
            cJSON_AddItemToObject(risky_drv_s, "ex_speed", items[0]);
            cJSON_AddItemToObject(risky_drv_s, "cont_ex_speed", items[1]);
            cJSON_AddItemToObject(risky_drv_s, "rapid_acc", items[2]);
            cJSON_AddItemToObject(risky_drv_s, "rapid_start", items[3]);
            cJSON_AddItemToObject(risky_drv_s, "rapid_dec", items[4]);
            cJSON_AddItemToObject(risky_drv_s, "rapid_braking", items[5]);
            cJSON_AddItemToObject(risky_drv_s, "rapid_lane_change", items[6]);
            cJSON_AddItemToObject(risky_drv_s, "sharp_passing", items[7]);
            cJSON_AddItemToObject(risky_drv_s, "sharp_left_turn", items[8]);
            cJSON_AddItemToObject(risky_drv_s, "sharp_right_turn", items[9]);
            cJSON_AddItemToObject(risky_drv_s, "sharp_u_turn", items[10]);
            break;
        }
        case SED_JSON_OBJECT_ROAD_EVENT_S:
        {
            cJSON *road_event_s = cJSON_CreateObject();
            cJSON *items[1];
            int bitfiled = *(int*)data;
            int bitptr = 1;
            for(int i = 0; i < 1; i++)
            {
                items[i] = cJSON_CreateNumber(((bitfiled/bitptr) % 256));
                bitptr *= 256;
            }
            //cJSON_AddItemToArray(g_sed_JSON_data.key_0_list.road_event_s, road_event);
            cJSON_AddItemToObject(g_sed_JSON_data.sed, "road_event", road_event_s);
            cJSON_AddItemToObject(road_event_s, "road_cond", items[0]);
            break;
        }        
        case SED_JSON_OBJECT_DMS_EVENT_S:
        {
            cJSON *dms_event_s = cJSON_CreateObject();
            cJSON *items[2];
            int bitfiled = *(int*)data;
            int bitptr = 1;
            for(int i = 0; i < 2; i++)
            {
                items[i] = cJSON_CreateNumber(((bitfiled/bitptr) % 256));
                bitptr *= 256;
            }
            //cJSON_AddItemToArray(g_sed_JSON_data.key_0_list.road_event_s, dms_event);
            cJSON_AddItemToObject(g_sed_JSON_data.sed, "dms_event", dms_event_s);
            cJSON_AddItemToObject(dms_event_s, "drowsy_drv", items[0]);
            cJSON_AddItemToObject(dms_event_s, "driver_distraction", items[1]);
            break;
        }       
        default:
            return -1;
            break;
    }

    return 0;
}
/**
 * @brief JSON 객체(전역변수) 출력
 * @param out_buffer 출력 버퍼
 * @return 성공 시 0, 실패 -1, -2
 */
EXPORT int SED_JSON_Print(char **out_buffer)
{

    if(*out_buffer == NULL)
    {
        *out_buffer = cJSON_Print(g_sed_JSON_data.sed);
        if(*out_buffer == NULL)
        {
            return -1;
        }
    }else{
        return -2;
    }

    return 0;
}
