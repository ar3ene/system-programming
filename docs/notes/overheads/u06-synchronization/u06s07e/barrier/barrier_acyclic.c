#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N 10

pthread_barrier_t bar;

void *f (void *par) {
  int *np, n;

  np = (int *) par;
  n = *np;
  fprintf (stdout, "T%d-In\n", n);
  pthread_barrier_wait(&bar);
  fprintf (stdout, "  T%d-Out\n", n);
  
  pthread_exit (NULL);
}

int main () {
  int i, v[N];
  pthread_t th[N];

  setbuf (stdout, 0);
  pthread_barrier_init (&bar, NULL, N+1);

  for (i=0; i<N; i++) {
    v[i] = i;
    pthread_create (&th[i], NULL, f, (void *) &v[i]);
  }

  fprintf (stdout, "Main-In\n");
  pthread_barrier_wait (&bar);
  fprintf (stdout, "  Main-Out\n");
  
  for (i=0; i<N; i++) {
    pthread_join (th[i], NULL);
  }
  
  pthread_barrier_destroy(&bar);
  
  return 0;
}

