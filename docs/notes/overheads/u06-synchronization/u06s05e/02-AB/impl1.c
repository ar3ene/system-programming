// Two B and one A

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pthread.h"
#include "semaphore.h"

sem_t *sa, *sb, *me;
int n;

static void *TA (void *);
static void *TB (void *);
  
int main (int argc, char **argv) {
  pthread_t th;
  int n1, n2;

  if (argc != 2) {
    fprintf (stderr, "Syntax: %s num_threads\n", argv[0]);
    return (1);
  }
  n1 = atoi(argv[1]);
  n2 = 2 * n1;
  sa = (sem_t *) malloc (sizeof(sem_t));
  sb = (sem_t *) malloc (sizeof(sem_t));
  me = (sem_t *) malloc (sizeof(sem_t));
  sem_init (sa, 0, 1);
  sem_init (sb, 0, 2);
  sem_init (me, 0, 1);
  
  setbuf(stdout, 0);

  pthread_create (&th, NULL, TA, &n1);
  pthread_create (&th, NULL, TB, &n2);
  
  pthread_exit(0);
}

static void *TA (void *arg) {
  int i, *n_cycle;

  n_cycle = (int *) arg;
  pthread_detach (pthread_self ());

  for (i=0; i<*n_cycle; i++) {
    sleep (rand()%2);
    sem_wait (sa);
    sem_wait (me);
    printf ( "A");
    n++;
    if (n>=3) {
      printf ("\n");
      n = 0;
      sem_post (sa);
      sem_post (sb);
      sem_post (sb);
    }
    sem_post (me);
  }
  
  return 0;
}  

static void *TB (void *arg) {
  int i, *n_cycle;

  n_cycle = (int *) arg;
  pthread_detach (pthread_self ());

  for (i=0; i<*n_cycle; i++) {
    sleep (rand()%2);
    sem_wait (sb);
    sem_wait (me);
    printf ("B");
    n++;
    if (n>=3) {
      printf ("\n");
      n = 0;
      sem_post (sa);
      sem_post (sb);
      sem_post (sb);
    }
    sem_post (me);
  }
  
  return 0;
}  
	

