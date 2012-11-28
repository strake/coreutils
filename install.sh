#!/bin/sh

mode=f;
DFlag='';
SFlag='';
sFlag='';
unset usr grp perm;

while getopts 'DScdg:m:o:s' o; do
	case $o in
		(D)	DFlag=yes;;
		(S)	SFlag=yes;;
		(d)	mode=d;;
		(g)	grp="$OPTARG";;
		(m)	perm="$OPTARG";;
		(o)	usr="$OPTARG";;
		(s)	sFlag=yes;;
	esac
done

shift $(dc -e "$OPTIND 1 - p");

function install1 {
	test -n $DFlag && test -n "$(dirname $2)" && mkdir -p "$(dirname $2)";
	if test -n $SFlag; then
		unset TMPDIR;
		tmp="$(mktemp -p $(dirname $2))" || exit;
		cp "$1" "$tmp";
		mv "$tmp" "$2";
	else cp "$1" "$2"; fi
	test -n "$sFlag" && "${STRIP:-strip}" "$2"
	test -n "$usr"  && chown  "$usr" "$2";
	test -n "$grp"  && chown :"$grp" "$2";
	test -n "$perm" && chmod "$perm" "$2";
}

case $mode in
	(d)	mkdir -p "$@";
		test -n "$usr"  && chown  "$usr" "$@";
		test -n "$grp"  && chown :"$grp" "$@";
		test -n "$perm" && chmod "$perm" "$@";
		;;
	(f)	for t in "$@"; do true; done # find last positional parameter
		if test -d "$t"; then
			case "$t" in
				(*/)	true;;
				(*)	t="$t/";;
			esac
		fi
		case $# in
			(0)	echo "$0: No files given" >&2;
				exit 1;;
			(1)	echo "$0: No target path given" >&2;
				exit 1;;
			(2)	case "$t" in
					(*/)	true;;
					(*)	install1 "$1" "$2"; exit;;
				esac;&
			(*)	case "$t" in
					(*/)	while test -n "$2"; do install1 "$1" "$t$(basename $1)"; shift; done;;
					(*)	echo "$t: Not a directory" >&2; exit 1;;
				esac;;
		esac;;
esac
