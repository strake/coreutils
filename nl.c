#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <utf.h>
#include <sys/types.h>
#include <regex.h>
#include "util.h"

regex_t re;

/* return whether line must have number */
int chkLn (char mode, char *x) {
	switch (mode) {
	case 'a':
		return 1;
	case 'n':
		return 0;
	case 'p':
		return !regexec (&re, x, 0, 0, 0);
	case 't':
		return (x[0] != rs);
	}
}

void go (char mode, FILE *f) {
	char *x;
	int n = 0;
	
	while (afgetln (&x, f) >= 0) {
		if (chkLn (mode, x)) printf ("%12d", ++n);
		else fputs ("            ", stdout);
		printf ("%C%s", fs, x);
	}
}

int main (int argc, char *argu[]) {
	char mode = 't';
	int regflags = REG_NOSUB;
	
#include "argPrae.c"
	case 'E':
		regflags |= REG_EXTENDED;
		break;
	case 'b':
		mode = argu[++ii][0];
		if (mode == 'p' && regcomp (&re, argu[ii] + 1, regflags) != 0) {
			eprintf ("%s:", argu[0]);
		}
		goto nextArgument;
#include "argPost.c"
	
	findFSRS (0);
	
#define GO(f, name) go (mode, f);
#include "goCatlike.c"
}
