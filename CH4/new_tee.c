/* The Linux Programming Interface, Michael Kerrisk

Exercise 4-1:
Implement the "tee" function using I/O system calls.  
Create the "-a" command-line option (tee -a file), which
causes tee to append text to the end of a file if it 
already exists.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXBUF 1024

int main(int argc, char **argv) 
{
	/* parse command-line options */
	int opt, appendMode = 0;
	while((opt = getopt(argc, argv, "a")) != -1) {
		switch(opt) {
			case 'a': appendMode = 1; break;

			case '?': if (isprint(optopt))
					    fprintf(stderr, "Unknown option -%c\n", optopt);
					  else
				 	    fprintf(stderr, "Unknown option character \\x%x\n", optopt);
					  exit(EXIT_FAILURE);
			default:  appendMode = 0; break;
			          
		}		
	}
	printf("Writing to file: %s\n", argv[optind]);

	/* open file descriptor */
	mode_t filePerms;
	int fd, openFlags;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | 
	            S_IWGRP | S_IROTH | S_IWOTH; /* rw-rw-rw */
	if (appendMode) 
		openFlags = O_CREAT | O_WRONLY | O_APPEND;
	else
		openFlags = O_CREAT | O_WRONLY | O_TRUNC;

	fd = open(argv[optind], openFlags, filePerms);

	/* Check if file open succeeded */
	if (fd == -1) {
		fprintf(stderr, "File open failed.\n");
		exit(EXIT_FAILURE);
	}

	
	/* read from stdin and write to both stdout and fd */
	ssize_t numRead;
	char buffer[MAXBUF];
	while ((numRead = read(STDIN_FILENO, buffer, MAXBUF)) > 0) {
		 	if (write(fd, buffer, numRead) != numRead || write(STDOUT_FILENO, buffer, numRead) !=numRead) { 
				fprintf(stderr, "Couldn't write whole buffer!\n");
				exit(EXIT_FAILURE);
			}
	}
	
	/* close file */
	if (close(fd) == -1) {
		fprintf(stderr, "File close failed.\n");
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}
