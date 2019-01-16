#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "server_fsm_handlers.h"
#include "server.h"
#include "config.h"

extern struct server my_server;

int HANDLE_ACCEPTED( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle accepted.\n" );
    fcntl( client_fd, F_SETFL, O_NONBLOCK );
    if ( my_server.max_fd >= my_server.clients_size ) {
        printf( "Reallocing clients array.\n" );
        my_server.clients = realloc( my_server.clients,
                ( my_server.max_fd / CLIENTS_REALLOC_STEP + 1) * CLIENTS_REALLOC_STEP * sizeof( client_info* ) );
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
    return nextState;
}

int HANDLE_CMND_EHLO( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle command EHLO.\n" );
    return nextState;
}

int HANDLE_CMND_MAIL( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle command MAIL.\n" );
    return nextState;
}

int HANDLE_CMND_RCPT( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle command RCPT.\n" );
    return nextState;
}

int HANDLE_CMND_DATA( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle command DATA.\n" );
    return nextState;
}

int HANDLE_MAIL_DATA( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle mail data.\n" );
    return nextState;
}

int HANDLE_MAIL_SAVED( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle mail saved.\n" );
    return nextState;
}

int HANDLE_MAIL_END( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle mail end.\n" );
    return nextState;
}

int HANDLE_CMND_RSET( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle command RSET.\n" );
    return nextState;
}

int HANDLE_CLOSE( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle close.\n" );
    return nextState;
}
