/* System headers */
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/* Project headers */
#include "xrand.h"
#include "rwall.h"


/* Constants */
#define MAXROWS 10
#define MAXRND  128


/*
 * http://stackoverflow.com/questions/6127503/shuffle-array-in-c
 *
 * Arrange the N elements of ARRAY in random order.
 * Only effective if N is much smaller than RAND_MAX;
 * if this may not be the case, use a better random
 * number generator.
 */
static void shuffle (unsigned n, unsigned * array)
{
  srand (nswall ());

  if (n > 1)
    {
      unsigned i;
      unsigned j;
      unsigned t;
      for (i = 0; i < n - 1; i ++)
        {
          j = i + rand () / (RAND_MAX / (n - i) + 1);
          t = array [j];
          array [j] = array [i];
          array [i] = t;
        }
    }
}


/* Generate pseudo-random numbers in the range [0 n-1] */
unsigned xrand (unsigned n)
{
  struct timeval tv;

  gettimeofday (& tv, NULL);
  srand (tv . tv_sec ^ tv . tv_usec);

  return ! n ? 0 : rand () % n;
}


/* Generate pseudo-random strings of length [len] */
char * mkrndstr (unsigned len)
{
  static char charset [] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  static char rndstr [MAXROWS] [MAXRND + 1];
  static unsigned row = 0;

  char * dst = rndstr [row];
  unsigned n;

  memset (dst, 0x0, MAXRND + 1);

  row = (row + 1) % MAXROWS;

  for (n = 0; n < len && n < MAXRND; n ++)
    dst [n] = charset [xrand (sizeof (charset) - 1)];

  dst [n] = '\0';

  return dst;
}


/* Evaluate a random array of the given length with items in [0 - n-1] */
unsigned * rndarray (unsigned n)
{
  unsigned * array = (unsigned *) calloc (n, sizeof (unsigned));
  unsigned i;
  for (i = 0; i < n; i ++)
    array [i] = i;
  shuffle (n, array);
  return array;
}
