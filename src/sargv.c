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


/*
 * Utilities  to handle dynamic arrays of strings
 */

#ifdef __cplusplus
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wwrite-strings"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif
#endif /* __cplusplus */


/* Project headers */
#include "sargv.h"
#include "safe.h"


static int argscmp (const void * _a, const void * _b)
{
  return strcmp (* (char **) _a, * (char **) _b);
}


/* Lookup for an element into the table of arguments */
int argsmember (char * argv [], char * item)
{
  int i = -1;

  while (item && argv && * argv)
    if (! strcmp (* argv ++, item))
      return i + 1;
    else
      i ++;

  return -1;
}


/* Lookup for an element into the table of arguments */
int argscasemember (char * argv [], char * item)
{
  int i = -1;

  while (item && argv && * argv)
    if (! strcasecmp (* argv ++, item))
      return i + 1;
    else
      i ++;

  return -1;
}


/* Return the # of items in an array */
unsigned argslen (char * argv [])
{
  return valen ((void **) argv);
}


/* Add an element to an array */
char ** argsmore (char * argv [], char * s)
{
  return s ? (char **) vamore ((void **) argv, strdup (s)) : argv;
}


/* Remove an item from an array */
char ** argsless (char * argv [], char * s)
{
  int argc = 0;
  int i;
  int j;

  if ((i = argsmember (argv, s)) != -1)
    {
      argc = argslen (argv);
      free (argv [i]);                   /* free the argument */

      for (j = i; j < argc - 1; j ++)    /* move pointers back one position */
	argv [j] = argv [j + 1];

      argv [j] = NULL;                   /* terminate the table */

      if (argc > 1)
	argv = (char **) realloc (argv, argc * sizeof (char *));  /* the size is argc not argc-1 because of trailing NULL */
      else
	free (argv);
    }

  return argc > 1 ? argv : NULL;
}


/* Free memory associated to an array */
char ** argsclear (char * argv [])
{
  return (char **) vaclear ((void **) argv, free);
}


/* Duplicate array 'argv' */
char ** argsdup (char * argv [])
{
  char ** dst = NULL;
  while (argv && * argv)
    dst = argsmore (dst, * argv ++);
  return dst;
}


/* Concatenate array 'b' to 'a' */
char ** argscat (char * a [], char * b [])
{
  return (char **) vacat ((void **) a, (void **) b);
}


char ** argssort (char * argv [])
{
  return (char **) vasort ((void **) argv, argscmp);
}


/* Join items in argv[] */
char * argsjoin (char * argv [])
{
  unsigned size = 0;
  char * join = NULL;
  while (argv && * argv)
    {
      size += (strlen (* argv) + 1 + 1);  /* '\n' plus a blank separator */
      if (join)
	{
	  strcat (join, " ");
	  join = (char *) realloc (join, size);
	  strcat (join, * argv ++);
	}
      else
	{
	  join = (char *) calloc (size + 1, 1);  /* one more char to avoid strncat overflow */
	  strcpy (join, * argv ++);
	}
    }
  return join;
}


/* Split a string into pieces */
char ** argssplit (char * str, char * sep)
{
  char ** argv = NULL;
  char * rest = NULL;
  char * param;
  char * data;
  char * m;

  if (! str || ! sep)
    return NULL;

  data = strdup (str);                       /* this is due strtok_r() modifies the input buffer 'str' */
  m = data;

  param = strtok_r (data, sep, & rest);
  while (param)
    {
      /* Add current field to the array */
      argv = argsmore (argv, param);

      /* Process empty fields (add the separator) */
      if (rest && * rest == * sep)
	{
	  char * p = rest;
	  while (* p ++ == * sep)
	    argv = argsmore (argv, sep);
	}
      /* Next field */
      param = strtok_r (NULL, sep, & rest);
    }

  if (m)
    free (m);

  return argv;
}


/* Split a string into pieces */
char ** argspieces (char * list, char * sep)
{
  char ** argv = NULL;
  char * param;
  char * names = list ? strdup (list) : NULL;

  if (names)
    while ((param = strtok (! argv ? names : NULL, sep)))
      argv = argsmore (argv, param);

  if (names)
    free (names);

  return argv;
}


/* Split a blank separated list of strings into pieces */
char ** argsblanks (char * list)
{
  return argspieces (list, " ");
}


/* Lookup for a name in a table */
bool argsexists (char * argv [], char * name)
{
  while (name && argv && * argv)
    if (! strcmp (* argv ++, name))
      return true;
  return false;
}


/* Add an item to a table (if not already in) */
char ** argsuniq (char * argv [], char * item)
{
  return argsexists (argv, item) ? argv : argsmore (argv, item);
}


/* Evaluate widest string in argv */
unsigned argswidest (char * argv [])
{
  unsigned widest = 0;

  while (argv && * argv)
    {
      widest = RMAX (widest, strlen (* argv));
      argv ++;
    }
  return widest;
}


int args_iter (unsigned rows, unsigned cols, unsigned r, unsigned c)
{
  return r < rows - 1 && c < cols - 1 ? r * cols + c : -1;
}


int args_iter_tx (unsigned rows, unsigned cols, unsigned r, unsigned c)
{
  return r < rows - 1 && c < cols - 1 ? c * rows + r : -1;
}


/* Print items by rows - use a single array instead of an array of arrays and access with [r * cols + c] */
void args_print_rows (char * argv [], unsigned width)
{
  unsigned argc = arrlen (argv);
  unsigned max  = argswidest (argv) + 1;    /* add separator */
  unsigned rows;
  unsigned cols;
  unsigned r;
  unsigned c;

  /* how many columns? */
  cols = width / max;
  if (cols == 0)
    cols = 1;

  /* how many rows? */
  rows = (argc + cols - 1) / cols;

  for (r = 0; r < rows; r ++)
    {
      for (c = 0; c < cols; c ++)
	if (r * cols + c < argc)
	  printf ("%-*.*s", max, max, argv [r * cols + c]);
	else
	  break;
      printf ("\n");
    }
}


/* Print items by columns - use a single array instead of an array of arrays and access with [c * rows + r] */
void args_print_cols (char * argv [], unsigned width)
{
  unsigned argc = arrlen (argv);
  unsigned max  = argswidest (argv) + 1;    /* add separator */
  unsigned rows;
  unsigned cols;
  unsigned r;
  unsigned c;

  /* how many columns? */
  cols = width / max;
  if (cols == 0)
    cols = 1;

  /* how many rows? */
  rows = (argc + cols - 1) / cols;

  for (r = 0; r < rows; r ++)
    {
      for (c = 0; c < cols; c ++)
	if (c * rows + r < argc)
	  printf ("%-*.*s", max, max, argv [c * rows + r]);
	else
	  break;
      printf ("\n");
    }
}


/* Print all items in [argv] - each item in its own line terminated by [newline] */
char ** print_lines (char * argv [])
{
  char ** lines = argv;
  if (argv)
    while (* argv)
      printf ("%s\n", * argv ++);
  return lines;
}


/* Print all items in [argv] - each item in its own line prefixed by counter and terminated by [newline] */
void argsrows (char * argv [])
{
  unsigned argc = 0;
  if (argv)
    while (* argv)
      printf ("%3d. \"%s\"\n", ++ argc, * argv ++);
}


/* Replace the element 's' in the array of arguments with 'd' */
void argsreplace (char * argv [], char * s, char * d)
{
  int i;

  if ((i = argsmember (argv, s)) != -1)
    {
      safefree (argv [i]);                   /* free the argument */
      argv [i] = strdup (d);
    }
}
