#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <json-c/json.h>

#include "city.h"
#include "city_custom.h"

static city_list_s *format_list = NULL;
static city_list_s *location_list = NULL;

static bool add_format(const char *const data, const int size, city_format_e format) {
   bool ok = false;
   city_format_s *const city = (city_format_s*)malloc(sizeof(city_format_s));

   if (NULL == city) {
      printf("EMERG: Allocating memory failed - error(%m)\n");
   } else {
      (void)strncpy(city->data, data, sizeof(city->data));
      city->size = size;
      city->format = format;
      format_list = city_list_add(format_list, city);
      ok = true;
   }

   return ok;
}

static bool add_location(const char *const data, const int size, city_search_e search) {
   bool ok = false;
   city_search_s *const city = (city_search_s*)malloc(sizeof(city_search_s));

   if (NULL == city) {
      printf("EMERG: Allocating memory failed - error(%m)\n");
   } else {
      (void)strncpy(city->data, data, sizeof(city->data));
      city->size = size;
      city->search = search;
      location_list = city_list_add(location_list, city);
      ok = true;
   }

   return ok;
}

static bool city_coords(json_object *jb, const char *const k) {
   bool ok = true;
   const char *lon = NULL;
   const char *lat = NULL;
   //char lon[MAX_COORDINATES_LEN] = {0};
   //char lat[MAX_COORDINATES_LEN] = {0};

   json_object *new_obj = json_object_object_get(jb, k);
   json_object_object_foreach(new_obj, key, val) {
      if (strncmp(key, "lon", sizeof("lon")) == 0) {
         lon = json_object_get_string(val);
      } else if (strncmp(key, "lat", sizeof("lat")) == 0) {
         lat = json_object_get_string(val);
      } else {
         ok = false;
         printf("Unknown key: %s\n", key);
      }
   }

   if ((NULL != lat) && (NULL != lon)) {
      char loc[MAX_LOCATION_LEN] = {0};
      const int size = snprintf(loc, sizeof(loc), "lat=%s&lon=%s%c", lat, lon, '\0');
      add_location(loc, size, CITY_SEARCH_COORD);
   }

   return ok;
}

static bool city_zone(json_object *jb, const char *const k, const char **zone, city_search_e *search) {
   bool ok = true;

   json_object *new_object = json_object_object_get(jb, k);
   json_object_object_foreach(new_object, key, val) {
      if (strncmp(key, "name", sizeof("name")) == 0) {
         *zone = json_object_get_string(val);
         *search = CITY_SEARCH_NAME;
      } else if (strncmp(key, "zipcode", sizeof("zipcode")) == 0) {
         *zone = json_object_get_string(val);
         *search = CITY_SEARCH_ZIP;
      } else {
         ok = false;
         printf("Unknown key: %s\n", key);
      }
   }

   return ok;
}

static bool city_region(json_object *jb, const char *const k) {
   bool ok = true;
   const char *zone = NULL;
   const char *country = NULL;
   city_search_e search = CITY_SEARCH_NONE;

   json_object *new_object = json_object_object_get(jb, k);
   json_object_object_foreach(new_object, key, val) {
      if (strncmp(key, "country", sizeof("country")) == 0) {
         country = json_object_get_string(val);
      } else if (strncmp(key, "city", sizeof("city")) == 0) {
         ok = city_zone(new_object, key, &zone, &search);
      } else {
         ok = false;
         printf("Unknown key: %s\n", key);
      }
   }

   if ((NULL != country) && (NULL != zone)) {
      char loc[MAX_LOCATION_LEN] = {0};
      const int size = snprintf(loc, sizeof(loc), "q=%s,%s%c", zone, country, '\0');
      add_location(loc, size, search);
   }

   return ok;
}

bool city_city_custom(const char *const json) {
   int size = 0;
   bool ok = false;
   char loc[MAX_LOCATION_LEN] = {0};
   char format[MAX_FORMAT_LEN] = {0};

   json_object *obj = json_object_from_file(json);
   if (NULL != obj) {
      json_object_object_foreach(obj, key, val) {
         if (strncmp(key, "id", sizeof("id")) == 0) {
            size = snprintf(loc, sizeof(loc), "id=%s%c", json_object_get_string(val), '\0');
            add_location(loc, size, CITY_SEARCH_ID);
         } else if (strncmp(key, "language", sizeof("language")) == 0) {
            size = snprintf(format, sizeof(format), "&lang=%s%c", json_object_get_string(val), '\0');
            add_format(format, size, CITY_FORMAT_LANG);
         } else if (strncmp(key, "units", sizeof("units")) == 0) {
            size = snprintf(format, sizeof(format), "&units=%s%c", json_object_get_string(val), '\0');
            add_format(format, size, CITY_FORMAT_UNIT);
         } else if (strncmp(key, "region", sizeof("region")) == 0) {
            ok = city_region(obj, key);
         } else if (strncmp(key, "coord", sizeof("coord")) == 0) {
            ok = city_coords(obj, key);
         } else {
            ok = true;
            printf("Unknown key: %s\n", key);
         }
      }
   } else {
      ok = false;
      printf("Reading JSON '%s' failed - (error=%m)\n", json);
   }

   return ok;
}

bool city_url_custom(const char *const json, url_config_s *url) {
   bool ok = true;

   json_object *obj = json_object_from_file(json);
   if (NULL != obj) {
      json_object_object_foreach(obj, key, val) {
         if (0 == strncmp(key, "url", sizeof("url"))) {
            (void)strncpy(url->url, json_object_get_string(val), sizeof(url->url));
         } else if (0 == strncmp(key, "key", sizeof("key"))) {
            (void)strncpy(url->key, json_object_get_string(val), sizeof(url->key));
         } else {
            ok = false;
            printf("Unknown key: %s\n", key);
         }
      }
   }

   return ok;
}

city_list_s *get_format( void ) {
   return format_list;
}

city_list_s *get_location( void ) {
   return location_list;
}
