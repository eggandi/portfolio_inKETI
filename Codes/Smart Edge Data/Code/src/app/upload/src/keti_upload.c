/**
 * @file
 * @brief
 * @date 2025-02-06
 * @author user
 */

// 의존 헤더 파일
#include "sed_json.h"
#include "sed_azure.h"
#include "sed_buffer.h"

#define _DEBUG_LOG_AZURE 1
#define _DEBUG_LOG_BUFFER 1


#define URL "https://sliceduploadtest.blob.core.windows.net/sliced-data/uploaded_test_5?sp=racwdl&st=2025-01-26T15:05:40Z&se=2026-01-01T23:05:40Z&spr=https&sv=2022-11-02&sr=c&sig=CXjVrtaipY6syiT71zEHzVI66RxkzHwbyCrh3KBhDsk%3D"
#define _DEBUG_LOG printf("[DEBUG][%d][%s]\n", __LINE__, __func__);

int main()
{
    int expried_time = 3;
    // 1) Init Azure Blob client.
    #if USING_BLOB_CLIENT
        az_storage_blobs_blob_client *blob_client = (az_storage_blobs_blob_client *)SED_Azure_Blob_Client_Init(URL);
    #else
        char* const blob_url = URL;//getenv(URL_WITH_SAS);
        SED_Azure_Blob_Client_Init(blob_url);
    #endif
    // 2) Init Buffer and Set Expired Time.
    SED_Buffer_Init(&expried_time);
    // 3) Set Active Expired Signal Handler.
    SED_Buffer_Signal_Expired_Active(true);

    //while(1)
    {
        // 5) Push Data to Buffer.
        srand(time(NULL));
        int item_count = 1;
        struct sed_buffer_message_main_t sed_buffer_message_main; 
        sed_buffer_message_main.msgid = 1;
        memset(sed_buffer_message_main.timestamp, 0x00, SED_BUFFER_MESSAGE_DEFAULT_TIMESTAMP_SIZE);
        memcpy(sed_buffer_message_main.timestamp, "250111111511", SED_BUFFER_MESSAGE_DEFAULT_TIMESTAMP_SIZE - 1);
        memset(sed_buffer_message_main.vehicle_info.vin, 0x00, SED_BUFFER_MESSAGE_DEFAULT_VIN_SIZE);
        memcpy(sed_buffer_message_main.vehicle_info.vin, "12345678901234567", SED_BUFFER_MESSAGE_DEFAULT_VIN_SIZE - 1);
        sed_buffer_message_main.vehicle_info.vehicle_type = (rand() % 4) * 16;
        sed_buffer_message_main.vehicle_info.vehicle_lat = 12331;
        sed_buffer_message_main.vehicle_info.vehicle_lng = 33453;
        sed_buffer_message_main.risky_drv_s.count = 1;
            sed_buffer_message_main.risky_drv_s.risky_drv[0].ex_speed = rand() % 2; 
            sed_buffer_message_main.risky_drv_s.risky_drv[0].cont_ex_speed = rand() % 2; 
            sed_buffer_message_main.risky_drv_s.risky_drv[0].rapid_acc = rand() % 2; 
            sed_buffer_message_main.risky_drv_s.risky_drv[0].rapid_start = rand() % 2; 
            sed_buffer_message_main.risky_drv_s.risky_drv[0].rapid_dec = rand() % 2; 
            sed_buffer_message_main.risky_drv_s.risky_drv[0].rapid_braking = rand() % 2; 
            sed_buffer_message_main.risky_drv_s.risky_drv[0].rapid_lane_change = rand() % 2; 
            sed_buffer_message_main.risky_drv_s.risky_drv[0].sharp_passing = rand() % 2; 
            sed_buffer_message_main.risky_drv_s.risky_drv[0].sharp_left_turn = rand() % 2; 
            sed_buffer_message_main.risky_drv_s.risky_drv[0].sharp_right_turn = rand() % 2; 
            sed_buffer_message_main.risky_drv_s.risky_drv[0].sharp_u_turn = rand() % 2; 
        sed_buffer_message_main.road_event_s.count = 1;
            sed_buffer_message_main.road_event_s.road_event[0].road_cond = rand() % 2; 
        sed_buffer_message_main.dms_event_s.count = 1;
            sed_buffer_message_main.dms_event_s.dms_event[0].drowsy_drv = rand() % 2; 
            sed_buffer_message_main.dms_event_s.dms_event[0].driver_distraction = rand() % 2; 

        SED_Buffer_Push_Data(SED_BUFFER_MESSAGE_MSGID, (void*)&sed_buffer_message_main.msgid, 1);
        SED_Buffer_Push_Data(SED_BUFFER_MESSAGE_TIMESTAMP, (void*)&sed_buffer_message_main.timestamp, 1);
        SED_Buffer_Push_Data(SED_BUFFER_MESSAGE_VEHICLE_INFO, (void*)&sed_buffer_message_main.vehicle_info, 1);
        SED_Buffer_Push_Data(SED_BUFFER_MESSAGE_RISKY_DRV_S, (void*)&sed_buffer_message_main.risky_drv_s, item_count);
        SED_Buffer_Push_Data(SED_BUFFER_MESSAGE_ROAD_EVENT_S, (void*)&sed_buffer_message_main.road_event_s, item_count);
        SED_Buffer_Push_Data(SED_BUFFER_MESSAGE_DMS_EVENT_S, (void*)&sed_buffer_message_main.dms_event_s, item_count);

        // 6) Parsing JSON Data(It's made by buffer that ).
        char *out_buffer = NULL;
        int ret = SED_JSON_Print(&out_buffer);
        printf("SED_JSON_Presing:%s\n", ret == 0 ? "Success" : "Error");  
        free(out_buffer);
        Sleep(1 * 1000);
    }

    Sleep(5 * 1000);
    return 0;
}