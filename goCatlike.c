for (int ii = 1; ii < argc; ii++) {
	FILE *f;
	f = strcmp ("-", argu[ii]) == 0 ? stdin : fopen (argu[ii], "r");
	if (!f) enprintf (0, "%s: %s:", argu[0], argu[ii]);
	GO(f);
	if (f && f != stdin) fclose (f);
}
if (argc <= 1) GO(stdin);
