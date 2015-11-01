/* getstr.c */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#include "../defs/functions.h"
#include "../defs/structures.h"

char *getstr()
{
    int c;
    size_t n;
    char *s, *t;
    if(!(s=t=malloc(1))) goto cleanup;
    for(n=0; (c=getchar())!=EOF && c-'\n' && (t=realloc(s, n+2)); s[n++]=c) s=t;
    if(!t) goto cleanup;
    s[n]='\0';
    return s;

cleanup:
    free(s);
    die("Allocation Error!");
    return NULL;
}
/* end of getstr.c */
