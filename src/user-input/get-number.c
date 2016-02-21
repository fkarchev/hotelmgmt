/* get-number.c */
/* Date  : 31 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

int get_number(char *prompt, char *argument)
{
    int number;
    char *user_input;

    if(!argument){
        fprintf(stderr, "%s: ", prompt);
        fflush(stderr);

        user_input = argument = getstr();
    }
    else user_input = NULL;

    number = to_whole(argument);
    if(number == -1)
        error_print("Positive numbers expected!");

    free(user_input);
    return number;
}
/* end of get-number.c */
