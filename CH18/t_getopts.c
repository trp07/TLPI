/* TLPI, appendix B

Use of getopt() to parse command-line arguments to a function call.

extern int optind, opterr, optopt;
extern char *optarg;

int getopt(int argc, char *const argv[], const char *opstring);

RETURN: if an option was found, returns the option character.
        -1 if the end of the options list was reached.

optarg = if an option is found and an argument exists for that option, optarg
         points to the argument.  Set to NULL if no argument is given.

opstring = specifies the set of options that getopt() should look for in
           argv.  A sequence of chars, each identifying an option.  Each option
           may be followed by a colon (:), indicating that option expects
           an argument.

optind = on each call to getopt(), updates to contain the index of the next
         unprocessed element of argv.  Automatically set to 1 before first
         call to getopt().
         * if optind = -1 (no more options present) and optind < argc, then
           argv[optind] = next nonoption word from the command line.
         * explicitly set optind = 1 if scanning multiple command-line vectors,
           or rescanning same command line.
         * if next option has double-hyphen (--), getopt stops, but optind
           points to next word after double-hyphen as first "nonoption arg"

optopt = erroneous option characters (one that is unrecognized or one that has
         a required argument missing)

opterr = used to suppress error messages printed by getopt().  Default = 1, set
         to 0 to suppress error messages.  Otherwise can set colon (:) as first
         char in opstring to suppress errors as well, but different...
         : at start of opstring =>
                                Return '?' for unrecognized option
                                Return ':' for missing argument


*/

#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define printable(ch) (isprint((unsigned char) ch) ? ch : '#')

static void usageError(char *progName, char *msg, int opt)
{
    if (msg != NULL && opt != 0)
        fprintf(stderr, "%s (-%c)\n", msg, printable(opt));
    fprintf(stderr, "Usage: %s [-p arg] [-x]\n", progName);
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    int opt, xfnd;
    char *pstr;

    xfnd = 0;       /* counter for -x option */
    pstr = NULL;

    /* ":p:x" leads with a ':' => supresses error messages
       'p:' => p requires an argument.
       'x'  => x does not require an argument   */
    while ((opt = getopt(argc, argv, ":p:x")) != -1) {
        printf("opt = %3d (%c); optind = %d", opt, printable(opt), optind);
        if (opt == '?' || opt == ':')
            printf("; optopt = %3d (%c)", optopt, printable(optopt));
        puts("\n");

        switch (opt) {
            case 'p' : pstr = optarg;       break;
            case 'x' : xfnd++;              break;
            case ':' : usageError(argv[0], "Missing Argument", optopt);
            case '?' : usageError(argv[0], "Unrecognized option", optopt);
            default  : printf("Unexpcted case in switch()\n");
                       exit(EXIT_FAILURE);
        }
    }

    if (xfnd != 0)
        printf("-x was specified (count=%d)\n", xfnd);
    if (pstr != NULL)
        printf("-p was specified with value \"%s\"\n", pstr);
    if (optind < argc)
        printf("First nonoption argument is \"%s\" at argv[%d]\n",
                argv[optind], optind);
    exit(EXIT_SUCCESS);

}
