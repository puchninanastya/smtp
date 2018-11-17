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
    server_initialize_fd_sets();
    if ( listen( my_server.server_socket_fd, 1 ) < 0 ) {
        fail_on_error( "Can not start listening server socket!" );
    }
    printf("Server successfully initialized.\n");
    return 0;   
}

void server_update_fd_sets() 
{
    // TODO: check do we need to set write for server socket
    printf("Building fd sets for select...\n");
    
    FD_ZERO( &my_server.read_fds_set );
    FD_SET( my_server.server_socket_fd, &( my_server.read_fds_set ) );

    FD_ZERO( &my_server.write_fds_set );

    FD_ZERO( &my_server.exceptions_fds_set );

    printf("Fd sets successfully built.\n");
    return 0;   
}

int server_run() 
{
    /* Main server loop */
    while ( !my_server.break_main_loop ) {

        server_update_fd_sets();

        printf( "Waiting for select activity...\n" );
        int activity = select(1, &my_server.read_fds_set, &my_server.write_fds_set, 
                            &my_server.exceptions_fds_set, NULL);

        switch ( activity ) {
        case -1: 
            fail_on_error( "Select returns -1." );
            break;
        case 0:
            fail_on_error( "Select returns 0." );
            break;     
        default:
            /* All set fds should be checked. */

            if ( FD_ISSET( my_server.server_socket_fd, &my_server.read_fds_set ) ) {
                server_handle_new_connection();
            }

            if ( FD_ISSET( my_server.server_socket_fd, &my_server.exceptions_fds_set ) ) {
                fail_on_error( "Exception listen socket fd." );
            }

            // TODO: need to check work with node of list (need pointer)
            for (struct fd_linked_list current_client = my_server.client_sockets_fds; 
                current_client.fd; 
                current_client.next ) {

                if ( FD_ISSET( current_client.fd, &my_server.read_fds_set ) ) {
                    handle_received_message( current_client.fd );
                }
        
                if ( FD_ISSET( current_client.fd, &my_server.write_fds_set ) ) {
                    handle_send_message( current_client.fd );
                }

                if ( FD_ISSET( current_client.fd, &my_server.exceptions_fds_set ) ) {
                    printf( "Exception in client with fd %i.\n", current_client.fd );
                    server_close_client_connection( &current_client );
                }
            }

            break;
        }
    }

    return 0;
}

void server_handle_new_connection() 
{
    printf( "Trying to accept new connection..." );
    int client_socket_fd = accept( my_server.server_socket_fd, NULL, 0 );
    if ( client_socket_fd < 0 ) {
        fail_on_error( "Can not accept client!" );
    }
    fd_linked_list_add_fd( &my_server.client_sockets_fds, client_socket_fd );
    printf( "Client accepted client socket added to list." );
}

int handle_received_message( int client_fd )
{
    printf( "Trying to read message from client with fd %d...\n", client_fd );
    char buffer[BUFFER_SIZE];
    ssize_t actual_received = read( client_fd, buffer, BUFFER_SIZE );
    if ( actual_received < 0 ) {
        fail_on_error( "Can not read data from client!" );
    }
    printf( "Message \"%s\" received from client.\n", buffer );
    return 0;
}

int handle_send_message( int client_fd )
{
    printf( "Trying to send message to client with fd %d...\n", client_fd );
    const char* message_to_send = "Echo hello from server!";
    ssize_t actual_sent = send( client_fd, message_to_send, strlen(message_to_send), 0 ); 
    if ( actual_sent < 0 ) {
        fail_on_error( "Can not sent data to client!" );
    }
    printf( "Message \"%s\" sent to client.\n", message_to_send );
    return 0;
}

void server_close_client_connection( struct fd_linked_list *client ) 
{
    printf( "Trying to close client connection..." );
    close( client->fd );
    fd_linked_list_delete_fd( &my_server.client_sockets_fds, client->fd );
    printf( "Client socket closed.\n" );
}

void server_close() 
{
    close( my_server.server_socket_fd );
    printf( "Server socket closed.\n" );
}