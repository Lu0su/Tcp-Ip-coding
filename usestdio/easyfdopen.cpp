/**
 * 
 * 简单的是一哦嗯fdopen
 * 将文件描述符转化为文件指针
 * 
 * 简单的fileno
 * 将文件指针转化为文件描述符
 * 
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void test_for_fdopen();
void test_for_fileon();

int main(int argc, char* argv[]){
    test_for_fdopen();
    test_for_fileon();
    exit(0);
}

void test_for_fdopen(){
    int file_fd = open("newfile.txt", O_RDWR | O_CREAT, 0777);
    if (file_fd == -1) exit(-1);

    //将fd -> fp
    //一个fd可以转化为多个fp
    //失败返回NULL
    FILE* fp = fdopen(file_fd, "w");
    if (fp == NULL) exit(-1);

    fputs("Hello my name is Lusou", fp);

    fclose(fp);
    return;
}

void test_for_fileon(){
    FILE* fp = fopen("newfile.txt", "rb");
    if (fp == NULL) exit(-1);
    
    int fd = fileno(fp);
    char buf[1024];
    int read_cnt = 0;
    int read_total = 0;
    while ((read_cnt = read(fd, &buf[read_total], 1024 - 1)) > 0){
        read_total += read_cnt;
    }
    buf[read_total] = '\n';
    fputs(buf, stdout);

    return;
}

