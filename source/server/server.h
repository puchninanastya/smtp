#ifndef SERVER_H
#define SERVER_H

int server_initialize();
int server_run();

struct server {
	int socket_fd_max;
};

#endif // SERVER_H