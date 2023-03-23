#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSIZE 30

void error_handling(const char* msg);
void create_uclient(const char* ip, const char* port);

int main(int argc, char* argv[]){
    if (argc != 3){
        error_handling("usage: <ip> <port>");
    }

    create_uclient(argv[1], argv[2]);
    exit(0);
}

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}

void create_uclient(const char* ip, const char* port){
    int fd_clnt = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd_clnt == -1){
        error_handling("socket() error");
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(atoi(port));

    while (1){
        fputs("please enter msg you want to send: ", stdout);
        char msg[BUFSIZE];
        fgets(msg, BUFSIZE - 1, stdin);
        msg[BUFSIZ - 1] = 0x00;

        if (!strcmp(msg, "Q\n") || !strcmp(msg, "q\n")){
            break;
        }

        printf("before sendto the msg is: %s ", msg);
        sendto(fd_clnt, &msg, BUFSIZE, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        printf("after sendto the msg is: %s \n", msg);
        socklen_t recv_cnt = 0;
        recvfrom(fd_clnt, msg, BUFSIZE - 1, 0, (struct sockaddr*)&serv_addr, &recv_cnt);
        printf("after recvfrom the msg is : %s\n", msg);
        msg[BUFSIZE - 1] = 0x00;

    }
    close(fd_clnt);
    return;
}