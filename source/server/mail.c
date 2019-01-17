#include <stdlib.h>

#include "mail.h"

void free_mail( mail* mail ) {
    free( mail->sender );
    mail->sender = NULL;
    for ( int i = 0; i < mail->recepients_num; i++ ) {
        free( mail->recepients[ i ] );
    }
    free( mail->recepients );
    free( mail->data );
    free( mail );
}