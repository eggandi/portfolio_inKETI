#ifndef _V2XHO_UTIL_ROUTER_H_
#define _V2XHO_UTIL_ROUTER_H_

typedef struct {
    uint16_t htype;      // 하드웨어 타입 (Ethernet은 1)
    uint16_t ptype;      // 프로토콜 타입 (IPv4는 0x0800)
    uint8_t hlen;        // 하드웨어 주소 길이 (Ethernet은 6)
    uint8_t plen;        // 프로토콜 주소 길이 (IPv4는 4)
    uint16_t oper;       // ARP 오퍼레이션 (1: Request, 2: Reply)
    uint8_t sha[6];      // 발신자 하드웨어 주소 (MAC)
    uint8_t spa[4];      // 발신자 프로토콜 주소 (IP)
    uint8_t tha[6];      // 대상 하드웨어 주소 (MAC)
    uint8_t tpa[4];      // 대상 프로토콜 주소 (IP)
} __attribute__((__packed__)) arp_header_t;

enum v2xho_util_router_metric_state_e{

    V2XHO_METRIC_STATIC_ACTIVE_WITH_GW = 128,
    V2XHO_METRIC_STATIC_ACTIVE,
    V2XHO_METRIC_DYNAMIC_ACTIVE_WITH_GW = 256,
    V2XHO_METRIC_DYNAMIC_ACTIVE,
    V2XHO_METRIC_DEFAULT_ACTIVE_WITH_GW = 384,
    V2XHO_METRIC_DEFAULT_ACTIVE,

    V2XHO_METRIC_STATIC_STANDBY_WITH_GW = 512,
    V2XHO_METRIC_STATIC_STANDBY,
    V2XHO_METRIC_DYNAMIC_STANDBY_WITH_GW = 640,
    V2XHO_METRIC_DYNAMIC_STANDBY,
    V2XHO_METRIC_DEFAULT_STANDBY_WITH_GW = 768,
    V2XHO_METRIC_DEFAULT_STANDBY,

    V2XHO_METRIC_STATIC_SLEEP_WITH_GW = 1024,
    V2XHO_METRIC_STATIC_SLEEP,
    V2XHO_METRIC_DYNAMIC_SLEEP_WITH_GW = 1152,
    V2XHO_METRIC_DYNAMIC_SLEEP,
    V2XHO_METRIC_DEFAULT_SLEEP_WITH_GW = 1280,
    V2XHO_METRIC_DEFAULT_SLEEP,
    
    V2XHO_METRIC_SPECIAL_ACTIVE = 2048,
    V2XHO_METRIC_INITIAL = 1024000,
};

#endif