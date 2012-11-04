#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <utf.h>
#include "util.h"

enum {
	wFlag = 1,
};

int main (int argc, char *argu[]) {
	int flags = 0, width = 0;
	unsigned long last = -1, step = 1, first = 1;
	
	findFSRS (0);
	
#include "argPrae.c"
	case 's':
		chartorune (&rs, argu[++ii]);
		goto nextArgument;
	case 'w':
		flags |= wFlag;
#include "argPost.c"
	
	switch (argc) {
	case 2:
		last = strtoul (argu[1], 0, 10);
		break;
	case 3:
		first = strtoul (argu[1], 0, 10);
		last  = strtoul (argu[2], 0, 10);
		break;
	case 4:
		first = strtoul (argu[1], 0, 10);
		step  = strtoul (argu[2], 0, 10);
		last  = strtoul (argu[3], 0, 10);
		break;
	}
	
	if (flags & wFlag) {
		FILE *f;
		
		f = fopen ("/dev/null", "w");
		if (!f) eprintf ("%s:", argu[0]);
		
		width = fprintf (f, "%d", last);
		
		fclose (f);
	}
	
	for (int n = first; n <= last; n += step) printf ("%0*d%C", width, n, rs);
}
