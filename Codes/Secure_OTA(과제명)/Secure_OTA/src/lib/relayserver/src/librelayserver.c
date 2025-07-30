  /* LIBRARY Source */
  #include <./librelayserver.h>

  #define  _DEBUG_LOG printf("[DEBUG][%s][%d]\n", __func__, __LINE__);
  #define _DEBUG_PRESS_TIMER(t) time_t time_end = time(NULL);while(getchar() != '\n'){if(time(NULL) - time_end > t){break;}else{sleep(1);}}printf("\n");
  #define _DEBUG_PRESS(x) printf("\n");printf("[%d][Opration_Gateway] " "\033[0;33m" "Press Any Key" "\033[0;0m" " to [%s:%d]\n", __LINE__, __func__, x);printf("\x1B[1A\r"); _DEBUG_PRESS_TIMER(5)
  
  bool *g_curl_isused;
  /* 
  Brief:
  Parameter[In]
  Parameter[Out]
      socket_info_t
   */
  struct socket_info_t F_s_RelayServer_TcpIp_Initial_Server(char *Device_Name, int *Port, int *err)
  {
      int ret = 0;
      struct socket_info_t Socket_Info;
      //Check Argurements
      if(!Port)
      {
          F_RelayServer_Print_Debug(2, "[Error][%s] No Input Argurements.(Port:%p)\n", __func__, Port);
          *err = -1;
          return Socket_Info;
      }else{
          Socket_Info.Socket_Type = SERVER_SOCKET;
          Socket_Info.Port = *Port;
           //Socket_Setup
          Socket_Info.Socket = socket(PF_INET, SOCK_DGRAM, 0);
          memset(&Socket_Info.Socket_Addr, 0x00, sizeof(Socket_Info.Socket_Addr)); 
          Socket_Info.Socket_Addr.sin_family = AF_INET;
          Socket_Info.Socket_Addr.sin_port = htons(Socket_Info.Port);
          
          if(Device_Name)
          {
              //Getting the Ethernet Device IP Address  
              Socket_Info.Device_Name = Device_Name;
              ret = F_i_RelayServer_TcpIp_Get_Address(Socket_Info.Device_Name, Socket_Info.Device_IPv4_Address);
              if(ret < 0)
              {
                  F_RelayServer_Print_Debug(2,"[Error][%s] Return_Value:%d\n", __func__, ret);
                  *err = -1;
                  return Socket_Info;
              }
              Socket_Info.Socket_Addr.sin_addr.s_addr = inet_addr(Socket_Info.Device_IPv4_Address);
          }else 
          {
              Socket_Info.Device_Name = "INADDR_ANY";
              Socket_Info.Socket_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
          }
  
          ret = f_i_RelayServer_TcpIp_Setup_Socket(&Socket_Info.Socket, 250, true);
          if(ret < 0)
          {
              F_RelayServer_Print_Debug(2,"[Error][%s] Return_Value:%d\n", __func__, ret);
              *err = -1;
              return Socket_Info;
          }
  
      }
      g_curl_isused = malloc(sizeof(bool));
      *g_curl_isused = 0;
      return Socket_Info;
  }
  
  /* 
  Brief:Getting the IPV4 address of inputed the device name.
  Parameter[In]
      Device_Name:Device Name
      Output_IPv4Adrress[]:Array that size 40 to store the IPv4 address.
  Parameter[Out]
      int < 0 = Error_Code
   */
  int F_i_RelayServer_TcpIp_Get_Address(char *Device_Name, char Output_IPv4Adrress[40])
  {
      int ret = 0;
  
      //Check Argurement
      if(!Device_Name)
      {
          F_RelayServer_Print_Debug(2, "[Error][%s] No Input Argurements.(Device_Name:%p)\n", __func__, Device_Name);
          return -1;
      }
  
      /* Use the Ethernet Device Name to find IP Address at */
      struct ifreq ifr;
      int IP_Parsing_Socket;
      
      IP_Parsing_Socket = socket(AF_INET, SOCK_DGRAM, 0);
      strncpy(ifr.ifr_name, Device_Name, IFNAMSIZ);
  
      if (ioctl(IP_Parsing_Socket, SIOCGIFADDR, &ifr) < 0) {
          printf("Error");
      } else {
          inet_ntop(AF_INET, ifr.ifr_addr.sa_data+2, Output_IPv4Adrress, sizeof(struct sockaddr));
          F_RelayServer_Print_Debug(1, "[Info][%s] %s IP Address is %s\n", __func__, Device_Name, Output_IPv4Adrress);
      }
      ret = f_i_RelayServer_TcpIp_Setup_Socket(&IP_Parsing_Socket, 100, true);
      if(ret < 0)
      {
          return -1;
      }
      close(IP_Parsing_Socket);
      return  0;
  }
  /* 
  Brief:
  Parameter[In]
  Parameter[Out]
   */
  int F_i_RelayServer_TcpIp_Task_Run(struct socket_info_t *Socket_Info)
  {
      int ret;
      ret = f_i_RelayServer_TcpIp_Bind(&Socket_Info->Socket, Socket_Info->Socket_Addr);
      if(ret < 0)
      {
              F_RelayServer_Print_Debug(2, "[Error][%s][f_i_RelayServer_TcpIp_Bind] Return Value:%d", __func__, ret);
      }else{
          pthread_create(&(Socket_Info->Task_ID), NULL, th_RelayServer_TcpIp_Task_Server, (void*)Socket_Info);  
          pthread_detach((Socket_Info->Task_ID));
          F_RelayServer_Print_Debug(1, "[Sucess][%s][Task_ID:%ld]\n", __func__, Socket_Info->Task_ID);
  
      }
      return 0;
  }
  /* 
  Brief:
  Parameter[In]
  Parameter[Out]
   */
  void *Th_RelayServer_Job_Task(void *Data)
  {
      Data = Data;
      struct Memory_Used_Data_Info_t *Data_Info = (struct Memory_Used_Data_Info_t *)Data;
      int ret;
      // Using_Timer
      int32_t TimerFd = timerfd_create(CLOCK_MONOTONIC, 0);
      struct itimerspec itval;
      struct timespec tv;
      uint32_t Task_Timer_Max = 100 * 1000;
      uint32_t Task_Timer_min = 10 * 1000;
      uint64_t res;
      
      int mTime = Task_Timer_Max / 1000;
      setsockopt(TimerFd, SOL_SOCKET, SO_RCVTIMEO, (char*)&mTime, sizeof( mTime));
  
      clock_gettime(CLOCK_MONOTONIC, &tv); 
      itval.it_interval.tv_sec = 0;
      itval.it_interval.tv_nsec = (Task_Timer_Max % 1000000) * 1e3;
      itval.it_value.tv_sec = tv.tv_sec + 1;
      itval.it_value.tv_nsec = 0;
      ret = timerfd_settime (TimerFd, TFD_TIMER_ABSTIME, &itval, NULL);
  
      uint32_t tick_count_10ms = 0;
      tick_count_10ms = (uint32_t)tick_count_10ms;
      int Task_Timer_now = Task_Timer_Max;
      size_t Before_data_count = (size_t)(*(Data_Info->Data_Count));
      float Timer_Index;
      while(1)
      {   
          ret = read(TimerFd, &res, sizeof(res));
          if(ret < 0)
          {
              
          }else{
              switch((size_t)(*(Data_Info->Data_Count)))
              {
                  case 0:
                      Task_Timer_now = Task_Timer_Max;
                      clock_gettime(CLOCK_MONOTONIC, &tv); 
                      itval.it_interval.tv_nsec = (Task_Timer_now % 1000000) * 1e3;
                      itval.it_value.tv_sec = tv.tv_sec + 1;
                      timerfd_settime (TimerFd, TFD_TIMER_ABSTIME, &itval, NULL);
                      Before_data_count = 0;
                      break;
                  case MEMORY_USED_DATA_LIST_SIZE:
                      Task_Timer_now = Task_Timer_min;
                      clock_gettime(CLOCK_MONOTONIC, &tv); 
                      itval.it_interval.tv_nsec = (Task_Timer_now % 1000000) * 1e3;
                      itval.it_value.tv_sec = tv.tv_sec + 1;
                      timerfd_settime (TimerFd, TFD_TIMER_ABSTIME, &itval, NULL);
                      break;
                  default:
                      if(Before_data_count + 20 < (size_t)(*(Data_Info->Data_Count)))
                      {
                          Before_data_count = (size_t)(*(Data_Info->Data_Count));
                          Timer_Index = ((size_t)*(Data_Info->Data_Count) * 1e4) / (MEMORY_USED_DATA_LIST_SIZE * 1e4);
                          Task_Timer_now = (Task_Timer_Max * 1e4) * (1e4 - Timer_Index);
                          Task_Timer_now = Task_Timer_now / 1e4;
                          if(Task_Timer_now < Task_Timer_min)
                          {
                          Task_Timer_now = Task_Timer_min;
                          }                    
                          clock_gettime(CLOCK_MONOTONIC, &tv); 
                          itval.it_interval.tv_nsec = (Task_Timer_now % 1000000) * 1e3;
                          itval.it_value.tv_sec = tv.tv_sec + 1;
                          timerfd_settime (TimerFd, TFD_TIMER_ABSTIME, &itval, NULL);
                      }
                      break;
                  break;
              }
          
              size_t data_size = 0;
              for(int data_is = 0; data_is < (size_t)*(Data_Info->Data_Count); data_is++)
              {
                  if(F_Memory_Data_isEmpty(Data_Info))
                  {
                  }else{
                      uint8_t *out_data = (uint8_t*)F_v_Memory_Data_Pop(Data_Info, &data_size); 
                      //F_RelayServer_Print_Debug(6,"[Debug][%s][%d][Pop_Data:%s/%d][%d]\n", __func__, __LINE__, out_data, data_size, (size_t)*(Data_Info->Data_Count));
                      if(out_data)
                      {
                          struct data_header_info_t Data_Header_Info = f_s_Parser_Data_Header((char*)out_data, HEADER_SIZE);
                          F_RelayServer_Print_Debug(6,"[Debug][%s][%d][Client:%u]\n", __func__, __LINE__, Data_Header_Info.Client_fd);
                          enum job_type_e Now_Job;
                          if(*G_Clients_Info.connected_client_num > 0)
                          { 
                              for(int client_is = 0; client_is < MAX_CLIENT_SIZE; client_is++)
                              {
                                  if(G_Clients_Info.socket[client_is] == Data_Header_Info.Client_fd)
                                  {
                                      F_RelayServer_Print_Debug(2,"[Info][%s] Now_Job:%d\n", __func__, Data_Header_Info.Job_State);
                                      Now_Job = f_e_RelayServer_Job_Process_Do(&Data_Header_Info, &out_data, client_is, Data_Info);
                                      F_RelayServer_Print_Debug(6,"[Info][%s] Now_Job:%d\n", __func__, Now_Job);
                                      switch(Now_Job)
                                      {
                                          case Initial:
                                          case FirmwareInfoReport:
                                          case FirmwareInfoResponse:
                                          case FirmwareInfoIndication:
  
                                          case ProgramInfoReport: 
                                          case ProgramInfoResponse:
                                          case ProgramInfoIndication:
  
                                          case HandOverReminingData:
                                              F_RelayServer_Print_Debug(2,"[Info][%s] Now_Job:%d\n", __func__, Now_Job);
                                              data_size = Data_Header_Info.Message_size + HEADER_SIZE;
                                              F_RelayServer_Print_Debug(2,"[Debug][%s][%d][Push:%s/%d]\n", __func__, __LINE__, out_data, data_size);
                                              F_i_Memory_Data_Push(Data_Info, out_data, data_size);
                                              break;
  
                                          case FirmwareInfoRequest:
                                          case ProgramInfoRequest:
                                          case Finish:
                                              F_RelayServer_Print_Debug(0,"[Info][%s] Now_Job:%d\n", __func__, Now_Job);
                                              break;
                                          default:
                                              F_RelayServer_Print_Debug(0,"[Info][%s] Now_Job:%d\n", __func__, Now_Job);
                                              break; 
                                      }  
                                      break;
                                  }else{
                                          if(0)//(client_is == *G_Clients_Info.connected_client_num - 1)
                                          {
                                              F_RelayServer_Print_Debug(1, "[Debug][%s][Client Closed:%d]\n", __func__, G_Clients_Info.socket[client_is]);
                                              F_RelayServer_Print_Debug(1, "[Debug][%s][Client Closed:%d]\n", __func__, Data_Header_Info.Client_fd);
                                          }
                                  }
                              }
                          }
                          
                          F_RelayServer_Print_Debug(4,"[Debug][%s][Free:%d, Address:%p]\n", __func__, __LINE__, out_data);
                          Relay_safefree(out_data);
                      }
                  
                  }
              }
          }
  
  
      }
  }
  
  int f_i_Hex2Dec(char data)
  {
      int ret;
      if(48 <= (int)(data)  && (int)(data)  <= 57){
          ret = (int)(data) - 48;
      }else if(65 <= (int)(data)  && (int)(data)  <= 70)
      {
          ret = (int)(data) - 65 + 10;
      }else if(97 <= (int)(data)  && (int)(data)  <= 102)
      {
          ret = (int)(data)- 97 + 10;
      }
      return ret;
  }
  
  struct data_header_info_t f_s_Parser_Data_Header(char *Data, size_t Data_Size)
  {
      struct data_header_info_t out_data;
      int Data_Num = 0;
      for(int i = 0; i < 4; i++)
      {
          switch(Data_Num)
          {
              case 0:
                  out_data.Job_State = f_i_Hex2Dec(Data[0]);
                  out_data.Protocol_Type = f_i_Hex2Dec(Data[1]);
                  break;
              case 1:
                  out_data.Client_fd = 0;
                  for(int i = 0; i < 8; i++)
                  {
                      out_data.Client_fd = out_data.Client_fd * 16 + f_i_Hex2Dec(Data[2 + i]);
                  }
                  break;
              case 2:
                  out_data.Message_seq = f_i_Hex2Dec(Data[10]) * 16 + f_i_Hex2Dec(Data[11]);
                  break;
              case 3:
                  out_data.Message_size = 0;
                  for(int i = 0; i < 8; i++)
                  {
                      out_data.Message_size = out_data.Message_size * 16 + f_i_Hex2Dec(Data[12 + i]);
                  }
                  break;
              default:
                  break;
          }
          Data_Num++;
      }
      return out_data;
  }
  
  void* th_RelayServer_TcpIp_Task_Server(void *socket_info)
  {
      int ret;
      struct socket_info_t *Socket_Info = (struct socket_info_t*)socket_info;
      //int Client_Socket;
      struct sockaddr_in  Client_Address;
      //socklen_t adr_sz = sizeof(Client_Address);
  #if 0 
      ret = listen(Socket_Info->Socket, 5);
      if(ret == -1)
      {
          F_RelayServer_Print_Debug(2,"[Error][%s][listen] Return Value:%d\n", __func__, ret);
          return NULL;
      }
  #endif
      pthread_mutex_init(&G_Clients_Info.mtx, NULL);
     
      int client_is;
      char *recv_buf = malloc(TCP_RECV_BUFFER_SIZE);
      while(1)
      {
          struct sockaddr_in from_adr;
          socklen_t from_adr_sz;
          memset(recv_buf, 0x00, 128);
          int str_len = recvfrom(Socket_Info->Socket, recv_buf, 128, 0, (struct sockaddr*)&from_adr, &from_adr_sz);
          if(str_len > 0)
          {
              if(*G_Clients_Info.connected_client_num == MAX_CLIENT_SIZE)
              {
                  F_RelayServer_Print_Debug(2,"[Error][%s][%d] Connected Client Num > MAX_CLIENT_SIZE:%d/%d\n", __func__, __LINE__, *G_Clients_Info.connected_client_num, MAX_CLIENT_SIZE);
              }else{
                  for(client_is = 0; client_is < MAX_CLIENT_SIZE; client_is++)
                  {
                      if(G_Clients_Info.socket[client_is] == 0)
                      {
                          pthread_mutex_lock(&G_Clients_Info.mtx);
                          G_Clients_Info.socket[client_is] = (int)(from_adr.sin_addr.s_addr);
                          G_Clients_Info.Life_Timer[client_is] = G_TickTimer.G_100ms_Tick + SOCKET_TIMER;
                          G_Clients_Info.socket_message_seq[client_is] = 0;
                          *G_Clients_Info.connected_client_num = *G_Clients_Info.connected_client_num + 1;
                          pthread_mutex_unlock(&G_Clients_Info.mtx);
                          break;
                      }else if(G_Clients_Info.socket[client_is] == (int)(from_adr.sin_addr.s_addr))
                      {
                          pthread_mutex_lock(&G_Clients_Info.mtx);
                          G_Clients_Info.Life_Timer[client_is] = G_TickTimer.G_100ms_Tick + SOCKET_TIMER;
                          char addr_str[40];
                          inet_ntop(AF_INET, (void *)&from_adr.sin_addr, addr_str, sizeof(addr_str));
                          //F_RelayServer_Print_Debug(2,"[DEBUG][%s][%d] Connected Client %s:%d\n", __func__, __LINE__, addr_str, from_adr.sin_port);
                          G_Clients_Info.socket_message_seq[client_is]++;
                           
                          uint8_t *push_data = malloc(sizeof(uint8_t) * (str_len + HEADER_SIZE));
                          sprintf((char*)push_data, HEADER_PAD,  //Client Data Protocol(Header:Hex_Sring,Payload:OCTETs)
                          0x0, //:job_state(1)
                          0x1, //protocol_type(1)
                          G_Clients_Info.socket[client_is], //client_fd(8)
                          G_Clients_Info.socket_message_seq[client_is], //message_seq(2);
                          str_len - 1);//message_size(8);
                          memcpy(push_data + HEADER_SIZE, recv_buf, str_len);//data(payload_size)
                          //F_RelayServer_Print_Debug(2,"[Debug][%s][%d][Push_Data:%s/%d]\n", __func__, __LINE__, push_data, str_len + HEADER_SIZE);
                          size_t left_buf = F_i_Memory_Data_Push(&G_Data_Info, (void *)push_data, str_len + HEADER_SIZE);
                          pthread_mutex_unlock(&G_Clients_Info.mtx);
                          //F_RelayServer_Print_Debug(2,"[Debug][%s][%d][Free Address:%p]\n", __func__, __LINE__, push_data);
                          Relay_safefree(push_data);
                          if(left_buf >= 0)
                          {
                              F_RelayServer_Print_Debug(2,"[Info][%s] Left_Buffer_Size:%ld\n", __func__, left_buf);
                          }else{
                              F_RelayServer_Print_Debug(2,"[Error][%s] No left buffer:%ld\n", __func__, left_buf);
                          }
                          break;
                      }
                  }
              }
          }  
          
  #if 1
          if(1)//(init_time + 1 < G_TickTimer.G_100ms_Tick)
          {
              //init_time = G_TickTimer.G_100ms_Tick;
              for(int i = 0; i < MAX_CLIENT_SIZE; i++)
              {   
                  if(G_Clients_Info.socket[i] == Socket_Info->Socket)
                  {
                     
                  }else if(G_Clients_Info.socket[i]  != 0)
                  {   
                      if(G_Clients_Info.Life_Timer[i] <= G_TickTimer.G_100ms_Tick)
                      {
                          F_RelayServer_Print_Debug(0,"[Debug][%s][close:%d, Timer:%d/%d, socket:%d]\n", __func__, __LINE__, G_Clients_Info.Life_Timer[i] ,G_TickTimer.G_100ms_Tick ,G_Clients_Info.socket[i]);
                          pthread_mutex_lock(&G_Clients_Info.mtx);
                          G_Clients_Info.socket[i] = 0;
                          memset(G_Clients_Info.client_data_info[i].ID, 0x00, 8);
                          memset(G_Clients_Info.client_data_info[i].Division, 0x00, 1);
                          memset(G_Clients_Info.client_data_info[i].Version, 0x00, 8);
                          G_Clients_Info.Life_Timer[i] = 0;
                          G_Clients_Info.socket_message_seq[i] = 0;
                          G_Clients_Info.socket_job_state[i] = -1; 
                          if(*G_Clients_Info.connected_client_num > 0)
                          {
                              *G_Clients_Info.connected_client_num = *G_Clients_Info.connected_client_num - 1;
                          }else if(*G_Clients_Info.connected_client_num < 0){
                              *G_Clients_Info.connected_client_num = 0;
                          }
                          pthread_mutex_unlock(&G_Clients_Info.mtx);
                          close(G_Clients_Info.socket[i]);  
                      }
                  }
              }
          }
  #endif
      }
      free(recv_buf);
      printf("While_Loop_Broken!%d\n", __LINE__);
      return (void*)NULL;
  }
  /* 
  Brief:The Socket binding.
  Parameter[In]
      Socket:Server Socket
      Socket_Addr:Server Socket Address
  Parameter[Out]
      int 0 < Error_Code
   */
  int f_i_RelayServer_TcpIp_Bind(int *Server_Socket, struct sockaddr_in Socket_Addr)
  {
      int ret, Retry_Count;
      int Retry_Max = 10;
      char addr_str[40];
      inet_ntop(AF_INET, (void *)&Socket_Addr.sin_addr, addr_str, sizeof(addr_str));
      do
      {
          ret = bind(*Server_Socket, (struct sockaddr*)&(Socket_Addr), sizeof(Socket_Addr));
          if(ret < 0 ) 
          {
              F_RelayServer_Print_Debug(0, "[Error][%s][Return_Value:%d]", __func__, ret);
              F_RelayServer_Print_Debug(0, "[Error][%s]\
              Server_Socket:%d;\
              Ip:Port:%s:%d\n",\
               __func__, *Server_Socket, addr_str, Socket_Addr.sin_port);
              if(Retry_Count == Retry_Max)
              {
                  close(*Server_Socket);
                  return -1;
              }
              Retry_Count++;
  
              sleep(1);
          }else{
  
              F_RelayServer_Print_Debug(0, "[Sucess][%s]\
              Server_Socket:%d;\
              Ip:Port:%s:%d\n",\
               __func__, *Server_Socket, addr_str, Socket_Addr.sin_port);
              return 0;
          }
      }while(Retry_Count < 10);
      return 0;
  }
  
  /* 
  Brief:Setting features of the Socket. The Timer set a socket block timer. The Linger set a socket remaining time after closing socket.
  Parameter[In]
      Socket:socket
      Timer:socket block time
      Linger:socket remaining time
  Parameter[Out]
      int 0 < Error_Code
   */
  int f_i_RelayServer_TcpIp_Setup_Socket(int *Socket, int Timer, bool Linger)
  {
      if(!Socket || Timer < 0)
      {
          F_RelayServer_Print_Debug(2, "[Error][%s][No Input Argurements.](Socket:%p, Timer:%d)\n", __func__, Socket, Timer);
          return -1;
      }
      if(Linger)
      {
          struct linger solinger = { 1, 0 };  /* Socket FD close when the app down. */
          if (setsockopt(*Socket, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger)) == SO_ERROR) {
              perror("setsockopt(SO_LINGER)");
              return -3;
          }
      }
      if(Timer > 0)
      {
          struct timeval tv;                  /* Socket Connection End Timer */           
          tv.tv_sec = (int)(Timer / 1000);
          tv.tv_usec = (Timer % 1000) * 1000; 
          if (setsockopt(*Socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) == SO_ERROR) {
              perror("setsockopt(SO_RCVTIMEO)");
              return -2;
          }
          if (setsockopt(*Socket, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) == SO_ERROR) {
              perror("setsockopt(SO_SNDTIMEO)");
              return -1;
          }
      }
      return 0;
  }
  
  /* 
  Brief:According to the input debug level to print out a message in compare with the Global Debug Level.
  Parameter[In]
      Debug_Lever_e:Debug Level
      String:A message to be print
      format:The Message Format
  Parameter[Out]
      NULL
   */
  void F_RelayServer_Print_Debug(enum debug_lever_e Debug_Level, const char *format, ...)
  {
  
    if(Debug_Level == 0)
    {
      va_list arg;
      struct timespec ts;
      struct tm tm_now;
  
      clock_gettime(CLOCK_REALTIME, &ts);
      localtime_r((time_t *)&ts.tv_sec, &tm_now);
      fprintf(stderr, "[%04u%02u%02u.%02u%02u%02u.%06ld]", tm_now.tm_year+1900, tm_now.tm_mon+1, tm_now.tm_mday, \
              tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec, ts.tv_nsec / 1000);
      va_start(arg, format);
      vprintf(format, arg);
      va_end(arg);
    }else{
      return;
    }
  }
  
  /* 
  Brief:
  Parameter[In]
  Parameter[Out]
   */
  void* Th_i_RelayServer_TickTimer(void *Data)
  {
      Data = Data;
      int ret;
      // Using_Timer
      int32_t TimerFd = timerfd_create(CLOCK_MONOTONIC, 0);
      struct itimerspec itval;
      struct timespec tv;
      uint32_t usec = 10 * 1000;
      uint64_t res;
  
      clock_gettime(CLOCK_MONOTONIC, &tv); 
      itval.it_interval.tv_sec = 0;
      itval.it_interval.tv_nsec = (usec % 1000000) * 1e3;
      itval.it_value.tv_sec = tv.tv_sec + 1;
      itval.it_value.tv_nsec = 0;
      ret = timerfd_settime (TimerFd, TFD_TIMER_ABSTIME, &itval, NULL);
  
      uint32_t tick_count_10ms = 0;
      tick_count_10ms = (uint32_t)tick_count_10ms;
  
      while(1)
      {   
          ret = read(TimerFd, &res, sizeof(res));
          if(ret < 0)
          {
  
          }else{
              G_TickTimer.G_10ms_Tick = tick_count_10ms;
              switch(tick_count_10ms % 10)
              {
                  case 0:
                  {
                      G_TickTimer.G_100ms_Tick++; 
                      break;
                  }
                  default: break;
              }
              switch(tick_count_10ms % 100)
              {
                  case 0:
                  {
                      G_TickTimer.G_1000ms_Tick++;
                      break;
                  }
                  default:break;
              }
              tick_count_10ms++;
          }
  
      }
  }
  
  
  /* 
  Brief:
  Parameter[In]
  Parameter[Out]
   */
  enum job_type_e f_e_RelayServer_Job_Process_Do(struct data_header_info_t *Now_Header, uint8_t **Data, int Client_is, struct Memory_Used_Data_Info_t *Data_Info)
  {
      int ret;
      enum job_type_e Now_Job_State = Now_Header->Job_State;
      enum job_type_e After_Job_State;
  
      switch(Now_Job_State)
      {
          case Initial: // Now_Job_State:0
              G_Clients_Info.client_data_info[Client_is] = f_s_RelayServer_Job_Process_Initial(Now_Header, *Data, &ret);
              break;
          case FirmwareInfoReport:// Now_Job_State:2
          case ProgramInfoReport: // Now_Job_State:7
              ret = f_i_RelayServer_Job_Process_InfoReport(Now_Header, *Data);
              if(ret < 0)
              {
                  break;
              }else{
                  Now_Job_State = ret;
              }
          case FirmwareInfoRequest: // Now_Job_State:3
          case ProgramInfoRequest:  // Now_Job_State:8
              ret = f_i_RelayServer_Job_Process_InfoRequest(Now_Header, Data, Data_Info);
              break;
          case FirmwareInfoResponse:// Now_Job_State:4
          case ProgramInfoResponse: // Now_Job_State:9
              ret = f_i_RelayServer_Job_Process_InfoResponse(Now_Header, Data);
              break;
          case FirmwareInfoIndication:// Now_Job_State:5
          case ProgramInfoIndication:// Now_Job_State:11
              break;
          case Finish: // Now_Job_State:1
              ret = f_i_RelayServer_Job_Process_Finish(Now_Header, *Data, Client_is);
              break;
          case HandOverReminingData:
              //f_s_RelayServer_Job_Process_HandOverReminingData()
              break;
          default:break;
      }
      
      if(ret > 0)
      {
          After_Job_State = ret;
      }else{
          After_Job_State = 1;
      }
      if(Now_Job_State == After_Job_State)
      {
       
      }else{
          Now_Header->Job_State = After_Job_State;
          G_Clients_Info.socket_job_state[Client_is] = After_Job_State;
      }
      if(After_Job_State == 1)
      {
          for(int data_is = 0; data_is < (size_t)*(Data_Info->Data_Count); data_is++)
          {
              size_t data_size;
              
              uint8_t *out_data = (uint8_t*)F_v_Memory_Data_Pop(Data_Info, &data_size); 
              if(out_data)
              {
                  struct data_header_info_t Data_Header_Info = f_s_Parser_Data_Header((char*)out_data, HEADER_SIZE);
                  size_t clear_data_size = 0;
                  struct sockaddr_in client_addr = {.sin_addr.s_addr = Data_Header_Info.Client_fd};
                  F_RelayServer_Print_Debug(0,"[Info] Flushing Received Data by Client[%s].\n", inet_ntoa(client_addr.sin_addr));
                  uint8_t *clear_data = (uint8_t*)F_v_Memory_Data_Pop(Data_Info, &clear_data_size); 
                  if(clear_data)
                  {
                      struct data_header_info_t Data_Header_Info_clear = f_s_Parser_Data_Header((char*)clear_data, HEADER_SIZE);
                      if(Data_Header_Info_clear.Client_fd != Data_Header_Info.Client_fd)
                      {
                          F_i_Memory_Data_Push(Data_Info, clear_data, &clear_data_size);
                      }
                  }
              }
          }
      }
      return After_Job_State;
  }
  
  /* 
  Brief:
  Parameter[In]
      Now_Header:
      Data:
      err:
  Parameter[Out]
      client_data_info_t:It is made by the function 
   */
  struct client_data_info_t f_s_RelayServer_Job_Process_Initial(struct data_header_info_t *Now_Header, uint8_t *Data, int *err)
  {
      struct client_data_info_t out_data;
      if(Data)
      {
          uint8_t *Payload = (Data + HEADER_SIZE); 
          if(Payload[0] == 0x44) // Check STX
          {
              switch((int)Payload[1])
              {
                  case 1:
                      if(Now_Header->Message_size  > 23) //Will Make the Over Recv Error Solution
                      {
  
                      }
                      out_data.Payload_Type = Fireware;
                      Now_Header->Job_State = 2;
                      Data[0] = *("2");
                      *err = Now_Header->Job_State;
                      break;
                  case 3:
                      if(Now_Header->Message_size > 23) //Will Make the Over Recv Error Solution
                      {
                          F_RelayServer_Print_Debug(6, "[Error][%s][Payload_type:%c]\n", __func__, Payload[1]);
                      }
                      out_data.Payload_Type = Program;
                      Now_Header->Job_State = 7;
                      Data[0] = *("7");
                      *err = Now_Header->Job_State;
                      break;
                  default:
                      F_RelayServer_Print_Debug(6, "[Error][%s][Payload_type:%c]\n", __func__, Payload[1]);
                      *err = -1;
                      return out_data;
  
              }
              memcpy((out_data.ID), Payload + 2, 8);
              memset((out_data.Division), 0x0A, 1);
              memcpy((out_data.Version), Payload + 10, 8);
          }else{
              Now_Header->Job_State = 1;
              *err = Now_Header->Job_State;
          }
          
      }
      return out_data;
  }
  
  /* 
  Brief:
  Parameter[In]
      Now_Header:
      Data:
      Client_is:
  Parameter[Out]
      int 0 < Return Error Code
   */
   int f_i_RelayServer_Job_Process_Finish(struct data_header_info_t *Now_Header, uint8_t *Data, int Client_is)
  {
      if(Data)
      {
          switch(Now_Header->Job_State)
          {
              case Finish:
                  pthread_mutex_lock(&G_Clients_Info.mtx);
                  G_Clients_Info.socket_job_state[Client_is] = -1;
                  pthread_mutex_unlock(&G_Clients_Info.mtx);
                  break;
              default:
                  break;
          }
      }else{
          F_RelayServer_Print_Debug(2, "[Error][%s][No Data]\n", __func__);
          return -1;
      }
      return 0;
  }
  
  /* 
  Brief:
  Parameter[In]
      Now_Header:
      Data:
  Parameter[Out]
      int 0 < Return Error Code
   */
   int f_i_RelayServer_Job_Process_InfoReport(struct data_header_info_t *Now_Header, uint8_t *Data)
  {
      
      if(Data)
      {
          uint8_t *Payload = (Data + HEADER_SIZE); 
          printf("[%d]Now_Header->Message_size:%d\n", __LINE__, Now_Header->Message_size);
          
          if(Payload[0] == 0x44) // Check STX
          {
              
              switch(Now_Header->Job_State)
              {
                  case FirmwareInfoReport:
                      if(Now_Header->Message_size == 23 && Payload[Now_Header->Message_size - 1] == 0xAA)
                      {
                           
                          Now_Header->Job_State = 3;
                          Data[0] = *"3";
                          F_RelayServer_Print_Debug(0, "[Info][%s][Job_State:%d, STX:%02X ETX:%02X]\n",__func__, Now_Header->Job_State, Payload[0], Payload[Now_Header->Message_size - 1]);
                          return Now_Header->Job_State;
                      }else{
                          printf("Now_Header->Message_size:%d,%02X\n", Now_Header->Message_size, Payload[Now_Header->Message_size - 1] );
                          F_RelayServer_Print_Debug(2, "[Error][%s][Now_Header->Message_size:%d, ETX:%02X]\n",__func__, Now_Header->Message_size, Payload[Now_Header->Message_size - 1]);
                          return -3;
                      }
                      break;
                  case ProgramInfoReport:
                      if(Now_Header->Message_size == 23 && Payload[Now_Header->Message_size] == 0xAA)
                      {
                          Now_Header->Job_State = 8;
                          Data[0] = *"8";
                          F_RelayServer_Print_Debug(2, "[Info][%s][Job_State:%d, STX:%02X ETX:%02X]\n",__func__, Now_Header->Job_State, Payload[0], Payload[Now_Header->Message_size - 1]);
                          return Now_Header->Job_State;
                      }else{
                          F_RelayServer_Print_Debug(2, "[Error][%s][Now_Header->Message_size:%d, ETX:%02X]\n",__func__, Now_Header->Message_size, Payload[Now_Header->Message_size - 1]);
                          return -8;
                      }
                  default:
                      return 0;
              }     
          } 
      }else{
          F_RelayServer_Print_Debug(2, "[Error][%s][No Data]\n",__func__);
          return -1;
      }
      
      return 0;
  }
  
  /* 
  Brief:
  Parameter[In]
      Now_Header:
      Data:
      err:
  Parameter[Out]
      client_data_info_t:It is made by the function 
   */
   int f_i_RelayServer_Job_Process_InfoRequest(struct data_header_info_t *Now_Header, uint8_t **Data, struct Memory_Used_Data_Info_t *Data_Info)
  {
      
      if(Data)
      {
          uint8_t *Payload = (*Data + HEADER_SIZE + 7); 
          struct http_socket_info_t *http_socket_info = malloc(sizeof(struct http_socket_info_t));
          F_RelayServer_Print_Debug(0,"[Debug][%s][malloc:%d, Address:%p]\n", __func__, __LINE__, http_socket_info);
          switch(Now_Header->Job_State)
          {
              case FirmwareInfoRequest:
              {
                  char *TEST_DATA_ECU = "ECU_0001_300";
                  http_socket_info->request_len = f_i_RelayServer_HTTP_Payload(G_HTTP_Request_Info_Fireware, TEST_DATA_ECU, 12, &http_socket_info->request);
                  break;
              }
              case ProgramInfoRequest:
              {
                  char *TEST_DATA_IDS = "IDS_0001_300";
                  http_socket_info->request_len = f_i_RelayServer_HTTP_Payload(G_HTTP_Request_Info_Program, TEST_DATA_IDS, 12, &http_socket_info->request);
                  break;
              }
              default:
                  F_RelayServer_Print_Debug(2, "[Error][%s][Job_State:%d]\n", __func__, Now_Header->Job_State);
                  return -1;
          }   
              http_socket_info->Now_Header = Now_Header;
              http_socket_info->Data_Info = Data_Info;
              Now_Header->Job_State = f_i_RelayServer_HTTP_Task_Run(Now_Header, http_socket_info, Data);
              F_RelayServer_Print_Debug(0, "[Info][%s][Job_State:%d]\n",__func__, Now_Header->Job_State);
              Relay_safefree(http_socket_info);
      }else{
          F_RelayServer_Print_Debug(2, "[Error][%s][No Data]\n",__func__);
          return -1;
      }
  
      return Now_Header->Job_State;
  }
  
  
  /* 
  Brief:
  Parameter[In]
      Now_Header:
      Data:
      err:
  Parameter[Out]
      client_data_info_t:It is made by the function 
   */
   int f_i_RelayServer_Job_Process_InfoResponse(struct data_header_info_t *Now_Header, uint8_t **Data)
  { 
      if(Data)
      {
          char *Payload = *Data + HEADER_SIZE;
  //* ADD 230906
  
          if(Now_Header->Message_size > 0)
          {
              printf("Now_Header->Message_size:%d\n", Now_Header->Message_size);
              char *URL;
              if(Now_Header->Message_size < 100)
              {
                  URL = Payload;
              }else{
                  Relay_safefree(*Data);
                  URL = malloc(strlen("https://itp-self.wtest.biz/v1/system/firmwareDownload.php?fileSeq=350"));
                  sprintf(URL, "%s", "https://itp-self.wtest.biz/v1/system/firmwareDownload.php?fileSeq=350");
              }
              printf("URL:%s\n", URL);
              CURL *curl_handle;
              CURLcode res;
              
              struct MemoryStruct chunk;
              chunk.memory = malloc(1);
              chunk.size = 0;
          
              while(*g_curl_isused)
              {
                  usleep(1 * 1000);
                  printf("g_curl_isused:%d\n", *g_curl_isused);printf("\x1B[1A\r");
              }
              printf("\n");
              *g_curl_isused = 1;
              curl_handle = curl_easy_init();
              if(curl_handle)
              {
                  curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT_MS , 1000);
                  curl_easy_setopt(curl_handle, CURLOPT_URL, URL);
                  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
                  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
                  
                  res = curl_easy_perform(curl_handle);
                  curl_easy_cleanup(curl_handle);
                  
                  if(res != CURLE_OK) {
                      F_RelayServer_Print_Debug(0, "[Error][%s]: %s\n", __func__, curl_easy_strerror(res));
                      Relay_safefree(chunk.memory);
                      Now_Header->Job_State = -1;
                      return -1;
                  }
              }
              *g_curl_isused = 0;
              char *Http_Recv_data = malloc(sizeof(uint8_t) * (chunk.size + HEADER_SIZE));
              memset(Http_Recv_data, 0x00, sizeof(uint8_t) * (chunk.size + HEADER_SIZE));
              switch(Now_Header->Job_State)
              {
                  case FirmwareInfoResponse:
                      Now_Header->Job_State = 0x5;
                      sprintf(Http_Recv_data, HEADER_PAD, 0x5, 0x0, Now_Header->Client_fd, Now_Header->Message_seq, chunk.size);
                      Now_Header->Message_size = chunk.size;
                      break;
                  case ProgramInfoResponse:
                      Now_Header->Job_State = 0xA;
                      sprintf(Http_Recv_data, HEADER_PAD, 0xA, 0x0, Now_Header->Client_fd, Now_Header->Message_seq, chunk.size);
                      Now_Header->Message_size = chunk.size;
                      break;
                  default:
                      Now_Header->Job_State = -1;
                      Relay_safefree(chunk.memory);
                      return -1;
              }
              memcpy(Http_Recv_data + HEADER_SIZE, chunk.memory, chunk.size);
              Relay_safefree(*Data);
              chunk.size = 0;
              Relay_safefree(chunk.memory);
              *Data = (uint8_t*)Http_Recv_data;
              int ret = f_i_RelayServer_Job_Process_InfoIndication(Now_Header, Data);
              if(ret > 0)
              {
                  Now_Header->Job_State = ret;
              }
              printf("f_i_RelayServer_Job_Process_InfoIndication:%d\n", Now_Header->Job_State );
          }
          
      }else{
          Now_Header->Job_State = -1;
          return -1;
      }
      return Now_Header->Job_State;
  }
  
  
  static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
  {
      size_t realsize = size * nmemb;
      struct MemoryStruct *mem = (struct MemoryStruct *)userp;
      printf("mem->size:%d\n", mem->size);
      char *ptr = realloc(mem->memory, mem->size + realsize + 1);
      if(!ptr) {
      /* out of memory! */
      printf("not enough memory (realloc returned NULL)\n");
      return 0;
      }
  
      mem->memory = ptr;
      memcpy(&(mem->memory[mem->size]), contents, realsize);
      mem->size += realsize;
      mem->memory[mem->size] = 0;
  
      return realsize;
  }
  
  #if 0
   size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void **userp)
  {
      size_t realsize = size * nmemb;
      struct MemoryStruct *mem = (struct MemoryStruct*)*userp;
  
      if(mem->size > 0)
      {   
          size_t temp_size = mem->size;
          free(*userp);
          mem = malloc(sizeof(struct MemoryStruct));
          mem->memory = malloc(temp_size + realsize + 1);
          mem->size = temp_size;
          *userp = (void*)mem;
      }else{
          free(*userp);
          mem = malloc(sizeof(struct MemoryStruct));
          mem->memory = malloc(realsize + 1);    
          mem->size = 0;
          *userp = (void*)mem;
      }
      if(!mem) {
          /* out of memory! */
          printf("not enough memory (realloc returned NULL)\n");
          return 0;
      }
      memcpy(&(mem->memory[mem->size]), contents, realsize);
      mem->size += realsize;
      mem->memory[mem->size] = 0;
      return realsize;
  }
  #endif
  /* 
  Brief:
  Parameter[In]
      Now_Header:
      Data:
      err:
  Parameter[Out]
      client_data_info_t:It is made by the function 
   */
   int f_i_RelayServer_Job_Process_InfoIndication(struct data_header_info_t *Now_Header, uint8_t **Data)
  {
      {
          _DEBUG_PRESS(Now_Header->Job_State)
  
          if(Data)
          {       
              uint8_t *Payload = *Data + HEADER_SIZE;
  
              int ret;
              //Socket_Setup
              int sock;
              struct sockaddr_in sock_addr; 
              int dest_port = 50000;
              sock = socket(PF_INET, SOCK_DGRAM, 0);
              memset(&sock_addr, 0x00, sizeof(sock_addr));
              sock_addr.sin_family = AF_INET;
              sock_addr.sin_port = htons(dest_port);
              sock_addr.sin_addr.s_addr = Now_Header->Client_fd;
              ret = f_i_RelayServer_TcpIp_Setup_Socket(&sock, 1000, true);
              switch(Now_Header->Job_State)
              {
                  case FirmwareInfoIndication:
                  case ProgramInfoIndication:
                      if(Now_Header->Message_size <= 0)
                      {
                          Now_Header->Job_State = 1;
                          //ret = send(Now_Header->Client_fd, Payload, 20, MSG_DONTWAIT);
                          ret = sendto(sock, Payload, 20, 0, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
                          if(ret <= 0)
                          {
                              F_RelayServer_Print_Debug(0,"[Debug][%s][send:%d, ret:%p]\n", __func__, __LINE__, ret);
                          }else{
                              printf("send:%d\n", ret);
                          }
                          break;
                      }else{
                          //ret = send(Now_Header->Client_fd, Payload, Now_Header->Message_size, MSG_DONTWAIT);
                          struct Keti_UDP_Header_t send_info;
                          memset(&send_info, 0x00, sizeof(struct Keti_UDP_Header_t));
                          uint32_t div_hdr_len = sizeof(struct Keti_UDP_Header_t);
                          send_info.div_num = 0;
                          uint32_t send_p_n = 0;
                          char *sendbuf = malloc(sizeof(char)* 1024);
                          send_info.STX = 0xAA;
                          send_info.ETX = 0xCE;
                          send_info.type[0] = 0x01;
                          send_info.type[1] = 0x02;
                          send_info.total_data_len = Now_Header->Message_size;
                          send_info.div_len = 1024 - div_hdr_len;
  
                          while(send_p_n < Now_Header->Message_size)
                          {
                              send_p_n = ((1024 - div_hdr_len) * send_info.div_num);
                              if(Now_Header->Message_size - send_p_n >= 1024)
                              {
                                  
                                  int left_len = (Now_Header->Message_size - send_p_n);
                                  memcpy(sendbuf, &send_info, div_hdr_len);
                                  printf("send_data:(%d + %d)/%d, left:%d\n", send_p_n, Now_Header->Message_size - (send_p_n + left_len),  Now_Header->Message_size, left_len);
                                  printf("\033[A");
                                  memcpy(sendbuf + div_hdr_len, Payload + send_p_n, 1024);
                                  ret = sendto(sock, sendbuf, 1024, 0, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
                              }else{
                                  int left_len = (Now_Header->Message_size - send_p_n);
                                  memcpy(sendbuf, &send_info, div_hdr_len);
                                  printf("send_data:(%d + %d)/%d, left:%d\n", send_p_n, Now_Header->Message_size - (send_p_n + left_len),  Now_Header->Message_size, left_len);
                                  printf("\033[A");
                                  memcpy(sendbuf + div_hdr_len, Payload + send_p_n, left_len);
                                  ret = sendto(sock, sendbuf, left_len + div_hdr_len, 0, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
                                  send_p_n = send_p_n + left_len;
                              }
                              if(ret <= 0)
                              {
                                  F_RelayServer_Print_Debug(0,"\n[Debug][%s][%d][send:%d/%d]\n", __func__, __LINE__, ret, Now_Header->Message_size);
                              }else{
                                  send_info.div_num  = send_info.div_num  + 1;
                              }
                              usleep(5 * 1000);
                          }
                          printf("\n");
  
                          printf("\n");
  #if 1
                          FILE *file;
                          time_t current_time;
                          time(&current_time);
                          struct tm *local_time = localtime(&current_time);
                          char *file_name = malloc(sizeof(char) * 64);
                          sprintf(file_name, "%s%04d%02d%02d_%02d%02d%02d.bin", "firmware_",local_time->tm_year  + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, \
                                                                                            local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
                          char *file_path = malloc(sizeof("./database/download/") + strlen(file_name));
                          sprintf(file_path, "%s%s", "./database/download/", file_name);
                          file = fopen(file_path, "wb");
                          fwrite(Payload, sizeof(char), Now_Header->Message_size/sizeof(char), file);
                          fclose(file);
                          Relay_safefree(file_name);
                          Relay_safefree(file_path);
  #endif  
                          uint8_t *out_data = malloc(sizeof(uint8_t) * HEADER_SIZE);
                          Now_Header->Job_State = Finish;                        
                          sprintf((char*)out_data, HEADER_PAD, Now_Header->Job_State, 0x0, Now_Header->Client_fd, Now_Header->Message_seq, 0);
                          Relay_safefree(*Data);
                          *Data = out_data;
                      }
                      break;
                  default:
                      F_RelayServer_Print_Debug(0, "[Error][%s][Job_State:%d]", __func__, Now_Header->Job_State);
                      return -1;
              }
              close(sock);
          }else{
              F_RelayServer_Print_Debug(0, "[Error][%s][No Data]\n",__func__);
              return -1;
          }
      }
      return Now_Header->Job_State;
  }
  
  int F_i_RelayServer_HTTP_Initial(uint8_t *G_HTTP_Request_Info, struct http_info_t *http_info)
  {
      uint8_t *request = G_HTTP_Request_Info;
      if(http_info)
      {
          sprintf((char*)request, "%s %s %s/%s\r\n", http_info->Request_Line.Method, http_info->Request_Line.To, http_info->Request_Line.What, http_info->Request_Line.Version);
          if(http_info->HOST){
              sprintf((char*)request, "%s%s: %s:%s\r\n", request , "Host", http_info->HOST, http_info->PORT);
          }else{
              sprintf((char*)request, "%s%s: %s:%s\r\n", request , "Host", DEFAULT_HTTP_SERVER_FIREWARE_URL, "80");
          }
          if(http_info->ACCEPT){
              sprintf((char*)request, "%s%s: %s\r\n", request , "Accept", http_info->ACCEPT);
          }else{
              sprintf((char*)request, "%s%s: %s\r\n", request , "Accept", DEFAULT_HTTP_ACCEPT);
          }
          if(http_info->CONTENT_TYPE){
              sprintf((char*)request, "%s%s: %s\r\n", request , "Content-Type", http_info->CONTENT_TYPE);
          }else{
              sprintf((char*)request, "%s%s: %s\r\n", request , "Content-Type", DEFAULT_HTTP_CONTENT_TYPE);
          }
      }else
      {
          sprintf((char*)request, "%s %s %s/%s\r\n", DEFAULT_HTTP_METHOD, DEFAULT_HTTP_SERVER_FIREWARE_URL, "HTTP", DEFAULT_HTTP_VERSION);
          sprintf((char*)request, "%s%s: %s\r\n", request , "Host", DEFAULT_HTTP_SERVER_FIREWARE_URL);
          sprintf((char*)request, "%s%s: %s\r\n", request , "Accept", DEFAULT_HTTP_ACCEPT);
          sprintf((char*)request, "%s%s: %s\r\n", request , "Content-Type", DEFAULT_HTTP_CONTENT_TYPE);
      }
  
      return 0;
  }
  
  size_t f_i_RelayServer_HTTP_Payload(uint8_t *G_HTTP_Request_Info, char *Body, size_t Body_Size, uint8_t **Http_Request)
  {
      size_t request_len;
      char *request = malloc(sizeof(char) * 512);
      if(G_HTTP_Request_Info){
          memcpy(request, G_HTTP_Request_Info, strlen((char*)G_HTTP_Request_Info));
      }else{
          return -1;
      }
      char temp_body[512] = {NULL, };
      int temp_body_len = 0;
      if(Body)
      {
          if(strstr(request, "multipart/form-data"))
          {  
              memcpy(temp_body + temp_body_len, DEFAULT_HTTP_FROMDATA_BOUNDARY, strlen(DEFAULT_HTTP_FROMDATA_BOUNDARY));
              temp_body_len += strlen(DEFAULT_HTTP_FROMDATA_BOUNDARY);
              memcpy(temp_body + temp_body_len, "\r\n", 2);
              temp_body_len += 2;
              char *from_boundary= "Content-Disposition:form-data;name=\"version\"\r\n";
              memcpy(temp_body + temp_body_len, from_boundary, strlen(from_boundary));
              temp_body_len += strlen(from_boundary);
              memcpy(temp_body + temp_body_len, "\r\n", 2);
              temp_body_len += 2;
  
              memcpy(temp_body + temp_body_len, Body, Body_Size);
              temp_body_len += Body_Size;
              memcpy(temp_body + temp_body_len, "\r\n", 2);
              temp_body_len += 2;
  
              memcpy(temp_body + temp_body_len, DEFAULT_HTTP_FROMDATA_BOUNDARY, strlen(DEFAULT_HTTP_FROMDATA_BOUNDARY));
              temp_body_len += strlen(DEFAULT_HTTP_FROMDATA_BOUNDARY);
              memcpy(temp_body + temp_body_len, "--", 2);
              temp_body_len += 2;
          }else if(strstr(request, "Application/json")){
  
          }else if(strstr(request, "Application/octet-stream")){
              memcpy(temp_body + temp_body_len, Body, Body_Size);
              temp_body_len += Body_Size;
          }
          
          if(temp_body_len > 0)
          {
              sprintf(request, "%s%s: %ld\r\n", request , "Content-Length", temp_body_len);
          }
          sprintf(request, "%s\r\n", request);
          request_len = strlen(request) + temp_body_len;
          memcpy(request + strlen(request), temp_body, temp_body_len);
          *Http_Request = malloc(sizeof(uint8_t) * request_len);
          memcpy(*Http_Request, request, request_len);
          Relay_safefree(request);
      }else {
          return -1;
      }
      return request_len;
  }
  
  # if 0
  #else // 20240909 Change //20240912 Confirm
  int f_i_RelayServer_HTTP_Task_Run(struct data_header_info_t *Now_Header, struct http_socket_info_t *http_socket_info, uint8_t **out_data)
  {
      curl_socket_t sockfd;   
      CURLcode res;
      char *URL;
      switch(http_socket_info->Now_Header->Job_State)
      {
          case FirmwareInfoRequest:
          {
              URL = DEFAULT_HTTP_SERVER_PROGRAM_URL;
              break;
          }
          case ProgramInfoRequest:
          {
              URL = DEFAULT_HTTP_SERVER_PROGRAM_URL;
              break;
          }
          default:
          {
              Now_Header->Job_State = -1;
              goto th_RelayServer_HTTP_Task_Receive_OUT;
          }    
      }
      if(1)
      {
          while(*g_curl_isused)
          {
              usleep(1 * 1000);
              printf("g_curl_isused:%d\n", *g_curl_isused);printf("\x1B[1A\r");
          }
          printf("\n");
          *g_curl_isused = 1;
          CURL *curl = curl_easy_init();
          if(curl)
          {
              curl_easy_setopt(curl, CURLOPT_URL, URL);
              curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
              curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS , 1000);
              res = curl_easy_perform(curl);
              if(res != CURLE_OK) {
                  F_RelayServer_Print_Debug(0, "[Error][%s]: %s\n", __func__, curl_easy_strerror(res));
                  Now_Header->Job_State = -1;
                  goto th_RelayServer_HTTP_Task_Receive_OUT;
              }
  
              res = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sockfd);
              if(res != CURLE_OK) {
                  F_RelayServer_Print_Debug(2, "[Error][%s]: %s\n", __func__, curl_easy_strerror(res));
                  Now_Header->Job_State = -1;
                  goto th_RelayServer_HTTP_Task_Receive_OUT;
              }
              size_t nsent_total = 0;
          do 
              {
                  size_t nsent;
                  do {
                      nsent = 0;
                      res = curl_easy_send(curl, http_socket_info->request + nsent_total, http_socket_info->request_len - nsent_total, &nsent);
                      nsent_total += nsent;
  
                      if(res == CURLE_AGAIN && !f_i_RelayServer_HTTP_WaitOnSocket(sockfd, 0, HTTP_SOCKET_TIMEOUT)) 
                      {
                          F_RelayServer_Print_Debug(2, "[Error][%s]: timeout.\n", __func__);
                          Now_Header->Job_State = -1;
                          goto th_RelayServer_HTTP_Task_Receive_OUT;
                      }
                  } while(res == CURLE_AGAIN);
  
                  if(res != CURLE_OK) 
                  {
                      F_RelayServer_Print_Debug(2, "[Error][%s]: %s\n", __func__, curl_easy_strerror(res));
                      Now_Header->Job_State = -1;
                      goto th_RelayServer_HTTP_Task_Receive_OUT;
                  }
              } while(nsent_total < http_socket_info->request_len);
  
              char buf[HTTP_BUFFER_SIZE] = {0,};
              size_t buf_len = 0;
              for(;;) 
              {
                  
                  size_t nread;
                  do {
                      nread = 0;
                      res = curl_easy_recv(curl, buf, sizeof(buf), &nread);
                      buf_len += nread;
                      if(res == CURLE_AGAIN && !f_i_RelayServer_HTTP_WaitOnSocket(sockfd, 1, HTTP_SOCKET_TIMEOUT)) 
                      {
                          F_RelayServer_Print_Debug(2, "[Error][%s]: timeout.\n", __func__);
                          Now_Header->Job_State = -1;
                          goto th_RelayServer_HTTP_Task_Receive_OUT;
                      }
                  } while(res == CURLE_AGAIN);
                  
                  if(res != CURLE_OK) 
                  {
                      buf_len = 0;
                      F_RelayServer_Print_Debug(2, "[Error][%s]: %s\n", __func__, curl_easy_strerror(res));
                      break;
                  }
                  if(nread == 0) {
                      break;
                  }
              }
              curl_easy_cleanup(curl);
              if(buf_len > 0)
              {
                  char* ptr = strstr(buf, "\r\n\r\n");
                  ptr = ptr + 4;
                  //json parsing
                  char *url_length_json = f_c_RelayServer_HTTP_Json_Object_Parser(ptr, "url_length");
                  int http_body_len = atoi(url_length_json);
                  Relay_safefree(url_length_json);
  
                  if(http_body_len > 0)
                  {
                      char *url_json = f_c_RelayServer_HTTP_Json_Object_Parser(ptr, "url\"");
                      char http_body[http_body_len];
                      memcpy(http_body, url_json, http_body_len);
                      Relay_safefree(url_json);
                      uint8_t *Http_Recv_data = malloc(sizeof(uint8_t) * (http_body_len + HEADER_SIZE));
                      memset(Http_Recv_data, 0x00, sizeof(uint8_t) * (http_body_len + HEADER_SIZE));
  
                      switch(Now_Header->Job_State)
                      {
                          case FirmwareInfoRequest:
                              Now_Header->Job_State = 4;
                              sprintf(Http_Recv_data, HEADER_PAD, 0x4, 0x0, Now_Header->Client_fd, Now_Header->Message_seq, http_body_len);
                              Now_Header->Message_size = http_body_len;
                              break;
                          case ProgramInfoRequest:
                              Now_Header->Job_State = 9;
                              sprintf(Http_Recv_data, HEADER_PAD, 0x9, 0x0, Now_Header->Client_fd, Now_Header->Message_seq, http_body_len);
                              Now_Header->Message_size = http_body_len;
                              break;
                          default:
                              memset(http_body, 0x00, http_body_len);
                              Now_Header->Job_State = -1;
                              goto th_RelayServer_HTTP_Task_Receive_OUT;
                      }
  
                      memcpy(Http_Recv_data + HEADER_SIZE, http_body, http_body_len);        
                      memset(http_body, 0x00, http_body_len);
                      Relay_safefree(*out_data);
                      
                      *out_data = Http_Recv_data;
                      goto th_RelayServer_HTTP_Task_Receive_OUT;
                  }else{
                      char *message_json = f_c_RelayServer_HTTP_Json_Object_Parser(ptr, "message");
                      printf("[DEBUG] %s\n", message_json);
                      Relay_safefree(message_json);
                      
                      Now_Header->Job_State = -1;
                      goto th_RelayServer_HTTP_Task_Receive_OUT;
                  }
  
              } 
      th_RelayServer_HTTP_Task_Receive_OUT:
  
          /* always cleanup */
          memset(buf, 0x00, sizeof(buf));
          close(sockfd);
  
          }
      }
      *g_curl_isused = 0;
      return Now_Header->Job_State;
  }
  #endif
  
   void f_v_RelayServer_HTTP_Message_Parser(char *data_ptr, char *compare_word, void **ret, size_t *ret_len)
  { 
      int ptr_right = 0;
      int compare_word_len = strlen(compare_word);
      if(ret == NULL)
      {
          return;
      }
      while(data_ptr[ptr_right])
      {
          if(strncmp(data_ptr + ptr_right,  compare_word, compare_word_len) == 0)
          {
              char *ptr = strtok(data_ptr + ptr_right, "\r\n");
              *ret = ptr;
              if(*ret_len == 0)
              {
                  strtok(ptr, "\"");
                  strtok(NULL, "\"");
                  ptr = strtok(NULL, "\"");
                  *ret = ptr;
                 size_t char_len = 0;
                  char_len = strlen(ptr);
                  *ret_len = char_len;
                  return;
              }else{
                  int test = atoi(ptr);
                  memcpy(*ret, &test, *ret_len);
                  return;
              }
              break;
          }
          ptr_right++;
      }
      *ret_len = 0;
  }
  /**
   * @brief 
   * 
   * @param json_object 
   * @param key 
   * @return 
   */
  static char* f_c_RelayServer_HTTP_Json_Object_Parser(const char *json_object, char *key)
  {
      int ret;
      char *key_ptr = strstr(json_object, key);
      
      char *tmp = malloc(strlen(key_ptr));
      memcpy(tmp, key_ptr, strlen(key_ptr));
  
      strtok(tmp, "\"");
      strtok(NULL, "\"");
      char *ptr = strtok(NULL, "\"");
      size_t ptr_len = strlen(ptr);
  
      char *out = malloc(ptr_len);
      memcpy(out, ptr, ptr_len);
      free(tmp);
      return out;
  }
  
  
int f_i_RelayServer_HTTP_WaitOnSocket(curl_socket_t sockfd, int for_recv, long timeout_ms)
{
    struct timeval tv;
    fd_set infd, outfd, errfd;
    int res;

    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (int)(timeout_ms % 1000) * 1000;

    FD_ZERO(&infd);
    FD_ZERO(&outfd);
    FD_ZERO(&errfd);

    FD_SET(sockfd, &errfd); /* always check for error */

    if(for_recv) {
    FD_SET(sockfd, &infd);
    }
    else {
    FD_SET(sockfd, &outfd);
    }

    /* select() returns the number of signalled sockets or -1 */
    res = select((int)sockfd + 1, &infd, &outfd, &errfd, &tv);
    return res;
}

/* Define NUVO */
#define DNM_Req_Signal 0x00
#define DNM_Done_Signal 0xFF

struct protobuf_data_list_t g_protobuf_data;
//* ADD 230906 */
extern void *Th_RelayServer_NUVO_Client_Task(void *d)
{
    struct NUVO_recv_task_info_t *nubo_info = (struct NUVO_recv_task_info_t*)d;
    int ret;

    // Using_Timer
    int32_t TimerFd = timerfd_create(CLOCK_REALTIME, 0);//CLOCK_MONOTONIC(시각동기화 미사용시)
    struct itimerspec itval;
    struct timespec tv;
    uint32_t timer_tick_usec = 100 * 1000; //ms
    uint64_t res = 0;
    clock_gettime(CLOCK_REALTIME, &tv); 
    itval.it_interval.tv_sec = 0;
    itval.it_interval.tv_nsec = (timer_tick_usec % 1000000) * 1e3;
    itval.it_value.tv_sec = tv.tv_sec + 1;
    itval.it_value.tv_nsec = 0;
    ret = timerfd_settime (TimerFd, TFD_TIMER_ABSTIME, &itval, NULL);
    nubo_info->life_time = 0;
    sprintf(nubo_info->ACK,"%s%02X", "ACK", 0x5D);
    nubo_info->state = 0;

    uint32_t timer_100ms_tick = 0;
    //int tick_count_10ms = 0;

    srand(time(NULL));//Random 값의 Seed 값 변경
    uint32_t timer_op_1s = ((rand() % 9) + 0);
    nubo_info->task_info_state = malloc(sizeof(int));
    *nubo_info->task_info_state = 1;

    nubo_info->sock = socket(PF_INET, SOCK_DGRAM, 0);
    
    memset(&nubo_info->serv_adr, 0, sizeof(nubo_info->serv_adr));
    nubo_info->serv_adr.sin_family = AF_INET;
    nubo_info->serv_adr.sin_addr.s_addr = inet_addr(DEFAULT_NUVO_ADDRESS);
    nubo_info->serv_adr.sin_port = htons(atoi(DEFAULT_NUVO_PORT));

    struct timeval sock_tv;                  /* Socket Send/Recv Block Timer */               
    sock_tv.tv_sec = (int)(50 / 1000);
    sock_tv.tv_usec = (90 % 1000) * 1000; 
    if (setsockopt(nubo_info->sock, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&sock_tv, sizeof(struct timeval)) == SO_ERROR) {
    perror("setsockopt(SO_RCVTIMEO)");
    }
    sock_tv.tv_usec = (50 % 1000) * 1000; 
    if (setsockopt(nubo_info->sock, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&sock_tv, sizeof(struct timeval)) == SO_ERROR) {
    perror("setsockopt(SO_SNDTIMEO)");
    }
    
    bind(nubo_info->sock, (struct sockaddr*)&(nubo_info->serv_adr), sizeof(nubo_info->serv_adr));

    printf("[DRIVING HISTORY] UDP Socket Initial\n");
    printf("[DRIVING HISTORY] UDP Socket Infomation ...... NUVO IP Address:Port - %s:%d\n", inet_ntoa(nubo_info->serv_adr.sin_addr), atoi(DEFAULT_NUVO_PORT));

    time_t now = time(NULL);
    for(int i = 0; i < 1; i++)
    {
        printf("[DRIVING HISTORY] Waiting ECU Indication ...... %ld[s](Working Time)\n", time(NULL) - now);
        sleep(1);
    }

    printf("[DRIVING HISTORY] Received ECU Start Indication ...... %ld[s]\n", time(NULL) - now);
    //printf("[DRIVING HISTORY] " "\033[0;33m" "Press Any Key" "\033[0;0m" " to continue ...... %ld[s]\n", time(NULL) - now);while(getchar() != '\n');printf("\x1B[1A\r");   
    nubo_info->state = GW_SLEEP_CONNECTIONING_NUVO;
    char Ack_Data[11] = {0,};
    nubo_info->life_time = -1;
    uint32_t Start_Save_Driving_History = 0;
    //char *file_data = NULL;
    size_t file_data_len = 0;
    time_t timer = time(NULL);
    struct tm *t = localtime(&timer);
    char file_name[28];
    sprintf(file_name, "%04d%02d%02d_%02d%02d%02d.protobuf", 1900 + t->tm_year, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    int scp_file_name_len = 0;
    char scp_file_name[256] = {0,};
    for(;;)
    {     
        ret = read(TimerFd, &res, sizeof(uint64_t));
        if(nubo_info->life_time >= 0)
        {
            nubo_info->life_time += 1;
            if((timer_100ms_tick % 50 == 0 && nubo_info->life_time >= 0) || nubo_info->life_time > 50)
            {
                Ack_Data[9] = (int)(nubo_info->life_time / 10) % 0xF0;
                
                ret = sendto(nubo_info->sock , Ack_Data, 11, 0, (struct sockaddr*)&nubo_info->serv_adr, sizeof(nubo_info->serv_adr));
                printf("[DRIVING HISTORY] [Send Ack Every 5sec] ...... %ld[s]\n", time(NULL) - now);
                nubo_info->life_time = 0;
            }
        }
        
        switch((timer_100ms_tick % 10) - timer_op_1s)
        {
            default:
            {
No_GW_SLEEP_CONNECTIONING_NUVO: 
                if(nubo_info->state != GW_RECEIVE_DRIVING_HISTORY_DATA_FROM_NOVO)
                {
                    struct sockaddr_in from_adr;
                    socklen_t from_adr_sz;
                    char recv_buf[128] = {0,};
                    ret = recvfrom(nubo_info->sock , recv_buf, 128, 0, (struct sockaddr*)&from_adr, &from_adr_sz);
                    if(ret > 0)
                    {
                        printf("[DRIVING HISTORY] [Recvive From NUVO] Received Data Length:%d ...... %ld[s]\n", ret, time(NULL) - now);
                        printf("[DRIVING HISTORY] [Recvive From NUVO] Received Data Hex Stream : ");
                        for(int k = 0; k < ret; k++)
                        {   
                            if(k == 9)
                            {
                                printf("\033[0;32m");
                            }else{
                                printf("\033[0m");
                            }         
                            printf("%02X ", recv_buf[k]);
                        }
                        printf("\n");
                    }
                    switch(recv_buf[9])
                    {
                        default:break;
                        case NUVO_SIGNAL_STATE_RES_CONNECT:
                        {
                            printf("[DRIVING HISTORY] [Recvive Response Connecting] Response From NUVO ...... %ld[s]\n", time(NULL) - now);
                            printf("[DRIVING HISTORY] [Recvive Response Connecting] Receive Success ...... %ld[s]\n", time(NULL) - now);
                            nubo_info->state = GW_REQUEST_SAVE_DRIVING_HISTORY_TO_NUVO;
                            break;
                        }
                        case NUVO_SIGNAL_STATE_RES_SAVEDATA:
                        {
                            printf("[DRIVING HISTORY] [Recvive Response Save Driving History] Response From NUVO  ...... %ld[s]\n", time(NULL) - now);
                            printf("[DRIVING HISTORY] [Recvive Response Save Driving History] Receive Success ...... %ld[s]\n", time(NULL) - now);
                            nubo_info->state = GW_WAIT_DONE_SAVE_DRIVING_HISTORY_FROM_ECU;
                            break;
                        }
                        case NUVO_SIGNAL_STATE_DOWNLOAD_PREPARE:
                        {
                            printf("[DRIVING HISTORY] [Waiting Driving History Data Info] Response From NUVO  ...... %ld[s]\n", time(NULL) - now);
                            printf("[DRIVING HISTORY] [Waiting Driving History Data Info] Receive Success ...... %ld[s]\n", time(NULL) - now);
                            memcpy(&file_data_len, recv_buf + 6 + 4, 4);
                            printf("[DRIVING HISTORY] [Waiting Driving History Data Info] Prepare File Size %ld ...... %ld[s]\n", file_data_len, time(NULL) - now);
                            nubo_info->state = GW_RECEIVE_DRIVING_HISTORY_DATA_FROM_NOVO;
                            break;
                        }
                    }
                    
                }
                switch(nubo_info->state)
                {
                    default: 
                    {
                        break;
                    }
                    case GW_WATING_REPLY_CONNECTION_FROM_NUVO:
                    {
                        if(0)
                        {
                            if(nubo_info->life_time > 20)
                            {
                                printf("[DRIVING HISTORY] [Recvive Response Connecting] Response From NUVO ...... %ld[s]\n", time(NULL) - now);
                                printf("[DRIVING HISTORY] [Recvive Response Connecting] Receive Success ...... %ld[s]\n", time(NULL) - now);
                                printf("[DRIVING HISTORY] [Recvive Response Connecting] Receive Data(Hex) ...... ");
                                char hdr[6] = {0,};
                                hdr[0] = 0x43;
                                hdr[1] = 0x08;
                                int data_length = 256;
                                memcpy(&hdr[2], &data_length, 4);
                                char STX = 0x43;
                                char ETX = 0xAA;
                                char *send_buf = malloc(sizeof(char) * (6 + 4 + 1));
                                memcpy(send_buf, hdr, 6);
                                nubo_info->ACK[0] = 'A';
                                nubo_info->ACK[1] = 'C';
                                nubo_info->ACK[2] = 'K';
                                nubo_info->ACK[3] = NUVO_SIGNAL_STATE_RES_CONNECT;
                                memcpy(send_buf + 6, &nubo_info->ACK[0], 4);
                                memcpy(send_buf + 6 + 4, &ETX, 1);
                                for(int k = 0; k < 11; k++)
                                {
                                    if(k == 9)
                                    {
                                        printf("\033[0;32m");
                                    }else{
                                        printf("\033[0m");
                                    }                                    
                                    printf("%02X ", send_buf[k]);
                                }
                                printf("\n");
                                nubo_info->life_time = 1;
                                memcpy(Ack_Data, send_buf, 11);
                                Relay_safefree(send_buf);
                                nubo_info->state = GW_REQUEST_SAVE_DRIVING_HISTORY_TO_NUVO;
                            }
                        }else{
                            if(timer_100ms_tick % 10 == 0)
                            {
                                printf("[DRIVING HISTORY] [Recvive Response Connecting] Wating Response ...... %ld[s]\n", time(NULL) - now);
                            }
                        }
                        break;
                    }
                    case GW_REQUEST_SAVE_DRIVING_HISTORY_TO_NUVO:
                    {
                        char hdr[6] = {0,};
                        hdr[0] = 0x43;
                        hdr[1] = 0x08;
                        int data_length = 256;
                        memcpy(&hdr[2], &data_length, 4);
                        char STX = 0x43;
                        char ETX = 0xAA;
                        char *send_buf = malloc(sizeof(char) * (6 + 4 + 1));
                        memcpy(send_buf, hdr, 6);
                        nubo_info->ACK[0] = 'A';
                        nubo_info->ACK[1] = 'C';
                        nubo_info->ACK[2] = 'K';
                        nubo_info->ACK[3] = NUVO_SIGNAL_STATE_REQ_SAVEDATA;
                        memcpy(send_buf + 6, &nubo_info->ACK[0], 4);
                        int DNM = 1234;
                        memcpy(send_buf + 6 + 4, &DNM, 4);
                        memcpy(send_buf + 6 + 4 + 4, &ETX, 1);
                        printf("\n");printf("[DRIVING HISTORY] " "\033[0;33m" "Press Any Key" "\033[0;0m" " to [Send Request Start Save Driving History] ...... %ld[s]\n", time(NULL) - now);while(getchar() != '\n');printf("\x1B[1A\r");
                        printf("[DRIVING HISTORY] [Send Request Start Save Driving History] 'Request Start Save Driving History To NUVO' ...... %ld[s]\n", time(NULL) - now);
                        ret = sendto(nubo_info->sock , send_buf, 11, 0, (struct sockaddr*)&nubo_info->serv_adr, sizeof(nubo_info->serv_adr));
                        printf("[DRIVING HISTORY] [Send Request Start Save Driving History] Send Success ...... %ld[s]\n", time(NULL) - now);
                        printf("[DRIVING HISTORY] [Send Request Start Save Driving History] Send Data(Hex) ...... ");
                        Start_Save_Driving_History =  time(NULL) - now;
                        for(int k = 0; k < 15; k++)
                        {
                            if(k == 9)
                            {
                                printf("\033[0;32m");
                            }else{
                                printf("\033[0m");
                            }
                            printf("%02X ", send_buf[k]);
                        }
                        printf("\n");
                        Relay_safefree(send_buf); 
                        nubo_info->state = GW_WATING_REPLY_SAVE_DRIVING_HISTORY_FROM_NUVO;
                        break;
                    }
                    case GW_WATING_REPLY_SAVE_DRIVING_HISTORY_FROM_NUVO:
                    {
                        if(0)
                        {
                            printf("[DRIVING HISTORY] [Recvive Response Save Driving History] Response From NUVO  ...... %ld[s]\n", time(NULL) - now);
                            printf("[DRIVING HISTORY] [Recvive Response Save Driving History] Receive Success ...... %ld[s]\n", time(NULL) - now);
                            printf("[DRIVING HISTORY] [Recvive Response Save Driving History] Receive Data(Hex) ...... ");
                            char hdr[6] = {0,};
                            hdr[0] = 0x43;
                            hdr[1] = 0x08;
                            int data_length = 256;
                            memcpy(&hdr[2], &data_length, 4);
                            char STX = 0x43;
                            char ETX = 0xAA;
                            char *send_buf = malloc(sizeof(char) * (6 + 4 + 1));
                            memcpy(send_buf, hdr, 6);
                            nubo_info->ACK[0] = 'A';
                            nubo_info->ACK[1] = 'C';
                            nubo_info->ACK[2] = 'K';
                            nubo_info->ACK[3] = NUVO_SIGNAL_STATE_RES_SAVEDATA;
                            memcpy(send_buf + 6, &nubo_info->ACK[0], 4);
                            int DNM = 5678;
                            memcpy(send_buf + 6 + 4, &DNM, 4);
                            memcpy(send_buf + 6 + 4 + 4, &ETX, 1);
                            for(int k = 0; k < 15; k++)
                            {
                                if(k == 9)
                                {
                                    printf("\033[0;32m");
                                }else{
                                    printf("\033[0m");
                                }                                    
                                printf("%02X ", send_buf[k]);
                            }
                            printf("\n"); 
                            nubo_info->state = GW_WAIT_DONE_SAVE_DRIVING_HISTORY_FROM_ECU;
                        }else{
                            if(timer_100ms_tick % 10 == 0)
                            {
                                printf("[DRIVING HISTORY] [Recvive Response Save Driving History] Wating Response ...... %ld[s]\n", time(NULL) - now);
                            }
                        }
                        
                    }
                    case GW_WAIT_DONE_SAVE_DRIVING_HISTORY_FROM_ECU:
                    {
                        if(1)
                        {
                            printf("[DRIVING HISTORY] Received ECU Done Indication ...... %ld[s]\n", time(NULL) - now);
                            char hdr[6] = {0,};
                            hdr[0] = 0x43;
                            hdr[1] = 0x08;
                            int data_length = 256;
                            memcpy(&hdr[2], &data_length, 4);
                            char STX = 0x43;
                            char ETX = 0xAA;
                            char *send_buf = malloc(sizeof(char) * (6 + 4 + 1));
                            memcpy(send_buf, hdr, 6);
                            nubo_info->ACK[0] = 'A';
                            nubo_info->ACK[1] = 'C';
                            nubo_info->ACK[2] = 'K';
                            nubo_info->ACK[3] = NUVO_SIGNAL_STATE_DONE_SAVEDATA;
                            memcpy(send_buf + 6, &nubo_info->ACK[0], 4);
                            int DNM = 9101112;
                            memcpy(send_buf + 6 + 4, &DNM, 4);
                            memcpy(send_buf + 6 + 4 + 4, &ETX, 1);
                            printf("\n");printf("[DRIVING HISTORY] " "\033[0;33m" "Press Any Key" "\033[0;0m" " to [Send Request Done Save Driving History] ...... %ld[s]\n", time(NULL) - now);while(getchar() != '\n');printf("\x1B[1A\r");
                            printf("[DRIVING HISTORY] [Send Request Done Save Driving History] 'Request Done Save Driving History To NUVO' ...... %ld[s]\n", time(NULL) - now);
                            #if 0 
                            struct sockaddr_in from_adr;
                            socklen_t from_adr_sz;
                            char recv_buf[1024] = {0,};
                            do{
                                ret = recvfrom(nubo_info->sock , recv_buf, 1024, 0, (struct sockaddr*)&from_adr, &from_adr_sz);
                                printf("[DRIVING HISTORY] Receive buffer Remain Data:%d ...... %ld[s]\n", ret, time(NULL) - now);
                                printf("\033[A");
                                if(ret > 0)
                                {
                                    
                                }
                            }while(ret > 0);
                            printf("\n");
                            #endif
                            ret = sendto(nubo_info->sock , send_buf, 11, 0, (struct sockaddr*)&nubo_info->serv_adr, sizeof(nubo_info->serv_adr));
                            printf("[DRIVING HISTORY] [Send Request Done Save Driving History] Send Success ...... %ld[s]\n", time(NULL) - now);
                            printf("[DRIVING HISTORY] [Send Request Done Save Driving History] Send Data(Hex) ...... ");
                            Start_Save_Driving_History =  time(NULL) - now;
                            for(int k = 0; k < 15; k++)
                            {
                                if(k == 9)
                                {
                                    printf("\033[0;32m");
                                }else{
                                    printf("\033[0m");
                                }
                                printf("%02X ", send_buf[k]);
                            }
                            printf("\n");
                            Relay_safefree(send_buf); 
                            nubo_info->state = GW_WAIT_DRIVING_HISTORY_INFO_FROM_NOVO;
                        }else{
                            if(timer_100ms_tick % 10 == 0)
                            {
                                printf("[DRIVING HISTORY] Wating ECU Done Indication ...... %ld[s]\n", time(NULL) - now);
                            }
                        }     
                        break;
                    }
                    case GW_WAIT_DRIVING_HISTORY_INFO_FROM_NOVO:
                    {
                        if(0)
                        {
                            printf("[DRIVING HISTORY] [Waiting Driving History Data Info] Response From NUVO  ...... %ld[s]\n", time(NULL) - now);
                            printf("[DRIVING HISTORY] [Waiting Driving History Data Info] Receive Success ...... %ld[s]\n", time(NULL) - now);
                            printf("[DRIVING HISTORY] [Waiting Driving History Data Info] Receive Data(Hex) ...... ");
                            char hdr[6] = {0,};
                            hdr[0] = 0x43;
                            hdr[1] = 0x08;
                            int data_length = 256;
                            memcpy(&hdr[2], &data_length, 4);
                            char STX = 0x43;
                            char ETX = 0xAA;
                            char *send_buf = malloc(sizeof(char) * (6 + 4 + 1));
                            memcpy(send_buf, hdr, 6);
                            nubo_info->ACK[0] = 'A';
                            nubo_info->ACK[1] = 'C';
                            nubo_info->ACK[2] = 'K';
                            nubo_info->ACK[3] = NUVO_SIGNAL_STATE_DOWNLOAD_PREPARE;
                            memcpy(send_buf + 6, &nubo_info->ACK[0], 4);
                            int Data_Length = 2781319;
                            memcpy(send_buf + 6 + 4, &Data_Length, 4);
                            memcpy(send_buf + 6 + 4 + 4, &ETX, 1);
                            for(int k = 0; k < 15; k++)
                            {
                                if(k == 9)
                                {
                                    printf("\033[0;32m");
                                }else{
                                    printf("\033[0m");
                                }                                    
                                printf("%02X ", send_buf[k]);
                            }
                            printf("\n"); 
                            nubo_info->state = GW_RECEIVE_DRIVING_HISTORY_DATA_FROM_NOVO;
                        }else{
                            if(timer_100ms_tick % 10 == 0)
                            {
                                printf("[DRIVING HISTORY] [Waiting Driving History Data Info] Waiting Start Upload Signal From Nuvo ...... %ld[s]\n", time(NULL) - now);
                            }
                        }     

                    }
                    case GW_RECEIVE_DRIVING_HISTORY_DATA_FROM_NOVO:
                    {
                        printf("\n");printf("[DRIVING HISTORY] " "\033[0;33m" "Press Any Key" "\033[0;0m" " to Recvive Driving History Data]...... %ld[s]\n", time(NULL) - now);while(getchar() != '\n');printf("\x1B[1A\r");
                        struct sockaddr_in from_adr;
                        socklen_t from_adr_sz;
                        size_t total_recv_len = 0;
                        time_t recv_end_time;
                        char *large_file_name;
                        int large_file_name_length = 0;
                        char *large_file_path;
#define DEFAULT_HUGE_FILE_FROM_NUVO_SAVE_LOCATION "./file_DB/From_Nuvo_Now/"
                        printf("[DRIVING HISTORY] [Recvive Driving History Data] Start Recvive From NUVO ...... %ld[s]\n", time(NULL) - now);
                        for(int tt = 0; tt < 60 * 2; tt++)
                        {   
                            char recv_buf[MAX_UDP_RECV_DATA] = {0,};
                            int recv_len = 0;
                            recv_len = recvfrom(nubo_info->sock , recv_buf, MAX_UDP_RECV_DATA, 0, (struct sockaddr*)&from_adr, &from_adr_sz);
                            if(recv_len > 0)
                            {
                                printf("[DRIVING HISTORY] [Recvive Driving History Data] Recvive Data From %s:%d ...... %d\n", inet_ntoa(from_adr.sin_addr), htons(from_adr.sin_port), recv_len);
                                printf("\033[A");

                            }
                            if(recv_buf[9] == 0xFC)
                            {
                                memcpy(&large_file_name_length, recv_buf + 6 + 4, sizeof(int));
                                large_file_name = malloc(large_file_name_length);
                                memcpy(large_file_name, recv_buf + 6 + 4 + 4, large_file_name_length);
                                large_file_path = malloc(strlen(DEFAULT_HUGE_FILE_FROM_NUVO_SAVE_LOCATION) + large_file_name_length);
                                sprintf(large_file_path, "%s%s", DEFAULT_HUGE_FILE_FROM_NUVO_SAVE_LOCATION, large_file_name);
                                printf("\n");

                            }
                            
                            if(access(large_file_path, F_OK) == 0)
                            {
                                printf("[DRIVING HISTORY] [Recvive Driving History Data] Done Recvive Data From %s:%d ...... %ld[s]\n", inet_ntoa(from_adr.sin_addr), htons(from_adr.sin_port), time(NULL) - now);
                                break;
                            }else{
                                printf("[DRIVING HISTORY] [Recvive Driving History Data] Waiting Recvive Data From %s:%d ...... %ld[s]\n", inet_ntoa(from_adr.sin_addr), htons(from_adr.sin_port), time(NULL) - now);
                                printf("\033[A");
                            }
                            usleep(100 * 1000);
                        }
                        printf("\n");
                        if(large_file_name == NULL)
                        {
                            
                        }else{
                            
                                printf("[DRIVING HISTORY] [Combine Start Driving History Data] ...... %ld[s]\n", time(NULL) - now);
                                FILE *recv_file_data = fopen(large_file_path, "rb");
                                if (recv_file_data == NULL) {
                                    printf("파일을 열 수 없습니다: %s\n", large_file_path);
                                    return -1;
                                }
                                fseek(recv_file_data, 0, SEEK_END);

                                // 현재 위치(파일 끝)의 오프셋을 가져와 파일 크기로 사용
                                total_recv_len = ftell(recv_file_data);
                                fseek(recv_file_data, 0, SEEK_SET); 
                                ret = f_i_RelayServer_Protobuf_Input_Data(&g_protobuf_data, 1, recv_file_data, total_recv_len);
                                char *v2x_file_path = "/home/root/Project_Relayserver/v2x_sample/v2x_sample.log";
                                FILE *v2x_fp_data = fopen(v2x_file_path, "rb");
                                if(v2x_fp_data)
                                {
                                    fseek(v2x_fp_data, 0, SEEK_END);
                                    size_t v2x_recv_len = ftell(v2x_fp_data);
                                    fseek(v2x_fp_data, 0, SEEK_SET);
                                    ret = f_i_RelayServer_Protobuf_Input_Data(&g_protobuf_data, 2, v2x_fp_data, v2x_recv_len);

                                }else{
                                    ret = f_i_RelayServer_Protobuf_Input_Data(&g_protobuf_data, 2, NULL, 0);
                                }
                                
                                f_i_RelayServer_Protobuf_Input_Data(&g_protobuf_data, 3, "ECUDATA", 7);
                                
                                FileData file_data = FILE_DATA__INIT;
                                ret = f_i_RelayServer_Protobuf_Create_Abs(&file_data, &g_protobuf_data);
                                
                                char* proto_file_path = "./file_DB/History_Data";
                                ret = access(proto_file_path, F_OK);
                                if(ret < 0)
                                {
                                    ret = mkdir(proto_file_path, 0777);
                                }
                                char *file_path = malloc(sizeof(char) * strlen(file_name) + strlen(proto_file_path));
                                sprintf(file_path, "%s/%s", proto_file_path, file_name);

                                size_t proto_packed_len = protobuf_savetofile(file_path, &file_data);
                                
                                //sleep(3);
                                printf("[DRIVING HISTORY] [Combine Done Driving History Data] ...... %ld[s]\n", time(NULL) - now);
                                printf("[DRIVING HISTORY] [Combine Done] File Name ...... %s\n", file_name);
                                printf("[DRIVING HISTORY] [Combine Done] File Length ...... %lu[byte]\n", proto_packed_len);
#define HISTORY_DEVICE_ID 11323
                                printf("\n");printf("[DRIVING HISTORY] " "\033[0;33m" "Press Any Key" "\033[0;0m" " to [Send DRIVING HISTORY DATA To Server] ...... File_Name:" "\033[0;31m" "%s" "\033[0;0m" "\n", file_name);while(getchar() != '\n');printf("\x1B[1A\r");
                                printf("[DRIVING HISTORY] [Send Start History Data To Server] ...... %ld[s]\n\n", time(NULL) - now);
                                char cmd[512] = {0,};
                                //char *upload_url = "http://itp-self.wtest.biz//v1/system/IDSUpload.php";
                                char *upload_url = "http://itp-self.wtest.biz//v1/system/firmwareUpload.php";

                                sprintf(cmd, "curl -X POST \"%s\"   -H \"Content-Type: multipart/form-data\" \
                                                                    -F \"title=%s_%04d_%03d\" \
                                                                    -F \"deviceId=%04d\" \
                                                                    -F \"file=@%s;type=application/octet-stream\"", upload_url, "History", HISTORY_DEVICE_ID, 001, HISTORY_DEVICE_ID, large_file_path);
                                FILE *fp = popen(cmd, "r");
                                if (fp == NULL) {
                                    printf("Failed to run command\n" );
                                    exit(1);
                                }
                                
                                char buffer[128];
                                while (fgets(buffer, sizeof(buffer), fp) != NULL) {
                                    printf("%s", buffer);
                                }
                                memset(cmd, 0x00, 512);
                                pclose(fp);

                                sprintf(cmd, "rm -rf %s/*", DEFAULT_HUGE_FILE_FROM_NUVO_SAVE_LOCATION);
                                system(cmd);

                                printf("\n");
                                printf("[DRIVING HISTORY] [Send Done History Data To Server] 'Send Success' ...... %ld[s]\n", time(NULL) - now);
                                printf("[DRIVING HISTORY] [Send Done History Data To Server] Send Data(Hex) ...... ");
                                printf("[DRIVING HISTORY] [Send Done History Data To Server] ...... %ld[s]\n", time(NULL) - now);
                                
                                //fclose(v2x_fp_data);_DEBUG_LOG       
                                //fclose(recv_file_data);
                         
                                Relay_safefree(file_path);_DEBUG_LOG
                                Relay_safefree(large_file_path);_DEBUG_LOG
                                Relay_safefree(large_file_name);_DEBUG_LOG
                                
                                sleep(2);
                          }   
                          
                          goto GW_JOB_BY_NUBO_DONE;
                          break;
                      }
                  }
              }
              case 0:
              {
                  switch(nubo_info->state)
                  {
                      default:
                      {
                          goto No_GW_SLEEP_CONNECTIONING_NUVO;
                          break;
                      }
                      case GW_TRYING_CONNECTION_NUVO:
                      case GW_TRYING_CONNECTION_NUVO_REPEAT_1:
                      case GW_TRYING_CONNECTION_NUVO_REPEAT_2:
                      case GW_TRYING_CONNECTION_NUVO_REPEAT_3:
                      case GW_TRYING_CONNECTION_NUVO_REPEAT_4:
                      case GW_TRYING_CONNECTION_NUVO_REPEAT_5:
                      case GW_SLEEP_CONNECTIONING_NUVO:
                      {
                          srand(time(NULL));//Random 값의 Seed 값 변경
                          usleep(((rand() % 20) + 4) * 1000); // 매초 + 4~20ms의 랜덤값을 갖는 시간에 동작
                          char hdr[6] = {0,};
                          hdr[0] = 0x43;
                          hdr[1] = 0x08;
                          int data_length = 256;
                          memcpy(&hdr[2], &data_length, 4);
                          //char STX = 0x43;
                          char ETX = 0xAA;
                          char *send_buf = malloc(sizeof(char) * (6 + 4 + 1));
                          memcpy(send_buf, hdr, 6);
                          nubo_info->ACK[0] = 'A';
                          nubo_info->ACK[1] = 'C';
                          nubo_info->ACK[2] = 'K';
                          nubo_info->ACK[3] = NUVO_SIGNAL_STATE_REQ_CONNECT;
                          memcpy(send_buf + 6, &nubo_info->ACK[0], 4);
                          memcpy(send_buf + 6 + 4, &ETX, 1);
                          printf("\n");printf("[DRIVING HISTORY] " "\033[0;33m" "Press Any Key" "\033[0;0m" " to [Send Request Connecting]...... %ld[s]\n", time(NULL) - now);while(getchar() != '\n');printf("\x1B[1A\r");
                          printf("[DRIVING HISTORY] [Send Request Connecting] 'Connecting To NUVO' ...... %ld[s]\n", time(NULL) - now);
                          ret = sendto(nubo_info->sock , send_buf, 11, 0, (struct sockaddr*)&nubo_info->serv_adr, sizeof(nubo_info->serv_adr));
                          if(ret <= 0)
                          {
                              if(nubo_info->state != GW_TRYING_CONNECTION_NUVO)
                              {
                                  nubo_info->state = GW_TRYING_CONNECTION_NUVO;
                              }else{
                                  nubo_info->state++;
                                  if(nubo_info->state == GW_TRYING_CONNECTION_NUVO_REPEAT_MAX)
                                  {
                                      goto CONNECTION_REPEAT_MAX;
                                  }
                              }
                              printf("[DRIVING HISTORY] [Send Request Connecting] 'Connecting To NUVO Error - Count:%d' ...... %ld[s]\n", nubo_info->state - GW_TRYING_CONNECTION_NUVO, time(NULL) - now);
                          }
                          printf("[DRIVING HISTORY] [Send Request Connecting] Send Success ...... %ld[s]\n", time(NULL) - now);
                          printf("[DRIVING HISTORY] [Send Request Connecting] Send Data(Hex) ...... ");
                          for(int k = 0; k < 11; k++)
                          {
                              if(k == 9)
                              {
                                  printf("\033[0;32m");
                              }else{
                                  printf("\033[0m");
                              }
                              printf("%02X ", send_buf[k]);
                          }
                          printf("\n");
                          Relay_safefree(send_buf);
                          nubo_info->life_time = 0;
                          nubo_info->state = GW_WATING_REPLY_CONNECTION_FROM_NUVO;
                          break;
                      }
                  }
                  break;
              }
          }
          timer_100ms_tick = (timer_100ms_tick + 1) % 0xF0; 
      }
  
  
  GW_JOB_BY_NUBO_DONE:
  
  CONNECTION_REPEAT_MAX:
  
      if(nubo_info->state == GW_TRYING_CONNECTION_NUVO_REPEAT_MAX)
      {
          *nubo_info->task_info_state = -15;
          close(nubo_info->sock);
          Relay_safefree(nubo_info->task_info_state);
      }else{
          *nubo_info->task_info_state = 2;
          close(nubo_info->sock);
          *nubo_info->task_info_state = 0;
          Relay_safefree(nubo_info->task_info_state);
      }
  }
  
   int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms)
  {
    struct timeval tv;
    fd_set infd, outfd, errfd;
    int res;
   
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (int)(timeout_ms % 1000) * 1000;
   
    FD_ZERO(&infd);
    FD_ZERO(&outfd);
    FD_ZERO(&errfd);
   
    FD_SET(sockfd, &errfd); /* always check for error */
   
    if(for_recv) {
      FD_SET(sockfd, &infd);
    }
    else {
      FD_SET(sockfd, &outfd);
    }
   
    /* select() returns the number of signalled sockets or -1 */
    res = select((int)sockfd + 1, &infd, &outfd, &errfd, &tv);
    return res;
  }
  
  
   int f_i_RelayServer_HTTP_Check_URL(const char *url) {
      CURL *curl;
      CURLcode res;
      long response_code = 0;
  
      // curl 초기화
      curl = curl_easy_init();
      if(curl) {
          // URL 설정
          curl_easy_setopt(curl, CURLOPT_URL, url);
          // 응답을 받지 않게 설정
          curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); // HEAD 요청만 보냄
          curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS , 1000);
          // 요청 실행
          res = curl_easy_perform(curl);
  
          // 요청이 성공했는지 확인
          if (res == CURLE_OK) {
              // HTTP 응답 코드 확인
              curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
  
              if (response_code == 200) {
                  printf("URL is valid and reachable: %ld\n", response_code);
              } else {
                  printf("URL is not reachable, HTTP response code: %ld\n", response_code);
              }
          } else {
              printf("URL:%s\n", url);
              printf("curl_easy_perform() URL:%s, failed: \n%s\n", curl_easy_strerror(res), url);
          }
  
          // curl 정리
          curl_easy_cleanup(curl);
          curl_global_cleanup();
          
      }
  
      return (res == CURLE_OK && response_code == 200) ? 1 : 0;  // 1 = valid, 0 = invalid
  }
  
  
  #define _V2X_ENABLE 1
  #if _V2X_ENABLE
  
  #define DEFAULT_TIMESTAMP_SIZE 19
  #define DEFAULT_V2X_SAVE_FILE_PATH "./file_DB/v2x_history/"
  
  char g_timestamp[DEFAULT_TIMESTAMP_SIZE];
  
  bool g_v2x_data_save_start;
  bool g_v2x_file_is;
  
  static void Debug_Msg_Print_Data(int msgLv, unsigned char* data, int len)
  {
      int rep;
      if(msgLv <= 3)
      {
          printf("\n\t (Len : 0x%X(%d) bytes)", len, len);
          printf("\n\t========================================================");
          printf("\n\t Hex.   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
          printf("\n\t--------------------------------------------------------");
          for(rep = 0 ; rep < len ; rep++)
          {
              if(rep % 16 == 0) printf("\n\t %03X- : ", rep/16);
              printf("%02X ", data[rep]);
          }
          printf("\n\t========================================================");
          printf("\n\n");
      }
  }
  extern void *Th_RelayServer_V2X_UDP_Task(void *arg)
  {
      arg = (void*)arg;
  
      int recv_sock, send_sock;
      struct sockaddr_in recv_addr, send_addr;
  
      recv_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
      send_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  
      bzero(&recv_addr,sizeof(recv_addr));
      recv_addr.sin_family = AF_INET;
      recv_addr.sin_addr.s_addr = inet_addr(MYHOSTNAME);
      recv_addr.sin_port = htons(S_PORT);
      bind(recv_sock, (struct sockaddr*) &recv_addr, sizeof(recv_addr));
  
      bzero(&send_addr,sizeof(send_addr));
      send_addr.sin_family = AF_INET;
      send_addr.sin_addr.s_addr = inet_addr(S_HOSTNAME);
      send_addr.sin_port = htons(S_PORT);
  
      struct linger solinger = { 1, 0 };  /* Socket FD close when the app down. */
      setsockopt(recv_sock, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger));
      setsockopt(send_sock, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger));
  
      struct timeval t_val={1, 0}; //sec, msec
      setsockopt(recv_sock, SOL_SOCKET, SO_RCVTIMEO, &t_val, sizeof(t_val));
      setsockopt(send_sock, SOL_SOCKET, SO_SNDTIMEO, &t_val, sizeof(t_val));
  
      int ret_recv, ret_send, recv_len;
  
      FILE *v2x_file_db;
      int ptr_now = 0;
      f_v_Timestamp_Get();
      char *v2x_file_path = malloc(sizeof(char) * (strlen(g_timestamp) * 2) + strlen(DEFAULT_V2X_SAVE_FILE_PATH) - 3);
      
      memcpy(v2x_file_path + ptr_now, DEFAULT_V2X_SAVE_FILE_PATH, strlen(DEFAULT_V2X_SAVE_FILE_PATH));
      ptr_now += strlen(DEFAULT_V2X_SAVE_FILE_PATH);
      memcpy(v2x_file_path + ptr_now, g_timestamp, strlen(g_timestamp) - 4);
      ptr_now += strlen(g_timestamp) - 4;
      memcpy(v2x_file_path + ptr_now, "/", 1);
      ptr_now += 1;
      if(!access(v2x_file_path, F_OK))
      {
          mkdir(v2x_file_path);
      }
  
      while(1)
      {
          if(g_v2x_data_save_start && !g_v2x_file_is)
          {
              f_v_Timestamp_Get();
              memcpy(v2x_file_path + ptr_now, g_timestamp, 19);
              v2x_file_db = fopen(v2x_file_path, "wb");
              if(v2x_file_db)
              {
                  g_v2x_file_is = true;
              }
          }
           
          struct sockaddr_in client_addr;
          bzero(&client_addr,sizeof(client_addr));
          char msg[1024] = {0,};
          ret_recv = recvfrom(recv_sock, (char*)msg, 1024, 0, (struct sockaddr *)&client_addr, &recv_len);
          char temp_sendr_id[4] = {0x00, 0x22, 0x55, 0x33};
          if(ret_recv > 0)
          {
              Msg_Header *t_msg_hdr = (Msg_Header *)msg;            
              if(t_msg_hdr->MagicKey == 0xF1F1)
              {
                  if(g_v2x_data_save_start && g_v2x_file_is)
                  {
                      char *file_data_buf = malloc(sizeof(char) * (1 + 19 + 4 + ret_recv + 1));
                      ptr_now = 0;
                      memcpy(file_data_buf + ptr_now, 0xA8, 1);
                      ptr_now += 1;
                      f_v_Timestamp_Get();
                      memcpy(file_data_buf + ptr_now, g_timestamp, strlen(g_timestamp));
                      ptr_now += 19;
                      memcpy(file_data_buf + ptr_now, &ret_recv, sizeof(int));
                      ptr_now += sizeof(int);
                      memcpy(file_data_buf + ptr_now, msg, ret_recv);
                      ptr_now += ret_recv;
                      memcpy(file_data_buf + ptr_now, 0x38, 1);
                      ptr_now += 1;
                      fwrite(file_data_buf, sizeof(char), ptr_now, v2x_file_db);
                      Relay_safefree(file_data_buf);
                  }
                  DNM_Req *t_msg_req =(DNM_Req *)msg;
                  switch(t_msg_req->header.MsgType)
                  {
                      default:
                      {
                          break;
                      }
                      case 4://DNM_REQ
                      {
                          printf("[DEBUG] recvfrom() UDP read len : %d\n", ret_recv);
                          printf("[DEBUG] t_msg_hdr->MagicKey = %04X\n", t_msg_hdr->MagicKey);
                          printf("[DEBUG] t_msg_hdr->MsgType = %d\n", t_msg_hdr->MsgType);
                          printf("[DEBUG] t_msg_hdr->PacketLen = %d\n", t_msg_hdr->PacketLen);
                          printf("[DEBUG] t_msg_req->Sender = %08X\n", t_msg_req->Sender);
                          printf("[DEBUG] t_msg_req->Receiver = %08X\n", t_msg_req->Receiver);
                          printf("[DEBUG] t_msg_req->RemainDistance = %02X\n", t_msg_req->RemainDistance);
                          char send_buf[1024] = {0,};
                          DNM_Res *t_msg_res =(DNM_Res *)send_buf;
                          t_msg_res->header.MagicKey = 0xF1F1;
                          t_msg_res->header.MsgType = 5;
                          t_msg_res->header.PacketLen = htons(sizeof(DNM_Res));
                          
                          memcpy(&t_msg_res->Sender, temp_sendr_id, 4);
                          t_msg_res->Receiver = t_msg_req->Sender;
                          //t_msg_req->Receiver = htonl(11);
                          t_msg_res->AgreeFlag = 0; //Default - Agreement 1, Disagreement 0
                          
                          ret_send = sendto(send_sock, (char*)send_buf, ntohs(t_msg_res->header.PacketLen), 0, (struct sockaddr*)&send_addr, sizeof(send_addr));
                          if(ret_send < 0)
                          {
                              printf("[DEBUG] Send Error ! :%d\n", ret_send);
                          }else{
                              printf("[DEBUG] Send Sucess ! :%d\n", ret_send);
                              Debug_Msg_Print_Data(3, (char*)send_buf, ntohs(t_msg_res->header.PacketLen));	
                          }
  
                          break;
                      }
                      case 1://ETRI_TYPE_BSM_NOTI
                      {
  
                          break;
                      }
                      case 2://ETRI_TYPE_PIM_NOTI
                      {
                          break;
                      }
                  }
                 
              }
          }
  
      }
      if(g_v2x_data_save_start && g_v2x_file_is)
      {
          fclose(v2x_file_db);
      }
      Relay_safefree(v2x_file_path);
      return (void*)NULL;
  }
  
  
  
  #endif
  
  
  #if 1 //20241010 PROTOBUF 병합 기능 추가
  
  int f_i_RelayServer_Protobuf_Input_Data(struct protobuf_data_list_t *data, enum protobuf_type_e type, char *input_data, size_t input_data_len)
  {
      struct protobuf_data_list_t *protobuf_data = (struct protobuf_data_list_t*)data;
      switch(type)
      {
          default:break;
          case 1:
          {
              if(!protobuf_data->data_nubo.data_isalloc)
              {
                  if(input_data)
                  {
                      protobuf_data->data_nubo.data = input_data;
                      //printf("protobuf_data->data_nubo.data:%p\n", protobuf_data->data_nubo.data);
                      protobuf_data->data_nubo.data_isalloc = true;
                      protobuf_data->data_nubo.data_len = (size_t)input_data_len;
                  }else{
                      return -1;
                  }
              }
              break;
          }
          case 2:
          {
              if(!protobuf_data->data_v2x.data_isalloc)
              {
                if(input_data){
                    protobuf_data->data_v2x.data = input_data;
                    //printf("protobuf_data->data_v2x.data:%p\n", protobuf_data->data_v2x.data);
                    protobuf_data->data_v2x.data_isalloc = true;
                    protobuf_data->data_v2x.data_len = (size_t)input_data_len;
                }else{
                    return -2;
                }
            }
            break;
        }
          case 3:
          {
              if(!protobuf_data->data_ecuids.data_isalloc)
              {
                if(input_data){
                    protobuf_data->data_ecuids.data = input_data;
                    //printf("protobuf_data->data_ecuids.data:%p\n", protobuf_data->data_ecuids.data);
                    protobuf_data->data_ecuids.data_isalloc = true;
                    protobuf_data->data_ecuids.data_len = (size_t)input_data_len;
                }else{
                    return -3;
                }
              }
              break;
           }
      }
      
      return 0;
  }

  f_i_RelayServer_Protobuf_Create_Abs(FileData *protobuf, struct protobuf_data_list_t *protobuf_data)
  {
      if(protobuf_data->data_nubo.data_isalloc)
      {
          protobuf->content_1.data = protobuf_data->data_nubo.data;
          //printf("protobuf->content_1.data:%p\n", protobuf->content_1.data);
          protobuf->content_1.len = protobuf_data->data_nubo.data_len;
      }
      if(protobuf_data->data_v2x.data_isalloc)
      {
          protobuf->content_2.data = protobuf_data->data_v2x.data;
          //printf("protobuf->content_2.data:%p\n", protobuf->content_2.data);
          protobuf->content_2.len = protobuf_data->data_v2x.data_len;
      }
      if(protobuf_data->data_ecuids.data_isalloc)
      {
          protobuf->content_3.data = protobuf_data->data_ecuids.data;
          //printf("protobuf->content_3.data:%p\n", protobuf->content_3.data);
          protobuf->content_3.len = protobuf_data->data_ecuids.data_len;
      }
      return 0;
  }
  
  #endif
  
  void f_v_Timestamp_Get()
  {
      struct timeval tv;
      gettimeofday(&tv, NULL); 
      int milliseconds = (tv.tv_usec / 1000) % 1000;
     
      time_t timer = time(NULL);
      struct tm *t = localtime(&timer);
      memset(g_timestamp, 0x00, DEFAULT_TIMESTAMP_SIZE);
      sprintf(g_timestamp, "%04d%02d%02d_%02d%02d%02d_%03d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, milliseconds);
  }
  
  char *Timestamp_Print()
  {
      f_v_Timestamp_Get();
      return g_timestamp;
  }
  