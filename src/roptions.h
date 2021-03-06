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
#include <getopt.h>


#if !defined(RMAX)
#define RMAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif


#ifdef __cplusplus
extern "C" {

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

#endif /* __cplusplus */


unsigned optmax (struct option * options);
char * optlegitimate (struct option * options);
char * optname (struct option * options, unsigned n, unsigned val);
void usage_item (struct option * options, unsigned n, unsigned val, char * description);

/* Helpers */
void _version_ (char * progname, const char * version, char * date, char * time);
void _welcome_ (char * progname, const char * version, char * date, char * time);
char * _built_ (char * progname, const char * version, char * date, char * time);
char * _running_ (void);


#ifdef __cplusplus
}
#endif /* __cplusplus */
