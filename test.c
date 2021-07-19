#include <stdio.h>
#include <unistd.h>
#include "thread-pool.h"

void counter(void* arg) {
    int i = (int) arg;
    printf("The %d task is running.\n", i);
}

int main() {
    ;int ret;
    thread_pool_t* pool = thread_pool_create(10);
    if(pool == NULL) {
        printf("thread pool create failed\n");
        return -1;
    }
    for (int i=0;i < 1000;++i) {
        ret = thread_pool_push_task(pool, counter, (void *) i);
        if(ret != 0) {
            printf("task push failed\n");
            return -1;
        }
    }
    sleep(2);
    return 0;
}
