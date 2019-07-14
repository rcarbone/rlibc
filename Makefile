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


# The list of directories where to make something
SUBDIRS += src
# SUBDIRS += leaks
SUBDIRS += tests

# The main target is responsible to recursively scan subdirectories and build all the modules
all: ${SUBDIRS}
	@for dir in ${SUBDIRS} ; do \
           if [ -d $$dir ] ; then \
             if [ -f $$dir/Makefile ] ; then \
               echo "Making directory $$dir ..." ; \
               make -C $$dir -s --no-print-directory $@ ; \
             fi ; \
           else \
             echo "Warning: missing directory $$dir" ; \
           fi \
         done

# Cleanup rules
clean distclean:
	@rm -f *~
	@for dir in ${SUBDIRS} ; do \
           if [ -d $$dir ] ; then \
             if [ -f $$dir/Makefile ] ; then \
               make -C $$dir -s --no-print-directory $@ ; \
             fi \
           fi \
         done

# Targets that are eventually handled by recursive Makefile(s)
%:
	@for dir in ${SUBDIRS} ; do \
           if [ -d $$dir ] ; then \
             echo $$dir; \
             if [ -f $$dir/Makefile ] ; then \
               make -C $$dir -s --no-print-directory $@ ; \
               echo ; \
             fi ; \
           else \
             echo "Warning: missing directory $$dir" ; \
           fi \
         done

# Fake targets
.PHONY: all clean distclean ${SUBDIRS}
