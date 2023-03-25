#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 4096

void error_handling(const char* msg);
void create_server(const char* port);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        error_handling("usage: <port>");
    }

    create_server(argv[1]);
    exit(0);
}

void error_handling(const char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}

void create_server(const char* port)
{

    FILE* fp = fopen("../file.txt", "rb");
    if (fp == NULL) error_handling("fopen() error");

    int fd_serv = socket(PF_INET, SOCK_STREAM, 0);
    if (fd_serv == -1) error_handling("socket() error");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(atoi(port));

    if (bind(fd_serv, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(fd_serv, 5) == -1) error_handling("listen() error");

    socklen_t serv_addr_len = sizeof(serv_addr);
    int       fd_clnt       = accept(fd_serv, (struct sockaddr*)&serv_addr, &serv_addr_len);
    if (fd_clnt == -1) error_handling("accept() error");

    char msg[BUFSIZE];

    while (1) {
        //读BUFSIZE个元素，每个元素大小为1B
        int fread_cnt = fread(msg, 1, BUFSIZE, fp);
        if (BUFSIZE > fread_cnt) {
            int write_cnt = write(fd_clnt, msg, fread_cnt);
            if (write_cnt == -1) error_handling("write() error");
            break;
        }
        write(fd_clnt, msg, BUFSIZE);
    }

    shutdown(fd_clnt, SHUT_WR);
    memset(msg, 0, BUFSIZE);

    int read_cnt = 0;
    int read_total = 0;
    while (read_cnt = read(fd_clnt, &msg[read_total], BUFSIZE - 1) > 0
        && read_total < BUFSIZE){
        read_total += read_cnt;
    }
    msg[BUFSIZE - 1] = 0x00;

    fputs(msg, stdout);


    fclose(fp);
    close(fd_clnt);
    close(fd_serv);

    return;
}
