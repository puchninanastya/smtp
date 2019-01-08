#ifndef SERVER_H
#define SERVER_H

#include <sys/select.h>

#include "my_socket.h"
#include "linked_list.h"

int server_initialize();
void server_update_fd_sets();
int server_run();
void handle_new_connection();
int handle_received_message( int client_fd );
int send_message_to_client( int client_fd );
void close_client_connection( int client_fd );
void server_close();

struct server {
	int server_socket_fd;
	int break_main_loop;

	node *client_sockets_fds;

	fd_set* read_fds_set;
	fd_set* write_fds_set;
	fd_set* exceptions_fds_set;

	int max_fd;
};

#endif // SERVER_H