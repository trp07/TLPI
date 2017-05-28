#ifndef NFTW_STATS_H
#define NFTW_STATS_H

typedef struct _FileTreeStats {
    unsigned long reg;
    unsigned long dir;
    unsigned long chr;
    unsigned long blk;
    unsigned long slink;
    unsigned long fifo;
    unsigned long sock;
} FTS;

extern FTS FileTreeStats;

int dirTree(const char *pathname, const struct stat *sbuf, int type,
                   struct FTW *ftwb);

#endif
