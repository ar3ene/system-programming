/*
 * Bubble Sort with 2 barriers
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/timeb.h>

typedef enum {
  false, true
} boolean;

typedef struct {
  int count;
  pthread_mutex_t mutex;
} Counter;

Counter *c;
int num_threads;
int vet_size;
int *vet;
boolean sorted = false;
boolean all_ok = true;
pthread_barrier_t *barrier1;
sem_t *barrier2;
pthread_mutex_t *me;

static int max_random (int);
void *sorter (void *);
  
int main (int argc, char **argv) {
  pthread_t *th;
  int i, *pi;
  struct timeb tb;

  if (argc != 2) {
    printf ("Syntax: %s vet_size\n", argv[0]);
    return (1);
  }
    
  setbuf(stdout,0);
  vet_size = atoi (argv[1]);
  num_threads = vet_size - 1;
  vet = (int *) malloc (vet_size * sizeof (int));

  th = (pthread_t *) malloc (num_threads * sizeof (pthread_t));
  c = (Counter *) malloc (sizeof (Counter));
  c->count = 0;
  pthread_mutex_init (&c->mutex, NULL);
  barrier1 = (pthread_barrier_t *) malloc (sizeof (pthread_barrier_t));
  pthread_barrier_init (barrier1, NULL, vet_size-1);
  barrier2 = (sem_t *) malloc (sizeof (sem_t));
  sem_init (barrier2, 0, 0);
  me = (pthread_mutex_t *) malloc (vet_size * sizeof (pthread_mutex_t));
  for (i = 0; i < vet_size; i++)
    pthread_mutex_init (&me[i], NULL);

  ftime (&tb);
  srand (tb.millitm * getpid ());
  //srand (tb.millitm);
  for (i = 0; i < vet_size; i++) {
    vet[i] = max_random (1000);
    printf ("%d ", vet[i]);
  }
  printf ("\n");

  /* Create the threads */
  for (i = 0; i < num_threads; i++) {
    pi = (int *) malloc (sizeof (int));
    *pi = i;
    pthread_create (&th[i], NULL, sorter, pi);
  }

  //pthread_barrier_destroy (barrier1);
  //free (barrier1);

  pthread_exit (0);
}

void *sorter (void *arg) {
  int *ip, i, j, tmp;

  ip = (int *) arg;
  i = *ip;

  pthread_detach (pthread_self ());

  while (!sorted) {
    pthread_mutex_lock (&me[i]);
    pthread_mutex_lock (&me[i + 1]);
    if (vet[i] > vet[i + 1]) {
      fprintf (stdout, "Thread %d: vet %d %d; swap\n", i, vet[i], vet[i+1]);
      tmp = vet[i];
      vet[i] = vet[i + 1];
      vet[i + 1] = tmp;
      all_ok = false;
    } else {
      fprintf (stdout, "Thread %d: vet %d %d; no swap\n", i, vet[i], vet[i+1]);
    }
    pthread_mutex_unlock (&me[i + 1]);
    pthread_mutex_unlock (&me[i]);

    /* Barrier 1 */
    pthread_barrier_wait (barrier1);

    /* Barrier 2 */
    pthread_mutex_lock (&c->mutex);
    c->count++;
    if (c->count == num_threads) {
      fprintf (stdout, "all_ok %d\n", all_ok);
      for (j=0; j < vet_size; j++)
        fprintf (stdout, "%d ", vet[j]);
      fprintf (stdout, "\n");
      if (all_ok)
        sorted = true;
      all_ok = true;
      for (j=0; j<num_threads; j++) {
        sem_post (barrier2);
      }
      c->count = 0;
    }
    pthread_mutex_unlock (&c->mutex);
    sem_wait (barrier2);
  }
  return 0;
}


static int max_random (int max) {
  return (int) (random () % max);
}
