include "./v2x_handover_main.h"

#define V2XHO_ERROR_CODE_FILE_CODE V2XHO_ERROR_CODE_FILE_CODE_UTIL_DASHBOARD

#define V2XHO_DASHBOARD_SIZE_MAX_Y 20
#define V2XHO_DASHBOARD_SIZE_MAX_X 100

extern void* Th_p_V2XHO_(void* d)
{

    int cursor_pos_main[2] = {2, 4};
    int cursor_pos_equip_info[2] = {4, 4};

    f_v_V2XHO_Debug_Dashboard_Cursor_Position(cursor_pos_main[0],cursor_pos_main[1]);
    printf("V2X HANDOVER DASHBOARD");


    while(1)
    {
        usleep(100 * 1000);
    return 0;
    }
}
