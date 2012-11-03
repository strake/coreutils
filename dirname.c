#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <utf.h>
#include "util.h"

int main (int argc, char *argu[]) {
	char *path, *p;

	if (argc < 2) eprintf ("%s: no arguments\n", argu[0]);
	
	path = argu[1];
	p = strrchr (path, '/');
	if (p) p[0] = 0; else path = ".";
	
	fputs (path, stdout);
	return 0;
}
