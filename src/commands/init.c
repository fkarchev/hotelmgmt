/* init.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void init(char *hotel_name)
{
    char hotel_filename[NAME_MAXIMUM + 1];
    FILE *hotel_file;
    hotel hotel_new;

    if(!get_name("Hotel Name", hotel_new.name, hotel_name)) return;

    to_filename(strcpy(hotel_filename, hotel_new.name));
    strcat(hotel_filename, NAME_EXTENSION);

    hotel_file = fopen(hotel_filename, "rb");
    if(hotel_file){
        error_print("File already exists!");
        goto cleanup;
    }

    hotel_file = fopen(hotel_filename, "wb");
    if(!hotel_file){
        error_print("File not opened!");
        return;
    }

    hotel_new.rooms = get_number("Rooms", NULL);
    if(hotel_new.rooms == -1){
        fclose(hotel_file);
        remove(hotel_filename);
        return;
    }

    fwrite(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(ferror(hotel_file)){
        error_print("Data not written!");
        goto cleanup;
    }

cleanup:
    fclose(hotel_file);
}
/* end of init.c */
