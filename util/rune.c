#include <stdio.h>
#include <utf.h>
#include <stdarg.h>
#include "../util.h"

int runentochar (char *s, Rune *r, int n) {
	int ii;
	for (ii = 0; ii < n; ii++) s += runetochar (s, &r[ii]);
	return ii;
}

long fgetrune (FILE *f) {
	Rune r;
	char x[UTFmax];
	int n = 0;
	
	while (n < UTFmax) {
		int c = fgetc (f);
		if (c < 0) return c;
		x[n++] = c;
		
		if (fullrune (x, n)) {
			chartorune (&r, x);
			return r;
		}
	}
	
	return Runeerror;
}

int fputrune (Rune r, FILE *f) {
	char x[UTFmax];
	int n;
	
	n = runetochar (x, &r);
	if (n == 0 || fwrite (x, 1, n, f) < n) return (-1);
	return n;
}
