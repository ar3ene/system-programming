#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N 4
#define C 5

pthread_barrier_t bar;

void *f (void *par) {
  int i, j, *np, n;

  np = (int *) par;
  n = *np;

  for (i=0; i<C; i++) {
    for (j=0; j<i; j++)
      fprintf (stdout, "\t");
    fprintf (stdout, "T%d-In%d\n", n, i);
    pthread_barrier_wait(&bar);
    for (j=0; j<i; j++)
      fprintf (stdout, "\t");
    fprintf (stdout, "  T%d-Out%d\n", n, i);
  }
  
  pthread_exit (NULL);
}

int main () {
  int i, v[N];
  pthread_t th[N];

  setbuf (stdout, 0);
  pthread_barrier_init (&bar, NULL, N);

  for (i=0; i<N; i++) {
    v[i] = i;
    pthread_create (&th[i], NULL, f, (void *) &v[i]);
  }

  for (i=0; i<N; i++) {
    pthread_join (th[i], NULL);
  }
  
  pthread_barrier_destroy(&bar);
  
  return 0;
}

