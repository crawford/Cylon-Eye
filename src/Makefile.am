AUTOMAKE_OPTIONS = foreign

AC_LDFLAGS = -I$(top_srcdir)

lib_LTLIBRARIES = libcylon.la
libcylon_la_SOURCES = cylon.c
libcylon_la_LDFLAGS = -release @VERSION@ -module $(AC_LDFLAGS)
include_HEADERS = cylon.h
