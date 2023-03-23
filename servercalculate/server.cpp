#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>

#define BUFSIZE 4096
#define OPSZ = 4 

void createEchoServer(const char* port);
void errorHandling(const char* msg);
int calculate(int opnum, int* opnds, char op); 

int main(int argc, char* argv[]){
    if (argc != 2){
        printf("usage: %s <port> \n", argv[0]);
    }
    createEchoServer(argv[1]);
    exit(1);
}

void createEchoServer(const char* port){
    int  serv_sock, clnt_sock;
    char opinfo[BUFSIZE];
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

    int recv_len= 0;
    int result = 0;
    clnt_adr_len = sizeof(clnt_addr);
    for (int i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_len);
        if (clnt_sock == -1) {
            errorHandling("accrpt() error");
        }
        else {
            printf("connect client %d \n", i + 1);
        }

        //整体思想其实还是保证每次处理是的字符数量是确定的
        //这里用第一个字符作为边界条件，指出需要接受的字符数量.

        //client 会首先传递操作数的数量,我们将它读入opnd_cnt中
        //一个操作数占用4字节
        int opnd_cnt = 0;
        read(clnt_sock, &opnd_cnt, 1); //最大可接收字节数为1B

        //接受的总字节数
        int recv_cnt = 0;
        //需要接受的总空间大小，一个int占用4字节，最后一个字符占用1字节
        while ((opnd_cnt * 4 + 1) > recv_len){
            //每次循环结束，更新recv_len
            recv_cnt = read(clnt_sock, &opinfo[recv_len], BUFSIZ - 1);
            recv_len += recv_cnt;
        }
        //交给calculate处理.
        result = calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len - 1]);
        write(clnt_sock, (char*)&result, sizeof(result));
        close(clnt_sock);
    }
    close(serv_sock);
    return;
}
void errorHandling(const char* msg){
   fputs(msg, stderr); 
   fputc('\n', stderr);
   exit(1);
}

int calculate(int opnum, int* opnds, char op){
    int result = opnds[0];
    switch (op){
        case '+':
        for (int i = 1; i < opnum; i++) result += opnds[i];
        break;
        case '-':
        for (int i = 1; i < opnum; i++) result -= opnds[i];
        break;
        case '*':
        for (int i = 1; i < opnum; i++) result *= opnds[i];
        break;
    }

    return result;
}
