/*  TLPI page, exercise 6-2, page 138 */

#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void func(void);

int main(void)
{
  func();
  
  int num1, num2, num3;
  num1 = 13;
  num2 = 14;
  num3 = 15;

  int num4 = num1 * num2 * num3;
  printf("a main() number = %d\n", num4);
  
  longjmp(env, 1);
  puts("finishing main()\n");

  return 0;
  
}

void func(void)
{
  int a, b, c;
  a = b = c = 1;
  setjmp(env);
  printf("some func() numbers = %d, %d, %d\n", a, b, c);
}
