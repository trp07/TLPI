#include <stdio.h>
#include "get_num.h"

int main(int argc, char *argv[])
{
    int i, j;
    i = getInt(argv[1], 0, "i");
    j = getInt(argv[2], 0, "j");

    printf("i=%d, j=%d\n", i, j);
    return 0;
}
