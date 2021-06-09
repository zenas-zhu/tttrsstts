CFLAGS = -g -Wall -Wextra
IFLAGS = -Iinclude
LDFLAGS = -lncurses

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)
OBJECTS = $(patsubst src/%.c, obj/%.o, $(SOURCES))

.PHONY: default clean

default : ./main

./main : $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

obj/%.o : src/%.c $(HEADERS) | obj
	$(CC) $(CFLAGS) -fPIC $(IFLAGS) -c $< -o $@

obj :
	mkdir -p $@

clean :
	$(RM) $(OBJECTS) ./main
