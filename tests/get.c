/* System headers */
#include <stdio.h>


/* Project headers */
#include "rmatrix.h"
#include "common.c"


int main (int argc, char * argv [])
{
  unsigned rows = 3;
  unsigned cols = 8;
  unsigned r;
  unsigned c;

  mx_t * mx = mxalloc (rows, cols);
  mxpopulate (mx);

  for (r = 0; r < rows; r ++)
    for (c = 0; c < cols; c ++)
      {
	char * is = mxget (mx, r, c);
	char * be = cellfmt (r + 1, c + 1);
	if (strcmp (is, cellfmt (r + 1, c + 1)))
	  printf ("Error at [%u, %u] - [%s] vs [%s]\n", r, c, is, be);
      }

  mxfree (mx);

  return 0;
}
