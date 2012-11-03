#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <utf.h>
#include "util.h"

int main (int argc, char *argu[]) {
	char *path, *suffix, *pEnd, *sEnd;
	int ii;

	if (argc < 2) eprintf ("%s: no arguments\n", argu[0]);
	
	path = strrchr (argu[1], '/');
	suffix = argc > 2 ? argu[2] : "";
	if (path) path++; else path = argu[1];
	
	pEnd = &path[strlen (path)];
	sEnd = &suffix[strlen (suffix)];
	
	for (ii = 0; path[ii] && suffix[ii]; ii++) if (pEnd[-ii] != sEnd[-ii]) goto finish;
	pEnd[-ii] = 0;
	
finish:	fputs (path, stdout);
	return 0;
}
