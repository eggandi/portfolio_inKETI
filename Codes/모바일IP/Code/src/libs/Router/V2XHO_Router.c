#include <V2XHO_Router.h>

static int f_i_Router_IProute_Modify(int cmd, unsigned int flags, int argc, char **argv); 

static int f_i_V2XHO_IProuter_Route_Dump_Filter(struct nlmsghdr *nlh, int reqlen); 
static int f_i_V2XHO_Iproute_Route_Print(struct nlmsghdr *n, void *arg); 
static void f_v_V2XHO_Iproute_Route_Reset_Filter(int ifindex); 
static int f_i_V2XHO_Iproute_Link_Parse_Args(int argc, char **argv, struct iplink_req *req, char **type); 
static int F_i_V2XHO_Iproute_Tunnel_Parse_Args(int argc, char **argv, int cmd, struct ip_tunnel_parm *p); 

struct link_util *f_i_V2XHO_Iproute_Link_GetLinkKind(const char *id); 

static int nl_get_ll_addr_len(const char *ifname); 
static void *BODY; 		/* cached dlopen(NULL) handle */
static struct link_util *linkutil_list; 

struct V2XHO_IProuter_Route_List_info_IPv4_t g_route_list_info[DEFAULT_ROUTE_LIST_NUM_MAX]; 
int g_list_num; 
static struct
{
	unsigned int tb;
	int cloned;
	int flushed;
	char *flushb;
	int flushp;
	int flushe;
	int protocol, protocolmask;
	int scope, scopemask;
	__u64 typemask;
	int tos, tosmask;
	int iif, iifmask;
	int oif, oifmask;
	int mark, markmask;
	int realm, realmmask;
	__u32 metric, metricmask;
	inet_prefix rprefsrc;
	inet_prefix rvia;
	inet_prefix rdst;
	inet_prefix mdst;
	inet_prefix rsrc;
	inet_prefix msrc;
} filter; 

const char *get_ip_lib_dir(void)
{
	const char *lib_dir;

	lib_dir = getenv("IP_LIB_DIR");
	if (!lib_dir)
		lib_dir = "/usr/lib/ip";

	return lib_dir;
}


extern int F_i_V2XHO_Router_Hendler_Open(struct rtnl_handle *rthl)
{
	int ret = rtnl_open(rthl, 0);
    if(ret < 0)
    {
        printf("ret:%d\n", ret);
		return -1;
    }
	return 0;
}

extern void F_v_V2XHO_Router_Hendler_Close(struct rtnl_handle *rthl)
{
	rtnl_close(rthl);
}

extern int F_i_V2XHO_IProuter_Route_Save_File(char *file_name)
{
	int ret;
	char sys[128] = {0,};
	ret = access(DEFAUlT_ROUTE_FILE_PATH, F_OK);
	if(ret < 0)
	{
		mkdir(DEFAUlT_ROUTE_FILE_PATH, 777);
	}

	if(file_name)
	{
		sprintf(sys, "%s%s", DEFAUlT_ROUTE_FILE_PATH, file_name);
	}else{
				sprintf(sys, "ip route save > %s/%s", DEFAUlT_ROUTE_FILE_PATH, DEFAUlT_ROUTE_FILE_NAME);
	}
	ret = system(sys);
    if (ret == -1) 
	{
        perror("system");
    } else {
        // 정상적으로 종료되었는지 확인
        if (WIFEXITED(ret)) {
        }
        // 비정상 종료(신호로 인해 종료) 여부 확인
        else if (WIFSIGNALED(ret)) {
            printf("Command was terminated by signal: %d\n", WTERMSIG(ret));
        }
    }
	return ret;
}

extern int F_i_V2XHO_IProuter_Route_Flush()
{
	int ret;
	ret = system("ip route flush table all");
	ret = system("ip route flush all");
    if (ret == -1) {
        perror("system");
    } else {
        // 정상적으로 종료되었는지 확인
        if (WIFEXITED(ret)) {
        }
        // 비정상 종료(신호로 인해 종료) 여부 확인
        else if (WIFSIGNALED(ret)) {
            printf("Command was terminated by signal: %d\n", WTERMSIG(ret));
        }
    }
	return ret;
}
extern int F_i_V2XHO_IProuter_Route_Load_File(char *file_name)
{
    int ret;
	char sys[128] = {0,};
	ret = access(DEFAUlT_ROUTE_FILE_PATH, F_OK);
	if(ret < 0)
	{
		mkdir(DEFAUlT_ROUTE_FILE_PATH, 777);
	}

	ret = F_i_V2XHO_IProuter_Route_Flush();
	usleep(100 * 1000);
	if(file_name)
	{
		sprintf(sys, "ip route restore < %s/%s", DEFAUlT_ROUTE_FILE_PATH, file_name);
	}else{
		sprintf(sys, "ip route restore < %s/%s", DEFAUlT_ROUTE_FILE_PATH, DEFAUlT_ROUTE_FILE_NAME);
	}
	ret = system(sys);
    if (ret == -1) {
        perror("system");
    } else {
        // 정상적으로 종료되었는지 확인
        if (WIFEXITED(ret)) {
        }
        // 비정상 종료(신호로 인해 종료) 여부 확인
        else if (WIFSIGNALED(ret)) {
            printf("Command was terminated by signal: %d\n", WTERMSIG(ret));
        }
    }
	return ret;
}

extern int F_i_V2XHO_IProuter_Route_Add(char *address, int *prefix, char *gateway, bool local_gateway, char *interface, int *metric, char *table, bool forecd)
{
	int input_arg_list_num = 0;
	char *input_arg_list[64];
	if(forecd >= 1)
	{
		F_i_V2XHO_IProuter_Route_Del(address, prefix, table);
	}
	char *dst_addr = address;
	if(strncmp(dst_addr, "default", 7) == 0){
		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * 32);
		memcpy(input_arg_list[input_arg_list_num], dst_addr, 7);
	}else{
		if(*prefix >= 0)
		{
			input_arg_list[input_arg_list_num] = malloc(sizeof(char) * 64);
			if(prefix == NULL)
			{
				sprintf(input_arg_list[input_arg_list_num], "%s/%d", dst_addr, 32);
			}else{
				sprintf(input_arg_list[input_arg_list_num], "%s/%02d", dst_addr, *prefix);
			}
			input_arg_list_num++;
		}
	}
	
	if(gateway)
	{
		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * 3);
		sprintf(input_arg_list[input_arg_list_num], "%s", "via");
		input_arg_list_num++;

		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * 32);
		sprintf(input_arg_list[input_arg_list_num], "%s", gateway);
		input_arg_list_num++;
		if(!local_gateway)
		{
			input_arg_list[input_arg_list_num] = malloc(sizeof(char) * 6);
			sprintf(input_arg_list[input_arg_list_num], "%s", "onlink");
			input_arg_list_num++;
		}
	}
	if(interface)
	{
		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * 3);
		sprintf(input_arg_list[input_arg_list_num], "%s", "dev");
		input_arg_list_num++;

		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * strlen(interface));
		sprintf(input_arg_list[input_arg_list_num], "%s", interface);
		input_arg_list_num++;
	}else{
		return -1;
	}
	if(*metric)
	{
		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * 6);
		sprintf(input_arg_list[input_arg_list_num], "%s", "metric");
		input_arg_list_num++;

		input_arg_list[input_arg_list_num] = malloc(sizeof(int));
		sprintf(input_arg_list[input_arg_list_num], "%d", *metric);
		input_arg_list_num++;
	}
	if(table)
	{
		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * 5);
		sprintf(input_arg_list[input_arg_list_num], "%s", "table");
		input_arg_list_num++;

		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * strlen(table));
		sprintf(input_arg_list[input_arg_list_num], "%s", table);
		input_arg_list_num++;
	}
	
	return f_i_Router_IProute_Modify(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_EXCL, input_arg_list_num, input_arg_list);
}

extern int F_i_V2XHO_IProuter_Route_Del(char *address, int *prefix, char *table)
{
	int input_arg_list_num = 0;
	char *input_arg_list[64];
	
	char *dst_addr = address;
	if(*prefix >= 0)
	{
		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * 32);
		if(*prefix == 0)
		{
			sprintf(input_arg_list[input_arg_list_num], "%s/%d", dst_addr, *prefix);
		}else
		{	
			sprintf(input_arg_list[input_arg_list_num], "%s/%02d", dst_addr, *prefix);
		}
		input_arg_list_num++;
	}
	
	if(table)
	{
		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * 5);
		sprintf(input_arg_list[input_arg_list_num], "%s", "table");
		input_arg_list_num++;

		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * strlen(table));
		sprintf(input_arg_list[input_arg_list_num], "%s", table);
		input_arg_list_num++;
	}
	
	return f_i_Router_IProute_Modify(RTM_DELROUTE, 0, input_arg_list_num, input_arg_list);
}

extern int F_i_V2XHO_IProuter_Route_Get_List(char *dev_name, struct V2XHO_IProuter_Route_List_info_IPv4_t *route_list, int *list_num)
{
	int dump_family = preferred_family;
	rtnl_filter_t filter_fn;
	int idx;

	filter_fn = f_i_V2XHO_Iproute_Route_Print;
	f_v_V2XHO_Iproute_Route_Reset_Filter(0);

	filter.tb = RT_TABLE_MAIN;

	if (dump_family == AF_UNSPEC && filter.tb)
		dump_family = AF_INET;
	
	idx = ll_name_to_index(dev_name);
	if (!idx)
		return nodev(dev_name);
	filter.oif = idx;
	if (rtnl_routedump_req(&rth, dump_family, f_i_V2XHO_IProuter_Route_Dump_Filter) < 0) {
		perror("Cannot send dump request");
		return -2;
	}

	if (rtnl_dump_filter(&rth, filter_fn, stdout) < 0) {
		fprintf(stderr, "Dump terminated\n");
		return -2;
	}
	fflush(stdout);

	if(route_list)
	{
		for(int i = 0; i<DEFAULT_ROUTE_LIST_NUM_MAX; i++)
		{
			if(g_route_list_info[i].dest_addr.s_addr != 0 || g_route_list_info[i].gateway_addr.s_addr != 0)
			{
				route_list[i] = g_route_list_info[i];
			}
		g_route_list_info[i].dest_addr.s_addr = 0;
		g_route_list_info[i].gateway_addr.s_addr = 0;
		g_route_list_info[i].dev_name_len = 0;
		memset(g_route_list_info[i].dev_name, 0x00, 64);
		}
	}

	if(list_num)
	{
		*list_num = g_list_num;
		g_list_num = 0;
	}
	return 0;
}

extern int F_i_V2XHO_IProuter_Address_Set(char *interface, char *address, int *prefix, bool force)
{
	if(force)
	{
		char cmd[128] = {0,};
		sprintf(cmd, "ip address flush dev %s", interface);
		system(cmd);memset(cmd, 0x00, 128);
	}
	if(!address || !interface)
	{
		return -2;
	}
	if(*prefix > 32 || !prefix)
	{
		return -*prefix;
	}
	struct {
		struct nlmsghdr	n;
		struct ifaddrmsg	ifa;
		char			buf[256];
	} req = {
		.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifaddrmsg)),
		.n.nlmsg_flags = NLM_F_REQUEST | NLM_F_CREATE|NLM_F_EXCL,
		.n.nlmsg_type = RTM_NEWADDR,
		.ifa.ifa_family = preferred_family,
	};
	char  *d = NULL;
	inet_prefix lcl = {};
	unsigned int ifa_flags = 0;
	int ret;
    char *router_addr = address;
	if(*prefix >= 0)
	{
		char *router_addr_prefix = malloc(sizeof(char) * 32);
		if(*prefix == 0)
		{
			sprintf(router_addr_prefix, "%s/%d", router_addr, 32);
		}else
		{	
			sprintf(router_addr_prefix, "%s/%02d", router_addr, *prefix);
		}
		get_prefix(&lcl, router_addr_prefix, req.ifa.ifa_family);
		if (req.ifa.ifa_prefixlen == 0)
			req.ifa.ifa_prefixlen = lcl.bitlen;
		free(router_addr_prefix);
	}else{
		get_prefix(&lcl, router_addr, req.ifa.ifa_family);
	}
    if (req.ifa.ifa_family == AF_UNSPEC)
	{
        req.ifa.ifa_family = lcl.family;
	}
	addattr_l(&req.n, sizeof(req), IFA_LOCAL, &lcl.data, lcl.bytelen);
    //local_len = lcl.bytelen;

    d = interface;
    req.ifa.ifa_index = ll_name_to_index(d);
	if (ifa_flags <= 0xff)
    {
		req.ifa.ifa_flags = ifa_flags;
    }else
    {
		addattr32(&req.n, sizeof(req), IFA_FLAGS, ifa_flags);
    }

    ret = rtnl_talk(&rth, &req.n, NULL);
    if(ret < 0)
    {
        printf("ret:%d\n", ret);
		return ret;
    }
	return 0;
}

extern int F_i_V2XHO_Router_Address_Del(char* interface, char* address, int *prefix)
{
	if(!address && !interface)
	{
		return -2;
	}
	if(*prefix > 32 || !prefix)
	{
		return -*prefix;
	}

	struct {
		struct nlmsghdr	n;
		struct ifaddrmsg	ifa;
		char			buf[256];
	} req = {
		.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifaddrmsg)),
		.n.nlmsg_flags = NLM_F_REQUEST,
		.n.nlmsg_type = RTM_DELADDR,
		.ifa.ifa_family = preferred_family,
	};
	char  *d = NULL;
	inet_prefix lcl = {};
	unsigned int ifa_flags = 0;
	int ret;
    
	char *router_addr = address;
	if(*prefix >= 0)
	{
		char *router_addr_prefix = malloc(sizeof(char) * 32);
		if(*prefix == 0)
		{
			sprintf(router_addr_prefix, "%s/%d", router_addr, 32);
		}else
		{	
			sprintf(router_addr_prefix, "%s/%02d", router_addr, *prefix);
		}
		get_prefix(&lcl, router_addr_prefix, req.ifa.ifa_family);
		if (req.ifa.ifa_prefixlen == 0)
			req.ifa.ifa_prefixlen = lcl.bitlen;
		free(router_addr_prefix);
	}else{
		get_prefix(&lcl, router_addr, req.ifa.ifa_family);
	}
    if (req.ifa.ifa_family == AF_UNSPEC)
	{
        req.ifa.ifa_family = lcl.family;
	}
    
    if (req.ifa.ifa_family == AF_UNSPEC)
        req.ifa.ifa_family = lcl.family;
    addattr_l(&req.n, sizeof(req), IFA_ADDRESS, &lcl.data, lcl.bytelen);
	d = interface;
    req.ifa.ifa_index = ll_name_to_index(d);
	if (ifa_flags <= 0xff)
    {
		req.ifa.ifa_flags = ifa_flags;
    }else
    {
		addattr32(&req.n, sizeof(req), IFA_FLAGS, ifa_flags);
    }
    ret = rtnl_talk(&rth, &req.n, NULL);
    if(ret < 0)
    {

    }
	
	return 0;
}

static int f_i_Router_IProute_Modify(int cmd, unsigned int flags, int argc, char **argv)
{
	struct {
		struct nlmsghdr	n;
		struct rtmsg		r;
		char			buf[4096];
	} req = {
		.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)),
		.n.nlmsg_flags = NLM_F_REQUEST | flags,
		.n.nlmsg_type = cmd,
		.r.rtm_family = preferred_family,
		.r.rtm_table = RT_TABLE_MAIN,
		.r.rtm_scope = RT_SCOPE_NOWHERE,
	};
	char  mxbuf[256];
	struct rtattr *mxrta = (void *)mxbuf;
	unsigned int mxlock = 0;
	char  *d = NULL;
	int gw_ok = 0;
	int dst_ok = 0;
	int nhs_ok = 0;
	int scope_ok = 0;
	int table_ok = 0;
	//int raw = 0;
	int type_ok = 0;
	__u32 nhid = 0;
	int ret;

	if (cmd != RTM_DELROUTE) {
		req.r.rtm_protocol = RTPROT_BOOT;
		req.r.rtm_scope = RT_SCOPE_UNIVERSE;
		req.r.rtm_type = RTN_UNICAST;
	}

	mxrta->rta_type = RTA_METRICS;
	mxrta->rta_len = RTA_LENGTH(0);

	while (argc > 0) {
		if (strcmp(*argv, "via") == 0) {
			inet_prefix addr;
			int family;

			if (gw_ok) {
				invarg("use nexthop syntax to specify multiple via\n",
				       *argv);
			}
			gw_ok = 1;
			NEXT_ARG();
			family = read_family(*argv);
			if (family == AF_UNSPEC)
				family = req.r.rtm_family;
			else
				NEXT_ARG();
			get_addr(&addr, *argv, family);
			if (req.r.rtm_family == AF_UNSPEC)
				req.r.rtm_family = addr.family;
			if (addr.family == req.r.rtm_family)
				addattr_l(&req.n, sizeof(req), RTA_GATEWAY,
					  &addr.data, addr.bytelen);
			else
				addattr_l(&req.n, sizeof(req), RTA_VIA,
					  &addr.family, addr.bytelen+2);
		}  else if (matches(*argv, "metric") == 0 || matches(*argv, "priority") == 0 ||strcmp(*argv, "preference") == 0) {
			__u32 metric;

			NEXT_ARG();
			if (get_u32(&metric, *argv, 0))
				invarg("\"metric\" value is invalid\n", *argv);
			addattr32(&req.n, sizeof(req), RTA_PRIORITY, metric);
		}else if (strcmp(*argv, "onlink") == 0) {
			req.r.rtm_flags |= RTNH_F_ONLINK;
		} else if (matches(*argv, "protocol") == 0) {
			__u32 prot;
			NEXT_ARG();
			if (rtnl_rtprot_a2n(&prot, *argv))
				invarg("\"protocol\" value is invalid\n", *argv);
			req.r.rtm_protocol = prot;
		} else if (matches(*argv, "table") == 0) {
			__u32 tid;
			NEXT_ARG();
			if (rtnl_rttable_a2n(&tid, *argv))
				invarg("\"table\" value is invalid\n", *argv);
			if (tid < 256)
				req.r.rtm_table = tid;
			else {
				req.r.rtm_table = RT_TABLE_UNSPEC;
				addattr32(&req.n, sizeof(req), RTA_TABLE, tid);
			}
			table_ok = 1;
		} else if (strcmp(*argv, "dev") == 0 || strcmp(*argv, "oif") == 0) {
			NEXT_ARG();
			d = *argv;
		/*} else if (strcmp(*argv, "encap") == 0) 
		{
			char buf[1024];
			struct rtattr *rta = (void *)buf;

			rta->rta_type = RTA_ENCAP;
			rta->rta_len = RTA_LENGTH(0);

			lwt_parse_encap(rta, sizeof(buf), &argc, &argv,
					RTA_ENCAP, RTA_ENCAP_TYPE);

			if (rta->rta_len > RTA_LENGTH(0))
				addraw_l(&req.n, 1024
					 , RTA_DATA(rta), RTA_PAYLOAD(rta));
		*/} else {
			//int type;
			inet_prefix dst;
			if (dst_ok)
				duparg2("to", *argv);
			get_prefix(&dst, *argv, req.r.rtm_family);
			if (req.r.rtm_family == AF_UNSPEC)
				req.r.rtm_family = dst.family;
			req.r.rtm_dst_len = dst.bitlen;
			dst_ok = 1;
			if (dst.bytelen)
				addattr_l(&req.n, sizeof(req),
					  RTA_DST, &dst.data, dst.bytelen);
		}
		argc--; argv++;
	}
	
	if (d) {
		int idx = ll_name_to_index(d);

		if (!idx)
			return nodev(d);
		addattr32(&req.n, sizeof(req), RTA_OIF, idx);
	}

	if (mxrta->rta_len > RTA_LENGTH(0)) {
		if (mxlock)
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_LOCK, mxlock);
		addattr_l(&req.n, sizeof(req), RTA_METRICS, RTA_DATA(mxrta), RTA_PAYLOAD(mxrta));
	}

	if (req.r.rtm_family == AF_UNSPEC)
		req.r.rtm_family = AF_INET;

	if (!table_ok) {
		if (req.r.rtm_type == RTN_LOCAL || req.r.rtm_type == RTN_BROADCAST || req.r.rtm_type == RTN_NAT || req.r.rtm_type == RTN_ANYCAST)
			req.r.rtm_table = RT_TABLE_LOCAL;
	}
	if (!scope_ok) {
		if (req.r.rtm_family == AF_INET6 || req.r.rtm_family == AF_MPLS)
			req.r.rtm_scope = RT_SCOPE_UNIVERSE;
		else if (req.r.rtm_type == RTN_LOCAL || req.r.rtm_type == RTN_NAT)
			req.r.rtm_scope = RT_SCOPE_HOST;
		else if (req.r.rtm_type == RTN_BROADCAST || req.r.rtm_type == RTN_MULTICAST || req.r.rtm_type == RTN_ANYCAST)
			req.r.rtm_scope = RT_SCOPE_LINK;
		else if (req.r.rtm_type == RTN_UNICAST || req.r.rtm_type == RTN_UNSPEC) 
		{
			if (cmd == RTM_DELROUTE)
				req.r.rtm_scope = RT_SCOPE_NOWHERE;
			else if (!gw_ok && !nhs_ok && !nhid)
				req.r.rtm_scope = RT_SCOPE_LINK;
		}
	}

	if (!type_ok && req.r.rtm_family == AF_MPLS)
		req.r.rtm_type = RTN_UNICAST;


	ret = rtnl_talk(&rth, &req.n, NULL);

	if (ret)
		return -2;

	return 0;
}

int f_i_V2XHO_Iproute_Route_Print(struct nlmsghdr *n, void *arg)
{
	arg = (void *)arg;
	struct rtmsg *r = NLMSG_DATA(n);
	int len = n->nlmsg_len;
	struct rtattr *tb[RTA_MAX+1];
	int family, host_len;
	SPRINT_BUF(b1);
	SPRINT_BUF(b2);

	host_len = af_bit_len(r->rtm_family);
	parse_rtattr(tb, RTA_MAX, RTM_RTA(r), len);
	if(r->rtm_table == RT_TABLE_MAIN)
	{		
		if (tb[RTA_DST]) {
			family = get_real_family(r->rtm_type, r->rtm_family);
			if (r->rtm_dst_len != host_len) {
				snprintf(b1, sizeof(b1),
					"%s", rt_addr_n2a_rta(family, tb[RTA_DST]));
					//r->rtm_dst_len);
			} else {
				const char *hostname = format_host_rta_r(family, tb[RTA_DST],
						b2, sizeof(b2));
				if (hostname)
					strncpy(b1, hostname, sizeof(b1) - 1);
			}
		} else if (r->rtm_dst_len) {
			snprintf(b1, sizeof(b1), "0/%d ", r->rtm_dst_len);
		} else {
			strncpy(b1, "default", sizeof(b1));
		}
		//print_color_string(PRINT_ANY, COLOR_NONE, "dst", "%s ", b1);
		if (tb[RTA_OIF])
		{
			const char *ifname = ll_index_to_name(rta_getattr_u32(tb[RTA_OIF]));
			g_route_list_info[g_list_num].dev_name_len = strlen(ifname);
			memcpy(g_route_list_info[g_list_num].dev_name, ifname, g_route_list_info[g_list_num].dev_name_len);
		}
		if(tb[RTA_GATEWAY])
		{
			const char *gateway = format_host_rta(r->rtm_family, tb[RTA_GATEWAY]);
			//print_color_string(PRINT_FP, ifa_family_color(r->rtm_family), NULL, "%s ", gateway);

			g_route_list_info[g_list_num].dest_addr.s_addr = inet_addr(b1);
			g_route_list_info[g_list_num].gateway_addr.s_addr = inet_addr(gateway);
			g_list_num = g_list_num + 1;
			
		}else{
			g_route_list_info[g_list_num].dest_addr.s_addr = inet_addr(b1);
			g_route_list_info[g_list_num].gateway_addr.s_addr = inet_addr("0.0.0.0");
			g_list_num = g_list_num + 1;
		}
		
	}
	return 0;
}
void f_v_V2XHO_Iproute_Route_Reset_Filter(int ifindex)
{
	memset(&filter, 0, sizeof(filter));
	filter.mdst.bitlen = -1;
	filter.msrc.bitlen = -1;
	filter.oif = ifindex;
	if (filter.oif > 0)
		filter.oifmask = -1;
}

static int f_i_V2XHO_IProuter_Route_Dump_Filter(struct nlmsghdr *nlh, int reqlen)
{
	struct rtmsg *rtm = NLMSG_DATA(nlh);
	int err;

	rtm->rtm_protocol = filter.protocol;
	if (filter.cloned)
		rtm->rtm_flags |= RTM_F_CLONED;
	if (filter.tb) {
		err = addattr32(nlh, reqlen, RTA_TABLE, filter.tb);
		if (err)
			return err;
	}
	if (filter.oif) {
		err = addattr32(nlh, reqlen, RTA_OIF, filter.oif);
		if (err)
			return err;
	}

	return 0;
}


/*
int Router_Test()
{
	int ret;
	char *interface = "eth0";
	char *address = "244.0.0.1";
	rth = malloc(sizeof(struct rtnl_handle));
	ret = F_i_V2XHO_Router_Hendler_Open(rth);
	if(ret < 0)
    {
        printf("ret:%d\n", ret);
    }
	int prefix = 0;
	ret = F_i_V2XHO_IProuter_Address_Set(rth, interface, address, &prefix);

	char *gateway = "192.168.0.2";
	bool local_gateway = true;
	enum v2xho_util_router_metric_state_e metric = 100;
	ret = F_i_V2XHO_IProuter_Route_Add(address, &prefix, gateway, local_gateway,  interface, &metric, NULL, false);

	if(ret < 0)
    {
        printf("ret:%d\n", ret);
    }

    F_i_V2XHO_Router_Hendler_Close(rth);
	free(rth);
	return 0;
}
*/

//F_i_V2XHO_Iproute_Route_Modify(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_REPLACE, int argc, char **argv)

extern int F_i_V2XHO_Iproute_Route_Flush_Cache(int argc, char **argv, int action)
{
	argc = argc;
	argv = argv;
	action = action;
	return 0;
}

extern int F_i_V2XHO_Iproute_Route_Modify(int cmd, unsigned int flags, int argc, char **argv)
{
	struct {
		struct nlmsghdr	n;
		struct rtmsg		r;
		char			buf[4096];
	} req = {
		.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)),
		.n.nlmsg_flags = NLM_F_REQUEST | flags,
		.n.nlmsg_type = cmd,
		.r.rtm_family = preferred_family,
		.r.rtm_table = RT_TABLE_MAIN,
		.r.rtm_scope = RT_SCOPE_NOWHERE,
	};
	char  mxbuf[256];
	struct rtattr *mxrta = (void *)mxbuf;
	unsigned int mxlock = 0;
	char  *d = NULL;
	int gw_ok = 0;
	int dst_ok = 0;
	int nhs_ok = 0;
	int scope_ok = 0;
	int table_ok = 0;
	int raw = 0;
	int type_ok = 0;
	__u32 nhid = 0;
	int ret;

	if (cmd != RTM_DELROUTE) {
		req.r.rtm_protocol = RTPROT_BOOT;
		req.r.rtm_scope = RT_SCOPE_UNIVERSE;
		req.r.rtm_type = RTN_UNICAST;
	}

	mxrta->rta_type = RTA_METRICS;
	mxrta->rta_len = RTA_LENGTH(0);

	while (argc > 0) {
		if (strcmp(*argv, "src") == 0) {
			inet_prefix addr;

			NEXT_ARG();
			get_addr(&addr, *argv, req.r.rtm_family);
			if (req.r.rtm_family == AF_UNSPEC)
				req.r.rtm_family = addr.family;
			addattr_l(&req.n, sizeof(req),
				  RTA_PREFSRC, &addr.data, addr.bytelen);
		} else if (strcmp(*argv, "as") == 0) {
			inet_prefix addr;
			NEXT_ARG();
			if (strcmp(*argv, "to") == 0) {
				NEXT_ARG();
			}
			get_addr(&addr, *argv, req.r.rtm_family);
			if (req.r.rtm_family == AF_UNSPEC)
				req.r.rtm_family = addr.family;
			addattr_l(&req.n, sizeof(req),
				  RTA_NEWDST, &addr.data, addr.bytelen);
		} else if (strcmp(*argv, "via") == 0) {
			inet_prefix addr;
			int family;

			if (gw_ok) {
				invarg("use nexthop syntax to specify multiple via\n",
				       *argv);
			}
			gw_ok = 1;
			NEXT_ARG();
			family = read_family(*argv);
			if (family == AF_UNSPEC)
				family = req.r.rtm_family;
			else
				NEXT_ARG();
			get_addr(&addr, *argv, family);
			if (req.r.rtm_family == AF_UNSPEC)
				req.r.rtm_family = addr.family;
			if (addr.family == req.r.rtm_family)
				addattr_l(&req.n, sizeof(req), RTA_GATEWAY,
					  &addr.data, addr.bytelen);
			else
				addattr_l(&req.n, sizeof(req), RTA_VIA,
					  &addr.family, addr.bytelen+2);
		} else if (strcmp(*argv, "from") == 0) {
			inet_prefix addr;

			NEXT_ARG();
			get_prefix(&addr, *argv, req.r.rtm_family);
			if (req.r.rtm_family == AF_UNSPEC)
				req.r.rtm_family = addr.family;
			if (addr.bytelen)
				addattr_l(&req.n, sizeof(req), RTA_SRC, &addr.data, addr.bytelen);
			req.r.rtm_src_len = addr.bitlen;
		} else if (strcmp(*argv, "tos") == 0 ||
			   matches(*argv, "dsfield") == 0) {
			__u32 tos;

			NEXT_ARG();
			if (rtnl_dsfield_a2n(&tos, *argv))
				invarg("\"tos\" value is invalid\n", *argv);
			req.r.rtm_tos = tos;
		} else if (strcmp(*argv, "expires") == 0) {
			__u32 expires;

			NEXT_ARG();
			if (get_u32(&expires, *argv, 0))
				invarg("\"expires\" value is invalid\n", *argv);
			addattr32(&req.n, sizeof(req), RTA_EXPIRES, expires);
		} else if (matches(*argv, "metric") == 0 ||
			   matches(*argv, "priority") == 0 ||
			   strcmp(*argv, "preference") == 0) {
			__u32 metric;

			NEXT_ARG();
			if (get_u32(&metric, *argv, 0))
				invarg("\"metric\" value is invalid\n", *argv);
			addattr32(&req.n, sizeof(req), RTA_PRIORITY, metric);
		} else if (strcmp(*argv, "scope") == 0) {
			__u32 scope = 0;

			NEXT_ARG();
			if (rtnl_rtscope_a2n(&scope, *argv))
				invarg("invalid \"scope\" value\n", *argv);
			req.r.rtm_scope = scope;
			scope_ok = 1;
		} else if (strcmp(*argv, "mtu") == 0) {
			unsigned int mtu;

			NEXT_ARG();
			if (strcmp(*argv, "lock") == 0) {
				mxlock |= (1<<RTAX_MTU);
				NEXT_ARG();
			}
			if (get_unsigned(&mtu, *argv, 0))
				invarg("\"mtu\" value is invalid\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_MTU, mtu);
		} else if (strcmp(*argv, "hoplimit") == 0) {
			unsigned int hoplimit;

			NEXT_ARG();
			if (strcmp(*argv, "lock") == 0) {
				mxlock |= (1<<RTAX_HOPLIMIT);
				NEXT_ARG();
			}
			if (get_unsigned(&hoplimit, *argv, 0) || hoplimit > 255)
				invarg("\"hoplimit\" value is invalid\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_HOPLIMIT, hoplimit);
		} else if (strcmp(*argv, "advmss") == 0) {
			unsigned int mss;

			NEXT_ARG();
			if (strcmp(*argv, "lock") == 0) {
				mxlock |= (1<<RTAX_ADVMSS);
				NEXT_ARG();
			}
			if (get_unsigned(&mss, *argv, 0))
				invarg("\"mss\" value is invalid\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_ADVMSS, mss);
		} else if (matches(*argv, "reordering") == 0) {
			unsigned int reord;

			NEXT_ARG();
			if (strcmp(*argv, "lock") == 0) {
				mxlock |= (1<<RTAX_REORDERING);
				NEXT_ARG();
			}
			if (get_unsigned(&reord, *argv, 0))
				invarg("\"reordering\" value is invalid\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_REORDERING, reord);
		} else if (strcmp(*argv, "rtt") == 0) {
			unsigned int rtt;

			NEXT_ARG();
			if (strcmp(*argv, "lock") == 0) {
				mxlock |= (1<<RTAX_RTT);
				NEXT_ARG();
			}
			if (get_time_rtt(&rtt, *argv, &raw))
				invarg("\"rtt\" value is invalid\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_RTT,
				(raw) ? rtt : rtt * 8);
		} else if (strcmp(*argv, "rto_min") == 0) {
			unsigned int rto_min;

			NEXT_ARG();
			mxlock |= (1<<RTAX_RTO_MIN);
			if (get_time_rtt(&rto_min, *argv, &raw))
				invarg("\"rto_min\" value is invalid\n",
				       *argv);
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_RTO_MIN,
				      rto_min);
		} else if (matches(*argv, "window") == 0) {
			unsigned int win;

			NEXT_ARG();
			if (strcmp(*argv, "lock") == 0) {
				mxlock |= (1<<RTAX_WINDOW);
				NEXT_ARG();
			}
			if (get_unsigned(&win, *argv, 0))
				invarg("\"window\" value is invalid\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_WINDOW, win);
		} else if (matches(*argv, "cwnd") == 0) {
			unsigned int win;

			NEXT_ARG();
			if (strcmp(*argv, "lock") == 0) {
				mxlock |= (1<<RTAX_CWND);
				NEXT_ARG();
			}
			if (get_unsigned(&win, *argv, 0))
				invarg("\"cwnd\" value is invalid\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_CWND, win);
		} else if (matches(*argv, "initcwnd") == 0) {
			unsigned int win;

			NEXT_ARG();
			if (strcmp(*argv, "lock") == 0) {
				mxlock |= (1<<RTAX_INITCWND);
				NEXT_ARG();
			}
			if (get_unsigned(&win, *argv, 0))
				invarg("\"initcwnd\" value is invalid\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf),
				      RTAX_INITCWND, win);
		} else if (matches(*argv, "initrwnd") == 0) {
			unsigned int win;

			NEXT_ARG();
			if (strcmp(*argv, "lock") == 0) {
				mxlock |= (1<<RTAX_INITRWND);
				NEXT_ARG();
			}
			if (get_unsigned(&win, *argv, 0))
				invarg("\"initrwnd\" value is invalid\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf),
				      RTAX_INITRWND, win);
		} else if (matches(*argv, "features") == 0) {
			unsigned int features = 0;

			while (argc > 0) {
				NEXT_ARG();

				if (strcmp(*argv, "ecn") == 0)
					features |= RTAX_FEATURE_ECN;
				else
					invarg("\"features\" value not valid\n", *argv);
				break;
			}

			rta_addattr32(mxrta, sizeof(mxbuf),
				      RTAX_FEATURES, features);
		} else if (matches(*argv, "quickack") == 0) {
			unsigned int quickack;

			NEXT_ARG();
			if (get_unsigned(&quickack, *argv, 0))
				invarg("\"quickack\" value is invalid\n", *argv);
			if (quickack != 1 && quickack != 0)
				invarg("\"quickack\" value should be 0 or 1\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf),
				      RTAX_QUICKACK, quickack);
		} else if (matches(*argv, "congctl") == 0) {
			NEXT_ARG();
			if (strcmp(*argv, "lock") == 0) {
				mxlock |= 1 << RTAX_CC_ALGO;
				NEXT_ARG();
			}
			rta_addattr_l(mxrta, sizeof(mxbuf), RTAX_CC_ALGO, *argv,
				      strlen(*argv));
		} else if (matches(*argv, "rttvar") == 0) {
			unsigned int win;

			NEXT_ARG();
			if (strcmp(*argv, "lock") == 0) {
				mxlock |= (1<<RTAX_RTTVAR);
				NEXT_ARG();
			}
			if (get_time_rtt(&win, *argv, &raw))
				invarg("\"rttvar\" value is invalid\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_RTTVAR,
				(raw) ? win : win * 4);
		} else if (matches(*argv, "ssthresh") == 0) {
			unsigned int win;

			NEXT_ARG();
			if (strcmp(*argv, "lock") == 0) {
				mxlock |= (1<<RTAX_SSTHRESH);
				NEXT_ARG();
			}
			if (get_unsigned(&win, *argv, 0))
				invarg("\"ssthresh\" value is invalid\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_SSTHRESH, win);
		} else if (strcmp(*argv, "onlink") == 0) {
			req.r.rtm_flags |= RTNH_F_ONLINK;
		} else if (strcmp(*argv, "nexthop") == 0) {
			nhs_ok = 1;
			break;
		} else if (!strcmp(*argv, "nhid")) {
			NEXT_ARG();
			if (get_u32(&nhid, *argv, 0))
				invarg("\"id\" value is invalid\n", *argv);
			addattr32(&req.n, sizeof(req), RTA_NH_ID, nhid);
		} else if (matches(*argv, "protocol") == 0) {
			__u32 prot;

			NEXT_ARG();
			if (rtnl_rtprot_a2n(&prot, *argv))
				invarg("\"protocol\" value is invalid\n", *argv);
			req.r.rtm_protocol = prot;
		} else if (matches(*argv, "table") == 0) {
			__u32 tid;

			NEXT_ARG();
			if (rtnl_rttable_a2n(&tid, *argv))
				invarg("\"table\" value is invalid\n", *argv);
			if (tid < 256)
				req.r.rtm_table = tid;
			else {
				req.r.rtm_table = RT_TABLE_UNSPEC;
				addattr32(&req.n, sizeof(req), RTA_TABLE, tid);
			}
			table_ok = 1;
		} else if (strcmp(*argv, "dev") == 0 ||
			   strcmp(*argv, "oif") == 0) {
			NEXT_ARG();
			d = *argv;
		
		} else if (strcmp(*argv, "ttl-propagate") == 0) {
			__u8 ttl_prop;

			NEXT_ARG();
			if (matches(*argv, "enabled") == 0)
				ttl_prop = 1;
			else if (matches(*argv, "disabled") == 0)
				ttl_prop = 0;
			else
				invarg("\"ttl-propagate\" value is invalid\n",
				       *argv);

			addattr8(&req.n, sizeof(req), RTA_TTL_PROPAGATE,
				 ttl_prop);
		} else if (matches(*argv, "fastopen_no_cookie") == 0) {
			unsigned int fastopen_no_cookie;

			NEXT_ARG();
			if (get_unsigned(&fastopen_no_cookie, *argv, 0))
				invarg("\"fastopen_no_cookie\" value is invalid\n", *argv);
			if (fastopen_no_cookie != 1 && fastopen_no_cookie != 0)
				invarg("\"fastopen_no_cookie\" value should be 0 or 1\n", *argv);
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_FASTOPEN_NO_COOKIE, fastopen_no_cookie);
		} else {
			inet_prefix dst;

			if (strcmp(*argv, "to") == 0) {
				NEXT_ARG();
			}

			if (dst_ok)
				duparg2("to", *argv);
			get_prefix(&dst, *argv, req.r.rtm_family);
			if (req.r.rtm_family == AF_UNSPEC)
				req.r.rtm_family = dst.family;
			req.r.rtm_dst_len = dst.bitlen;
			dst_ok = 1;
			if (dst.bytelen)
				addattr_l(&req.n, sizeof(req),
					  RTA_DST, &dst.data, dst.bytelen);
		}
		argc--; argv++;
	}

	if (d) {
		int idx = ll_name_to_index(d);

		if (!idx)
			return nodev(d);
		addattr32(&req.n, sizeof(req), RTA_OIF, idx);
	}

	if (mxrta->rta_len > RTA_LENGTH(0)) {
		if (mxlock)
			rta_addattr32(mxrta, sizeof(mxbuf), RTAX_LOCK, mxlock);
		addattr_l(&req.n, sizeof(req), RTA_METRICS, RTA_DATA(mxrta), RTA_PAYLOAD(mxrta));
	}

	if (req.r.rtm_family == AF_UNSPEC)
		req.r.rtm_family = AF_INET;

	if (!table_ok) {
		if (req.r.rtm_type == RTN_LOCAL ||
		    req.r.rtm_type == RTN_BROADCAST ||
		    req.r.rtm_type == RTN_NAT ||
		    req.r.rtm_type == RTN_ANYCAST)
			req.r.rtm_table = RT_TABLE_LOCAL;
	}
	if (!scope_ok) {
		if (req.r.rtm_family == AF_INET6 ||
		    req.r.rtm_family == AF_MPLS)
			req.r.rtm_scope = RT_SCOPE_UNIVERSE;
		else if (req.r.rtm_type == RTN_LOCAL ||
			 req.r.rtm_type == RTN_NAT)
			req.r.rtm_scope = RT_SCOPE_HOST;
		else if (req.r.rtm_type == RTN_BROADCAST ||
			 req.r.rtm_type == RTN_MULTICAST ||
			 req.r.rtm_type == RTN_ANYCAST)
			req.r.rtm_scope = RT_SCOPE_LINK;
		else if (req.r.rtm_type == RTN_UNICAST ||
			 req.r.rtm_type == RTN_UNSPEC) {
			if (cmd == RTM_DELROUTE)
				req.r.rtm_scope = RT_SCOPE_NOWHERE;
			else if (!gw_ok && !nhs_ok && !nhid)
				req.r.rtm_scope = RT_SCOPE_LINK;
		}
	}

	if (!type_ok && req.r.rtm_family == AF_MPLS)
		req.r.rtm_type = RTN_UNICAST;


	ret = rtnl_talk(&rth, &req.n, NULL);

	if (ret)
		return -2;

	return 0;
}

extern int F_i_V2XHO_Iproute_Link_Modify(int cmd, unsigned int flags, int argc, char **argv)
{
	char *type = NULL;
	struct iplink_req req = {
		.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg)),
		.n.nlmsg_flags = NLM_F_REQUEST | flags,
		.n.nlmsg_type = cmd,
		.i.ifi_family = preferred_family,
	};
	int ret;

	ret = f_i_V2XHO_Iproute_Link_Parse_Args(argc, argv, &req, &type);
	if (ret < 0)
		return ret;

	if (type) {
		struct link_util *lu;
		struct rtattr *linkinfo;
		char *ulinep = strchr(type, '_');
		int iflatype;

		linkinfo = addattr_nest(&req.n, sizeof(req), IFLA_LINKINFO);
		addattr_l(&req.n, sizeof(req), IFLA_INFO_KIND, type,
			 strlen(type));

		lu = f_i_V2XHO_Iproute_Link_GetLinkKind(type);
		if (ulinep && !strcmp(ulinep, "_slave"))
			iflatype = IFLA_INFO_SLAVE_DATA;
		else
			iflatype = IFLA_INFO_DATA;

		argc -= ret;
		argv += ret;

		if (lu && lu->parse_opt && argc) {
			struct rtattr *data;

			data = addattr_nest(&req.n, sizeof(req), iflatype);

			if (lu->parse_opt(lu, argc, argv, &req.n))
				return -1;

			addattr_nest_end(&req.n, data);
		}
		addattr_nest_end(&req.n, linkinfo);
	} else if (flags & NLM_F_CREATE) {
		fprintf(stderr,
			"Not enough information: \"type\" argument is required\n");
		return -1;
	}

	ret = rtnl_talk(&rth, &req.n, NULL);

	if (ret)
		return -2;

	/* remove device from cache; next use can refresh with new data */
	ll_drop_by_index(req.i.ifi_index);

	return 0;
}

struct link_util *f_i_V2XHO_Iproute_Link_GetLinkKind(const char *id)
{
	void *dlh;
	char buf[256];
	struct link_util *l;

	for (l = linkutil_list; l; l = l->next)
		if (strcmp(l->id, id) == 0)
			return l;

	snprintf(buf, sizeof(buf), "%s/link_%s.so", get_ip_lib_dir(), id);
	dlh = dlopen(buf, RTLD_LAZY);
	if (dlh == NULL) {
		/* look in current binary, only open once */
		dlh = BODY;
		if (dlh == NULL) {
			dlh = BODY = dlopen(NULL, RTLD_LAZY);
			if (dlh == NULL)
				return NULL;
		}
	}

	snprintf(buf, sizeof(buf), "%s_link_util", id);
	l = dlsym(dlh, buf);
	if (l == NULL)
		return NULL;

	l->next = linkutil_list;
	linkutil_list = l;
	return l;
}

static int f_i_V2XHO_Iproute_Link_Parse_Args(int argc, char **argv, struct iplink_req *req, char **type)
{
	bool move_netns = false;
	char *name = NULL;
	char *dev = NULL;
	char *link = NULL;
	int ret, len;
	char abuf[32];
	int qlen = -1;
	int mtu = -1;
	int netns = -1;
	int numtxqueues = -1;
	int numrxqueues = -1;
	int link_netnsid = -1;
	int index = 0;
	int group = -1;
	int addr_len = 0;
	int err;

	ret = argc;

	while (argc > 0) {
		if (strcmp(*argv, "up") == 0) {
			req->i.ifi_change |= IFF_UP;
			req->i.ifi_flags |= IFF_UP;
		} else if (strcmp(*argv, "down") == 0) {
			req->i.ifi_change |= IFF_UP;
			req->i.ifi_flags &= ~IFF_UP;
		} else if (strcmp(*argv, "name") == 0) {
			NEXT_ARG();
			if (name)
				duparg("name", *argv);
			if (check_ifname(*argv))
				invarg("\"name\" not a valid ifname", *argv);
			name = *argv;
			if (!dev)
				dev = name;
		} else if (strcmp(*argv, "index") == 0) {
			NEXT_ARG();
			if (index)
				duparg("index", *argv);
			index = atoi(*argv);
			if (index <= 0)
				invarg("Invalid \"index\" value", *argv);
		} else if (matches(*argv, "link") == 0) {
			NEXT_ARG();
			link = *argv;
		} else if (matches(*argv, "address") == 0) {
			NEXT_ARG();
			addr_len = ll_addr_a2n(abuf, sizeof(abuf), *argv);
			if (addr_len < 0)
				return -1;
			addattr_l(&req->n, sizeof(*req),
				  IFLA_ADDRESS, abuf, addr_len);
		} else if (matches(*argv, "broadcast") == 0 ||
			   strcmp(*argv, "brd") == 0) {
			NEXT_ARG();
			len = ll_addr_a2n(abuf, sizeof(abuf), *argv);
			if (len < 0)
				return -1;
			addattr_l(&req->n, sizeof(*req),
				  IFLA_BROADCAST, abuf, len);
		} else if (matches(*argv, "txqueuelen") == 0 ||
			   strcmp(*argv, "qlen") == 0 ||
			   matches(*argv, "txqlen") == 0) {
			NEXT_ARG();
			if (qlen != -1)
				duparg("txqueuelen", *argv);
			if (get_integer(&qlen,  *argv, 0))
				invarg("Invalid \"txqueuelen\" value\n", *argv);
			addattr_l(&req->n, sizeof(*req),
				  IFLA_TXQLEN, &qlen, 4);
		} else if (strcmp(*argv, "mtu") == 0) {
			NEXT_ARG();
			if (mtu != -1)
				duparg("mtu", *argv);
			if (get_integer(&mtu, *argv, 0))
				invarg("Invalid \"mtu\" value\n", *argv);
			addattr_l(&req->n, sizeof(*req), IFLA_MTU, &mtu, 4);
		} else if (strcmp(*argv, "netns") == 0) {
			NEXT_ARG();
			if (netns != -1)
				duparg("netns", *argv);
			netns = netns_get_fd(*argv);
			if (netns >= 0)
				addattr_l(&req->n, sizeof(*req), IFLA_NET_NS_FD,
					  &netns, 4);
			else if (get_integer(&netns, *argv, 0) == 0)
				addattr_l(&req->n, sizeof(*req),
					  IFLA_NET_NS_PID, &netns, 4);
			else
				invarg("Invalid \"netns\" value\n", *argv);
			move_netns = true;
		} else if (matches(*argv, "master") == 0) {
			int ifindex;

			NEXT_ARG();
			ifindex = ll_name_to_index(*argv);
			if (!ifindex)
				invarg("Device does not exist\n", *argv);
			addattr_l(&req->n, sizeof(*req), IFLA_MASTER,
				  &ifindex, 4);
		} else if (matches(*argv, "nomaster") == 0) {
			int ifindex = 0;

			addattr_l(&req->n, sizeof(*req), IFLA_MASTER,
				  &ifindex, 4);
		} else if (matches(*argv, "type") == 0) {
			NEXT_ARG();
			*type = *argv;
			argc--; argv++;
			break;
		} else if (matches(*argv, "alias") == 0) {
			NEXT_ARG();
			len = strlen(*argv);
			if (len >= IFALIASZ)
				invarg("alias too long\n", *argv);
			addattr_l(&req->n, sizeof(*req), IFLA_IFALIAS,
				  *argv, len);
		} else if (strcmp(*argv, "group") == 0) {
			NEXT_ARG();
			if (group != -1)
				duparg("group", *argv);
			if (rtnl_group_a2n(&group, *argv))
				invarg("Invalid \"group\" value\n", *argv);
			addattr32(&req->n, sizeof(*req), IFLA_GROUP, group);
		}else if (matches(*argv, "numtxqueues") == 0) {
			NEXT_ARG();
			if (numtxqueues != -1)
				duparg("numtxqueues", *argv);
			if (get_integer(&numtxqueues, *argv, 0))
				invarg("Invalid \"numtxqueues\" value\n",
				       *argv);
			addattr_l(&req->n, sizeof(*req), IFLA_NUM_TX_QUEUES,
				  &numtxqueues, 4);
		} else if (matches(*argv, "numrxqueues") == 0) {
			NEXT_ARG();
			if (numrxqueues != -1)
				duparg("numrxqueues", *argv);
			if (get_integer(&numrxqueues, *argv, 0))
				invarg("Invalid \"numrxqueues\" value\n",
				       *argv);
			addattr_l(&req->n, sizeof(*req), IFLA_NUM_RX_QUEUES,
				  &numrxqueues, 4);
		} else if (matches(*argv, "link-netnsid") == 0) {
			NEXT_ARG();
			if (link_netnsid != -1)
				duparg("link-netns/link-netnsid", *argv);
			if (get_integer(&link_netnsid, *argv, 0))
				invarg("Invalid \"link-netnsid\" value\n",
				       *argv);
			addattr32(&req->n, sizeof(*req), IFLA_LINK_NETNSID,
				  link_netnsid);
		} else if (strcmp(*argv, "protodown") == 0) {
			unsigned int proto_down;

			NEXT_ARG();
			proto_down = parse_on_off("protodown", *argv, &err);
			if (err)
				return err;
			addattr8(&req->n, sizeof(*req), IFLA_PROTO_DOWN,
				 proto_down);
		} else if (strcmp(*argv, "gso_max_size") == 0) {
			unsigned int max_size;

			NEXT_ARG();
			if (get_unsigned(&max_size, *argv, 0))
				invarg("Invalid \"gso_max_size\" value\n",
				       *argv);
			addattr32(&req->n, sizeof(*req),
				  IFLA_GSO_MAX_SIZE, max_size);
		} else if (strcmp(*argv, "gro_max_size") == 0) {
			unsigned int max_size;

			NEXT_ARG();
			if (get_unsigned(&max_size, *argv, 0))
				invarg("Invalid \"gro_max_size\" value\n",
				       *argv);
			addattr32(&req->n, sizeof(*req),
				  IFLA_GRO_MAX_SIZE, max_size);
		} else if (strcmp(*argv, "gso_ipv4_max_size") == 0) {
			unsigned int max_size;

			NEXT_ARG();
			if (get_unsigned(&max_size, *argv, 0))
				invarg("Invalid \"gso_ipv4_max_size\" value\n",
				       *argv);
			addattr32(&req->n, sizeof(*req),
				  IFLA_GSO_IPV4_MAX_SIZE, max_size);
		}  else if (strcmp(*argv, "gro_ipv4_max_size") == 0) {
			unsigned int max_size;

			NEXT_ARG();
			if (get_unsigned(&max_size, *argv, 0))
				invarg("Invalid \"gro_ipv4_max_size\" value\n",
				       *argv);
			addattr32(&req->n, sizeof(*req),
				  IFLA_GRO_IPV4_MAX_SIZE, max_size);
		} else if (strcmp(*argv, "parentdev") == 0) {
			NEXT_ARG();
			addattr_l(&req->n, sizeof(*req), IFLA_PARENT_DEV_NAME,
				  *argv, strlen(*argv) + 1);
		} else {
			if (strcmp(*argv, "dev") == 0)
				NEXT_ARG();
			if (dev != name)
				duparg2("dev", *argv);
			if (check_altifname(*argv))
				invarg("\"dev\" not a valid ifname", *argv);
			dev = *argv;
		}
		argc--; argv++;
	}

	ret -= argc;

	/* Allow "ip link add dev" and "ip link add name" */
	if (!name)
		name = dev;
	else if (!dev)
		dev = name;
	else if (!strcmp(name, dev))
		name = dev;

	if (dev && addr_len &&
	    !(req->n.nlmsg_flags & NLM_F_CREATE)) {
		int halen = nl_get_ll_addr_len(dev);

		if (halen >= 0 && halen != addr_len) {
			fprintf(stderr,
				"Invalid address length %d - must be %d bytes\n",
				addr_len, halen);
			return -1;
		}
	}

	if (index &&
	    (!(req->n.nlmsg_flags & NLM_F_CREATE) &&
	     !move_netns)) {
		fprintf(stderr,
			"index can be used only when creating devices or when moving device to another netns.\n");
		exit(-1);
	}

	if (group != -1) {
		if (!dev) {
			if (argc) {
				fprintf(stderr,
					"Garbage instead of arguments \"%s ...\". Try \"ip link help\".\n",
					*argv);
				exit(-1);
			}
			if (req->n.nlmsg_flags & NLM_F_CREATE) {
				fprintf(stderr,
					"group cannot be used when creating devices.\n");
				exit(-1);
			}

			*type = NULL;
			return ret;
		}
	}

	if (!(req->n.nlmsg_flags & NLM_F_CREATE)) {
		if (!dev) {
			fprintf(stderr,
				"Not enough information: \"dev\" argument is required.\n");
			exit(-1);
		}

		req->i.ifi_index = ll_name_to_index(dev);
		if (!req->i.ifi_index)
			return nodev(dev);

		/* Not renaming to the same name */
		if (name == dev)
			name = NULL;

		if (index)
			addattr32(&req->n, sizeof(*req), IFLA_NEW_IFINDEX, index);
	} else {
		if (name != dev) {
			fprintf(stderr,
				"both \"name\" and \"dev\" cannot be used when creating devices.\n");
			exit(-1);
		}

		if (link) {
			int ifindex;

			ifindex = ll_name_to_index(link);
			if (!ifindex)
				return nodev(link);
			addattr32(&req->n, sizeof(*req), IFLA_LINK, ifindex);
		}

		req->i.ifi_index = index;
	}

	if (name) {
		addattr_l(&req->n, sizeof(*req),
			  IFLA_IFNAME, name, strlen(name) + 1);
	}

	return ret;
}

static int nl_get_ll_addr_len(const char *ifname)
{
	int len;
	int dev_index = ll_name_to_index(ifname);
	struct iplink_req req = {
		.n = {
			.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg)),
			.nlmsg_type = RTM_GETLINK,
			.nlmsg_flags = NLM_F_REQUEST
		},
		.i = {
			.ifi_family = preferred_family,
			.ifi_index = dev_index,
		}
	};
	struct nlmsghdr *answer;
	struct rtattr *tb[IFLA_MAX+1];

	if (dev_index == 0)
		return -1;

	if (rtnl_talk(&rth, &req.n, &answer) < 0)
		return -1;

	len = answer->nlmsg_len - NLMSG_LENGTH(sizeof(struct ifinfomsg));
	if (len < 0) {
		free(answer);
		return -1;
	}

	parse_rtattr_flags(tb, IFLA_MAX, IFLA_RTA(NLMSG_DATA(answer)),
			   len, NLA_F_NESTED);
	if (!tb[IFLA_ADDRESS]) {
		free(answer);
		return -1;
	}

	len = RTA_PAYLOAD(tb[IFLA_ADDRESS]);
	free(answer);
	return len;
}

int rtnl_rtntype_a2n(int *id, char *arg)
{
	char *end;
	unsigned long res;

	if (strcmp(arg, "local") == 0)
		res = RTN_LOCAL;
	else if (strcmp(arg, "nat") == 0)
		res = RTN_NAT;
	else if (matches(arg, "broadcast") == 0 ||
		 strcmp(arg, "brd") == 0)
		res = RTN_BROADCAST;
	else if (matches(arg, "anycast") == 0)
		res = RTN_ANYCAST;
	else if (matches(arg, "multicast") == 0)
		res = RTN_MULTICAST;
	else if (matches(arg, "prohibit") == 0)
		res = RTN_PROHIBIT;
	else if (matches(arg, "unreachable") == 0)
		res = RTN_UNREACHABLE;
	else if (matches(arg, "blackhole") == 0)
		res = RTN_BLACKHOLE;
	else if (matches(arg, "xresolve") == 0)
		res = RTN_XRESOLVE;
	else if (matches(arg, "unicast") == 0)
		res = RTN_UNICAST;
	else if (strcmp(arg, "throw") == 0)
		res = RTN_THROW;
	else {
		res = strtoul(arg, &end, 0);
		if (!end || end == arg || *end || res > 255)
			return -1;
	}
	*id = res;
	return 0;
}

extern int F_i_V2XHO_Iproute_Rule_Modify(int cmd, int argc, char **argv)
{
	int l3mdev_rule = 0;
	int table_ok = 0;
	__u32 tid = 0;
	struct {
		struct nlmsghdr	n;
		struct fib_rule_hdr	frh;
		char			buf[1024];
	} req = {
		.n.nlmsg_type = cmd,
		.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct fib_rule_hdr)),
		.n.nlmsg_flags = NLM_F_REQUEST,
		.frh.family = preferred_family,
		.frh.action = FR_ACT_UNSPEC,
	};
	int ret;

	if (cmd == RTM_NEWRULE) {
		if (argc == 0) {
			fprintf(stderr,
				"\"ip rule add\" requires arguments.\n");
			return -1;
		}
		req.n.nlmsg_flags |= NLM_F_CREATE|NLM_F_EXCL;
		req.frh.action = FR_ACT_TO_TBL;
	}

	if (cmd == RTM_DELRULE && argc == 0) {
		fprintf(stderr, "\"ip rule del\" requires arguments.\n");
		return -1;
	}

	while (argc > 0) {
		if (strcmp(*argv, "not") == 0) {
			req.frh.flags |= FIB_RULE_INVERT;
		} else if (strcmp(*argv, "from") == 0) {
			inet_prefix dst;

			NEXT_ARG();
			get_prefix(&dst, *argv, req.frh.family);
			req.frh.src_len = dst.bitlen;
			addattr_l(&req.n, sizeof(req), FRA_SRC,
				  &dst.data, dst.bytelen);
		} else if (strcmp(*argv, "to") == 0) {
			inet_prefix dst;

			NEXT_ARG();
			get_prefix(&dst, *argv, req.frh.family);
			req.frh.dst_len = dst.bitlen;
			addattr_l(&req.n, sizeof(req), FRA_DST,
				  &dst.data, dst.bytelen);
		} else if (matches(*argv, "preference") == 0 ||
			   matches(*argv, "order") == 0 ||
			   matches(*argv, "priority") == 0) {
			__u32 pref;

			NEXT_ARG();
			if (get_u32(&pref, *argv, 0))
				invarg("preference value is invalid\n", *argv);
			addattr32(&req.n, sizeof(req), FRA_PRIORITY, pref);
		} else if (strcmp(*argv, "tos") == 0 ||
			   matches(*argv, "dsfield") == 0) {
			__u32 tos;

			NEXT_ARG();
			if (rtnl_dsfield_a2n(&tos, *argv))
				invarg("TOS value is invalid\n", *argv);
			req.frh.tos = tos;
		} else if (strcmp(*argv, "fwmark") == 0) {
			char *slash;
			__u32 fwmark, fwmask;

			NEXT_ARG();

			slash = strchr(*argv, '/');
			if (slash != NULL)
				*slash = '\0';
			if (get_u32(&fwmark, *argv, 0))
				invarg("fwmark value is invalid\n", *argv);
			addattr32(&req.n, sizeof(req), FRA_FWMARK, fwmark);
			if (slash) {
				if (get_u32(&fwmask, slash+1, 0))
					invarg("fwmask value is invalid\n",
					       slash+1);
				addattr32(&req.n, sizeof(req),
					  FRA_FWMASK, fwmask);
			}
		} else if (matches(*argv, "protocol") == 0) {
			__u32 proto;

			NEXT_ARG();
			if (rtnl_rtprot_a2n(&proto, *argv))
				invarg("\"protocol\" value is invalid\n", *argv);
			addattr8(&req.n, sizeof(req), FRA_PROTOCOL, proto);
		} else if (matches(*argv, "tun_id") == 0) {
			__u64 tun_id;

			NEXT_ARG();
			if (get_be64(&tun_id, *argv, 0))
				invarg("\"tun_id\" value is invalid\n", *argv);
			addattr64(&req.n, sizeof(req), FRA_TUN_ID, tun_id);
		} else if (matches(*argv, "table") == 0 ||
			   strcmp(*argv, "lookup") == 0) {
			NEXT_ARG();
			if (rtnl_rttable_a2n(&tid, *argv))
				invarg("invalid table ID\n", *argv);
			if (tid < 256)
				req.frh.table = tid;
			else {
				req.frh.table = RT_TABLE_UNSPEC;
				addattr32(&req.n, sizeof(req), FRA_TABLE, tid);
			}
			table_ok = 1;
		} else if (matches(*argv, "suppress_prefixlength") == 0 ||
			   strcmp(*argv, "sup_pl") == 0) {
			int pl;

			NEXT_ARG();
			if (get_s32(&pl, *argv, 0) || pl < 0)
				invarg("suppress_prefixlength value is invalid\n",
				       *argv);
			addattr32(&req.n, sizeof(req),
				  FRA_SUPPRESS_PREFIXLEN, pl);
		} else if (matches(*argv, "suppress_ifgroup") == 0 ||
			   strcmp(*argv, "sup_group") == 0) {
			NEXT_ARG();
			int group;

			if (rtnl_group_a2n(&group, *argv))
				invarg("Invalid \"suppress_ifgroup\" value\n",
				       *argv);
			addattr32(&req.n, sizeof(req),
				  FRA_SUPPRESS_IFGROUP, group);
		} else if (strcmp(*argv, "dev") == 0 ||
			   strcmp(*argv, "iif") == 0) {
			NEXT_ARG();
			if (check_ifname(*argv))
				invarg("\"iif\"/\"dev\" not a valid ifname", *argv);
			addattr_l(&req.n, sizeof(req), FRA_IFNAME,
				  *argv, strlen(*argv)+1);
		} else if (strcmp(*argv, "oif") == 0) {
			NEXT_ARG();
			if (check_ifname(*argv))
				invarg("\"oif\" not a valid ifname", *argv);
			addattr_l(&req.n, sizeof(req), FRA_OIFNAME,
				  *argv, strlen(*argv)+1);
		} else if (strcmp(*argv, "l3mdev") == 0) {
			addattr8(&req.n, sizeof(req), FRA_L3MDEV, 1);
			table_ok = 1;
			l3mdev_rule = 1;
		} else if (strcmp(*argv, "uidrange") == 0) {
			struct fib_rule_uid_range r;

			NEXT_ARG();
			if (sscanf(*argv, "%u-%u", &r.start, &r.end) != 2)
				invarg("invalid UID range\n", *argv);
			addattr_l(&req.n, sizeof(req), FRA_UID_RANGE, &r,
				  sizeof(r));
		} else if (strcmp(*argv, "nat") == 0 ||
			   matches(*argv, "map-to") == 0) {
			NEXT_ARG();
			fprintf(stderr, "Warning: route NAT is deprecated\n");
			addattr32(&req.n, sizeof(req), RTA_GATEWAY,
				  get_addr32(*argv));
			req.frh.action = RTN_NAT;
		} else {
			int type;

			if (strcmp(*argv, "type") == 0)
				NEXT_ARG();

			if (matches(*argv, "goto") == 0) {
				__u32 target;

				type = FR_ACT_GOTO;
				NEXT_ARG();
				if (get_u32(&target, *argv, 0))
					invarg("invalid target\n", *argv);
				addattr32(&req.n, sizeof(req),
					  FRA_GOTO, target);
			} else if (matches(*argv, "nop") == 0)
				type = FR_ACT_NOP;
			else if (rtnl_rtntype_a2n(&type, *argv))
				invarg("Failed to parse rule type", *argv);
			req.frh.action = type;
			table_ok = 1;
		}
		argc--;
		argv++;
	}

	if (l3mdev_rule && tid != 0) {
		fprintf(stderr,
			"table can not be specified for l3mdev rules\n");
		return -EINVAL;
	}

	if (req.frh.family == AF_UNSPEC)
		req.frh.family = AF_INET;

	if (!table_ok && cmd == RTM_NEWRULE)
		req.frh.table = RT_TABLE_MAIN;

	ret = rtnl_talk(&rth, &req.n, NULL);

	if (ret)
		return -2;

	return 0;
}


static const char *tnl_defname(const struct ip_tunnel_parm *p)
{
	switch (p->iph.protocol) {
	case IPPROTO_IPIP:
		if (p->i_flags & VTI_ISVTI)
			return "ip_vti0";
		else
			return "tunl0";
	case IPPROTO_GRE:
		return "gre0";
	case IPPROTO_IPV6:
		return "sit0";
	}
	return NULL;
}

static void set_tunnel_proto(struct ip_tunnel_parm *p, int proto)
{
	if (p->iph.protocol && p->iph.protocol != proto) {
		fprintf(stderr,
			"You managed to ask for more than one tunnel mode.\n");
		exit(-1);
	}
	p->iph.protocol = proto;
}

static int tnl_get_ioctl(const char *basedev, void *p)
{
	struct ifreq ifr;
	int fd;
	int err;

	int copy_len = strlen(basedev);
	if(copy_len > IFNAMSIZ)
	{
		copy_len = IFNAMSIZ;
	}
	for(int i = 0; i < copy_len; i++)
	{
		ifr.ifr_name[i] = basedev[i];
	}
	
	ifr.ifr_ifru.ifru_data = (void *)p;

	fd = socket(preferred_family, SOCK_DGRAM, 0);
	if (fd < 0) {
		fprintf(stderr, "create socket failed: %s\n", strerror(errno));
		return -1;
	}

	err = ioctl(fd, SIOCGETTUNNEL, &ifr);
	if (err)
		fprintf(stderr, "get tunnel \"%s\" failed: %s\n", basedev,
			strerror(errno));

	close(fd);
	return err;
}

int tnl_add_ioctl(int cmd, const char *basedev, const char *name, void *p)
{
	struct ifreq ifr;
	int fd;
	int err;
	memset(ifr.ifr_name, '\0', IFNAMSIZ);
	if (cmd == SIOCCHGTUNNEL && name[0])
	{
		int copy_len = strlen(name);
		if(copy_len > IFNAMSIZ)
		{
			copy_len = IFNAMSIZ;
		}
		for(int i = 0; i < copy_len; i++)
		{
			ifr.ifr_name[i] = name[i];
		}
	}else{
		int copy_len = strlen(basedev);
		if(copy_len > IFNAMSIZ)
		{
			copy_len = IFNAMSIZ;
		}
		for(int i = 0; i < copy_len; i++)
		{
			ifr.ifr_name[i] = basedev[i];
		}
	}
	ifr.ifr_ifru.ifru_data = p;

	fd = socket(preferred_family, SOCK_DGRAM, 0);
	if (fd < 0) {
		fprintf(stderr, "create socket failed: %s\n", strerror(errno));
		return -1;
	}
	err = ioctl(fd, cmd, &ifr);
	if (err)
		fprintf(stderr, "add tunnel \"%s\" failed: %s\n", ifr.ifr_name,
			strerror(errno));
	close(fd);
	return err;
}

__be32 tnl_parse_key(const char *name, const char *key)
{
	unsigned int uval;

	if (strchr(key, '.'))
		return get_addr32(key);

	if (get_unsigned(&uval, key, 0) < 0) {
		fprintf(stderr,
			"invalid value for \"%s\": \"%s\"; it should be an unsigned integer\n",
			name, key);
		exit(-1);
	}
	return htonl(uval);
}

extern int F_i_V2XHO_Iproute_Tunnel_Modify(int cmd, int argc, char **argv)
{
	struct ip_tunnel_parm p;
	const char *basedev;
	switch (preferred_family) {
		case AF_UNSPEC:
			preferred_family = AF_INET;
			break;
		case AF_INET:
			break;
	}

	if (F_i_V2XHO_Iproute_Tunnel_Parse_Args(argc, argv, cmd, &p) < 0)
		return -1;

	if (p.iph.ttl && p.iph.frag_off == 0) {
		fprintf(stderr, "ttl != 0 and nopmtudisc are incompatible\n");
		return -1;
	}

	basedev = tnl_defname(&p);
	if (!basedev) {
		fprintf(stderr,
			"cannot determine tunnel mode (ipip, gre, vti or sit)\n");
		return -1;
	}

	return tnl_add_ioctl(cmd, basedev, p.name, &p);
}


static int F_i_V2XHO_Iproute_Tunnel_Parse_Args(int argc, char **argv, int cmd, struct ip_tunnel_parm *p)
{
	int count = 0;
	const char *medium = NULL;
	int isatap = 0;

	memset(p, 0, sizeof(*p));
	p->iph.version = 4;
	p->iph.ihl = 5;
#ifndef IP_DF
#define IP_DF		0x4000		/* Flag: "Don't Fragment"	*/
#endif
	p->iph.frag_off = htons(IP_DF);

	while (argc > 0) {
		if (strcmp(*argv, "mode") == 0) {
			NEXT_ARG();
			if (strcmp(*argv, "ipip") == 0 ||
			    strcmp(*argv, "ip/ip") == 0) {
				set_tunnel_proto(p, IPPROTO_IPIP);
			} else if (strcmp(*argv, "gre") == 0 ||
				   strcmp(*argv, "gre/ip") == 0) {
				set_tunnel_proto(p, IPPROTO_GRE);
			} else if (strcmp(*argv, "sit") == 0 ||
				   strcmp(*argv, "ipv6/ip") == 0) {
				set_tunnel_proto(p, IPPROTO_IPV6);
			} else if (strcmp(*argv, "isatap") == 0) {
				set_tunnel_proto(p, IPPROTO_IPV6);
				isatap++;
			} else if (strcmp(*argv, "vti") == 0) {
				set_tunnel_proto(p, IPPROTO_IPIP);
				p->i_flags |= VTI_ISVTI;
			} else {
				fprintf(stderr,
					"Unknown tunnel mode \"%s\"\n", *argv);
				exit(-1);
			}
		} else if (strcmp(*argv, "key") == 0) {
			NEXT_ARG();
			p->i_flags |= GRE_KEY;
			p->o_flags |= GRE_KEY;
			p->i_key = p->o_key = tnl_parse_key("key", *argv);
		} else if (strcmp(*argv, "ikey") == 0) {
			NEXT_ARG();
			p->i_flags |= GRE_KEY;
			p->i_key = tnl_parse_key("ikey", *argv);
		} else if (strcmp(*argv, "okey") == 0) {
			NEXT_ARG();
			p->o_flags |= GRE_KEY;
			p->o_key = tnl_parse_key("okey", *argv);
		} else if (strcmp(*argv, "seq") == 0) {
			p->i_flags |= GRE_SEQ;
			p->o_flags |= GRE_SEQ;
		} else if (strcmp(*argv, "iseq") == 0) {
			p->i_flags |= GRE_SEQ;
		} else if (strcmp(*argv, "oseq") == 0) {
			p->o_flags |= GRE_SEQ;
		} else if (strcmp(*argv, "csum") == 0) {
			p->i_flags |= GRE_CSUM;
			p->o_flags |= GRE_CSUM;
		} else if (strcmp(*argv, "icsum") == 0) {
			p->i_flags |= GRE_CSUM;
		} else if (strcmp(*argv, "ocsum") == 0) {
			p->o_flags |= GRE_CSUM;
		} else if (strcmp(*argv, "nopmtudisc") == 0) {
			p->iph.frag_off = 0;
		} else if (strcmp(*argv, "pmtudisc") == 0) {
			p->iph.frag_off = htons(IP_DF);
		} else if (strcmp(*argv, "remote") == 0) {
			NEXT_ARG();
			p->iph.daddr = get_addr32(*argv);
		} else if (strcmp(*argv, "local") == 0) {
			NEXT_ARG();
			p->iph.saddr = get_addr32(*argv);
		} else if (strcmp(*argv, "dev") == 0) {
			NEXT_ARG();
			medium = *argv;
		} else if (strcmp(*argv, "ttl") == 0 ||
			   strcmp(*argv, "hoplimit") == 0 ||
			   strcmp(*argv, "hlim") == 0) {
			__u8 uval;

			NEXT_ARG();
			if (strcmp(*argv, "inherit") != 0) {
				if (get_u8(&uval, *argv, 0))
					invarg("invalid TTL\n", *argv);
				p->iph.ttl = uval;
			}
		} else if (strcmp(*argv, "tos") == 0 ||
			   strcmp(*argv, "tclass") == 0 ||
			   matches(*argv, "dsfield") == 0) {
			char *dsfield;
			__u32 uval;

			NEXT_ARG();
			dsfield = *argv;
			strsep(&dsfield, "/");
			if (strcmp(*argv, "inherit") != 0) {
				dsfield = *argv;
				p->iph.tos = 0;
			} else
				p->iph.tos = 1;
			if (dsfield) {
				if (rtnl_dsfield_a2n(&uval, dsfield))
					invarg("bad TOS value", *argv);
				p->iph.tos |= uval;
			}
		} else {
			if (strcmp(*argv, "name") == 0)
				NEXT_ARG();

			if (p->name[0])
				duparg2("name", *argv);
			if (get_ifname(p->name, *argv))
				invarg("\"name\" not a valid ifname", *argv);
			if (cmd == SIOCCHGTUNNEL && count == 0) {
				union {
					struct ip_tunnel_parm ip_tnl;
					//struct ip6_tnl_parm2 ip6_tnl;
				} old_p = {};

				if (tnl_get_ioctl(*argv, &old_p))
					return -1;

				if (old_p.ip_tnl.iph.version != 4 ||
				    old_p.ip_tnl.iph.ihl != 5)
					invarg("\"name\" is not an ip tunnel",
					       *argv);

				*p = old_p.ip_tnl;
			}
		}
		count++;
		argc--; argv++;
	}


	if (p->iph.protocol == 0) {
		if (memcmp(p->name, "gre", 3) == 0)
			p->iph.protocol = IPPROTO_GRE;
		else if (memcmp(p->name, "ipip", 4) == 0)
			p->iph.protocol = IPPROTO_IPIP;
		else if (memcmp(p->name, "sit", 3) == 0)
			p->iph.protocol = IPPROTO_IPV6;
		else if (memcmp(p->name, "isatap", 6) == 0) {
			p->iph.protocol = IPPROTO_IPV6;
			isatap++;
		} else if (memcmp(p->name, "vti", 3) == 0) {
			p->iph.protocol = IPPROTO_IPIP;
			p->i_flags |= VTI_ISVTI;
		}
	}

	if ((p->i_flags & GRE_KEY) || (p->o_flags & GRE_KEY)) {
		if (!(p->i_flags & VTI_ISVTI) &&
		    (p->iph.protocol != IPPROTO_GRE)) {
			fprintf(stderr, "Keys are not allowed with ipip and sit tunnels\n");
			return -1;
		}
	}

	if (medium) {
		p->link = ll_name_to_index(medium);
		if (!p->link)
			return nodev(medium);
	}

	if (p->i_key == 0 && IN_MULTICAST(ntohl(p->iph.daddr))) {
		p->i_key = p->iph.daddr;
		p->i_flags |= GRE_KEY;
	}
	if (p->o_key == 0 && IN_MULTICAST(ntohl(p->iph.daddr))) {
		p->o_key = p->iph.daddr;
		p->o_flags |= GRE_KEY;
	}
	if (IN_MULTICAST(ntohl(p->iph.daddr)) && !p->iph.saddr) {
		fprintf(stderr, "A broadcast tunnel requires a source address\n");
		return -1;
	}
	if (isatap)
		p->i_flags |= SIT_ISATAP;

	return 0;
}

