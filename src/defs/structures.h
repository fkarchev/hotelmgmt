/* structures.h */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#ifndef apSTRUCTURES_H
#define apSTRUCTURES_H

#define NAME_MAXIMUM 80
#define NAME_EXTENSION ".hms"
#define TEMP_EXTENSION ".tmp"
#define NAME_PRE_MAXIMUM (NAME_MAXIMUM - 4)

typedef struct{
    char name[NAME_PRE_MAXIMUM + 1];
    int rooms;
    time_t last_updated;
} hotel;

typedef struct{
    char guest[NAME_MAXIMUM + 1], service_request[NAME_MAXIMUM + 1];
    int bill_due;
} room;

#endif
/* end of structures.h */
