#!/bin/bash

INSTALLDIR="/usr/bin"

echo "[$0] install bin: pre-install checks" >&2

if [ $EUID -ne 0 ]
then
	echo "[$0] install bin: run the script as root!" >&2
	exit 1
fi

stat nimped > /dev/null 2> /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] install bin: build the program before installing!" >&2
	exit 1
fi

echo "[$0] install bin: copying" >&2
cp nimped $INSTALLDIR 2> /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] install bin: failed to copy!" >&2
	exit 1
fi

echo "[$0] install bin: finished successfully" >&2
