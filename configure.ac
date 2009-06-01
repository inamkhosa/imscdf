dnl --------------------------------
dnl Initialization macros.
dnl --------------------------------

AC_INIT([offlinecharging],[1.0.0])
AC_PREREQ(2.59)

TOPLEVEL_CONFIGURE_ARGUMENTS=""
AC_SUBST(TOPLEVEL_CONFIGURE_ARGUMENTS)

PACKAGE=offlinecharging

MAJOR_VERSION=1
MINOR_VERSION=0
MICRO_VERSION=0

VERSION=$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION
RELEASE=$MAJOR_VERSION.$MINOR_VERSION

PACKAGE_VERSION=$VERSION

#shared library versioning
LIBRARY_VERSION=$MAJOR_VERSION:$MINOR_VERSION:$MICRO_VERSION
#                       | | |
#                +------+ | +---+
#                |        |     |
#             current:revision:age
#                |        |     |
#                |        |     +- increment if interfaces have been added
#                |        |        set to zero if interfaces have been removed
#                                  or changed
#                |        +- increment if source code has changed
#                |           set to zero if current is incremented
#                +- increment if interfaces have been added, removed or changed


AC_SUBST(LIBRARY_VERSION)
AC_SUBST(PACKAGE)
AC_SUBST(PACKAGE_VERSION)
AC_SUBST(VERSION)
AC_SUBST(RELEASE)

AC_CONFIG_SRCDIR([src/offline_charging_server.c])
AM_CONFIG_HEADER([config.h])

dnl -----------------------------------------------
dnl Package name and version number (user defined)
dnl -----------------------------------------------

AM_INIT_AUTOMAKE($PACKAGE, $VERSION, no-define)
AC_LANG_CPLUSPLUS

dnl -----------------------------------------------
dnl Checks for programs.
dnl -----------------------------------------------

AC_PROG_CC
AC_PROG_CXX
AM_PROG_LIBTOOL
AM_SANITY_CHECK

dnl -----------------------------------------------
dnl Checks for libraries.
dnl -----------------------------------------------

PKG_CHECK_MODULES(DIAMETER_DEPS, libdiameter-1.0 >= 1.0.0)
AC_SUBST(DIAMETER_DEPS_CFLAGS)
AC_SUBST(DIAMETER_DEPS_LIBS)

PKG_CHECK_MODULES(DIAMAPPCOMMON_DEPS, libdiamappcommon-1.0 >= 1.0.0)
AC_SUBST(DIAMAPPCOMMON_DEPS_CFLAGS)
AC_SUBST(DIAMAPPCOMMON_DEPS_LIBS)

dnl -----------------------------------------------
dnl Checks for header files.
dnl -----------------------------------------------

AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([unistd.h])


dnl -----------------------------------------------
dnl Checks for typedefs, structures, and compiler characteristics.
dnl -----------------------------------------------


AC_C_CONST
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_STRUCT_TM
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])


dnl -----------------------------------------------
dnl Checks for functions.
dnl -----------------------------------------------

AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([getcwd gethostname memchr memset])

dnl -----------------------------------------------
dnl Generates Makefiles.
dnl -----------------------------------------------

AC_CONFIG_FILES(Makefile \
		src/Makefile \
		src/rf_app/Makefile \
		src/db_module/Makefile \
		src/cdr_filewriter/Makefile \
		src/cgf/Makefile \
		src/ASNFiles/Makefile
)

dnl -----------------------------------------------
dnl Generates Makefiles.
dnl -----------------------------------------------

AC_OUTPUT

