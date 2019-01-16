#include <stdio.h>
#include <memory.h>

#include "parser.h"
#include "error_fail.h"

// Regular Expressions patterns for SMTP commands from smtp_re_commands
const char* smtp_re_patterns[ SMTP_RE_CMNDS_COUNTER ] = {
        "^NOOP\\r\\n",
        "^HELO:\\s*<.+>\\r\\n",
        "^EHLO:\\s*<.+>\\r\\n",
        "^MAIL FROM:\\s*<.+>\r\n",
        "^RCPT TO:\\s*<(.+@.+)>\\r\\n",
        "^VRFY:\\s*<.+>\\r\\n",
        "^DATA\\r\\n",
        "^RSET\\r\\n",
        "^QUIT\\r\\n"
};

// Regular expressions compiled with PCRE2
pcre2_code* smtp_re_compiled[ SMTP_RE_CMNDS_COUNTER ];

int re_initialize() {
    printf( "Initializing Reg Expressions with PCRE2 library...\n" );
    int code = 0;

    for ( int re_name = 0; re_name < SMTP_RE_CMNDS_COUNTER; re_name++ ) {
        code |= re_compile(re_name);
        printf( "Debug: Re compiled reg exp %d with result code %d.\n", re_name, code );
    }

    return !code;
}

int re_finalize() {
    printf( "Finalizing PCRE2 regular expressions...\n" );
    for ( int re_name = 0; re_name < SMTP_RE_CMNDS_COUNTER; re_name++ ) {
        free( smtp_re_compiled[ re_name ] );
    }

    printf( "PCRE2 regular expessions finalized.\n" );
    return 0;
}

int re_compile( smtp_re_commands re_pattern_name )
{
    int errornumber;
    PCRE2_SIZE erroroffset;

    smtp_re_compiled[ re_pattern_name ] = pcre2_compile(
            ( PCRE2_SPTR )smtp_re_patterns[ re_pattern_name ],   /* the pattern */
            PCRE2_ZERO_TERMINATED, /* indicates pattern is zero-terminated */
            PCRE2_CASELESS,        /* default options */
            &errornumber,          /* for error number */
            &erroroffset,          /* for error offset */
            NULL );                 /* use default compile context */

    if ( !smtp_re_compiled[ re_pattern_name ] ) {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
        printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset,
               buffer);
        return 1;
    }

    return 0;
}
