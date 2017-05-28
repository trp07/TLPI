/* TLPI page 176  */

#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    uid_t ruid, euid, suid;  // real, effective, saved uid
    if (getresuid(&ruid, &euid, &suid) == -1) {
      fprintf(stderr, "getresuid failed!\n");
      exit(EXIT_FAILURE);
    }

    printf("ruid = %u, euid = %u, suid = %u\n", ruid, euid, suid);
    return 0;
}
