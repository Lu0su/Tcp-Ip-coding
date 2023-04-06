#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_main(void* ptr_age) {
    int age = *(int*)ptr_age;
    printf("I am %d years old.", age);
    puts("thread end");
    return NULL;
}

void create_thread() {
    pthread_t p_id;
    int age = 234;
    //(线程id， 属性(null默认， 线程函数， 线程函数的参数)
    int res = pthread_create(&p_id, nullptr, thread_main, &age);
    //sleep位于unistd中
    sleep(10);    
    puts("main end");
}

int main(){
    create_thread();
    return 0;
}