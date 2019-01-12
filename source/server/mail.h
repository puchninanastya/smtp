#ifndef SMTP_MTA_MAIL_H
#define SMTP_MTA_MAIL_H

typedef struct mail mail;
struct mail {
    char *sender;
    char **recepients;
    int recepients_num;
    char *data;
    int data_length;
};

#endif //SMTP_MTA_MAIL_H
