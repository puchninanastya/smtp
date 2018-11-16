#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "server.h"

void exit_handler( int signal ) {
    // TODO: stop server and clean to release ports
    
    const char *signal_name;

    // Find out which signal we're handling
    switch (signal) {
        case SIGHUP:
            signal_name = "SIGHUP";
            break;
        case SIGTERM:
            signal_name = "SIGTERM";
            break;
        case SIGQUIT:
            signal_name = "SIGQUIT";
            break;
        case SIGUSR1:
            signal_name = "SIGUSR1";
            break;
        case SIGINT:
            signal_name = "SIGUSR1";
        default:
            fprintf(stderr, "Caught wrong signal: %d\n", signal);
            return;
    }

    printf("Caught %s, exiting now\n", signal_name);
    exit(0);
}

int set_handler_to_signals() {
    struct sigaction sa;

    sa.sa_handler = &exit_handler;
    sa.sa_flags = SA_SIGINFO;

    if ( sigemptyset( &sa.sa_mask ) != 0 )
        return 1;
    if ( sigaddset( &sa.sa_mask, SIGTERM ) != 0 )
        return 1;
    if ( sigaddset( &sa.sa_mask, SIGHUP ) != 0 )
        return 1;
    if ( sigaddset( &sa.sa_mask, SIGQUIT ) != 0 )
        return 1;
    if ( sigaddset( &sa.sa_mask, SIGINT ) != 0 )
        return 1;

    sigaction( SIGTERM, &sa, 0 );
    sigaction( SIGHUP, &sa, 0 );
    sigaction( SIGQUIT, &sa, 0 );
    sigaction( SIGINT, &sa, 0 );

    return 0;
}

int main( int argc, char **argv ) {
    if ( set_handler_to_signals() ) {
        printf("Can not set handlers to signals.\n");
        return 1;
    }
	printf("Successfully set handlers to signals.\n");

    
	return 0;
}