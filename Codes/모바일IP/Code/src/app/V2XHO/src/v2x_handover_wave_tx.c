include "v2x_handover_main.h"

#define V2XHO_ERROR_CODE_FILE_CODE V2XHO_ERROR_CODE_FILE_CODE_MAIN

static int f_i_V2XHO_OBU_(void)
{
    int ret;
    ret = WAL_Init(kWalLogLevel_Err);
    if (ret < 0) {
        printf("Fail to init v2X - WAL_Init() failed - %d\n", ret);
        return -1; 
    }
    ret = Dot2_Init(kDot2LogLevel_Err, kDot2SigningParamsPrecomputeInterval_Default, "/dev/random", kDot2LeapSeconds_Default);
    if (ret < 0) {
        printf("Fail to init v2X - Dot2_Init() failed - %d\n", ret);
        return -1; 
    }
    Dot2_RegisterProcessSPDUCallback(WT_ProcessDot2MsgCallback);

    ret = Dot3_Init(kDot3LogLevel_Err);
    if (ret < 0) {
        printf("Fail to init v2X - Dot3_Init() failed - %d\n", ret);
        return -1; 
    }
    Dot3_SetWSMMaxLength(2048);
    return 0;
}