/*  TLPI page 132 */

#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void func(void);

int main(void)
{
  int i = 0;

  setjmp(env);

  i += 1;
  if (i > 10) {
    puts("i is > 10; exiting.");
    return 0;
  }

  func();

  return -1;
}

void func(void)
{
  longjmp(env, 1);
}
