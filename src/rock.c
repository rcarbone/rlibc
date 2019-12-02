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
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Project headers */
#include "rock.h"


/* Useful macros */
#define VXMIN(a, b) (((a) < (b)) ? (a) : (b))
#define VXMAX(a, b) (((a) > (b)) ? (a) : (b))

#define MAXLINE 1024


struct vx
{
  unsigned size;
  void ** data;

};


/* Allocate a vector of given [size] */
vx_t * vxalloc (unsigned size)
{
  vx_t * vx;

  if (! size)
    return NULL;

  vx = calloc (1, sizeof (* vx));
  vx -> size = size;
  vx -> data = calloc (size, sizeof (void *));

  return vx;
}


/* Free a vector */
void vxfree (vx_t * vx)
{
  unsigned i;

  if (! vx)
    return;

  for (i = 0; i < vx -> size; i ++)
    if (vx -> data [i])
      free (vx -> data [i]);

  free (vx -> data);
  free (vx);
}


/* Free a vector */
void vxrm (vx_t * vx)
{
  if (! vx)
    return;

  free (vx -> data);
  free (vx);
}


/* Check if a matrix is empty */
bool vxisempty (vx_t * vx)
{
  unsigned i;

  if (! vx)
    return true;

  for (i = 0; i < vx -> size; i ++)
    if (vx -> data [i])
      return false;

  return true;
}


void vxclear (vx_t * vx)
{
  unsigned i;

  if (! vx)
    return;

  for (i = 0; i < vx -> size; i ++)
    vx -> data [i] = NULL;
}


unsigned vxsize (vx_t * vx)
{
  return vx ? vx -> size : 0;
}


void ** vxdata (vx_t * vx)
{
  return vx ? vx -> data : NULL;
}


/* Get an element at given [index] */
char * vxget (vx_t * vx, unsigned index)
{
  return vx && vx -> size ? vx -> data [index] : NULL;
}


/* Set an element at given [index] */
void vxset (vx_t * vx, void * data, unsigned index)
{
  if (vx && vx -> size && data)
    vx -> data [index] = data;
}
