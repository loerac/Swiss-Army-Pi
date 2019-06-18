#include "ais_parser.h"
#include "type_compat.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <json-c/json.h>

#define LONGITUDE 180
#define LATITUDE  85
#define TO_NEGATIVE(val)   0 - val

#define NASA_OPERATION "/SAP/conf/ais/nasa_operation.json"

static iss_now_s iss = {0};
static nasa_operation_s nasa = {0};

/* See iss_parser.h for description */
bool nasaConf( void ) {
   bool ok = true;

   json_object *obj = json_object_from_file(NASA_OPERATION);
   if (NULL != obj) {
      json_object_object_foreach(obj, key, val) {
         if (0 == strncmp(key, "save_location", sizeof("save_location"))) {
            char path[PATH_MAX + 1U] = {0};
            istrncpy(path, json_object_get_string(val), sizeof(path));
            struct stat info = {0};
            if (0 == stat(path, &info)) {
               if (S_ISDIR(info.st_mode)) {
                  istrncpy(nasa.save_location, path, sizeof(nasa.save_location));
                  printf("Saving images to '%s'\n", nasa.save_location);
               } else {
                  printf("NOTICE: '%s', not a directory\n", path);
               }
            } else {
               printf("NOTICE: Getting '%s' information failed - %m\n", path);
            }
         } else {
            ok = false;
            printf("NOTICE: Unknown key: %s\n", key);
         }
      }
   } else {
      ok = false;
      printf("NOTICE: Reading JSON '%s' failed - %m\n", NASA_OPERATION);
   }

   return ok;
}

/**
 * INPUT:
 *    obj - Current location of the JSON file
 * OUTPUT:
 *    obj - Point to the next location of the JSON file
 * RETURN:
 *    True if the latitude and longitude are valid,
 *    Else, false
 * DESCRIPTION:
 *    Parse the position of the station
 **/
static bool issParserPosition(json_object *obj) {
   int retval = 0;

   printf("Location = ");
   json_object_object_foreach(obj, key, val) {
      if (0 == strncmp(key, "latitude", sizeof("latitude"))) {
         const float lat = json_object_get_double(val);
         if ( (LATITUDE >= lat) && (TO_NEGATIVE(LATITUDE) <= lat) ) {
            iss.lat = lat;
            printf("LATITUDE(%f) ", iss.lat);
         } else {
            retval |= 1;
            printf("NOTICE: Invalid latitude value - (%f)\n", lat);
         }
      } else if (0 == strncmp(key, "longitude", sizeof("longitude"))) {
         const float lon = json_object_get_double(val);
         if ( (LONGITUDE >= lon) && (TO_NEGATIVE(LONGITUDE) <= lon) ) {
            iss.lon = lon;
            printf("LONGITUDE(%f) ", iss.lon);
         } else {
            retval |= 2;
            printf("NOTICE: Invalid longitude value - (%f)\n", lon);
         }
      } else {
         // Invalid item found in JSON file
         retval |= 4;
      }
   }

   return (0 == retval);
}

/* See iss_parser.h for description */
bool issParser(ftp_info_s ftp) {
   bool ok = true;

   if (  (!ftp.valid) || (ftp.data == NULL) ) {
      printf("NOTICE: No data available\n");
   } else {
      json_object *obj = json_tokener_parse(ftp.data);
      json_object_object_foreach(obj, key, val) {
         if (0 == strncmp(key, "message", sizeof("message"))) {
            printf("message = %s\n", json_object_get_string(val));
         } else if (0 == strncmp(key, "timestamp", sizeof("timestamp"))) {
            iss.timestamp = (unsigned)json_object_get_int64(val);
            printf("timestamp = %lu\n", iss.timestamp);
         } else if (0 == strncmp(key, "iss_position", sizeof("iss_position"))) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = issParserPosition(new_obj);
         } else {
            ok = false;
            break;
         }
      }
   }

   return ok;
}

