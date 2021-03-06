//
// Created by titto on 2021/7/18.
//

#include "thread-pool.h"

static void
thread_main_func(worker_t* worker) {
    while (1) {
        thread_pool_t* pool = worker->pool;
        if(pool == NULL) {
            break;
        }
        pthread_mutex_lock(&pool->mutex);
        while (TAILQ_EMPTY(&pool->task_queue) && (!pool->terminate)) {
            pthread_cond_wait(&pool->cond, &pool->mutex);
        }
        if(pool->terminate) {
            pthread_mutex_unlock(&pool->mutex);
            break;
        }
        task_t* task = TAILQ_LAST(&pool->task_queue, task_queue);
        TAILQ_REMOVE(&pool->task_queue, task, entry);
        pthread_mutex_unlock(&pool->mutex);
        task->func(task->arg);
        free(task);
    }
    pthread_exit(NULL);
}

thread_pool_t*
thread_pool_create(int thread_num) {
    thread_pool_t* pool = calloc(1, sizeof(thread_pool_t));
    int i;
    if(pool == NULL) {
        return NULL;
    }

    pool->workers = calloc(thread_num, sizeof(worker_t*));
    if(pool->workers == NULL) {
        return NULL;
    }

    TAILQ_INIT(&pool->task_queue);

    pool->thread_num = thread_num;
    pool->terminate = 0;

    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);

    for(i=0;i < thread_num;++i) {
        worker_t* worker = calloc(1, sizeof(worker_t));
        if(worker == NULL) {
            return NULL;
        }
        pool->workers[i] = worker;
        worker->idx = i;
        worker->pool = pool;
        pthread_create(&worker->tid, NULL,
                       (void *(*)(void *)) thread_main_func, worker);
    }
    return pool;
}

void
thread_pool_destroy(thread_pool_t* pool) {
    worker_t* worker = NULL;
    for(int i=0;i<pool->thread_num;++i) {
        worker = pool->workers[i];
        if(worker == NULL) continue;
        worker->pool = NULL;
        pthread_detach(worker->tid);
        free(worker);
    }
    free(pool);
}

int
thread_pool_push_task(thread_pool_t* pool, void (*func)(void*), void* arg) {
    task_t* task = calloc(1, sizeof(task_t));
    if(task == NULL) {
        return -1;
    }
    task->func = func;
    task->arg = arg;

    pthread_mutex_lock(&pool->mutex);
    TAILQ_INSERT_HEAD(&pool->task_queue, task, entry);
    pthread_mutex_unlock(&pool->mutex);
    pthread_cond_signal(&pool->cond);

    return 0;
}
