#include "relay_utils.h"
/**
 * @brief IPv4 주소를 가져오는 함수
 * @details 지정된 네트워크 인터페이스 명의 IPv4 주소를 가져옵니다.
 * @param iface 인터페이스 이름 (예: "eth0", "wlan0")
 * @param out_ipv4_addr IPv4 주소를 저장할 버퍼 (문자열 형식)
 * @param out_prefix 서브넷 마스크 길이를 저장할 포인터
 * @return void
 */
extern int RELAY_INNO_Utils_IP_Get_IPv4(const char *iface, char *out_ipv4_addr, int *out_prefix) 
{
  struct ifaddrs *ifaddr, *ifa;

  // 시스템에 등록된 모든 네트워크 인터페이스 정보를 가져옴
  if (getifaddrs(&ifaddr) == -1) {
    _DEBUG_PRINT("getifaddrs");
    return -1;
  }

  // 모든 인터페이스를 순회하면서 IPv4 주소 검색
  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
	{
    if (ifa->ifa_addr == NULL) continue;

    // 인터페이스가 지정한 이름과 일치하고 AF_INET (IPv4) 주소인 경우
    if (strcmp(ifa->ifa_name, iface) == 0 && ifa->ifa_addr->sa_family == AF_INET)
		{
      struct sockaddr_in *sockaddr = (struct sockaddr_in *)ifa->ifa_addr;

      // IPv4 주소를 문자열로 변환하여 출력
      if (inet_ntop(AF_INET, &sockaddr->sin_addr, out_ipv4_addr, INET_ADDRSTRLEN) != NULL) {
        // 성공적으로 변환된 경우
        _DEBUG_PRINT("IPv4 address of %s: %s\n", iface, out_ipv4_addr);
      } else {
        _DEBUG_PRINT("inet_ntop");
				return -2;
      }

      int out_prefix_length = 0;
      unsigned char *ptr = (unsigned char *)&sockaddr->sin_addr;
      for (int i = 0; i < INET_ADDRSTRLEN; i++) 
      {
        unsigned char byte = ptr[i];
        while (byte) 
				{
          out_prefix_length += byte & 1;
          byte >>= 1;
        }
      }
      *out_prefix = out_prefix_length;
    }
  }
		return 0;
  freeifaddrs(ifaddr);	
}