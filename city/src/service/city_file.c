#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <json-c/json.h>

#include "city.h"
#include "city_file.h"
#include "city_types.h"

bool city_coords(json_object *jb, const char *const k) {
   bool ok = true;
   json_object *new_obj = json_object_object_get(jb, k);
   json_object_object_foreach(new_obj, key, val) {
      if (strncmp(key, "lon", sizeof("lon")) == 0) {
         //(void)strncpy(u->lon, json_object_get_string(val), MAX_LAT_LON);
         printf("LON: %s\n", json_object_get_string(val));
      } else if (strncmp(key, "lat", sizeof("lat")) == 0) {
         //(void)strncpy(u->lat, json_object_get_string(val), MAX_LAT_LON);
         printf("LAT: %s\n", json_object_get_string(val));
      } else {
         ok = false;
         printf("Unknown key: %s\n", key);
      }
   }
   return ok;
}

bool city_zone(json_object *jb, const char *const k) {
   bool ok = true;
   json_object *new_object = json_object_object_get(jb, k);
   json_object_object_foreach(new_object, key, val) {
      if (strncmp(key, "name", sizeof("name")) == 0) {
         //(void)strncpy(u->city, json_object_get_string(val), MAX_CITY_NAME);
         printf("NAME: %s\n", json_object_get_string(val));
      } else if (strncmp(key, "zipcode", sizeof("zipcode")) == 0) {
         //(void)strncpy(u->zip, json_object_get_string(val), MAX_ZIPCODE);
         printf("ZIP: %s\n", json_object_get_string(val));
      } else {
         ok = false;
         printf("Unknown key: %s\n", key);
      }
   }
   return ok;
}

bool city_region(json_object *jb, const char *const k) {
   bool ok = true;
   json_object *new_object = json_object_object_get(jb, k);
   json_object_object_foreach(new_object, key, val) {
      if (strncmp(key, "country", sizeof("country")) == 0) {
         //(void)strncpy(u->country, json_object_get_string(val), MAX_COUNTRY_CODE);
         printf("COUNTRY: %s\n", json_object_get_string(val));
      } else if (strncmp(key, "city", sizeof("city")) == 0) {
         ok = city_zone(new_object, key);
      } else {
         ok = false;
         printf("Unknown key: %s\n", key);
      }
   }
   return ok;
}

bool city_config(const char *const json) {
   bool ok = true;
   json_object *obj = json_object_from_file(json);
   if (NULL != obj) {
      json_object_object_foreach(obj, key, val) {
         if (strncmp(key, "id", sizeof("id")) == 0) {
            //(void)strncpy(u->id, json_object_get_string(val), MAX_ID);
            printf("ID: %s\n", json_object_get_string(val));
         } else if (strncmp(key, "language", sizeof("language")) == 0) {
            //(void)strncpy(u->lang, json_object_get_string(val), MAX_LANG_SIZE);
            printf("LANG: %s\n", json_object_get_string(val));
         } else if (strncmp(key, "units", sizeof("units")) == 0) {
            //(void)strncpy(u->unit, json_object_get_string(val), MAX_UNIT_FORMAT);
            printf("UNITS: %s\n", json_object_get_string(val));
         } else if (strncmp(key, "region", sizeof("region")) == 0) {
            ok = city_region(obj, key);
         } else if (strncmp(key, "coord", sizeof("coord")) == 0) {
            ok = city_coords(obj, key);
         } else {
            ok = false;
            printf("Unknown key: %s\n", key);
         }
      }
   } else {
      ok = false;
      printf("Reading JSON '%s' failed - (error=%m)\n", json);
   }
   return ok;
}
