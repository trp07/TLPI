/* TPLI, exercise 12-1, page 231
   List PID's and Command Name for 
   all processes being run by the 
   username specified on the commandline 
   as argv[1].
*/

//#define DEBUG
#define _BSD_SOURCE
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listPid.h"


/* error printing function */
void errExit(char *msg)
{
    fprintf(stderr, "%s", msg);
    exit(EXIT_FAILURE);
}


/* print all the PIDs */
void printPID(char *dirpath, uid_t uid)
{
    struct dirent *direntp;
    DIR *dirp;
    char linebuf[256];
    char filename[256];
    char uidstr[32];
    char procname[256];
    char pid[32];
    char *key, *value;
    int keepme;
    FILE *fp;

    sprintf(uidstr, "%d", uid);
    
    /* open the directory path */
    dirp = opendir(dirpath);

    errno = 0;
    for (;;) { /* infinite loop walking all PID dir's */
        keepme = 0;

        errno = 0;
        direntp = readdir(dirp);
        if (direntp == NULL)    /* reached the end of the dirpath list */
            break;  

        /* skip . and .. folders */
        if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
            continue;   

        /* error handling in case process terminates before 
           we can read it */
        if (errno != 0) {
            if (errno == ENOENT) {
                continue; /* process doesn't exist anymore */
            } else {
                errExit("readdir failed\n");
            }
        }

        /* open /proc/PID/status file  for reading */
        strcpy(filename, "/proc/");
        strcat(filename, direntp->d_name);
        strcat(filename, "/status");

#ifdef DEBUG
        printf("filename = %s\n", filename);
#endif

        fp = fopen(filename, "r");
        if (fp == NULL) {
            continue; /* can't open file? skip it. */
        }

        /* read file */
        while (fgets(linebuf, sizeof(linebuf), fp)) {
            /* parse lines with ":" as delimiter */
            key = strtok(linebuf, ":"); 
            value = strtok(NULL, ":");
            if (key != NULL && value != NULL) {
                trim(key);
                trim(value);
                if (strcmp(key, "Uid") == 0 && strstr(value, uidstr) != NULL) {
                    keepme = 1;
                }
            }
            
            /* get process name */
            if (strcmp(key, "Name") == 0) {
                strcpy(procname, value);
            }
    
            /* get process id */
            if (strcmp(key, "Pid") == 0) {
                strcpy(pid, value);
            }
        }
                
        if (keepme) {
            printf("%s: %s\n", pid, procname);
        }           
    }       
}    


/**************************************************************************/
int main(int argc, char **argv)
{
    /* check arg count */
    if (argc < 2) {
        errExit("Usage error: ./progrname <username>\n");
    }

    /* get user id */
    struct passwd *user = getpwnam(argv[1]);
    uid_t uid;
    if(user == NULL) {
        errExit("getpwname failed\n");
    }
    uid = user->pw_uid;

    /* print the user's PIDs in the given dirpath */
    printPID("/proc", uid);
       
    exit(EXIT_SUCCESS);  
}
