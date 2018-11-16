CC=gcc

STD= -std=gnu99

RFLAGS= -Wall -O

DFLAGS= -Wall -g 

all: clean client

client: 
	$(CC) $(STD) $(RFLAGS) my_socket.c client.c main.c -o client

client: 
	rm -rf *.o client
