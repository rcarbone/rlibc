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


/* How to pretty print a tree upright in a rectangular area on your computer screen */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "gnode.h"

#define GMAX(a,b) ((a) > (b) ? (a) : (b))


/* UTF-8 defines by Johan Myreen, updated by Ben Winslow */
#define UTF_V        "\342\224\202"        /* U+2502, Vertical line drawing char */
#define UTF_H        "\342\224\200"        /* U+2500, Horizontal                 */
#define UTF_UR       "\342\224\224"        /* U+2514, Up and right               */
#define UTF_HD       "\342\224\254"        /* U+252C, Horizontal and down        */


#define SPACE " "
#define HBAR  "-"
#define VBAR  "|"
#if defined(ROCCO)
#define PLUS  "+"
#else
#define PLUS  UTF_HD
#endif /* ROCCO */


/* All that should be recorded because of interest in a node */
typedef struct
{
  GNode * n;       /* pointer to the node in the tree */
} node_t;


typedef struct
{
  unsigned rows;      /* how many rows are in the bidimensional array to print */
  unsigned cols;      /* how many columns are in the bidimensional array to print */
  unsigned visited;   /* how many nodes are so far traversed */
  unsigned last;      /* last used column */
  node_t * nodes;     /* the bidimensional array with all the information needed to pretty print the tree */
  guint * maremma;    /* */

} info_t;


typedef struct
{
  unsigned n;
  char * stems;

} randy_t;


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/*                   S T A B L E                      */
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* Print a string centered in n characters */
static void center (char * s, unsigned n)
{
  int x;       /* # of initial blanks */
  unsigned y;  /* the string itself */
  int z;       /* # of final blanks */

  if (! s || ! n)
    {
      printf ("%*s", n, SPACE);
      return;
    }

  y = strlen (s);
  x = (n - y) / 2;
  z = n - x - y;

  if (x > 0)
    printf ("%*s", x, SPACE);       /* header */
  if (y < n)
    printf ("%-*.*s", y, y, s);     /* string */
  else
    printf ("%-*.*s", n, n, s);
  if (z > 0)
    printf ("%*s", z, SPACE);       /* footer */
}


static void setnode (node_t * n, GNode * node)
{
  n -> n = node;
}


static gboolean g_node_free_foreach (GNode * node, gpointer _unused)
{
  if (node -> data)
    free (node -> data);

  /* Never stop the traversal */
  return FALSE;
}


static gboolean g_node_print_foreach (GNode * node, gpointer _unused)
{
  unsigned n = g_node_depth (node);

  /* Indent */
  if (n > 1)
    printf ("%*.*s", n - 1, n - 1, " ");

  /* Print data */
  printf ("%s\n", (char *) node -> data);

  /* Never stop the traversal */
  return FALSE;
}


/*
 * x = columns
 * y = rows
 *               x
 *   +------------>
 *   |
 * y |
 *   V
 */
static info_t * initscreen (unsigned rows, unsigned cols)
{
  info_t * info = calloc (1, sizeof (* info));

  unsigned r;
  unsigned c;

  info -> rows    = rows;
  info -> cols    = cols;
  info -> visited = 0;
  info -> last    = 0;
  info -> nodes   = calloc (rows * cols, sizeof (node_t));
  info -> maremma = calloc (cols, sizeof (guint));

  for (r = 0; r < rows; r ++)
    for (c = 0; c < cols; c ++)
      setnode (& (* (info -> nodes + r + c * rows)), NULL);

  return info;
}


/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

static void printitem1 (GNode * n, unsigned star)
{
  printf ("%s ", n ? star ? "*" : (char *) n -> data : " ");
}


void printscreen1 (info_t * info, unsigned star)
{
  unsigned row = 0;
  for (row = 0; row < info -> rows; row ++)
    {
      unsigned col = 0;
      for (col = 0; col < info -> cols; col ++)
	printitem1 ((* (info -> nodes + col + row * info -> cols)) . n, star);
      printf ("\n");
    }
}


static void printitem (GNode * node, unsigned spaces)
{
  center (node && node -> data ? node -> data : NULL, spaces);
}


/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

/*
 * x = 0 => 
 * x = 1 => *
 * x = 2 => *+
 * x = 3 => +*+
 * x = 4 => +*-+
 * x = 5 => +-*-+
 * x = 6 => +-*--+
 * x = 7 => +--*--+
 */
static void connector (unsigned x, char * center, char * first, char * last, char * left, char * right)
{
  unsigned m1 = 0;
  unsigned m2 = 0;

  switch (x)
    {
    case 0: break;
    case 1: printf ("%s", center); break;
    case 2: printf ("%s%s", center, last); break;
    case 3: printf ("%s%s%s", first, center, last); break;

    default:
      m1 = (x - 3) / 2;
      m2 = m1 + (x - 3) % 2;

      connector (1, first, "?", "?", "?", "?");
      connector (m1, left, left, left, left, left);
      connector (1 + m2 + 1, right, center, last, right, right);

      break;
    }
}


static void pipesopra (GNode * n, unsigned spaces)
{
  connector (spaces, n && n -> parent ? VBAR : SPACE, SPACE, SPACE, SPACE, SPACE);
}


static void pipesotto (GNode * n, unsigned spaces)
{
  connector (spaces, n && ! G_NODE_IS_LEAF (n) ? VBAR : SPACE, SPACE, SPACE, SPACE, SPACE);
}


/*
 * Foreach node print a line
 *
 * +----*----+
 *      *----+
 * +----*
 * [         ]
 */
static void linesopra (GNode * n, unsigned spaces, bool last)
{
  static bool more = false;

  if (n && n -> parent)
    {
      char * left = g_node_n_children (n -> parent) == 1 || g_node_first_child (n -> parent) == n ? SPACE : HBAR;
      char * right = g_node_n_children (n -> parent) != 1 &&
	(g_node_first_child (n -> parent) == g_node_last_child (n -> parent) || g_node_last_child (n -> parent) != n) ? HBAR : SPACE;

      connector (spaces, PLUS, left, right, left, right);

      if (g_node_first_child (n -> parent) == n)
	more = true;
      else if (g_node_last_child (n -> parent) == n)
	more = false;

      if (g_node_n_children (n -> parent) == 1)
	more = false;
    }
  else
    {
      if (last)
	{
	  if (more)
	    connector (spaces, HBAR, HBAR, HBAR, HBAR, HBAR);
	  else
	    connector (spaces, SPACE, SPACE, SPACE, SPACE, SPACE);
	}
      else
	{
	  if (more)
	    connector (spaces, PLUS, HBAR, HBAR, SPACE, SPACE);
	  else
	    connector (spaces, SPACE, SPACE, SPACE, SPACE, SPACE);
	}

      /* Check for last column */
      if (! last)
	more = false;
    }
}


/* Go row by row and from left to right to find and print non empty items */
static void treeprettyprint (info_t * info)
{
  unsigned row = 0;
  unsigned col = 0;

  for (row = 0; row < info -> rows; row ++)
    {
      /* Print connettors between nodes in a separate line (not at the first level) */
      if (row)
	{
	  /* Print HBAR connectors between parent nodes in a separate line (not at first level) */
	  for (col = 0; col < info -> cols; col ++)
	    linesopra ((* (info -> nodes + col + row * info -> cols)) . n, * (info -> maremma + col), col < info -> cols - 1);
	  printf ("\n");

	  /* Print pipe VBAR connettors between nodes in a separate line */
	  for (col = 0; col < info -> cols; col ++)
	    pipesopra ((* (info -> nodes + col + row * info -> cols)) . n, * (info -> maremma + col));
	  printf ("\n");
	}

      /* Print the content of the node */
      for (col = 0; col < info -> cols; col ++)
	printitem ((* (info -> nodes + col + row * info -> cols)) . n, * (info -> maremma + col));
      printf ("\n");

      /* Print VBAR connettors between parent and child in a separate line (not at last level) */
      if (row < info -> rows - 1)
	{
	  for (col = 0; col < info -> cols; col ++)
	    pipesotto ((* (info -> nodes + col + row * info -> cols)) . n, * (info -> maremma + col));
	  printf ("\n");
	}
    }
}


/* While traversing the tree from left to right, store a pointer to the current node in a
 * bidimensional array (rectangular) and save the number of nodes that will be printed
 * on the same row */
static gboolean x_foreach (GNode * node, gpointer _info)
{
#define MORE 1

  info_t * info = _info;

  /*
   * x = rows
   * y = columns
   *               y
   *   +------------>
   *   |
   * x |
   *   V
   */

  /* Save information about the current node at position [x + y * rows] in the table of nodes */
  unsigned x = g_node_depth (node) - 1;
  unsigned y = info -> last;

  /* Special check to save space in the built table and improve printing.
   * If the current node has a parent with just an unique children then
   * the same column identified by 'y' can be used.
   */
  if (node -> parent && g_node_n_children (node -> parent) == 1)
    {
      /* Save useful information for this parent node at [x - 1, y] (last value of y used) */
      setnode (& (* (info -> nodes + y + (x - 1) * info -> cols)), node -> parent);
      * (info -> maremma + y) = GMAX (strlen (node -> parent -> data) + MORE, * (info -> maremma + y));
    }

  /* Special check to avoid insertion of already present nodes */
  if (g_node_n_children (node) != 1)
    {
      setnode (& (* (info -> nodes + y + x * info -> cols)), node);
      * (info -> maremma + y) = GMAX (strlen (node -> data) + MORE, * (info -> maremma + y));

      info -> last ++;
    }

  /* One more node */
  info -> visited ++;

  /* Never stop the traversal */
  return FALSE;
}


/* Implementation from Rosetta Code http://rosettacode.org/wiki/Visualize_a_tree#C adapted to use glib-gnode */
static void print_rosetta (GNode * root, GNode * stems)
{
#if defined(ROCCO)
  static char * more = "--";
  static char * down = "  |";
  static char * last = "  `";
#else
  static char * more = UTF_H UTF_H;
  static char * down = "  " UTF_V;
  static char * last = "  " UTF_UR;
#endif /* ROCCO */
  static char * none = "   ";

  GNode col = {NULL, NULL};
  GNode * stem;

  if (! root)
    return;

  /* Print indent */
  for (stem = stems; stem; stem = stem -> next)
    {
      printf ("%s", (char *) stem -> data);
      if (! stem -> next)
	break;
    }

  /* Print data */
  printf ("%s%s\n", more, (char *) root -> data);

  /* Update stems for the next iteration */
  if (stem && stem -> data == last)
    stem -> data = none;

  if (! stem)
    stem = stems = & col;    /* first */
  else
    stem -> next = & col;    /* others */

  /* Iterate over all childs */
  root = root -> children;
  while (root)
    {
      /* Update stems for the next iteration */
      col . data = root -> next ? down : last;

      /* Recursive print */
      print_rosetta (root, stems);

      root = root -> next;
    }

  stem -> next = NULL;
}


/*
 * http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees
 * (rendering collapse the concept of branch |-- and last `--)
 */
static void print_randy (GNode * root, randy_t * randy)
{
  if (! root)
    return;

  /* Print data */
  printf (" %s\n", (char *) root -> data);

  /* Iterate over all childs */
  root = root -> children;
  while (root)
    {
      /* Print indent */
      printf ("%s `--", randy -> stems);

      /* Push */
      randy -> stems [randy -> n ++] = ' ';
      randy -> stems [randy -> n ++] = root -> next ? '|' : ' ';
      randy -> stems [randy -> n ++] = ' ';
      randy -> stems [randy -> n ++] = ' ';
      randy -> stems [randy -> n]    = '\0';

      /* Recursive print */
      print_randy (root, randy);

      /* Pop */
      randy -> stems [randy -> n -= 4] = '\0';

      root = root -> next;
    }
}


/*
 * -=-=-=-=-=-=-
 *      API
 * -=-=-=-=-=-=-
 */

/* Memory cleanup */
void g_node_no_more (GNode * root)
{
  if (root)
    g_node_traverse (root, G_IN_ORDER, G_TRAVERSE_ALL, -1, g_node_free_foreach, NULL);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/*                V E R T I C A L                     */
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* We need to know the horizontal x and vertical y coordinates of each of the strings
 * in the given tree to print these strings on the correct position within the rectangular
 * position area on the screen.
 */
void g_node_vpretty_tree (GNode * root)
{
  /* The variable used in step (1) */
  info_t * info;

  if (! root)
    return;

  /* step (1):
   *   INORDER tree traversal to save some information needed by step (2) */
  g_node_traverse (root, G_IN_ORDER, G_TRAVERSE_ALL, -1, x_foreach,
		   info = initscreen (g_node_max_height (root), g_node_n_nodes (root, G_TRAVERSE_ALL)));

  /* step (2):
   *   go systematically row by row, and from left to the right, to pretty print the tree */
  treeprettyprint (info);

  /* Memory cleanup */
  free (info -> nodes);
  free (info -> maremma);
  free (info);
}


/* Print nodes by level without connectors (all nodes at a given level on a single row) */
void g_node_vprint_tree (GNode * root)
{
  /* The variable used in step (1) */
  info_t * info;

  if (! root)
    return;

  /* Initialize the variable used in step (1) */
  info = initscreen (g_node_max_height (root), g_node_n_nodes (root, G_TRAVERSE_ALL));

  /* step (1):
   * INORDER tree traversal to save some information needed by step (2) */
  g_node_traverse (root, G_IN_ORDER, G_TRAVERSE_ALL, -1, x_foreach, info);

  /* step (2):
   * go systematically row by row, and from left to the right, to bare print the tree */
  printscreen1 (info, 0);
  printf ("\n");

  /* Memory cleanup */
  free (info -> nodes);
  free (info -> maremma);
  free (info);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/*               H O R I Z O N T A L                  */
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* Very simple print (each node on its own row) */
void g_node_hprint_tree (GNode * root)
{
  if (root)
    g_node_traverse (root, G_PRE_ORDER, G_TRAVERSE_ALL, -1, g_node_print_foreach, NULL);
}


/* Same as the previous but iterate over nodes rather than use g_node_traverse() */
void g_node_hprint_tree_1 (GNode * root)
{
  unsigned n;

  if (! root)
    return;

  n = g_node_depth (root);

  /* Indent */
  if (n > 1)
    printf ("%*.*s", n - 1, n - 1, " ");

  /* Print data */
  printf ("%s\n", (char *) root -> data);

  /* Iterate over all childs */
  root = root -> children;
  while (root)
    {
      g_node_hprint_tree_1 (root);
      root = root -> next;
    }
}


/* Implementation from Rosetta Code http://rosettacode.org/wiki/Visualize_a_tree#C adapted to use glib-gnode */
void g_node_hprint_rosetta (GNode * root)
{
  print_rosetta (root, NULL);
}


/* http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees */
void g_node_randy_print_tree (GNode * root)
{
  randy_t randy;

  randy . n = 0;
  randy . stems = calloc (1, g_node_n_nodes (root, G_TRAVERSE_ALL) * 5);  /* 5 bytes per stem */
  print_randy (root, & randy);

  free (randy . stems);
}


#if defined(ROCCO)
/* Still bugged because of missing '|' in some lines */
void g_node_hprint_tree_bug (GNode * root)
{
  while (root)
    {
      bool last = root -> next ? false : true;
      bool more = root -> parent && root -> parent -> next ? true : false;

      /* Indent the line */
      indent (g_node_depth (root) - 1, last, more);

      /* Print the node data */
      printf ("%s\n", (char *) root -> data);

      /* Down one level */
      g_node_hprint_tree_bug (root -> children);

      root = root -> next;
    }
}

#else

static void g_node_rocco (GNode * root, GNode * stems)
{
  static char * more = "|--";
  static char * down = "|  ";
  static char * last = "`--";
  static char * none = "   ";

  GNode item = {NULL, NULL};
  GNode * stem = stems;

  if (! root)
    return;

  /* Iterate over the list of stems and print each item */
  while (stem)
    {
      printf ("%s", (char *) stem -> data);
      if (! stem -> next)
	break;
      stem = stem -> next;
    }

  /* Print data */
#if !defined(ROCCO)
  printf ("%s\n", (char *) root -> data);
#else
  printf ("%s [%c|%c|%c]\n", (char *) root -> data, root -> prev ? 'P' : '.', root -> next ? 'N' : '.', root -> children ? 'C' : '.');
#endif /* ROCCO */

  /* Replace first stem for the next iteration */
  if (stem && stem -> data == last)
    stem -> data = none;

  /* Update stems for the next iteration */
  if (! stem)
    stem = stems = & item;    /* set first item of the list */
  else
    stem -> next = & item;    /* add an item to the list */

  /* Iterate over all childs */
  root = root -> children;
  while (root)
    {
      /* Update stems for the next iteration */
      if (root -> next)
	{
	  /* Still bug here */
	  if (root -> children)
	    {
	      item . data = down;
	    }
	  else
	    {
	      item . data = more;     /* branch |-- */
	    }
	}
      else
	item . data = last;

      /* Recursive print */
      g_node_rocco (root, stems);

      /* Left one */
      root = root -> next;
    }

  stem -> next = NULL;
}

void g_node_hprint_tree_bug (GNode * root)
{
  g_node_rocco (root, NULL);
}
#endif /* ROCCO */
