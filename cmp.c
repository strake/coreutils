#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <utf.h>
#include <unistd.h>
#include <fcntl.h>
#include "util.h"

enum {
	lFlag = 1,
	sFlag = 2,
};

int lesser (int x, int y) {
	return (x < y ? x : y);
}

int main (int argc, char *argu[]) {
	int f = 0, g = 0, flags = 0, c = 0;
	int fSkip = 0, gSkip = 0;
	int b = 0, l = 0; /* no. of bytes, lines read */
	
#include "argPrae.c"
	case 'l':
		flags |= lFlag;
		break;
	case 's':
		flags |= sFlag;
		break;
#include "argPost.c"
	
	if (argc < 3) eprintf ("%s: too few arguments\n", argu[0]);
	
	findFSRS (0);
	
	f = strcmp (argu[1], "-") ? open (argu[1], O_RDONLY) : 0;
	g = strcmp (argu[2], "-") ? open (argu[2], O_RDONLY) : 0;
	
	if (f < 0 || g < 0) enprintf (2, "%s:", argu[0]);
	
	if (argc >= 4) fSkip = strtoul (argu[3], 0, 10);
	if (argc >= 5) gSkip = strtoul (argu[4], 0, 10);
	
	if (f == g) return (fSkip == gSkip ? 0 : 1);
	
	while (fSkip > 0 || gSkip > 0) {
		int m, n;
		char x[4096];
		m = read (f, x, lesser (fSkip, 4096));
		n = read (g, x, lesser (gSkip, 4096));
		if (m < 0 || n < 0) enprintf (2, "%s:", argu[0]);
		fSkip -= m;
		gSkip -= n;
	}
	
	for (;;) {
		int m, n;
		char x, y;
		m = read (f, &x, 1);
		n = read (g, &y, 1);
		if (m  < 0 || n  < 0) enprintf (2, "%s:", argu[0]);
		if (m == 0 && n == 0) return c;
		b++;
		if (x == rs || y == rs) l++; /* Not sure which line numbers to use, so use both */
		if (m == 0 || n == 0) {
			c = 1;
			if (flags & lFlag) {
				if (!(flags & sFlag)) printf ("%d%C%o%C%o%C", b, fs, x, fs, y, rs);
			}
			else enprintf (1, "%s: EOF on %s\n", argu[0], argu[m?2:1]);
		}
		if (x != y) {
			c = 1;
			if (!(flags & sFlag)) {
				if (flags & lFlag) printf ("%d%C%o%C%o%C", b, fs, x, fs, y, rs);
				else printf ("%s ≠ %s: byte %d, line %d%C", argu[1], argu[2], b, l, rs);
			}
		}
	}
}
