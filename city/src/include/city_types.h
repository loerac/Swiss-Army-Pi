#ifndef _CITY_TYPESH_
#define _CITY_TYPESH_

#include <stdio.h>

typedef enum city_init {
   CITY_OK = 0,
   CITY_CITY_CUSTOM,
   CITY_URL_CUSTOM,
   CITY_WEATHER_URL,
   CITY_WEATHER_PARSE
} city_init_e;

typedef enum city_search {
   CITY_SEARCH_NONE = 0,
   CITY_SEARCH_ID = 1,
   CITY_SEARCH_NAME = 2,
   CITY_SEARCH_ZIP = 4,
   CITY_SEARCH_COORD = 8
} city_search_e;

typedef enum city_format {
   CITY_FORMAT_NONE = 0,
   CITY_FORMAT_LANG = 1,
   CITY_FORMAT_UNIT = 2
} city_format_e;

#endif
