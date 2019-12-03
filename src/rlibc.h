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

#include "safe.h"
#include "vargv.h"
#include "sargv.h"

#include "rtypes.h"
#include "replace.h"
#include "rstring.h"
#include "support.h"
#include "rctype.h"
#include "rwall.h"
#include "interval.h"
#include "elapsed.h"
#include "roptions.h"
#include "gnode.h"
#include "rmatrix.h"
#include "xrand.h"
#include "parsecfg.h"
#include "fmemdmp.h"
#include "other.h"
#include "rsi.h"


/* Public functions in file gnu-argv.c */
char ** dupargv (char * const *argv);
void freeargv (char **vector);
char **buildargv (const char *input);
int writeargv (char * const *argv, FILE *f);
void expandargv (int *argcp, char ***argvp);
int countargv (char * const *argv);


/* Public functions in file curses.c */
void initialize_curses (void);
void terminate_curses (void);
int get_cols (void);
int get_rows (void);

/* Public functions in file xlog.c */
void xloginit (char * progname, char * node, char * logname, struct timeval * started, bool fmt, bool flush);
void xlogterm (void);
int xlog (const char * fmt, ...);
bool getlogformat (void);
void datelogformat (void);
void timelogformat (void);
void togglelogformat (void);


char * tvtouptime (struct timeval * tv);
