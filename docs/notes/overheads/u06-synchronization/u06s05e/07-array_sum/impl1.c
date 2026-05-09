#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 3

typedef struct {
  int *vet;
  sem_t *sem;
  int n;
  int id;
} args_t;

int array_sum (int *, int);
void *adder (void *);

int main (int argc, char *argv[]) {
  int i, n, *v;

  fprintf (stderr, "The size of the array must be aprower of 2.\n");  
  if (argc != 2) {
    fprintf (stderr, "Syntax: %s array_size\n", argv[0]);
    return (1);
  }
  n = atoi(argv[1]);

  v = (int *) malloc (n * sizeof (int));
  if (v==NULL) {
    fprintf (stderr, "Allocation error.\n");
    return (1);
  }

  fprintf (stdout, "Initial array: ");    
  for (i=0; i<n; i++) {
    v[i] = rand() % N;
    fprintf (stdout, "%d ", v[i]);
  }
  fprintf (stdout, "\n");  

  array_sum (v, n);

  fprintf (stdout, "Final   array: ");    
  for (i=0; i<n; i++) {
    fprintf (stdout, "%d ", v[i]);
  }
  fprintf (stdout, "\n");  
  
  return (0);
}

int array_sum (int *vet, int n) {
  int k=n/2; pthread_t *tids; args_t *args; sem_t *sem;
  tids = (pthread_t *) malloc (k*sizeof(pthread_t));
  sem = (sem_t *) malloc (k*sizeof(sem_t));
  for (int i=0; i<k; ++i) sem_init(&sem[i], 0, 0);
  args = (args_t *) malloc (k*sizeof(args_t));
  for (int i=0; i<k; ++i) {
    args[i].id = i; args[i].vet = vet;
    args[i].n = n; args[i].sem = sem;
  }
  for (int i=0; i<k; ++i)
    pthread_create (&tids[i], NULL, adder, &args[i]);
  pthread_join (tids[0], NULL);
  for (int i=0; i<k; ++i) sem_destroy(&sem[i]);
  free (tids);
  free (sem);
  free (args);
  return vet[0];
}

void *adder (void * arg) {
  sem_t *sem = ((args_t *) arg)->sem;
  int *vet = ((args_t *) arg)->vet;
  int id = ((args_t *) arg)->id;
  int n = ((args_t *) arg)->n;
  int k = n/2;

  int i = 0;
  while (k != 0) {
    if (i!=0 && k<n/2) {
      fprintf (stdout, "%d %d %d\n", id, k, id+k);
      sem_wait (&sem[id + k]);
    } else {
      i++;   // During the first iteration wait nothing
    }
    vet[id] += vet[id + k];
    k = k/2;
    if (id >= k) {
      sem_post (&sem[id]);
      break;
    }
  }
  
  pthread_exit(0);
}
