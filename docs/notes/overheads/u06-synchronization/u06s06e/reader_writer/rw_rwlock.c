#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "pthread.h"
#include "semaphore.h"

#define N 20

pthread_rwlock_t rw;
struct timeval t0;

static void *reader (void *);
static void *writer (void *);
  
int main (void) {
  pthread_t th_a[N], th_b[N];
  int i, v[N];
  
  setbuf (stdout, NULL);

  pthread_rwlock_init (&rw, NULL);
  gettimeofday (&t0,NULL);
  for (i=0; i<N; i++){
    v[i] = i;
    pthread_create (&th_a[i], NULL, reader, (void *) &v[i]);
    pthread_create (&th_b[i], NULL, writer, (void *) &v[i]);
  }

    // Join threads
    for (i=0; i<N; i++) {
        pthread_join(th_a[i], NULL); // Wait for reader thread to finish
        pthread_join(th_b[i], NULL); // Wait for writer thread to finish
    }

    pthread_rwlock_destroy(&rw); // Destroy the read-write lock
  
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

  pthread_rwlock_rdlock (&rw);  
  
  gettimeofday (&t1,NULL);
  timeval_subtract(&delta_time, &t1, &t0);
  printf("Thread %d reading  at time %ld and %ld\n", i, delta_time.tv_sec, delta_time.tv_usec); 
  tim.tv_nsec = rand() & 500000;
  nanosleep (&tim, NULL);

  pthread_rwlock_unlock (&rw);    

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

  pthread_rwlock_wrlock (&rw);
  
  gettimeofday (&t1,NULL);
  timeval_subtract(&delta_time, &t1, &t0);
  printf("Thread %d writing  at time %ld and %ld\n", i, delta_time.tv_sec, delta_time.tv_usec); 
  tim.tv_nsec = rand() & 500000;
  nanosleep (&tim, NULL);

  pthread_rwlock_unlock (&rw);

  pthread_exit (NULL);
}

