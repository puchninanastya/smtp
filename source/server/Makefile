CC=gcc

STD= -std=gnu99

RFLAGS= -Wall -Werror -O

DFLAGS= -Wall -g

INCLUDE_COMMON_FLAGS = -I./../common/include

INCLUDE_COMMON_SOURCE= ./../common/source/*.c

BUILD_DIR= build

all: clean build_dir server test_client

all-debug: clean build_dir server-debug test_client-debug

server:
	$(CC) $(STD) $(RFLAGS) $(INCLUDE_COMMON_FLAGS) -o ./$(BUILD_DIR)/server \
    server.c my_socket.c main.c $(INCLUDE_COMMON_SOURCE)

test_client:
	$(CC) $(STD) $(RFLAGS) $(INCLUDE_COMMON_FLAGS) -o ./$(BUILD_DIR)/test_client \
	test_client.c $(INCLUDE_COMMON_SOURCE)

server-debug:
	$(CC) $(STD) $(DFLAGS) $(INCLUDE_COMMON_FLAGS) -o ./$(BUILD_DIR)/server \
    server.c my_socket.c main.c $(INCLUDE_COMMON_SOURCE)

test_client-debug:
	$(CC) $(STD) $(DFLAGS) $(INCLUDE_COMMON_FLAGS) -o ./$(BUILD_DIR)/test_client \
	test_client.c $(INCLUDE_COMMON_SOURCE)

clean: 
	rm -rf *.o ./build

build_dir:
	mkdir -p $(BUILD_DIR)
