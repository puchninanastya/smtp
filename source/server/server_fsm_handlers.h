#ifndef SMTP_MTA_SERVER_FSM_HANDLERS_H
#define SMTP_MTA_SERVER_FSM_HANDLERS_H

#include "server-fsm.h"

int HANDLE_INIT_CONN_ACCEPTED();
int HANDLE_READY_CONN_ACCEPTED();
int HANDLE_NEED_SENDER_CONN_ACCEPTED();
int HANDLE_NEED_RECIPIENT_CONN_ACCEPTED();
int HANDLE_NEED_DATA_CONN_ACCEPTED();
int HANDLE_WAITING_FOR_DATA_CONN_ACCEPTED();
int HANDLE_SAVING_MAIL_CONN_ACCEPTED();
int HANDLE_CLOSED_CONN_ACCEPTED();

int HANDLE_READY_CMND_NOOP();
int HANDLE_READY_CMND_HELO();
int HANDLE_READY_CMND_EHLO();
int HANDLE_READY_CMND_QUIT();

int HANDLE_NEED_SENDER_CMND_NOOP();
int HANDLE_NEED_SENDER_CMND_MAIL();
int HANDLE_NEED_SENDER_CMND_RSET();
int HANDLE_NEED_SENDER_CMND_QUIT();

int HANDLE_NEED_RECIPIENT_CMND_NOOP();
int HANDLE_NEED_RECIPIENT_CMND_RCPT();
int HANDLE_NEED_RECIPIENT_CMND_RSET();
int HANDLE_NEED_RECIPIENT_CMND_QUIT();

int HANDLE_NEED_DATA_CMND_NOOP();
int HANDLE_NEED_DATA_CMND_RCPT();
int HANDLE_NEED_DATA_CMND_DATA();
int HANDLE_NEED_DATA_CMND_RSET();
int HANDLE_NEED_DATA_CMND_QUIT();

int HANDLE_WAITING_FOR_DATA_MAIL_END();

#endif //SMTP_MTA_SERVER_FSM_HANDLERS_H
