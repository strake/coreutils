#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <utf.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "util.h"

enum {
	aFlag = 1,
	sFlag = 2,
};

int notAllDots (struct dirent *e) {
	for (int ii = 0; e -> d_name[ii]; ii++) if (e -> d_name[ii] != '.') return 1;
	return 0;
}

int grief = 0;

int du (int top, int bs, int flags, char *path) {
	struct stat s;
	struct dirent **es;
	if (lstat (path, &s) < 0) {
		fprintf (stderr, "du: %s: %s\n", path, strerror (errno));
		grief = 1;
		return 0;
	}
	if (s.st_mode & S_IFDIR) {
		int n;
		n = scandir (path, &es, notAllDots, 0);
		if (n < 0) {
			fprintf (stderr, "du: %s: %s\n", path, strerror (errno));
			grief = 1;
		}
		else
		for (int ii = 0; ii < n; ii++) {
			char *subpath;
			if (asprintf (&subpath, "%s/%s", path, es[ii] -> d_name) < 0) eprintf ("du:");
			s.st_size += du (0, bs, flags, subpath);
			free (subpath);
		}
	}
	if (top || (flags & aFlag || s.st_mode & S_IFDIR) && !(flags & sFlag)) printf ("%16d%C%s%C", s.st_size / bs, fs, path, rs);
	return s.st_size;
}
															
int main (int argc, char *argu[]) {
	int flags = 0;
	int bs = 512;
#include "argPrae.c"
	case 'a':
		flags |= aFlag;
		break;
	case 's':
		flags |= sFlag;
		break;
	case 'k':
		bs = 1024;
		break;
#include "argPost.c"
	
	findFSRS (FSRSNoSlash);
	
	for (int ii = 1; ii < argc; ii++) du (1, bs, flags, argu[ii]);
	if (argc <= 1) du (1, bs, flags, ".");
	
	return grief;
}
