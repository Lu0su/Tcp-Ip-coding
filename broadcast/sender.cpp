#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#define TTL 64
#define BUF_SIZE 1024

void error_handling(const char* msg);
void the_sender(const char* ip, const char* port);

int main(int argc, char* argv[]){
    if (argc != 3){
        error_handling("usage: <ip> <port>");
    }

    the_sender(argv[1], argv[2]);
    exit(0);
}

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr); 
    exit(-1);
}
void the_sender(const char* ip, const char* port){
    int fd_send = socket(PF_INET, SOCK_DGRAM, 0);
    if (fd_send == -1) error_handling("socket() error");

    struct sockaddr_in broad_addr;
    memset(&broad_addr, 0, sizeof(broad_addr));
    broad_addr.sin_family = AF_INET;
    //不过现在需要自己设置组播地址了，从D类地址中选择一个可用的就可以了.
    broad_addr.sin_addr.s_addr = inet_addr(ip);
    broad_addr.sin_port = htons(atoi(port));

    //相较于普通的udp传输，多了TTL的设置
    int time_live = TTL;
    setsockopt(fd_send, IPPROTO_IP, IP_MULTICAST_TTL, &time_live, sizeof(time_live));
    //在数据的发送方开启广播选项.
    int broad_opt = 1;
    setsockopt(fd_send, SOL_SOCKET, SO_BROADCAST, &broad_opt, sizeof(broad_opt));

    int fd_open = open("news.txt", O_RDONLY);
    if (fd_open == -1) error_handling("open() error");

    char msg[BUF_SIZE];
    int read_cnt;
    //设置多播地址为sendto地址信息
    while((read_cnt = read(fd_open, msg, BUF_SIZE)) > 0){
        sendto(fd_send, msg, read_cnt, 0, (struct sockaddr*)&broad_addr, sizeof(broad_addr));
        sleep(2);
    }
    
    close(fd_open);
    close(fd_send);
    return;
}