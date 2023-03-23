#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 4096

void createEchoServer(const char* port);
void errorHandling(const char* msg);

int main(int argc, char* argv[]){
    if (argc != 2){
        printf("usage: %s <port> \n", argv[0]);
    }
    createEchoServer(argv[1]);
    exit(1);
}
void createEchoServer(const char* port){
    int  serv_sock, clnt_sock;
    char message[BUFSIZE];
    int  str_len, i;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t          clnt_adr_len;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        errorHandling("socket() error");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port        = htons(atoi(port));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
        errorHandling("bind error");
    }
    if (listen(serv_sock, 5) == -1) {
        errorHandling("listen() error");
    }
    clnt_adr_len = sizeof(clnt_addr);
    for (int i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_len);
        if (clnt_sock == -1) {
            errorHandling("accrpt() error");
        }
        else {
            printf("connect client %d \n", i + 1);
        }

        while ((str_len = read(clnt_sock, message, BUFSIZE)) > 0) {
            write(clnt_sock, message, str_len);
        }
        close(clnt_sock);
    }
}
void errorHandling(const char* msg){
   fputs(msg, stderr); 
   fputc('\n', stderr);
   exit(1);
}
