
CFLAGS = -g --std=gnu99
CC = gcc

all:
	make clean && make server.out

server.out: multi_server.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	-rm -rf server.out
