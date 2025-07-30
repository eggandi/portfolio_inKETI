#include "enet.h"


#include "fsl_debug_console.h"
#include "fsl_snvs_hp.h"
#include "mflash_drv.h"
#include "flash_info.h"

extern struct Eth_Info_t G_eth_info;
extern struct Eth_Addr_Info_t *G_addr_info;

#define _DEBUG_LOG PRINTF("[DEBUG][%s][%d]\r\n", __func__, __LINE__);

#define KETI_IPV4_ADDR_PAD(x)  x[0], x[1], x[2], x[3]

static u8_t Keti_Raw_Receiver(void *arg, struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *addr);
static void Keti_Eth_UDP_Receiver(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
static void Keti_Eth_UDP_Receiver_1G(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
static Keti_Err_e Keit_Eth_UDP_Sender(struct udp_pcb *pcb, struct raw_pcb *raw_pcb, u8_t *data, u16_t data_len);
static struct Keti_UDP_Header_t Keti_UDP_Header_Parser(char* data);
static int Keti_Common_Hex2Dec(char data);

char test_dump[23] = {  0x44,
						0x01,
						0x00,0x00,0x00,0x17,
						0x32,0x33,0x34,0x45,0x46,0x47,0x48,0x41,
						0x42,0x43,0x44,0x35,0x36,0x37,0x38,0x44,
						0xAA};

extern void Keti_Do_Job()
{
    err_t ret_err;
    struct Eth_Addr_Info_t *info = (struct Eth_Addr_Info_t*)(G_eth_info.eth_100M);
    if(G_eth_info.eth_100M->rs == Fireware_Ethernet_Device_No_Initial)
    {
        G_eth_info.eth_100M->udp_pcb = (struct udp_pcb *)udp_new();
        G_eth_info.eth_100M->raw_pcb = (struct raw_pcb *)raw_new(IP_PROTO_UDP);
        memcpy(&G_eth_info.eth_100M->udp_pcb->local_ip, &G_eth_info.eth_100M->src_addr, sizeof(ip4_addr_t));
        memcpy(&G_eth_info.eth_100M->udp_pcb->remote_ip, &G_eth_info.eth_100M->dst_addr, sizeof(ip4_addr_t));
        G_eth_info.eth_100M->udp_pcb->local_port = 50000;
        G_eth_info.eth_100M->udp_pcb->remote_port = 50000;
        ret_err = udp_bind(G_eth_info.eth_100M->udp_pcb, &G_eth_info.eth_100M->src_addr, G_eth_info.eth_100M->udp_pcb->local_port);
        udp_recv(G_eth_info.eth_100M->udp_pcb, Keti_Eth_UDP_Receiver, G_eth_info.eth_100M);
        G_eth_info.eth_100M->udp_pcb->netif_idx = netif_get_index(&G_eth_info.eth_100M->netif);
    	G_eth_info.eth_100M->rs = Fireware_Ethernet_Device_Initial;
    }
    if(0)//G_eth_info.eth_1G->rs == Fireware_Ethernet_Device_No_Initial)
    {
        G_eth_info.eth_1G->udp_pcb = (struct udp_pcb *)udp_new();
        G_eth_info.eth_1G->raw_pcb = (struct raw_pcb *)raw_new(IP_PROTO_UDP);
        memcpy(&G_eth_info.eth_1G->udp_pcb->local_ip, &G_eth_info.eth_1G->src_addr, sizeof(ip4_addr_t));
        memcpy(&G_eth_info.eth_1G->udp_pcb->remote_ip, &G_eth_info.eth_1G->dst_addr, sizeof(ip4_addr_t));
        G_eth_info.eth_1G->udp_pcb->local_port = 50000;
        G_eth_info.eth_1G->udp_pcb->remote_port = 50000;
        ret_err = udp_bind(G_eth_info.eth_1G->udp_pcb, &G_eth_info.eth_1G->src_addr, G_eth_info.eth_1G->udp_pcb->local_port);
        ret_err = udp_connect(G_eth_info.eth_1G->udp_pcb, &G_eth_info.eth_1G->src_addr, G_eth_info.eth_1G->udp_pcb->local_port);
        udp_recv(G_eth_info.eth_1G->udp_pcb, Keti_Eth_UDP_Receiver_1G, G_eth_info.eth_1G);
        G_eth_info.eth_1G->udp_pcb->netif_idx = netif_get_index(&G_eth_info.eth_1G->netif);
    	G_eth_info.eth_1G->rs = Fireware_Ethernet_Device_Initial;
    }

    info->send_buf = test_dump;
    if (Enet_GetLinkUp_100M() == 1)
	{
    	if(info->rs == Fireware_Waiting_Download_Divied_Data_From_ECU)
    	{

    	}else{
        	ret_err = Keit_Eth_UDP_Sender(G_eth_info.eth_100M->udp_pcb, G_eth_info.eth_100M->raw_pcb, info->send_buf, 24);
        	info->rs = Fireware_Download_Task_Start;
    	}
	}
}
struct image_version
{
    uint8_t iv_major;
    uint8_t dummy1;
    uint8_t iv_minor;
    uint8_t dummy2;
    uint16_t iv_revision;
};
#define PP_HTONS(x) ((u16_t)((((x) & (u16_t)0x00ffU) << 8) | (((x) & (u16_t)0xff00U) >> 8)))

static Keti_Err_e Keit_Eth_UDP_Sender(struct udp_pcb *pcb, struct raw_pcb *raw_pcb, u8_t *data, u16_t data_len)
{
	struct pbuf *p;
	err_t ret_err;

	u16_t udp_size = (u16_t)sizeof(struct udp_hdr) + data_len;
	
	p = pbuf_alloc(PBUF_IP, (u16_t)udp_size, PBUF_POOL);
	struct udp_hdr *udphdr = (struct udp_hdr*)p->payload;
	udphdr->src = PP_HTONS(pcb->local_port);
	udphdr->dest = PP_HTONS(pcb->remote_port);
	udphdr->len = PP_HTONS(sizeof(struct udp_hdr) + data_len);
	udphdr->chksum = 0x0000;
	u16_t udpchksum = ip_chksum_pseudo(p, IP_PROTO_UDP, p->tot_len, (ip_addr_t*)&pcb->local_ip, (ip_addr_t*)&pcb->remote_ip);
	if (udpchksum == 0x0000) {
	  udpchksum = 0xffff;
	}
	udphdr->chksum = udpchksum;
	u8_t *payload = (u8_t*)(udphdr + sizeof(struct udp_hdr)/8);
	memcpy(payload, data, data_len);
	ret_err = raw_sendto(raw_pcb, p, (ip_addr_t*)&pcb->remote_ip);
	pbuf_free(p);

	return ERR_OK;
}
struct Keti_UDP_Data_Recv_Buf_t g_udp_recv_buf;
static void Keti_Eth_UDP_Receiver(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    struct ip_hdr *iphdr;
	struct udp_hdr *udphdr;
	struct Eth_Addr_Info_t *info = (struct Eth_Addr_Info_t*)(arg);
    u8_t STX = 0xAA;
    u8_t ETX = 0xCE;
    int recv_hdr_len = sizeof(struct Keti_UDP_Header_t);
    size_t recv_len = p->len - recv_hdr_len;
    int ret;

#if 0
    PRINTF("recvf_len:%d, ", p->len);
    PRINTF("recv_hdr_len:%d, ", recv_hdr_len);
    PRINTF("recv_len:%d, recv_data:", recv_len);
    for(int i = 0; i < recv_hdr_len; i++)
    {
    	PRINTF("%02X ", ((char*)p->payload)[i]);
    }
    PRINTF("\r\n");
#endif

    switch(info->rs)
    {
        default:break;
        case Fireware_Download_Task_Start:
        case Fireware_Wait_Indication_From_ECU:
        {
            if(strncmp((char*)p->payload, (char*)&STX, 1) == 0 && strncmp((u8_t*)(p->payload) + 1, (u8_t*)&ETX, 1) == 0)
            {
				struct Keti_UDP_Header_t hdr;
				memcpy(&hdr, (u8_t*)p->payload, recv_hdr_len);
				memcpy(&hdr.div_len, &hdr.div_len, 4);
				memcpy(&hdr.div_num, &hdr.div_num, 4);
				memcpy(&hdr.total_data_len, &hdr.total_data_len, 4);
#if 0
				PRINTF("hdr.STX:%02X\n\r", hdr.STX);
				PRINTF("hdr.div_len:%d\n\r", hdr.div_len);
				PRINTF("hdr.div_num:%d\n\r", hdr.div_num);
				PRINTF("hdr.total_data_len:%d\n\r", hdr.total_data_len);
				PRINTF("hdr.ETX:%02X\n\r", hdr.ETX);
				if(ret == 0){
					PRINTF("[DEBUG] Flash OK!\r\n");
				}else{
					PRINTF("[DEBUG] Flash Error!\r\n");
				}

			    PRINTF("\r\n");
#endif
                if(g_udp_recv_buf.isempty == 0);
                {
                    g_udp_recv_buf.isempty = 1;
                    g_udp_recv_buf.recv_length.totla_len = hdr.total_data_len;
                    g_udp_recv_buf.recv_length.now_len = 0;

                    g_udp_recv_buf.recv_data = mem_malloc(g_udp_recv_buf.recv_length.totla_len);
                    memset(g_udp_recv_buf.recv_data, 0x00, g_udp_recv_buf.recv_length.totla_len);

                    memcpy(g_udp_recv_buf.recv_data + g_udp_recv_buf.recv_length.now_len, (char*)p->payload + recv_hdr_len, recv_len);
                    g_udp_recv_buf.recv_length.now_len = g_udp_recv_buf.recv_length.now_len + recv_len;
                    info->rs = Fireware_Waiting_Download_Divied_Data_From_ECU;
                }
            }
            break;
        }
	    case Fireware_Response_Info_From_ECU:
        {
            break;
        }
	    case Fireware_Waiting_Download_From_ECU:
        case Fireware_Waiting_Download_Divied_Data_From_ECU:
        {
            if(strncmp((char*)p->payload, (char*)&STX, 1) == 0 && strncmp((u8_t*)(p->payload) + 1, (u8_t*)&ETX, 1) == 0)
            {
				struct Keti_UDP_Header_t hdr;
				memcpy(&hdr, (u8_t*)p->payload, recv_hdr_len);
				memcpy(&hdr.div_len, &hdr.div_len, 4);
				memcpy(&hdr.div_num, &hdr.div_num, 4);
				memcpy(&hdr.total_data_len, &hdr.total_data_len, 4);

                if(g_udp_recv_buf.isempty == 1);
                {
                    g_udp_recv_buf.isempty = 1;
                    memcpy(g_udp_recv_buf.recv_data + g_udp_recv_buf.recv_length.now_len, (char*)p->payload + recv_hdr_len, recv_len);
                    if(g_udp_recv_buf.recv_length.now_len + recv_len <= hdr.total_data_len)
                    {
                        g_udp_recv_buf.recv_length.now_len = g_udp_recv_buf.recv_length.now_len + recv_len;
                    }
                    info->rs = Fireware_Waiting_Download_Divied_Data_From_ECU;
                }

				if(hdr.total_data_len ==  g_udp_recv_buf.recv_length.now_len)
				{
	                PRINTF("RECV_OK! Recv_Data:[%d], %d/%d\r\n", recv_len, g_udp_recv_buf.recv_length.now_len, hdr.total_data_len);
	                uint8_t ota_img_version[8];
	                PRINTF("[DEBUG] SLOT 2 erasing.\r\n");
	                ret = flash_erase(FLASH_AREA_IMAGE_2_OFFSET,  FLASH_AREA_IMAGE_2_SIZE);PRINTF("\r\n");
	                PRINTF("[DEBUG] SLOT 3 erasing.\r\n");
	                ret = flash_erase(FLASH_AREA_IMAGE_3_OFFSET,  FLASH_AREA_IMAGE_3_SIZE);PRINTF("\r\n");

	                if(1)
	                {
	                	ret = boot_flash_area_read(FLASH_AREA_IMAGE_2_OFFSET + IMAGE_VERSION_OFFSET, ota_img_version, 8);
						struct image_version *ota_img_ver = (struct image_version *)ota_img_version;
						PRINTF("Slot 2(Memory area start 0x30800000) image verison: %c.%c.%c\r\n", ota_img_ver->iv_major, ota_img_ver->iv_minor, ota_img_ver->iv_revision);
	                }
	                if(1)
	                {
	                	ret = boot_flash_area_read(FLASH_AREA_IMAGE_3_OFFSET + IMAGE_VERSION_OFFSET, ota_img_version, 8);
						struct image_version *ota_img_ver = (struct image_version *)ota_img_version;
						PRINTF("Slot 3(Memory area start 0x30C00000) image verison: %c.%c.%c\r\n", ota_img_ver->iv_major, ota_img_ver->iv_minor, ota_img_ver->iv_revision);
	                }

	                PRINTF("[DEBUG] Flash flash_program: Write Size is %d.\r\n", g_udp_recv_buf.recv_length.now_len);
	                if(0)//g_udp_recv_buf.recv_length.now_len > 240000)
	                {
	                	u32_t offset_shift = g_udp_recv_buf.recv_length.now_len - 140000;
	                	ret = boot_flash_area_write(FLASH_AREA_IMAGE_3_OFFSET + offset_shift, (void*)g_udp_recv_buf.recv_data + offset_shift, 140000);
	                	ret = boot_flash_area_write(FLASH_AREA_IMAGE_3_OFFSET, (void*)g_udp_recv_buf.recv_data, offset_shift);
	                }else{
	                	ret =  boot_flash_area_write(FLASH_AREA_IMAGE_3_OFFSET, (void*)g_udp_recv_buf.recv_data,  g_udp_recv_buf.recv_length.now_len);
	                }

	                PRINTF("[DEBUG] Flash flash_program Done:%d\r\n", ret);
	                memset(g_udp_recv_buf.recv_data, 0x00, g_udp_recv_buf.recv_length.now_len);
	                mem_free(g_udp_recv_buf.recv_data);

	                if(1)
	                {
	                	ret = boot_flash_area_read(FLASH_AREA_IMAGE_1_OFFSET + IMAGE_VERSION_OFFSET, ota_img_version, 8);
						struct image_version *ota_img_ver = (struct image_version *)ota_img_version;
						PRINTF("[DEBUG] SLOT 1(Memory area start 0x30400000) image verison: %c.%c.%c\r\n", ota_img_ver->iv_major, ota_img_ver->iv_minor, ota_img_ver->iv_revision);
	                }
	                if(1)
	                {
	                	ret = boot_flash_area_read(FLASH_AREA_IMAGE_2_OFFSET + IMAGE_VERSION_OFFSET, ota_img_version, 8);
						struct image_version *ota_img_ver = (struct image_version *)ota_img_version;
						PRINTF("[DEBUG] SLOT 2(Memory area start 0x30C00000) image verison: %c.%c.%c\r\n", ota_img_ver->iv_major, ota_img_ver->iv_minor, ota_img_ver->iv_revision);
	                }
	                struct image_version *now_ota_img_ver;
	                if(1)
	                {
	                	ret = boot_flash_area_read(FLASH_AREA_IMAGE_3_OFFSET + IMAGE_VERSION_OFFSET, ota_img_version, 8);
	                	now_ota_img_ver = (struct image_version *)ota_img_version;
						PRINTF("[DEBUG] SLOT 3(Memory area start 0x30C00000) image verison: %c.%c.%c\r\n", now_ota_img_ver->iv_major, now_ota_img_ver->iv_minor, now_ota_img_ver->iv_revision);
	                }
	                if(now_ota_img_ver->iv_major == 3 &  now_ota_img_ver->iv_minor == 0 & now_ota_img_ver->iv_revision == 0)
	                {
						uint8_t f_img_verify_result = do_firmware_update_rollback();
						PRINTF("[DEBUG] f_img_verify_result:%d\n\r\n", f_img_verify_result);
						if( f_img_verify_result == 2 ) {
							PRINTF("set_magic_field_on_remap_flag!\r\n");
							set_magic_field_on_remap_flag();
						}
	                }else
					{
						uint8_t f_img_verify_result = do_firmware_update();
						PRINTF("[DEBUG] f_img_verify_result:%d\n\r\n", f_img_verify_result);
						if( f_img_verify_result == 2 ) {
							PRINTF("set_magic_field_on_remap_flag!\r\n");
							set_magic_field_on_remap_flag();
						}
					}
						g_udp_recv_buf.recv_length.totla_len = 0;
						g_udp_recv_buf.recv_length.now_len = 0;
						g_udp_recv_buf.isempty = 0;

				}mem_free(g_udp_recv_buf.recv_data);
            }
            break;
        }
	    case Fireware_Finish_Download_From_ECU:
	    {
            break;
        }
        case Fireware_Check_Download_Payload:
	    {
            break;
        }
        case Fireware_Error_Download_From_ECU:
	    {
            break;
        }
        case Fireware_Flashing_Download_Payload:
	    {
            break;
        }
        case Fireware_Check_Flashing_Download_Payload:
	    {
            break;
        }
        case Fireware_Error_Flashing_Download_Payload:
        {
            break;
        }

    }
    pbuf_clen(p);
    pbuf_free(p);
    return;
}


static void Keti_Eth_UDP_Receiver_1G(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	return;
}

static int Keti_Common_Hex2Dec(char data)
{
    int ret;
    if(48 <= (int)(data)  && (int)(data)  <= 57){
        ret = (int)(data) - 48;
    }else if(65 <= (int)(data)  && (int)(data)  <= 70)
    {
        ret = (int)(data) - 65 + 10;
    }else if(97 <= (int)(data)  && (int)(data)  <= 102)
    {
        ret = (int)(data)- 97 + 10;
    }
    return ret;
}


static struct Keti_UDP_Header_t Keti_UDP_Header_Parser(char* data)
{

}
