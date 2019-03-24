#ifndef _CITY_FILEH_
#define _CITY_FILEH_

#include <stdbool.h>

#include "city_list.h"
#include "city_types.h"

typedef struct city_search_s {
    size_t size;
    char data[MAX_FORMAT_LEN];
    city_search_e search;
} city_search_s;

typedef struct city_format_s {
    size_t size;
    char data[MAX_LOCATION_LEN];
    city_format_e format;
} city_format_s;

bool city_custom(const char *const json);
city_list_s *get_format( void );
city_list_s *get_location( void );

#endif
