#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <utf.h>
#include <errno.h>
#include "util.h"

int main (int argc, char *argu[]) {
	int n;
	
#include "argPrae.c"
#include "argPost.c"
	
	findFSRS (0);
	
	for (;;) {
		char *x, *y, *z, **argv;
		int ii;
		
		n = afgetln (&x, stdin);
		if (n == 0) break;
		if (n < 0) {
			if (errno) eprintf ("%s:", argu[0]);
			else return 0;
		}
		z = x;

		argv = malloc (sizeof (char *) * (argc + 1));
		if (!argv) eprintf ("%s:", argu[0]);
		for (ii = 0; ii < argc - 1; ii++) argv[ii] = argu[ii + 1];
		if (ii == 0) argv[ii++] = "echo";
		/* ii now index of xargument in argv */
		
		for (; x; x = y) {
			int pid, c;
			
			y = utfrune (x, fs);
			if (y) {
				Rune _;
				int l = chartorune (&_, y);
				y[0] = 0;
				y += l;
			}
			else {
				y = utfrune (x, rs);
				if (y) y[0] = 0;
				y = 0;
			}
			
			argv[ii]     = x;
			argv[ii + 1] = 0;
			
			pid = fork ();
			if (pid == 0) {
				execvp (argv[0], argv);
				eprintf ("%s: %s:", argu[0], argv[0]);
			}
			waitpid (pid, &c, 0);
			if (c != 0) return 1;
		}
	}
}
