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
 
#define RED   0 
#define GREEN 1 

#define N_OF_THREAD  1
#define N_OF_LOOP    10

int state = RED; 
sem_t s_red; 
sem_t s_green; 
sem_t m; 

static void *my_thread (void *);
void red();
void green();
void change();

int main (int argc, char *argv[]) {
  pthread_t threadId;
  int i;
  
  setbuf (stdout, 0);
  srand (getpid());
  
  sem_init(&s_red, 0, 1); 
  sem_init(&s_green, 0, 0); 
  sem_init(&m, 0, 1);

  
  for (i=0; i<N_OF_THREAD; i++) {
    pthread_create (&threadId, NULL, my_thread, NULL);
  }

  pthread_exit (0);
}

static void *my_thread (void *) {
  int i;

  for (i=0; i<N_OF_LOOP; i++) {
    if ((rand()%2)==0) {
      fprintf (stdout, "  STATE %d T %ld call %d.\n", state, pthread_self(), RED);
      red();
    } else {
      if ((rand()%2)==0) {
        fprintf (stdout, "  STATE %d T %ld call %d.\n", state, pthread_self(), GREEN);	
        green();
      } else {
        fprintf (stdout, "  STATE %d T %ld call change.\n", state, pthread_self());		
	change();
      }
    }
    sleep (1);
  }

  pthread_exit (0);
}

void red() { 
  sem_wait(&s_red); 
  sem_post(&s_red); 
} 

void green() { 
  sem_wait(&s_green); 
  sem_post(&s_green); 
} 

void change() { 
  sem_wait(&m); 
  if(state == RED){ 
    state = GREEN; 
    sem_wait(&s_red); 
    sem_post(&s_green); 
  } else { 
    state = RED; 
    sem_wait(&s_green); 
    sem_post(&s_red); 
  } 
  sem_post(&m); 
}











