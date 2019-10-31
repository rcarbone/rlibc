
#pragma once

/* System headers */
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void fmemdmp (FILE * fd, char * ptr, unsigned size, char * label);
void memdmp (char * ptr, unsigned size, char * label);
void mem2c (char * ptr, unsigned size, char * name);


#ifdef __cplusplus
}
#endif
