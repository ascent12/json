# libjson11 version
VERSION = 0.1

PREFIX ?= /usr/local
LIBDIR ?= $(PREFIX)/lib
INCLUDEDIR ?= $(PREFIX)/include

CFLAGS ?= -g -Wall -Wextra -pedantic -O2
CPPFLAGS ?= #-DNDEBUG
