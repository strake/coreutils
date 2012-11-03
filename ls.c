#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <utf.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <errno.h>
#include "util.h"

enum {
	aFlag =  1,
	dFlag =  2,
	lFlag =  4,
	tFlag =  8,
	RFlag = 16,
	slashFlag = 32,
};

unsigned int parseXD (char *s, int l) {
	unsigned int n = 0;
	for (int ii = 0; ii < l; ii++) {
		n *= 16;
		if (s[ii] <= '9' && s[ii] >= '0') {
			n += s[ii] - '0';
			continue;
		}
		if (s[ii] & 0xDF <= 'Z' && s[ii] & 0xDF >= 'A') {
			n += s[ii] - 'A';
			continue;
		}
		eprintf ("ls: no parse: %s\n", s);
	}
	return n;
}

void ls1 (char *fmt, int flags, char *path) {
	struct stat s;
	if (lstat (path, &s) < 0) eprintf ("ls: %s:", path);
	for (; fmt[0]; fmt++) {
		if (fmt[0] == '%') {
			int w = 0;
			char *mode, *p;
			if(!*fmt++) {
				fputs ("ls: bad format\n", stderr);
				exit (1);
			}
			mode = strdup ("----------");
			if (!mode) eprintf ("ls:");
			if (fmt[0] >= '0' && fmt[0] <= '9') w = strtoul (fmt, &fmt, 10);
			switch (fmt[0]) {
			case '%':
				fputc ('%', stdout);
				break;
			case 'n':
				fputc ('\n', stdout);
				break;
			case 't':
				fputc ('\t', stdout);
				break;
			case 'd':
				printf ("%*d", w ? w : 12, s.st_dev);
				break;
			case 'i':
				printf ("%*d", w ? w : 16, s.st_ino);
				break;
			case 'p':
				for (int ii = 0; ii < 9; ii++) {
					char *rwx;
					rwx = "xwr";
					if (s.st_mode & (1 << ii)) mode[9 - ii] = rwx[ii % 3];
				}
				if (s.st_mode & S_IFDIR) mode[0] = 'd';
				printf ("%s", mode);
				break;
			case 'l':
				printf ("%*d", w ? w : 4, s.st_nlink);
				break;
			case 'u':
				printf ("%*d", w ? w : 12, s.st_uid);
				break;
			case 'g':
				printf ("%*d", w ? w : 12, s.st_gid);
				break;
			case 'm':
				printf ("%*d", w ? w : 20, s.st_mtime);
				break;
			case 'x':
				printf ("%c", parseXD (++fmt, 4));
				fmt += 3;
				break;
			case 'z':
				printf ("%*d", w ? w : 16, s.st_size);
				break;
			case 'N':
				p = flags & slashFlag ? 0 : utfrrune (path, L'/');
				fputs (p ? p + runelen (L'/') : path, stdout);
				break;
			default:
				fprintf (stderr, "ls: unrecognized format spec: %c\n", fmt[0]);
				exit (1);
			}
			free (mode);
		}
		else fputc (fmt[0], stdout);
	}
}

int noDot (const struct dirent *e) {
	return (e -> d_name[0] != '.');
}

int comparMTime (const struct dirent **e, const struct dirent **f) {
	struct stat s, t;
	if (lstat ((*e) -> d_name, &s) < 0) eprintf ("ls: %s:", (*e) -> d_name);
	if (lstat ((*f) -> d_name, &t) < 0) eprintf ("ls: %s:", (*f) -> d_name);
	return ((int)(t.st_mtime - s.st_mtime));
}

int comparName (const struct dirent **e, const struct dirent **f) {
	return (strcmp ((*e) -> d_name, (*f) -> d_name));
}

void ls (char *fmt, int flags, char *path) {
	struct dirent **es;
	int n;
	if (flags & dFlag) {
		ls1 (fmt, flags, path);
		return;
	}
	n = scandir (path, &es, flags & aFlag ? 0 : noDot, flags & tFlag ? comparMTime : comparName);
	if (n < 0) {
		if (errno == ENOTDIR) {
			ls1 (fmt, flags, path);
			return;
		}
		else eprintf ("ls: %s:", path);
	}

	if (flags & RFlag) ls1 (fmt, flags, path);
	for (int ii = 0; ii < n; ii++) {
		char *subpath;
		if (asprintf (&subpath, "%s/%s", path, es[ii] -> d_name) < 0) eprintf ("ls:");
		(flags & RFlag ? ls : ls1) (fmt, flags, subpath);
		free (subpath);
	}
}

int main (int argc, char *argu[]) {
	int flags = 0;
	char *fmt;
	fmt = 0;
#include "argPrae.c"
	case 'a':
		flags |= aFlag;
		break;
	case 'd':
		flags |= dFlag;
		break;
	case 'l':
		flags |= lFlag;
		break;
	case 't':
		flags |= tFlag;
		break;
	case 'f':
		fmt = argu[++ii];
		goto nextArgument;
	case 'R':
		flags |= RFlag;
		break;
	case '/':
		flags |= slashFlag;
		break;
#include "argPost.c"
	
	findFSRS (FSRSNoSlash);

	if (!(flags & lFlag) && asprintf (&fmt, "%%N%C", rs) < 0) eprintf ("%s:", selfName);
	if (!fmt) fmt = "%p  %l %u %g %z %m %N%n";
	if (argc != 0) for (int ii = 0; ii < argc; ii++) ls (fmt, flags, argu[ii]);
	else ls (fmt, flags, ".");
	
	return 0;
}
