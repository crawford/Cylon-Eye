#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([cylon], [0.1], [russ@csh.rit.edu])
AC_CONFIG_SRCDIR([cylon.c])
#AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([cylon], [0.1])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_INSTALL

CFLAGS+=" -Wall "

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([strerror asprintf])

AC_OUTPUT([Makefile cylon.pc])
