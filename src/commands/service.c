/* service.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void serve(char *hotel_current, char *description)
{
    int i, service_id;
    char hotel_filename[NAME_MAXIMUM + 1],
         service_description[NAME_MAXIMUM + 1], guest_name[NAME_MAXIMUM + 1];
    FILE *hotel_file, *service_file;
    fpos_t write_position;
    hotel hotel_new;
    service service_new;
    room room_new;

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

    service_file = fopen(SERVICE_FILENAME, "rb");
    if(!service_file){
        error_print("Services file unavailable!");
        fclose(hotel_file);
        return;
    }

    if(!get_name("Description", service_description, description))
        goto cleanup;

    service_id = 0;
    do{
        fread(&service_new, sizeof(service_new), 1, service_file);
        if(ferror(service_file)){
            error_print("Data not read!");
            fclose(service_file);
            goto cleanup;
        }
        ++service_id;
    } while(
        !feof(service_file) &&
        strcmp(service_new.description, service_description)
    );

    if(feof(service_file)){
        error_print("Service not found!");
        goto cleanup;
    }

    if(!get_name("Guest Name", guest_name, NULL))
        goto cleanup;

    fread(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(feof(hotel_file) || ferror(hotel_file)){
        error_print("Data not read!");
        goto cleanup;
    }

    for(i = 0; i < hotel_new.rooms; ++i){
        fgetpos(hotel_file, &write_position);
        fread(&room_new, sizeof(room_new), 1, hotel_file);
        if(ferror(hotel_file)){
            error_print("Data not read!");
            goto cleanup;
        }
        if(feof(hotel_file)) break;
        if(!strcmp(room_new.guest, guest_name)) break;
    }

    if(i == hotel_new.rooms || feof(hotel_file)){
        error_print("Guest not found!");
        goto cleanup;
    }

    if(room_new.number_of_services < SERVICE_MAXIMUM)
        room_new.service_list[room_new.number_of_services++] = service_id;
    else{
        error_print("Maximum service limit reached! Use\n\tbill <guest name>");
        goto cleanup;
    }

    fsetpos(hotel_file, &write_position);
    fwrite(&room_new, sizeof(room_new), 1, hotel_file);
    if(ferror(hotel_file)){
        error_print("Data not written!");
        goto cleanup;
    }

cleanup:
    fclose(hotel_file);
    fclose(service_file);
}
/* end of service.c */
