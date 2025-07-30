enum payload_type_e
{
    Fireware,
    Program,
};

struct client_data_info_t 
{
    uint8_t ID[8];
    uint8_t Version[8];
    enum payload_type_e Payload_Type;
    size_t  Payload_Size;
};

struct clients_info_t
{
    pthread_mutex_t mtx;
    int connected_client_num;
    int socket[MAX_CLIENT_SIZE];
    uint32_t socket_message_seq[MAX_CLIENT_SIZE];
    enum job_type_e socket_job_state[MAX_CLIENT_SIZE];
    
    struct client_data_info_t client_data_info[MAX_CLIENT_SIZE];
};

struct data_header_info_t
{
    uint8_t Job_State;
    uint8_t Protocol_Type;
    uint16_t Client_fd;
    uint16_t Message_seq;
    uint16_t Message_size;
};

enum job_type_e{
    JobInitial,
    JobFinish,

    FirmwareInfoReport,
    FirmwareInfoRequest,
    FirmwareInfoResponse,
    FirmwareInfoIndication,
    FirmwareInfoResponseIndication,

    ProgramInfoReport,
    ProgramInfoRequest,
    ProgramInfoResponse,
    ProgramInfoIndication,
    ProgramInfoResponseIndication,

    HandOverReminingData
};

extern enum job_type_e F_e_RelayServer_Job_Process_Do(struct data_header_info_t *Now_Hader, uint8_t *Data, int Client_is);

static struct client_data_info_t f_s_RelayServer_Job_Process_JobInitial(struct data_header_info_t *Now_Hader, uint8_t *Data, int *err);
static int f_i_RelayServer_Job_Process_FirmwareInfoReport(struct data_header_info_t *Now_Hader, uint8_t *Data);
static int f_i_RelayServer_Job_Process_FirmwareInfoRequest(struct data_header_info_t *Now_Hader, uint8_t *Data);
static int f_i_RelayServer_Job_Process_FirmwareInfoResponse(struct data_header_info_t *Now_Hader, uint8_t *Data);
static int f_i_RelayServer_Job_Process_FirmwareInfoIndication(struct data_header_info_t *Now_Hader, uint8_t *Data);
static int f_i_RelayServer_Job_Process_JobFinish(struct data_header_info_t *Now_Hader, uint8_t *Data, int Client_is);

/* 
Brief:
Parameter[In]
Parameter[Out]
 */
enum job_type_e f_e_RelayServer_Job_Process_Do(struct data_header_info_t *Now_Hader, uint8_t *Data, int Client_is)
{
    int ret;
    enum job_type_e Now_Job_State = *Now_Hader.Job_State;
    enum job_type_e After_Job_State;
    
    switch(Now_Job_State)
    {
        case JobInitial:
            G_Client_Infos.client_data_info[Client_is] = f_s_RelayServer_Job_Process_JobInitial(Now_Hader, Data, &ret);
        case JobFinish:
            ret = f_i_RelayServer_Job_Process_JobFinish(Now_Hader, Data, Client_is);
        case FirmwareInfoReport:
        case ProgramInfoReport: 
            ret = f_e_RelayServer_Job_Process_FirmwareInfoReport(Now_Hader, Data);
            break;
        case FirmwareInfoRequest:
        case ProgramInfoRequest:
            ret = f_i_RelayServer_Job_Process_FirmwareInfoRequest(Now_Hader, Data);
            break;
        case FirmwareInfoResponse:
        case ProgramInfoResponse:
            ret = f_e_RelayServer_Job_Process_FirmwareInfoResponse(Now_Hader, Data);
            break;
        case FirmwareInfoIndication:
        case ProgramInfoIndication:
            ret = f_i_RelayServer_Job_Process_FirmwareInfoIndication(Now_Hader, Data);
            break;

        case HandOverReminingData:
            //f_s_RelayServer_Job_Process_HandOverReminingData()
            break;
        default:break;
    }
    if(ret > 0)
    {
        After_Job_State = Data[0];
    }
    if(Now_Job_State == After_Job_State)
    {
        
    }else{
        Now_Hader.Job_State = After_Job_State;
        G_Client_Infos.socket_job_state[Client_is] = After_Job_State;
    }

    return After_Job_State;
}

/* 
Brief:
Parameter[In]
    Now_Hader:
    Data:
    err:
Parameter[Out]
    client_data_info_t:It is made by the function 
 */
struct client_data_info_t f_s_RelayServer_Job_Process_JobInitial(struct data_header_info_t *Now_Hader, uint8_t *Data, int *err)
{
    if(Data)
    {
        char *Payload = (Data + HEADER_SIZE); 
        struct client_data_info_t out_data;
        if(Payload[0] == 0x44) // Check STX
        {
            switch((int)Payload[1])
            {
                case 1:
                    if(Now_Hader.Message_size > 18) //Will Make the Over Recv Error Solution
                    {

                    }
                    out_data.Payload_Type = Fireware;
                    Now_Hader->Job_State = 2;
                    Data[0] = "2";
                    break;
                case 3:
                    if(Now_Hader.Message_size > 18) //Will Make the Over Recv Error Solution
                    {

                    }
                    out_data.Payload_Type = Program;
                    Now_Hader->Job_State = 7;
                    Data[0] = "7";
                    break;
                default:
                    F_RelayServer_Print_Debug(0, "[Error][f_s_RelayServer_Job_Process_JobInitial][Payload_type:%d]", (int)Payload[1]);
                    *err = -1;
                    return 0;

            }
            memcpy(&(out_data.ID), Payload, 8);
            memcpy(&(out_data.Version), Payload + 8, 8);
        }
    }
    return out_data;
}

/* 
Brief:
Parameter[In]
    Now_Hader:
    Data:
    Client_is:
Parameter[Out]
    int 0 < Return Error Code
 */
int f_i_RelayServer_Job_Process_JobFinish(struct data_header_info_t *Now_Hader, uint8_t *Data, int Client_is)
{
    if(Data)
    {
        int ret;
        switch(Now_Hader->Job_State)
        {
            case JobFinish:
                memset(G_Clients_Info.client_data_info[Client_is].ID, 0x00, 8);
                memset(G_Clients_Info.client_data_info[Client_is].Version, 0x00, 8);
                G_Clients_Info.socket_message_seq[Client_is] = 0;
                G_Clients_Info.socket_job_state[Client_is] = 0;
                close(G_Clients_Info.socket[Client_is]);
                connected_client_num--;
            default:
                F_RelayServer_Print_Debug(0, "[Error][%s][Job_State:%d]", __func__, Now_Hader->Job_State);
                return -1;
        }
    }else{
        F_RelayServer_Print_Debug(0, "[Error][%s][No Data]\n", __func__);
        return -1;
    }
    return 0;
}

/* 
Brief:
Parameter[In]
    Now_Hader:
    Data:
Parameter[Out]
    int 0 < Return Error Code
 */
int f_i_RelayServer_Job_Process_FirmwareInfoReport(struct data_header_info_t *Now_Hader, uint8_t *Data)
{
    if(Data)
    {
        char *Payload = (Data + HEADER_SIZE); 
        if(Payload[0] == 0x44) // Check STX
        {
            switch(Now_Hader->Job_State)
            {
                case FirmwareInfoReport:
                    if(Now_Hader.Message_size == 18 && Payload[Now_Hader.Message_size] == 0xAA)
                    {
                        Now_Hader->Job_State = 3;
                        Data[0] = "3";
                        return Now_Hader->Job_State;
                    }else{
                        F_RelayServer_Print_Debug(0, "[Error][%s][Now_Hader.Message_size:%d, ETX:%02X]",__func__, Now_Hader.Message_size, Payload[Now_Hader.Message_size]);
                        return -3;
                    }
                    break;
                case ProgramInfoReport:
                    if(Now_Hader.Message_size == 18 && Payload[Now_Hader.Message_size] == 0xAA)
                    {
                        Now_Hader->Job_State = 8;
                        Data[0] = "8";
                        return Now_Hader->Job_State;
                    }else{
                        F_RelayServer_Print_Debug(0, "[Error][%s][Now_Hader.Message_size:%d, ETX:%02X]",__func__, Now_Hader.Message_size, Payload[Now_Hader.Message_size]);
                        return -8;
                    }
                default:
                    return 0;
            }     
        } 
    }else{
        F_RelayServer_Print_Debug(0, "[Error][%s][No Data]\n",__func__,);
        return -1;
    }
    return 0;
}

/* 
Brief:
Parameter[In]
    Now_Hader:
    Data:
    err:
Parameter[Out]
    client_data_info_t:It is made by the function 
 */
char f_i_RelayServer_Job_Process_FirmwareInfoRequest(struct data_header_info_t *Now_Hader, uint8_t *Data)
{
    if(Data)
    {
        switch(Now_Hader->Job_State)
        {
            case FirmwareInfoRequest:
                //Send the Data To PC_Server with HTTP Protocol
                Now_Hader->Job_State = 4;
                free(Data);
                char *out_data = malloc(sizeof(uint8_t) * HEADER_SIZE);
                sprintf(out_data, "%01X%01X%02X%02X%02X", 0x4, 0x0, Now_Hader->Client_fd, Now_Hader->Message_seq, 0x00);
                Data = out_data;
                break;
            case ProgramInfoRequest:
                //Send the Data To PC_Server with HTTP Protocol
                Now_Hader->Job_State = 9;
                free(Data);
                char *out_data = malloc(sizeof(uint8_t) * HEADER_SIZE);
                sprintf(out_data, "%01X%01X%02X%02X%02X", 0x4, 0x0, Now_Hader->Client_fd, Now_Hader->Message_seq, 0x00);
                Data = out_data;
            default:
                F_RelayServer_Print_Debug(0, "[Error][%s][Job_State:%d]", __func__, Now_Hader->Job_State);
                return -1;
        }     
    }else{
        F_RelayServer_Print_Debug(0, "[Error][%s][No Data]\n",__func__,);
        return -1;
    }
    return 0;
}

/* 
Brief:
Parameter[In]
    Now_Hader:
    Data:
    err:
Parameter[Out]
    client_data_info_t:It is made by the function 
 */
int f_i_RelayServer_Job_Process_FirmwareInfoResponse(struct data_header_info_t *Now_Hader, uint8_t *Data)
{
    if(Data)
    {
        switch(Now_Hader->Job_State)
        {
            case FirmwareInfoResponse:
                //Recv the Data From PC_Server with HTTP Protocol
                free(Data);
                char *out_data = malloc(sizeof(uint8_t) * (HEADER_SIZE + recv_data_size));
                Now_Hader->Job_State = 5;
                sprintf(out_data, "%01X%01X%02X%02X%02X", 0x5, 0x0, Now_Hader->Client_fd, Now_Hader->Message_seq, recv_data_size, recv_data);
                Data = out_data;
                break;
            case ProgramInfoResponse:
                //Recv the Data From PC_Server with HTTP Protocol
                free(Data);
                Now_Hader->Job_State = 0xA;
                char *out_data = malloc(sizeof(uint8_t) * (HEADER_SIZE + recv_data_size));
                sprintf(out_data, "%01X%01X%02X%02X%02X%s", 0xA, 0x0, Now_Hader->Client_fd, Now_Hader->Message_seq, recv_data_size, recv_data);
                Data = out_data;
                break;
            default:
                F_RelayServer_Print_Debug(0, "[Error][%s][Job_State:%d]", __func__, Now_Hader->Job_State);
                return -1;
        }
    }else{
        F_RelayServer_Print_Debug(0, "[Error][%s][No Data]\n",__func__,);
        return -1;
    }
    return 0;
}

/* 
Brief:
Parameter[In]
    Now_Hader:
    Data:
    err:
Parameter[Out]
    client_data_info_t:It is made by the function 
 */
int f_i_RelayServer_Job_Process_FirmwareInfoIndication(struct data_header_info_t *Now_Hader, uint8_t *Data)
{
    if(Data)
    {
        char *Payload = Data + HEADER_SIZE;
        int ret;
        switch(Now_Hader->Job_State)
        {
            case FirmwareInfoIndication:
            case ProgramInfoIndication:
                if(Now_Hader->Message_size <= 0)
                {
                    ret = send(ow_Hader->Client_fd, Payload, 20);
                    free(Data);
                    Now_Hader->Job_State = 1;
                    char *out_data = malloc(sizeof(uint8_t) * HEADER_SIZE);
                    sprintf(out_data, "%01X%01X%02X%02X%02X", 0x1, 0x0, Now_Hader->Client_fd, Now_Hader->Message_seq);
                    Data = out_data;
                    break;
                }else{
                    ret = send(ow_Hader->Client_fd, Payload, Now_Hader->Message_size);
                    free(Data);
                    Now_Hader->Job_State = 1;
                    char *out_data = malloc(sizeof(uint8_t) * HEADER_SIZE);
                    sprintf(out_data, "%01X%01X%02X%02X%02X", 0x1, 0x0, Now_Hader->Client_fd, Now_Hader->Message_seq);
                    Data = out_data;
                }
                break;
            default:
                F_RelayServer_Print_Debug(0, "[Error][%s][Job_State:%d]", __func__, Now_Hader->Job_State);
                return -1;
        }
    }else{
        F_RelayServer_Print_Debug(0, "[Error][%s][No Data]\n",__func__,);
        return -1;
    }
    return 0;
}

