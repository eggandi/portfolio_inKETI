#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main()
{
	 char rx_buf[256];
	 rx_buf[0] = 0xAB;
	 rx_buf[1] = 0xCD;
	 rx_buf[2] = 0xEF;
	 rx_buf[3] = 0x01;
	 uint16_t stx = (*(uint16_t*)(rx_buf));
	printf("STX: %04X\n", stx);
	 return 0;
}