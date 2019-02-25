#!/bin/bash


SOURCES=`ls src/*.c 2>/dev/null`

[ -z $SOURCES ] && exit 1


CFLAGS="-g -Wall -Iinclude "
CFLAGS+=` pkg-config --libs gtk+-3.0 webkit2gtk-4.0`
LIBS=` pkg-config --libs gtk+-3.0 webkit2gtk-4.0`

for src in ${SOURCES[@]}
do
    target="bin/`basename $src ".c"`"
    echo "compile $src ...  --> [$target]"
    gcc -std=gnu99 -o $target $src $CFLAGS $LIBS
done
