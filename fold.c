#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <utf.h>
#include "util.h"

enum {
	sFlag = 1,
	bFlag = 2,
};

void go (int w, int flags, FILE *f) {
	for (;;) {
		char *x, *y, *z;
		Rune _;
		int n;
		
		n = afgetln (&x, f);
		if (n < 0) return;
		z = x;
		
		if (flags & sFlag) {
			for (y = x; y; y = utfrune (x, fs)) {
				if (y - x >= w) {
					int l = chartorune (&_, y);
					y[0] = 0;
					fputs (x,  stdout);
					fputc (rs, stdout);
					x = y + l;
				}
			}
			fputs (x, stdout);
		}
		else {
			y = utfrrune (x, rs);
			if (y) y[0] = 0; /* not print rs twice */
			for (n = flags & bFlag ? n : utflen (x); x[0] && n > 0; n -= w) {
				for (int m = 0; m < w; m++) {
					if (!x[0]) break;
					(flags & bFlag ? fputc : fputrune) (x[0], stdout);
					x += flags & bFlag ? 1 : chartorune (&_, x);
				}
				fputrune (rs, stdout);
			}
		}
		
		free (z);
	}
}

int main (int argc, char *argu[]) {
	int w = 80, flags = 0;

#include "argPrae.c"
	case 's':
		flags |= sFlag;
		break;
	case 'w':
		w = strtoul (argu[++ii], 0, 10);
		goto nextArgument;
#include "argPost.c"
	
	findFSRS (0);
	
#define GO(f, name) go (w, flags, f)
#include "goCatlike.c"
}
