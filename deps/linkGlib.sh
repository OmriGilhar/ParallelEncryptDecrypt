#!/bin/bash

if [ ! -L "/usr/include/glib-2.0/glibconfig.h" ]; then
	ln -s /usr/lib/x86_64-linux-gnu/glib-2.0/include/glibconfig.h /usr/include/glib-2.0/glibconfig.h
else
	echo "Link to glib is stable"
fi

