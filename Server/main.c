#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>

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

    //Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return INVALID_SOCKET;
    }

    //Prepare the sockaddr_in structure
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    //Bind
    int bind_res = bind(server_socket, (struct sockaddr *) &server, sizeof(server));
    if (bind_res == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return INVALID_SOCKET;
    }

    //Listen to incoming connections
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
        printf("Client connected\n");
        memset(buffer, 0, 1000);
        recv(client_socket, buffer, 999, 0);
        printf("Client message: %s\n", buffer);

        char *response;
        if (buffer[0] == ':') {
            int index;
            sscanf(buffer, ":%d", &index);
            if (index < size) {
                response = messages[index];
            } else {
                response = malloc(15);
                sprintf(response, ":%d", size);
            }
        } else {
            messages[size] = malloc(strlen(buffer));
            strcpy(messages[size], buffer);
            size++;

            response = "Message Received";
        }
        send(client_socket, response, strlen(response), 0);
        printf("Server response: %s\n\n", response);
        closesocket(client_socket);
    }
}
