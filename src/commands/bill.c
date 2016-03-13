/* bill.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void bill(char *hotel_current, char *guest)
{
    int i, service_id, bill_amount;
    char hotel_filename[NAME_MAXIMUM + 1], guest_name[NAME_MAXIMUM + 1];
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

    if(!get_name("Guest Name", guest_name, guest)) goto cleanup;

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

    if(!room_new.number_of_services) goto cleanup;

    printf(
        "Bill for %s\n"
        "+----+------+------------------------------------------------------+\n"
        "| ID | Cost |                     Description                      |\n"
        "+----+------+------------------------------------------------------+\n"
        , to_name(room_new.guest)
    );

    bill_amount = 0;
    for(i = 0; i < room_new.number_of_services; ++i){
        for(service_id = 1; ; ++service_id){
            fread(&service_new, sizeof(service_new), 1, service_file);
            if(feof(service_file) || ferror(service_file)){
                error_print("Data not read!");
                goto cleanup;
            }

            if(service_id != room_new.service_list[i]) continue;

            printf(
                "| %2d | %4d | %52.52s |\n",
                service_id, service_new.cost, to_name(service_new.description)
            );
            bill_amount += service_new.cost;
            break;
        }
        rewind(service_file);
    }
    room_new.number_of_services = 0;

    fsetpos(hotel_file, &write_position);
    fwrite(&room_new, sizeof(room_new), 1, hotel_file);
    if(ferror(hotel_file)){
        error_print("Data not written!");
        goto cleanup;
    }

    printf(
        "+----+------+------------------------------------------------------+\n"
        "|    | %4d | Total                                                |\n"
        "+----+------+------------------------------------------------------+\n"
        , bill_amount
    );

cleanup:
    fclose(hotel_file);
    fclose(service_file);
}
/* end of bill.c */
