/* System headers */
#include <stdio.h>
#include <string.h>


/* Format a buffer for test */
char * cellfmt (unsigned r, unsigned c)
{
  static char cell [128];
  sprintf (cell, "r%02u - c%02u", r, c);
  return cell;
}


char * celldup (unsigned r, unsigned c)
{
  return strdup (cellfmt (r, c));
}


/* Populate a matrix (all cells formatted with fixed different content for testing purposes) */
void mxpopulate (mx_t * mx)
{
  unsigned i;

  if (! mx)
    return;

  for (i = 0; i < mxrows (mx) * mxcols (mx); i ++)
    mxdata (mx) [i] = celldup (((i - (i % mxcols (mx))) / mxcols (mx)) + 1, (i % mxcols (mx)) + 1);
}


void mxprint_as_array (mx_t * mx)
{
  unsigned i;

  if (! mx)
    return;

  for (i = 0; i < mxrows (mx) * mxcols (mx); i ++)
    printf ("%2u: %s\n", i + 1, mxdata (mx) [i]);
}


/* Populate a matrix (using row x col access) */
void mxfill (mx_t * mx)
{
  unsigned r;
  unsigned c;

  if (! mx)
    return;

  for (r = 0; r < mxrows (mx); r ++)
    for (c = 0; c < mxcols (mx); c ++)
      mxdata (mx) [r * mxcols (mx) + c] = celldup (r + 1, c + 1);
}


/* Populate all the columns of a given row */
void mxfill_row (mx_t * mx, unsigned r)
{
  unsigned c;

  if (! mx)
    return;

  for (c = 0; c < mxcols (mx); c ++)
    mxdata (mx) [r * mxcols (mx) + c] = celldup (r + 1, c + 1);
}


/* Populate all the rows of a given column */
void mxfill_col (mx_t * mx, unsigned c)
{
  unsigned r;

  if (! mx)
    return;

  for (r = 0; r < mxrows (mx); r ++)
    mxdata (mx) [r * mxcols (mx) + c] = celldup (r + 1, c + 1);
}


/* Populate by cell */
void mxfill_cell (mx_t * mx, unsigned r, unsigned c)
{
  mxset (mx, celldup (r + 1, c + 1), r, c);
}
