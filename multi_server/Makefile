
CFLAGS = -g --std=gnu99 -D DEBUG=true
CC = gcc

all:
	make clean && make server.out

server.out: multi_server_thread.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	-rm -rf server.out
