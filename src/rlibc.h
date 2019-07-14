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

#include "rctype.h"
#include "support.h"
#include "rstring.h"
#include "rwall.h"
#include "interval.h"
#include "elapsed.h"
#include "roptions.h"
#include "gnode.h"
#include "rmatrix.h"


/* Public functions in file gnu-argv.c */
char ** dupargv (char * const *argv);
void freeargv (char **vector);
char **buildargv (const char *input);
int writeargv (char * const *argv, FILE *f);
void expandargv (int *argcp, char ***argvp);
int countargv (char * const *argv);
