/* error.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "defs/functions.h"
#include "defs/structures.h"

void error_print(char *message)
{
    fprintf(stderr, "%s\n", message);
}

void die(char *message)
{
    if(errno) perror(message);
    else fprintf(stderr, "ERROR: %s\n", message);
    exit(errno ? errno : -1);
}
/* end of error.c */
