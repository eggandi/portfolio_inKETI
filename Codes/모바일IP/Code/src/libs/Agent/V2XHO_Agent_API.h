#include <V2XHO_Agent_Define.h>

extern void* F_th_V2XHO_Agent_Task_Doing(void *data);
extern int F_i_V2XHO_Agent_Extention_Set(struct V2XHO_Agent_Info_t *agent_info, uint8_t extention_num, void *data);
extern int F_i_V2XHO_Agent_Extention_Get(struct V2XHO_Agent_Info_t *agent_info, uint8_t extention_num, void *out_data);

extern int F_i_V2XHO_Agent_ICMP_Router_addr_Add(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs, struct V2XHO_Router_Addr_es_t *router_addrs);
extern int F_i_V2XHO_Agent_ICMP_Router_addr_Del(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs,  uint32_t *router_addrs);
extern int F_i_V2XHO_Agent_ICMP_Router_addr_Flush(struct V2XHO_Router_Addr_es_t **router_es, uint16_t *num_addrs);
extern int F_i_V2XHO_Agent_Set_ICMP_Preference_Level(struct V2XHO_Agent_Info_t *agent_info,  uint32_t *router_addrs, int Preference_Level);

extern int F_i_V2XHO_Agent_Route_Add(struct V2XHO_Agent_Info_t *agent_info, enum V2XHO_Ip_Dst_Type_e dst_type, char *address, int prefix, char *gateway_addr, bool local_gateway, enum v2xho_util_router_metric_state_e metric, char *table, bool NonForeced);
extern int F_i_V2XHO_Agent_Interface_Addr_Add(struct V2XHO_Agent_Info_t *agent_info, enum V2XHO_Ip_Dst_Type_e dst_type, char *address, int prefix);
