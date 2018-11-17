#ifndef SERVER_H
#define SERVER_H

#include "my_socket.h"

int server_initialize();
int server_run();
void server_close();

struct server {
	int server_socket_fd;
	int break_main_loop;
};

#endif // SERVER_H