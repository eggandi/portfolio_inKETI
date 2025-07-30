/* Base Include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

  /* Epoll */
  #include <poll.h>
  #include <sys/epoll.h>
  /* Timer with timerfd */
  #include <sys/timerfd.h>

/* Util DebugPrintf */
#include <sys/time.h>
#include <stdarg.h>

/* Util IPV4_Task */
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <./memory_allocation_include.h>
#include <./memory_allocation_api.h>
#include <./memory_allocation_param.h>

#include <./parson.h>

#include <curl/curl.h>

#include "protobufabs.h"
#include <protobuf.pb-c.h>

#define NOT_CODE_FINISH 0

#define MAX_CLIENT_SIZE 128
#define SOCKET_TIMER 100 // 100ms
#define DeviceName "eth1"

#define HEADER_SIZE 20
#define HEADER_PAD "%01X%01X%08X%02X%08X"
#define TCP_RECV_BUFFER_SIZE 1024

#define Relay_safefree(ptr) do { free((ptr)); (ptr) = NULL;} while(0)

enum debug_lever_e{
    ERROR,
    LEVEL_1,
    LEVEL_2,
};

// Job Source Code
enum payload_type_e{
    Fireware,
    Program,
};

struct client_data_info_t 
{
    uint8_t ID[8];
    uint8_t Division[1];
    uint8_t Version[8];
    enum payload_type_e Payload_Type;
    //size_t Payload_Size;
};

struct data_header_info_t
{
    uint8_t Job_State;
    uint8_t Protocol_Type;
    uint32_t Client_fd;
    uint16_t Message_seq;
    uint32_t Message_size;
};

enum job_type_e{
    Initial,
    Finish,

    FirmwareInfoReport = 2,
    FirmwareInfoRequest,
    FirmwareInfoResponse,
    FirmwareInfoIndication = 5,
    FirmwareInfoResponseIndication,

    ProgramInfoReport = 7,
    ProgramInfoRequest,
    ProgramInfoResponse,
    ProgramInfoIndication = 10,
    ProgramInfoResponseIndication,

    HandOverReminingData
};

enum socket_type_e{
    SERVER_SOCKET,
    CLIENT_SOCKET,
    HTTP,
};

struct socket_info_t
{
    //Sokket_Info
    enum socket_type_e Socket_Type;
    int Socket;
    char *Device_Name;
    char Device_IPv4_Address[40];
    int Port;
    struct sockaddr_in Socket_Addr;

    pthread_t Task_ID;
};

struct clients_info_t
{
    pthread_mutex_t mtx;
    int *connected_client_num;
    int socket[MAX_CLIENT_SIZE];
    uint32_t socket_message_seq[MAX_CLIENT_SIZE];
    uint32_t Life_Timer[MAX_CLIENT_SIZE];
    enum job_type_e socket_job_state[MAX_CLIENT_SIZE];
    struct client_data_info_t client_data_info[MAX_CLIENT_SIZE];
};

struct Keti_UDP_Header_t
{
    uint8_t STX;
    uint8_t ETX;
    char type[2];
    uint32_t div_len;
    uint32_t div_num;
    uint32_t total_data_len;
};

struct ticktimer_t
{
    uint32_t G_10ms_Tick;
    uint32_t G_100ms_Tick;
    uint32_t G_1000ms_Tick;
    pthread_t Th_TickTimer;
};

struct MemoryStruct {
  char *memory;
  size_t size;
};

extern enum debug_lever_e G_Debug_Level;
extern struct ticktimer_t G_TickTimer;
extern struct clients_info_t G_Clients_Info;
extern struct Memory_Used_Data_Info_t G_Data_Info;


extern struct socket_info_t F_s_RelayServer_TcpIp_Initial_Server(char *Device_Name, int *Port, int *err);
extern int F_i_RelayServer_TcpIp_Get_Address(char *Device_Name, char Output_IPv4Adrress[40]);
extern int F_i_RelayServer_TcpIp_Task_Run(struct socket_info_t *Socket_Info);

 void* th_RelayServer_TcpIp_Task_Server(void *socket_info);
 int f_i_RelayServer_TcpIp_Bind(int *Server_Socket, struct sockaddr_in Socket_Addr);
 int f_i_RelayServer_TcpIp_Setup_Socket(int *Socket, int Timer, bool Linger);

//General Type Code
 int f_i_Hex2Dec(char data);
 struct data_header_info_t f_s_Parser_Data_Header(char *Data, size_t Data_Size);

extern void F_RelayServer_Print_Debug(enum debug_lever_e Debug_Level, const char *format, ...);
extern void* Th_i_RelayServer_TickTimer(void *Data);

extern void *Th_RelayServer_Job_Task(void *Data);

enum job_type_e f_e_RelayServer_Job_Process_Do(struct data_header_info_t *Now_Header, uint8_t **Data, int Client_is, struct Memory_Used_Data_Info_t *Data_Info);
 struct client_data_info_t f_s_RelayServer_Job_Process_Initial(struct data_header_info_t *Now_Header, uint8_t *Data, int *err);
 int f_i_RelayServer_Job_Process_InfoReport(struct data_header_info_t *Now_Header, uint8_t *Data);
 int f_i_RelayServer_Job_Process_InfoRequest(struct data_header_info_t *Now_Header, uint8_t **Data, struct Memory_Used_Data_Info_t *Data_Info);
 int f_i_RelayServer_Job_Process_InfoResponse(struct data_header_info_t *Now_Header, uint8_t **Data);
 int f_i_RelayServer_Job_Process_InfoIndication(struct data_header_info_t *Now_Header, uint8_t **Data);
 int f_i_RelayServer_Job_Process_Finish(struct data_header_info_t *Now_Header, uint8_t *Data, int Client_is);
 int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms);
 int f_i_RelayServer_HTTP_Check_URL(const char *url);



#define DEFAULT_HTTP_INFO_SIZE 512
#define DEFAULT_HTTP_METHOD "POST"
#define DEFAULT_HTTP_SERVER_FIREWARE_URL  "https:/""/itp-self.wtest.biz/v1/system/verCheckForFiles.php"
#define DEFAULT_HTTP_SERVER_PROGRAM_URL  "https:/""/itp-self.wtest.biz/v1/system/verCheckForFiles.php"
#define DEFAULT_HTTP_VERSION "1.1"
#define DEFAULT_HTTP_ACCEPT "*/""*"
#define DEFAULT_HTTP_CONTENT_TYPE "Application/octet-stream"
#define DEFAULT_HTTP_FROMDATA_BOUNDARY "----RelayServerFormBoundary"
#define DEFAULT_HTTP_HOST "itp-self.wtest.biz"
#define HTTP_BUFFER_SIZE 10240
#define HTTP_SOCKET_TIMEOUT 1000L //ms
#define MAX_UDP_RECV_DATA 2048

struct http_socket_info_t{
    uint8_t *request;
    size_t request_len;

    struct data_header_info_t *Now_Header; 
    struct Memory_Used_Data_Info_t *Data_Info;

    pthread_t Task_ID;
    int Timer;
};

struct http_request_line{
    char *Method;
    char *To;
    char *What;
    char *Version;
};
struct http_info_t
{
    struct http_request_line Request_Line;
    char *HOST;
    char *PORT;
    char *ACCEPT;
    char *CONTENT_TYPE;
};

extern uint8_t *G_HTTP_Request_Info_Program;
extern uint8_t *G_HTTP_Request_Info_Fireware;

extern int F_i_RelayServer_HTTP_Initial(uint8_t *G_HTTP_Request_Info, struct http_info_t *http_info);
size_t f_i_RelayServer_HTTP_Payload(uint8_t *G_HTTP_Request_Info, char *Body, size_t Body_Size, uint8_t **Http_Request);

void f_v_RelayServer_HTTP_Message_Parser(char *data_ptr, char *compare_word, void **ret, size_t *ret_len);
int f_i_RelayServer_HTTP_Task_Run(struct data_header_info_t *Now_Header, struct http_socket_info_t *http_socket_info, uint8_t **out_data);
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
void *th_RelayServer_HTTP_Task_Receive(void *data);
int f_i_RelayServer_HTTP_WaitOnSocket(curl_socket_t sockfd, int for_recv, long timeout_ms);
static char* f_c_RelayServer_HTTP_Json_Object_Parser(const char *json_object, char *key);

enum NUVO_signal_state_e
{
    NUVO_SIGNAL_STATE_CONUNT = 0x00,
    NUVO_SIGNAL_STATE_REQ_CONNECT = 0xF1,
    NUVO_SIGNAL_STATE_RES_CONNECT,
    NUVO_SIGNAL_STATE_REQ_SAVEDATA,
    NUVO_SIGNAL_STATE_RES_SAVEDATA,
    NUVO_SIGNAL_STATE_DONE_SAVEDATA,
    NUVO_SIGNAL_STATE_DOWNLOAD_PREPARE = 0xFD,
    NUVO_SIGNAL_STATE_SCP_UPLOAD = 0xFC,
    NUVO_SIGNAL_STATE_DOWNOAD_DONE
};

#define DEFAULT_NUVO_ADDRESS "192.168.137.1"
#define DEFAULT_NUVO_PORT "50000"

enum NUVO_connection_state{
    GW_SLEEP_CONNECTIONING_NUVO = 0,
	
    GW_TRYING_CONNECTION_NUVO = 10,
    GW_TRYING_CONNECTION_NUVO_REPEAT_1,
    GW_TRYING_CONNECTION_NUVO_REPEAT_2,
    GW_TRYING_CONNECTION_NUVO_REPEAT_3,
    GW_TRYING_CONNECTION_NUVO_REPEAT_4,
    GW_TRYING_CONNECTION_NUVO_REPEAT_5,
    GW_TRYING_CONNECTION_NUVO_REPEAT_MAX,
    
    GW_WATING_REPLY_CONNECTION_FROM_NUVO = 20,

    GW_REQUEST_SAVE_DRIVING_HISTORY_TO_NUVO = 30,
    GW_WATING_REPLY_SAVE_DRIVING_HISTORY_FROM_NUVO,
    GW_WAIT_DONE_SAVE_DRIVING_HISTORY_FROM_ECU, 
    GW_REQUEST_DONE_SAVE_DRIVING_HISTORY_TO_NUVO,

    GW_WAIT_DRIVING_HISTORY_INFO_FROM_NOVO = 40,
    GW_RECEIVE_DRIVING_HISTORY_DATA_FROM_NOVO,
    GW_RECEIVE_DRIVING_HISTORY_DATA_FROM_NOVO_USED_SCP,
};


struct NUVO_recv_task_info_t
{
    int *task_info_state;
    int sock;
    struct sockaddr_in serv_adr;
    socklen_t server_addr_len;
    enum NUVO_connection_state state;
    char ACK[4];
    int life_time;
};

extern void *Th_RelayServer_NUVO_Client_Task(void *d);

enum protobuf_type_e
{
    PROTOBUF_DATA_NUBO = 1,
    PROTOBUF_DATA_V2X,
    PROTOBUF_DATA_LOGECU,
    PROTOBUF_DATA_LOGIDE
};
struct protobuf_data_info_t
{
    bool data_isalloc;
    char *data;
    size_t data_len;
};
struct protobuf_data_list_t
{
    struct protobuf_data_info_t data_nubo;
    struct protobuf_data_info_t data_v2x;
    struct protobuf_data_info_t data_ecuids;
};

int f_i_RelayServer_Protobuf_Input_Data(struct protobuf_data_list_t *data, enum protobuf_type_e type, char *input_data, size_t input_data_len);
f_i_RelayServer_Protobuf_Create_Abs(FileData *protobuf, struct protobuf_data_list_t *protobuf_data);

#define S_HOSTNAME "192.168.1.50" //OBU IP
#define S_PORT    63113 

#define MYHOSTNAME "192.168.1.100" //Autonomous Vehicle Data Receive Socket IP
#define MYPORT    63113 


typedef struct
{
	unsigned short MagicKey;
	unsigned char MsgType;
	unsigned short crc;
	unsigned short PacketLen;

} __attribute__((__packed__)) Msg_Header;
typedef struct 
{
	Msg_Header header;
	unsigned char MsgCount; //1byte
	char TmpID[4];     //4byte -> 4byte
	unsigned short DSecond; //2byte
	int Latitude;
	int Longitude;
	short Elevation; //2byte

	//unsigned int postionalAccuracy; //4byte -> 4byte
	unsigned char SemiMajor;
	unsigned char SemiMinor;
	unsigned short Orientation;

	unsigned short TransmissionState; //2byte
	short heading;
	unsigned char SteeringWheelAngle;

	//unsigned char AccelerationSet4Way[7]; //7byte -> 7byte
	short Accel_long;
	short  Accel_lat;
	unsigned char Accel_vert;
	short YawRate;
	
	unsigned short BrakeSystemStatus;

	//unsigned char VehicleSize[3]; //3byte -> 4byte
	unsigned short Width;
	unsigned short Length;

	unsigned int L2id; //Add 4byte 
} __attribute__((__packed__)) BSM_Core;  

typedef struct
{
	Msg_Header header;
	unsigned int Sender;	
	unsigned int Receiver; //ADD receivder, for broadcast Receivdr = FFFFFFFF, for unciast Receiver = L2ID
	unsigned short ManeuverType;
	unsigned char RemainDistance;
} __attribute__((__packed__)) DMM; // Driving Maneuver Message(3) 

typedef struct
{
	Msg_Header header;
	unsigned int Sender;	
	unsigned int Receiver;
	unsigned char RemainDistance;
} __attribute__((__packed__)) DNM_Req; 

typedef struct
{
	Msg_Header header;
	unsigned int Sender;	
	unsigned int Receiver;
	unsigned char AgreeFlag; //Agreement 1 (default), disagreement (0)
} __attribute__((__packed__)) DNM_Res; 
typedef struct
{
	Msg_Header header;
	unsigned int Sender;	
	unsigned int Receiver;
	unsigned char NegoDone; //Default 0, Negotiation ing 1, Done 2
} __attribute__((__packed__)) DNM_Done; 

extern void *Th_RelayServer_V2X_UDP_Task(void *arg);
void f_v_Timestamp_Get();
char *Timestamp_Print();