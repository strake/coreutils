#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <utf.h>
#include <unistd.h>
#include "util.h"

void go (FILE *f) {
	char *x;
	int n;
	
	for (;;) {
		n = afgetln (&x, f);
		if (n < 0) return;
		
		fwrite (x, 1, n, stdout);
		free (x);
	}
}

int main (int argc, char *argu[]) {
#include "argPrae.h"
	case 'v':
		eprintf ("%s -v considered harmful\n", selfName);
#include "argPost.h"
	
	findFSRS (0);
	
#define GO(f) go (f)
#include "goCatlike.h"
	
	return 0;
}
