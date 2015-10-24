/* hotel-sh.c */
/* Date  : 20 October 2015
 * Author: Ankit Pati
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

typedef struct{
    char name[80];
    unsigned floors, rooms;
    time_t time_of_creation;
} hotel;

void die(char *message)
{
    if(errno) perror(message);
    else fprintf(stderr, "ERROR: %s\n", message);
    exit(errno ? errno : -1);
}

void init()
{
    char *user_input, hotel_filename[80];
    FILE *hotel_file;
    hotel hotel_new;

    puts("Filename?");
    user_input = getstr();
    if(!user_input) die("Allocation Error!");
    strncpy(hotel_filename, user_input, 79);
    hotel_filename[79] = '\0';
    printf("\tEntered  filename: %s\n\tAccepted filename: %s\n",
            user_input, hotel_filename);
    free(user_input);
    putchar('\n');

    hotel_file = fopen(hotel_filename, "wb");
    if(!hotel_file){
        fprintf(stderr, "File could not be opened!\n");
        return;
    }

    puts("Hotel Name?");
    user_input = getstr();
    if(!user_input) die("Allocation Error!");
    strncpy(hotel_new.name, user_input, 79);
    hotel_new.name[79] = '\0';
    printf("\tEntered  Hotel Name: %s\n\tAccepted Hotel Name: %s\n",
            user_input, hotel_new.name);
    free(user_input);
    putchar('\n');

    time(&hotel_new.time_of_creation);

    fwrite(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(ferror(hotel_file)) fprintf(stderr, "Data could not be written!\n");
    else printf("Hotel Created.");
    putchar('\n');
}

int main()
{
    char *user_input;

    for(;;){
        printf("hotel_sh%s> ", "");
        fflush(stdout);

        user_input = getstr();
        if(!user_input) die("Allocation Error!");

             if(!strcmp(user_input, "init")) init();
        else if(!strcmp(user_input, "exit")) exit(0);
        else if(!strcmp(user_input, ""    )) continue;
        else puts("Unknown Command!\n");

        free(user_input);
    }

    return 0;
}
/* end of hotel-sh.c */
