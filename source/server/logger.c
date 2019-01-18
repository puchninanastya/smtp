#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "logger.h"
#include "error_fail.h"
#include "helpers.h"
#include "server.h"

extern struct server my_server;

int logger_fork_and_initialize()
{
    printf( "Logger: fork and init.\n" );
    pid_t pid;
    switch ( pid = fork() ) {
        case -1:
            printf( "ERROR! Logger fork() failed.\n" );
            return 0;
        case 0:  // logger
            printf("logger process forked with pid: %d\n", getpid());
            printf("parent pid: %d\n", getppid());

            // TODO: add handling of signals

            logger_t logger_listener;

            logger_initialize( &logger_listener );
            logger_run_loop( &logger_listener );
            logger_destroy( &logger_listener );

            kill( getpid(), SIGTERM );

        default: // server
            logger_initialize( &( my_server.logger ) );
            break;
    }
    return pid;
}

int logger_initialize( logger_t* logger )
{
    printf( "Logger: initializing...\n" );
    /* create Sys MQ for log messages */

    logger->msg_queue_key = 5;

    // TODO: fix. ftok don't work and returns < 0 =(
    //if ( ( logger->msg_queue_key = ftok( LOGGER_QUEUE_NAME, 1 ) ) < 0 ) {
    //    fail_on_error( "logger_initialize(): ftok()" );
    //}

    if ( ( logger->msg_queue_id = msgget( logger->msg_queue_key, 0666 | IPC_CREAT) ) < 0) {
        fail_on_error("logger_initialize(): msgget()");
    }

    // TODO: open log file

    printf( "Logger: initialized ok.\n" );
    return 0;
}

void logger_destroy( logger_t* logger )
{
    // destroy the message queue
    msgctl( logger->msg_queue_id, IPC_RMID, NULL);
}

void logger_run_loop( logger_t* logger )
{
    /* Sys MQ data for log msg */
    log_msg_t log_msg;
    ssize_t log_msg_sz;
    log_msg_sz = sizeof( log_msg.msg_text );

    /* Logger in loop until exit message will be received */
    while(1) {

        // msgrcv to receive message
        msgrcv( logger->msg_queue_id, &log_msg, log_msg_sz, 1, 0);

        // break loop if received LOG_MSG_EXIT
        if ( strcmp( log_msg.msg_text, LOG_MSG_EXIT ) == 0 ) {
            printf( "Logger: log msg is exit cmnd.\n" );
            break;
        }

        // display the message
        printf( "%s \n", log_msg.msg_text );

        // TODO: write msg to file
    }
}

char* logger_get_log_time()
{
    char* result = malloc( sizeof(char) * 50 );
    time_t now = time( 0 );
    strftime ( result, 100, "[%Y-%m-%d %H:%M:%S]", localtime ( &now ) );
    return result;
}

char* logger_get_log_type( log_msg_type_t type )
{
    char* result = malloc( sizeof( char ) * 5 );
    switch ( type ) {
        case LOG_MSG_TYPE_DEBUG:
            result = "DEBUG";
            break;
        case LOG_MSG_TYPE_INFO:
            result = "INFO";
            break;
        case LOG_MSG_TYPE_ERROR:
            result = "ERROR";
            break;
        default:
            result = "UNDEF";
            break;
    }
    return result;
}

int logger_log_msg( logger_t* logger, log_msg_type_t msg_type, char* msg)
{
    /* Sys MQ data for log msg */
    log_msg_t log_msg;
    log_msg.msg_type = 1;
    ssize_t log_msg_sz  = sizeof( log_msg.msg_text );

    /* create full log message string */
    char* timestring = logger_get_log_time();
    char* typestring = logger_get_log_type( msg_type );
    sprintf( log_msg.msg_text, "%s\t%s\t%s", timestring, typestring, msg );

    /* Send log message to logger_listener */
    if ( ( msgsnd( logger->msg_queue_id, &log_msg, log_msg_sz, IPC_NOWAIT ) ) < 0 ) {
        printf("ERROR logger_log_msg(): msgsnd()\n");
        return 1;
    }

    return 0;
}
