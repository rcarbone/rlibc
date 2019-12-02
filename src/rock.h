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


/* dynamically resizeable vector of [void *] */

#pragma once


#ifdef __cplusplus
extern "C" {
#endif


/* System headers */
#include <stdbool.h>


/*
 * Public Datatypes. The internals of the structures are all private
 */

/* Opaque type for a vector
 *
 * A variable of type vx_t represents a vector.
 * Create one with vxalloc() and free with vxfree().
 */
typedef struct vx vx_t;


/*
 * Public functions
 */

char * vxrule (vx_t * vx);
vx_t * vxalloc (unsigned size);
void vxfree (vx_t * vx);
void vxrm (vx_t * vx);

bool vxisempty (vx_t * vx);
void vxclear (vx_t * vx);
unsigned vxsize (vx_t * vx);
void ** vxdata (vx_t * vx);

char * vxget (vx_t * vx, unsigned index);
void vxset (vx_t * vx, void * data, unsigned index);
char ** vxgetrow (vx_t * vx, unsigned r);
char ** vxgetcol (vx_t * vx, unsigned c);

#ifdef __cplusplus
}
#endif
