/* switch.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

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
/* end of switch.c */
