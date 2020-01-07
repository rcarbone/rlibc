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
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <netdb.h>
#include <arpa/inet.h>

/* Project headers */
#include "support.h"


/* Return the hostname */
char * rnodename (void)
{
  static struct utsname machine;

  uname (& machine);

  return machine . nodename;
}


/* Return the full qualified hostname */
char * rfqname (void)
{
  struct utsname machine;
  struct hostent * he;
  struct sockaddr_in in;

  uname (& machine);

  /* Attempt to resolv hostname to get the internet address */
  he = gethostbyname (machine . nodename);

  if (he)
    memcpy (& in . sin_addr, he -> h_addr_list [0], he -> h_length);
  else
    in . sin_addr . s_addr = inet_addr (machine . nodename);

  /* Back to the full qualified domain address */
  he = gethostbyaddr ((char *) & in . sin_addr, sizeof (struct in_addr), AF_INET);

  return ! he || ! he -> h_name ? inet_ntoa (in . sin_addr) : he -> h_name;
}


/* Announce to the world! */
void rhello (char * progname, char * version, char * date, char * _time, char * nodename, char * author)
{
  time_t now = time (NULL);
  char * nowstring = ctime (& now);

  printf ("This is %s %s (%s %s)\n", progname, version, date, _time);
  printf ("Started %24.24s on %s\n", nowstring, nodename);
  printf ("By %s\n", author);
  printf ("\n");
  fflush (stdout);
}


/* Display version information */
void rversion (char * progname, char * version, char * date, char * time)
{
  printf ("%s version %s built on %s %s\n", progname, version, date, time);
  fflush (stdout);
}
