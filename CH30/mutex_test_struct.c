/*
    TLPI, exercise 30-1

    Passes a struct as the arguments to the thread function

    Notes:
        * In order to pass in a struct to multiple threads in a loop,
          must create a struct with malloc(), otherwise it'll pass in
          the same pointer to each thread, causing multiple threads to
          have the same attributes
        * Before creating all threads, since C doesn't like to allow you
          to create an unspecified number of threads at compile time, should
          call malloc() to create the memory for the command-line arg specified
          number of threads and increment through that, otherwise creating a
          large array of threads stored on the stack will cause a seg fault.

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

/* Typedefs and Structs for threads */
typedef struct _threadStruct {
    unsigned int ID;
    unsigned int loops;
} threadStruct;

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
    threadStruct *ts = (threadStruct *) arg;
    int loc, j, s;

    for (j = 0; j < ts->loops; j++) {
        s = pthread_mutex_lock(&mtx);   /* acquire the lock */
        if (s != 0)
            threadExit("Mutex lock failed", (long) pthread_self());

        loc = glob;     /* do work on global var */
        loc++;
        glob = loc;

        printf("   [+] Thread: %u ... glob = %d\n", ts->ID, glob);

        s = pthread_mutex_unlock(&mtx); /* release the lock */
        if (s != 0)
            threadExit("Mutex unlock failed", (long) pthread_self());
    }

    /* free memory for struct passed in since it was dynamically allocated  */
    free(arg);
    return NULL;
}
/******************************************************************************/
int
main(int argc, char *argv[])
{
    int numThreads, s, i, loops;
    threadStruct *ts;
    pthread_t *threads; /* dynamically allocate memory for all threads */

    /* verify command line arguments */
    if (argc != 3) {
        fprintf(stderr, "Usage error: %s <numThreads> <numIters>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* assign command line arguments to variables */
    numThreads = atoi(argv[1]);
    loops = atoi(argv[2]);

    /* create threads */
    threads = malloc(numThreads * sizeof(pthread_t));
    printf("CREATING %d THREADS ... glob = %d\n", numThreads, glob);
    for (i = 0; i < numThreads; i++) {
        ts = (threadStruct *) malloc(sizeof(threadStruct));
        if (ts == NULL)
            threadExit("Malloc failed on thread: ", (long) i);

        ts->ID = i + 1;
        ts->loops = loops;

        printf("Creating thread: %u\n", ts->ID);
        s = pthread_create(&threads[i], NULL, threadFunc, (void *) ts);
        if (s != 0)
            threadExit("Thread creation failed", (long) i);
    }


    /* wait for threads to finish */
    for (i = 0; i < numThreads; i++) {
        s = pthread_join(threads[i], NULL);
        if (s != 0)
            threadExit("Thread join failed", (long) i);
    }

    free(threads);  /* free the memory reserved for threads */

    /* All threads should now be finished */
    printf("ALL THREADS COMPLETE!!!\nglob = %d\n", glob);
    exit(EXIT_SUCCESS);
}
