/* list.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void list()
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
/* end of list.c */
