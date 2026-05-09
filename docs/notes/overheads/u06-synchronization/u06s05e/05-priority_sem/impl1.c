#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pthread.h"
#include "semaphore.h"

typedef struct list_s {
  sem_t *sem;
  int priority;
  struct list_s *next;
} list_t;

typedef struct my_sem_s {
  sem_t *mutex;
  int cnt;
  list_t *sem_list;
} my_sem_t;

my_sem_t *my_sem_init (int value);
sem_t *enqueue_sorted (list_t * head, int priority);
sem_t *dequeue_sorted (list_t * head);
void my_sem_wait (my_sem_t * s, int prio);
void my_sem_signal (my_sem_t * s);

my_sem_t *my_sem;

my_sem_t *my_sem_init (int value) {
  my_sem_t *s;
  s = (my_sem_t *) malloc (sizeof (my_sem_t));
  s->cnt = value;
  s->mutex = (sem_t *) malloc (sizeof (sem_t));
  sem_init (s->mutex, 0, 1);
  s->sem_list = (list_t *) malloc (sizeof (list_t));
  // create empty queue with sentinels
  s->sem_list->priority = -1;
  s->sem_list->next = (list_t *) malloc (sizeof (list_t));
  s->sem_list->next->priority = 1000000;
  s->sem_list->next->next = NULL;
  return s;
}

void my_sem_wait (my_sem_t * s, int prio) {
  sem_t *new_sem;

  sem_wait (s->mutex);
  if (--s->cnt < 0) {
    new_sem = enqueue_sorted (s->sem_list, prio);
    sem_post (s->mutex);
    sem_wait (new_sem);
    sem_destroy (new_sem);
  }
  else
    sem_post (s->mutex);
}

void my_sem_signal (my_sem_t * s) {
  sem_t *sem;

  sem_wait (s->mutex);
  if (++s->cnt <= 0) {
    sem = dequeue_sorted (s->sem_list);
    sem_post (sem);
  }
  sem_post (s->mutex);
}


sem_t *enqueue_sorted (list_t * head, int priority) {
  list_t *p, *new_elem;

  // search
  p = head;
  while (priority > p->next->priority)
    p = p->next;

  new_elem = (list_t *) malloc (sizeof (list_t));
  new_elem->sem = (sem_t *) malloc (sizeof (sem_t));
  sem_init (new_elem->sem, 0, 0);
  new_elem->priority = priority;
  new_elem->next = p->next;
  p->next = new_elem;
  printf("%u with priority = %d  waits on sem %p\n",
    (unsigned int) pthread_self(), priority, new_elem->sem);

  return new_elem->sem;
}

sem_t *dequeue_sorted (list_t * head) {
  list_t *p;
  sem_t *s;

  p = head->next;
  s = head->next->sem;
  printf("%u with priority = %d awakes from sem %p\n",
    (unsigned int) pthread_self(), head->next->priority, s);
  head->next = head->next->next;
  free (p);
  return s;
}

static void *runner (void *arg) {
  int *i = (int *) arg;
  pthread_detach (pthread_self ());
  sleep (random () % 5);
  printf ("request from %u %d\n", (unsigned int) pthread_self (), *i);
  my_sem_wait (my_sem, *i);
  printf ("service of   %u %d\n", (unsigned int) pthread_self (), *i);
  return 0;
}


int main (void) {
  pthread_t th_a;
  int i, *pi;

  setbuf (stdout, 0);

  my_sem = my_sem_init (0);

  for (i = 1; i <= 4; i++) {
    pi = (int *) malloc (sizeof (int));
    *pi = i * 10;
    pthread_create (&th_a, NULL, runner, pi);
  }

  sleep (10);

  for (i = 0; i < 4; i++) {
    my_sem_signal (my_sem);
    sleep(1);
  }

  printf ("main exiting\n");

  pthread_exit (0);
}
