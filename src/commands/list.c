/* list.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

static void list_hotels()
{
    char hotel_filename[NAME_MAXIMUM + 1];
    FILE *hotel_file;
    DIR *dir;
    struct dirent *ent;
    hotel hotel_disp;

    if(!(dir = opendir("./"))){
        error_print("Directory not opened!");
        return;
    }

    while((ent = readdir(dir)))
        if(
            strlen(ent->d_name) < NAME_MAXIMUM &&
            !strcmp(
                ent->d_name + strlen(ent->d_name) - strlen(NAME_EXTENSION),
                NAME_EXTENSION
            )
        ){
            strcpy(hotel_filename, ent->d_name);

            hotel_file = fopen(hotel_filename, "rb");
            if(!hotel_file){
                error_print("File not opened!");
                goto cleanup;
            }

            fread(&hotel_disp, sizeof(hotel_disp), 1, hotel_file);
            if(feof(hotel_file) || ferror(hotel_file)){
                error_print("Data not read!");
                goto loop_cleanup;
            }

            printf("%s\n", to_name(hotel_disp.name));

        loop_cleanup:
            fclose(hotel_file);
        }

cleanup:
    closedir(dir);
}

static void list_guests(char *hotel_current)
{
    int i;
    char hotel_filename[NAME_MAXIMUM + 1];
    FILE *hotel_file;
    hotel hotel_new;
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
        puts(to_name(room_new.guest));
    }

cleanup:
    fclose(hotel_file);
}

static void list_services()
{
    int service_id;
    FILE *service_file;
    service service_new;

    service_file = fopen("services.srv", "rb");
    if(!service_file) return;

    printf(
        "+----+------+------------------------------------------------------+\n"
        "| ID | Cost |                     Description                      |\n"
        "+----+------+------------------------------------------------------+\n"
    );

    for(service_id = 1; ; ++service_id){
        fread(&service_new, sizeof(service_new), 1, service_file);
        if(ferror(service_file)){
            error_print("Data not read!");
            goto cleanup;
        }
        if(feof(service_file)) goto cleanup;

        printf(
            "| %2d | %4d | %52.52s |\n",
            service_id, service_new.cost, to_name(service_new.description)
        );
    }

cleanup:
    printf(
        "+----+------+------------------------------------------------------+\n"
    );
    fclose(service_file);
}

void list(char *hotel_current, char *what)
{
    char *user_input;

    if(!what){
        printf("What to list? (hotels / guests / services): ");
        fflush(stdout);

        user_input = what = getstr();
    }
    else user_input = NULL;

         if(!strcmp(what, "hotels"  )) list_hotels();
    else if(!strcmp(what, "guests"  )) list_guests(hotel_current);
    else if(!strcmp(what, "services")) list_services();
    else error_print("Unknown list!");

    free(user_input);
}
/* end of list.c */
