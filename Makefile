SHELL = /bin/sh

CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2

#dodatkowe foldery, gdzie wyszukiwane będą docelowe obiekty
VPATH=src/ bin/ obj/

SRCS=forests.c dataholder.c commandinterpreter.c commandhandler.c
OBJS=$(SRCS:.c=.o)

all: forests

.c.o:
	$(CC) $(CFLAGS) -c -o obj/$*.o  $<

forests: $(OBJS)
	$(CC) $(CFLAGS) -o bin/forests $(addprefix obj/, $(OBJS))

#źródło, gdzie znalazłem jak generować pliki z zależnościami
#http://mistyp.es/2014/11/improving-makefile-c-header-dependencies/
%.d: %.c
	@echo
	@echo $(@:.d=.o)
	gcc -MM -MT '$(@:.d=.o)' $< > obj/$@

ifneq ($(MAKECMDGOALS), clean)
-include $(SRCS:.c=.d)
endif

clean:
	-rm bin/forests
	-rm $(addprefix obj/, $(OBJS))
	-rm obj/*.d