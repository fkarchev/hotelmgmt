/* create-service.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void create_service(char *description)
{
    char *user_input;
    FILE *service_file;
    service service_new;

    service_file = fopen("services.srv", "ab");
    if(!service_file){
        fprintf(stderr, "File not opened!\n");
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
            if(user_input) free(user_input);
            return;
        }

        strcpy(service_new.description, to_name(description));

        free(user_input);
    }

    /* get service id and cost */
    {
        printf("ID  : ");
        fflush(stdout);

        user_input = getstr();
        if(!user_input) die("Allocation Error!");

        service_new.id = to_whole(user_input);
        free(user_input);

        printf("Cost: ");
        fflush(stdout);

        user_input = getstr();
        if(!user_input) die("Allocation Error!");

        service_new.cost = to_whole(user_input);
        free(user_input);

        if(service_new.id == -1 || service_new.cost == -1){
            fprintf(stderr, "Positive numbers expected!\n");
            goto cleanup;
        }
    }

    fwrite(&service_new, sizeof(service_new), 1, service_file);
    if(ferror(service_file)){
        fprintf(stderr, "Data not written!\n");
        fclose(service_file);
        return;
    }

    puts("Service created.");

cleanup:
    fclose(service_file);
}
/* end of create-service.c */
