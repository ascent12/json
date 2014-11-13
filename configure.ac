AC_INIT([json11], [0.1], [ascent@cock.li])

AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-xz subdir-objects])

AM_SILENT_RULES([yes])

AC_PROG_CC

LT_INIT

if test "x$GCC" = "xyes"; then
	GCC_CFLAGS="-Wall -Wextra -Wno-unused-parameter -g -Wstrict-prototypes -Wmissing-prototypes -fvisibility=hidden"
fi
AC_SUBST(GCC_CFLAGS)

AC_CHECK_HEADERS([stdint.h])
AC_CHECK_HEADER_STDBOOL

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES([
	Makefile
])

AC_OUTPUT
