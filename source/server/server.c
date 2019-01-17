#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>

#include "server.h"
#include "my_socket.h"
#include "re_parser.h"
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
    my_server.read_fds_set  = ( fd_set* ) malloc( sizeof ( fd_set ) );
    my_server.write_fds_set = ( fd_set* ) malloc( sizeof ( fd_set ) );
    my_server.exceptions_fds_set = ( fd_set* ) malloc( sizeof ( fd_set ) );
    if ( re_initialize() == 0 ) {
        fail_on_error( "Can not initialize regular expressions for parser!" );
    }
    printf( "Server successfully initialized.\n" );
    return 0;   
}

void server_update_fd_sets()
{
    printf( "Updating fd sets for select...\n" );

    /* Reading server's fd set */

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

    /* Writing server's fd set */
    FD_ZERO( my_server.write_fds_set );


    /* Expections server's fd sets */
    FD_ZERO( my_server.exceptions_fds_set );
    FD_SET( my_server.server_socket_fd, my_server.exceptions_fds_set );

    printf( "Fd sets successfully updated.\n" );
}

int server_run() 
{
    /* Main server loop */
    while ( !my_server.break_main_loop ) {

        server_update_fd_sets();

        printf( "Waiting for pselect activity...\n" );
        int activity = pselect( my_server.max_fd + 1, my_server.read_fds_set,
                my_server.write_fds_set, my_server.exceptions_fds_set, NULL, NULL );
        
        printf( "Pselect woke up with activity: %d\n", activity );

        switch ( activity ) {
        case -1: 
            fail_on_error( "Select returns -1." ); // NOTE: remove this for IDE debug
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
                    handle_client_read( current_client->data );
                } else if ( FD_ISSET( current_client->data, my_server.write_fds_set ) ) {
                    printf( "ATTENTION! No handler for send message!!!\n" ); // TODO: add handler
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
                      client_socket_fd, NULL, 0 );
    my_server.client_sockets_fds = linked_list_add_node( my_server.client_sockets_fds,
            client_socket_fd );
    printf( "Client accepted and client socket added to clients array.\n" );
}

int handle_client_read(int client_fd)
{
    printf( "Trying to read message from client with fd %d...\n", client_fd );

    client_info* client = my_server.clients[ client_fd ];

    char buffer[BUFFER_SIZE];
    memset( buffer, 0, BUFFER_SIZE );

    ssize_t actual_received = recv( client_fd, buffer, BUFFER_SIZE, 0 );

    if ( actual_received < 0 ) {
        fail_on_error( "Can not read data from client!" );
    } else if ( actual_received == 0 ) {
        close_client_connection( client_fd );
    } else {
        printf( "Message \"%s\" received from client, message lenght: %zd.\n",
                buffer, actual_received );
        memcpy( client->buffer, buffer, actual_received );

        // parse for command and send response
        char** matchdata = 0;
        int matchdatalen = 0;
        smtp_re_commands cmnd = re_match_for_command( client->buffer, &matchdata, &matchdatalen );
        printf( "Re match for command result cmnd: %d\n", cmnd );
        printf( "Re match data len: %d\n", matchdatalen );
        for( int i = 0; i < matchdatalen; i++ ) {
            printf( "Re match data num %d: %s\n", i, matchdata[ i ] );
        }
        te_smtp_server_state next_st = smtp_server_step( client->smtp_state,
                ( te_smtp_server_event ) cmnd, client_fd, &matchdata, matchdatalen );
        client->smtp_state = next_st;
        printf( "New current state for client %d is %d.\n", client_fd, client->smtp_state );
    }

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
    re_finalize();
    printf( "Server is closed.\n" );
}