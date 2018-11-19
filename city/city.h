#ifndef _CITYH_
#define _CITYH_

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <json-c/json.h>

#define MAX_ID_SIZE         8U
#define MAX_ZIPCODE_SIZE    16U
#define MAX_LATITUDE_SIZE   10U
#define MAX_LONGITUDE_SIZE  12U
#define MAX_CITY_NAME       32U
#define MAX_COUNTRY_CODE    2U
#define MAX_KEY_SIZE        32U

typedef struct url_sts_s {
    char key[MAX_KEY_SIZE + 1];
} url_sts;

typedef struct sus_sts_s {
    float rise;
    float set;
} sun_sts;

typedef struct weather_sts_s {
    float temp;
    float humidity;
    float temp_max;
    float temp_min;
    float pressure;
    char *description;
} weather_sts;

typedef struct location_sts_s {
   char id[MAX_ID_SIZE];
   char zipcode[MAX_ZIPCODE_SIZE];
   char latitude[MAX_LATITUDE_SIZE];
   char longitude[MAX_LONGITUDE_SIZE];
   char city[MAX_CITY_NAME];
   char country[MAX_COUNTRY_CODE];
} location_sts;

typedef struct map_sts_s {
    url_sts         url;
    sun_sts         sun;
    weather_sts     wthr;
    location_sts    locn;
} map_sts;

#endif
