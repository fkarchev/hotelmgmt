/* structures.h */
/* Date  : 28 October 2015
 * Author: Ankit Pati
 */

#ifndef apSTRUCTURES_H
#define apSTRUCTURES_H

#define NAME_MAXIMUM      80
#define SERVICE_MAXIMUM   80
#define NAME_EXTENSION    ".hms"
#define TEMP_EXTENSION    ".tmp"
#define SERVICE_EXTENSION ".srv"
#define NAME_PRE_MAXIMUM  (NAME_MAXIMUM - 4)

typedef struct{
    char name[NAME_PRE_MAXIMUM + 1];
    int rooms;
} hotel;

typedef struct{
    char guest[NAME_MAXIMUM + 1];
    int service_list[SERVICE_MAXIMUM], number_of_services;
} room;

typedef struct{
    char description[NAME_MAXIMUM];
    int id, cost;
} service;

#endif
/* end of structures.h */
