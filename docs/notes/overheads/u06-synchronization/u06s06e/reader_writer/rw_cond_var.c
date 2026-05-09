#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "pthread.h"

#define N 20

typedef struct rw_s {
    int nr, nw;
    pthread_mutex_t lock;
    pthread_cond_t read;
    pthread_cond_t write;
} rw_t;

rw_t *rw;
struct timeval t0;

static void init (rw_t *);
static void *reader (void *);
static void *writer (void *);

int main (void) {
    pthread_t th_a[N], th_b[N];
    int i, v[N];

    setbuf (stdout, NULL);

    rw = (rw_t *) malloc (sizeof(rw_t));

    init (rw);
    gettimeofday (&t0,NULL);
    for (i=0; i<N; i++) {
        v[i] = i;
        pthread_create (&th_a[i], NULL, reader, (void *) &v[i]);
        pthread_create (&th_b[i], NULL, writer, (void *) &v[i]);
    }

    // Join threads
    for (i=0; i<N; i++) {
        pthread_join(th_a[i], NULL); // Wait for reader thread to finish
        pthread_join(th_b[i], NULL); // Wait for writer thread to finish
    }

    free (rw);

//    pthread_exit(NULL);
    return 0;
}

int
timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y){
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait. tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

static void init (rw_t *rw) {
    pthread_mutex_init (&rw->lock, NULL);
    pthread_cond_init (&rw->read, NULL);
    pthread_cond_init (&rw->write, NULL);
    rw->nr = 0;
    rw->nw = 0;
}

static void *reader (void *arg) {
    int *p = (int *) arg;
    int i = *p;
    struct timeval t1, delta_time;
    struct timespec tim;

    printf ("Thread reader %d starts.\n", i);

    tim.tv_sec = 0;
    tim.tv_nsec = rand() & 500000;
    nanosleep (&tim, NULL);
    gettimeofday (&t1,NULL);
    timeval_subtract(&delta_time, &t1, &t0);
    printf ("   Thread %2d trying to read at time %ld.%ld\n", i, delta_time.tv_sec, delta_time.tv_usec);
    pthread_mutex_lock (&rw->lock);
    rw->nr++;
    while (rw->nw > 0)
        pthread_cond_wait (&rw->read, &rw->lock);
    pthread_mutex_unlock (&rw->lock);

    gettimeofday (&t1,NULL);
    timeval_subtract(&delta_time, &t1, &t0);
    printf("      Thread %2d reading at time %ld.%ld\n", i, delta_time.tv_sec, delta_time.tv_usec);
    tim.tv_nsec = rand() & 500000;
    nanosleep (&tim, NULL);

    pthread_mutex_lock (&rw->lock);
    rw->nr--;
    pthread_mutex_unlock (&rw->lock);
    pthread_cond_broadcast (&rw->read);
    pthread_cond_broadcast (&rw->write);

    pthread_exit (NULL);
}

static void *writer(void *arg) {
    int *p = (int *) arg;
    int i = *p;
    struct timeval t1, delta_time;
    struct timespec tim;

    printf ("Thread writer %d starts.\n", i);

    tim.tv_sec = 0;
    tim.tv_nsec = rand() & 500000;
    nanosleep (&tim, NULL);
    gettimeofday (&t1,NULL);
    timeval_subtract(&delta_time, &t1, &t0);
    printf  ("   Thread %2d trying to write at time %ld.%ld\n", i, delta_time.tv_sec, delta_time.tv_usec);
    pthread_mutex_lock (&rw->lock);
    while (rw->nw > 0 || rw->nr > 0)
        pthread_cond_wait (&rw->write, &rw->lock);
    pthread_mutex_unlock (&rw->lock);

    pthread_mutex_lock (&rw->lock);
    rw->nw++;
    pthread_mutex_unlock (&rw->lock);

    gettimeofday (&t1,NULL);
    timeval_subtract(&delta_time, &t1, &t0);
    printf("      Thread %2d writing at time %ld.%ld\n", i, delta_time.tv_sec, delta_time.tv_usec);
    tim.tv_nsec = rand() & 500000;
    nanosleep (&tim, NULL);

    pthread_mutex_lock (&rw->lock);
    rw->nw--;
    pthread_mutex_unlock (&rw->lock);
    pthread_cond_broadcast (&rw->read);
    pthread_cond_broadcast (&rw->write);

    pthread_exit (NULL);
}

