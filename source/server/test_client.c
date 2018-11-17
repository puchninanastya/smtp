#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

#include "error_fail.h"
#include "config.h"

int main( int argc, char **argv )
{
    printf( "Initializing client socket...\n" );

    int client_socket_fd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( client_socket_fd < 0 ) { 
        fail_on_error( "Can not create client socket!" ); 
    } 

    printf( "Setting up server address...\n" );
    struct sockaddr_in server_address; 
    memset( &server_address, '0', sizeof( server_address ) ); 
    server_address.sin_family = AF_INET; 
    server_address.sin_port = htons(SERVER_PORT); 
    if( inet_pton( AF_INET, "127.0.0.1", &server_address.sin_addr ) <= 0 )  { 
        fail_on_error( "Can not convert server ip address" ); 
    } 
    printf( "Succsessfully initialized...\n" );

    printf( "Trying to connect to server...\n" );
    if ( connect( client_socket_fd, ( struct sockaddr* )&server_address, 
        sizeof( server_address ) ) < 0 ) { 
        fail_on_error( "Can not connect to server!" );
    }
    printf( "Succsessfully connected to server...\n" );

    printf( "Trying to send data to server...\n" );
    const char* message_to_send = "Hello from test client!";
    ssize_t actual_sent = send( client_socket_fd, message_to_send, strlen(message_to_send), 0 ); 
    if ( actual_sent < 0 ) {
        fail_on_error( "Can not sent data to server!" );
    }
    printf( "Message \"%s\" sent to server\n", message_to_send );

    printf( "Trying to read data from server...\n" );
    char buffer[BUFFER_SIZE];
    memset( buffer, 0, BUFFER_SIZE * sizeof( char ) );
    ssize_t actual_read = read( client_socket_fd, buffer, sizeof( buffer ) );
    if ( actual_read < 0 ) {
        fail_on_error( "Can not read data from server!" );
    }
    printf( "Message \"%s\" received from server\n", buffer );

    printf( "Done.\n" );

    close( client_socket_fd );
    printf( "Client socket closed.\n" );

	return 0;
}