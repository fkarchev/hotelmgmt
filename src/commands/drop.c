/* drop.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void drop(char *hotel_current, char *hotel_name)
{
    char hotel_filename[NAME_MAXIMUM + 1];
    FILE *hotel_file;

    if(!get_name("Hotel Name", hotel_filename, hotel_name)) return;

    to_filename(hotel_filename);
    strcat(hotel_filename, NAME_EXTENSION);

    hotel_file = fopen(hotel_filename, "rb");
    if(!hotel_file){
        error_print("Hotel does not exist!");
        return;
    }
    fclose(hotel_file);

    if(!strcmp(hotel_current, to_name(hotel_name))) *hotel_current = '\0';

    if(remove(hotel_filename) == -1)
        error_print("File not removed!");
}
/* end of drop.c */
