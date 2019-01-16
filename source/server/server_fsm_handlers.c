#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "rexprs.h"
#include "server_fsm_handlers.h"
#include "server.h"
#include "config.h"
#include "error_fail.h"

extern struct server my_server;

int send_message_to_client( int client_fd, const char* response )
{
    printf( "Trying to send message to client with fd %d...\n", client_fd );
    //const char* message_to_send = "OK!\r\n";
    ssize_t actual_sent = send( client_fd, response, strlen( response ), 0 );
    if ( actual_sent < 0 ) {
        fail_on_error( "Can not sent data to client!" );
    }
    printf( "Message \"%s\" sent to client.\n", response );
    return 0;
}

int HANDLE_ACCEPTED( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle accepted.\n" );
    fcntl( client_fd, F_SETFL, O_NONBLOCK );
    if ( my_server.max_fd >= my_server.clients_size ) {
        printf( "Reallocing clients array.\n" );
        my_server.clients = realloc( my_server.clients,
                ( my_server.max_fd / CLIENTS_REALLOC_STEP + 1) * CLIENTS_REALLOC_STEP
                * sizeof( client_info* ) );
        my_server.clients_size += CLIENTS_REALLOC_STEP;
    }
    client_info* client = malloc( sizeof( client_info ) );
    memset( client, 0, sizeof( client_info ) );
    client->buffer = malloc( BUFFER_SIZE );
    memset( client->buffer, 0, BUFFER_SIZE );
    client->buffer_data_size = 0;
    client->smtp_state = SMTP_SERVER_ST_READY;
    my_server.clients[ client_fd ] = client;
    printf( "New client current smtp state: %d\n", my_server.clients[ client_fd ]->smtp_state );
    return nextState;
}

int HANDLE_CMND_HELO( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle command HELO.\n" );
    client_info* client = my_server.clients[ client_fd ];

    // TODO: add DNS

    /* find domain in command data */

    // cut off the end of line in command buffer
    char* eol = strstr( client->buffer, "\r\n");
    eol[0] = '\0';
    char* command_data = ( char* ) malloc( BUFFER_SIZE );
    strcpy(command_data, &client->buffer[5]);
    printf( "Debug: Command data copied: %s\n", command_data );

    // skip spaces and < to find start position
    int start = 0;
    for ( start = 0;
        command_data[ start ] == ' ' || command_data[ start ] == '<';
        start++ );

    int len = strlen( command_data ) - start - 1; // -1 is for '>'
    printf( "Debug: Command data real length: %d\n", len );

    char* host = malloc( len + 1 ); // TODO: check
    strncpy( host, &command_data[ start ], len );
    printf( "Debug: Host: %s\n", host );

    /* compare host from command data and real ip address */

    struct sockaddr_in peer;
    unsigned int peer_len = sizeof( peer );

    if ( getpeername ( client_fd, ( struct sockaddr* )( &peer ), &peer_len ) == -1) {
        fail_on_error( "getpeername() failed" );
    }

    char* host_ip = inet_ntoa( peer.sin_addr );
    printf("Peer's IP address is: %s\n", host_ip );
    printf("Peer's port is: %d\n", ( int ) ntohs( peer.sin_port ) );

    if ( strcmp( host, host_ip ) == 0 ) {
        printf( "Client's (%d) address is verified.\r\n", client_fd );
        send_message_to_client( client_fd, RE_RESP_OK );
    } else {
        printf( "Client's (%d) address is not verified!\r\n", client_fd );
        send_message_to_client( client_fd, RE_RESP_OK );
    }

    return nextState;
}

int HANDLE_CMND_EHLO( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle command EHLO.\n" );
    send_message_to_client( client_fd, RE_RESP_OK );
    return nextState;
}

int HANDLE_CMND_MAIL( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle command MAIL.\n" );
    send_message_to_client( client_fd, RE_RESP_OK );
    return nextState;
}

int HANDLE_CMND_RCPT( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle command RCPT.\n" );
    send_message_to_client( client_fd, RE_RESP_OK );
    return nextState;
}

int HANDLE_CMND_DATA( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle command DATA.\n" );
    send_message_to_client( client_fd, RE_RESP_OK );
    return nextState;
}

int HANDLE_MAIL_DATA( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle mail data.\n" );
    send_message_to_client( client_fd, RE_RESP_OK );
    return nextState;
}

int HANDLE_MAIL_SAVED( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle mail saved.\n" );
    send_message_to_client( client_fd, RE_RESP_OK );
    return nextState;
}

int HANDLE_MAIL_END( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle mail end.\n" );
    send_message_to_client( client_fd, RE_RESP_OK );
    return nextState;
}

int HANDLE_CMND_RSET( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle command RSET.\n" );
    send_message_to_client( client_fd, RE_RESP_OK );
    return nextState;
}

int HANDLE_CLOSE( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle close.\n" );
    send_message_to_client( client_fd, RE_RESP_OK );
    return nextState;
}
