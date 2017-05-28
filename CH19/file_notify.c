/* TLPI, exercise 19-1.

Partially works!

To-Do: 
    * Create a linked list or hash table of the directories being monitored.
      That way, one can call a function to print out the current list/table,
      which should be updated when a new subdir is added or deleted.
    * Create a command-line option to enable/disable DEBUG, as well as more or
      less verbosity in the printout.
    * Multi-threaded approach.
    * Built-in sleeps so it's not running 100% of the time.

Write a program that logs all file creations, deletions, and renames under 
the directory named in its command-line argument.  The program should monitor 
events in all of the subdirectories under the specified directory.  To obtain 
a list of all of these subdirectories, you will need to make use of nftw() 
(Section 18.9).  When a new subdirectory is added under the tree or a directory 
is deleted, the set of monitored subdirectories should be updated accordingly.

*/

#define _XOPEN_SOURCE 500  /* needed for nftw() declaration */
#include <ftw.h>
#include <sys/inotify.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define DEBUG 1

/* GLOBALS */
#define BUF_LEN (100 * (sizeof(struct inotify_event) + NAME_MAX + 1))
int inotifyFd;

/* Function prototypes */
void errExit(const char *message);
static int dirTree(const char *pathname, const struct stat *sbuf,
                   int type, struct FTW *ftw);
static int createInotifyWatch(const char *pathname);
static void displayInotifyEvent(struct inotify_event *i);


/******************************************************************************/
/* error reporting function */ 
void errExit(const char *message)
{
    fprintf(stderr, "*** Error in: %s\n", message);
    exit(EXIT_FAILURE);
}

/******************************************************************************/
/* nftw() utility function to initiate a watch on all directories
   and subdirectories on the file tree walk */
static int dirTree(const char *pathname, const struct stat *sbuf,
                   int type, struct FTW *ftw)
{

    if ( (sbuf->st_mode & S_IFMT) == S_IFDIR) {   /* this is the directory */
        printf("Creating inotify watch for directory: %s", pathname);
        createInotifyWatch(pathname);
    }
    return 0;
}

/******************************************************************************/
/* create an inotify watch for a given directory
   Returns 0 on success, -1 on failure  */
int createInotifyWatch(const char *pathname)
{
    int wd;
    wd = inotify_add_watch(inotifyFd, pathname, IN_ALL_EVENTS);

#ifdef DEBUG
    printf("\twd = %d\n", wd);
#endif

    if (wd == -1)
        errExit("createInotifyWatch");
    return 0;
}

/******************************************************************************/
/* Displays file events in a given directory  */
static void displayInotifyEvent(struct inotify_event *i)
{
    printf("    wd=%2d; ", i->wd);
    if (i->cookie > 0)
        printf("cookie=%4d; ", i->cookie);

    printf("mask = ");
    if (i->mask & IN_ACCESS)        printf("IN_ACCESS ");
    if (i->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
    if (i->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
    if (i->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE ");
    if (i->mask & IN_CREATE)        printf("IN_CREATE ");
    if (i->mask & IN_DELETE)        printf("IN_DELETE ");
    if (i->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
    if (i->mask & IN_IGNORED)       printf("IN_IGNORED ");
    if (i->mask & IN_ISDIR)         printf("IN_ISDIR ");
    if (i->mask & IN_MODIFY)        printf("IN_MODIFY ");
    if (i->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
    if (i->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
    if (i->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");
    if (i->mask & IN_OPEN)          printf("IN_OPEN ");
    if (i->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
    if (i->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
    printf("\n");

    if (i->len > 0)
        printf("        name = %s\n", i->name);
}

/******************************************************************************
 * ------------------------------   MAIN --------------------------------------
 * ****************************************************************************/

int main(int argc, char *argv[])
{
    char buf[BUF_LEN]; // __attribute__((align(8)));
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    /* verify corrent args */
    if (argc < 2 || argc > 2) {
        fprintf(stderr, "Usage Error: %s dirpath\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    inotifyFd = inotify_init();

#ifdef DEBUG
    printf("inotifyFd = %d\n", inotifyFd);
#endif

    /* kick off the program by running nftw() and create a watch for all
       subdirs on the dirpath   */
    if (nftw(argv[1], dirTree, 10, FTW_DEPTH) == -1)
        errExit("nftw");

    printf("\n***nftw() finished... now watching... \n");

    for (;;) {
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0)
            errExit("read() from inotify fd returned 0!");

        if (numRead == -1)
            errExit("read returned -1");

        /* process all events in buffer returned by read() */
        for (p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);
            p += sizeof(struct inotify_event) + event->len;  /* go to next event */
        }
    }

    return 0;
}
