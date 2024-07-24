.POSIX:
.SUFFIXES:
CC     = cc
CFLAGS = -Wall -O -g
LDLIBS = -lcjson -Wl,-rpath=/usr/local/lib

all: parse test_parse

parse: src/parse.c src/parse.h
	$(CC) $(CFLAGS) $< $(LDLIBS) -o parse

test_parse: src/parse.c src/parse.h
	$(CC) -DTESTING $(CFLAGS) $< $(LDLIBS) -o test_parse

test: test_parse
	./test_parse

clean:
	rm test_parse parse