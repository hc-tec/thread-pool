//
// Created by titto on 2021/7/18.
//

#ifndef THREAD_POLL_THREAD_POLL_H
#define THREAD_POLL_THREAD_POLL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <semaphore.h>
#include <sys/queue.h>

struct task {
    TAILQ_ENTRY(task) entry;
    void (*func)(void*);
    void* arg;
};

TAILQ_HEAD(task_queue, task);

struct worker {
    pthread_t tid;
    struct thread_pool* pool;
};

struct thread_pool {

    struct worker* workers;
    struct task_queue task_queue;

    int thread_num;
    int terminate;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

typedef struct task task_t;
typedef struct worker worker_t;
typedef struct thread_pool thread_pool_t;

int thread_pool_create(thread_pool_t* pool, int thread_num);

void thread_pool_destroy(thread_pool_t* pool);

int thread_pool_push_task(thread_pool_t* pool, void (*func)(void*), void* arg);

#endif //THREAD_POLL_THREAD_POLL_H
