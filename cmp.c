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

int main (int argc, char *argu[]) {
	int f = 0, g = 0, flags = 0, c = 0;
	int b = 0, l = 0; /* no. of bytes, lines read */
	
#include "argPrae.h"
	case 'l':
		flags |= lFlag;
		break;
	case 's':
		flags |= sFlag;
		break;
#include "argPost.h"
	
	if (argc < 2) eprintf ("%s: too few arguments\n", selfName);

	findFSRS (0);
	
	f = strcmp (argu[0], "-") ? open (argu[0], O_RDONLY) : 0;
	g = strcmp (argu[1], "-") ? open (argu[1], O_RDONLY) : 0;
	
	if (f < 0 || g < 0) enprintf (2, "%s:", selfName);
	
	if (f == g) return 0;
	
	for (;;) {
		int m, n;
		char x, y;
		m = read (f, &x, 1);
		n = read (g, &y, 1);
		if (m  < 0 || n  < 0) enprintf (2, "%s:", selfName);
		if (m == 0 && n == 0) return c;
		b++;
		if (x == rs || y == rs) l++; /* Not sure which line numbers to use, so use both */
		if (m == 0 || n == 0) {
			c = 1;
			if (flags & lFlag) {
				if (!(flags & sFlag)) printf ("%d%C%o%C%o%C", b, fs, x, fs, y, rs);
			}
			else enprintf (1, "%s: EOF on %s\n", selfName, argu[m?1:0]);
		}
		if (x != y) {
			c = 1;
			if (!(flags & sFlag)) {
				if (flags & lFlag) printf ("%d%C%o%C%o%C", b, fs, x, fs, y, rs);
				else printf ("%s ≠ %s: byte %d, line %d%C", argu[0], argu[1], b, l, rs);
			}
		}
	}
}
