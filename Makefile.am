lib_LTLIBRARIES = libjson11.la

include_HEADERS = src/json11.h

libjson11_la_CFLAGS = $(GCC_CFLAGS)
libjson11_la_LDFLAGS = -version-info 0:1:0
libjson11_la_SOURCES =	\
	src/array.c	\
	src/object.c	\
	src/parser.c	\
	src/value.c	\
	src/all.h	\
	src/json11.h	\
	src/value.h

ACLOCAL_AMFLAGS = -I m4
