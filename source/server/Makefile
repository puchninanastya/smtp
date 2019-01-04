CC=gcc

STD= -std=gnu99

RFLAGS= -Wall -O

DFLAGS= -Wall -g 

all: clean server test_client

server: 
	$(CC) $(STD) $(RFLAGS) server.c my_socket.c linked_list.c error_fail.c main.c -o server

test_client:
	$(CC) $(STD) $(RFLAGS) test_client.c error_fail.c -o test_client

clean: 
	rm -rf *.o server test_client
