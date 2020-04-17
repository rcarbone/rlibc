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
#include "rtrim.h"


/* Trim leading characters [c] from [str] */
char * rtrim_leading (char * str, char c)
{
  if (! str)
    return str;

  while (* str && * str == c)
    str ++;
  return str;
}


/* Trim trailing characters [c] from [str] */
char * rtrim_trailing (char * str, char c)
{
  char * e;

  if (! str)
    return str;

  e = str + strlen (str) - 1;
  while (e > str && * e == c)
    * e -- = '\0';
  return str;
}


/* Trim leading and trailing characters [c] from [str] */
char * rtrim (char * str, char c)
{
  char * e;

  if (! str)
    return str;

  e = str + strlen (str) - 1;
  while (* str && * str == c)
    str ++;
  while (e > str && * e == c)
    * e -- = '\0';
  return str;
}


/* Remove both trailing cr [\r] and/or nl [\n] */
void rtrimcrlf (char * str)
{
  if (str)
    while (* str)
      {
	if (str [strlen (str) - 1] == '\n' || str [strlen (str) - 1] == '\r')
	  str [strlen (str) - 1] = 0x00;
	str ++;
      }
}
