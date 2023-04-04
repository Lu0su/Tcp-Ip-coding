#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void func() {
    //确实，只有用rw打开的fd才能生存写入的文件指针
    int fd = open("newtext.txt", O_RDWR| O_CREAT, 0777);
    FILE* writefp = fdopen(fd, "w");

    char msg[] = "hello my name is luosu";
    fputs(msg, writefp);
}

int main(){
    func();
    exit(0);
}