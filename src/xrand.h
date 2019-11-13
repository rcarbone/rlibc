#pragma once


#ifdef __cplusplus
extern "C" {
#endif

unsigned xrand (unsigned n);
char * mkrndstr (unsigned len);
unsigned * rndarray (unsigned n);

#ifdef __cplusplus
}
#endif
