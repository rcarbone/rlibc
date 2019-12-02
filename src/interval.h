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
#include <time.h>


/* Time difference */
int _days_ (time_t t2, time_t t1);
int _hours_ (time_t t2, time_t t1);
int _mins_ (time_t t2, time_t t1);

struct timeval * tvnow (void);
bool samet (struct timeval * t2, struct timeval * t1);

time_t timenow (void);
time_t seconds (struct timeval * t);
time_t milliseconds (struct timeval * t);
time_t microseconds (struct timeval * t);

/* Time difference */
int tvdays (struct timeval * t2, struct timeval * t1);
int tvhours (struct timeval * t2, struct timeval * t1);
int tvmins (struct timeval * t2, struct timeval * t1);
int tvsecs (struct timeval * t2, struct timeval * t1);
time_t tvmsecs (struct timeval * t2, struct timeval * t1);
time_t tvusecs (struct timeval * t2, struct timeval * t1);

void print_time_in_secs (struct timeval * t, char * label);

char * elapsedtime (struct timeval * t1, struct timeval * t2);
