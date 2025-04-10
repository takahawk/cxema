CC ?= gcc
CFLAGS = -g -I./
HEADERS = *.h
OBJS = *.o

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

repl: *.c execs/repl.c
	$(CC) -o $@ $^ $(CFLAGS)
