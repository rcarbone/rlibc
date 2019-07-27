/* System headers */
#include <stdio.h>


/* Project headers */
#include "rmatrix.h"
#include "common.c"


int main (int argc, char * argv [])
{
  unsigned rows = 3;
  unsigned cols = 8;

  mx_t * mx = mxalloc (rows, cols);
  mxpopulate (mx);

  /* Print as array */
  printf ("Print as array\n");
  mxprint_as_array (mx);
  printf ("\n");

  printf ("Print row by col [%u x %u]\n", rows, cols);
  mxprint (mx);
  printf ("\n");

  printf ("Print col by row [%u x %u]\n", cols, rows);
  mxrevprint (mx);

  mxfree (mx);

  /* Bye bye! */
  return 0;
}
