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
#include <stdarg.h>

/* Constants */
#define BUFNO   1024               /* # of small buffers */
#define BUFLEN  4096               /* small buffer size  */


/* Select a buffer in a pool of static buffers */
static char * getbuf (void)
{
  static char buffers [BUFNO] [BUFLEN];   /* pool    */
  static unsigned idx = 0;                /* index   */

  char * s = buffers [idx];               /* current */
  idx = (idx + 1) % BUFNO;                /* next    */
  return s;
}


/* Format a local buffer */
char * rfmt (char * fmt, ...)
{
  char * s = getbuf ();       /* pointer to a local buffer */
  va_list va_ap;
  va_start (va_ap, fmt);
  vsprintf (s, fmt, va_ap);
  va_end (va_ap);
  return s;
}
