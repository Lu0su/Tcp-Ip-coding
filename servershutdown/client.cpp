#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 4096

void error_handling(const char* msg);
void create_client(const char* ip, const char* port);

int main(int argc, char* argv[]){
    if (argc != 3){
        error_handling("usage: <ip> <port>");
    }

    create_client(argv[1], argv[2]);
    exit(0);
}

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}

void create_client(const char* ip, const char* port){
    int fd_clnt = socket(PF_INET, SOCK_STREAM, 0);
    if (fd_clnt == -1)
        error_handling("socket() error");
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(atoi(port));

    if (connect(fd_clnt, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    
    int fd_file = open("recv.txt", O_WRONLY | O_CREAT | O_APPEND, 0777);
    if (fd_file == -1)
        error_handling("open() error");

    char msg[BUFSIZE];
    while(1){
        int read_cnt = read(fd_clnt, msg, BUFSIZE);
        if (read_cnt == -1)
            error_handling("read() error");
        
        int write_to_file = write(fd_file, msg, read_cnt);
        if (write_to_file == -1)
            error_handling("write() error");
        else if(write_to_file < BUFSIZE)
            break;
    }

    const char* end = "Already get the file, thank you";
    write(fd_clnt, end, strlen(end));

    close(fd_file);
    close(fd_clnt);
    
    return;
}