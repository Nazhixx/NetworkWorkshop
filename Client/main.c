#include <stdio.h>
#include <winsock2.h>

int initialize(int);
void print_new();
char *send_data(char *);

struct sockaddr_in server;
int m_index = 0;

int main() {
    int port;
    scanf("%d", &port);
    int can_initialize = initialize(port);
    while (can_initialize == 0) {
        int num;
        scanf("%d", &num);
        if (num == 1){
            print_new();
        }else{
            char message[1000];
            scanf("%*c%[^\n]s", message);
            char* feedback = send_data(message);
            printf("%s\n", feedback);
        }
    }

}

int initialize(int port) {
    WSADATA wsadata;
    int wsaStartUp = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (wsaStartUp != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return -1;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    return 0;
}

char *send_data(char *data) {
    char *buffer = malloc(1000);
    memset(buffer, 0, 1000);
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        return buffer;
    }
    int can_connect = connect(client_socket, (struct sockaddr *) &server, sizeof(server));
    if (can_connect != 0) {
        return buffer;
    }
    send(client_socket, data, strlen(data), 0);
    recv(client_socket, buffer, 999, 0);
    closesocket(client_socket);
    return buffer;
}

void print_new() {
    while (1) {
        char *message;
        char data[15];
        sprintf(data, ":%d", m_index);
        message = send_data(data);
        if (message[0] == ':') {
            break;
        }
        printf("%s\n",message);
        m_index++;
    }
}