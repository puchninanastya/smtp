#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mail.h"
#include "config.h"

void free_mail( mail* mail ) {
    if ( mail->sender != NULL ) {
        free(mail->sender);
        mail->sender = NULL;
    }
    for ( int i = 0; i < mail->recepients_num; i++ ) {
        free( mail->recepients[ i ] );
    }
    if ( mail->recepients != NULL) {
        free( mail->recepients );
        mail->recepients = NULL;
    }
    if ( mail->data != NULL ) {
        free(mail->data);
        mail->data = NULL;
    }
    free( mail );
}


void append_data_to_mail( mail* mail, char* new_data, int new_data_len ) {
    // realloc if needed
    if ( strlen( mail->data ) + strlen( new_data ) + 1 >= mail->data_capacity ) { // +1 char for newline
        mail->data = realloc_mail_data_for_length( mail, new_data_len );
    }

    // append data from new buffer to mail buffer
    strcat( mail->data, new_data );
    mail->data[ strlen( mail->data ) ] = '\0';
}

char* realloc_mail_data_for_length( mail* mail, int len ) {
    int malloc_size = strlen( mail->data ) + BUFFER_SIZE;
    printf( "Realloc mail data buffer for new size: %d\n", malloc_size );
    mail->data = ( char * )realloc( mail->data, malloc_size ); // TODO: do i need to check all reallocs?
    mail->data_capacity = malloc_size;
    return  mail->data;
}

