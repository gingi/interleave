#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([interleave], [0.1], [shiranpasternak@gmail.com])
AC_CONFIG_SRCDIR([interleave.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([test])
AC_REQUIRE_AUX_FILE([tap-driver.sh])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_MAINTAINER_MODE([disable])

# Checks for programs.
AC_PROG_CC
AC_PROG_AWK

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile test/Makefile])

AC_OUTPUT
