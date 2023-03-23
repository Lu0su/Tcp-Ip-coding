#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSIZE 30

int main(int argc, char* argv[]){
    if (argc != 2){
        exit(-1);
    }

    int fd_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd_sock == -1){
        fputs("socket() error", stderr);
        exit(1);
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(fd_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        fputs("bind() error", stderr);
        exit(1);
    }
    

    char msg[BUFSIZE];
    //fgets(msg, BUFSIZE - 1, stdin);
    msg[BUFSIZE - 1] = 0x00;
    for (int i = 0; i < 3; ++i){
        sleep(5);
        //每次都要更新addr_len，作为addr的长度传入.
        socklen_t addr_len = sizeof(serv_addr);
        //BUFSIZE 写成了 BUFSIZ 运行愣是一下午没找到这个错误.
        //recvfrom在出现为问题是会阻塞在此位置,此时应该先去检测sendto查看是否是没有发送成功
        //如果没有发送成功，sendto的返回值将是-1
        recvfrom(fd_sock, msg, BUFSIZE - 1, 0, (struct sockaddr*)&serv_addr, &addr_len);
        fputs(msg, stdout);
    }

    close(fd_sock);
    exit(0);

}