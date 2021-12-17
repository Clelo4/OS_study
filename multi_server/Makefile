
CFLAGS = -g --std=gnu99
CC = gcc

all:
	make clean && make server.out

server.out: multi_server_pre_create_thread.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	-rm -rf server.out
