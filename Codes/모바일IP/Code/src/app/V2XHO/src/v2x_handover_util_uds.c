#include "v2x_handover_main.h"

#define V2XHO_ERROR_CODE_FILE_CODE V2XHO_ERROR_CODE_FILE_CODE_UTIL_UDS

extern int F_i_V2XHO_Util_UDS_Create_Socket_File(char *path, int force)
{
    int ret = open(path, O_RDWR | O_CREAT | O_NONBLOCK, force);
    if(ret == -1)
    {
        lreturn(V2XHO_UTIL_UDS_SOCKET_FILE_NOT_OPEN);
    }else{
        close(ret);
        ret = access(path, F_OK);
        if(ret == 0)
            unlink(path);
    }
    
    return 0;
}

extern int F_i_V2XHO_Util_UDS_Socket_Bind(char* file_path)
{
    int ret;
    int sock;
    struct sockaddr_un target_addr;
    ret = access(file_path, F_OK);
    if(ret == 0)
        unlink(file_path);

    sock  = socket(PF_FILE, SOCK_DGRAM, 0);
    memset(&target_addr, 0, sizeof( target_addr));
    target_addr.sun_family = AF_UNIX;
    strcpy(target_addr.sun_path, file_path);
    if( -1 == bind( sock, (struct sockaddr*)&target_addr, sizeof(target_addr)) )
    {
        return -1;
    }
    return sock;
}
