#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>

#include "server.h"
#include "my_socket.h"
#include "config.h"
#include "error_fail.h"

#include "autogen/server-fsm.h"

extern struct server my_server;

int server_initialize() 
{
    printf( "Initializing server...\n" );
    my_server.server_socket_fd = create_socket_on_port( SERVER_PORT );
    my_server.break_main_loop = 0;
    my_server.clients = NULL;
    my_server.clients_size = 0;
    my_server.client_sockets_fds = NULL;
    my_server.read_fds_set  = ( fd_set* ) malloc( sizeof ( fd_set) );
    my_server.write_fds_set = ( fd_set* ) malloc( sizeof ( fd_set ) );
    my_server.exceptions_fds_set = ( fd_set* ) malloc( sizeof ( fd_set ) );
    printf( "Server successfully initialized.\n" );
    return 0;   
}

void server_update_fd_sets()
{
    // TODO: check do we need to set write for server socket
    printf( "Updating fd sets for select...\n" );

    /* Adding server socket */
    FD_ZERO( my_server.read_fds_set );
    FD_SET( my_server.server_socket_fd, my_server.read_fds_set );

    my_server.max_fd = my_server.server_socket_fd;

    /* Adding clients sockets if exist */
    node* current_client = my_server.client_sockets_fds;
    while ( current_client != NULL ) {
        printf( "Adding client to fd set.\n" );
        FD_SET( current_client->data, my_server.read_fds_set );
        if ( current_client->data > my_server.max_fd ) {
            my_server.max_fd = current_client->data;
        }
        current_client = current_client->next;
    }

    FD_ZERO( my_server.write_fds_set );

    FD_ZERO( my_server.exceptions_fds_set );
    FD_SET( my_server.server_socket_fd, my_server.exceptions_fds_set );

    printf("Fd sets successfully updated.\n");
    return;   
}

int server_run() 
{
    /* Main server loop */
    while ( !my_server.break_main_loop ) {

        server_update_fd_sets();

        printf( "Waiting for pselect activity...\n" );
        int activity = pselect( my_server.max_fd + 1, my_server.read_fds_set, my_server.write_fds_set,
                            my_server.exceptions_fds_set, NULL, NULL );
        
        printf( "PSelect woke up with activity: %d\n", activity );

        switch ( activity ) {
        case -1: 
            fail_on_error( "Select returns -1." );
            break;
        case 0:
            fail_on_error( "Select returns 0." );
            break;     
        default:
            /* All select fd sets should be checked. */

            /* Checking server socket. */
            if ( FD_ISSET( my_server.server_socket_fd, my_server.read_fds_set ) ) {
                handle_new_connection();
            }

            if ( FD_ISSET( my_server.server_socket_fd, my_server.exceptions_fds_set ) ) {
                fail_on_error( "Exception listen socket fd." );
            }

            /* Checking clients sockets. */
            node* current_client = my_server.client_sockets_fds;
            while ( current_client != NULL ) {

                if ( FD_ISSET( current_client->data, my_server.read_fds_set ) ) {
                    handle_received_message( current_client->data );
                } else if ( FD_ISSET( current_client->data, my_server.write_fds_set ) ) {
                    printf( "ATTENTION! No handler for send message!!!\n" );
                    //handle_send_message( current_client->data );
                } else if ( FD_ISSET( current_client->data, my_server.exceptions_fds_set ) ) {
                    printf( "Exception in client with fd %i.\n", current_client->data );
                    close_client_connection( current_client->data );
                }

                current_client = current_client->next;
            }

            break;
        }
    }

    return 0;
}

void handle_new_connection() 
{           
    printf( "Trying to accept new connection...\n" );
    int client_socket_fd = accept( my_server.server_socket_fd, NULL, 0 );
    if ( client_socket_fd < 0 ) {
        fail_on_error( "Can not accept client!" );
    }
    smtp_server_step( SMTP_SERVER_ST_INIT, SMTP_SERVER_EV_CONN_ACCEPTED,
                      client_socket_fd );
    my_server.client_sockets_fds = linked_list_add_node( my_server.client_sockets_fds, client_socket_fd );
    printf( "Client accepted and client socket added to clients array.\n" );
}

int handle_received_message( int client_fd )
{
    printf( "Trying to read message from client with fd %d...\n", client_fd );
    char buffer[BUFFER_SIZE];
    ssize_t actual_received = read( client_fd, buffer, BUFFER_SIZE );
    if ( actual_received < 0 ) {
        fail_on_error( "Can not read data from client!" );
    } else if ( actual_received == 0 ) {
        close_client_connection( client_fd );
    } else {
        printf( "Message \"%s\" received from client.\n", buffer );
        send_message_to_client( client_fd );
    }
    return 0;
}

int send_message_to_client( int client_fd )
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

void close_client_connection( int client_fd )
{
    printf( "Trying to close client connection...\n" );
    close( client_fd );
    my_server.client_sockets_fds = linked_list_delete_node( my_server.client_sockets_fds, 
        client_fd );
    printf( "Client socket is closed.\n" );
}

void server_close() 
{
    close( my_server.server_socket_fd );
    printf( "Server socket is closed.\n" );
}