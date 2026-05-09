#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "pthread.h"
#include "semaphore.h"

#define N 20

typedef struct rw_s {
  int nr;
  sem_t semR;
  sem_t semW;
  sem_t w;
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

  rw = (rw_t *) malloc (1 * sizeof(rw_t));
  
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

    sem_destroy(&rw->semR);
    sem_destroy(&rw->semW);
    sem_destroy(&rw->w);
  free (rw);
  
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
  rw->nr = 0;
    sem_init(&rw->semR, 0, 1); // Initialize reader semaphore
    sem_init(&rw->semW, 0, 1); // Initialize writer semaphore
    sem_init(&rw->w, 0, 1);    // Initialize critical section semaphore
 
  return;
}

static void *reader(void *arg) {
  int *p = (int *) arg;
  int i = *p;
  struct timeval t1, delta_time;
  struct timespec tim;
  
  tim.tv_sec = 0;
  tim.tv_nsec = rand() & 500000;
  nanosleep (&tim, NULL);
  gettimeofday (&t1,NULL);
  timeval_subtract(&delta_time, &t1, &t0);
  printf ("%d trying to read at time %ld and %ld\n", i, delta_time.tv_sec, delta_time.tv_usec);

  sem_wait (&rw->semR);
  rw->nr++;
  if (rw->nr == 1)
    sem_wait (&rw->w);
  sem_post (&rw->semR);
  
  gettimeofday (&t1,NULL);
  timeval_subtract(&delta_time, &t1, &t0);
  printf("Thread %d reading  at time %ld and %ld\n", i, delta_time.tv_sec, delta_time.tv_usec); 
  tim.tv_nsec = rand() & 500000;
  nanosleep (&tim, NULL);

  sem_wait (&rw->semR);
  rw->nr--;
  if (rw->nr == 0)
    sem_post (&rw->w);
  sem_post (&rw->semR);

  pthread_exit (NULL);
}

static void *writer(void *arg) {
  int *p = (int *) arg;
  int i = *p;
  struct timeval t1, delta_time;
  struct timespec tim;

  tim.tv_sec = 0;
  tim.tv_nsec = rand() & 500000;
  nanosleep (&tim, NULL);
  gettimeofday (&t1,NULL);
  timeval_subtract(&delta_time, &t1, &t0);
  printf ("%d trying to write at time %ld and %ld\n", i, delta_time.tv_sec, delta_time.tv_usec);

  sem_wait (&rw->semW);
  sem_wait (&rw->w);
  
  gettimeofday (&t1,NULL);
  timeval_subtract(&delta_time, &t1, &t0);
  printf("Thread %d writing  at time %ld and %ld\n", i, delta_time.tv_sec, delta_time.tv_usec); 
  tim.tv_nsec = rand() & 500000;
  nanosleep (&tim, NULL);

  sem_post (&rw->w);
  sem_post (&rw->semW);

  pthread_exit (NULL);
}

