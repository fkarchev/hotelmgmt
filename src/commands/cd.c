/* cd.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void cd(char *directory_name)
{
    char *user_input;

    if(!directory_name){
        printf("Directory Name: ");
        fflush(stdout);

        user_input = directory_name = getstr();
        if(!user_input) die("Allocation Error!");
    }
    else user_input = NULL;

    if(chdir(directory_name) == -1)
        fprintf(stderr, "Working directory not changed!\n");

    if(user_input) free(user_input);
}
/* end of cd.c */
