/* TLPI, page 216
   test the sysconf() call to test
   limits on a current machine.
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

static void sysconfPrint(const char *msg, int name)
{
  long lim;
  errno = 0;
  lim = sysconf(name);
  if (lim != -1) {  /* call succeeded */
      printf("%s %ld\n", msg, lim);
  } else {
      if (errno == 0) {  /* call succeeded, limit indeterminant */
          printf("%s (indeterminant)\n", msg);
      } else {	/* call failed */
          printf("sysconf failed\n");
          exit(EXIT_FAILURE);
      }
  }
}
int main(void)
{
  sysconfPrint("_SC_ARG_MAX:		", _SC_ARG_MAX);
  sysconfPrint("_SC_LOGIN_NAME_MAX:	", _SC_LOGIN_NAME_MAX);
  sysconfPrint("_SC_NGROUPS_MAX:	", _SC_NGROUPS_MAX);

  return 0;
}
