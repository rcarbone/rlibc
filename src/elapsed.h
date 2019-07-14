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

/* Project headers */
#include "rtypes.h"
#include "rwall.h"


#ifdef __cplusplus
extern "C" {
#endif


/* Return a well formatted string rapresenting given time at nanosecond resolution */
char * ns2a (rtime_t nsecs);
char * relapsed (rtime_t nsecs);
void show_elapsed (rtime_t min, rtime_t avg, rtime_t max, rtime_t elapsed);

#ifdef __cplusplus
}
#endif
