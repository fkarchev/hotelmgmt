/* drop.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

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
/* end of drop.c */
