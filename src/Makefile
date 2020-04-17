#
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#                   _ _ _
#              _ __| (_) |__   ___
#             | '__| | | '_ \ / __|
#             | |  | | | |_) | (__
#             |_|  |_|_|_.__/ \___|
#
#         C library of useful functions
#
# R. Carbone (rocco@tecsiel.it)
#   2Q 2019
#
# SPDX-License-Identifier: BSD-2-Clause
#
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#


# Project directories
ROOTDIR   = ../..

# C source files
MAINSRCS +=
LIBSRCS  += safe.c
LIBSRCS  += replace.c
LIBSRCS  += buffers.c
LIBSRCS  += rstring.c
LIBSRCS  += rtrim.c
LIBSRCS  += vargv.c
LIBSRCS  += sargv.c
LIBSRCS  += gnu-argv.c
LIBSRCS  += gnode.c
LIBSRCS  += more-gnode.c
LIBSRCS  += rctype.c
LIBSRCS  += rwall.c
LIBSRCS  += interval.c
LIBSRCS  += elapsed.c
LIBSRCS  += support.c
LIBSRCS  += xrand.c
LIBSRCS  += roptions.c
LIBSRCS  += parsecfg.c
LIBSRCS  += rwall.c
LIBSRCS  += interval.c
LIBSRCS  += flux.c
LIBSRCS  += elapsed.c
LIBSRCS  += xlog.c
LIBSRCS  += fmemdmp.c
LIBSRCS  += plugins.c
LIBSRCS  += other.c
LIBSRCS  += rsi.c
LIBSRCS  += rmatrix.c
LIBSRCS  += curses.c
LIBSRCS  += rock.c

# The name of the games
LIBNAME   = rlibc
PROGRAMS += ${MAINSRCS:%.c=%}

# All C source files
SRCS      = ${LIBSRCS} ${MAINSRCS}

# All libraries
STLIB     = lib${LIBNAME}.a
# SHLIB     = ${STLIB:%.a=%.so}

# All targets
TARGETS   = ${STLIB} ${SHLIB} ${PROGRAMS}

# Object and depend files
LIBOBJS   = ${LIBSRCS:%.c=%.o}
MAINOBJS  = ${MAINSRCS:%.c=%.o}
OBJS      = ${LIBOBJS} ${MAINOBJS}
DEPS      = ${SRCS:%.c=%.M}

# C/C++ Compilers and flags
INCLUDES  = -I./
INCLUDES += ${INCLUDE}
CC        = gcc
AR        = ar
LD        = gcc
CFLAGS    = -g -Wall
CFLAGS   += -fPIC
CFLAGS   += -fno-diagnostics-color
CFLAGS   += ${INCLUDES}
ARFLAGS   = rv
LDFLAGS   =

# User and System Libraries
USRLIBS  += ${STLIB}
SYSLIBS  +=

# The main target is responsible to make all
all: ${TARGETS}

# Static library
${STLIB}: ${LIBOBJS}
	@echo "=*= making static library $@ =*="
	@${AR} ${ARFLAGS} $@ $^ 1> /dev/null 2>& 1

# Shared library
${SHLIB}: ${LIBOBJS}
	@echo "=*= making shared library $@ =*="
	@${CC} -shared $^ ${USRLIBS} ${SYSLIBS} -o $@

# Binary programs
%: %.o ${USRLIBS}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

# Cleanup rules
clean:
	@rm -f ${TARGETS}
	@rm -f ${OBJS}
	@rm -f *~

distclean: clean
	@rm -f ${DEPS}

# How to make an object file from a C source
%.o: %.c
	@echo "=*= making object $@ =*="
	@${CC} -c ${CFLAGS} $<

# How to make a depend file from a C source
%.M: %.c
	@-${CC} -MM ${INCLUDES} $< -o $@

-include ${DEPS}

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Check for errors and memory leaks
PARSER    = /usr/local/bin/parse_valgrind_suppressions.awk

VFLAGS   += --error-exitcode=1
VFLAGS   += --show-leak-kinds=all
VFLAGS   += --leak-check=full

# Create a raw suppression file
%.X: %
	@echo -n "=*= making suppression file $@ ... "
	@rm -f $^.o
	@${CC} -c ${CFLAGS} $^.c
	@valgrind --leak-check=full --show-reachable=yes --error-limit=no --gen-suppressions=all --log-file=$^.log $^ > /dev/null
	@echo -n "parsing ... "
	@cat $^.log | ${PARSER} > $@
	@echo "done!"
	@rm -f $^.log
	@rm -f $^.o

# Run valgrind (using raw suppression file)
leak-%: %
	@echo "=*= ckecking $^ (suppression file $^.X) =*="
	@echo
	@valgrind ${VFLAGS} --suppressions=$^.X $^

# Run valgrind (no suppression file)
checks-%: %
	@echo "=*= ckecking $^ =*="
	@echo
	@valgrind ${VFLAGS} $^

# Create all suppression files
# RAWS = ${SRCS:%.c=%.X}
# RAWS = memchk.X
# X: ${RAWS}

# All errors and memory leaks
leaks: ${PROGRAMS}
	@for bin in ${PROGRAMS} ; do \
           echo "=*= checking program $$bin for errors =*=" ; \
           make leak-$$bin ; \
         done

checks: ${PROGRAMS}
	@for bin in ${PROGRAMS} ; do \
           echo "=*= checking program $$bin for errors =*=" ; \
           make check-$$bin ; \
         done

# The help target
help:
	@echo "Usage:"
	@echo "  make all     --> compile only (just to check for C program syntax)"
	@echo "  make X       --> make suppression files [*.X] (Damned Oracle!)"
	@echo "  make leaks   --> ckeck for memory leaks using suppression files *.X"
	@echo "  make ckecks  --> ckeck for memory leaks (no suppression files)"
	@echo
	@echo "  make leaks-X --> ckeck for memory leaks just in module X"
	@echo
	@echo "  The following modules have been currently defined:"
	@echo -n "   "
	@for p in ${PROGRAMS} ; do \
           echo -n "$$p " ; \
         done
	@echo

.DEFAULT:
	@echo "No rule has been defined to make: $@"
