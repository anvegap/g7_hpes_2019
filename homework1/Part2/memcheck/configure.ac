#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT(memcheck, 1.0, anthony.vega19@gmail.com)
AC_CONFIG_SRCDIR(src)
AC_CONFIG_AUX_DIR(build-aux)

AM_PROG_AR

AC_CONFIG_MACRO_DIRS([m4])
AM_PROG_LIBTOOL
AC_PROG_INSTALL

AC_CONFIG_FILES([Makefile src/Makefile lib/Makefile])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

LT_INIT([shared disable-static])

AC_OUTPUT
