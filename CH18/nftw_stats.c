/* TLPI exercise 18-7, page 373

Write a program that uses nftw() to traverse a directory tree 
and finished by printing out counts and percentages of the various types 
(regular, directory, symbolic link, and so on) of files in the tree.

*/

#define _XOPEN_SOURCE 600       /* Get nftw() and S_IFSOCK delcarations */
#include <unistd.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include "nftw_stats.h"

/* initialize struct with file stats */
FTS FileTreeStats = {0,0,0,0,0,0,0};

/* function prototype */
static void usageError(const char *progName, const char *msg);

int main(int argc, char *argv[])
{
    int flags, opt;

    flags = 0;
    while ((opt = getopt(argc, argv, "dmp")) != -1) {
        switch (opt) {
            case 'd' : flags |= FTW_DEPTH; break;
            case 'm' : flags |= FTW_MOUNT; break;
            case 'p' : flags |= FTW_PHYS;  break;
            default: usageError(argv[0], NULL);
        }
    }

    if (argc > optind + 1)  /* too many args */
        usageError(argv[0], NULL);

    const char *searchpath = (argc > optind) ? argv[optind] : "."; 

    if (nftw(searchpath, dirTree, 10, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }

    /* print stats from FileTreeStats struct */
    unsigned long total = FileTreeStats.reg + FileTreeStats.dir + FileTreeStats.chr + 
                          FileTreeStats.blk + FileTreeStats.slink + FileTreeStats.fifo +
                          FileTreeStats.sock;
    
    printf("\n\nFILE STATS***\n");
    printf("TOTAL FILES             = %lu\n", total);
    printf("total files             = %lu\t(%2.1f%%)\n", FileTreeStats.reg, (float) 100 * FileTreeStats.reg/total);
    printf("total dirs              = %lu\t(%2.1f%%)\n", FileTreeStats.dir, (float) 100 * FileTreeStats.dir/total);
    printf("total character devices = %lu\t(%2.1f%%)\n", FileTreeStats.chr, (float) 100 * FileTreeStats.chr/total);
    printf("total block count       = %lu\t(%2.1f%%)\n", FileTreeStats.blk, (float) 100 * FileTreeStats.blk/total);
    printf("total symbolic links    = %lu\t(%2.1f%%)\n", FileTreeStats.slink, (float) 100 * FileTreeStats.slink/total);
    printf("total FIFOS             = %lu\t(%2.1f%%)\n", FileTreeStats.fifo, (float) 100 * FileTreeStats.fifo/total);
    printf("total sockets           = %lu\t(%2.1f%%)\n", FileTreeStats.sock, (float) 100 * FileTreeStats.sock/total);    

    exit(EXIT_SUCCESS);
}


static void usageError(const char *progName, const char *msg)
{
    if (msg != NULL)
        fprintf(stderr, "%s\n", msg);

    fprintf(stderr, "Usage: %s [-d] [-m] [-p] [directory-path]\n", progName);
    fprintf(stderr, "\t-d Use FTW_DEPTH flag\n");
    fprintf(stderr, "\t-m Use FTW_MOUNT flag\n");
    fprintf(stderr, "\t-p use FTW_PHYS flag\n");
    exit(EXIT_FAILURE);
}
