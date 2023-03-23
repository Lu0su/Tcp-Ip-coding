#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 30
void error_handling(const char* msg);
void create_userver(const char* port);

int main(int argc, char* argv[]){
    if (argc != 2){
        error_handling("usage: <port>");
    }

    create_userver(argv[1]);
    exit(0);
}

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}

void create_userver(const char* port){
    int fd_serv = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd_serv == -1){
        error_handling("socket() error");
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(port));

    if (bind(fd_serv, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind() errror");
    }

    while (1){
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        char msg[BUFSIZE];
        int str_len = recvfrom(fd_serv, &msg, BUFSIZ, 0, 
            (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        sendto(fd_serv, msg, str_len, 0, (struct sockaddr*)&clnt_addr, 
            clnt_addr_size);
    }
    close(fd_serv);
    return;
}