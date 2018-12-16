#ifndef _CITY_CURLH_
#define _CITY_CURLH_

#include <stdbool.h>
#include <curl/curl.h>

#include "city.h"

typedef enum get_city_by_s{
    GET_BY_ID = 0,
    GET_BY_ZIPCODE,
    GET_BY_CITY,
    GET_BY_LATLOT,
    GET_FAILED
} get_city_by;

typedef struct city_info_s {
    size_t size;
    char *data;
} city_info;

void destroyCity( void );
bool weatherURL(const url_sts *url);
const city_info getCityInfo( void );

#endif
