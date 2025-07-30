include "./v2x_handover_main.h"

#define V2XHO_ERROR_CODE_FILE_CODE V2XHO_ERROR_CODE_FILE_CODE_UTIL_DASHBOARD

#define V2XHO_DASHBOARD_SIZE_MAX_Y 20
#define V2XHO_DASHBOARD_SIZE_MAX_X 100

extern void* Th_p_V2XHO_Debug_Dashboard_Main_Task(void* d)
{

    int cursor_pos_main[2] = {2, 4};
    int cursor_pos_equip_info[2] = {4, 4};

    f_v_V2XHO_Debug_Dashboard_Cursor_Position(cursor_pos_main[0],cursor_pos_main[1]);
    printf("V2X HANDOVER DASHBOARD");


    while(1)
    {
        usleep(100 * 1000);
    }
    return 0;
}

static void f_v_V2XHO_Debug_Dashboard_Cursor_Position(int x, int y) {
   printf("\033[%d;%dH", y, x);
}

static int f_i_V2XHO_Debug_Dashboard_Check_Window_Size(int *x, int *y) 
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        enum v2xho_error_code_e code = V2XHO_UTILS_DASHBOARD_WINDOW_SIZE_SMALLER;
        return (code * 0x1000000) + (V2XHO_ERROR_CODE_FILE_CODE * 0x010000) + (__LINE__ % 0xFFFF); 
    } else {
        *y = w.ws_row;
        *x = w.ws_col;
        if(V2XHO_DASHBOARD_SIZE_MAX_Y > w.ws_row && V2XHO_DASHBOARD_SIZE_MAX_X > w.ws_col)
        {
            enum v2xho_error_code_e code = V2XHO_UTILS_DASHBOARD_WINDOW_SIZE_SMALLER;
            return (code * 0x1000000) + (V2XHO_ERROR_CODE_FILE_CODE * 0x010000) + (__LINE__ % 0xFFFF); 
        }
    }
    return 0;
}


static void f_v_V2XHO_Debug_Dashboard_Background() {
    for(int cur_y = 0, cur_y < V2XHO_DASHBOARD_SIZE_MAX_Y; cur_y++)
    {
        if (cur_y == 0 || cur_y == V2XHO_DASHBOARD_SIZE_MAX_Y - 1)
        {
            for(int cur_x = 0, cur_x < V2XHO_DASHBOARD_SIZE_MAX_X; cur_x++)
            {
                printf("\033[%d;%dH", cur_y, cur_x);
                printf("*");
            }
        }else{
            printf("\033[%d;%dH", cur_y, 1);
            printf("*");
            printf("\033[%d;%dH", cur_y, V2XHO_DASHBOARD_SIZE_MAX_Y);
            printf("*");
        }
    }
}