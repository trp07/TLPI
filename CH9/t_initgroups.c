/* TLPI Ex 9-4, page 184
   implement initgroups() using setgroups() and library
   functions for retrieving information from the password
   and groups files.

   must have root privilidges to use this program:
   $ sudo ./t_initgroups <username>
*/

#define _BSD_SOURCE
//#define DEBUG
#include <grp.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Function prototypes */
int init_groups(const char *user, gid_t group);


int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage error: %s <username>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct passwd *pwd;
    pwd = getpwnam(argv[1]);
    if (pwd == NULL) {
        fprintf(stderr, "main.getpwname failed!\n");
        exit(EXIT_FAILURE);
    }

    if (init_groups(argv[1], pwd->pw_gid) == -1) {
        fprintf(stderr, "main.init_groups failed\n");
        exit(EXIT_FAILURE);
    } else {
        fprintf(stderr, "main.init_groups was successful\n");
    }

    return 0;
}

/*  given a username and a gid, searches all the groups that user belongs to
    from the /etc/group file.  

    Returns 0 on success, -1 on failure via the setgroups function call
*/
int init_groups(const char *user, gid_t group)
{
    struct group *grp;
    char *username;
    int i, grp_idx;
    gid_t groups[1024];			// space for 1024 groups
    grp_idx = 0;
    groups[grp_idx++] = group;  // set original group parameter in array

    while((grp = getgrent()) != NULL) {	// reads the /etc/group file
        i = 0;
        while ((username = grp->gr_mem[i++]) != NULL) { // go through all user id's in that group
            if (strcmp(username, user) == 0) {
                groups[grp_idx++] = grp->gr_gid;		// add gid to groups if user is in that group
            }
        }
    }
    endgrent();		// reset /cat/group file position
	
#ifdef DEBUG		// uncomment the #define DEBUG line above to print all gid's
	/* print groups */
	printf("groups:\n");
	for (i = 0; i <= grp_idx; i++) {
		printf("\t%u\n", groups[i]);
	}
#endif

    return setgroups(grp_idx, groups);
}

