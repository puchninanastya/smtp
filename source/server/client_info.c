#include <stdlib.h>

#include "client_info.h"
#include "server.h"

extern struct server my_server;

void reset_client_info( int client_fd ) {
    client_info* client = my_server.clients[ client_fd ];
    client->buffer_data_size = 0;
    client->buffer_read_offset = 0;
    client->mail_header_received = 0;
    free_mail( client->mail );
    client->mail = NULL;
}

void free_client_info( int client_fd ) {
    client_info* client = my_server.clients[ client_fd ];
    if ( client == NULL ) {
        return;
    }
    if ( client->buffer != NULL ) {
        free(client->buffer);
        client->buffer = NULL;
    }
    if ( client->mail != NULL ) {
        free_mail( client->mail );
        client->mail = NULL;
    }
    free( client );
    my_server.clients[ client_fd ] = NULL;
}