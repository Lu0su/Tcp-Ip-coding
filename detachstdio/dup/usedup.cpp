#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int fd = open("newfile.txt", O_RDWR | O_CREAT, 0777);
    if (fd == -1) exit(-1);

    int copy_fd = dup(fd);
    close(fd);
    char buf[] = "hello my name is luosu";
    //printf("buf sizeof is : %d, buf strlen is: %d \n", sizeof(buf), strlen(buf));
    write(copy_fd, buf, strlen(buf) + 1); //如果写入了0就会成为二进制文件.
    close(copy_fd);
    return 0;
}