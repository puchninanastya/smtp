#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>

#include "rexprs.h"
#include "server_fsm_handlers.h"
#include "server.h"
#include "config.h"
#include "error_fail.h"
#include "helpers.h"
#include "network.h"

extern struct server my_server;

int send_response_to_client(int client_fd, const char *response)
{
    printf( "Trying to send message to client with fd %d...\n", client_fd );
    ssize_t actual_sent = send( client_fd, response, strlen( response ), 0 );

    // TODO: проверить, что он реально неблокирующий
    if ( actual_sent < 0 && errno == EWOULDBLOCK ) {
            printf( "Error while sending message (EWouldblock), continue..\n" );
            return 1;
    }

    printf( "Actual sent size: %zd\n", actual_sent );
    printf( "Message \"%s\" sent to client.\n", response );
    return 0;
}

int HANDLE_CMND_NOOP( int client_fd, te_smtp_server_state nextState ) {
    printf( "Handling command NOOP...\n" );
    send_response_to_client( client_fd, RE_RESP_OK );
    printf( "Handling command NOOP finished.\n" );
    return nextState;
}

int HANDLE_ACCEPTED( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handling accepted.\n" );

    if ( set_socket_as_nonblocking( client_fd ) ) {
        printf("ERROR! Fail to set flag 'O_NONBLOCK' for socket.\n");
    } else {
        printf("Client's socket set as nonblocking.\n");
    }

    // realloc array of clients
    if ( my_server.max_fd >= my_server.clients_size ) {
        printf( "Reallocing clients array.\n" );
        my_server.clients = realloc( my_server.clients,
                ( my_server.max_fd / CLIENTS_REALLOC_STEP + 1) * CLIENTS_REALLOC_STEP
                * sizeof( client_info* ) );
        my_server.clients_size += CLIENTS_REALLOC_STEP;
    }

    // initialize client_info struct for this client_fd
    client_info* client = malloc( sizeof( client_info ) );
    memset( client, 0, sizeof( client_info ) );
    client->buffer = malloc( BUFFER_SIZE );
    memset( client->buffer, 0, BUFFER_SIZE );
    client->buffer_data_size = 0;
    client->smtp_state = SMTP_SERVER_ST_READY;
    client->mail = NULL;

    // and add client to clients[]
    my_server.clients[ client_fd ] = client;

    send_response_to_client( client_fd, RE_RESP_READY );

    printf( "New client current smtp state: %d\n", my_server.clients[ client_fd ]->smtp_state );

    printf( "Handling accepted finished.\n" );
    return nextState;
}

int HANDLE_CMND_HELO( int client_fd, char*** matchdata, int matchdatalen, te_smtp_server_state nextState )
{
    printf( "Handling command HELO...\n" );
    // client_info* client = my_server.clients[ client_fd ];

    // TODO: add DNS checking

    /* compare command data address and real client ip address */

    char* host = NULL;
    if ( matchdatalen == 1 ) {
        host = ( *matchdata )[ matchdatalen - 1 ];
    }
    printf( "Debug: Host: %s\n", host );

    char* host_ip = get_socket_ip_address( client_fd );
    printf( "Debug: Peer's IP address is: %s\n", host_ip );

    if ( strcmp( host, host_ip ) == 0 ) {
        printf( "Client's (%d) address is verified.\r\n", client_fd );
    } else {
        // it doesn't matter
        printf( "Client's (%d) address is not verified!\r\n", client_fd );
    }

    send_response_to_client( client_fd, RE_RESP_OK );

    printf( "Handling command HELO finished.\n" );
    return nextState;
}

int HANDLE_CMND_EHLO( int client_fd, char*** matchdata, int matchdatalen, te_smtp_server_state nextState )
{
    printf( "Handle command EHLO.\n" );
    // TODO: add DNS checking

    /* compare command data address and real client ip address */

    char* host = NULL;
    if ( matchdatalen == 1 ) {
        host = ( *matchdata )[ matchdatalen - 1 ];
    }
    printf( "Debug: Host: %s\n", host );

    char* host_ip = get_socket_ip_address( client_fd );
    printf( "Debug: Peer's IP address is: %s\n", host_ip );

    if ( strcmp( host, host_ip ) == 0 ) {
        printf( "Client's (%d) address is verified.\r\n", client_fd );
    } else {
        // it doesn't matter
        printf( "Client's (%d) address is not verified!\r\n", client_fd );
    }

    // TODO: to add supported smtp commands to response?
    send_response_to_client( client_fd, RE_RESP_OK );

    printf( "Handling command EHLO finished.\n" );
    return nextState;
}

int HANDLE_CMND_MAIL( int client_fd, char*** matchdata, int matchdatalen, te_smtp_server_state nextState )
{
    printf( "Handling command MAIL...\n" );
    client_info* client = my_server.clients[ client_fd ];

    char* email_address = NULL;
    if ( matchdatalen == 1 && ( strcmp(( *matchdata )[ matchdatalen - 1 ], "") != 0 ) ) {
        email_address = ( *matchdata )[ matchdatalen - 1 ];
        printf( "Debug: 'Mail from' email address: %s.\n", email_address );
    } else {
        printf( "Debug: 'Mail from' without email address.\n" );
    }

    // adding sender address to client's mail
    client->mail = malloc(sizeof( mail ) );
    client->mail->sender = email_address;

    send_response_to_client( client_fd, RE_RESP_OK );

    printf( "Handling command MAIL finished.\n" );
    return nextState;
}

int HANDLE_CMND_RCPT( int client_fd, char*** matchdata, int matchdatalen, te_smtp_server_state nextState )
{
    printf( "Handling command RCPT...\n" );
    client_info* client = my_server.clients[ client_fd ];

    char* email_address = NULL;
    if ( matchdatalen == 1 && ( strcmp(( *matchdata )[ matchdatalen - 1 ], "") != 0 ) ) {
        email_address = ( *matchdata )[ matchdatalen - 1 ];
        printf( "Debug: 'Rcpt to' email address: %s.\n", email_address );
    } else {
        printf( "Debug: 'Rcpt to' without email address.\n" );
    }

    if ( client->mail->recepients_num++ > MAX_RCPT_CLIENTS ) {
        printf( "Client's mail already has max number of recepients! Can't add one more.\r\n" );
        send_response_to_client(client_fd, RE_RESP_OK);
    } else {
        client->mail->recepients = malloc( sizeof( char* ) * MAX_RCPT_CLIENTS ); // TODO: change allocation (add realloc)
        client->mail->recepients[ client->mail->recepients_num ] = email_address;
        client->mail->recepients_num++;
        send_response_to_client(client_fd, RE_RESP_OK);
    }

    printf( "Handling command RCPT finished.\n" );
    return nextState;
}

int HANDLE_CMND_DATA( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handling command DATA...\n" );
    client_info* client = my_server.clients[ client_fd ];

    // initializing a little buffer for mail data
    client->mail->data = malloc( sizeof( char ) );
    client->mail->data[ 0 ] = '\0';
    client->mail->data_capacity = 0;

    send_response_to_client( client_fd, RE_RESP_START_MAIL );

    printf( "Handling command DATA finished.\n" );
    return nextState;
}

int HANDLE_MAIL_DATA( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handling mail data...\n" );
    client_info* client = my_server.clients[ client_fd ];

    // TODO: check if two dots - delete one? (rfc 821)

    append_data_to_mail( client->mail, client->buffer, strlen( client->buffer ) );

    printf( "Handling mail data finished.\n" );
    return nextState;
}

int HANDLE_MAIL_END( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handling end of mail data...\n" );
    client_info* client = my_server.clients[ client_fd ];

    send_response_to_client( client_fd, RE_RESP_OK );
    printf( "Full client's mail data looks like: \n %s\n", client->mail->data );

    printf( "Handling end of mail data finished.\n" );
    return nextState;
}

int HANDLE_MAIL_SAVED( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle mail saved.\n" );
    send_response_to_client( client_fd, RE_RESP_OK );
    return nextState;
}

int HANDLE_CMND_RSET( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handle command RSET.\n" );
    reset_client_info( client_fd );
    send_response_to_client( client_fd, RE_RESP_OK );
    printf( "Handling command RSET finished.\n" );
    return nextState;
}

int HANDLE_CLOSE( int client_fd, te_smtp_server_state nextState )
{
    printf( "Handling close...\n" );
    send_response_to_client( client_fd, RE_RESP_CLOSE );
    printf( "Handling close finished.\n" );
    return nextState;
}

int HANDLE_ERROR( int client_fd, te_smtp_server_state nextState ) {
    printf( "Handling error...\n" );
    send_response_to_client( client_fd, RE_RESP_ERR_BAD_SEQ );
    printf( "Handling error finished.\n" );
    return nextState;
}
