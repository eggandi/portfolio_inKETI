#include <V2XHO_Router_Include.h>

#define DEFAULT_ROUTE_LIST_NUM_MAX 256
#define DEFAUlT_ROUTE_FILE_PATH "./route_table"
#define DEFAUlT_ROUTE_FILE_NAME "init_table.table"

extern struct rtnl_handle rth;

struct V2XHO_IProuter_Route_List_info_IPv4_t
{
    struct in_addr dest_addr;
    struct in_addr gateway_addr;
    uint32_t metric;
    char dev_name[64];
    uint16_t dev_name_len;
};

struct V2XHO_IProuter_Route_List_Info_IPv6_t
{
    struct in6_addr dest_addr;
    uint32_t dest_prefix;
    struct in6_addr gateway_addr;
    uint32_t metric;
    uint32_t dev_name_len;
    char dev_name[64];

    int route_list_num;
};

