#include "slist.h"
#include "aqap_config.h"
#include "type_compat.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <json-c/json.h>

#define AQAP_CONFIG    "/SAP/config/aqap_config.json"
#define AQAP_CUSTOM    "/SAP/custom/aqap_custom.json"

typedef struct aqap {
    char key[];
    slist_s *cities;
} aqap_s;

/**
 * INPUT:
 *    obj - JSON object
 *    key - Key value of the JSON file
 * OUTPUT:
 *    NONE
 * RETURN:
 *    true if able to get the city names successfully, else flase
 * DESCRIPTION:
 *    Iterate through the city names array
 **/
static bool aqap_config_city(json_object *obj, const char * const key) {
   bool ok = true;
   json_object *json_array = obj;
   if (NULL != key) {
      json_array = json_object_object_get(obj, key);
      const unsigned int array_len = json_object_array_length(json_array);
      json_object *arr_obj = NULL;

      for (size_t i = 0; ( (ok) && (i < array_len) ); i++) {
         arr_obj = json_object_array_get_idx(json_array, i);
         if (json_type_null != json_object_get_type(arr_obj)) {

            json_object_object_foreach(arr_obj, arr_key, arr_val) {
                if (0 == strncmp(arr_key, "name", sizeof("name"))) {
                    printf("%s: %s\n", arr_key, json_object_get_string(arr_val));
                    //aqap_s.cities = slistPrepend(aqap.cities, json_object_get_string(arr_val));
                } else {
                    ok = false;
                }
            }

         }
      }
   } else {
      ok = false;
      printf("NOTICE: Missing key\n");
   }

   return ok;
}

/* See aqicn_config.h for description */
bool aqap_config_init( void ) {
   bool ok = true;

   json_object *obj = json_object_from_file(AQICN_CONFIG);
   if (NULL != obj) {
      json_object_object_foreach(obj, key, val) {
         if (0 == strncmp(key, "key", sizeof("key"))) {
            printf("%s: %s", key, json_object_get_string(val));
         } else if (0 == strncmp(key, "city", sizeof("city"))) {
            ok = aqap_config_city(obj, key);
         } else {
            ok = false;
            printf("NOTICE: Unknown key: %s\n", key);
         }
      }
   } else {
      ok = false;
      printf("NOTICE: Reading JSON '%s' failed - %m\n", AQICN_CONFIG);
   }

   return ok;
}

/* See aqicn_config.h for description */
char *aqap_get_key( void ) {
    return aqap_s.key;
}

/* See aqicn_config.h for description */
slist_s *aqap_get_list( void ) {
    return aqap_s.cities;
}

