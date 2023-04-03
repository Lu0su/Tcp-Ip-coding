#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 1024
void error_handling(const char* msg);
void the_recviver(const char* group, const char* port);

int main(int argc, char* argv[]){
    if (argc != 3){
        error_handling("usage: <group> <port>");
    }

    the_recviver(argv[1], argv[2]);
    exit(0);
}

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}
void the_recviver(const char* group, const char* port){
    int fd_recv = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd_recv == -1) error_handling("socket() errror");

    struct sockaddr_in recv_addr;
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    //这里的INADDR_ANY将本主机的随机一个ip分配给了传输套接字
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_addr.sin_port = htons(atoi(port));

    if (bind(fd_recv, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) == -1)
        error_handling("bind() error");

    struct ip_mreq join_addr;
    //当前主机需要加入的多播族地质
    join_addr.imr_multiaddr.s_addr = inet_addr(group);
    //选择需要加入多播族的ip(本机随便选一个),如果有本机器有多个ip地址，而分配的ip地址与分配给udp的不一样，会出错吧
    join_addr.imr_interface.s_addr = htonl(INADDR_ANY);

    //设置需要加入的多播组，这里要的是ip_mreq的地址.
    setsockopt(fd_recv, IPPROTO_IP, IP_ADD_MEMBERSHIP, &join_addr, sizeof(join_addr));

    char buf[BUF_SIZE];
    int read_cnt;
    //这里接受广播数据，不需要获取sender方的地址，后面全都是0
    while ((read_cnt = recvfrom(fd_recv, buf, BUF_SIZE - 1, 0, nullptr, 0)) > 0){
        buf[read_cnt] = 0;
        fputs(buf, stdout);
    }
    close(fd_recv);
    return;
}