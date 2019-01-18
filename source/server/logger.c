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
    // to destroy the message queue
    msgctl( logger->msg_queue_id, IPC_RMID, NULL);
}

void logger_run_loop( logger_t* logger )
{
    printf( "Logger: run loop(), queue id: %d.\n", logger->msg_queue_id );
    log_msg_t log_msg;

    ssize_t log_msg_sz;
    log_msg_sz = sizeof( log_msg.msg_text );

    /* Loop until exit message will be received */
    while(1) {

        printf( "Logger: in while loop.\n" );

        // msgrcv to receive message
        msgrcv( logger->msg_queue_id, &log_msg, log_msg_sz, 1, 0);

        printf( "Logger: after msgrcv().\n" );

        // break loop if received LOG_MSG_EXIT
        if ( strcmp( log_msg.msg_text, LOG_MSG_EXIT ) == 0 ) {
            printf( "Logger: log msg is exit cmnd.\n" );
            break;
        }

        // display the message
        printf( "Log msg: %s \n", log_msg.msg_text );

        // TODO: write msg to file
    }
}

int logger_log_msg( logger_t* logger, char* msg )
{

    printf( "Logger: log_msg(), queue id: %d.\n", logger->msg_queue_id );

    ssize_t log_msg_sz;
    log_msg_t log_msg;

    /*
    char timestring[ 50 ];
    time_t now = time( 0 );
    strftime ( timestring, 100, "%Y-%m-%d %H:%M:%S", localtime ( &now ) );

    printf( "Logger: log msg time: %s\n", timestring );*/

    //char* lmsg = concat_strings( timestring, msg );
    strcat( log_msg.msg_text, msg );
    log_msg_sz = sizeof( log_msg.msg_text );
    log_msg.msg_type = 1;

    printf( "Logger: before msgsnd(), sent log msg (sz: %zd): %s \n",
            log_msg_sz, log_msg.msg_text );

    if ( ( msgsnd( logger->msg_queue_id, &log_msg, log_msg_sz, IPC_NOWAIT ) ) < 0 ) {
        printf("ERROR logger_log_msg(): msgsnd()\n");
        return 1;
    }

    printf( "Logger: after msgsnd()\n" );

    return 0;
}
