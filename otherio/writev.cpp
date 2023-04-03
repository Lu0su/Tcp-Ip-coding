#include <stdio.h>
#include <sys/uio.h>

void write_to_consloe(){
    struct iovec vec[2];
    char buf1[] = "ABCDEFGH";
    char buf2[] = "1233452";

    //iov_len就是实际需要从buf中传送的数据量
    vec[0].iov_base = buf1;
    vec[0].iov_len = 3;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 4;

    //返回总共传送的数据量， 传送到目标文件是没有间隔符号的.
    //ABC1233 是这样的.
    int write_cnt = writev(1, vec, 2);
    putc('\n', stdout);
    printf("write bytes: %d \n", write_cnt);
    return;
};

int main(){
    write_to_consloe();
    return 0;
}