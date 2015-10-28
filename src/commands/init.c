/* init.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

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

    fwrite(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(ferror(hotel_file)) fprintf(stderr, "Data not written!\n");
    else puts("Hotel Created.");

cleanup:
    fclose(hotel_file);
}
/* end of init.c */
