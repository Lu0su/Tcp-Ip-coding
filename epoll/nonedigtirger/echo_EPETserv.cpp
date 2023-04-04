#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 60

void error_handling(const char* msg);
void setnonblocksock(int fd);
void create_server(const char* port);

int main(int argc, char* argv[])
{
    if (argc != 2) error_handling("Usage: <port>");
    create_server(argv[1]);
    exit(0);
}

void error_handling(const char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}

//设置read与write为非阻塞的，无论是否存在书数据都不会阻塞
void setnonblocksock(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);             //获取fd的信息，注意是getfl，不是fd
    fd = fcntl(fd, F_SETFL, flag | O_NONBLOCK);   //设置fd的信息,看来是没有append选项
}
void create_server(const char* port)
{
    int serv_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_fd == -1) error_handling("socket() error");

    int                 ep_fd = epoll_create(EPOLL_SIZE);
    struct epoll_event* ep_events =
        (struct epoll_event*)malloc(sizeof(epoll_event) * EPOLL_SIZE);   //用于wait中
    struct epoll_event event;
    event.events  = EPOLLIN;
    event.data.fd = serv_fd;
    epoll_ctl(ep_fd, EPOLL_CTL_ADD, serv_fd, &event);


    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(atoi(port));

    if (bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_fd, 5) == -1) error_handling("listen() error");

    setnonblocksock(serv_fd);
    struct sockaddr_in clnt_addr;
    socklen_t          clnt_addr_len = sizeof(clnt_addr);
    char               buf[BUF_SIZE];
    while (1) {
        int event_cnt =
            epoll_wait(ep_fd, ep_events, EPOLL_SIZE, -1);   // timeout == -1,将一直等待直到发生事件
        if (event_cnt == -1) {
            error_handling("epoll_wait() error");
        }
        puts("return epoll_wait()");
        for (int i = 0; i < event_cnt; ++i) {
            int tmp_fd = ep_events[i].data.fd;
            //不用担心，这个复制出来的fd是同样的，并不产生指向同一个sock的多余套接字.
            // printf("tmp_fd = %d, get_fd = %d", tmp_fd, ep_events[i].data.fd);
            if (tmp_fd == serv_fd) {
                int clnt_fd = accept(serv_fd, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
                if (clnt_fd == -1) error_handling("accept() error");
                //添加clnt_fd到监听列表中
                setnonblocksock(clnt_fd);
                struct epoll_event clnt_event;
                clnt_event.events  = EPOLLIN | EPOLLET;
                clnt_event.data.fd = clnt_fd;
                epoll_ctl(ep_fd, EPOLL_CTL_ADD, clnt_fd, &clnt_event);
            }
            else {
                int read_cnt = 0;
                while (1) {
                    read_cnt = read(tmp_fd, buf, BUF_SIZE);
                    if (read_cnt == 0) {
                        epoll_ctl(ep_fd, EPOLL_CTL_DEL, tmp_fd, nullptr);
                        close(tmp_fd);
                        printf("close clnt_fd: %d \n", tmp_fd);
                        break;
                    }
                    else if (read_cnt == -1) {
                        break;
                    }
                    else {
                        //为何存在多余的输出？
                        //假设第一次的字符串长度为 hello\n, 第二次是 ni\n
                        //第二次后buf字符串为 ni\nlo\n,所有会有奇怪的显示.
                        printf("%s", buf);
                        write(tmp_fd, buf, read_cnt);
                    }
                }
            }
        }
    }
    close(serv_fd);   //先关闭serv_fd然后是ep_fd
    close(ep_fd);
    return;
}