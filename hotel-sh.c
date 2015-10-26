/* hotel-sh.c */
/* Date  : 20 October 2015
 * Author: Ankit Pati
 */

#include <unistd.h>
#include <dirent.h>
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

#define NAME_MAXIMUM 80
#define NAME_EXTENSION ".hms"
#define NAME_PRE_MAXIMUM (NAME_MAXIMUM - strlen(NAME_EXTENSION))

typedef struct{
    char name[NAME_MAXIMUM + 1];
    int floors, rooms;
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
        if( /* POSIX Portable Filename Character Set + <space> */
            isalnum(s[i]) || isdigit(s[i]) ||
            s[i] == ' ' || s[i] == '.' || s[i] == '-' || s[i] == '_'
        ) continue;
        s[i] = '_';
    }
    return s;
}

char *to_name(char *s)
{
    size_t i;
    for(i = 0; s[i]; ++i)
        if(s[i] == '\n' && iscntrl(s[i]))
            s[i] = '_'; /* sanitising display names to protect terminals */
    return s;
}

int to_whole(char *s)
{
    size_t i;
    for(i = 0; isdigit(s[i]); ++i);
    return (i == strlen(s)) ? atoi(s) : -1;
}

void init(char *hotel_name)
{
    char *user_input,
          hotel_filename[NAME_MAXIMUM + 1];
    FILE *hotel_file;
    hotel hotel_new;

    /* get hotel name and filename */
    {
        if(!hotel_name){
            printf("Hotel Name: ");
            fflush(stdout);

            user_input = hotel_name = getstr();
            if(!user_input) die("Allocation Error!");
        }
        else user_input = NULL;

        if(strlen(hotel_name) > NAME_PRE_MAXIMUM){
            fprintf(stderr, "Name too large. Maximum %u characters allowed.\n",
                    (unsigned)NAME_PRE_MAXIMUM);
            if(user_input) free(user_input);
            return;
        }

        strcpy(hotel_new.name, to_name(hotel_name));
        strcpy(hotel_filename, to_filename(hotel_name));
        strcat(hotel_filename, NAME_EXTENSION);
        free(user_input);
    }

    hotel_file = fopen(hotel_filename, "rb");
    if(hotel_file){
        fprintf(stderr, "File already exists!\n");
        fclose(hotel_file);
        return;
    }

    hotel_file = fopen(hotel_filename, "wb");
    if(!hotel_file){
        fprintf(stderr, "File not opened!\n");
        return;
    }

    /* get floors and rooms */
    {
        printf("Floors: ");
        fflush(stdout);
        user_input = getstr();
        if(!user_input) die("Allocation Error!");
        hotel_new.floors = to_whole(user_input);
        free(user_input);

        printf("Rooms : ");
        fflush(stdout);
        user_input = getstr();
        if(!user_input) die("Allocation Error!");
        hotel_new.rooms = to_whole(user_input);
        free(user_input);

        if(hotel_new.floors == -1 || hotel_new.rooms == -1){
            fprintf(stderr, "Positive numbers expected!\n");
            fclose(hotel_file);
            return;
        }
    }

    time(&hotel_new.last_updated);

    fwrite(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(ferror(hotel_file)) fprintf(stderr, "Data not written!\n");
    else puts("Hotel Created.");

    fclose(hotel_file);
}

void list()
{
    char hotel_filename[NAME_MAXIMUM + 1];
    FILE *hotel_file;
    DIR *dir;
    struct dirent *ent;
    hotel hotel_disp;

    if(!(dir = opendir("./"))){
        fprintf(stderr, "Directory not opened!\n");
        return;
    }

    while((ent = readdir(dir)))
        if(
            strlen(ent->d_name) < NAME_MAXIMUM &&
            !strcmp(
                ent->d_name + strlen(ent->d_name) - strlen(NAME_EXTENSION),
                NAME_EXTENSION
            )
        ){
            strcpy(hotel_filename, ent->d_name);

            hotel_file = fopen(hotel_filename, "rb");
            if(!hotel_file){
                fprintf(stderr, "File not opened!\n");
                closedir(dir);
                return;
            }

            fread(&hotel_disp, sizeof(hotel_disp), 1, hotel_file);
            if(ferror(hotel_file)){
                fprintf(stderr, "Data not read from %s!\n",
                        to_filename(hotel_filename));
                continue;
            }

            printf("%s\n", to_name(hotel_disp.name));

            fclose(hotel_file);
        }

    closedir(dir);
}

void back(char *hotel_current)
{
    *hotel_current = '\0';
}

void switching(char *hotel_current, char *hotel_name)
{
    char *user_input,
          hotel_filename[NAME_MAXIMUM + 1];
    FILE *hotel_file;
    hotel hotel_new;

    /* get hotel name and filename */
    {
        if(!hotel_name){
            printf("Hotel Name: ");
            fflush(stdout);

            user_input = hotel_name = getstr();
            if(!user_input) die("Allocation Error!");
        }
        else user_input = NULL;

        if(strlen(hotel_name) > NAME_PRE_MAXIMUM){
            fprintf(stderr, "Name too large. Maximum %u characters allowed.\n",
                    (unsigned)NAME_PRE_MAXIMUM);
            if(user_input) free(user_input);
            return;
        }

        strcpy(hotel_filename, to_filename(hotel_name));
        strcat(hotel_filename, NAME_EXTENSION);
        free(user_input);
    }

    hotel_file = fopen(hotel_filename, "rb");
    if(!hotel_file){
        fprintf(stderr, "File not opened!\n");
        return;
    }

    fread(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(ferror(hotel_file)) fprintf(stderr, "Data not read!\n");
    else strcpy(hotel_current, to_name(hotel_new.name));

    fclose(hotel_file);
}

int main(int argc, char **argv)
{
    int sh_allowed;
    char current_hotel[NAME_MAXIMUM + 1], *user_input, *command;

    sh_allowed = system(NULL);
    *current_hotel = '\0';

    if(argc > 2)
        die("Incorrect Usage! Only valid argument is --no-shell-escape");

    if(argc == 2){
        if(!strcmp(argv[1], "--no-shell-escape")) sh_allowed &= 0;
        else die("Incorrect Usage! Only valid argument is --no-shell-escape");
    }

    for(;;){
        printf("%s%s> ", *current_hotel ? "@" : "",
                *current_hotel ? current_hotel : "hotel-sh");
        fflush(stdout);

        user_input = getstr();
        if(!user_input) die("Allocation Error!");

        command = strtok(user_input, " ");

             if(!command) continue;
        else if(!strcmp(command, "init")) init(strtok(NULL, ""));
        else if(!strcmp(command, "list")) list();
        else if(!strcmp(command, "back")) back(current_hotel);
        else if(!strcmp(command, "switch"))
            switching(current_hotel, strtok(NULL, ""));
        else if(!strcmp(command, "cd")){
            if(chdir(strtok(NULL, "")) == -1)
                fprintf(stderr, "Working directory not changed!\n");
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
