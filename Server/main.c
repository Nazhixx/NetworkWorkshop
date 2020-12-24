#include <stdio.h>
#ifndef LINUX
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

int initialize(int);
void start(int);

int main() {
    int port;
    scanf("%d", &port);
    int server_socket = initialize(port);
    if(server_socket != INVALID_SOCKET){
        start(server_socket);
    }
    return 0;
}

int initialize(int port) {
    WSADATA wsadata;
    int wsaStartUp = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (wsaStartUp != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return INVALID_SOCKET;
    }
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return INVALID_SOCKET;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    int is_bind = bind(server_socket, (struct sockaddr *) &server, sizeof(server));
    if (is_bind == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return INVALID_SOCKET;
    }

    listen(server_socket, SOMAXCONN);
    return server_socket;
}

void start(int server_socket) {
    int len = sizeof(struct sockaddr_in);
    int client_socket;
    struct sockaddr_in client;
    char *messages[1000];
    int size = 0;
    char buffer[1000];
    while ((client_socket = accept(server_socket, (struct sockaddr *) &client, &len)) != INVALID_SOCKET) {
        memset(buffer, 0, 1000);
        recv(client_socket, buffer, 999, 0);
        if (buffer[0] == ':') {
            int index;
            sscanf(buffer, "%d", &index);
            if (index < size) {
                send(client_socket, messages[index], strlen(messages[index]), 0);
            } else {
                char output[15];
                sprintf(output, ":%d", size);
                send(client_socket, output, strlen(output), 0);
            }
        } else {
            messages[size] = malloc(strlen(buffer));
            strcpy(messages[size], buffer);
            size++;
            send(client_socket, "Message Recieved", 16, 0);
        }
        closesocket(client_socket);
    }
}