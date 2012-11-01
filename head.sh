#!/bin/sh

n=10;

while getopts 'n:' o; do
	case $o in
		(n)	n="$OPTARG";;
	esac
done

shift $(dc -e "$OPTIND 1 - p");

exec sed ${n}q "$@";
