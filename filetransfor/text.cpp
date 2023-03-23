#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
int main(void){
    int fd = open("server.txt\n", O_RDONLY);
    if (fd == -1){
        printf("filename is server.txt, and it has the length %d \n", strlen("server.txt"));
    } else {
        printf("file open successfully \n");
    }
}