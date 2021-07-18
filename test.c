#include <stdio.h>

#include "thread-pool.h"

void counter(void* arg) {
    int* i = (int*) arg;
    printf("The %d task is running.\n", *i);
}

int main() {
    thread_pool_t pool;
    int ret;
    ret = thread_pool_create(&pool, 10);
    if(ret != 0) {
        printf("thread pool create failed\n");
        return -1;
    }
    for (int i=0;i < 1000;++i) {
        ret = thread_pool_push_task(&pool, counter, &i);
        if(ret != 0) {
            printf("task push failed\n");
            return -1;
        }
    }
    return 0;
}
