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

typedef struct url_config {
   char url[MAX_URL_LEN + 1];
   char key[MAX_KEY_LEN + 1];
   char search[MAX_LOCATION_LEN + 1];
   char format[MAX_FORMAT_LEN + 1];
} url_config_s;

typedef struct city_coord {
   float lon;
   float lat;
} city_coord_s;

typedef struct city_weather {
   int id;
   char main[MAX_WEATHER_PAR_LEN];
   char desc[MAX_WEATHER_DESC_LEN];
   char icon[MAX_ICON_ID_LEN];
} city_weather_s;

typedef struct city_main {
   float temp;
   float pressure;
   float humidity;
   float high;
   float low;
   float sea_level;
   float grnd_level;
} city_main_s;

typedef struct city_wind {
   float speed;
   float deg;
   float gust;
} city_wind_s;

typedef struct city_clouds {
   int all;
} city_cloud_s;

typedef struct city_rain {
   float hour_1;
   float hour_2;
} city_rain_s;

typedef struct city_snow {
   float hour_1;
   float hour_2;
} city_snow_s;

typedef struct city_sys {
   int id;
   int type;
   char message[MAX_MESSAGE_SIZE_LEN];
   char country[MAX_COUNTRY_CODE_LEN];
   char sunrise[MAX_SUN_RISE_SET_LEN];
   char sunset[MAX_SUN_RISE_SET_LEN];
} city_sys_s;

typedef struct city_misc {
   char base[MAX_BASE_PAR_LEN];
   float visibility;
   long dt;
   long id;
   char name[MAX_CITY_NAME_LEN];
   long cod;
} city_misc_s;

typedef struct city_map {
   city_coord_s    coord;
   city_weather_s  weather[MAX_WEATHER_TOTAL];
   city_main_s     main;
   city_wind_s     wind;
   city_cloud_s    cloud;
   city_rain_s     rain;
   city_snow_s     snow;
   city_sys_s      sys;
   city_misc_s     misc;
} city_map_s;

#endif
