#include <stdio.h>
#include <stdarg.h>
#include <utf.h>
#include "util.h"

int main (int argc, char *argu[]) {
	findFSRS (0);
	
	for (;;) {
		for (int ii = 1; ii < argc; ii++) printf ("%s%C", argu[ii], ii + 1 == argc ? rs : fs);
		if (argc < 2) printf ("y%C", rs);
	}
}
