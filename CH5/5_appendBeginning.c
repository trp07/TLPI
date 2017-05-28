/* 
	TLPI Ch5, Exercise 5-2
	Opens an existing filepath (argv[1]) for writing with the
	O_APEND flag, then seeks to the beginning of the file before
	writing data.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc != 2 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "Usage: ./progname <file>\n");
		exit(EXIT_FAILURE);
	}

	/* open file and set pointer to start of file */
	int fd = open(argv[1], O_RDWR | O_APPEND);
	lseek(fd, 0, SEEK_SET);

	/* create characters string */
	int i, LEN = 100;
	char buffer[LEN];
	for (i = 0; i < LEN; i++)
		buffer[i] = 'A';

	/* write character string to file */
	fprintf(stderr, "Writing the following string to file: ");
	write(STDERR_FILENO, buffer, LEN);	
	puts("\n");

	if (write(fd, buffer, LEN) != LEN) {
		fprintf(stderr, "Error occured during write\n");
		exit(EXIT_FAILURE);
	}
	
	fprintf(stderr, "Successfully wrote 100 bytes bytes to file: %s\n", argv[1]);
	exit(EXIT_SUCCESS);
}
