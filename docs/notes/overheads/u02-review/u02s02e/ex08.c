#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
 
static void *T1 ();
static void *T2 ();

int main (int argc, char *argv[]) {
  pthread_t thread;
  int n = atoi (argv[1]);
  setbuf (stdout, 0);
  pthread_create (&thread, NULL, T1, &n);
  pthread_join (thread, NULL);
  return 1;
}

void *T1 (void *p){
  pthread_t thread;
  int *pn = (int *) p;
  int n = *pn;
  if (n>0) {
	n-=2;
	printf ("(%d)", n);
	pthread_create (&thread, NULL, T2, &n);
  }
  pthread_join (thread, NULL);
  pthread_exit (NULL);
}

void *T2 (void *p){
  pthread_t thread;
  int *pn = (int *) p;
  int n = *pn;
  if (n>0) {
	n+=1;
	printf ("[%d]", n);
	pthread_create (&thread, NULL, T1, &n);
  }
  pthread_join (thread, NULL);
  pthread_exit (NULL);
}










