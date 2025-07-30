#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <unistd.h>
#include <pthread.h>

// IP 헤더 구조체
#if 0
struct iphdr {
    unsigned int ihl:4;        // Header length
    unsigned int version:4;    // Version
    unsigned char tos;         // Type of service
    unsigned short tot_len;    // Total length
    unsigned short id;         // Identification
    unsigned short frag_off;   // Fragment offset
    unsigned char ttl;         // Time to live
    unsigned char protocol;    // Protocol
    unsigned short check;      // Checksum
    unsigned int saddr;        // Source address
    unsigned int daddr;        // Destination address
};

// TCP 헤더 구조체
struct tcphdr {
    unsigned short source;     // Source port
    unsigned short dest;       // Destination port
    unsigned int seq;          // Sequence number
    unsigned int ack_seq;      // Acknowledgment number
    unsigned short res1:4;     // Reserved
    unsigned short doff:4;     // Data offset
    unsigned short fin:1;      // FIN flag
    unsigned short syn:1;      // SYN flag
    unsigned short rst:1;      // RST flag
    unsigned short psh:1;      // PSH flag
    unsigned short ack:1;      // ACK flag
    unsigned short urg:1;      // URG flag
    unsigned short res2:2;     // Reserved
    unsigned short window;     // Window size
    unsigned short check;      // Checksum
    unsigned short urg_ptr;    // Urgent pointer
};
#endif

#define PORT 8080

struct flag_off_t{
    uint16_t flagnemt_offet:13;
    uint16_t M:1;
    uint16_t D:1;
    uint16_t x:1;
};

int receiver(char *argv[]);
int sender(char *argv[]);
long get_file_size(const char *filename);

int main(int argc, char *argv[])
{
    if (argc != 3 && argc != 4) 
    {
        fprintf(stderr, "Usage: %s <r> <server_addr>\n", argv[0]);
        fprintf(stderr, "Usage: %s <s> <dest_addr> <file_path>\n", argv[0]);
        return 1;
    }
    if(argc == 3)
    {
        if(strcmp(argv[1],"r") == 0)
        {
            receiver(argv);
        }
    }
    if(argc == 4)
    {
        if(strcmp(argv[1],"s") ==0)
        {
            sender(argv);
        }
    }
}

int sender(char *argv[])
{
     int sock = 0;
    struct sockaddr_in serv_addr;

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 서버 주소 설정
    if (inet_pton(AF_INET, argv[2], &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");
    long file_size = get_file_size(argv[3]);
    // 파일 열기
    FILE *file = fopen(argv[3], "rb");
    if (!file) {
        perror("File open failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read;
    char *buffer = malloc(file_size * sizeof(char));
    memset(buffer, 0x00, file_size);
    bytes_read = fread(buffer, 1, file_size, file);
    
    ssize_t ret = sendto(sock, buffer, bytes_read, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret > 0)
    {
        printf("Tcp Send Success %d/%d\n", ret, bytes_read);
    }
    close(sock);
    fclose(file);

    return 0;
}

long get_file_size(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // 파일 끝으로 이동
    fseek(file, 0, SEEK_END);

    // 현재 위치(파일 크기) 가져오기
    long size = ftell(file);

    // 파일 닫기
    fclose(file);

    return size;
}
void *th_UDP_RECEIVER(void *d)
{
    char *ip_addr= (char*)d;    
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    // 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    // 주소 설정
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip_addr, &address.sin_addr);
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, addrlen) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for connections...\n");
    ssize_t bytes_received;
    char buffer[65536] = {0,};
    while (1)
    {
        bytes_received = recvfrom(server_fd, buffer, 65536, 0, NULL, NULL);
        if(bytes_received > 0)
        {
            //printf("File received successfully. %d\n", bytes_received);
        }
    }
}
int receiver(char *argv[]) 
{
    int sock_raw;
    struct sockaddr_in source, dest;
    unsigned char *buffer = (unsigned char *)malloc(65536); // 패킷 데이터 저장 버퍼
    struct iphdr *ip_header;
    //struct tcphdr *tcp_header;

    // Raw 소켓 생성
    sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sock_raw < 0) {
        perror("Socket creation failed");
        return 1;
    }

    printf("Listening for incoming UDP packets...\n");
    //pthread_t th_udp;
    //pthread_create(&th_udp, NULL, th_UDP_RECEIVER, (void*)argv[2]);
    //pthread_detach(th_udp);
    while (1) {
        //socklen_t addr_len = sizeof(source);
        int data_size = recvfrom(sock_raw, buffer, 65536, 0, NULL, NULL);

        if (data_size < 0) {
            perror("Packet reception failed");
            continue;
        }

        // IP 헤더 파싱
        ip_header = (struct iphdr *)buffer;
        unsigned short iphdrlen = ip_header->ihl * 4;

        // TCP 헤더 파싱
        struct udphdr *udp_header = (struct udphdr *)(buffer + iphdrlen);

        // IP 주소 변환
        memset(&source, 0, sizeof(source));
        source.sin_addr.s_addr = ip_header->saddr;

        memset(&dest, 0, sizeof(dest));
        dest.sin_addr.s_addr = ip_header->daddr;

        // 헤더 정보 출력

        if(ip_header->daddr == inet_addr(argv[2]) && udp_header->dest == htons(8080))
        {
            printf("\n=== IP Header ===\n");
            //printf("Version: %u\n", ip_header->version);
            //printf("Header Length: %u bytes\n", ip_header->ihl * 4);
            //printf("Type of Service: %u\n", ip_header->tos);
            printf("Total Length: %u bytes\n", ntohs(ip_header->tot_len));
            printf("Identification: %u\n", ntohs(ip_header->id));
            struct flag_off_t flag;
            uint16_t f = ntohs(ip_header->frag_off);
            memcpy(&flag, &f, 2);
            printf("Length Before Fragmention: %u\n", ntohs(ip_header->frag_off));
            printf("Don't Flagment: %u\n", flag.D);
            printf("More Fragment: %u\n", flag.M);
            printf("Length Before Fragmention: %u\n", flag.flagnemt_offet);
            printf("Time to Live (TTL): %u\n", ip_header->ttl);
            //printf("Protocol: %u\n", ip_header->protocol);
            //printf("Header Checksum: %u\n", ntohs(ip_header->check));
            //printf("Source IP: %s\n", inet_ntoa(source.sin_addr));
            //printf("Destination IP: %s\n", inet_ntoa(dest.sin_addr));
            
            printf("\n=== UDP Packet Header ===\n");
            printf("Source Port: %u\n", ntohs(udp_header->source));
            printf("Destination Port: %u\n", ntohs(udp_header->dest));
            printf("Length: %u bytes\n", ntohs(udp_header->len));
            //printf("Source IP: %s\n", inet_ntoa(source.sin_addr));
            //printf("Destination IP: %s\n", inet_ntoa(dest.sin_addr));
            //printf("Source Port: %u\n", ntohs(tcp_header->source));
            //printf("Destination Port: %u\n", ntohs(tcp_header->dest));
            //printf("Sequence Number: %u\n", ntohl(tcp_header->seq));
            //printf("Acknowledgment Number: %u\n", ntohl(tcp_header->ack_seq));
            //printf("Flags: SYN=%d, ACK=%d, PSH=%d, FIN=%d\n",
            //    tcp_header->syn, tcp_header->ack, tcp_header->psh, tcp_header->fin);
            //printf("Window Size: %u\n", ntohs(tcp_header->window));
        }
    }

    close(sock_raw);
    free(buffer);
    return 0;
}
