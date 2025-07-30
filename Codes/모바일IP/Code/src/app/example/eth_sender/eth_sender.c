#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <netinet/ether.h>


#define DEST_MAC "ff:ff:ff:ff:ff:ff" // 목적지 MAC 주소

void usage(const char *prog_name) {
    printf("Usage: %s  <IP> <Port> <Interval(ms)> <Packet Size(bytes)> <IP_Src> <IF> <MAC>\n", prog_name);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        usage(argv[0]);
    }

    // Parse arguments

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int interval_ms = atoi(argv[3]);
    int packet_size = atoi(argv[4]);
    int sockfd;
    if(argc == 7)
    {    
        const char *if_name = argv[5];
        const char *mac = argv[6];
        struct ifreq if_idx;
        struct ifreq if_mac;
        struct sockaddr_ll socket_address;
        char buffer[ETH_FRAME_LEN]; // 이더넷 프레임 버퍼
         // 인터페이스 이름 설정
        memset(&if_idx, 0, sizeof(struct ifreq));
        strncpy(if_idx.ifr_name, if_name, IFNAMSIZ - 1);
        if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0) {
            perror("SIOCGIFINDEX");
            close(sockfd);
            return -1;
        }
        // 인터페이스 MAC 주소 가져오기
        memset(&if_mac, 0, sizeof(struct ifreq));
        strncpy(if_mac.ifr_name, if_name, IFNAMSIZ - 1);
        if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0) {
            perror("SIOCGIFHWADDR");
            close(sockfd);
            return -1;
        }
            // 이더넷 프레임 생성
        memset(buffer, 0, ETH_FRAME_LEN);
        struct ether_header *eh = (struct ether_header *) buffer;

        // 목적지 MAC 주소 설정
        sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
            &eh->ether_dhost[0], &eh->ether_dhost[1], &eh->ether_dhost[2],
            &eh->ether_dhost[3], &eh->ether_dhost[4], &eh->ether_dhost[5]);

        // 소스 MAC 주소 설정
        memcpy(eh->ether_shost, if_mac.ifr_hwaddr.sa_data, 6);

    }else if(argc == 5){
            // Create socket
        
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }

        // Prepare destination address
        struct sockaddr_in dest_addr;
        memset(&dest_addr, 0, sizeof(dest_addr));
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip, &dest_addr.sin_addr) <= 0) {
            perror("Invalid IP address");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        if (packet_size < 1 || packet_size > 65507) { // Maximum UDP payload size
        fprintf(stderr, "Invalid packet size. Must be between 1 and 65507 bytes.\n");
        exit(EXIT_FAILURE);
        }

        // Prepare packet data
        char *packet = (char *)malloc(packet_size);
        if (!packet) {
            perror("Memory allocation failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        memset(packet, 'A', packet_size); // Fill with dummy data

        printf("Sending UDP packets to %s:%d every %d ms with %d bytes of data.\n",
            ip, port, interval_ms, packet_size);

        while (1) {
            // Send packet
            ssize_t sent_bytes = sendto(sockfd, packet, packet_size, 0,(struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (sent_bytes < 0) {
                perror("Send failed");
                break;
            }
            printf("Sent %d bytes to %s:%d\n", sent_bytes, ip, port);

            // Sleep for the specified interval
            usleep(interval_ms * 1000); // Convert ms to microseconds
        }

        // Clean up
        free(packet);
        close(sockfd);
    }else if(argc == 6){

    }


   
    return 0;
}


int main() {
    int sockfd;
    struct ifreq if_idx;
    struct ifreq if_mac;
    struct sockaddr_ll socket_address;
    char buffer[ETH_FRAME_LEN]; // 이더넷 프레임 버퍼

    // 소켓 생성
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // 인터페이스 이름 설정
    memset(&if_idx, 0, sizeof(struct ifreq));
    strncpy(if_idx.ifr_name, INTERFACE, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0) {
        perror("SIOCGIFINDEX");
        close(sockfd);
        return -1;
    }

    // 인터페이스 MAC 주소 가져오기
    memset(&if_mac, 0, sizeof(struct ifreq));
    strncpy(if_mac.ifr_name, INTERFACE, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0) {
        perror("SIOCGIFHWADDR");
        close(sockfd);
        return -1;
    }

    // 이더넷 프레임 생성
    memset(buffer, 0, ETH_FRAME_LEN);
    struct ether_header *eh = (struct ether_header *) buffer;

    // 목적지 MAC 주소 설정
    sscanf(DEST_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &eh->ether_dhost[0], &eh->ether_dhost[1], &eh->ether_dhost[2],
           &eh->ether_dhost[3], &eh->ether_dhost[4], &eh->ether_dhost[5]);

    // 소스 MAC 주소 설정
    memcpy(eh->ether_shost, if_mac.ifr_hwaddr.sa_data, 6);

    // 이더넷 타입 설정
    eh->ether_type = htons(ETH_P_IP);

    // 데이터 페이로드 추가
    const char *payload = "Hello, MAC Layer!";
    size_t payload_len = strlen(payload);
    memcpy(buffer + sizeof(struct ether_header), payload, payload_len);

    // 소켓 주소 설정
    memset(&socket_address, 0, sizeof(struct sockaddr_ll));
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    socket_address.sll_halen = ETH_ALEN;
    memcpy(socket_address.sll_addr, eh->ether_dhost, 6);

    // 패킷 송신
    if (sendto(sockfd, buffer, sizeof(struct ether_header) + payload_len, 0,
               (struct sockaddr *) &socket_address, sizeof(struct sockaddr_ll)) < 0) {
        perror("Send failed");
        close(sockfd);
        return -1;
    }

    printf("Packet sent successfully!\n");

    close(sockfd);
    return 0;
}

