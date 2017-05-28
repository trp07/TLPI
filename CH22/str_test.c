#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int i;
    long t;
    char *endptr;

    for (i = 1; i < argc; i++) {
        t = strtol(argv[i], &endptr, 0);
        if (*endptr != '\0') {
            printf("Weird arg: %s, endptr=%s, strtol=%ld\n",
                    argv[i], endptr, t);
        }
        printf("Normal arg: %s, endptr=%s, strtol=%ld\n",
                argv[i], endptr, t);
    }

    return 0;
}
