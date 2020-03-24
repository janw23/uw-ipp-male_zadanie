SHELL = /bin/sh

CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2

#dodatkowe foldery, gdzie wyszukiwane będą docelowe obiekty
VPATH=src/ bin/ obj/

SRCS=forests.c dataholder.c commandinterpreter.c commandhandler.c
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)

all: forests

.c.o:
	$(CC) $(CFLAGS) -c -o obj/$*.o  $<

bin/:
	@-mkdir bin

obj/:
	@-mkdir obj

forests: bin obj $(OBJS)
	$(CC) $(CFLAGS) -o bin/forests $(addprefix obj/, $(OBJS))

#www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
obj/%.d: %.c
	$(CC) -MM -MT $(notdir $(@:.d=.o)) $< > $@

#pomija wywołanie 'include' dla make clean
ifneq ($(MAKECMDGOALS), clean)
-include $(addprefix obj/, $(DEPS))
endif

.PHONY: clean
clean:
	-rm bin/forests
	-rm $(addprefix obj/, $(OBJS))
	-rm obj/*.d