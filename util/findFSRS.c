#include <stdio.h>
#include <utf.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../util.h"

Rune fs, rs;

void findFSRS (int flags) {
	char *fsp, *rsp;
	
	fsp = getenv ("FS");
	rsp = getenv ("RS");

	fs = '\t';
	rs = '\n';
	
	if (fsp && chartorune (&fs, fsp) == 0) eprintf ("Bad rune in FS\n");
	if (rsp && chartorune (&rs, rsp) == 0) eprintf ("Bad rune in RS\n");

	if (flags & FSRSNoSlash && (fs == '/' || rs == '/')) eprintf ("∀ X . Will not use slash as XS!\n");
}
