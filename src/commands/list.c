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
        fprintf(stderr, "Directory not opened!\n");
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
                fprintf(stderr, "File not opened!\n");
                goto cleanup;
            }

            fread(&hotel_disp, sizeof(hotel_disp), 1, hotel_file);
            if(feof(hotel_file) || ferror(hotel_file)){
                fprintf(stderr, "Data not read from %s!\n",
                        to_filename(hotel_filename));
                goto loop_cleanup;
            }

            printf("%s\n", to_name(hotel_disp.name));

        loop_cleanup:
            fclose(hotel_file);
        }

cleanup:
    closedir(dir);
}

static void list_guests()
{
}

static void list_services()
{
    int service_id;
    FILE *service_file;
    service service_new;

    service_file = fopen("services.srv", "rb");
    if(!service_file){
        fprintf(stderr, "File not opened!\n");
        return;
    }

    printf(
        "+----+------+------------------------------------------------------+\n"
        "| ID | Cost |                     Description                      |\n"
        "+----+------+------------------------------------------------------+\n"
    );

    for(service_id = 1; ; ++service_id){
        fread(&service_new, sizeof(service_new), 1, service_file);
        if(ferror(service_file)){
            fprintf(stderr, "Data not read!\n");
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

void list(char *what)
{
    char *user_input;

    if(!what){
        printf("What to list? (hotels / guests / services): ");
        fflush(stdout);

        user_input = what = getstr();
        if(!user_input) die("Allocation Error!");
    }
    else user_input = NULL;

         if(!strcmp(what, "hotels")) list_hotels();
    else if(!strcmp(what, "guests")) list_guests();
    else if(!strcmp(what, "services")) list_services();
    else fprintf(stderr, "Unknown List!\n");

    free(user_input);
}
/* end of list.c */
