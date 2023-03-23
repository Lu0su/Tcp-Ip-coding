#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>   //read write

#define BUFSIZE 4096
#define INTSIZE sizeof(int)


int get_string_length(char* str);
void error_handling(const char* msg);
void create_client(const char* ip, const char* port);

int main(int argc, char* argv[])
{
    if (argc != 3) {
        error_handling("Usage: <ip> <port>");
    }

    create_client(argv[1], argv[2]);
    exit(1);
}


int get_string_length(char* str){
    char* ptr = str;
    int length = 0;
    while (*ptr != '\n'){
        ++length;
    }

    return length;
}


void error_handling(const char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}

void create_client(const char* ip, const char* port)
{
    int fd_clnt = socket(PF_INET, SOCK_STREAM, 0);
    if (fd_clnt == -1) {
        error_handling("socket() error");
    }

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port        = htons(atoi(port));

    if (connect(fd_clnt, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    }

    char msg[BUFSIZE];
    fputs("please input the string you want to transfor: \n", stdout);
    // scanf("%s", msg);
    fgets(msg, 20, stdin);

    printf("msg: %s \n", msg);
    int str_len = 20; 
    printf("str_len: %d \n", str_len);

    write(fd_clnt, (const char*)&str_len, INTSIZE);
    write(fd_clnt, &msg, str_len);

    int read_str_len   = 0;
    int read_str_cnt   = 0;
    int read_str_total = 0;
    memset(msg, 0, BUFSIZE);
    read(fd_clnt, (char*)&read_str_len, INTSIZE);

    while (read_str_len > read_str_total) {
        read_str_cnt = read(fd_clnt, &msg[read_str_cnt], BUFSIZE - 1);
        read_str_total += read_str_cnt;
    }
    printf("get message from server: %s \n", msg);
}