#include <V2XHO_Router_Define.h>

extern int F_i_V2XHO_Router_Hendler_Open(struct rtnl_handle *rthl);
extern void F_v_V2XHO_Router_Hendler_Close(struct rtnl_handle *rthl);
extern int F_i_V2XHO_IProuter_Route_Save_File(char *file_name);
extern int F_i_V2XHO_IProuter_Route_Load_File(char *file_name);
extern int F_i_V2XHO_IProuter_Route_Flush();
extern int F_i_V2XHO_IProuter_Route_Add(char *address, int *prefix, char *gateway, bool local_gateway, char *interface, int *metric, char *table, bool forecd);
extern int F_i_V2XHO_IProuter_Route_Del(char *address, int *prefix, char *table);
extern int F_i_V2XHO_IProuter_Route_Get_List(char *dev_name, struct V2XHO_IProuter_Route_List_info_IPv4_t *route_list, int *list_num);
extern int F_i_V2XHO_IProuter_Address_Set(char *interface, char *address, int *prefix, bool force);
extern int F_i_V2XHO_Router_Address_Del(char* interface, char* address, int *prefix);

extern int F_i_V2XHO_Iproute_Route_Modify(int cmd, unsigned int flags, int argc, char **argv);
extern int F_i_V2XHO_Iproute_Route_Flush_Cache(int argc, char **argv, int action);
extern int F_i_V2XHO_Iproute_Link_Modify(int cmd, unsigned int flags, int argc, char **argv);
extern int F_i_V2XHO_Iproute_Rule_Modify(int cmd, int argc, char **argv);
extern int F_i_V2XHO_Iproute_Tunnel_Modify(int cmd, int argc, char **argv);
