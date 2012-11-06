#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <utf.h>
#include "util.h"

typedef struct {
	int min, max;
} Range;

int inRange (Range r, unsigned int n) {
	if ((0 == r.max || n <= r.max) && n >= r.min) return 1;
	else return 0;
}

void cutLineF (Rune d, unsigned int s, Range *ranges, char *x) {
	int n, nMax;
	char **xs;
	if (!utfrune (x, d)) {
		if (!s) fputs (x, stdout);
		return;
	}
	xs = malloc (sizeof (char *) * (strlen (x) + 2));
	if (!xs) eprintf ("cut:");
	for (n = 1; x; n++) {
		xs[n] = x;
		x = utfrune (x, d);
		if (x) {
			*x = 0;
			x += runelen (d);
		}
	}
	xs[n] = xs[n-1] + strlen (xs[n-1]);
	nMax = n;
	for (int ii = 0; ranges[ii].min; ii++) {
		if (nMax <= ranges[ii].min) continue;
		for (n = ranges[ii].min; (ranges[ii].max ? n <= ranges[ii].max : 1) && n < nMax; n++) {
			fputs (xs[n], stdout);
			fputrune (d, stdout);
		}
	}
	free (xs);
}

void cutLineC (Range *ranges, char *x) {
	Rune _r;
	int n;
	for (int ii = 0; ranges[ii].min; ii++) {
		char *y;
		y = x;
		for (n = 1; *y; n++) {
			int l = chartorune (&_r, y);
			if (inRange (ranges[ii], n)) fwrite (y, 1, l, stdout);
			y += l;
		}
	}
}

void cutLineB (Range *ranges, char *x) {
	int n;
	for (int ii = 0; ranges[ii].min; ii++) {
		for (n = ranges[ii].min - 1; ranges[ii].max ? n < ranges[ii].max : x[n]; n++) {
			fputc (x[n], stdout);
		}
	}
}

void go (int mode, Rune d, unsigned int s, Range *ranges, FILE *f) {
	char *x, *y;
	x = 0; y = 0;

	while (afgetln (&x, f) > 0) {
		/* must delete newline here, and redo later;
		   otherwise, unknown whether it was included in cut */
		y = utfrune (x, rs);
		if (y) y[0] = 0;
		switch (mode) {
		case 'f':
			if (!utfrune (x, d)) {
				if (s) break;
				fputs (x, stdout);
			}
			else cutLineF (d, s, ranges, x);
			fputrune (rs, stdout);
			break;
		case 'c': cutLineC (ranges, x); fputrune (rs, stdout); break;
		case 'b': cutLineB (ranges, x); fputrune (rs, stdout); break;
		}
	}
}

int main (int argc, char *argu[]) {
	int mode = 0;
	unsigned int s = 0;
	Range *ranges = 0;
	
	findFSRS (0);
	
	/* parse options */
#include "argPrae.c"
	case 'b':
	case 'c':
	case 'f':
		mode = argu[ii][jj];
		
		if (++ii >= argc) {
			fputs ("cut: No range argument\n", stderr);
			return 1;
		}
		
		ranges = malloc (sizeof (Range) * (utflen (argu[ii]) + 1));
		if (!ranges) eprintf ("cut:");
		
		/* ensure space delimitation for strtoul */
		for (jj = 0; argu[ii][jj]; jj++) if (argu[ii][jj] == ',') argu[ii][jj] = ' ';
		
		/* parse ranges */
		/* max = 0 to denote last */
		{
			char *p;
			p = argu[ii];
			jj = 0;
			while (*p) {
				ranges[jj].min = *p == '-' ? 1 : strtoul (p, &p, 10);
				ranges[jj].max = *p == '-' ? strtoul (++p, &p, 10) : ranges[jj].min;
				switch (*p) {
				case '\0':
					ranges[++jj].min = 0;
					break;
				case ' ':
				case '\f':
				case '\v':
				case '\t':
				case '\r':
				case '\n':
					jj++;
					break;
				default:
					fputs ("cut: Malformed ranges\n", stderr);
					return 1;
				}
			}
		}
		goto nextArgument;
	case 'd':
		chartorune (&fs, argu[++ii]);
		goto nextArgument;
	case 's':
		s = 1;
		break;
#include "argPost.c"

	if (!mode) {
		fputs ("cut: No mode given\n", stderr);
		return 1;
	}

#define GO(f, name) go (mode, fs, s, ranges, f);
#include "goCatlike.c"
	
	return 0;
}
