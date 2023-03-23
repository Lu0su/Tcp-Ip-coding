#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <iostream>
//its already all 0
//buf --- 1024B
char buf[1024];
void error_handing(const char* msg); // processing error
void copy(const char* sFile, const char* dFile);//copy
void readToBuf(const char* fileName); //read
void writeFromBuf(const char* fileName);//write

int main(void){
    char sFile[26];
    char dFile[26];
    //memset(buf, 0, 1024);
    //for (int i = 0; i < 1024; ++i){
    //    printf("%d ", buf[i]);
    //}
    printf("please input the sFile and dFile you want:");
    scanf("%s %s", sFile, dFile);
    copy(sFile, dFile);
}


void error_handing(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
void readToBuf(const char* fileName){
    int  fd = open(fileName, O_RDONLY);
    if (fd == -1){
        error_handing("ropen() error");
    }
    if (read(fd, buf, 1024) == -1){
        error_handing("rread() error");
    }
    close(fd);
    return;
}
void writeFromBuf(const char* fileName){
    int fd = open(fileName, O_WRONLY);
    if (fd == -1){
        error_handing("wopen() error");
    }
    //why it is full of @ even if i only put 12 char
    if (write(fd, buf, 12) == -1){
        error_handing("wwrite() error");
    }
    close(fd);
    return;
}
void copy(const char* sFile, const char* dFile){
    readToBuf(sFile);
    writeFromBuf(dFile);
    return;
}
