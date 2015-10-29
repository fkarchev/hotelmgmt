/* list-service.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void list_service()
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
/* end of list-service.c */
