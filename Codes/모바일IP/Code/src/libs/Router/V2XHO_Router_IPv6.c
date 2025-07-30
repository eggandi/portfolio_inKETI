#include <V2XHO_Router.h>

struct V2XHO_IProuter_Route_List_Info_IPv6_t g_route_list_info[MAX_ROUTE_LIST_NUM];
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

#define V2XHO_IProuter_Route_Save_Path_IPv6 "./route_restore_file/"
#define V2XHO_Router_IPv6_Adrress_Size_String 256
/**
 * @brief IPv6 라우터 테이블 목록 저장
 * 
 * @param dev_name 장치별로 route_list에 라우팅 테이블 저장할 때
 * @param route_list 라우팅 테이블 값 입력받을 포인터
 * @param list_num 리스트에 저장된 갯수
 * @return int 
 */
extern int F_i_V2XHO_IProuter_Route_Get_List_IPv6(char *dev_name, struct V2XHO_IProuter_Route_List_Info_IPv6_t *route_list, int *list_num)
{
	int family = preferred_family;
	rtnl_filter_t filter_fn;
	int idx;

	filter_fn = f_i_V2XHO_IProuter_Route_Save_Ipv6;
	f_v_V2XHO_IProuter_Route_Reset_Filter(0);

	filter.tb = RT_TABLE_MAIN;

	if (family == AF_UNSPEC && filter.tb)
		family = AF_INET6;
		filter.af = RTNL_FAMILY_IP6MR;
	if(dev_name)
	{
		idx = ll_name_to_index(dev_name);
		if (!idx)
			return nodev(dev_name);
		filter.oif = idx;
	}
	if (rtnl_routedump_req(&rth, family, f_v_V2XHO_IProuter_Route_Reset_Filter) < 0) {
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
		for(int i = 0; i<MAX_ROUTE_LIST_NUM; i++)
		{
			if(g_route_list_info[i].dest_addr.s6_addr != 0 || g_route_list_info[i].gateway_addr.s6_addr != 0)
			{
				route_list[i] = g_route_list_info[i];
			}
			g_route_list_info[i].dest_addr.s6_addr = 0;
			g_route_list_info[i].dest_prefix = 0;
			g_route_list_info[i].gateway_addr.s6_addr = 0;
			g_route_list_info[i].metric = 0;
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

static void f_v_V2XHO_IProuter_Route_Reset_Filter(int ifindex)
{
	memset(&filter, 0, sizeof(filter));
	filter.mdst.bitlen = -1;
	filter.msrc.bitlen = -1;
	filter.oif = ifindex;
	if (filter.oif > 0)
		filter.oifmask = -1;
}


static int f_v_V2XHO_IProuter_Route_Dump_Filter(struct nlmsghdr *nlh, int reqlen)
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

int f_i_V2XHO_IProuter_Route_Save_Ipv6(struct nlmsghdr *n, void *arg)
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
		if (tb[RTA_DST]) 
		{
			family = get_real_family(r->rtm_type, r->rtm_family);
			color = ifa_family_color(family);

			if (r->rtm_dst_len != host_len) {
				snprintf(b1, sizeof(b1), "%s", rt_addr_n2a_rta(family, tb[RTA_DST]));
					
			} else {
				const char *hostname = format_host_rta_r(family, tb[RTA_DST], b2, sizeof(b2));
				if (hostname)
					strncpy(b1, hostname, sizeof(b1) - 1);
			}
		} else if (r->rtm_dst_len) {
			snprintf(b1, sizeof(b1), "0/%d ", r->rtm_dst_len);
		} else {
			strncpy(b1, "default", sizeof( ));
		}
		if (tb[RTA_DST])
		{
			if(tb[RTA_GATEWAY])
			{
				inet_pton(AF_INET6, inet_addr(b1), (void*)&g_route_list_info[g_list_num].dest_addr.s6_addr);
				g_route_list_info[g_list_num].dest_prefix = r->rtm_dst_len;
				inet_pton(AF_INET6, inet_addr(format_host_rta(r->rtm_family, tb[RTA_GATEWAY])), (void*)&g_route_list_info[g_list_num].gateway_addr.s6_addr);
				if (tb[RTA_PRIORITY] && filter.metricmask != -1)
				{
					g_route_list_info[g_list_num].metric = rta_getattr_u32(tb[RTA_PRIORITY]);
				}
				g_list_num = g_list_num + 1;
			}
			if (tb[RTA_OIF])
			{
				const char *ifname = ll_index_to_name(rta_getattr_u32(tb[RTA_OIF]));
				g_route_list_info[g_list_num].dev_name_len = strlen(ifname);
				memcpy(g_route_list_info[g_list_num].dev_name, ifname, g_route_list_info[g_list_num].dev_name_len);
			}
		}
		
	}
	return 0;
}
/**
 * @brief 
 * 
 * @param dst_addr 
 * @param prefix 
 * @param gateway 
 * @param local_gateway 
 * @param interface 
 * @param metric 
 * @param non_forecd 
 * @return int 
 */
int F_i_V2XHO_IProuter_Route_Add_IPv6(char *dst_addr, int *prefix, char *gateway, bool local_gateway, char *interface, char* interface_addr, int *metric, bool non_forecd)
{
	
	int input_arg_list_num = 0;
	char *input_arg_list[64];
	if(!non_forecd)
	{
		F_i_V2XHO_IProuter_Route_Delete_IPv6(address, prefix);
	}
	
	if(*prefix >= 0)
	{
		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * V2XHO_Router_IPv6_Adrress_Size_String);
		if(*prefix == 0)
		{
			sprintf(input_arg_list[input_arg_list_num], "%s/%d", dst_addr, *prefix);
		}else{
			sprintf(input_arg_list[input_arg_list_num], "%s/%02d", dst_addr, *prefix);
		}
		input_arg_list_num++;
	}
	if(gateway)
	{
		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * 3);
		sprintf(input_arg_list[input_arg_list_num], "%s", "via");
		input_arg_list_num++;

		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * V2XHO_Router_IPv6_Adrress_Size_String);
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
	if(interface_addr)
	{
		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * 3);
		sprintf(input_arg_list[input_arg_list_num], "%s", "src");
		input_arg_list_num++;

		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * V2XHO_Router_IPv6_Adrress_Size_String);
		sprintf(input_arg_list[input_arg_list_num], "%s", interface_addr);
		input_arg_list_num++;
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
	
	return f_i_IProuter_Route_Modify_IPv6(RTM_NEWROUTE, NLM_F_CREATE|NLM_F_EXCL, input_arg_list_num, input_arg_list);
}

/**
 * @brief 
 * 
 * @param interface 
 * @param address 
 * @param prefix 
 * @return int 
 */
int F_i_V2XHO_IProuter_Route_Delete_IPv6(char *address, int *prefix)
{
	int input_arg_list_num = 0;
	char *input_arg_list[64];
	
	char *dst_addr = address;
	if(*prefix >= 0)
	{
		input_arg_list[input_arg_list_num] = malloc(sizeof(char) * V2XHO_Router_IPv6_Adrress_Size_String);
		if(*prefix == 0)
		{
			sprintf(input_arg_list[input_arg_list_num], "%s/%d", dst_addr, *prefix);
		}else
		{	
			sprintf(input_arg_list[input_arg_list_num], "%s/%02d", dst_addr, *prefix);
		}
		input_arg_list_num++;
	}
	
	return f_i_Router_IProute_Modify(RTM_DELROUTE, 0, input_arg_list_num, input_arg_list);
}

/**
 * @brief 
 * 
 * @param cmd 
 * @param flags 
 * @param argc 
 * @param argv 
 * @return int 
 */
static int f_i_IProuter_Route_Modify_IPv6(int cmd, unsigned int flags, int argc, char **argv)
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
				addattr_l(&req.n, sizeof(req), RTA_GATEWAY, &addr.data, addr.bytelen);
			else
				addattr_l(&req.n, sizeof(req), RTA_VIA, &addr.family, addr.bytelen+2);
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
		req.r.rtm_family = AF_INET6;

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

exterh int F_i_V2XHO_IProuter_Address_Set_IPv6(char *interface, char *address, int *prefix)
{
	
	if(!address || !interface)
	{
		return -2;
	}
	if(*prefix > 128 || !prefix)
	{
		return *prefix * -1;
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
		char *router_addr_prefix = malloc(sizeof(char) * V2XHO_Router_IPv6_Adrress_Size_String);
		if(*prefix == 0)
		{
			sprintf(router_addr_prefix, "%s/%d", router_addr, 128);
		}else
		{	
			sprintf(router_addr_prefix, "%s/%03d", router_addr, *prefix);
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


int F_i_V2XHO_Router_Address_Del_IPv6(char* interface, char* address, int *prefix)
{
	if(!address && !interface)
	{
		return -2;
	}
	if(*prefix > 128 || !prefix)
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
		char *router_addr_prefix = malloc(sizeof(char) * V2XHO_Router_IPv6_Adrress_Size_String);
		if(*prefix == 0)
		{
			sprintf(router_addr_prefix, "%s/%d", router_addr, 128);
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