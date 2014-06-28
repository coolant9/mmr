#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([micro_media_renderer], [0.1], [coolant9@gmail.com])
AM_INIT_AUTOMAKE(micro_media_renderer, 0.1)
AC_CONFIG_SRCDIR([sample_util.c])
#AC_CONFIG_HEADERS([config.h])
AM_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lixml':
AC_CHECK_LIB([ixml], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lthreadutil':
AC_CHECK_LIB([threadutil], [main])
# FIXME: Replace `main' with a function in `-lupnp':
AC_CHECK_LIB([upnp], [main])
# FIXME: Replace `main' with a function in `-lmpdclient':
AC_CHECK_LIB([mpdclient], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strdup])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
