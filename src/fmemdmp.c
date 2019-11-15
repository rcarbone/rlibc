/*
 * fmemdmp.c - Routines to dump memory areas
 */


/* System headers */
#include <stdio.h>
#include <ctype.h>

/* Public header */
#include "fmemdmp.h"


/*
 * Pretty print function
 *
 * This function dumps a buffer in memory in the (pretty !!!) format
 *
 *   off:  printable          hexadecimal notation
 * --------------------------------------------------------------------------
 *
 * Dump of memory area at address 0x10000444 for 51 bytes
 *     0:  abcdefghijklmnop   61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70
 *    16:  qrstuvzxyw012345   71 72 73 74 75 76 7a 78 79 77 30 31 32 33 34 35
 *    32:  6789~!@#$%^&*()_   36 37 38 39 7e 21 40 23 24 25 5e 26 2a 28 29 5f
 *    48:  -+=                2d 2b 3d
 *
 * Dump of memory area at address 0x7fffbc03 for 16 bytes
 *    0:  rocco@tecsiel.it    72 6f 63 63 6f 40 74 65 63 73 69 65 6c 2e 69 74
 */
void fmemdmp (FILE * fd, char * ptr, unsigned size, char * label)
{
  unsigned bytes_per_line = 16;
  unsigned offset = 0;
  unsigned total;

  if (! ptr || ! size)
    return;

  if (label && * label)
    fprintf (fd, "\"%s\" at address %p for %d bytes\n", label, ptr, size);

  for (total = 0; total < size; total += bytes_per_line)
    {
      unsigned i = 0;

      /* Print the offset */
      fprintf (fd, "%6d:  ", offset);

      /* Print the bytes in a line (each byte in ASCII notation) */
      for (i = 0; i < bytes_per_line; i ++)
	if (total + i < size)
	  fprintf (fd, "%c", isprint (* (ptr + total + i) & 0x000000ff) ? (* (ptr + total + i)) : '.');
	else
	  fprintf (fd, " "); /* 1 blank character */

      /* Print the separator */
      fprintf (fd, "  ");

      /* Print the bytes in a line (each byte in Hexadecimal notation) */
      for (i = 0; i < bytes_per_line && i < size; i ++)
	if (total + i < size)
	  fprintf (fd, "%02x ", * (ptr + total + i) & 0x000000ff);
	else
	  fprintf (fd, "   "); /* 3 more blanks characters */

      fprintf (fd, "\n");
      offset += bytes_per_line;
    }
  fflush (fd);
}


void memdmp (char * ptr, unsigned size, char * label)
{
  fmemdmp (stdout, ptr, size, label);
}


void mem2c (char * ptr, unsigned size, char * name)
{
  unsigned bytes_per_line = 16;
  unsigned total;

  if (! ptr || ! size || ! name)
    return;

  printf ("static char %s [%u] =\n", name, size);
  printf ("{\n");

  for (total = 0; total < size; total += bytes_per_line)
    {
      unsigned i = 0;

      /* Indent 1 blank */
      printf (" ");

      /* Print the bytes in a line (each byte in hexadecimal notation) */
      for (i = 0; i < bytes_per_line && i < size; i ++)
	if (total + i < size)
	  printf (" 0x%02x%c", * (ptr + total + i) & 0xff, total + i < size - 1 ? ',' : ' ');

      printf ("\n");
    }

  printf ("};\n");
}
