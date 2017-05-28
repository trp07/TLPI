/* TLPI, exercise 18-7, page 373.

functions to pass to nftw() function

struct FTW {
    int base;   = Offset to basename part of pathname
    int level;  = Depth of file within tree traversal
}

struct stat {
    dev_t       st_dev;     IDs of device on which file resides
    ino_t       st_ino;     I-node number of file
    mode_t      st_mode;    File type and permissions
    nlink_t     st_nlink;   Number of (hard) links to file
    uid_t       st_uid;     Uer ID of file owner
    gid_t       st_gid;     Group ID of file owner
    dev_t       st_rdev;    IDs for device special files
    off_t       st_size;    Total file size (bytes)
    blksize_t   st_blksize; Optimal block size for I/O (bytes)
    blkcnt_t    st_blocks;  Number of (512B) blocks allocated
    time_t      st_atime;   Time of last file access
    time_t      st_mtime;   Time of last file modification
    time_t      st_ctime;   Time of last status change
}

*/

#define _XOPEN_SOURCE 600   /* Get nftw() and S_IFSOCK delcarations (_XOPEN_SOURCE > 500) */
#include <ftw.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "nftw_stats.h"


/* Walk file tree and prints out file types.
   Writes stats to FileTreeStats struct    */
int dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    switch(sbuf->st_mode & S_IFMT) {     /* get file type.  Bitwise & with S_IFMT to get flag */
        case S_IFREG:  printf("-");  FileTreeStats.reg += 1;   break;   /* regular file */
        case S_IFDIR:  printf("d");  FileTreeStats.dir += 1;   break;   /* directory */
        case S_IFCHR:  printf("c");  FileTreeStats.chr += 1;   break;   /* character device */
        case S_IFBLK:  printf("b");  FileTreeStats.blk += 1;   break;   /* block device */
        case S_IFLNK:  printf("l");  FileTreeStats.slink += 1; break;   /* symbolic link */
        case S_IFIFO:  printf("p");  FileTreeStats.fifo += 1;  break;   /* FIFO */
        case S_IFSOCK: printf("s");  FileTreeStats.sock += 1;  break;   /* socket */
        default:       printf("?");     /* should never happen (on Linux)   */
    }

    printf("   %s   ",
                (type == FTW_D)  ? "D  " : (type == FTW_DNR) ? "DNR" :
                (type == FTW_DP) ? "DP " : (type == FTW_F)   ? "F  " :
                (type == FTW_SL) ? "SL " : (type == FTW_SLN) ? "SLN" :
                (type == FTW_NS) ? "NS " : " ");

    if (type != FTW_NS)
        printf("%7ld ", (long) sbuf->st_ino);
    else
        printf("    ");

    printf(" %*s", 4 * ftwb->level, "");        /* Indent suitably */
    printf("%s\n", &pathname[ftwb->base]);      /* Print basename */
    return 0;
}
