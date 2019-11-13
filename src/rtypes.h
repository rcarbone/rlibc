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


/* Typedefs that rlibc needs */

/* A counter for bytes and protocol units */
typedef uint64_t r_counter_t;

/* The wall time in nsecs */
typedef uint64_t rtime_t;

/* A sorting function */
typedef int sf_t (const void * _a, const void * _b);

/* The type definition for a test function */
typedef rtime_t rmain_f (unsigned argc, void * argv []);
