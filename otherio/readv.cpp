#include <stdio.h>
#include <sys/uio.h>

void read_from_stdin(){
    struct iovec vec[2];
    char buf1[10];
    char buf2[BUFSIZ];

    vec[0].iov_base = buf1;
    vec[0].iov_len = 5;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 10;

    //当然如果缓冲区中的数据比较少，那就按照比较少的读，而非指定的数据量读取
    int read_cnt = readv(0, vec, 2);
    printf("buf1 is : %s \n", buf1);
    printf("buf2 is : %s \n", buf2);
    printf("read_cnt is %d \n", read_cnt);

}

int main(){
    read_from_stdin();
    return 0;
}