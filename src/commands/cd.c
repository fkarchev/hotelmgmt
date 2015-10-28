/* cd.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

void cd(char *directory_name)
{
    if(chdir(directory_name) == -1)
        fprintf(stderr, "Working directory not changed!\n");
}
/* end of cd.c */
