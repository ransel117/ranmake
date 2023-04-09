#!/bin/bash

CC="gcc"
CFLAGS="-Wall -Wpedantic -ggdb -std=c99"
CDEFINES=""
CINCLUDES=""
CFILES="main.c"
CLIBS=""

EXEC="main"

echo "--------COMPILING--------"
echo "$CC $CFLAGS $CDEFINES $CINCLUDES $CFILES -o $EXEC $CLIBS"
$CC $CFLAGS $CDEFINES $CINCLUDES $CFILES -o $EXEC $CLIBS

echo "---------RUNNING---------"
echo "./$EXEC"
./$EXEC
