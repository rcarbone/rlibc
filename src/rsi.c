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


/* Project headers */
#include "rsi.h"


static char * rsi_errlist [] =
{
  "No error or unspecified error",
  "Hostname Not Found",
  "Cannot get endpoint",
  "Cannot bind to port",
  "Cannot connect to host",
  "Cannot accept request"
};


/*
 * Set option to linger on close if data present.
 *
 * A close() returns immediately.
 * The underlying stack discards any unsent data, and, in the case of connection-oriented protocols
 * such as TCP, sends a RST (reset) to the peer (this is termed a hard or abortive close).
 * All subsequent attempts by the peer's application to read()/recv() data will result in an ECONNRESET.
 *
 */
static int rsi_aggressive (int fd)
{
  int ret = 0;
  int keep = 1;
  struct linger li = { .l_onoff = 1, .l_linger = 0 };

#ifdef SO_KEEPALIVE
  ret |= setsockopt (fd, SOL_SOCKET, SO_KEEPALIVE, (char *) & keep, sizeof keep);
#endif

#ifdef SO_LINGER
  ret |= setsockopt (fd, SOL_SOCKET, SO_LINGER, (char *) & li, sizeof li);
#endif

  return ret;
}


/* Set option in order to reuse address if busy */
static void rsi_reuse (int fd)
{
  int reuse = 1;
  setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, (const char *) & reuse, sizeof (reuse));
}


/* Set non blocking mode of operation */
static void rsi_nonblock (int fd)
{
  fcntl (fd, F_SETFL, fcntl (fd, F_GETFL) | O_NONBLOCK);
}


char * rsi_error (int n)
{
  if (n > RSI_NONE || n < RSI_LAST)
    n = RSI_NONE;
  return rsi_errlist [- n];
}


/* Parse a string for local convention about [any] interface */
bool rsi_isany (char * str)
{
  return str && strcasecmp (str, RSI_ANYSTR) && strcmp (str, RSI_ANYADDRESS) && strcmp (str, RSI_ANYINTERFACE) ? false : true;
}


/* Return the address of [who] in dot notation */
char * rsi_dotname (char * who)
{
  static char name [16];

  struct in_addr inaddr;
  struct hostent * host;

  host = gethostbyname (who);
  if (host)
    memcpy (& inaddr, host -> h_addr_list [0], host -> h_length);
  else
    inaddr . s_addr = inet_addr (who);

  snprintf (name, sizeof (name), "%s", inet_ntoa (inaddr));

  return name;
}


/* Parse an IPv4 address into a socket address */
struct sockaddr_in * rsi_addrtosin (char * addr)
{
  static struct sockaddr_in sin;

  char hostname [512] = { RSI_ZERO };
  struct hostent * he;
  unsigned port;
  char * c;

  if (! addr)
    strncpy (hostname, RSI_ANYADDRESS, sizeof (hostname));
  else
    strncpy (hostname, addr, sizeof (hostname));

  c = strchr (hostname, ':');
  if (c)
    {
      port = atoi (c + 1);
      * c = 0x0;
    }
  else
    port = RSI_PORT;        /* default IPv4 port */

   /* Clear the sockaddr before using it, in case there are extra platform-specific fields that can mess us up */

  /* Setup socket address */
  memset (& sin, RSI_ZERO, sizeof (sin));
  sin . sin_family = AF_INET;
  sin . sin_port   = htons (port);

  /* Get network host entry for the given name */
  if (! rsi_isany (hostname))
    {
      errno = 0;
      he = gethostbyname (hostname);
      if (! he || he -> h_length > sizeof (sin . sin_addr))
	return NULL;
      memcpy (& sin . sin_addr, he -> h_addr, he -> h_length);
    }
  else
    sin . sin_addr . s_addr = htonl (INADDR_ANY);

  return & sin;
}


/*
 * Create an endpoint for outgoing TCP connections at given address and port
 *
 * Return values on error:
 *   RSI_HOST_NOT_FOUND
 *   RSI_SOCKET_ERROR
 *   RSI_BIND_ERROR
 */
int rsi_connect (char * remote, unsigned rport)
{
  /* Local variables */
  int fd;
  struct sockaddr_in server;
  struct hostent * host;

  /* Clear the sockaddr before using it, in case there are extra platform-specific fields that can mess us up */

  /* Setup server address */
  memset (& server, 0x00, sizeof (struct sockaddr_in));
  server . sin_family = AF_INET;
  server . sin_port   = htons (rport);

  errno = 0;
  host = gethostbyname (remote);
  if (host)
    memcpy (& server . sin_addr, host -> h_addr_list [0], host -> h_length);
  else
    server . sin_addr . s_addr = inet_addr (remote);
  if (server . sin_addr . s_addr == -1)
    return RSI_HOST_NOT_FOUND;

  fd = socket (AF_INET, SOCK_STREAM, 0);
  if (fd == -1)
    return RSI_SOCKET_ERROR;

  /* Ignore writes to connections that have been closed at the other end */
  signal (SIGPIPE, SIG_IGN);

  /* Set option in order to immediately close the socket */
  rsi_aggressive (fd);

  /* Initiate a TCP/IP connection on a socket */
  if (connect (fd, (struct sockaddr *) & server, sizeof (struct sockaddr_in)) == -1)
    {
      close (fd);
      return RSI_CONNECT_ERROR;
    }

  return fd;
}


/*
 * Create an endpoint for outgoing TCP connections at given address and port
 *
 * Return values on error:
 *   RSI_HOST_NOT_FOUND
 *   RSI_SOCKET_ERROR
 *   RSI_BIND_ERROR
 */
int rsi_aconnect (char * remote, unsigned rport, char * local, unsigned lport)
{
  /* Local variables */
  int fd;
  struct sockaddr_in me;
  struct sockaddr_in server;
  struct hostent * host;

  /* Clear the sockaddr before using it, in case there are extra platform-specific fields that can mess us up */

  /* Setup server address */
  memset (& server, 0x00, sizeof (struct sockaddr_in));
  server . sin_family = AF_INET;
  server . sin_port   = htons (rport);

  errno = 0;
  host = gethostbyname (remote);
  if (host)
    memcpy (& server . sin_addr, host -> h_addr_list [0], host -> h_length);
  else
    server . sin_addr . s_addr = inet_addr (remote);
  if (server . sin_addr . s_addr == -1)
    return RSI_HOST_NOT_FOUND;

  fd = socket (AF_INET, SOCK_STREAM, 0);
  if (fd == -1)
    return RSI_SOCKET_ERROR;

#if defined(ROCCO)
  /* Set non-blocking mode (asynchronous) */
  rsi_nonblock (fd);
#endif /* ROCCO */

  /* Set option in order to immediately close the socket */
  rsi_aggressive (fd);

  if (! rsi_isany (local))
    {
      struct hostent * h;

      /* Setup local address */
      memset (& me, 0x00, sizeof (struct sockaddr_in));
      me . sin_family = AF_INET;
      me . sin_port   = htons (lport);

      h = gethostbyname (local);
      if (h)
	memcpy (& me . sin_addr, h -> h_addr_list [0], h -> h_length);
      else
	me . sin_addr . s_addr = inet_addr (local);
      if (me . sin_addr . s_addr == -1)
	{
	  close (fd);
	  return RSI_HOST_NOT_FOUND;
	}

      /* Bind to local address */
      if (bind (fd, (struct sockaddr *) & me, sizeof (me)) == -1)
	{
	  close (fd);
	  return RSI_BIND_ERROR;
	}
    }
  else
    {
      if (lport)
	{
	  /* Setup local address */
	  memset (& me, 0x00, sizeof (struct sockaddr_in));
	  me . sin_family = AF_INET;
	  me . sin_port   = htons (lport);
	  me . sin_addr . s_addr = htonl (INADDR_ANY);

	  /* Bind to local address */
	  if (bind (fd, (struct sockaddr *) & me, sizeof (me)) == -1)
	    {
	      close (fd);
	      return RSI_BIND_ERROR;;
	    }
	}
    }

  /* Initiate a TCP/IP connection on a socket */
  if (connect (fd, (struct sockaddr *) & server, sizeof (struct sockaddr_in)) == -1)
    {
#if defined(ROCCO)
      if (errno != EINPROGRESS && errno != EWOULDBLOCK && errno != ECONNREFUSED)
	{
	  close (fd);
	  return RSI_CONNECT_ERROR;
	}
#else
      close (fd);
      return RSI_CONNECT_ERROR;
#endif /* ROCCO */
    }

  return fd;
}


void rsi_disconnect (unsigned fd)
{
  close (fd);
}


/*
 * Accept an incoming TCP connection on the given endpoint
 *
 * Return values on error:
 *  RSI_ACCEPT_ERROR
 *  RSI_HOST_NOT_FOUND
 */
int rsi_welcome (unsigned lfd, char ** remote, unsigned * rport)
{
  /* Local variables */
  unsigned retry = 10;
  int fd;
  struct sockaddr_in peeraddr;
  socklen_t peerlen = sizeof (peeraddr);
  struct hostent * rhost = NULL;

  /* Try to accept the connection */
  do
    {
      errno = 0;
      fd = accept (lfd, (struct sockaddr *) & peeraddr, & peerlen);
    } while (fd == -1 && errno == EINTR && retry --);

  if (fd == -1)
    return RSI_ACCEPT_ERROR;

  /* Get the IP address of the calling application and retrieve its name */
  memset ((char *) & peeraddr, 0x00, sizeof (peeraddr));
  if (getpeername (fd, (struct sockaddr *) & peeraddr, & peerlen))
    {
      close (fd);
      return RSI_HOST_NOT_FOUND;
    }

  /* Then lookup for a name.
   * I do not a forward lookup because I do not care of possible spoofing */
  rhost = gethostbyaddr ((char *) & peeraddr . sin_addr, sizeof (peeraddr . sin_addr), AF_INET);

  * rport = ntohs (peeraddr . sin_port);
  * remote = rhost ? strdup (rhost -> h_name) : strdup (inet_ntoa (peeraddr . sin_addr));

  rsi_aggressive (fd);
  rsi_nonblock (fd);

  return fd;
}


/*
 * Create an endpoint to listen for incoming TCP connections at given address and port
 *
 * Return values on error:
 *   RSI_HOST_NOT_FOUND
 *   RSI_SOCKET_ERROR
 *   RSI_BIND_ERROR
 */
int rsi_incoming (char * address, unsigned port, unsigned qlen)
{
  /* Local variables */
  int lfd   = -1;
  struct sockaddr_in server;
  struct hostent * he;

  errno = 0;

  /* Clear the sockaddr before using it, in case there are extra platform-specific fields that can mess us up */

  /* Setup server address */
  memset (& server, 0x00, sizeof (server));
  server . sin_family = AF_INET;
  server . sin_port   = htons (port);

  /* Get network host entry for the given name */
  if (! rsi_isany (address))
    {
      he = gethostbyname (address);
      if (! he || he -> h_length > sizeof (server . sin_addr))
	return RSI_HOST_NOT_FOUND;
      memcpy (& server . sin_addr, he -> h_addr, he -> h_length);
    }
  else
    server . sin_addr . s_addr = htonl (INADDR_ANY);

  /* Create a socket for the management of all the incoming calls */
  if ((lfd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    return RSI_SOCKET_ERROR;

  /* Set option in order to reuse address if busy */
  rsi_reuse (lfd);

  /* Set option to linger on close if data present */
  rsi_aggressive (lfd);

  /* Set non blocking mode of operation */
  rsi_nonblock (lfd);

  /* Bind to local address */
  if (bind (lfd, (struct sockaddr *) & server, sizeof (server)) == -1)
    {
      close (lfd);
      return RSI_BIND_ERROR;
    }

  /* Set queue limit for incoming calls */
  listen (lfd, qlen);

  return lfd;
}


int rsi_send (unsigned fd, char * frame, unsigned len)
{
  errno = 0;
  return send (fd, frame, len, 0);
}


/* If a message is available the ioctl() can be used to determine the amount of data that has been received */
unsigned rsi_bytes (unsigned fd)
{
  unsigned bytes;

  if (ioctl (fd, FIONREAD, (char *) & bytes) == -1)
    bytes = 0;

  return bytes;
}


/* Role to string */
char * rsi_rtoa (rsi_role_e role)
{
  switch (role)
    {
    case RSI_DAEMON:  return "Daemon";
    case RSI_CLIENT:  return "Client";
    case RSI_SERVER:  return "Server";
    }
  return "No role";
}


