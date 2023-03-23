//add 32bit int directly into the sockaddr_in 's in_addr struct
//c struct can be directly copy and the array can not do that
//so we use struct to packing an array
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <memory.h>

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

void packing_ip(const char* ip, struct sockaddr_in* addr_inet_ptr){
    if (!inet_aton(ip, &addr_inet_ptr->sin_addr)){
        error_handling("conversion error");
    } else {
        printf("network ordered integer addr: %#x \n", addr_inet_ptr->sin_addr.s_addr);
    }
    return;
}

//inet_ntoa return a char* ptr to a static buf which means the first ip will be overwrite
//after second call, so you need to copy the first string to a buf
void unpacking_ip(struct sockaddr_in* addr_inet_ptr){
    char* ip_ptr;
    char ip[32];
    memset(ip, 0, 32);
    ip_ptr = inet_ntoa(addr_inet_ptr->sin_addr);
    memcpy(ip, ip_ptr, strlen(ip_ptr) + 1);
    printf("ip string is: %s \n", ip);
}


int main(void){
    struct sockaddr_in addr_inet;
    const char* ip = "127.232.124.79";

    packing_ip(ip, &addr_inet);
    unpacking_ip(&addr_inet);

    exit(1);
}
