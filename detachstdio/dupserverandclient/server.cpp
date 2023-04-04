#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 4096

void error_handling(const char* msg);
void create_server(const char* port);

int main(int argc, char* argv[]) {
    if (argc != 2) error_handling("uasge:<port>");
    create_server(argv[1]);
    return 0;
}

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}

void create_server (const char* port){
    int serv_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_fd == -1) error_handling("socket() error");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(port));

    if (bind(serv_fd, (struct sockaddr* )&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_fd, 5) == -1)
        error_handling("listen() error");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    char buf[BUF_SIZE];
    for (int i = 0; i < 5; ++i){
        int clnt_fd = accept(serv_fd, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_fd == -1) error_handling("accept() error");

        FILE* readfp = fdopen(clnt_fd, "r");
        FILE* writefp = fdopen(clnt_fd, "w");

        fputs("From server: Hi~client? \n", writefp);
        fputs("I love alll of the world \n", writefp);
        fputs("You are awesone! \n", writefp);
        fflush(writefp);


        //本义是半关闭write流，向客户端发送eof
        //输入段依然可用(read)

        //但是writefp与readfp对应相同的文件描述符
        //关闭其中一个就将所有的都关闭了.
        fclose(writefp);
        fgets(buf, BUF_SIZE, readfp);
        fputs(buf, stdout);
        fclose(readfp);
    }
    close(serv_fd);
    return;
}