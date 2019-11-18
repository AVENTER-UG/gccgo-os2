/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 128 

typedef struct {
   unsigned short a;
   unsigned short b;
} s;

__attribute__ ((noinline)) int
main1 ()
{
  s arr[N];
  s *ptr = arr;
  s res[N];
  int i;

  for (i = 0; i < N; i++)
    {
      arr[i].a = i;
      arr[i].b = i * 2;
      asm volatile ("" ::: "memory");
    }

  for (i = 0; i < N; i++)
    {
      res[i].a = ptr->b - ptr->a;
      res[i].b = ptr->b + ptr->a;
      ptr++;
    }
  
  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (res[i].a != arr[i].b - arr[i].a
          || res[i].b != arr[i].a + arr[i].b)
	abort ();
    }

  return 0;
}

int main (void)
{
  int i;
  
  check_vect ();

  main1 ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect"  { target vect_strided2 } } } */
  
