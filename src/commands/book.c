/* book.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void book(char *hotel_current, char *guest)
{
    int   i;
    char hotel_filename[NAME_MAXIMUM + 1], guest_name[NAME_MAXIMUM + 1];
    FILE *hotel_file;
    hotel hotel_new;
    room  room_new;

    if(!*hotel_current){
        error_print("No hotel selected! Use\n\tswitch <hotel name>");
        return;
    }

    to_filename(strcpy(hotel_filename, hotel_current));
    strcat(hotel_filename, NAME_EXTENSION);

    hotel_file = fopen(hotel_filename, "rb+");
    if(!hotel_file){
        error_print("File not opened!");
        return;
    }

    fread(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(feof(hotel_file) || ferror(hotel_file)){
        error_print("Data not read!");
        goto cleanup;
    }

    for(i = 0; i < hotel_new.rooms; ++i){
        fread(&room_new, sizeof(room_new), 1, hotel_file);
        if(ferror(hotel_file)){
            error_print("Data not read!");
            goto cleanup;
        }
        if(feof(hotel_file)) break;
    }

    if(i == hotel_new.rooms){
        error_print("No rooms available!");
        return;
    }

    if(!get_name("Guest Name", guest_name, guest)) goto cleanup;

    strcpy(room_new.guest, guest_name);
    room_new.number_of_services = 0;

    fwrite(&room_new, sizeof(room_new), 1, hotel_file);
    if(ferror(hotel_file)){
        error_print("Data not written!");
        fclose(hotel_file);
        return;
    }

cleanup:
    fclose(hotel_file);
}
/* end of book.c */
