#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <utf.h>
#include "../util.h"

int afgetln (char **p, FILE *f) {
	return afgetr (p, rs, f);
}

int afgetr (char **p, Rune s, FILE *f) {
	size_t size = 1;
	int n = 0;
	Rune *x;
	
	*p = 0;
	x = malloc (sizeof (Rune) * size);
	for (;;) {
		long c;
		c = fgetrune (f);
		if (c < 0) return c;
		x[n++] = c;
		if (c == s) break;
		if (n >= size) {
			size *= 2;
			x = realloc (x, sizeof (Rune) * size);
			if (!x) return (-1);
		}
	}

	*p = malloc (runenlen (x, n));
	if (!*p) return -1;
	return runentochar (*p, x, n);
}
