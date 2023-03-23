#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 30

int main(int argc, char* argv[]){
    if (argc != 3){
        exit(-1);
    }

    int fd_clnt = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    char msg[BUFSIZE];
    fputs("Please int some message: \n", stdout);
    fgets(msg, BUFSIZE - 1, stdin);
    msg[BUFSIZE - 1] = 0x00;

    int res1 = sendto(fd_clnt, msg, BUFSIZE, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("res1: %d\n", res1);
    fputs(msg, stdout);
    sendto(fd_clnt, msg, BUFSIZE, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    fputs(msg, stdout);
    sendto(fd_clnt, msg, BUFSIZE, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    fputs(msg, stdout);
    

    close(fd_clnt);
    exit(-1);
}