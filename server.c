#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
    #define close_socket closesocket
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #define close_socket close
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    typedef int SOCKET;
#endif

int main(){
    setbuf(stdout, NULL);

    #ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) !=0) return 1;
    #endif

    char *port_env = getenv("PORT");
    int port = port_env ? atoi(port_env) : 7860;

    SOCKET server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[2048];

    if((server_fd = (socket(AF_INET, SOCK_STREAM, 0)))== INVALID_SOCKET){
        perror("Socket Failure !");
        return 1;
    }

    int opt=1;
    #ifdef _WIN32
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
    #else
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    #endif

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Binding Failure !");
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listening  Failure !");
        return 1;
    }

    printf("Server successfully running on port %d...\n", port);

    while(1){
        client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if(client_socket == INVALID_SOCKET) continue;

        int valread = recv(client_socket, buffer, sizeof(buffer)-1, 0);
        if (valread > 0) {
            buffer[valread] = '\0';
        }

        const char *body = "Server is Live !\n";
        char response[1024];

        const char *json_body = "{"
        "\"status\": \"success\", "
        "\"message\": \"Server is Live ✅\", "
        "\"code\": 200"
    "}";

        int response_len = snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n"
            "Content-Length: %d\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Connection: close\r\n\r\n%s",
            strlen(json_body), json_body
        );

        send(client_socket, response, response_len, 0);
        close_socket(client_socket);
    }

    #ifdef _WIN32
    WSACleanup();
    #endif

    return 0;
}

