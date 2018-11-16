#ifndef SERVER_H
#define SERVER_H

#include "my_socket.h"

int server_initialize();
int server_run();

struct server {
	int server_socket_fd;
};

#endif // SERVER_H