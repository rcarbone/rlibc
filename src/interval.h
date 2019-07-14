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

/* Time difference */
int days (struct timeval * t2, struct timeval * t1);
int hours (struct timeval * t2, struct timeval * t1);
int mins (struct timeval * t2, struct timeval * t1);
int secs (struct timeval * t2, struct timeval * t1);
time_t msecs (struct timeval * t2, struct timeval * t1);
time_t usecs (struct timeval * t2, struct timeval * t1);

char * elapsedtime (struct timeval * t1, struct timeval * t2);
