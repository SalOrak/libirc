#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BIND_ADDR "127.0.0.1"
#define BIND_PORT 8080
#define MAX 1024

#define reterr(ERR_MSG) \
    do { \
        perror(ERR_MSG); \
        return 1; \
    } while(0) 

const char *err_msg = NULL;


int handle_connection(int connfd) 
{
    char buff[MAX];
    char bufr[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(connfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));

        recv(connfd, bufr, sizeof(bufr), MSG_DONTWAIT);

        printf("From Server : %s\n", bufr);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }}

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

    int connfd = connect(sockfd, (struct sockaddr *) &srv_addr, sizeof(struct sockaddr));
    if (connfd == -1)
        reterr(err_msg);

    printf("[INFO] Socket File Descriptor: %d\n", sockfd);
    printf("[INFO] Connection File Descriptor: %d\n", connfd);

    handle_connection(sockfd);

    return 0;
}
