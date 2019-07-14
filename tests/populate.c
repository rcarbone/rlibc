/* System headers */
#include <stdio.h>
#include <stdlib.h>


/* Project headers */
#include "rmatrix.h"
#include "common.c"


int main (int argc, char * argv [])
{
  unsigned rows = 3;
  unsigned cols = 8;
  unsigned r;
  unsigned c;

  mx_t * mx1 = mxalloc (rows, cols);
  mx_t * mx2 = mxalloc (rows, cols);
  mx_t * mx3 = mxalloc (rows, cols);
  mx_t * mx4 = mxalloc (rows, cols);
  mx_t * mx5 = mxalloc (rows, cols);
  mx_t * mx6 = mxalloc (rows, cols);
  mx_t * mx7 = mxalloc (rows, cols);

  /* Initialize the matrix (sequential) */
  mxpopulate (mx1);

  /* Print the matrix */
  mxprint (mx1);

  /* Initialize the matrix */
  mxfill (mx2);
  if (! mxcmp (mx1, mx2))
    printf ("Different\n");

  /* Initialize the matrix by rows */
  for (r = 0; r < rows; r ++)
    mxfill_row (mx3, r);
  if (! mxcmp (mx1, mx3))
    printf ("Different\n");

  /* Initialize the matrix by cols */
  for (c = 0; c < cols; c ++)
    mxfill_col (mx4, c);
  if (! mxcmp (mx1, mx4))
    printf ("Different\n");

  /* Initialize the matrix by cells */
  for (r = 0; r < rows; r ++)
    for (c = 0; c < cols; c ++)
      mxfill_cell (mx5, r, c);
  if (! mxcmp (mx1, mx5))
    printf ("Different\n");

  /* Get row by row and initialize the matrix by cells */
  for (r = 0; r < rows; r ++)
    {
      char ** all = mxgetrow (mx1, r);
      for (c = 0; c < cols; c ++)
	mxcpy (mx6, all [c], r, c);
      free (all);
    }
  if (! mxcmp (mx1, mx6))
    printf ("Different\n");
  else
    printf ("Same\n");

  /* Get col by col and initialize the matrix by cells */
  for (c = 0; c < cols; c ++)
    {
      char ** all = mxgetcol (mx1, c);
      for (r = 0; r < rows; r ++)
	mxcpy (mx7, all [r], r, c);
      free (all);
    }
  if (! mxcmp (mx1, mx7))
    printf ("Different\n");
  else
    printf ("Same\n");

  mxfree (mx7);
  mxfree (mx6);
  mxfree (mx5);
  mxfree (mx4);
  mxfree (mx3);
  mxfree (mx2);
  mxfree (mx1);

  return 0;
}
