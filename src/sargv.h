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
#include <stdbool.h>
#include <string.h>


/* Project headers */
#include "vargv.h"

#if !defined(RMAX)
#define RMAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif


#ifdef __cplusplus
extern "C" {
#endif


int argsmember (char * argv [], char * item);
int argscasemember (char * argv [], char * item);
unsigned argslen (char * argv []);
char ** argsmore (char * argv [], char * s);
char ** argsless (char * argv [], char * s);
char ** argsclear (char * argv []);
char ** argsdup (char * argv []);
char ** argscat (char * a [], char * b []);
char ** argssort (char * argv []);
char * argsjoin (char * argv []);
char ** argssplit (char * str, char * sep);
char ** argspieces (char * list, char * sep);
char ** argsblanks (char * list);
char ** argsuniq (char * argv [], char * item);
bool argsexists (char * argv [], char * name);


/* evaluate widest string in argv */
unsigned argswidest (char * argv []);

void args_print_rows (char * argv [], unsigned width);
void args_print_cols (char * argv [], unsigned width);

char ** print_lines (char * argv []);
void argsrows (char * argv []);

void argsreplace (char * argv [], char * s, char * d);

#ifdef __cplusplus
}
#endif
