#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 1024

void error_handling(const char* msg);
void create_client(const char* ip, const char* port);

int main(int argc, char* argv[])
{
    if (argc != 3) {
        error_handling("Usage: %s <ip> <port>");
        exit(1);
    }
    create_client(argv[1], argv[2]);
    exit(1);
}

void error_handling(const char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

void create_client(const char* ip, const char* port)
{
    int                fd_sock;
    char               message[BUFSIZE];
    int                str_len, recv_len, recv_cnt;
    struct sockaddr_in serv_addr;

    fd_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (fd_sock == -1) {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port        = htons(atoi(port));

    if ((connect(fd_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) == -1) {
        error_handling("connect() error");
    }
    printf("connected.......");

//之前传输给服务器多少数据，就重新读入多少数据。
    while (1) {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUFSIZE, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            int write_cnt = write(fd_sock, message, strlen(message));
            shutdown(fd_sock, SHUT_WR);
            read(fd_sock, message, write_cnt);
            printf("last message is: %s", message);
            break;
        }
        
        str_len = write(fd_sock, message, strlen(message));

        recv_len = 0;
        while (recv_len < str_len){
            recv_cnt = read(fd_sock, &message[recv_cnt], BUFSIZE - 1);
            if (recv_cnt < 0){
                error_handling("read() error!");
            }
            recv_len += recv_cnt;
        }
        message[recv_len] = 0;
        printf("Message form server: %s", message);
    }
    close(fd_sock);
    return;
}