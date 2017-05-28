/* TLPI CH 10
   various uses of time functions
   pages 186-191

   multiple calls to "sleep(1)" are just to slow
   down the programs output.
*/

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

void main(void)
{
  time_t now1, now2;
  time(&now1);
  printf("time(&now1) it is: %ld\n", now1);

  sleep(1);
  now2 = time(NULL);
  printf("time(NULL) it is: %ld\n", now2);

  sleep(1);
  printf("ctime(&now2) it is: %s\n", ctime(&now2) );

  sleep(1);
  struct tm *now3;
  now3 = gmtime(&now2);
  printf("GMT time (d-m-y): %d-%d-%d\n", now3->tm_mday, now3->tm_mon+1, now3->tm_year+1900);

  sleep(1);
  time_t now4 = mktime(now3);
  printf("mktime(struct tm *now3) = %ld\n", now4);

  sleep(1);
  char *now5 = asctime(now3);
  printf("asctime(struct time *now3) = %s\n", now5);

  sleep(1);
  struct timeval tv;
  gettimeofday(&tv, NULL);
  printf("gettimeofday...  tv_sec = %ld  tv_usec= = %ld\n", tv.tv_sec, tv.tv_usec);
}
