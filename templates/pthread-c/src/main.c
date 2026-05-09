#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N_THREADS 4
#define N_ITERS 100000

typedef struct {
    int id;
    long *counter;
    pthread_mutex_t *mutex;
} thread_arg_t;

static void *worker(void *arg) {
    thread_arg_t *a = (thread_arg_t *)arg;

    for (int i = 0; i < N_ITERS; ++i) {
        pthread_mutex_lock(a->mutex);
        (*a->counter)++;
        pthread_mutex_unlock(a->mutex);
    }

    printf("thread %d done\n", a->id);
    return NULL;
}

int main(void) {
    pthread_t threads[N_THREADS];
    thread_arg_t args[N_THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    long counter = 0;

    for (int i = 0; i < N_THREADS; ++i) {
        args[i].id = i;
        args[i].counter = &counter;
        args[i].mutex = &mutex;
        if (pthread_create(&threads[i], NULL, worker, &args[i]) != 0) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < N_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    printf("counter = %ld, expected = %ld\n", counter, (long)N_THREADS * N_ITERS);
    return EXIT_SUCCESS;
}
