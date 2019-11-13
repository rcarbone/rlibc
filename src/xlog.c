/*
 * Simple logging functions
 */


/* System headers */
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


/* Public header file */
#include "safe.h"

/* Public functions */
char * tvtouptime (struct timeval * tv);


/* Local variables */
static char * logname  = NULL;
static char * lognode  = NULL;            /* Name of the system where the logging is performed */
static char * logfile  = NULL;            /* Logging filename                                  */
static FILE * logfd    = NULL;            /* Logging file descriptor                           */
static int logformat   = 1;               /* Boolean to set timestamp format to date/time      */
static pid_t pid       = -1;              /* Process pid                                       */
static struct timeval uptime = { 0, 0 };  /* Time the application has started                  */


/* Week day and month names */
static char * wdays [7]   = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static char * months [12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


/* Build a syslog-like timestamp string
 * in the format => www mmm dd hh:mm:ss yyyy
 */
static char * timestamp ()
{
  static char buf [128];

  struct timeval now;
  time_t t = time (0);
  struct tm * tm = localtime (& t);

  gettimeofday (& now, NULL);

  if (logformat)
    sprintf (buf, "%s %s %02d %02d:%02d:%02d %04d | ",
	     wdays [tm ->  tm_wday],
	     months [tm -> tm_mon],
	     tm -> tm_mday,
	     tm -> tm_hour,
	     tm -> tm_min,
	     tm -> tm_sec,
	     tm -> tm_year + 1900);
  else
    sprintf (buf, "%ld %s | ", (long) now . tv_sec, tvtouptime (& uptime));

  return buf;
}


/* Initialize the logging system */
void xloginit (char * name, char * node, char * log, struct timeval * started, int logfmt, int flush)
{
  /* Open log file */
  safeclose (logfd);
  logfd = safeopen (log);

  if (! logfd && log && strcmp (log, "*"))
    fprintf (stdout, "[%s] Warning: Cannot open logging file '%s'. Continue logging on standard output\n", name, log),
      logfd = stdout;

  /* Set unbuffered output */
  if (logfd && flush)
    setvbuf (logfd, NULL, _IONBF, 0);

  logname = strdup (name ? name : "");
  lognode = strdup (node ? node : "");
  logfile = strdup (log ? log : "");
  pid     = getpid ();
  logformat = logfmt ? 1 : 0;

  /* FIX name, node */
  if (! started)
    gettimeofday (& uptime, NULL);
  else
    uptime = * started;
}


/* Terminate the logging system */
void xlogterm (void)
{
  /* Reset global variables */
  logname   = safefree (logname);
  lognode   = safefree (lognode);
  logfile   = safefree (logfile);
  logfd     = safeclose (logfd);
  logformat = 1;
  pid       = -1;
  uptime . tv_sec = uptime . tv_usec = 0;
}


/* printf-like interface to implement logging (that's all folks!!!) */
int xlog (const char * fmt, ...)
{
  va_list va_ap;

  if (! logfd)
    return 0;

  errno = 0;

  va_start (va_ap, fmt);

  fprintf (logfd, "%s", timestamp ());
  vfprintf (logfd, fmt, va_ap);

  va_end (va_ap);

  return 0;
}
