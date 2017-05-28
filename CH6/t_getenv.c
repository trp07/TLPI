/* TLPI page 127 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("SHELL = %s\n", getenv("SHELL"));

	return 0;
}
