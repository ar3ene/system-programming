/*
 *  This program improves the previous version as:
 *  1. It does not use an array to store all TID but pointers to reach the parent structure;
 *     TID are displayed in the right order using a recursive function.
 *  3. It does protect the output phase including it in a critical section.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock;

typedef struct thread_s {
  int n;
  pthread_t tid;
  struct thread_s *parent;
} thread_t;

void *run_thread(void *);
void print_backward(thread_t *);
static void * my_malloc (size_t);
  
int main(int argc, char *argv[]) {
  int levels;
  pthread_t th1, th2;
  thread_t *tidElem;

  tidElem = my_malloc(sizeof(tidElem));

  sscanf(argv[1], "%d", &levels);

  tidElem->tid = pthread_self();
  tidElem->parent = NULL;
  tidElem->n = levels;

  pthread_create (&th1, NULL, run_thread, tidElem);
  pthread_create (&th2, NULL, run_thread, tidElem);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);

  free(tidElem);
  tidElem = NULL;

  return 0;
}

void *run_thread(void *thread) {
  pthread_t th1, th2;
  thread_t *info;

  info = my_malloc(sizeof(thread_t));
  info->n = ((thread_t *) thread)->n -1;
  info->tid = pthread_self();
  info->parent = (thread_t *) thread;

  if (info->n > 0) {
    pthread_create(&th1, NULL, run_thread, info);
    pthread_create(&th2, NULL, run_thread, info);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
  } else {
    // Begin Critical Section "Print"
    pthread_mutex_lock(&lock);
    print_backward(info->parent);
    // End Critical Section "Print"
    printf("\n");
    pthread_mutex_unlock(&lock);
  }

  free(info);
  info = NULL;

  return NULL;
}

/*
 * Recursive function to print the TID backward
 */
void print_backward (thread_t *tid) {
  if (tid==NULL) {
    return;
  }
  print_backward (tid->parent);
  printf("%lld ", (unsigned long long) tid->tid);

  return;
}

static void * my_malloc (size_t s) {
  void *p;
  
  p = (void *)  malloc (s);
  if (p == NULL) {
    fprintf (stderr, "Allocation error.\n");
    exit (1);
  }

  return (p);
}

