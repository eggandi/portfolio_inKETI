#include "common_include.h"
#include "common_uds_define.h"

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <pthread.h>

#include <sys/timerfd.h>
#include <dirent.h>
#include <fcntl.h>

#include <signal.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <time.h>

#ifndef __USE_GNU 
    #define __USE_GNU
#endif 
#include <fcntl.h>

#ifndef CUMMNICATION_LIST_CODE_UDS
    #define CUMMNICATION_LIST_CODE_UDS 0x00010000
#endif

#define COMUNICATIONS_UDS_DEFAULT_PATH "./communications/uds/"
#define COMUNICATIONS_UDS_DEFAULT_SIFM_NAME "sifm"
#define COMUNICATIONS_UDS_DEFAULT_SIDM_NAME "sidm"
#define COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_NAME "req_clients_to_server"
#define COMUNICATIONS_UDS_DEFAULT_REQUEST_DATA_STX "REQs"
#define COMUNICATIONS_UDS_DEFAULT_REQUEST_SOCKET_BUFF_SIZE 256
#define COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_NAME "res_server_to_client"
#define COMUNICATIONS_UDS_DEFAULT_RESPONSE_DATA_STX "RESs"
#define COMUNICATIONS_UDS_DEFAULT_RESPONSE_SOCKET_BUFF_SIZE 256

#define COMUNICATIONS_UDS_DEFAULT_UPSTREAM_SOCKET_BUFF_SIZE 1024 * 100
#define COMUNICATIONS_UDS_DEFAULT_DOWNSTREAM_SOCKET_BUFF_SIZE 1024 * 100


enum VH_UDS_Path_Type_e
{
    UDS_PATH_TYPE_NOUSE = 0,
    UDS_PATH_TYPE_RAMDISK,
    UDS_PATH_TYPE_FLASH,
    
};
enum VH_UDS_Server_Type_e
{
    UDS_NOSERVER_CLIENT = 0,
    UDS_SERVER_SIFM = 10,
    UDS_SERVER_SIDM = -10,
};
struct VH_UDS_Info_Setup_t
{
    enum VH_UDS_Path_Type_e path_type;
    enum VH_UDS_Server_Type_e server_type;
    const char *socket_path;
    const char *request_socket_name;
    int request_socket_buff_size;
    const char *response_socket_name;
    int response_socket_buff_size;
};
enum VH_UDS_Socket_Triffic_Direction_e
{
    TRIFFIC_TO_SERVER = 10,
    TRIFFIC_TO_CLIENT = -10,
    TRIFFIC_TO_SIFM = 20,
    TRIFFIC_TO_SIDM = -20,
    TRIFFIC_FROM_SIFM = 25,
    TRIFFIC_FROM_SIDM = -25,
    TRIFFIC_NON_DIRECTION = 0,
};
enum VH_UDS_Client_State_e{
    REQUEST_FROM_CLIENT = 1,
    WAITING_RECEIVER_TASK,
    WORKING_RECEIVER_TASK,
    RESPONES_TO_CLIENT = 5,
    WAITING_SENDER_TASK,
    WORKING_SENDER_TASK,
};

struct VH_UDS_Client_info_t
{
    char *client_name;
    enum VH_UDS_Socket_Triffic_Direction_e direction;
    enum VH_UDS_Client_State_e state;
    char *upstream_sockfd;
    uint32_t upstream_sockfd_buff_size;
    char *downstream_sockfd;
    uint32_t downstream_sockfd_buff_size;
    
};
struct VH_UDS_Client_list_t
{
    struct VH_UDS_Client_info_t *client;

    uint32_t *client_num;
    struct VH_UDS_Client_list_t *next;
    struct VH_UDS_Client_list_t *prev;
    
};
struct VH_UDS_Info_t
{
    struct VH_UDS_Info_Setup_t info_setup;

    bool *list_used;
    struct VH_UDS_Client_list_t *clients;
};

struct VH_UDS_Recv_Info_t{
    int target_sock;
    struct VH_UDS_Client_info_t *client;
    void* (*th_func_ptr)(void* d);
    void *th_input;

    pthread_t *recv_thid;
};


struct VH_UDS_Request_t
{
    char STX[4];
    char equip_name[64];
    enum VH_UDS_Socket_Triffic_Direction_e direction;
    pid_t pid;
};

struct VH_UDS_Response_t
{
    char STX[4];
    char socketfd[256];
    char signalfd[256];
    pid_t pid;
    int sock;
};

extern int F_i_VH_Communication_UDS_Server_Setup(struct VH_UDS_Info_Setup_t *setup_info);
extern int F_i_VH_Communication_UDS_Socket_Bind(char* file_path);
extern int F_i_VH_Communication_UDS_Sendto(char* data, size_t data_len, int flag, char* targetfd);
extern int F_i_VH_Communication_UDS_Th_Create_Recvfrom(void *info);
extern int F_i_VH_Communication_UDS_Request(char *request_socket_path, char *response_socket_path, enum VH_UDS_Socket_Triffic_Direction_e *socket_direction, char *equip_name, size_t equip_name_len, struct VH_UDS_Response_t *out_uds_info);

extern int F_i_VH_Communication_UDS_Release_Client(struct VH_Common_List_t *UDS_client_list , void *client_info);

extern void *Th_v_VH_Communication_UDS_Respone(void *info);