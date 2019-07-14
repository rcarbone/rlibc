/* System headers */
#include <stdio.h>


/* Project headers */
#include "rmatrix.h"


int main (int argc, char * argv [])
{
  unsigned rows = 3;
  unsigned cols = 8;

  mxfree (mxalloc (rows, cols));

  return 0;
}
