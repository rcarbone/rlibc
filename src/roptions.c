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
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/utsname.h>

/* Project headers */
#include "roptions.h"


/* Evaluate the max length of option names */
unsigned optmax (struct option * options)
{
  unsigned max = 0;

  /* Iterate over all options */
  while ((* options) . name)
    {
      max = RMAX (max, strlen ((* options) . name));
      options ++;
    }
  return max;
}


/* Evaluate the string containing the legitimate short option characters */
char * optlegitimate (struct option * options)
{
  static char buffer [1024];

  buffer [0] = 0x0;

  /* Iterate over all options */
  while ((* options) . name)
    {
      if (isascii ((* options) . val))
	{
	  switch ((* options) . has_arg)
	    {
	    case optional_argument: sprintf (buffer + strlen (buffer), "%c::", (* options) . val); break;
	    case required_argument: sprintf (buffer + strlen (buffer), "%c:",  (* options) . val); break;
	    case no_argument:	    sprintf (buffer + strlen (buffer), "%c",   (* options) . val); break;
	    }
	}
      options ++;
    }
  return buffer;
}


/* Lookup for a name in the table of long options */
char * optname (struct option * options, unsigned n, unsigned val)
{
  static char buffer [1024];

  /* Iterate over all options */
  while ((* options) . name)
    if ((* options) . val == val)
      break;
    else
      options ++;

  n += 9;  /* <val> */
  if ((* options) . name && (* options) . has_arg == required_argument)
    {
      char required [1024];
      sprintf (required, "%s <val>", (* options) . name);
      sprintf (buffer, "%-*.*s", n, n, required);
    }
  else
    sprintf (buffer, "%-*.*s", n, n, (* options) . name ? (* options) . name : "???");
  return buffer;
}


/* Display a single item of the usage */
void usage_item (struct option * options, unsigned n, unsigned val, char * description)
{
  if (! description)
    description = "";

  if (isascii (val))
    printf ("   -%c | --%s  %s\n", val, optname (options, n, val), description);
  else
    printf ("        --%s  %s\n", optname (options, n, val), description);
}


/* Display version information */
void _version_ (char * progname, const char * version, char * date, char * time)
{
  printf ("%s, ver. %s built on %s %s\n", progname, version, date, time);
  fflush (stdout);
}


/* Welcome on board! */
void _welcome_ (char * progname, const char * version, char * date, char * _time_)
{
  time_t now = time (NULL);
  struct utsname u;
  uname (& u);
  _version_ (progname, version, date, _time_);
  printf ("\n");
  printf ("Running on : %s %s %s %s %s\n", u . sysname, u . nodename, u . release, u . version, u . machine);
  printf ("Local time : %s", ctime (& now));
  printf ("\n");
  fflush (stdout);
}


char * _built_ (char * progname, const char * version, char * date, char * time)
{
  static char buf [1024];

  sprintf (buf, "%s, ver. %s built on %s %s", progname, version, date, time);
  return buf;
}


char * _running_ (void)
{
  static char buf [1024];

  struct utsname u;
  uname (& u);
  sprintf (buf, "%s %s %s %s %s", u . sysname, u . nodename, u . release, u . version, u . machine);
  return buf;
}
