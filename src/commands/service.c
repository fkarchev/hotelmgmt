/* service.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void serve(char *hotel_current, char *description)
{
    int i, service_id;
    char *user_input,
          hotel_filename[NAME_MAXIMUM + 1];
    FILE *hotel_file, *service_file;
    fpos_t write_position;
    hotel hotel_new;
    service service_new;
    room room_new;

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

    service_file = fopen("services.srv", "rb");
    if(!service_file){
        fprintf(stderr, "File not opened!\n");
        fclose(hotel_file);
        return;
    }

    /* get service description */
    {
        if(!description){
            printf("Description: ");
            fflush(stdout);

            user_input = description = getstr();
            if(!user_input) die("Allocation Error!");
        }
        else user_input = NULL;

        if(strlen(description) > NAME_MAXIMUM){
            fprintf(stderr, "Name too large. Maximum %u characters allowed.\n",
                    (unsigned)NAME_MAXIMUM);
            goto cleanup;
        }

        to_name(description);
    }

    service_id = 0;
    do{
        fread(&service_new, sizeof(service_new), 1, service_file);
        if(ferror(service_file)){
            fprintf(stderr, "Data not read!\n");
            fclose(service_file);
            goto cleanup;
        }
        ++service_id;
    } while(
        !feof(service_file) &&
        strcmp(service_new.description, description)
    );

    if(feof(service_file)){
        fprintf(stderr, "Service not found.\n");
        goto cleanup;
    }

    free(user_input);
    /* get guest details */
    {
        printf("Guest Name: ");
        fflush(stdout);

        user_input = getstr();
        if(!user_input) die("Allocation Error!");

        if(strlen(user_input) > NAME_MAXIMUM){
            fprintf(stderr, "Name too large. Maximum %u characters allowed.\n",
                    (unsigned)NAME_MAXIMUM);
            goto cleanup;
        }

        to_name(user_input);
    }

    fread(&hotel_new, sizeof(hotel_new), 1, hotel_file);
    if(feof(hotel_file) || ferror(hotel_file)){
        fprintf(stderr, "Data not read!\n");
        goto cleanup;
    }

    for(i = 0; i < hotel_new.rooms; ++i){
        fgetpos(hotel_file, &write_position);
        fread(&room_new, sizeof(room_new), 1, hotel_file);
        if(ferror(hotel_file)){
            fprintf(stderr, "Data not read!\n");
            goto cleanup;
        }
        if(feof(hotel_file)) break;
        if(!strcmp(room_new.guest, user_input)) break;
    }

    if(i == hotel_new.rooms || feof(hotel_file)){
        fprintf(stderr, "Guest not found.\n");
        goto cleanup;
    }

    if(room_new.number_of_services < SERVICE_MAXIMUM)
        room_new.service_list[room_new.number_of_services++] = service_id;
    else{
        fprintf(stderr,
                "Maximum service limit reached. Use\n\tbill <guest name>\n");
        goto cleanup;
    }

    fsetpos(hotel_file, &write_position);
    fwrite(&room_new, sizeof(room_new), 1, hotel_file);
    if(ferror(hotel_file)){
        fprintf(stderr, "Data not written!\n");
        goto cleanup;
    }

    puts("Service recorded.");

cleanup:
    if(user_input) free(user_input);
    fclose(hotel_file);
    fclose(service_file);
}
/* end of service.c */
