#ifndef _CITY_FILEH_
#define _CITY_FILEH_

#include <stdbool.h>

#include "city_types.h"

typedef struct city_search_s {
    size_t size;
    const char *data;
    city_search_e search;
} city_search_s;

typedef struct city_format_s {
    size_t size;
    const char *data;
    city_format_e format;
} city_format_s;

bool city_custom(const char *const json);

#endif
