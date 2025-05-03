#!/bin/bash

INSTALLDIR="/usr/bin"

echo "[$0] uninstall: pre-uninstall checks" >&2

if [ $EUID -ne 0 ]
then
	echo "[$0] uninstall: run the script as root!" >&2
	exit 1
fi

stat $INSTALLDIR/nimped > /dev/null 2> /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] uninstall: the program is not installed!" >&2
	exit 1
fi

echo "[$0] uninstall: deleting" >&2
rm $INSTALLDIR/nimped 2> /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] uninstall: failed to delete!" >&2
	exit 1
fi

echo "[$0] uninstall: finished successfully" >&2
