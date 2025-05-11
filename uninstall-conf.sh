#!/bin/bash

INSTALLDIR="$HOME/.config"

echo "[$0] uninstall conf: deleting conf directory" >&2
rm -rf $INSTALLDIR/nimped 2> /dev/null
if [ $? -ne 0 ]
then
	echo "[$0] uninstall conf: failed to delete!" >&2
	exit 1
fi

echo "[$0] uninstall conf: finished successfully" >&2
