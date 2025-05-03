#!/bin/bash

INCLUDE="-Isrc"
DEFINES=""
WARNINGS="-Wall -Wextra -Wshadow"
LIBRARIES=""
CFLAGS="-std=c99 -pedantic -O3 -D_GNU_SOURCE"

CC=gcc
CFLAGSFULL="$INCLUDE $DEFINES $WARNINGS $CFLAGS $LIBRARIES"

echo "[$0] build: compilation" >&2
$CC -o nimped src/main.c $CFLAGSFULL
if [ $? -ne 0 ]
then
	echo "[$0] build: failed to compile!" >&2
	exit 1
fi

echo "[$0] build: finished successfully" >&2
