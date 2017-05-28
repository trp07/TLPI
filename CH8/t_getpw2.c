/* TLPI, exercise 8-2, page 166
  
   Implement getpwnam using getpwent/setpwent/endpwent
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

int main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage error: %s <username>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct passwd *pwd;

  while ((pwd = getpwent()) != NULL) {
    if ( (strcmp(argv[1], pwd->pw_name)) == 0) {
        printf("Username \"%s\" found!\n", argv[1]);
        printf("%-8s %5ld\n", pwd->pw_name, (long) pwd->pw_uid);
    }
  }

  endpwent();

  return 0;
}
