#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BIND_ADDR "0.0.0.0"
#define BIND_PORT 8080

#define reterr(ERR) \
        do { \
            perror(ERR);  \
            return 1; \
        }while(0) 


// Global error message
const char *err_msg = NULL;

void handle_connection(int connfd){
    // char buffer[1024] = "Hello world\n\0";
    char rd_buffer[32];
    int rd_ret;

    for(;;){
        // write(connfd, buffer, strlen(buffer));
        do {
            rd_ret = read(connfd,rd_buffer, 32);
        } while(rd_ret > 0);
        printf("Return value is %d\n", rd_ret);
        printf(rd_buffer);
        close(connfd);
        return;
    }
}


int main()
{
    struct in_addr addr;

    if (inet_aton(BIND_ADDR, &addr) == 0){
        fprintf(stderr, "Invalid address %s\n", BIND_ADDR);
        return 1;
    }

    struct sockaddr_in srv_addr = {0};
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr = addr;
    srv_addr.sin_port = htons(BIND_PORT);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == 1)
        reterr(err_msg);

    if (bind(sockfd, (struct sockaddr *) &srv_addr, sizeof(struct sockaddr)) == -1) 
        reterr(err_msg);

    if (listen(sockfd, 100) != 0)
        reterr(err_msg);

    while(1){
        printf("Waiting to accept connection...\n");
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(struct sockaddr_in);

        int connfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);

        if (connfd == -1)
            reterr(err_msg);
        printf("Client (socket %d) connected. \n", connfd);
        printf("[INFO] Client address: %s\n", inet_ntoa(client_addr.sin_addr));
        printf("[INFO] Client port: %d\n", ntohs(client_addr.sin_port));

        handle_connection(connfd);
    };

    return 0;
}
