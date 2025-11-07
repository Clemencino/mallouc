#!/bin/sh

LIB=../libmalloc.so

make library

gcc tests/main_test.c -L. -lmalloc -o tests/main_test -Wl,--rpath=$(pwd)

LD_PRELOAD=$LIB ./tests/main_test

