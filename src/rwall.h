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
#include <sys/time.h>

/* Project headers */
#include "rtypes.h"


#ifdef __cplusplus
extern "C" {
#endif


/* Current wall time in nsec/usec/msec/sec */
rtime_t nswall (void);
rtime_t uswall (void);
rtime_t mswall (void);
rtime_t sswall (void);
double kops (rtime_t delta, unsigned n);

unsigned rrand (unsigned n);
unsigned x_rand (unsigned n);
void ** varnd (unsigned argc, void * argv []);


#ifdef __cplusplus
}
#endif
