#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 10000
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server;
    char buffer[BUF_SIZE];
    socklen_t len = sizeof(server);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUF_SIZE, stdin);

        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server, len) < 0) {
            perror("Send to failed");
            close(sockfd);
            exit(1);
        }

        memset(buffer, 0, BUF_SIZE);

        if (recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&server, &len) < 0) {
            perror("Recvfrom failed");
            close(sockfd);
            exit(1);
        }

        printf("Echo from server: %s", buffer);
    }

    close(sockfd);
    return 0;
}

