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
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

/* Basic Networking headers */
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

/* Addressing Networking headers */
#include <netdb.h>
#include <arpa/inet.h>


/* Project headers */
#include "rsi.h"


/* Constants */
#define RSI_ZERO            (0x00)
#define RSI_PORT            (1381)


/* Constants for IPv4 */
#define RSI_ANYADDRESS      "*"
#define RSI_ANYSTR          "any"
#define RSI_ANYINTERFACE    "0.0.0.0"
#define RSI_LOOPBACK        "127.0.0.1"
#define RSI_QLEN            (256)     /* default listen queue size */

/* Error codes */
#define RSI_NONE            (0)
#define RSI_HOST_NOT_FOUND  (-1)
#define RSI_SOCKET_ERROR    (-2)
#define RSI_BIND_ERROR      (-3)
#define RSI_CONNECT_ERROR   (-4)
#define RSI_ACCEPT_ERROR    (-5)
#define RSI_LAST            RSI_ACCEPT_ERROR

/* Helpers */
typedef struct sockaddr    sa_t;
typedef struct sockaddr_in sin_t;
typedef struct hostent     he_t;
typedef struct timeval     tv_t;


/* Function prototypes */
char * rsi_error (int n);
bool rsi_isany (char * str);
char * rsi_dotname (char * who);
struct sockaddr_in * rsi_addrtosin (char * addr);
int rsi_connect (char * remote, unsigned port);
int rsi_aconnect (char * remote, unsigned rport, char * local, unsigned lport);
void rsi_disconnect (unsigned fd);
int rsi_welcome (unsigned lfd, char ** remote, unsigned * rport);
int rsi_incoming (char * address, unsigned port, unsigned backlog);
int rsi_send (unsigned fd, char * frame, unsigned len);
unsigned rsi_bytes (unsigned fd);
