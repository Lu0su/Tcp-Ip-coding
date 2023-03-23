#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 4096

void error_handling(const char* msg);
void create_client(const char* ip, const char* port);

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("usage : %s <ip> <port>", argv[0]);
        exit(1);
    }
    create_client(argv[1], argv[2]);
    exit(1);
}

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

void create_client(const char* ip, const char* port){
    int fd;
    char message[BUFSIZE];
    int str_len;
    struct sockaddr_in sock_addr;

    fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd == -1){
        error_handling("socket() error");
    }

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family      = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(ip);
    sock_addr.sin_port        = htons(atoi(port));

    if ((connect(fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr))) == -1) {
        error_handling("connect() error");
    }
    else {
        printf("connected....");
    }

    while (1) {
        fputs("Input message(q to quit): ", stdout);
        fgets(message, BUFSIZE, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }

        write(fd, message, strlen(message));
        str_len          = read(fd, message, BUFSIZE - 1);
        message[str_len] = '\0';
        printf("message form server : %s", message);
    }
    close(fd);
    return;
}
