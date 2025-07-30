#include <stdio.h>     // 사용된 함수: printf, fprintf, perror, snprintf
#include <stdlib.h>    // 사용된 함수: malloc, free, exit, strdup
#include <stdint.h>    // 사용된 타입: uint8_t, uint16_t 등
#include <string.h>    // 사용된 함수: memset, memcpy, strlen, strdup
#include <arpa/inet.h> // 사용된 함수: inet_pton, htons
#include <unistd.h>    // 사용된 함수: close, write (및 기타 POSIX API)
#include <errno.h>     // 사용된 변수: errno (perror 내부에서 사용)
#include <stdbool.h>
#include <fcntl.h>     // 사용된 함수/매크로: open, O_RDWR, O_NOCTTY, O_SYNC
#include <termios.h>   // 사용된 매크로: B115200 (시리얼 통신 설정 등)

#ifndef _D_HEADER_GNSS
#define _D_HEADER_GNSS

#define GPSD_SERVER "127.0.0.1"
#define GPSD_PORT   2947

// NAV-PVT: 클래스=0x01, 메시지ID=0x07
#define UBX_SYNC1       0xB5
#define UBX_SYNC2       0x62
#define UBX_CLASS_NAV   0x01
#define UBX_ID_PVT      0x07

// (고정 길이 92바이트, ZED-F9P 기준)
#pragma pack(push, 1)
typedef struct {
    uint32_t iTOW;       // GPS time of week (ms)
    uint16_t year;       // Year (UTC)
    uint8_t  month;      // Month, range 1..12 (UTC)
    uint8_t  day;        // Day of month, range 1..31 (UTC)
    uint8_t  hour;       // Hour of day, range 0..23 (UTC)
    uint8_t  min;        // Minute of hour, range 0..59 (UTC)
    uint8_t  sec;        // Seconds of minute, range 0..60 (UTC)
    uint8_t  valid;      // Fix validity flags
    uint32_t tAcc;       // Time accuracy estimate (ns)
    int32_t  nano;       // Fraction of second (ns)
    uint8_t  fixType;    // GNSSfix Type (0: no fix, 1: dead reckoning, 2: 2D, 3: 3D, ...)
    uint8_t  flags;      // Fix status flags
    uint8_t  flags2;     // Additional flags
    uint8_t  numSV;      // Number of SVs used for navigation
    int32_t  lon;        // Longitude (1e-7)
    int32_t  lat;        // Latitude (1e-7)
    int32_t  height;     // Height above ellipsoid (mm)
    int32_t  hMSL;       // Height above mean sea level (mm)
    uint32_t hAcc;       // Horizontal accuracy estimate (mm)
    uint32_t vAcc;       // Vertical accuracy estimate (mm)
    int32_t  velN;       // NED north velocity (mm/s)
    int32_t  velE;       // NED east velocity (mm/s)
    int32_t  velD;       // NED down velocity (mm/s)
    int32_t  gSpeed;     // Ground speed (2-D, mm/s)
    int32_t  heading;    // Heading of motion (2-D, 1e-5 degrees)
    uint32_t sAcc;       // Speed accuracy estimate (mm/s)
    uint32_t headingAcc; // Heading accuracy estimate (1e-5 degrees)
    uint16_t pDOP;       // Position DOP (0.01)
    uint8_t  reserved1[6];
    int32_t  headVeh;    // Heading of vehicle (2-D, 1e-5 degrees)
    int16_t  magDec;     // Magnetic declination (1e-2 degrees)
    uint16_t magAcc;     // Magnetic declination accuracy (1e-2 degrees)
} __attribute__((__packed__)) ubx_nav_pvt_t;
#pragma pack(pop)

#endif //?_D_HEADER_GNSS

extern bool G_gpsd_task_isrun;
extern void *PRO_UBLOX_Gnss_Thread(void *arg);
extern int PRO_UBLOX_Initial_Set();