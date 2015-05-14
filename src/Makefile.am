AUTOMAKE_OPTIONS = foreign

CFLAGS = --pedantic -Wall -std=c99 -O3 $(shell pkg-config --cflags --libs MagickWand)
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libdhash.la

libdhash_la_SOURCES = dhash.c

include_HEADERS = dhash.h
