/* conversions.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "defs/functions.h"
#include "defs/structures.h"

char *to_filename(char *s)
{
    size_t i;
    for(i = 0; s[i]; ++i){
        if( /* POSIX Portable Filename Character Set + <space> */
            isalnum(s[i]) ||
            s[i] == ' ' || s[i] == '.' || s[i] == '-' || s[i] == '_'
        ) continue;
        s[i] = '_';
    }
    return s;
}

char *to_name(char *s)
{
    size_t i;
    for(i = 0; s[i]; ++i)
        if(s[i] == '\n' && iscntrl(s[i]))
            s[i] = '_'; /* sanitising display names to protect terminals */
    return s;
}

int to_whole(char *s)
{
    size_t i;
    for(i = 0; isdigit(s[i]); ++i);
    return (i == strlen(s)) ? atoi(s) : -1;
}
/* end of conversions.c */
