/* create-service.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void create_service(char *description)
{
    FILE *service_file;
    service service_new;

    service_file = fopen(SERVICE_FILENAME, "ab");
    if(!service_file){
        error_print("File not opened!");
        return;
    }

    if(!get_name("Description", service_new.description, description))
        goto cleanup;

    service_new.cost = get_number("Cost", NULL);
    if(service_new.cost == -1){
        if(!ftell(service_file)){
            fclose(service_file);
            remove(SERVICE_FILENAME);
            return;
        }
        goto cleanup;
    }

    fwrite(&service_new, sizeof(service_new), 1, service_file);
    if(ferror(service_file)){
        error_print("Data not written!");
        fclose(service_file);
        return;
    }

cleanup:
    fclose(service_file);
}
/* end of create-service.c */
