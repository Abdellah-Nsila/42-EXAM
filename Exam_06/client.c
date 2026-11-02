#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>  // Essential for structures and byte ordering
#include <sys/socket.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // 1. Create the socket (IPv4, TCP stream)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("[-] Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("[+] Socket successfully created.\n");

    // 2. Configure the server address structure
    memset(&server_addr, 0, sizeof(server_addr)); // Clear the memory block
    server_addr.sin_family = AF_INET;             // IPv4 Address family
    server_addr.sin_port = htons(8080);           // Convert port to network byte order (Big-Endian)

    // Convert IP address from text format to binary format
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("[-] Invalid address / Address not supported");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 3. Connect to the remote server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("[-] Connection to the server failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("[+] Successfully connected to the server!\n");

    // 4. Perform clean up when finished
    close(sockfd);
    return 0;
}
