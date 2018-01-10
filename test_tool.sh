#!/bin/bash

TMP1=/tmp/tmp1
TMP2=/tmp/tmp2
#DEBUG=#

if [ $# -lt 1 ];then
	echo "Usage: $0 /path/to/test/dir"
	exit 1
fi
if [ ! -d $1 ];then
	echo "Usage: $0 /path/to/test/dir"
	exit 1
fi

for f in $1/*
do
	./ft_nm $f > $TMP1
	nm $f > $TMP2
	diff $TMP1 $TMP2  $DEBUG 1>&2 2>/dev/null
	if [ $? -ne 0 ]; then
		echo "File $f mismatch"
		exit 1
	fi
done


