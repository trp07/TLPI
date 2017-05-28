/* TLPI, page 502

modification to highlight the process of this function shown in TLPI.
Parse a commandline arg (arg[1]) to provide time/interval for linux
timer API syscalls.

Examples:
    ./t_strchr 5:2
    ./t_strchr 5/3:2/7
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *dupstr, *cptr, *sptr;
    int tv_sec, tv_nsec;

    if (argc != 2) {
        fprintf(stderr, "Usage error: %s value.sec[/value.nsec]"
                          "[:interval.sec[/interval.nanosec]]\n", argv[0]);
        return -1;
    }


    dupstr = strdup(argv[1]);
    printf("dupstr = %s\n", dupstr);

    cptr = strchr(dupstr, ':');
    printf("cptr = %s\n", cptr);
    if (cptr != NULL) {
        *cptr = '\0';
    }
    printf("cptr = %s\n", cptr);

    sptr = strchr(dupstr, '/');
     printf("sptr = %s\n", sptr);

     tv_sec = atoi(dupstr);
     tv_nsec = (sptr != NULL) ? atoi(sptr+1) : 0;
     printf("tv_sec=%d,  tv_nsec=%d\n", tv_sec, tv_nsec);

     if (cptr == NULL) {
        tv_sec = 0;
        tv_nsec = 0;
     } else {
        sptr = strchr(cptr+1, '/');
        printf("sptr = %s\n", sptr);
        if (sptr != NULL)
            *sptr = '\0';
        tv_sec = atoi(cptr+1);
        tv_nsec = (sptr != NULL) ? atoi(sptr+1) : 0;
     }

    printf("tv_sec=%d,  tv_nsec=%d\n", tv_sec, tv_nsec);

    free(dupstr);
    return 0;
}
