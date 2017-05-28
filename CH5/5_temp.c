/*
	TLPI Ch5, page 109 
	Generates a temporary file on Desktop, then deletes it
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int fd;
	char template[] = "/home/phillipstr/Desktop/somestringXXXXXX";

	fd = mkstemp(template);
	if (fd == -1) {
		printf("Error occured with mkstemp\n");
		exit(EXIT_FAILURE);
	}
	printf("Generated filename was: %s\n", template);
	sleep(5);
	unlink(template);


	if (close(fd) == -1) {
		printf("Error occusred with close\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
