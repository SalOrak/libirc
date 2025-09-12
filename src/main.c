#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define BIND_ADDR "0.0.0.0"
#define BIND_PORT 8080

void welcome(int sockefd);

int main() 
{

    struct in_addr addr; 

    if (inet_aton(BIND_ADDR, &addr) == 0 )
    {
        printf("Invalid address\n");
        return 1;
    }

    printf("Address provided: %s\n", inet_ntoa(addr));
        
    struct sockaddr_in saddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_addr = addr;
    saddr.sin_port = htons(BIND_PORT);

    const char *err_msg = NULL;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == 1) 
    {
        perror(err_msg);
        return 1;
    }

    if (bind(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr)) == -1) 
    {
        perror(err_msg);
        return 1;
    }

    if (listen(fd, 20) != 0) 
    {
        perror(err_msg);
        return 1;
    }

    while (1){
        printf("Waiting to accept connection...\n");
        struct sockaddr_in caddr;
        socklen_t clen = sizeof(struct sockaddr_in);
        int sockfd = accept(fd, (struct sockaddr * )&caddr, &clen) ;
        if (sockfd == -1) 
        { perror(err_msg); return 1;}

        printf("Created fd %d ..\n", sockfd);
        printf("[CLIENT] Address: %s\n", inet_ntoa(caddr.sin_addr));
        printf("[CLIENT] Port : %d\n", ntohs(caddr.sin_port));
        welcome(sockfd);
    }

    return 0;
}

void welcome(int sockfd) 
{
    for (size_t i = 0; i < 10; i ++)
    {
        send(sockfd, "Hello world\n", 12, MSG_CONFIRM);
    }
    close(sockfd);
}

