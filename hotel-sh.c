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
#define TEMP_EXTENSION ".tmp"
#define NAME_PRE_MAXIMUM (NAME_MAXIMUM - 4)

typedef struct{
    char name[NAME_PRE_MAXIMUM + 1];
    int rooms;
    time_t last_updated;
} hotel;

typedef struct{
    char guest[NAME_MAXIMUM + 1], service_request[NAME_MAXIMUM + 1];
    int bill_due;
} room;

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
        to_filename(strcpy(hotel_filename, hotel_name));
        strcat(hotel_filename, NAME_EXTENSION);

        free(user_input);
    }

    hotel_file = fopen(hotel_filename, "rb");
    if(hotel_file){
        fprintf(stderr, "File already exists!\n");
        goto cleanup;
    }

    hotel_file = fopen(hotel_filename, "wb");
    if(!hotel_file){
        fprintf(stderr, "File not opened!\n");
        return;
    }

    /* get floors and rooms */
    {
        printf("Rooms: ");
        fflush(stdout);
        user_input = getstr();
        if(!user_input) die("Allocation Error!");
        hotel_new.rooms = to_whole(user_input);
        free(user_input);

        if(hotel_new.rooms == -1){
            fprintf(stderr, "Positive numbers expected!\n");
            goto cleanup;
        }
    }

    time(&hotel_new.last_updated);

    fwrite(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(ferror(hotel_file)) fprintf(stderr, "Data not written!\n");
    else puts("Hotel Created.");

cleanup:
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
                goto cleanup;
            }

            fread(&hotel_disp, sizeof(hotel_disp), 1, hotel_file);
            if(feof(hotel_file) || ferror(hotel_file)){
                fprintf(stderr, "Data not read from %s!\n",
                        to_filename(hotel_filename));
                goto loop_cleanup;
            }

            printf("%s\n", to_name(hotel_disp.name));

        loop_cleanup:
            fclose(hotel_file);
        }

cleanup:
    closedir(dir);
}

void drop(char *hotel_current, char *hotel_name)
{
    char *user_input,
          hotel_filename[NAME_MAXIMUM + 1];
    FILE *hotel_file;

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

        to_filename(strcpy(hotel_filename, hotel_name));
        strcat(hotel_filename, NAME_EXTENSION);

        free(user_input);
    }

    hotel_file = fopen(hotel_filename, "rb");
    if(!hotel_file){
        fprintf(stderr, "Hotel does not exist!\n");
        return;
    }
    fclose(hotel_file);

    if(!strcmp(hotel_current, to_name(hotel_name))) *hotel_current = '\0';

    if(remove(hotel_filename) == -1)
        fprintf(stderr, "File not removed!\n");
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

        to_filename(strcpy(hotel_filename, hotel_name));
        strcat(hotel_filename, NAME_EXTENSION);

        free(user_input);
    }

    hotel_file = fopen(hotel_filename, "rb");
    if(!hotel_file){
        fprintf(stderr, "File not opened!\n");
        return;
    }

    fread(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(feof(hotel_file) || ferror(hotel_file))
        fprintf(stderr, "Data not read!\n");
    else strcpy(hotel_current, to_name(hotel_new.name));

    fclose(hotel_file);
}

void back(char *hotel_current)
{
    *hotel_current = '\0';
}

void book(char *hotel_current, char *guest)
{
    int   i;
    char *user_input,
          hotel_filename[NAME_MAXIMUM + 1];
    FILE *hotel_file;
    hotel hotel_new;
    room  room_new;

    if(!*hotel_current){
        fprintf(stderr, "No hotel selected. Use\n\tswitch <hotel name>\n");
        return;
    }

    to_filename(strcpy(hotel_filename, hotel_current));
    strcat(hotel_filename, NAME_EXTENSION);

    hotel_file = fopen(hotel_filename, "r+");
    if(!hotel_file){
        fprintf(stderr, "File not opened!\n");
        return;
    }

    fread(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(feof(hotel_file) || ferror(hotel_file)){
        fprintf(stderr, "Data not read!\n");
        goto cleanup;
    }

    for(i = 0; i < hotel_new.rooms; ++i){
        fread(&room_new, sizeof(room_new), 1, hotel_file);
        if(ferror(hotel_file)){
            fprintf(stderr, "Data not read!\n");
            goto cleanup;
        }
        if(feof(hotel_file)) break;
    }

    if(i == hotel_new.rooms){
        fprintf(stderr, "No rooms available.\n");
        return;
    }

    /* get guest details */
    {
        if(!guest){
            printf("Guest Name: ");
            fflush(stdout);

            user_input = guest = getstr();
            if(!user_input) die("Allocation Error!");
        }
        else user_input = NULL;

        if(strlen(guest) > NAME_MAXIMUM){
            fprintf(stderr, "Name too large. Maximum %u characters allowed.\n",
                    (unsigned)NAME_MAXIMUM);
            if(user_input) free(user_input);
            goto cleanup;
        }

        strcpy(room_new.guest, to_name(guest));
        *room_new.service_request = '\0';
        room_new.bill_due = 0;

        if(user_input) free(user_input);
    }

    fwrite(&room_new, sizeof(room_new), 1, hotel_file);
    if(ferror(hotel_file)){
        fprintf(stderr, "Data not written!\n");
        fclose(hotel_file);
        return;
    }

    puts("Room booked.");

cleanup:
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
        errno = 0;

        printf("%s%s> ", *current_hotel ? "@" : "",
                *current_hotel ? current_hotel : "hotel-sh");
        fflush(stdout);

        user_input = getstr();
        if(!user_input) die("Allocation Error!");

        command = strtok(user_input, " ");

             if(!command) goto loop_cleanup;
        else if(!strcmp(command, "init")) init(strtok(NULL, ""));
        else if(!strcmp(command, "list")) list();
        else if(!strcmp(command, "drop")) drop(current_hotel, strtok(NULL, ""));
        else if(!strcmp(command, "switch"))
            switching(current_hotel, strtok(NULL, ""));
        else if(!strcmp(command, "cd")){
            if(chdir(strtok(NULL, "")) == -1)
                fprintf(stderr, "Working directory not changed!\n");
        }
        else if(!strcmp(command, "book")) book(current_hotel, strtok(NULL, ""));
        else if(!strcmp(command, "back")) back(current_hotel);
        else if(!strcmp(command, "exit")) exit(0);
        else if(sh_allowed){
            user_input[strlen(user_input)] = ' ';
            system(user_input);
            user_input[strlen(user_input)] = '\0';
        }
        else puts("Unknown Command!");

    loop_cleanup:
        free(user_input);
    }

    return 0;
}
/* end of hotel-sh.c */
