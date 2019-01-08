# Main program name
PROG = server

# Report name
REPORT = report.pdf

# Compiler
CC = gcc
STD = -std=gnu99

# Directories
INCLUDEDIR = ./../common/include
SOURCEDIR = ./../common/source
BUILDDIR = build

# Compilation Flags
RFLAGS = -Wall -Werror -I$(INCLUDEDIR) -O
DFLAGS = -Wall -I$(INCLUDEDIR) -g

# Generate check options files with autogen
./checkoptn.c: ./checkoptn.def
	SHELL=/bin/sh autogen checkoptn.def
./checkoptn.h:  ./checkoptn.def
	SHELL=/bin/sh autogen checkoptn.def

# Generate Finite State Machine files with autogen
./server-fsm.c: server.def
	autogen server.def
./server-fsm.h: server.def
	autogen server.def

# Targets
all: clean build_dir server test_client

all-debug: clean build_dir server-debug test_client-debug

server:
	$(CC) $(STD) $(RFLAGS) -o ./$(BUILDDIR)/server \
    server.c my_socket.c main.c $(SOURCEDIR)/*.c

test_client:
	$(CC) $(STD) $(RFLAGS) -o ./$(BUILDDIR)/test_client \
	test_client.c $(SOURCEDIR)/*.c

server-debug:
	$(CC) $(STD) $(DFLAGS) -o ./$(BUILDDIR)/server \
    server.c my_socket.c main.c $(SOURCEDIR)/*.c

test_client-debug:
	$(CC) $(STD) $(DFLAGS) -o ./$(BUILDDIR)/test_client \
	test_client.c $(SOURCEDIR)/*.c

clean: 
	rm -rf *.o ./build

build_dir:
	mkdir -p $(BUILDDIR)

