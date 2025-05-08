#!/bin/bash

INSTALLDIR="$HOME/.config"

echo "[$0] install conf: creating fresh conf directory" >&2
rm -rf $INSTALLDIR/nimped 2> /dev/null

mkdir $INSTALLDIR/nimped 2> /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] install conf: failed to create conf directory!" >&2
	exit 1
fi

echo "[$0] install conf: copying" >&2
cp -r conf/* $INSTALLDIR/nimped 2> /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] install conf: failed to copy!" >&2
	exit 1
fi

echo "[$0] install conf: finished successfully" >&2
