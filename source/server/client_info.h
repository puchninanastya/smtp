#ifndef SMTP_MTA_CLIENT_INFO_H
#define SMTP_MTA_CLIENT_INFO_H

#include "autogen/server-fsm.h"
#include "mail.h"

typedef struct client_info client_info;
struct client_info {
    te_smtp_server_state smtp_state;
    int socket_fd;
    char* buffer;
    int buffer_read_offset;
    int buffer_data_size;
    int mail_header_received;
    mail* mail;
};

void reset_client_info( int client_fd );
void free_client_info( int client_fd );

#endif //SMTP_MTA_CLIENT_INFO_H
