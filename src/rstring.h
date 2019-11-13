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
#include <stdint.h>
#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif


char * scenter (char * s, unsigned max);
void printcenter (char * s);
void rcenter (char * s, int n);
void rupper (char * s);
void rlower (char * s);

/* Remove trailing cr [\r] and nl [\n] */
void nocrlf (char * line);


#ifdef __cplusplus
}
#endif
