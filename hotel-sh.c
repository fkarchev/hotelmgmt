/* hotel-sh.c */
/* Date  : 20 October 2015
 * Author: Ankit Pati
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

char *getstr()
{
    int c;
    size_t n;
    char *s, *t;
    if(!(s=t=malloc(1))) return NULL;
    for(n=0; (c=getchar())!=EOF && c-'\n' && (t=realloc(s, n+2)); s[n++]=c) s=t;
    if(!t){
        free(s);
        return NULL;
    }
    s[n]='\0';
    return s;
}

#define NAME_MAX 80
#define NAME_EXTENSION ".hms"
#define NAME_PRE_MAX (NAME_MAX - strlen(NAME_EXTENSION))

typedef struct{
    char name[NAME_MAX + 1];
    unsigned floors, rooms;
    time_t last_updated;
} hotel;

void die(char *message)
{
    if(errno) perror(message);
    else fprintf(stderr, "ERROR: %s\n", message);
    exit(errno ? errno : -1);
}

char *to_filename(char *s)
{
    size_t i;
    for(i = 0; s[i]; ++i){
        if(
            isalnum(s[i]) || isdigit(s[i]) ||
            s[i] == ' ' || s[i] == '.' || s[i] == '-' || s[i] == '_'
        ) continue;
        s[i] = '_';
    }
    return s;
}

void init(char *hotel_name)
{
    char *user_input,
          hotel_filename[NAME_MAX + 1];
    FILE *hotel_file;
    hotel hotel_new;

    if(!hotel_name){
        puts("Hotel Name?");
        user_input = hotel_name = getstr();
        if(!hotel_name) die("Allocation Error!");
    }

    if(strlen(hotel_name) > NAME_PRE_MAX){
        fprintf(stderr, "Name too large. Maximum %u characters allowed.\n",
                (unsigned)NAME_PRE_MAX);
        return;
    }

    strcpy(hotel_new.name, hotel_name);
    strcpy(hotel_filename, to_filename(hotel_name));
    strcat(hotel_filename, NAME_EXTENSION);
    free(user_input);

    hotel_file = fopen(hotel_filename, "rb");
    if(hotel_file){
        fprintf(stderr, "File already exists!\n");
        fclose(hotel_file);
        return;
    }

    hotel_file = fopen(hotel_filename, "wb");
    if(!hotel_file){
        fprintf(stderr, "File could not be opened!\n");
        return;
    }

    time(&hotel_new.last_updated);

    fwrite(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(ferror(hotel_file)) fprintf(stderr, "Data could not be written!\n");
    else puts("Hotel Created.");
}

int main(int argc, char **argv)
{
    unsigned sh_allowed = ~0u;
    char *user_input, *command;

    if(argc > 2)
        die("Incorrect Usage! Only valid argument is --no-shell-escape");

    if(argc == 2){
        if(!strcmp(argv[1], "--no-shell-escape")) sh_allowed = 0;
        else die("Incorrect Usage! Only valid argument is --no-shell-escape");
    }

    sh_allowed &= system(NULL);

    for(;;){
        printf("hotel-sh%s> ", "");
        fflush(stdout);

        user_input = getstr();
        if(!user_input) die("Allocation Error!");

        command = strtok(user_input, " ");

             if(!command) continue;
        else if(!strcmp(command, "init")) init(strtok(NULL, ""));
        else if(!strcmp(command, "cd")){
            if(chdir(strtok(NULL, "")) == -1)
                fprintf(stderr, "Could not change working directory!\n");
        }
        else if(!strcmp(command, "exit")) exit(0);
        else if(sh_allowed){
            user_input[strlen(user_input)] = ' ';
            system(user_input);
            user_input[strlen(user_input)] = '\0';
        }
        else puts("Unknown Command!");

        free(user_input);
    }

    return 0;
}
/* end of hotel-sh.c */
