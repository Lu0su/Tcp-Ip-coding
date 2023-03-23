#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFSIZE 4096
#define INSIZE 4
#define FILENAMESIZE 100

void error_handling(const char* msg);
void create_client(const char* ip, const char* port);
void write_to_file(const char* filename, char* buf, int size);

int main(int argc, char* argv[]){
    if (argc != 3){
        error_handling("usage: <ip> <port>");
    }

    create_client(argv[1], argv[2]);
    exit(1);
}


void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}
void create_client(const char* ip, const char* port){
    int fd_clnt = socket(PF_INET, SOCK_STREAM, 0);
    if (fd_clnt == -1){
        error_handling("socket() error");
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(atoi(port));

    if (connect(fd_clnt, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        error_handling("connect() error");
    }

    fputs("please enter the file name you need: ", stdout);
    
    char filename[FILENAMESIZE];
    fgets(filename, FILENAMESIZE, stdin); 
    
    write(fd_clnt, &filename, FILENAMESIZE); //the name 

    char buf[BUFSIZ];

    int read_cnt = 0;
    while ((read_cnt = read(fd_clnt, buf, BUFSIZE)) > 0){
        write_to_file("client.txt", buf, read_cnt);
    }

    close(fd_clnt);
}
void write_to_file(const char* filename, char* buf, int size){
    int fd = open(filename, O_CREAT|O_WRONLY|O_APPEND, 0777); 
    if (fd == -1){
        error_handling("open() error");
    }

    write(fd, buf, size);
    close(fd);
}