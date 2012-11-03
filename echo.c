#include <stdio.h>
#include <stdarg.h>
#include <utf.h>
#include "util.h"

enum {
	nFlag = 1,
};

int main (int argc, char *argu[]) {
	int flags = 0;
	int ii;
	
#include "argPrae.c"
	case 'n':
		flags |= nFlag;
		break;
#include "argPost.c"
	
	findFSRS (0);
	
	for (ii = 0; ii < argc; ii++) {
		if (ii > 0) fputrune (fs, stdout);
		fputs (argu[ii], stdout);
	}
	if (!(flags & nFlag)) fputrune (rs, stdout);
	
	return 0;
}
