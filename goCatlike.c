if (argc != 0) for (int ii = 0; ii < argc; ii++) {
	FILE *f;
	f = strcmp ("-", argu[ii]) == 0 ? stdin : fopen (argu[ii], "r");
	if (!f) enprintf (0, "%s: %s:", selfName, argu[ii]);
	GO(f);
	if (f && f != stdin) fclose (f);
}
else GO(stdin);
