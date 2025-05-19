#!/bin/bash

THEME="gruvbox-dark"
INSTALLDIR="$HOME/.config"

echo "[$0] install conf: creating fresh conf directory" >&2
rm -rf $INSTALLDIR/nimped 2> /dev/null

mkdir $INSTALLDIR/nimped 2> /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] install conf: failed to create conf directory!" >&2
	exit 1
fi

echo "[$0] install conf: copying conf" >&2
cp -r conf/* $INSTALLDIR/nimped 2> /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] install conf: failed to copy conf!" >&2
	exit 1
fi

echo "[$0] install conf: copying theme" >&2
cp themes/$THEME.conf $INSTALLDIR/nimped/color.conf 2> /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] install conf: failed to copy theme!" >&2
	exit 1
fi

echo "[$0] install conf: finished successfully" >&2
