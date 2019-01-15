//
// Created by Yulia on 15.01.2019.
//

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>


//ПЕрекладывалка
// функция, объединяющая 2 строки
char* concat_2p(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the null-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

// функция, объединяющая 3 строки
char* concat_3p(const char *s1, const char *s2, const char *s3)
{
    char *result = malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1);//+1 for the null-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    strcat(result, s3);
    return result;
}

// функция, для проверки соответствие на рег. выражение
int match(const char *string, const char *pattern)
{
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return 0;
    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);
    if (status != 0) return 0;
    return 1;
}

int main(void)
{
    const char* re = "[a-z0-9]+";

    DIR *d, *_d;
    struct dirent *dir, *_dir;
    d = opendir("../maildir");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);
            bool b = match(dir->d_name, re) ? true : false;

            if(b){
                char* s_new = concat_3p("../maildir/", dir->d_name, "/new");
                char* s_cur = concat_3p("../maildir/", dir->d_name, "/cur");
//                printf("%s\n", s);
                _d = opendir(s_new);
                while ((_dir = readdir(_d)) != NULL)
                {
                    bool _b = match(_dir->d_name, re) ? true : false;
                    if(_b){
//                        printf("%s\n",s_cur);
                        char* _s = concat_3p(s_new, "/", _dir->d_name);
                        // С помощью rename() перемещаем письма из папки new в папку cur
                        rename(_s, concat_3p(s_cur, "/", _dir->d_name));

                        printf("%s\n",_s);
                    }
                }

                free(s_new);//deallocate the string
            }
        }
        closedir(d);
    }
    return(0);
}
