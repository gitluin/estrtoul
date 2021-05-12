CFLAGS= -Wall -g
LDADD=
LDFLAGS=

SRC= estrtoul.c
OBJ= ${SRC:.c=.o}

all: estrtoul

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}:

estrtoul: ${OBJ}
	${CC} ${LDFLAGS} -o $@ ${OBJ} ${LDADD}

clean:
	rm -f *.o estrtoul
