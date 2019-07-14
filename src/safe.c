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


/* Project headers */
#include "safe.h"


void * safefree (void * a)
{
  if (a)
    free (a);
  return NULL;
}


void * safedup (void * a)
{
  return a ? strdup (a) : a;
}


void * safereplace (void * a, void * b)
{
  safefree (b);
  return a ? strdup ((char *) a) : a;
}


FILE * safeopen (char * f)
{
  return ! strcmp (f, "*") ? NULL : ! strcmp (f, "-") ? stdout : fopen (f, "a+");
}


FILE * safeclose (FILE * fd)
{
  if (fd && fd != stdout)
    fclose (fd);
  return NULL;
}
