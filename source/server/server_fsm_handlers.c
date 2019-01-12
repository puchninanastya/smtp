#include "server_fsm_handlers.h"

extern struct server my_server;

int HANDLE_ACCEPTED() {
    te_smtp_server_state current_client_state = SMTP_SERVER_ST_INIT;
    printf( "Current server state: %d\n", current_server_state );
    te_smtp_server_state next_server_state = smtp_server_step( current_server_state, SMTP_SERVER_EV_CONN_ACCEPTED );

    printf( "New server state: %d\n", next_server_state );
    return SMTP_SERVER_ST_READY;
}

int HANDLE_CMND_HELO() {
    return 0;
}

int HANDLE_CMND_EHLO() {
    return 0;
}

int HANDLE_CMND_MAIL() {
    return 0;
}

int HANDLE_CMND_RCPT() {
    return 0;
}

int HANDLE_CMND_DATA() {
    return 0;
}

int HANDLE_MAIL_DATA() {
    return 0;
}

int HANDLE_MAIL_SAVED() {
    return 0;
}

int HANDLE_MAIL_END() {
    return 0;
}

int HANDLE_CMND_RSET() {
    return 0;
}

int HANDLE_CLOSE() {
    return 0;
}
