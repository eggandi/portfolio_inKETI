/**
 * @file
 * @brief
 * @date 2020-07-14
 * @author gyun
 */


#ifndef V2X_SW_DOT3_DEFINES_H
#define V2X_SW_DOT3_DEFINES_H


#ifndef MAC_ALEN
#define MAC_ALEN (6) ///< MAC 주소 길이 (바이트 단위)
#endif

#ifndef IPv6_ALEN
#define IPv6_ALEN (16) ///< IPv6 주소 길이 (바이트 단위)
#endif

#ifndef IPv6_ADDR_STR_MAX_LEN
#define IPv6_ADDR_STR_MAX_LEN ((IPv6_ALEN*2)+7) ///< IPv6 주소 문자열 최대 길이 (바이트 단위)
#endif

#ifndef AC_NUM
#define AC_NUM (4) ///< Access Category 개수
#endif

#ifndef MAC_QOS_HLEN
#define MAC_QOS_HLEN (26) ///< MAC QoS 헤더 길이 (바이트 단위)
#endif

#ifndef LLC_HLEN
#define LLC_HLEN (2) ///< LLC 헤더 길이 (바이트 단위)
#endif


#endif //V2X_SW_DOT3_DEFINES_H
