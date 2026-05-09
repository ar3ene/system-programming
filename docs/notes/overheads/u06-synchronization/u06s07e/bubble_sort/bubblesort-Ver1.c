/*
 * Bubble Sort with 1 semaphore for each thread
 */

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/timeb.h>

typedef enum {
  false, true
} boolean;

// Global variables
int num_threads;
int vet_size;
int *vet;
boolean sorted = false;
boolean all_ok = false;
sem_t semMaster;
sem_t *semSlave;
pthread_mutex_t *me;

// Function prototypes
static int max_random (int);
void *master (void *);
void *slave (void *);
  
int main (int argc, char **argv) {
  pthread_t thMaster, *thSlave;
  int i, *id;
  struct timeb tb;

  if (argc != 2) {
    printf ("Syntax: %s vet_size\n", argv[0]);
    return (1);
  }
    
  setbuf (stdout, 0);

  vet_size = atoi (argv[1]);
  num_threads = vet_size - 1;

  // Need to check for error on memory allocation
  vet = (int *) malloc (vet_size * sizeof (int));
  id = (int *) malloc (num_threads * sizeof (int));
  thSlave = (pthread_t *) malloc (num_threads * sizeof (pthread_t));
  semSlave = (sem_t *) malloc (num_threads * sizeof (sem_t));
  me = (pthread_mutex_t *) malloc (vet_size * sizeof (pthread_mutex_t));


  // Random array generation
  ftime (&tb);
  srand (tb.millitm * getpid ());
  //srand (tb.millitm);
  for (i=0; i<vet_size; i++) {
    vet[i] = max_random (1000);
  }
  for (i=0; i<vet_size; i++) {
    pthread_mutex_init (&me[i], NULL);
  }

  /* Create master thread */
  sem_init (&semMaster, 0, num_threads);
  pthread_create (&thMaster, NULL, master, &num_threads);

  /* Create slave threads */
  for (i=0; i<num_threads; i++) {
    id[i] = i;
    sem_init (&semSlave[i], 0, 0);
    pthread_create (&thSlave[i], NULL, slave, &id[i]);
  }

  /* Wait for all */
  for (i=0; i<num_threads; i++) {
    pthread_join (thSlave[i], NULL);
  }
  pthread_join (thMaster, NULL);

  /* Free memory and semaphores */
  free (vet);
  free (id);
  free (thSlave);
  sem_destroy (&semMaster);
  for (i=0; i<num_threads; i++) {
    pthread_mutex_destroy (&me[i]);
    sem_destroy (&semSlave[i]);
  }

  return (0);
}

void *master (void *arg) {
  int *ntp, nt, i;

  ntp = (int *) arg;
  nt = *ntp;
  
  while (!sorted) {
    /* Wait for all slaves */
    for (i=0; i<nt; i++) {
      sem_wait (&semMaster);
    }
    for (i=0; i<vet_size; i++) {
      fprintf (stdout, "%d ", vet[i]);
    }
    fprintf (stdout, "\n");
    if (all_ok) {
      sorted = true;
    } else {
      all_ok = true;
    }
    fprintf (stdout, "Thread Master: sorted %d\n", sorted);
    /* Wake-up all slaves */
    for (i=0; i<nt; i++) {
      sem_post (&semSlave[i]);
    }
  }

  pthread_exit (0);
}

void *slave (void *arg) {
  int *ip, i, tmp;

  ip = (int *) arg;
  i = *ip;

  while (1) {
    /* Wait for master */
    sem_wait (&semSlave[i]);

    if (sorted) {
       break;
    }
    
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

    /* Wake-up master */
    sem_post (&semMaster);
  }

  pthread_exit (0);
}

static int max_random (int max) {
  return (int) (random () % max);
}
