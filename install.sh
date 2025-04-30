#!/bin/bash

INSTALLDIR="/usr/bin"

echo "[$0] install: pre-install checks" >&2

if [ $EUID -ne 0 ]
then
	echo "[$0] install: run the script as root!" >&2
	exit 1
fi

stat nimped > /dev/null 2> /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] install: build the program before installing!" >&2
	exit 1
fi

echo "[$0] install: copying" >&2
cp nimped $INSTALLDIR 2> /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] install: failed to copy!" >&2
	exit 1
fi

echo "[$0] install: finished successfully" >&2
