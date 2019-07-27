/* System headers */
#include <stdio.h>


/* Project headers */
#include "rmatrix.h"
#include "rwall.h"
#include "sargv.h"


static char * strlowerrnd (char * str, unsigned m, unsigned n)
{
  static char lower [] = "abcdefghijklmnopqrstuvxywz";

  unsigned len = m + rrand (n);
  unsigned i;

  for (i = 0; i < len; i ++)
    str [i] = lower [rrand (26)];
  str [i] = 0x0;
  return str;
}


#if defined(ROCCO)
static void strupperrnd (char * str, unsigned m, unsigned n)
{
  static char upper [] = "ABCDEFGHIJKLMNOPQRSTUVXYWZ";

  unsigned len = m + rrand (n);
  unsigned i;

  for (i = 0; i < len; i ++)
    str [i] = upper [rrand (26)];
  str [i] = 0x0;
}
#endif /* ROCCO */

#if defined(ROCCO)

#define MAXLINE 1024
char * mxcenter (char * src, unsigned max)
{
  static char dst [MAXLINE];

  unsigned n = src ? strlen (src) : 0;
  unsigned k = max > n ? (max - n) / 2 : 0;   /* initial blanks */

  dst [max] = 0x0;

  if (! n || ! max)
    return dst;

  /* Black destination string */
  memset (dst, ' ', max);

  /* Overlap source string */
  memcpy (dst + k, src, n > max ? max : n);

  return dst;
}
#endif /* ROCCO */

int main (int argc, char * argv [])
{
  unsigned rows = 3;
  unsigned cols = 8;
  mx_t * mx = mxalloc (rows, cols);
  unsigned min = 5;
  unsigned max = 9;
  unsigned r;
  unsigned c;
  char str [128];

  /* Fill matrix */
  for (r = 0; r < rows; r ++)
    for (c = 0; c < cols; c ++)
      mxcpy (mx, mxcenter (strlowerrnd (str, min, max), max), r, c);

  /* Print matrix */
  mxprint (mx);

  /* Memory cleanup */
  mxfree (mx);

  /* Bye bye! */
  return 0;
}
