#ifndef _CITY_CURLH_
#define _CITY_CURLH_

#include <stdbool.h>
#include <curl/curl.h>

#include "city.h"

typedef struct city_info_s {
    size_t size;
    char *data;
} city_info;

void destroyCity( void );
bool weatherURL(const url_sts *url);
const city_info getCityInfo( void );

#endif
