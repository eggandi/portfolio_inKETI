#include <V2XHO_Node_Define.h>

extern void* F_th_V2XHO_Node_Task_Doing(void *data);

extern int F_i_V2XHO_Node_Route_Add(struct V2XHO_Node_Info_t *node_info, enum V2XHO_Ip_Dst_Type_e dst_type, char *address, int prefix, char *gateway_addr, bool local_gateway, enum v2xho_util_router_metric_state_e metric, char *table, bool NonForeced);
extern int F_i_V2XHO_Node_Interface_Addr_Add(struct V2XHO_Node_Info_t *node_info, enum V2XHO_Ip_Dst_Type_e dst_type, char *address, int prefix);

extern int F_i_V2XHO_Node_ICMP_Router_addr_Add(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs, struct V2XHO_Router_Addr_es_t *router_addrs);
extern int F_i_V2XHO_Node_ICMP_Router_addr_Del(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs, uint32_t *router_addrs);
extern int F_i_V2XHO_Node_ICMP_Router_addr_Flush(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs, bool Not_all);
extern int F_i_V2XHO_Node_Set_ICMP_Preference_Level(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs,  uint32_t *router_addrs, int Preference_Level)
;