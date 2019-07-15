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
#include <unistd.h>
#include <sys/ioctl.h>
#include <ncurses.h>

/* Public header */
#include "rlibc.h"


/* default values */
#define DEFAULT_HEIGHT  25
#define DEFAULT_WIDTH   100

static int height     = DEFAULT_HEIGHT;
static int width      = DEFAULT_WIDTH;
static int ioctl_rows = 0;
static int ioctl_cols = 0;


static void save_env (char * name)
{
  char fmt [128];
  char buffer [128];

  sprintf (fmt, "%s=%%ld", name);

  snprintf (buffer, sizeof (buffer), fmt, width);
  putenv (buffer);
}


/* Have we checked environment variables COLUMNS or LINES ? */
static int check_variable (char * variable, int value)
{
  if (! value)
    {
      /* have we checked environment variable? */
      const char * s = getenv (variable);
      value = -1;
      if (s && * s)
	{
	  char * endptr;
	  long t;

	  t = strtol (s, & endptr, 0);
	  if (t > 0 && ! * endptr)
	    value = t;

	  width = value;
	  save_env ("COLUMNS");
	}
    }
  return value;
}


/* Stolen from watch.c source file from Linux procps utilities */
static void get_terminal_size (void)
{
  struct winsize w;

  /* Have we checked COLUMNS and LINES ? */
  ioctl_cols = check_variable ("COLUMNS", ioctl_cols);
  ioctl_rows = check_variable ("LINES", ioctl_rows);

  if (ioctl_cols < 0 || ioctl_rows < 0)
    {
      if (ioctl (STDOUT_FILENO, TIOCGWINSZ, & w) == 0)
	{
	  if (ioctl_cols < 0 && w . ws_col > 0)
	    {
	      width = w . ws_col;
	      save_env ("COLUMNS");
	    }

	  if (ioctl_rows < 0 && w . ws_row > 0)
	    {
	      height = w . ws_row;
	      save_env ("LINES");
	    }
	}
    }
}


void initialize_curses (void)
{
  get_terminal_size ();
  initscr ();
  nonl ();
  noecho ();
  cbreak ();
}


void terminate_curses (void)
{
  endwin ();
}


int get_cols (void)
{
  if (ioctl_rows == 0 && ioctl_cols == 0)
    get_terminal_size ();
  return width;
}


int get_rows (void)
{
  if (ioctl_rows == 0 && ioctl_cols == 0)
    get_terminal_size ();
  return height;
}
