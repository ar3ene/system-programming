/*
Use a conditional variable to implement the
following behavior
 A program creates 3 threads
 Thread 1 and thread 2
 Update a variable (count), increasing it at each
iteration
 Thread 3
 Awaits until the variable (count) reaches a specified
value
 When the value is reached, it displays a message on
standard output
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct theadData
{
    pthread_cond_t *cond;
    pthread_mutex_t *mutex;
    int count;
    int specificValue;
} threadData_t;

int flag = 1; //variabile globale che potrebbe essere all'interno della struct

static void *ft12(void *arg) //corrisponde alla  inc
{
    threadData_t *td = (threadData_t *)arg;

    while (flag)
    {
        pthread_mutex_lock(td->mutex);
        //CS
        td->count++; 

        pthread_cond_signal(td->cond);

        pthread_mutex_unlock(td->mutex);
    }

    pthread_exit((void *)0);
}

static void *ft3(void *arg) //corrisponde alla watch
{

    threadData_t *td = (threadData_t *)arg;

    pthread_mutex_lock(td->mutex);
    while (td->count < td->specificValue)
    {
        pthread_cond_wait(td->cond, td->mutex);
    }

    printf("Specific Value Reached\n");
    flag = 0;

    pthread_mutex_unlock(td->mutex);
    pthread_exit((void *)0);
}

int main(int argc, char const *argv[])
{
    pthread_t t1, t2, t3;

    threadData_t td;

    td.cond = malloc(sizeof(pthread_cond_t));
    if (td.cond == NULL)
    {
        exit(EXIT_FAILURE);
    }

    td.mutex = malloc(sizeof(pthread_mutex_t));
    if (td.mutex == NULL)
    {
        exit(EXIT_FAILURE);
    }
    td.count = 0;
    td.specificValue = 5; 

    //for both it should be checked if the value returned is 0
    pthread_cond_init(td.cond, NULL);
    pthread_mutex_init(td.mutex, NULL);

    pthread_create(&t1, NULL, ft12, (void *)&td);
    pthread_create(&t2, NULL, ft12, (void *)&td);
    pthread_create(&t3, NULL, ft3, (void *)&td);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    return 0;
}
