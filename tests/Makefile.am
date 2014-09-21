## Process this file with automake to produce Makefile.in

TESTS = check_tmxeditor
check_PROGRAMS = check_tmxeditor
check_tmxeditor_SOURCES = check_tmxeditor.c $(top_builddir)/src/tmxeditor.h
check_tmxeditor_CFLAGS = @CHECK_CFLAGS@
check_tmxeditor_LDADD = $(top_builddir)/src/libtmxeditor.la @CHECK_LIBS@
