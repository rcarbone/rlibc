/* System headers */
#include <stdio.h>
#include <string.h>


/* Project headers */
#include "rmatrix.h"


int main (void)
{
  unsigned rows = 3;
  unsigned cols = 8;
  char * obj = "bar";         /* user object   */
  char * val;                 /* stored object */
  mx_t * mx;

  printf ("Containers can be divided into two groups\n");
  printf ("=========================================\n");
  printf ("\n");
  printf (" o value based containers\n");
  printf (" o reference based containers\n");
  printf ("\n");

  printf ("Value based containers store copies of objects.\n");
  printf ("Reference based containers store pointers to objects.\n");
  printf ("\n");

  printf ("This program checks the group of the implementation ... ");

  /* Create a matrix */
  mx = mxalloc (rows, cols);

  /* Push an object */
  mxcpy (mx, obj, 0, 0);

  /* Get an object */
  val = mxget (mx, 0, 0);
  if (val && ! strcmp (obj, val))
    {
      if (val == obj)
	printf ("reference based!\n");
      else
	printf ("value based!\n");
    }
  else
    {
      printf ("Error\n");
      if (val)
	{
	  printf ("user   (%s)\n", obj);
	  printf ("stored (%s)\n", val);
	}
    }

  /* Destroy the matrix */
  mxfree (mx);

  printf ("This program checks the group of the implementation ... ");

  /* Create a matrix */
  mx = mxalloc (rows, cols);

  /* Push an object */
  mxset (mx, obj, 0, 0);

  /* Get an object */
  val = mxget (mx, 0, 0);
  if (val && ! strcmp (obj, val))
    {
      if (val == obj)
	printf ("reference based!\n");
      else
	printf ("value based!\n");
    }
  else
    {
      printf ("Error\n");
      if (val)
	{
	  printf ("user   (%s)\n", obj);
	  printf ("stored (%s)\n", val);
	}
    }

  /* Destroy the matrix */
  mxrm (mx);

  return 0;
}
