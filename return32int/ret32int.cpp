//4B of char* to 32bit int
#include <stdio.h>
#include <arpa/inet.h>

void addr_handle(unsigned long int32_addr){
    if (int32_addr == INADDR_NONE)
        printf("error occured \n");
    else  
        printf("network ordered interger 32bit addr: %#lx \n", int32_addr); 
}
void func(){
    char* addr1 = "1.2.3.4";
    char* addr2 = "1.2.3.256";

    unsigned long int32_addr = inet_addr(addr1);
    addr_handle(int32_addr);
    int32_addr = inet_addr(addr2);
    addr_handle(int32_addr);
}

int main(void){
    func();

    return 0;
}
