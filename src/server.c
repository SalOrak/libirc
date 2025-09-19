#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>

#define BIND_ADDR "127.0.0.1"
#define BIND_PORT 8080
#define MAX 80
#define MAX_NUM_CLIENTS 10

#define reterr(ERR) \
        do { \
            perror(ERR);  \
            return 1; \
        }while(0) 


// Global error message
const char *err_msg = NULL;

// Basic read/write connection. Its blocking.
int handle_connection(int connfd){
    char rd_buffer[MAX]; 
    int read_count;
    int i;

    printf("[CLIENT]: [%d] Handling connection\n", connfd);
    for(;;){
        bzero(rd_buffer, MAX);

        // Read from the client
        read_count = recv(connfd, rd_buffer, sizeof(rd_buffer), 0);

        // Print from the client
        printf("[CLIENT] says: %s\n", rd_buffer);

        if (strncasecmp("EXIT",rd_buffer, 4) == 0) break;

        // Send message from CLi
        bzero(rd_buffer, MAX);
        i = 0;
        while( (rd_buffer[i++] = getchar()) != '\n'){};
        write(connfd,rd_buffer, sizeof(rd_buffer));
    }
    printf("[CLIENT]: [%d] Closing connection\n", connfd);
    close(connfd);
    return 1;
}


/// Polling based connections. "Asynchronous" per nature. Nonblocking.
void handle_clients(struct pollfd *clients, int num_clients){
    char buf[1024]; 
    short rev;

    for (int i = 0 ; i < MAX_NUM_CLIENTS && num_clients > 0; i ++){
        if (clients[i].fd <= 0 ) continue;

        // Zero-out read buffer
        bzero(buf, sizeof(buf));


        rev = clients[i].revents;
        // React to Events from each client
        if (rev &  POLLERR || rev & POLLHUP || rev & POLLRDHUP || rev & POLLPRI) {
            printf("[EVENT] Client %d closing connection...\n", clients[i].fd);
            close(clients[i].fd);
            printf("[EVENT] Client %d is closed\n", clients[i].fd);
            clients[i].fd = 0;
        } else if (rev & POLLIN) {
            read(clients[i].fd, buf, sizeof(buf));
            if (strlen(buf) == 0) continue;
            printf("[EVENT] Client %d says %s\n", clients[i].fd, buf);
        } else if (rev & POLLOUT){}
        num_clients--;
    }
}




int main()
{
    struct in_addr addr;
    struct sockaddr_in srv_addr;
    struct sockaddr_in client_addr;
    int sockfd;
    int connfd;
    int num_clients;
    struct pollfd *clients;

    if (inet_aton(BIND_ADDR, &addr) == 0){
        fprintf(stderr, "Invalid address %s\n", BIND_ADDR);
        return 1;
    }

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr = addr;
    srv_addr.sin_port = htons(BIND_PORT);

    sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (sockfd == 1)
        reterr(err_msg);

    if (bind(sockfd, (struct sockaddr *) &srv_addr, sizeof(struct sockaddr)) == -1) 
        reterr(err_msg);

    if (listen(sockfd, 100) != 0)
        reterr(err_msg);

    clients = malloc(sizeof(struct pollfd) * MAX_NUM_CLIENTS);

    socklen_t client_len = sizeof(struct sockaddr_in);

    printf("Waiting to accept connection...\n");

    while(1){
        num_clients = poll(clients, (nfds_t ) MAX_NUM_CLIENTS, 200);

        handle_clients(clients, num_clients);

        connfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (connfd == -1)
            continue;

        for (int i = 0; i < MAX_NUM_CLIENTS; i++){
            if (clients[i].fd <= 0) {
                printf("[INFO] Client connect at FD %d set at position %d\n", connfd, i);

                clients[i].fd = connfd;
                clients[i].events = POLLIN | POLLOUT | POLLHUP | POLLRDHUP ;

                break;
            }
        }
    };

    close(sockfd);
    free(clients);

    return 0;
}
