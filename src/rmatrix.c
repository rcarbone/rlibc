/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *                   _ _ _
 *              _ __| (_) |__   ___
 *             | '__| | | '_ \ / __|
 *             | |  | | | |_) | (__
 *             |_|  |_|_|_.__/ \___|
 *
 *         C library of useful functions
 *
 * R. Carbone (rocco@tecsiel.it)
 *   2Q 2019
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */


/* System headers */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Project headers */
#include "rmatrix.h"

/*
 * Use a single array instead of an array of arrays and access with i = [r * cols + c]
 *
 *   TODO: what about j = [c * rows + r] ?
 *
 * Given a value i in [0 - rows*cols - 1] then
 *   r = (i - (i % cols)) / cols
 *   c = (i % cols)
 */



/* Useful macros */
#define MXMIN(a, b) (((a) < (b)) ? (a) : (b))
#define MXMAX(a, b) (((a) > (b)) ? (a) : (b))

#define MAXLINE 1024


struct mx
{
  unsigned rows;
  unsigned cols;
  char ** data;

};


/* Check a cell content for equality */
static bool mxcelleq (char * cell1, char * cell2)
{
  if (! cell1 && ! cell2)
    return true;

  return cell1 && cell2 ? strcmp (cell1, cell2) : false;
}


/* A rule for pretty printing the matrix */
char * mxrule (mx_t * mx)
{
  static char line [MAXLINE];

  unsigned pos = 0;
  unsigned c;

  memset (line, '-', sizeof (line) - 1);

  line [pos] = '+';
  for (c = 0; c < mxcols (mx); c ++)
    {
      pos += mxcolmaxlen (mx, c) + 3;
      line [pos] = '+';
    }
  line [pos ++] = '+';
  line [pos ++] = '\n';
  line [pos] = 0x0;

  return line;
}


/* Allocate a bidimensional matrix [rows x cols] */
mx_t * mxalloc (unsigned rows, unsigned cols)
{
  mx_t * mx;

  if (! rows || ! cols)
    return NULL;

  mx = calloc (1, sizeof (* mx));
  mx -> rows = rows;
  mx -> cols = cols;
  mx -> data = calloc (rows * cols, sizeof (char *));

  return mx;
}


/* Free a bidimensional matrix [rows x cols] */
void mxfree (mx_t * mx)
{
  unsigned i;

  if (! mx)
    return;

  for (i = 0; i < mx -> rows * mx -> cols; i ++)
    if (mx -> data [i])
      free (mx -> data [i]);

  free (mx -> data);
  free (mx);
}


/* Free a bidimensional matrix [rows x cols] */
void mxrm (mx_t * mx)
{
  if (! mx)
    return;

  free (mx -> data);
  free (mx);
}


/* Check if a matrix is empty */
bool mxisempty (mx_t * mx)
{
  unsigned i;

  if (! mx)
    return true;

  for (i = 0; i < mx -> rows * mx -> cols; i ++)
    if (mx -> data [i])
      return false;

  return true;
}


void mxclear (mx_t * mx)
{
  unsigned i;

  if (! mx)
    return;

  for (i = 0; i < mx -> rows * mx -> cols; i ++)
    mx -> data [i] = NULL;
}


unsigned mxrows (mx_t * mx)
{
  return mx ? mx -> rows : 0;
}


unsigned mxcols (mx_t * mx)
{
  return mx ? mx -> cols : 0;
}


char ** mxdata (mx_t * mx)
{
  return mx ? mx -> data : NULL;
}


unsigned mxsize (mx_t * mx)
{
  unsigned n = 0;
  unsigned i;

  if (! mx || ! mx -> rows || ! mx -> cols || ! mx -> data)
    return 0;

  for (i = 0; i < mx -> rows * mx -> cols; i ++)
    if (mx -> data [i])
      n ++;

  return n;
}


/* Get an element at given row and column [r, c] */
char * mxget (mx_t * mx, unsigned r, unsigned c)
{
  return mx && r < mx -> rows && c < mx -> cols ? mx -> data [r * mx -> cols + c] : NULL;
}


/* Set an element at given row and column [r, c] */
void mxset (mx_t * mx, char * data, unsigned r, unsigned c)
{
  if (mx && r < mx -> rows && c < mx -> cols && data)
    mx -> data [r * mx -> cols + c] = data;
}


/* Copy an element at given row and column [r, c] */
void mxcpy (mx_t * mx, char * data, unsigned r, unsigned c)
{
  if (mx && r < mx -> rows && c < mx -> cols && data)
    mx -> data [r * mx -> cols + c] = strdup (data);
}


/* Get all elements at given row [r] */
char ** mxgetrow (mx_t * mx, unsigned r)
{
  char ** rows = NULL;
  unsigned c;

  if (! mx || ! mx -> cols || r >= mx -> rows)
    return NULL;

  rows = calloc (mx -> cols, sizeof (char *));

  for (c = 0; c < mx -> cols; c ++)
    rows [c] = mx -> data [r * mx -> cols + c];

  return rows;
}


/* Get all elements at given column [c] */
char ** mxgetcol (mx_t * mx, unsigned c)
{
  char ** cols = NULL;
  unsigned r;

  if (! mx || ! mx -> rows || c >= mx -> cols)
    return NULL;

  cols = calloc (mx -> rows, sizeof (char *));

  for (r = 0; r < mx -> rows; r ++)
    cols [r] = mx -> data [r * mx -> cols + c];

  return cols;
}


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


/* Print a matrix */
void mxprint (mx_t * mx)
{
  unsigned r;
  unsigned c;
  unsigned n;

  if (! mx)
    return;

  printf ("%s", mxrule (mx));

  for (r = 0; r < mx -> rows; r ++)
    {
      printf ("|");
      for (c = 0; c < mx -> cols; c ++)
	{
	  n = mxcolmaxlen (mx, c);
	  printf (" %-*.*s |", n, n, mx -> data [r * mx -> cols + c]);
	}
      printf ("\n");

      if (r == 0)
	printf ("%s", mxrule (mx));
    }
  printf ("%s", mxrule (mx));
}


/* Print a matrix in reverse order */
void mxrevprint (mx_t * mx)
{
  unsigned r;
  unsigned c;

  if (! mx)
    return;

  for (c = 0; c < mx -> cols; c ++)
    {
      printf ("| ");
      for (r = 0; r < mx -> rows; r ++)
	printf (" %9.9s |", mx -> data [r * mx -> cols + c]);
      printf ("\n");
    }
}


/* Compare 2 matrices */
bool mxcmp (mx_t * mx1, mx_t * mx2)
{
  unsigned r;
  unsigned c;

  if (! mx1 || ! mx2 || mx1 -> rows != mx2 -> rows || mx1 -> cols != mx2 -> cols)
    return false;

  for (r = 0; r < mx1 -> rows; r ++)
    for (c = 0; c < mx1 -> cols; c ++)
      if (mxcelleq (mx1 -> data [r * mx1 -> cols + c], mx2 -> data [r * mx2 -> cols + c]))
	return false;
  return true;
}


/* Get max len of all cells at given column [c] */
unsigned mxcolmaxlen (mx_t * mx, unsigned c)
{
  unsigned max = 0;
  unsigned r;

  if (! mx || ! mx -> rows || c >= mx -> cols)
    return 0;

  for (r = 0; r < mx -> rows; r ++)
    if (mx -> data [r * mx -> cols + c])
      max = MXMAX (max, strlen (mx -> data [r * mx -> cols + c]));

  return max;
}
