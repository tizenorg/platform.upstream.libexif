#! /bin/sh

libtoolize --copy --force
aclocal -I m4m
autoheader
autoconf
automake -a -c
