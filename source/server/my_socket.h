#ifndef SOCKET_H
#define SOCKET_H

int create_socket_on_port( int port );

struct fd_linked_list {
    int fd;
    struct fd_linked_list *next;
};

void fd_linked_list_add_fd( struct fd_linked_list *fd, int new_fd );
void fd_linked_list_delete_fd( struct fd_linked_list *fd, int fd_to_delete );

#endif // SOCKET_H