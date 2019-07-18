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


#ifdef __cplusplus
extern "C" {
#endif


/* System headers */
#include <stdbool.h>


/*
 * Public Datatypes. The internals of the structures are all private
 */

/* Opaque type for a matrix
 *
 * A variable of type mx_t represents a matrix.
 * Create one with mxalloc() and free with mxfree().
 */
typedef struct mx mx_t;


/*
 * Public functions
 */

char * mxrule (mx_t * mx);
mx_t * mxalloc (unsigned rows, unsigned cols);
void mxfree (mx_t * mx);
void mxrm (mx_t * mx);

bool mxisempty (mx_t * mx);
void mxclear (mx_t * mx);
unsigned mxrows (mx_t * mx);
unsigned mxcols (mx_t * mx);
char ** mxdata (mx_t * mx);
unsigned mxsize (mx_t * mx);

char * mxget (mx_t * mx, unsigned r, unsigned c);
void mxset (mx_t * mx, char * data, unsigned r, unsigned c);
void mxcpy (mx_t * mx, char * data, unsigned r, unsigned c);
char ** mxgetrow (mx_t * mx, unsigned r);
char ** mxgetcol (mx_t * mx, unsigned c);

char * mxcenter (char * src, unsigned max);
void mxprintf (mx_t * mx, FILE * fp);
void mxprint (mx_t * mx);
void mxrevprint (mx_t * mx);

bool mxcmp (mx_t * mx1, mx_t * mx2);

unsigned mxcolmaxlen (mx_t * mx, unsigned c);


#ifdef __cplusplus
}
#endif
