/*
 *  StQ 24.03.2022
 *  Thread-Pool
 *  A simple Producer and Consumer scheme with a Queue
 *  Tasks are random strings to capitalize
 *  Command line: N=number of tasks generated, C=Number of producers, C=number of consumers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>

#define SIZE       16
#define STR_SIZE   10

static void *my_malloc (int, int);
static void *producer (void *);
static char *generate ();
static void *consumer (void *);
static void convert (char *);
static void waitRandomTime (int);

typedef struct thread_s {
    int n, nP, nC;
    char **v;
    int size;
    int head;
    int tail;
    pthread_mutex_t  meP;
    pthread_mutex_t  meC;
    sem_t empty;
    sem_t full;
} thread_t;

int main (int argc, char *argv[]) {
    thread_t thread_d;
    pthread_t *tp, *tc;
    int i, N, P, C;

    setbuf (stdout, 0);
    srand (getpid());

    N = atoi (argv[1]);
    P = atoi (argv[2]);
    C = atoi (argv[3]);

    tp = my_malloc (P, sizeof (pthread_t));
    tc = my_malloc (C, sizeof (pthread_t));

    thread_d.n = N;
    thread_d.nP = thread_d.nC = 0;
    thread_d.size = SIZE;
    thread_d.v = my_malloc (thread_d.size, sizeof (char *));
    thread_d.head = 0;
    thread_d.tail = 0;
    pthread_mutex_init (&thread_d.meP, NULL);
    pthread_mutex_init (&thread_d.meC, NULL);
    sem_init (&thread_d.empty, 0, SIZE);
    sem_init (&thread_d.full, 0, 0);

    for (i=0; i<P; i++) {
        pthread_create(&tp[i], NULL, producer, (void *) &thread_d);
    }
    for (i=0; i<C; i++) {
        pthread_create(&tc[i], NULL, consumer, (void *) &thread_d);
    }

    for (i=0; i<P; i++) {
        pthread_join (tp[i], NULL);
    }
    for (i=0; i<C; i++) {
        pthread_join (tc[i], NULL);
    }

    return (1);
}

static void *producer (void *arg) {
    thread_t *p;
    int goon;

    p = (thread_t *) arg;
    goon = 1;
    while (goon == 1) {
        waitRandomTime (3);
        sem_wait (&p->empty);
        pthread_mutex_lock (&p->meP);
        // Check termination condition
        if (p->nP > p->n) {
            goon = 0;
        } else {
            p->nP = p->nP + 1;
            p->v[p->tail] = generate();
            fprintf (stdout, "Producing %d: %s\n", p->nP, p->v[p->tail]);
            p->tail = (p->tail+1) % SIZE;
        }
        pthread_mutex_unlock (&p->meP);
        sem_post (&p->full);
    }

    pthread_exit ((void *) 1);
}

static char *generate () {
    int l, i;
    char *str;

    l = 1 + rand() % STR_SIZE;
    str = my_malloc ((l+1), sizeof(char));
    for (i = 0; i < l; i++) {
        str[i] = (char) (((int) 'a') + rand() % 26);
    }
    str[i] = '\0';
    return str;
}

static void *consumer (void *arg) {
    thread_t *p;
    int goon;
    char *str;

    p = (thread_t *) arg;
    goon = 1;
    while (goon == 1) {
        waitRandomTime (3);
        pthread_mutex_lock (&p->meC);
        if (p->nC > p->n) {
            goon = 0;
        } else {
            p->nC = p->nC + 1;
            sem_wait (&p->full);
            str = p->v[p->head];
            convert (str);
            fprintf (stdout, "--- CONSUMING %d: %s\n", p->nC, str);
            free (str);
            p->head = (p->head+1) % SIZE;
            sem_post (&p->empty);
        }
        pthread_mutex_unlock (&p->meC);
    }

    pthread_exit ((void *) 1);
}

static void convert (char *str) {
    int i;
    for (i = 0; i < strlen(str); i++) {
        str[i] = toupper(str[i]);
    }
    return;
}

static void *my_malloc (int n, int size) {
    void *p;

    p = (void *)  malloc (n * size);
    if (p == NULL) {
        fprintf (stderr, "Allocation error.\n");
        exit (1);
    }

    return (p);
}

static void waitRandomTime (int max) {
#if 1
    sleep ((int) (rand() % max));
#endif
    return;
}
