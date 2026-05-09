/* The main trhead creates three threads.
   Two of the threads update a variable.
   The third thread waits until the  count variable reaches a specified value. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pthread.h"

#define NUM_THREADS  3
#define COUNT_LIMIT 12

typedef struct cond_s {
  pthread_mutex_t lock;  
  pthread_cond_t cond;
  int count;
} cond_t;

static void *inc_count (void *args) {
  cond_t *cond_d = (cond_t *) args;
  int stop = 0;
  
  while (!stop) {
    pthread_mutex_lock (&cond_d->lock);
    cond_d->count++;
    printf ("Increment count [T=%ld] (%d -> %d)\n",
      (long int) pthread_self(), cond_d->count-1, cond_d->count); 
    if (cond_d->count == COUNT_LIMIT) {
      pthread_cond_signal (&cond_d->cond);
      printf ("Threshold reached (count=%d).\n", cond_d->count);
    }
    if (cond_d->count >= COUNT_LIMIT) {
      stop = 1;
    }
    pthread_mutex_unlock (&cond_d->lock);

   sleep(1);
  }
  
  pthread_exit(0);
}

static void *watch_count (void *args) {
  cond_t *cond_d = (cond_t *) args;

  printf ("Starting watch on count\n");

  pthread_mutex_lock (&cond_d->lock);
  if (cond_d->count < COUNT_LIMIT) {
    pthread_cond_wait (&cond_d->cond, &cond_d->lock);
    printf ("Watch_count: Condition signal received.\n");
  }
  pthread_mutex_unlock (&cond_d->lock);

  pthread_exit(0);
}

int main () {
  cond_t cond_d;
  pthread_t tids[3];
  
  setbuf (stdout, 0);

  pthread_mutex_init (&cond_d.lock, NULL);
  pthread_cond_init (&cond_d.cond, NULL);
  cond_d.count = 0;
  
  pthread_create (&tids[0], NULL, inc_count,   (void *) &cond_d);
  pthread_create (&tids[1], NULL, inc_count,   (void *) &cond_d);
  pthread_create (&tids[2], NULL, watch_count, (void *) &cond_d);

  pthread_join (tids[0], NULL);
  pthread_join (tids[1], NULL);
  pthread_join (tids[2], NULL);
  
  pthread_exit(0);
}


