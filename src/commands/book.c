/* book.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

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

    hotel_file = fopen(hotel_filename, "rb+");
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
        room_new.number_of_services = 0;

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
/* end of book.c */
