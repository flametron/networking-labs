#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#define LIMIT 100

int main(int argc, char *argv[])
{

    char *msg, *input;                 //will just store a string.
    WSADATA wsa;                       // A datastructure that stores the Windows Socket Implementation
    SOCKET s;                          // Oh well, sockets....
    struct sockaddr_in server, client; // Will store addresses
    int c, receive_size, port;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    s = socket(AF_INET, SOCK_STREAM, 0);
    printf("Enter the SERVER IP You want to chat to:");
    input = (char *)calloc(16, sizeof(char));
    fgets(input, 15, stdin);
    server.sin_addr.s_addr = inet_addr(input);
    free(input);
    server.sin_family = AF_INET;
    printf("Enter the SERVER PORT You want to chat to:");
    scanf("%d", &port);
    fflush(stdin);
    server.sin_port = htons(port);
    c = sizeof(struct sockaddr_in);
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("ERROR Connecting to server, exiting.");
        return 1;
    }
    printf("[CLIENT] Chat started with SERVER [%s:%d]\n", inet_ntoa(server.sin_addr), server.sin_port);
    printf("Enter \'q\' to exit chat\n================\n");
    while (1)
    {

        msg = (char *)calloc(LIMIT, sizeof(char));
        receive_size = recv(s, msg, LIMIT, 0);
        msg[receive_size] = '\0';
        if (msg[0] == 'q' && msg[1] == '\n')
        { // This means server is quitting early.
            printf("SERVER has left the chat");
            closesocket(s);
            break;
        }
        printf("SERVER: %s\n", msg);
        free(msg);
        printf("Enter your message:");
        msg = (char *)calloc(LIMIT, sizeof(char));
        fgets(msg, LIMIT, stdin);
        if (msg[0] == 'q' && msg[1] == '\n')
        { // This means that client is quitting early.
            printf("Closing chat");
            send(s, msg, strlen(msg), 0);
            closesocket(s);
            break;
        }
        send(s, msg, strlen(msg), 0);
    }
    shutdown(s, SD_BOTH);
    closesocket(s);
    WSACleanup();
}