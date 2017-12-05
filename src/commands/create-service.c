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
    int service_id;
    service service_old;
	
    service_file = fopen(SERVICE_FILENAME, "ab+");
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

    //Set max price for service
    if(service_new.cost< 0 || service_new.cost > 9999){
	error_print("Service price should be between 0 and 9999");
	return;
}


//Check if this service already exists
 for(service_id = 1; ; ++service_id){
        fread(&service_old, sizeof(service_old), 1, service_file);
        if(ferror(service_file)){
            error_print("Problem with services file");
            goto cleanup;
        }
        if(feof(service_file)) {break;}

		if(strcmp(service_new.description,service_old.description)==0)
		{
			error_print("Service exists");
			goto cleanup;
		}
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
