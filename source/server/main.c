#include <csignal>
#include <stdio.h>

#include "server.h"

void exit_handler( int signal, siginfo_t* siginfo, void* context ) {
    std::cout << "Signal Number = " << siginfo->si_signo << std::endl;
    std::cout << "Signal Code = " << siginfo->si_code << std::endl;
    std::cout << "Sending PID = " << ( long )siginfo->si_pid << std::endl;
    std::cout << "Sending User ID = " << ( long )siginfo->si_uid << std::endl;
}

void set_handler_to_signals() {
    struct sigaction sa;
    memset( &sa, 0, sizeof( sa ) );

    sa.sa_sigaction = exit_handler;
    sa.sa_flags = SA_SIGINFO;

    if ( sigemptyset( &sa.sa_mask ) != 0 )
        return false;

    if ( sigaddset( &sa.sa_mask, SIGTERM ) != 0 )
        return false;
    if ( sigaddset( &sa.sa_mask, SIGHUP ) != 0 )
        return false;
    if ( sigaddset( &sa.sa_mask, SIGQUIT ) != 0 )
        return false;
    if ( sigaddset( &sa.sa_mask, SIGINT ) != 0 )
        return false;

    sigaction( SIGTERM, &sa, 0 );
    sigaction( SIGHUP, &sa, 0 );
    sigaction( SIGQUIT, &sa, 0 );
    sigaction( SIGINT, &sa, 0 );

    return true;
}

int main( int argc, char **argv ) {
    set_handler_to_signals();
	
	return 0;
}