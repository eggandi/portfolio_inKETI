include "mobileip_rse.h"

int main(int argc, char *argv[])
{




    return 0;
}

static int f_i_MobileIP_RSE_V2X_InitV2X(void)
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