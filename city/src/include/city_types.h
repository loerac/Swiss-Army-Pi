#ifndef _CITY_TYPESH_
#define _CITY_TYPESH_

#include <stdio.h>

typedef enum city_init_s {
    CITY_OK = 0,
    CITY_FILE_PROCESS_NOK,
    CITY_PARSE_CUSTOM_NOK,
    CITY_WEATHER_URL_NOK
} city_init;

typedef enum city_search_e {
    CITY_SEARCH_NONE = 0,
    CITY_SEARCH_ID = 1,
    CITY_SEARCH_NAME = 2,
    CITY_SEARCH_ZIP = 4,
    CITY_SEARCH_COORD = 8
} city_search_e;

typedef enum city_format_e {
    CITY_FORMAT_NONE = 0,
    CITY_FORMAT_LANG = 1,
    CITY_FORMAT_UNIT = 2
} city_format_e;

#endif
