PREFIX = /usr/local
CC = cc

all:
	${CC} scramble.c -o scramble

clean:
	rm -f scramble

install: all
	mkdir -p ${PREFIX}/bin
	cp -f scramble ${PREFIX}/bin
	chmod 755 ${PREFIX}/bin/scramble

uninstall:
	rm -f ${PREFIX}/bin/scramble
