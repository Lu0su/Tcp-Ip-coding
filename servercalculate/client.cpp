#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 4098

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

    fputs("Operator count: ", stdout);
    int opnd_cnt;
    scanf("%d", &opnd_cnt);
    message[0] = (char)opnd_cnt;

    for (int i = 0; i < opnd_cnt; ++i){
        printf("Operand %d: ", i + 1);
        scanf("%d", (int*)&message[i * sizeof(int) + 1]);
    }
    fgetc(stdin);
    fputs("Opeator: ", stdout);
    scanf("%c", &message[opnd_cnt * sizeof(int) + 1]);
    write(fd_sock, message, opnd_cnt*sizeof(int) + 2); //开头和结尾还有其两字节.
    int result = 0;
    read(fd_sock, &result, sizeof(int)); //为何这里不需要判断已经读取到的字节数
    //因为只有4字节马?
    printf("Operation result: %d \n", result);

    close(fd_sock);
    return;
}