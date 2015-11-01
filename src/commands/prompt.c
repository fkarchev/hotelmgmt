/* prompt.c */
/* Date  : 31 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void prompt(char *hotel_current, int *echo_allowed, char *on_or_off)
{
    if(!on_or_off){
        printf("%s%s\n", *hotel_current ? "@" : "",
                *hotel_current ? hotel_current : "hotel-sh");
        return;
    }

         if(!strcmp(on_or_off, "on")) *echo_allowed = 1;
    else if(!strcmp(on_or_off, "off")) *echo_allowed = 0;
    else error_print("Unrecognised operation! Only on or off allowed.");
}
/* prompt.c */
