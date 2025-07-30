#include "unix_domain_socket.h"
#include "share_memory.h"

#include <time.h>
#include <sys/time.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#define DTG_CONFIG_FILE_DEFAULT_PATH "./configs/DTG/"

enum VH_DTG_Data_State_e{
    DTG_SEND_HEADER = 1,
    DTG_RECV_O = 2,
    DTG_SEND_1sDATA,
};

struct VH_DTG_Data_Date_t{
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
};

struct VH_DTG_Data_1s_t
{
    uint8_t STX;
    uint8_t DataLength;
    uint8_t Idx[2];
    struct VH_DTG_Data_Date_t Date;
    uint8_t DayOdometer[2];
    uint8_t TotalOdometer[2];
    uint8_t Speed;
    uint32_t GPS_X;
    uint32_t GPS_Y;
    uint8_t Angle[2];
    uint8_t RPM[2];
    uint8_t BREAK;
    uint8_t ETX;
};

struct VH_DTG_Data_Header_t
{
    uint8_t STX;
    uint8_t DataLength;
    uint8_t Model_Name[20];
    uint8_t VIN[17];
    uint8_t Car_Type[2];
    uint8_t VRN[12];
    uint8_t BusineddNumber[10];
    uint8_t DriverCode[18];
    uint8_t ETX;
};