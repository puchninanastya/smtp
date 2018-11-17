#include "stdio.h"
#include <sys/socket.h> 
#include <unistd.h>

#include "server.h"
#include "my_socket.h"
#include "config.h"
#include "error_fail.h"

extern struct server my_server;

// TODO: change prints to logs

int server_initialize() 
{
    printf("Initializing server...\n");
    my_server.server_socket_fd = create_socket_on_port( SERVER_PORT );
    my_server.break_main_loop = 0;
    printf("Server successfully initialized.\n");
    return 0;   
}

int server_run() 
{
    printf("Starting server...\n");
    if ( listen( my_server.server_socket_fd, 1 ) < 0 ) {
        fail_on_error( "Can not start listening server socket!" );
    }
    printf("Server started.\n");

    printf("Waiting for the client...\n");
    while ( !my_server.break_main_loop ) {
        int clientSocket = accept(my_server.server_socket_fd, NULL, 0);
        if (clientSocket < 0) {
            fail_on_error( "Can not accept client!" );
        }
        printf("Client accepted.\n");

        char buffer[1024];
        ssize_t actual_received = read(clientSocket, buffer, 1024);
        if (actual_received < 0) {
            fail_on_error( "Can not read data from client!" );
        }
        printf(" started.\n");

        ssize_t actual_sent = write(clientSocket, buffer, actual_received);
        if (actual_sent < 0) {
            fail_on_error( "Cat not sent data to client!" );
        }
        printf("Server started.\n");

        my_server.break_main_loop = 1;
    }

    printf("Done\n");
    return 0;
}

void server_close() 
{
    close(my_server.server_socket_fd);
    printf("Server socket closed.\n");
}