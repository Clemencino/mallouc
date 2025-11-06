#!/bin/bash
set -e

# Chemin absolu vers libmalloc.so
LIB=$(pwd)/libmalloc.so

echo "=== Compilation de la librairie ==="
make library

echo "=== Compilation du programme de test ==="
gcc tests/main_test.c -L. -lmalloc -o tests/main_test -Wl,-rpath=$(pwd)

echo "=== Lancement du test avec LD_PRELOAD ==="
LD_PRELOAD=$LIB ./tests/main_test

