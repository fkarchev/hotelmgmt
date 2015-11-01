/* switch.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void switching(char *hotel_current, char *hotel_name)
{
    char hotel_filename[NAME_MAXIMUM + 1];
    FILE *hotel_file;
    hotel hotel_new;

    if(!get_name("Hotel Name", hotel_filename, hotel_name)) return;

    to_filename(hotel_filename);
    strcat(hotel_filename, NAME_EXTENSION);

    hotel_file = fopen(hotel_filename, "rb");
    if(!hotel_file){
        error_print("File not opened!");
        return;
    }

    fread(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(feof(hotel_file) || ferror(hotel_file)){
        error_print("Data not read!");
        goto cleanup;
    }
    strcpy(hotel_current, to_name(hotel_new.name));

cleanup:
    fclose(hotel_file);
}
/* end of switch.c */
