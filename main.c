#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define CHUNKSIZE 1024

int connect_socket(const char* ip_address, int port){
    // Open socket
    int sock;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    return sock;
}

int main(int argc, char const *argv[]) {
    printf("Welcome to the image color classifier!\n");
    if(argc == 3) {
        int socket;
        while (1) {
            printf("Please write the path to the image:\n");

            char *path = NULL;
            size_t len = 0;
            getline(&path, &len, stdin);

            // Open file

            printf("Path: %s\n", path);
            FILE *input = fopen(path, "rb");
            if (input == NULL) {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
            } else {
                socket = connect_socket(argv[1], atoi(argv[2]));
                unsigned char file_buffer[CHUNKSIZE];
                int read, sent, sent_total = 0;
                sent = send(socket, path, 30, 0);
                while ((read = fread(file_buffer, 1, CHUNKSIZE, input)) != 0) {
                    sent = send(socket, file_buffer, read, 0);
                    sent_total += sent;
                }
                printf("Total bytes sent: %d bytes\n", sent_total);
                fclose(input);
                close(socket);
            }
            printf("Do you want to classify another image? (y/n) ");
            int answer = getchar();
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            if(answer == 'n'){
                break;
            }
        }

    } else{
        printf("Two parameters required: server_ip_address server_ip_port");
        return 1;
    }
    return 0;
}
