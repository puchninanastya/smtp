#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"
#include "config.h"

char* find_domain( char* buffer ) {

    // cut off the end of line in command buffer
    char* eol = strstr( buffer, "\r\n");
    eol[0] = '\0';
    char* command_data = ( char* ) malloc( BUFFER_SIZE );
    strcpy(command_data, &buffer[5]);
    printf( "Debug: Command data copied: %s\n", command_data );

    // skip spaces and < to find start position
    int start = 0;
    for ( start = 0;
    command_data[ start ] == ' ' || command_data[ start ] == '<';
    start++ );

    int len = strlen( command_data ) - start - 1; // -1 is for '>'
    printf( "Debug: Command data real length: %d\n", len );

    char* host = malloc( len + 1 );
    strncpy( host, &command_data[ start ], len );

    return host;
}
