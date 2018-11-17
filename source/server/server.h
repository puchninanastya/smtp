#ifndef SERVER_H
#define SERVER_H

#include <sys/select.h>

#include "my_socket.h"

int server_initialize();
int server_run();
void server_handle_new_connection();
void server_close();

struct server {
	int server_socket_fd;
	int break_main_loop;

	struct fd_linked_list client_sockets_fds;

	fd_set read_fds_set;
	fd_set write_fds_set;
	fd_set exceptions_fds_set;
};

#endif // SERVER_H