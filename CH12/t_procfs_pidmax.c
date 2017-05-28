/* TLPI, page 228
   read and modify a /proc file
   this example reads the
   /proc/sys/kernel/pid_max file
   cmdline args will update it to the specified value

   must run as sudo

   example:
   sudo ./t_procfs_pidmax 10000
*/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE 100

int main(int argc, char **argv)
{
    int fd;
    char line[MAX_LINE];
    ssize_t n;

    /* open to read/write if cmdline arg specified, otherwise read-only */
    fd = open("/proc/sys/kernel/pid_max", (argc > 1) ? O_RDWR: O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "file open failed\n");
        exit(EXIT_FAILURE);
    }

    n = read(fd, line, MAX_LINE);
    if (n == -1) {
        fprintf(stderr, "read failed\n");
        exit(EXIT_FAILURE);
    }

    if (argc > 1)
        printf("Old value: ");
    printf("%.*s", (int) n, line);

    if (argc > 1) {
        if (lseek (fd, 0, SEEK_SET) == -1) {
            fprintf(stderr, "lseek failed\n");
            exit(EXIT_FAILURE);
        }    
        if (write(fd, argv[1], strlen(argv[1])) != strlen(argv[1])) {
            fprintf(stderr, "write failed\n");
            exit(EXIT_FAILURE);
        }
        system("echo /proc/sys/kernel/pid_max now contains "
               "`cat /proc/sys/kernel/pid_max`");
    }

    exit(EXIT_SUCCESS); 
}
