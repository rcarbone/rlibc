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


#pragma once


/* System headers */
#include <string.h>


/* =-= String helpers =-= */

/* Trim leading and trailing characters [c] from [str] */
char * rtrim_leading (char * str, char c);
char * rtrim_trailing (char * str, char c);
char * rtrim (char * str, char c);
void   rtrimcrlf (char * str);
