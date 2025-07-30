#include <V2XHO_Agent_Advertisements_Define.h>

extern int F_i_V2XHO_Agent_Advertisement_Do(struct V2XHO_Agent_Info_t *agent_info, struct V2XHO_IP_Header_Info_t ip_hdr_info);
extern int F_i_V2XHO_Agent_Advertisement_Set_ICMP_Code(struct V2XHO_Agent_Info_t *agent_info, enum V2XHO_ICMP_Header_Code_e code);
extern int F_i_V2XHO_Agent_Advertisement_Set_ICMP_LifeTime(struct V2XHO_Agent_Info_t *agent_info, uint16_t *lifetime);
extern struct V2XHO_extention_0_t* F_s_V2XHO_Agent_Advertisements_Initial_Extention_0(struct V2XHO_Agent_Info_t *agent_info, struct V2XHO_extention_0_t *extention_0);