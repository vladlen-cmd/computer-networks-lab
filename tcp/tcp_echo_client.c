#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PRN 123 
#define PORT (10000 + PRN)
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server on port %d.\n", PORT);

    while (1) {
        printf("Enter message (type 'exit' to quit): ");
        fgets(buffer, BUF_SIZE, stdin);

        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }

        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUF_SIZE);
        int bytes = recv(sock, buffer, BUF_SIZE, 0);
        if (bytes <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        printf("Server echoed: %s", buffer);
    }

    close(sock);
    printf("Client exited.\n");

    return 0;
}

