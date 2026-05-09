#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

static void *TA (void *);
static void *TB (void *);

sem_t s_a, s_b;

int main(int argc, char *argv[]){
  int N ;
  int i = 0;
  pthread_t *threads_A;
  pthread_t *threads_B;
  int *id;

  setbuf (stdout, 0);
  N = atoi(argv[1]);

  /* Allocation */
  sem_init(&s_a, 0, 1);
  sem_init(&s_b, 0, 0);
  threads_A = (pthread_t*)malloc(N*sizeof(pthread_t));
  threads_B = (pthread_t*)malloc(N*sizeof(pthread_t));
  id = (int*)malloc(N*sizeof(int));

  /* Generation */
  for (i=0; i<N;i++){
    id[i] = i;
    pthread_create(&threads_A[i], NULL, TA, (void *)&(id[i]));
    pthread_create(&threads_B[i], NULL, TB, (void *)&(id[i]));
  }

  /* Join */
  for (i=0; i<N;i++){
    pthread_join(threads_A[i], NULL);
    pthread_join(threads_B[i], NULL);
  }
 
  /* Deallocation */
  free(threads_A); free(threads_B); free(id);
  return 0;
}

static void *TA(void *arg){
  int id;
  id = *((int*)arg);

  sem_wait(&s_a);
  printf("A%d ", id);
  sem_post(&s_b);

  pthread_exit(NULL);
}

static void *TB(void *arg){
  int id;
  id = *((int*)arg);

  sem_wait(&s_b);
  printf("B%d ", id);
  sem_post(&s_a);

  pthread_exit(NULL);
}














