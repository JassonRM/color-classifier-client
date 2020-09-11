#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    // Open socket
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Open file
    char filename[] = "example1.png";
    printf("Path: %s\n", filename);
    FILE *input = fopen(filename, "rb");
    if (input == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    } else {
        unsigned char file_buffer[1024];
        int i = 0;
        while (fread(file_buffer, sizeof(file_buffer), 1, input) > 0 && i < 10) {
            printf("Still reading file %d\n", i);
            send(sock, file_buffer, sizeof(file_buffer), 0);
            i++;
        }
        fclose(input);
    }

    printf("Message sent\n");
    read(sock, buffer, 1024);

    printf("%s\n", buffer);
    return 0;
}
