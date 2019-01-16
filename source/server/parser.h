#ifndef SMTP_MTA_PARSER_H
#define SMTP_MTA_PARSER_H

#define PCRE2_CODE_UNIT_WIDTH 8 // to make use of the generic function names for code units widht (uint8_t, etc.)
#include "pcre2.h"

#include "rexprs.h"

typedef enum {
    SMTP_RE_CMND_NOOP,
    SMTP_RE_CMND_HELO,
    SMTP_RE_CMND_EHLO,
    SMTP_RE_CMND_MAIL,
    SMTP_RE_CMND_RCPT,
    SMTP_RE_CMND_VRFY,
    SMTP_RE_CMND_DATA,
    SMTP_RE_CMND_RSET,
    SMTP_RE_CMND_QUIT,
    SMTP_RE_CMNDS_COUNTER
} smtp_re_commands;


int re_initialize();
int re_finalize();
int re_compile( smtp_re_commands re_pattern_name );

#endif //SMTP_MTA_PARSER_H
