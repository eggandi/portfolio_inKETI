#include "v2x_handover_main.h"

#define V2XHO_ERROR_CODE_FILE_CODE V2XHO_ERROR_CODE_FILE_CODE_WAVE_IPV6
#define V2XHO_IPv4_COMPATIBLE_PAD(A,B,C,D) {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, (A),(B),(C),(D)} 

extern int F_i_V2XHO_WAVE_IPv6_Setup()
{
    int ret;
    char cmd[128] = {0,};

    F_v_V2XHO_Utils_Eth_Device_Flush(G_v2xho_config.v2xho_interface_name);
    sprintf(cmd, "ip link set dev %s down",  G_v2xho_config.v2xho_interface_name);
    lsystem(cmd, ret);
    sprintf(cmd, "ip link set dev %s address %s",  G_v2xho_config.v2xho_interface_name, G_v2xho_config.v2xho_interface_MAC_S);
    lsystem(cmd, ret);
    struct WalTxProfile ipv6_tx_profile;
    ipv6_tx_profile.chan_num = G_v2xho_config.u.dsrc.chan_0; ///< 채널번호
    ipv6_tx_profile.power = G_v2xho_config.u.dsrc.tx_power_0; ///< 송신파워
    ipv6_tx_profile.datarate = G_v2xho_config.u.dsrc.datarate_0; ///< 송신 데이터레이트
    ipv6_tx_profile.priority = kWalPriority_Max; ///< 송신 우선순위
    ret = WAL_RegisterTxProfile(0, &ipv6_tx_profile);
    uint8_t ipv4_compatible_addr[V2XHO_IPV6_ALEN_BIN] = {0x00,};
    struct in_addr addr_bin;
    inet_pton(AF_INET, G_v2xho_config.v2xho_interface_IP, &addr_bin);
    memcpy(ipv4_compatible_addr + (V2XHO_IPV6_ALEN_BIN - V2XHO_IPV4_ALEN_BIN), &addr_bin.s_addr, V2XHO_IPV4_ALEN_BIN);
    ret = WAL_SetIPv6Address(0, ipv4_compatible_addr, 96);
    if(ret < 0)
    {
        printf("[%d][V2X][Error][%s] return valut:%d\n", __LINE__, __func__, ret);
        lreturn(V2XHO_WAVE_IPV6_NOT_SETTING_ADDRESS);
    }

    ret = WAL_SetAutoLinkLocalIPv6Address(0);
    if(ret < 0)
    {
        printf("[%d][V2X][Error][%s] return valut:%d\n", __LINE__, __func__, ret);
        lreturn(V2XHO_WAVE_IPV6_NOT_SETTING_AUTO_LINK);
    }
    sprintf(cmd, "ip link set dev %s up",  G_v2xho_config.v2xho_interface_name);
    system(cmd);memset(cmd, 0x00, 128);
    return 0;
} 

