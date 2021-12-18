
CFLAGS = -g --std=gnu99 -D DEBUG=true
CC = gcc

all:
	make clean && make server.out

server.out: multi_server_thread_blocking_queue.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	-rm -rf server.out
