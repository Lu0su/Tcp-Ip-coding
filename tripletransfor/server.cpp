#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 4096
#define INTSIZE sizeof(int)

int get_string_length(char* str);
void error_handling(const char* msg);
void create_server(const char* port);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        error_handling("Usage: <port>");
    }

    create_server(argv[1]);
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

void create_server(const char* port){
    int fd_serv;
    fd_serv = socket(PF_INET, SOCK_STREAM, 0);
    if (fd_serv == -1){
        error_handling("socket() error");
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons(atoi(port));

    if (bind(fd_serv, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(fd_serv, 5) == -1){
        error_handling("listen() error");
    }

    for (int i = 0; i < 5; ++i){
        int fd_clnt;
        struct sockaddr_in clnt_addr;
        socklen_t clnt_size = sizeof(clnt_addr);
        if ((fd_clnt = accept(fd_serv, (struct sockaddr*)&clnt_addr, &clnt_size)) == -1){
            error_handling("accept() error");
        } else {
            printf("connect....\n");
        }

        int str_len = 0;
        read(fd_clnt, (char*)&str_len, INTSIZE);
        printf("str_len = %d \n", str_len);

        char msg[BUFSIZE];
        int read_str_cnt = 0;
        int read_str_total = 0;

        while (str_len > read_str_total){
            read_str_cnt = read(fd_clnt, &msg[read_str_total], BUFSIZ - 1);
            read_str_total += read_str_cnt;
        }
        printf("read from the client: %s \n", msg);

        //write something back
        fputs("please write something back: \n", stdout);
        // scanf("%s", msg);
        fgets(msg, 20, stdin);

        int write_str_len = 20;
        write(fd_clnt, (char* )&write_str_len, INTSIZE);
        write(fd_clnt, (char* )msg, write_str_len);
        close(fd_clnt);
    }
    return;
}