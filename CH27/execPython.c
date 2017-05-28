/*
This is not one of the TLPI exercises, but just practice.

demo of execve(const char *pathname, char *const argv[], char *const envp[])

opens a python3 interpreter
*/

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *arg[] = {"python3", "-q", NULL};  /* null-terminated argv array */
    char *env[] = {NULL};   /* null-terminated environment array */

    execve("/usr/bin/python3", arg, env);
    fprintf(stderr, "execve failed!\n");
    return 0;
}
