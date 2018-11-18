#ifndef _CITY_CURLH_
#define _CITY_CURLH_

#define WEATHER_URL     "api.openweathermap.org/data/2.5/weather?"
#define WEATHER_KEY     "&appid="

#include "city.h"
#include <curl/curl.h>

typedef enum get_city_by_s{
    GET_BY_ID = 0,
    GET_BY_ZIPCODE,
    GET_BY_CITY,
    GET_BY_LATLOT,
    GET_FAILED
} get_city_by;

void destroyData();
bool curlCityInfo();
void setCitySts(map_sts m);
const size_t getCitySize();
const char *getCityData();

#endif
