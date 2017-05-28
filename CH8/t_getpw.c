/* TLPI, page 157
   getpwname and getpwuid  */

#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

int main(int argc, char **argv)
{
  if (argc != 3) {
	fprintf(stderr, "Usage error: %s <username> <uid>\n", argv[0]);
	exit(EXIT_FAILURE);
  }

  struct passwd *test1, *test2;

  if ( (test1 = getpwnam(argv[1])) != NULL) {
	printf("getpwnam:\n");
	printf("\tpw_name = %s\n", test1->pw_name);
	printf("\tpw_passwd = %s\n", test1->pw_passwd);
	printf("\tpw_uid = %u\n", test1->pw_uid);
	printf("\tpw_guid = %u\n", test1->pw_gid);
	printf("\tpw_gecos = %s\n", test1->pw_gecos);
	printf("\tpw_dir = %s\n", test1->pw_dir);
	printf("\tpw_shell = %s\n", test1->pw_shell);
  } else {
	fprintf(stderr, "getpwnam returned NULL\n");
  }

  if ( (test2 = getpwuid(atoi(argv[2]))) != NULL) {
	printf("getpwuid:\n");
	printf("\tpw_name = %s\n", test2->pw_name);
	printf("\tpw_passwd = %s\n", test2->pw_passwd);
	printf("\tpw_uid = %u\n", test2->pw_uid);
	printf("\tpw_guid = %u\n", test2->pw_gid);
	printf("\tpw_gecos = %s\n", test2->pw_gecos);
	printf("\tpw_dir = %s\n", test2->pw_dir);
	printf("\tpw_shell = %s\n", test2->pw_shell);
  } else {
	fprintf(stderr, "getpwuid returned NULL\n");
  }

  return 0;
}
