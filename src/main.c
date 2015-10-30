/* main.c */
/* Date  : 20 October 2015
 * Author: Ankit Pati
 */

#include "defs/functions.h"
#include "defs/structures.h"

int main(int argc, char **argv)
{
    int sh_allowed;
    char hotel_current[NAME_MAXIMUM + 1], *user_input, *command;

    sh_allowed = system(NULL);
    *hotel_current = '\0';

    if(argc > 2)
        die("Incorrect Usage! Only valid argument is --no-shell-escape");

    if(argc == 2){
        if(!strcmp(argv[1], "--no-shell-escape")) sh_allowed &= 0;
        else die("Incorrect Usage! Only valid argument is --no-shell-escape");
    }

    for(;;){
        errno = 0;

        printf("%s%s> ", *hotel_current ? "@" : "",
                *hotel_current ? hotel_current : "hotel-sh");
        fflush(stdout);

        user_input = getstr();
        if(!user_input) die("Allocation Error!");

        command = strtok(user_input, " ");

             if(!command) goto loop_cleanup;

        else if(!strcmp(command, "cd")) cd(strtok(NULL, ""));

        else if(!strcmp(command, "init")) init(strtok(NULL, ""));
        else if(!strcmp(command, "list")) list(hotel_current, strtok(NULL, ""));
        else if(!strcmp(command, "drop")) drop(hotel_current, strtok(NULL, ""));

        else if(!strcmp(command, "create-service"))
            create_service(strtok(NULL, ""));
        else if(!strcmp(command, "service"))
            serve(hotel_current, strtok(NULL, ""));

        else if(!strcmp(command, "switch"))
            switching(hotel_current, strtok(NULL, ""));
        else if(!strcmp(command, "book")) book(hotel_current, strtok(NULL, ""));
        else if(!strcmp(command, "back")) back(hotel_current);

        else if(!strcmp(command, "exit")) exit(0);

        else if(sh_allowed){
            if(strtok(NULL, "")) user_input[strlen(user_input)] = ' ';
            system(user_input);
        }
        else fprintf(stderr, "Unknown Command!\n");

    loop_cleanup:
        free(user_input);
    }

    return 0;
}
/* end of main.c */
