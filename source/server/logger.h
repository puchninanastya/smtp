#ifndef SMTP_MTA_LOGGER_H
#define SMTP_MTA_LOGGER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "config.h"

#define LOG_MSG_EXIT "exit"

typedef enum {
    LOG_MSG_TYPE_UNDEF = 0,
    LOG_MSG_TYPE_DEBUG = 1,
    LOG_MSG_TYPE_INFO = 2,
    LOG_MSG_TYPE_ERROR = 3,
    LOG_MSG_TYPE_DEFAULT = LOG_MSG_TYPE_DEBUG
} log_msg_type_t;

typedef enum {
    LOG_MSG_LVL_DEBUG = 0,
    LOG_MSG_LVL_INFO = 1,
    LOG_MSG_LVL_ERROR = 2
} log_msg_level_t;

typedef struct log_msg_buffer {
    long msg_type;
    char msg_text[ LOGGER_MSG_CAPACITY ];
} log_msg_t;

typedef struct {
    log_msg_level_t log_level;
    /* log file */
    const char* filename;
    FILE* file;
    /* Sys Msg Queue data for log messages */
    key_t msg_queue_key;
    int msg_queue_id;
} logger_t;

int logger_fork_and_initialize();
int logger_log_msg( logger_t* logger, log_msg_type_t msg_type , char* msg );

int logger_initialize( logger_t* logger );
void logger_run_loop( logger_t* logger );
void logger_destroy( logger_t* logger );

#endif //SMTP_MTA_LOGGER_H
