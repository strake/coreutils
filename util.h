extern Rune fs, rs;
enum {
	FSRSNoSlash = 1,
};
void findFSRS (int);

void venprintf (int, const char *, va_list);
void enprintf (int, const char *, ...);
void eprintf (const char *, ...);

int runentochar (char *, Rune *, int);

int fputrune (Rune, FILE *);
long fgetrune (FILE *);

int afgetln (char **, FILE *);
int afgetr (char **, Rune, FILE *);
