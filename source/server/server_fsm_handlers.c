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
    fcntl(client_fd, F_SETFL, O_NONBLOCK);
    if ( my_server.max_fd >= my_server.clients_size ) {
        printf( "Reallocing clients array.\n" );
        my_server.clients = realloc( my_server.clients,
                ( my_server.max_fd / CLIENTS_REALLOC_STEP + 1) * CLIENTS_REALLOC_STEP * sizeof( client_info* ) );
        my_server.clients_size += CLIENTS_REALLOC_STEP;
    }
    my_server.clients[ client_fd ] = malloc( sizeof( client_info ) );
    memset( my_server.clients[ client_fd ], 0, sizeof( client_info ) );
    my_server.clients[ client_fd ]->smtp_state = SMTP_SERVER_ST_READY;
    printf( "New client current smtp state: %d\n", my_server.clients[ client_fd ]->smtp_state );
    return nextState;
}

int HANDLE_CMND_HELO( int client_fd, te_smtp_server_state nextState )
{
    return 0;
}

int HANDLE_CMND_EHLO( int client_fd, te_smtp_server_state nextState )
{
    return 0;
}

int HANDLE_CMND_MAIL( int client_fd, te_smtp_server_state nextState )
{
    return 0;
}

int HANDLE_CMND_RCPT( int client_fd, te_smtp_server_state nextState )
{
    return 0;
}

int HANDLE_CMND_DATA( int client_fd, te_smtp_server_state nextState )
{
    return 0;
}

int HANDLE_MAIL_DATA( int client_fd, te_smtp_server_state nextState )
{
    return 0;
}

int HANDLE_MAIL_SAVED( int client_fd, te_smtp_server_state nextState )
{
    return 0;
}

int HANDLE_MAIL_END( int client_fd, te_smtp_server_state nextState )
{
    return 0;
}

int HANDLE_CMND_RSET( int client_fd, te_smtp_server_state nextState )
{
    return 0;
}

int HANDLE_CLOSE( int client_fd, te_smtp_server_state nextState )
{
    return 0;
}
