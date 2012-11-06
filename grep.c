#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <utf.h>
#include <sys/types.h>
#include <regex.h>
#include "util.h"

enum {
	nFlag = 1,
	vFlag = 2,
};

int grep1 (char mode, int flags, const regex_t *p, FILE *f, char *name) {
	char *x;
	int m = 0, n;
	
	for (n = 1; afgetln (&x, f) >= 0; n++) {
		if (regexec (p, x, 0, 0, 0) == (flags & vFlag ? 0 : REG_NOMATCH)) continue;
		m++;
		
		switch (mode) {
		case 'c':
			break;
		case 'l':
			printf ("%s%C", name, rs);
			goto finish;
		case 'q':
			exit (0);
		default:
			if (name)          printf ("%s%C", name, fs);
			if (flags & nFlag) printf ("%d%C", n,    fs);
			fputs (x, stdout);
			break;
		}
	}
	
	if (mode == 'c') printf ("%d%C", m, rs);
	
finish:	free (x);
	return m;
}

int main (int argc, char *argu[]) {
	int flags = 0, m = 0;
	char mode = 0;
	int regflags = 0;
	regex_t re;
	
#include "argPrae.c"
	case 'E':
		regflags |= REG_EXTENDED;
		break;
	case 'c':
	case 'l':
	case 'q':
		mode = argu[ii][jj];
		break;
	case 'i':
		regflags |= REG_ICASE;
		break;
	case 'n':
		flags |= nFlag;
		break;
	case 'v':
		flags |= vFlag;
		break;
#include "argPost.c"

	if (argc <= 1) enprintf (2, "%s: No RE\n", argu[0]);
	if (regcomp (&re, argu[1], regflags) != 0) {
		enprintf (2, "%s:", argu[0]);
	}
	argu[1] = argu[0];
	argc--;
	argu++;
	
	findFSRS (0);
	
#define GO(f, name) { int n; n = grep1 (mode, flags, &re, f, name); if (n >= 0) m += n; }
#include "goCatlike.c"

	return (m ? 0 : 1);
}
