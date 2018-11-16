#ifndef SOCKET_H
#define SOCKET_H

int create_socket_on_port( int port );

struct my_socket {
    int socket_desc;
};

#endif // SOCKET_H