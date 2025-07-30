#ifndef _V2XHO_DEBUG_PINGTEST_H_
#define _V2XHO_DEBUG_PINGTEST_H_

struct v2xho_debug_pingtest_info_t {
    char dest_addr[INET_ADDRSTRLEN];
    uint32_t interval_ms;
    uint32_t packet_size;
};

#endif