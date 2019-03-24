#ifndef _CITYH_
#define _CITYH_

#include <time.h>
#include <stdio.h>

/* City weather details */
#define MAX_WEATHER_PAR_LEN      128U
#define MAX_WEATHER_DESC_LEN     256U
#define MAX_ICON_ID_LEN          4U

/* City sytem */
#define MAX_MESSAGE_SIZE_LEN     16U
#define MAX_COUNTRY_CODE_LEN     2U
#define MAX_SUN_RISE_SET_LEN     8U

/* Number of weather types */
#define MAX_WEATHER_TOTAL        12u

/* URL string */
#define MAX_URL_LEN              48U
#define MAX_KEY_LEN              32U
#define MAX_LOCATION_LEN         512U
#define MAX_FORMAT_LEN           128U

/* Misc string */
#define MAX_BASE_PAR_LEN         64U
#define MAX_CITY_NAME_LEN        256U

typedef struct url_sts_s {
   char url[MAX_URL_LEN + 1];
   char key[MAX_KEY_LEN + 1];
   char search[MAX_LOCATION_LEN + 1];
   char format[MAX_FORMAT_LEN + 1];
} url_sts;

typedef struct city_coord_s {
   float lon;
   float lat;
} city_coord;

typedef struct city_weather_s {
   int id;
   char main[MAX_WEATHER_PAR_LEN];
   char desc[MAX_WEATHER_DESC_LEN];
   char icon[MAX_ICON_ID_LEN];
} city_weather;

typedef struct city_main_s {
   float temp;
   float pressure;
   float humidity;
   float high;
   float low;
   float sea_level;
   float grnd_level;
} city_main;

typedef struct city_wind_s {
   float speed;
   float deg;
   float gust;
} city_wind;

typedef struct city_clouds_s {
   int all;
} city_cloud;

typedef struct city_rain_s {
   float hour_1;
   float hour_2;
} city_rain;

typedef struct city_snow_s {
   float hour_1;
   float hour_2;
} city_snow;

typedef struct city_sys_s {
   int id;
   int type;
   char message[MAX_MESSAGE_SIZE_LEN];
   char country[MAX_COUNTRY_CODE_LEN];
   char sunrise[MAX_SUN_RISE_SET_LEN];
   char sunset[MAX_SUN_RISE_SET_LEN];
} city_sys;

typedef struct city_misc_s {
   char base[MAX_BASE_PAR_LEN];
   float visibility;
   long dt;
   long id;
   char name[MAX_CITY_NAME_LEN];
   long cod;
} city_misc;

typedef struct city_map_s {
   city_coord      coord;
   city_weather    weather[MAX_WEATHER_TOTAL];
   city_main       main;
   city_wind       wind;
   city_cloud      cloud;
   city_rain       rain;
   city_snow       snow;
   city_sys        sys;
   city_misc       misc;
} city_map;

#endif
