#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

void usage(const char *prog_name) {
    printf("Usage: %s <IP> <Port> <Interval(ms)> <Packet Size(bytes)>\n", prog_name);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        usage(argv[0]);
    }

    // Parse arguments
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int interval_ms = atoi(argv[3]);
    int packet_size = atoi(argv[4]);

    if (packet_size < 1 || packet_size > 65507) { // Maximum UDP payload size
        fprintf(stderr, "Invalid packet size. Must be between 1 and 65507 bytes.\n");
        exit(EXIT_FAILURE);
    }

    // Create socket
    int sockfd;
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
        ssize_t sent_bytes = sendto(sockfd, packet, packet_size, 0,
                                    (struct sockaddr *)&dest_addr, sizeof(dest_addr));
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
    return 0;
}
