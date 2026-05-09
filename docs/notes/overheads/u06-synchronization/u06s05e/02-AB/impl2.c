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

  sa = (sem_t *) malloc (sizeof(sem_t));
  sb = (sem_t *) malloc (sizeof(sem_t));
  me = (sem_t *) malloc (sizeof(sem_t));
  sem_init (sa, 0, 1);
  sem_init (sb, 0, 2);
  sem_init (me, 0, 1);
  
  setbuf(stdout, 0);

  pthread_create (&th, NULL, TA, NULL);
  pthread_create (&th, NULL, TB, NULL);
  
  pthread_exit(0);
}

static void *TA (void *arg) {
  pthread_detach (pthread_self ());

  while (1) {
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
  pthread_detach (pthread_self ());

  while (1) {
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
	

