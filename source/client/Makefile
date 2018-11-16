CC=gcc

STD= -std=gnu99

RFLAGS= -Wall -O

DFLAGS= -Wall -g 

all: clean server

server: 
	$(CC) $(STD) $(RFLAGS) my_socket.c server.c main.c -o server

clean: 
	rm -rf *.o server
