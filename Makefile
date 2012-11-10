include config.mk

SUFFIXES = .c .o

LIB_UTIL = afgetr eprint findFSRS rune
LIB = ${LIB_UTIL:S/$/.o/:S/^/util\//}

MAN = basename.1 chroot.8 cat.1 cmp.1 cut.1 dirname.1 du.1 echo.1 expr.1 false.1 fold.1 grep.1 ls.1 mktemp.1 nl.1 seq.1 test.1 true.1 xargs.1 yes.1

SRC = ${MAN:S/.1/.c/:S/.8/.c/}
OBJ = ${SRC:S/.c/.o/}
BIN = ${SRC:S/.c//}

all:	${BIN}

$(OBJ):	util.h config.mk
$(BIN):	util.a

cat.o cut.o fold.o: goCatlike.c

.o:
	${LD} -o $@ $< util.a ${LDFLAGS}

.c.o:
	${CC} -c -o $@ $< ${CFLAGS}

util.a:	${LIB}
	${AR} -r -c $@ ${LIB}
	ranlib $@

clean:
	rm -f ${BIN} ${OBJ} ${LIB} util.a
