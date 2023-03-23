#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 4096
#define FILENAMESIZE 100

void error_handling(const char* msg);
void create_server(const char* port);
bool search_or_read_file(int fd, const char* filename, char* buf);

int main(int argc, char* argv[]){
    if (argc != 2){
        error_handling("Usage: <port>");
    }

    create_server(argv[1]);
    exit(1);
}


void error_handling(const char* msg){
    fputs(msg, stderr);   
    fputc('\n', stderr);
    exit(1);
}

void create_server(const char* port){
    int fd_serv = socket(PF_INET, SOCK_STREAM, 0);
    if (fd_serv == -1){
        error_handling("socket() error");
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(port));    

    if (bind(fd_serv, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind() error");
    }

    if (listen(fd_serv, 5) == -1){
        error_handling("listen() error");
    }

    for (int i = 0; i < 5; ++i){
        struct sockaddr_in clnt_addr;
        socklen_t clnt_len;
        int fd_clnt = accept(fd_serv, (struct sockaddr*)&clnt_addr, &clnt_len);
        if (fd_clnt == -1){
            error_handling("accept() error");
        }

        char filename[FILENAMESIZE];
        read(fd_clnt, &filename, FILENAMESIZE);
        
        for (int i = 0; i < FILENAMESIZE; ++i){
            if (filename[i] == '\n'){
                filename[i] = 0x00;
            }
        }
        
        char buf[BUFSIZ];
        search_or_read_file(fd_clnt,filename, buf);
        close(fd_clnt);
    }
    close(fd_serv);
    return;
}

bool search_or_read_file(int fd_clnt, const char* filename, char* buf){
    //有可能是write把\n也写进去了, tmd 还真是\n惹的.
    int fd_file = open(filename, O_RDONLY, 0777);
    if (fd_file == -1){
        printf("the filename strlen is : %ld, there is no such file: %s \n", strlen(filename), filename);
        close(fd_file);
        return false;
    }
    
    int read_cnt = 0;
    while ((read_cnt = read(fd_file, buf, BUFSIZE)) > 0){
        write(fd_clnt, buf, read_cnt);
    }
    close(fd_file);
    return true;
}