/*
    TLPI, exercise 30-1
    Thread mutex practice

    * compile with -lpthread flag
    * Usage: ./mutex_test <numThreads> <numIters>
*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* GLOBAL Var's and Mutex's */
static int glob = 0;    /* global var to be accessed by threads */
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

/******************************************************************************/
/* Error exit function */
void
threadExit(char *msg, unsigned long thread)
{
    fprintf(stderr, "%s => thread: %lu\n", msg, thread);
    exit(EXIT_FAILURE);
}


/******************************************************************************/
static void *
threadFunc(void *arg)
{
    int loops = *((int *) arg);   /* number of loops to iterate */
    int loc, j, s;

    for (j = 0; j < loops; j++) {
        s = pthread_mutex_lock(&mtx);   /* acquire the lock */
        if (s != 0)
            threadExit("Mutex lock failed", (long) pthread_self());

        loc = glob;     /* do work on global var */
        loc++;
        glob = loc;

        s = pthread_mutex_unlock(&mtx); /* release the lock */
        if (s != 0)
            threadExit("Mutex unlock failed", (long) pthread_self());
    }

    return NULL;
}
/******************************************************************************/
int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage error: %s <numThreads> <numIters>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int numThreads = atoi(argv[1]);
    int numIters = atoi(argv[2]);
    int s, i;
    pthread_t threads[numThreads];  /* array of threads */

    /* create threads */
    for (i = 0; i < numThreads; i++) {
        s = pthread_create(&threads[i], NULL, threadFunc, &numIters);
        if (s != 0)
            threadExit("Thread creation failed", (long) i);
    }

    /* wait for threads to finish */
    for (i = 0; i < numThreads; i++) {
        s = pthread_join(threads[i], NULL);
        if (s != 0)
            threadExit("Thread join failed", (long) i);
    }

    /* All threads should now be finished */
    printf("ALL THREADS COMPLETE!!!\nglob = %d\n", glob);
    exit(EXIT_SUCCESS);
}
