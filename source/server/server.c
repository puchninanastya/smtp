#include <stdio.h>
#include <sys/socket.h> 
#include <unistd.h>
#include <string.h>

#include "server.h"
#include "my_socket.h"
#include "config.h"
#include "error_fail.h"

extern struct server my_server;

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

    printf( "Waiting for the client...\n" );
    while ( !my_server.break_main_loop ) {
        int client_socket_fd = accept( my_server.server_socket_fd, NULL, 0 );
        if ( client_socket_fd < 0 ) {
            fail_on_error( "Can not accept client!" );
        }
        printf( "Client accepted.\n" );

        char buffer[1024];
        ssize_t actual_received = read( client_socket_fd, buffer, 1024 );
        if ( actual_received < 0 ) {
            fail_on_error( "Can not read data from client!" );
        }
        printf( "Message \"%s\" received from client.\n", buffer );

        const char* message_to_send = "Echo hello from server!";
        ssize_t actual_sent = send( client_socket_fd, message_to_send, strlen(message_to_send), 0 ); 
        if ( actual_sent < 0 ) {
            fail_on_error( "Can not sent data to client!" );
        }
        printf( "Message \"%s\" sent to client.\n", message_to_send );

        close(client_socket_fd);

        my_server.break_main_loop = 1;
    }

    return 0;
}

void server_close() 
{
    close(my_server.server_socket_fd);
    printf("Server socket closed.\n");
}