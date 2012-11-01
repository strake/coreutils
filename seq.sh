#!/bin/sh

wFlag='';

while getopts 's:f:w' o; do
	case $o in
		(s)	RS="$OPTARG";;
		(w)	wFlag=yes;;
	esac
done

shift $(dc -e "$OPTIND 1 - p");

start=1;
step=1;

case $# in
	(1)	stop="$1";;
	(2)	start="$1"; stop="$2";;
	(3)	start="$1"; step="$2"; stop="$3";;
esac

test -z "$RS" && RS='\n';

exec awk -v ORS="$RS" -v wFlag="$wFlag" -v start="$start" -v stop="$stop" -v step="$step" 'BEGIN { for (n = start; n <= stop; n += step) { printf "%0*d", length (wFlag) ? length (stop) : 0, n; print; } }'
