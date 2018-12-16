#ifndef _CITY_TYPESH_
#define _CITY_TYPESH_

#include <stdio.h>

typedef enum city_init_s {
    CITY_OK = 0,
    CITY_FILE_PROCESS_NOK,
    CITY_PARSE_CUSTOM_NOK,
    CITY_WEATHER_URL_NOK
} city_init;

#endif
