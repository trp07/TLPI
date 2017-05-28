/* TLPI, simple_thread.c
   page 626

    ** modified to handle more threads specified by argv[1]

   compile with -lpthread flag
*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/************************************/
static void *
threadFunc(void *arg)
{
    char *s = (char *) arg;

    printf("\tThread: %lu => %s\n", (long) pthread_self(), s);

    sleep(0.25);        /* sleep to simulate thread taking time to complete */
    return (void *) strlen(s);
}

/************************************/
int
main(int argc, char *argv[])
{
    /* check arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage error: %s numThreads\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int numThreads = atoi(argv[1]);     /* convert argv[1] to int */
    pthread_t threads[numThreads];     /* create array of threads */
    void *res;                         /* placeholder for thread return value */
    int s;                             /* return value of pthread api calls */

    /* create all the threads */
    for (int i = 0; i < numThreads; i++) {
        s = pthread_create(&threads[i], NULL, threadFunc, "Hello world\n");
        if (s != 0) {
            fprintf(stderr, "[!] pthread_create failed!\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Message from main()\n");

    /* join all threads */
    for (int i = 0; i < numThreads; i++) {
        s = pthread_join(threads[i], &res);
        if (s != 0) {
            fprintf(stderr,"[!] pthread_join failed!\n");
            exit(EXIT_FAILURE);
        }

        printf("Thread %lu returned %ld\n", (long) threads[i], (long) res);
    }

    /* all threads finished */
    exit(EXIT_SUCCESS);
}
