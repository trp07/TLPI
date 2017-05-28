/* TLPI page 127 */

#include <stdio.h>

extern char **environ;

int main(int argc, char *argv[])
{
	char **ep;

	for (ep = environ; *ep != NULL; ep++)
	  puts(*ep);

	return 0;
}
