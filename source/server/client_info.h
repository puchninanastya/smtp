#ifndef SMTP_MTA_CLIENT_INFO_H
#define SMTP_MTA_CLIENT_INFO_H

#include "autogen/server-fsm.h"
#include "mail.h"

typedef struct client_info client_info;
struct client_info {
    te_smtp_server_state smtp_state;
    int socket_fd;
    char* buffer
    mail* mail;
};

void reset_client_info( int client_fd );
void free_client_info( int client_fd );

#endif //SMTP_MTA_CLIENT_INFO_H
