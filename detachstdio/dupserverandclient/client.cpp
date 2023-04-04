#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 4096

void error_handling(const char* msg);
void create_client(const char* ip, const char* port);

int main(int argc, char* argv[]) {
    if (argc != 3) error_handling("uasge: <ip> <port>");
    create_client(argv[1], argv[2]);
    return 0;
}

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}

void create_client(const char* ip, const char* port){
    int clnt_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (clnt_fd == -1) error_handling("socket() error");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(atoi(port));

    if (connect(clnt_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    else 
        puts("connected .....");

    FILE* readfp = fdopen(clnt_fd, "r");
    FILE* writefp = fdopen(clnt_fd, "w");
    char buf[BUF_SIZE];

    while (1){
        if (fgets(buf, BUF_SIZE, readfp) == nullptr)
            break;
        
        fputs(buf, stdout);
        fflush(stdout);
    }
    //在受到eof后推出循环，传输最后的信息.
    //如果是close关闭，在受到eof是无法将这句话传到server的.
    fputs("From client: Thandk you! \n", writefp);
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);
    return;
}