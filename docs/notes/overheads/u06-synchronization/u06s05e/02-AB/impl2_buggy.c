// Two B and one A
// Buggy: A and B can start in parallel

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
  int i, num_threads, *pi;

  if (argc != 2) {
    fprintf (stderr, "Syntax: %s num_threads\n", argv[0]);
    return (1);
  }
  num_threads = atoi(argv[1]);
  sa = (sem_t *) malloc (sizeof(sem_t));
  sb = (sem_t *) malloc (sizeof(sem_t));
  me = (sem_t *) malloc (sizeof(sem_t));
  sem_init (sa, 0, 1);
  sem_init (sb, 0, 2);
  sem_init (me, 0, 1);
  
  setbuf(stdout, 0);
  
  for (i=0; i<2*num_threads; i++){
    pi = (int *) malloc (sizeof(int));
    *pi = i;
    pthread_create (&th, NULL, TB, pi);
  }
  for (i=0;i<num_threads;i++){
    pi = (int *) malloc(sizeof(int));
    *pi=i;
    pthread_create (&th, NULL, TA, pi);
  }
  
  pthread_exit(0);
}

static void *TA (void *arg) {
  int id;
  int *pi = (int *) arg;
  id = *pi;
	
  pthread_detach (pthread_self ());

  sleep (1);
  sem_wait (sa);
  printf ( "A%d ", id);
  sem_wait (me);
  n++;
  if (n>=3) {
    printf ("\n");
    n = 0;
    sem_post (sa);
    sem_post (sb);
    sem_post (sb);
  }
  sem_post (me);
  
  return 0;
}  

static void *TB (void *arg) {
  int id;
  int *pi = (int *) arg;
  id = *pi;
  
  pthread_detach (pthread_self ());

  sleep (1);
  sem_wait (sb);
  printf ("B%d ", id);
  sem_wait (me);
  n++;
  if (n>=3) {
    printf ("\n");
    n = 0;
    sem_post (sa);
    sem_post (sb);
    sem_post (sb);
  }
  sem_post (me);
  
  return 0;
}  
	

