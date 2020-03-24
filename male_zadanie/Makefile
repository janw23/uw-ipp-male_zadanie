SHELL = /bin/sh

CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2

#dodatkowe foldery, gdzie wyszukiwane będą docelowe obiekty
VPATH=src/ bin/ obj/

SRCS=forests.c dataholder.c commandinterpreter.c commandhandler.c
DEPS=$(SRCS:.c=.d)
OBJS=$(SRCS:.c=.o)

all: forests

%.d: %.c
	gcc -MM -MT '$(@:.d=.o)' $< > obj/$@

.c.o:
	$(CC) $(CFLAGS) -c -o obj/$*.o  $<

forests: bin obj $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o bin/forests $(addprefix obj/, $(OBJS))

bin:
	@-mkdir bin

obj:
	@-mkdir obj

clean:
	-rm bin/forests
	-rm $(addprefix obj/, $(OBJS))
	-rm obj/*.d