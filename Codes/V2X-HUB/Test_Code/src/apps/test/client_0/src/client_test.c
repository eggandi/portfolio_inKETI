#include "client_test.h"

int main()
{   
    int ret;
    int sock;
    struct VH_UDS_Info_t *uds_info = (struct VH_UDS_Info_t*)malloc(sizeof(struct VH_UDS_Info_t));
    struct VH_UDS_Info_Setup_t *info = (struct VH_UDS_Info_Setup_t*)&(uds_info->info_setup);

    info->path_type = UDS_PATH_TYPE_NOUSE;
    info->server_type = UDS_NOSERVER_CLIENT;

    ret = F_i_VH_Communication_UDS_Server_Setup(info);
    enum VH_UDS_Socket_Triffic_Direction_e sock_direction = TRIFFIC_TO_SERVER;
    char *equip_name = "TEST";
    struct VH_UDS_Response_t res_data;
    ret = F_i_VH_Communication_UDS_Request(NULL, NULL, &sock_direction, equip_name, sizeof(equip_name), &res_data);
    if(ret > 0)
    {
        ret = F_i_VH_Communication_UDS_Socket_Bind(res_data.socketfd);
    }
    if(ret > 0)
    {
        sock = ret;
    }

    while(1)
    {
        printf("sock:%d\n", sock);
        sleep(1);
    }
    close(res_data.sock);
    close(sock);
    return 0;
}
