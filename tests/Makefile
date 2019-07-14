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


# C source files
LIBSRCS  +=

# C source files
MAINSRCS += alloc-free.c
MAINSRCS += populate.c
MAINSRCS += print.c
MAINSRCS += get.c
MAINSRCS += typeof.c
MAINSRCS += pretty-table.c

# The name of the games
LIB       =

#
# External software components
#

# rlibc
RLIBCDIR  = ../src
INCLUDE  += -I${RLIBCDIR}
LIBRLIBC  = ${RLIBCDIR}/librlibc.a

# Object, depend and binary files
SHLIB     = ${LIB:%.a=%.so}
LIBOBJS   = ${LIBSRCS:%.c=%.o}
MAINOBJS  = ${MAINSRCS:%.c=%.o}
OBJS      = ${LIBOBJS} ${MAINOBJS}
DEPS      = ${LIBSRCS:%.c=%.M} ${MAINSRCS:%.c=%.M}
PROGRAMS  = ${MAINSRCS:%.c=%}

# Compiler flags
INCLUDES  = -I./
INCLUDES += ${INCLUDE}
CC        = gcc
LD        = gcc
CFLAGS    = -g -Wall ${INCLUDES}
CFLAGS   += -fPIC
CFLAGS   += -fno-diagnostics-color
ARFLAGS   = rs
LDFLAGS   =

# Static libraries
USRLIBS  += ${LIB}
USRLIBS  += ${LIBRLIBC}

# System libraries
SYSLIBS  +=

# The main target is responsible to make all the targets
all: ${LIB} ${SHLIB} ${PROGRAMS}

# Static library
${LIB}: ${LIBOBJS}
	@echo "=*= making static library $@ =*="
	@${AR} ${ARFLAGS} $@ $^ 1> /dev/null 2>& 1

# Dynamic Library
${SHLIB}: ${LIBOBJS}
	@echo "=*= making shared library $@ =*="
	@${CC} -shared $^ ${USRLIBS} ${SYSLIBS} -o $@

# Binary programs
%: %.o ${USRLIBS}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

# Cleanup rules
clean:
	@rm -f ${PROGRAMS}
	@rm -f ${SHLIB}
	@rm -f ${LIB}
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

# Check for errors and memory leaks
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
VFLAGS   += --error-exitcode=1
VFLAGS   += --leak-check=full
VFLAGS   += --show-leak-kinds=all

# Run valgrind (using raw suppression file)
leaks-%: %
	@echo "=*= ckecking $^ =*="
	@echo
	@valgrind ${VFLAGS} $^

# Run valgrind (no suppression file)
checks-%: %
	@echo "=*= ckecking $^ =*="
	@echo
	@valgrind ${VFLAGS} $^

# All errors and memory leaks
leaks: ${PROGRAMS}
	@for bin in ${PROGRAMS} ; do \
           if [ -x $$bin ] ; then \
             echo -n "=*= checking program [$$bin] " ; \
             status=`valgrind -q ${VFLAGS} $$bin > /dev/null` ; \
             if [ "$$status" != "" ] ; then \
               echo "" ; \
               echo "Checking program [$$bin] failed! Please verify errors running the following:" ; \
               echo "valgrind ${VFLAGS} $$bin" ; \
               exit 1 ; \
             else \
               echo "Ok! =*=" ; \
             fi \
           fi \
         done

checks: ${PROGRAMS}
	@for bin in ${PROGRAMS} ; do \
           echo "=*= checking program $$bin for errors =*=" ; \
           make checks-$$bin ; \
         done

# The help target
help:
	@echo "Usage:"
	@echo "  make all     --> compile only (just to check for C program syntax)"
	@echo "  make leaks   --> ckeck for memory leaks (no suppression files)"
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
