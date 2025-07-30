#define COMUNICATIONS_UDS_DEFAULT_PATH "./communications/uds/"
#define COMUNICATIONS_UDS_DEFAULT_SOCKET_WAVE_RECV_SIGNAL "wave_recv_signal"
#define COMUNICATIONS_UDS_DEFAULT_SOCKET_BUFF_SIZE 1024 * 100

enum v2xho_util_uds_socket_type_e{
    V2XHO_TCP_OBU_RECEIVE_WSMP_DATA = 10,
};

struct v2xho_util_uds_info_t{

    enum v2xho_util_uds_socket_type_e socket_type;
    char socket_path[128];
    char socket_name[64];
    char *socketfd_path;

    int socketfd;
    struct sockaddr_un addr;
};
