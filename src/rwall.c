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


/* System headers */
#include <stdlib.h>
#include <sys/time.h>

/* Public header */
#include "rwall.h"

#if !defined(CLOCK_MONOTONIC_RAW)
#define CLOCK_MONOTONIC_RAW CLOCK_MONOTONIC
#endif


static rtime_t ts2ns (struct timespec ts)
{
  return ts . tv_sec * 1e9 + ts . tv_nsec;
}


/* Return current wall time in nanoseconds */
rtime_t nswall (void)
{
  struct timespec ts;
  clock_gettime (CLOCK_MONOTONIC_RAW, & ts);
  return ts2ns (ts);
}


/* Return current wall time in microseconds */
rtime_t uswall (void)
{
  struct timespec ts;
  clock_gettime (CLOCK_MONOTONIC_RAW, & ts);
  return ts2ns (ts) / 1e3;
}


/* Return current wall time in milliseconds */
rtime_t mswall (void)
{
  struct timespec ts;
  clock_gettime (CLOCK_MONOTONIC_RAW, & ts);
  return ts2ns (ts) / 1e6;
}


/* Return current wall time in seconds */
rtime_t sswall (void)
{
  struct timespec ts;
  clock_gettime (CLOCK_MONOTONIC_RAW, & ts);
  return ts . tv_sec;
}


/* Return k operations per second */
double kops (rtime_t delta, unsigned n)
{
  return (1e6 / (delta / n)) / 1e3;
}


/* Evaluate a random number in the range [0 n-1] */
unsigned rrand (unsigned n)
{
  if (! n)
    return 0;

  srand (nswall ());
  return rand () % n;
}


/* Generate pseudo-random numbers in the range [0 n-1] */
unsigned x_rand (unsigned n)
{
  struct timeval tv;

  gettimeofday (& tv, NULL);
  srand (tv . tv_sec ^ tv . tv_usec);

  return ! n ? 0 : rand () % n;
}
