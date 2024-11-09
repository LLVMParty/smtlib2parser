CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99 -pedantic -g -Wall
LDFLAGS = -g -lgmp -lgmpxx -lstdc++ 

OBJECTS = smtlib2bisonparser.o \
	  smtlib2flexlexer.o \
	  smtlib2hashtable.o \
	  smtlib2abstractparser.o \
	  smtlib2termparser.o \
	  smtlib2utils.o \
	  smtlib2vector.o \
	  smtlib2charbuf.o \
	  smtlib2stream.o \
	  smtlib2scanner.o

all: libsmtlib2parser.a

clean:
	rm -f $(OBJECTS) libsmtlib2parser.a smtlib2bisonparser.c smtlib2bisonparser.h smtlib2flexlexer.c smtlib2flexlexer.h


libsmtlib2parser.a: $(OBJECTS)
	ar rc $@ $(OBJECTS)
	ranlib $@



%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


smtlib2bisonparser.c: smtlib2bisonparser.y smtlib2flexlexer.c
	$(BISON) -o $@ $<

smtlib2flexlexer.c: smtlib2flexlexer.l
	$(FLEX) --header-file="smtlib2flexlexer.h" -o $@ $<
