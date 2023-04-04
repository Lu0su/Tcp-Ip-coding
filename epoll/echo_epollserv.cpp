#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE  50

void error_handling(const char* msg);
void craete_server(const char* port);

int main(int argc, char* argv[]){
    if (argc != 2) error_handling("Usage: <ip>");
    craete_server(argv[1]);
    return 0;
}

void error_handling(const char* msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}
void craete_server(const char* port) {
    int serv_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_fd == -1) error_handling("socket() error");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(port)); 

    if (bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_fd, 5) == -1 )error_handling("listen() error");

    //创建epoll例程
    int epfd = epoll_create(EPOLL_SIZE);
    //用于wati,指向动态分配的内存，用于保存发送的event, 相当于分配了一个epoll_event数组，长度为EPOLL_SIZE，用存放events
    struct epoll_event* ep_events = (struct epoll_event*)malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

    //用于ctl,注册监视信息
    //需要注意的是，注册监视信息，依然是使用结构体epoll_event
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = serv_fd;
    //(epoll的文件描述符, 监视对象的添加/删除/更改, 监听对象的文件描述符, 被监听的对象的事件类型)
    //明明在event已经有了data.fd为何还需要一个单独的位置表示监听对象的文件描述符.
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_fd, &event);

    char buf[BUF_SIZE];
    while (1) {
        //等待监听事件的发生.如果没有监听事件发送，将进入阻塞状态
        //监听到的事件将被放入ep_events队列中(可能data.fd是用在这的，不过ctl中顺便可用罢了)
        //返回值为监听的事件中发送的事件数目.
        int event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_cnt == -1) {
            puts("epoll_wait() error");
            break;
        }

        //遍历发生了的事件的队列
        for (int i = 0; i < event_cnt; ++i){
            //如果是客户端input事件发生,建立连接套接字.
            if (ep_events[i].data.fd == serv_fd){
                struct sockaddr_in clnt_addr;
                socklen_t clnt_addr_size = sizeof(clnt_addr);
                int clnt_fd = accept(serv_fd, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
                if (clnt_fd == -1) error_handling("accept() error");
                //注册,传输套接字，监听其input
                event.events = EPOLLIN;
                event.data.fd = clnt_fd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_fd, &event); //将clnt_fd的input事件加入监听队列中.
                printf("connect client: %d \n", clnt_fd);
            } else {
                //如果是clnt_fd套接字发送input事件，读取并返回数据.(echo)
                int read_cnt = read(ep_events[i].data.fd, buf, BUF_SIZE);
                if (read_cnt == 0) {
                    //受到eof后,关闭套接字，shutdown由客户端实现.
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, nullptr);
                    close(ep_events[i].data.fd);
                    printf("close client: %d \n", ep_events[i].data.fd);
                } else {
                    write(ep_events[i].data.fd, buf, read_cnt);
                }
            }
        }
    }
    close(serv_fd);
    close(epfd);
    return;
}