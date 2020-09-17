#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define CHUNKSIZE 1024

int main(int argc, char const *argv[]) {
    // Open socket
    int sock;
    struct sockaddr_in serv_addr;
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
        unsigned char file_buffer[CHUNKSIZE];
        int read, sent, sent_total = 0;
        while ((read = fread(file_buffer, 1, CHUNKSIZE, input)) != 0) {
            sent = send(sock, file_buffer, read, 0);
            sent_total += sent;
        }
        printf("Total bytes sent: %d bytes\n", sent_total);
        fclose(input);
    }
    return 0;
}
