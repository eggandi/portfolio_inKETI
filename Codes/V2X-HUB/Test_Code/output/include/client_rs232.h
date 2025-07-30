#include "unix_domain_socket.h"
#include "share_memory.h"

#include <time.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

struct DSM_Event_Bitfield{
    char Drowsiness :1;
    char Distraction :1;
    char Yawning :1;
    char Phone :1;
    char Smoking :1;
    char Driver_Absence :1;
    char Mask :1;
    char Seat_Belt :1;  
};

struct Setting_Bitfield_0{
    uint8_t OnOff:1; 
    uint8_t Sound:2;
    uint8_t Vibration:1;
    uint8_t Sensitivity:2;
    uint8_t Dummy :2;
};

struct Setting_Bitfield_1{
    uint8_t Video_Format :1;
    uint8_t Point_Overlay_on_preview :1;
    uint8_t Transmit_Drowsiness :1;
    uint8_t Camera_block_repeat_alarm :1;
    uint8_t Transmit_Not_Listed :1;
    uint8_t Transmit_Distraction :1;
    uint8_t Transmit_Phone :1;
    uint8_t Transmit_Smoking :1;
};

struct Setting_Bitfield_2{
    uint8_t Voice :1;
    uint8_t G_Sensor :3;
    uint8_t log :1;
    uint8_t Dummy :3;
};

struct Setting_Bitfield_3{
    uint8_t Hardware_Switch :1;
    uint8_t Mirror_Display :1;
    uint8_t Test_Mode :1;
    uint8_t Type_of_transmit :1;
    uint8_t Timeout_feature :1;
    uint8_t Dummy :3;
};

struct VH_DMS_Config_Setting_t
{
    uint8_t STX;
    uint8_t Type1;
    uint8_t Type2;
    uint8_t Data_Length;
    struct Setting_Bitfield_0 Drowsiness_Setting; 
    struct Setting_Bitfield_0 Distration_Setting; 
    struct Setting_Bitfield_0 Phone_Setting; 
    struct Setting_Bitfield_0 Smoking_Setting; 
    struct Setting_Bitfield_0 Yawning_Setting; 
    struct{
        uint8_t Kmpermile:1; 
        uint8_t SeatBelt_Setting_OnOff:1;
        uint8_t SeatBelt_Setting_Sound:2; 
        uint8_t SeatBelt_Setting_Driver_Pos:1;
        uint8_t SeatBelt_Setting_Dummy:3;
    } bit_filed_0;
    uint8_t Volume_Level;
    uint8_t Working_Speed;
    struct Setting_Bitfield_2 DVR_Setting1;
    uint8_t DVR_Setting2;
    struct Setting_Bitfield_1 Miscellaneous_Setting;
    uint8_t Reboot;
    uint8_t Year[2];
    uint8_t Mon;
    uint8_t Day;
    uint8_t Hour;
    uint8_t Min;
    uint8_t Sec;
    struct Setting_Bitfield_3 Dip_Switch;
    uint8_t Snapshot_Request;
    uint8_t Working_Speed_Drowsiness;
    uint8_t Working_Speed_Distraction;
    uint8_t Working_Speed_Yawning;
    uint8_t Working_Speed_Phone;
    uint8_t Working_Speed_Smoking;
    uint8_t Checksum;
    uint8_t EXT;
};

struct VH_DMS_Snapshot_Hdr_t
{
    uint8_t STX;
    uint8_t Type1;
    uint8_t Type2;
    uint8_t Sending_Data_Format;
    uint8_t Get_Picture_Package;
    uint16_t Current_Package_Data_Size;
    uint8_t Current_Package_Number;
    uint16_t Total_Picture_Size_In_Byte;
    uint8_t Total_Picture_Packages;
};

struct VH_DMS_Snapshot_Retransmission_Hdr_t
{
    uint8_t STX;
    uint8_t Type1;
    uint8_t Type2;
    uint8_t Snap_Total_Index[2];
    uint8_t Snap_Current_Index[2];
    uint8_t DSM_Event;
    uint8_t Date_Year[2];
    uint8_t Date_Mon;
    uint8_t Date_Day;
    uint8_t Date_Hour;
    uint8_t Date_Min;
    uint8_t Date_Sec;
    uint32_t Location_Latitude;
    uint32_t Location_Longitude;
    uint8_t Serial_Number[15];
    uint8_t Speed;
    uint8_t KmPerMile;
    uint8_t Driver_name[10];
    uint32_t JPEG_Pkt_Current_Binary_Data_Size;
    uint32_t JPEG_Pkt_Total_Binary_Data_Size;
    uint8_t JPEG_Pkt_Current_Binary_Num;
    uint8_t JPEG_Pkt_Total_Binary_Num;
};