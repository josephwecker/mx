#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([mx], [0.0.1], [joseph.wecker@gmail.com])
AM_INIT_AUTOMAKE(mx, 0.0.1)
AM_CONFIG_HEADER(config.h:config.in)
AC_CONFIG_SRCDIR([mx.c])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([getcwd memset strdup strerror strtol strtoul])

AC_OUTPUT(Makefile)
