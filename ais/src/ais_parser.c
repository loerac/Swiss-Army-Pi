#include "ais_parser.h"
#include "type_compat.h"
#include "time_compat.h"

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

#define NASA_OPERATION "/SAP/config/ais/nasa_operation.json"

static iss_now_s iss = {0};
static nasa_operation_s nasa = {0};
static nasa_apod_s apod = {0};

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
                  printf("NOTICE: Saving images to '%s'\n", nasa.save_location);
               } else {
                  printf("NOTICE: '%s', not a directory\n", path);
               }
            } else {
               printf("NOTICE: Getting '%s' information failed - %m\n", path);
            }
         } else if (0 == strncmp(key, "use_hd", sizeof("use_hd"))) {
            nasa.use_hd = json_object_get_boolean(val);
            printf("Use HD URL: '%s'\n", nasa.use_hd ? "TRUE":"FALSE");
         } else if (0 == strncmp(key, "key", sizeof("key"))) {
            istrncpy(nasa.key, json_object_get_string(val), sizeof(nasa.key));
            printf("NASA API key '%s'\n", nasa.key);
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

static bool nasaGetPicture(const char *const url) {
   bool ok = false;;
   char save[(PATH_MAX * 2) + 1U] = {0};
   isnprintf(save, sizeof(save), "%s%s.jpg", nasa.save_location, timecompactStringTimestamp("%Y%m%d"));

   ok = ftpDownloadImage(url, save);
   if (!ok) {
      printf("Saving image failed\n");
   }

   return ok;
}

static bool nasaStoreInfo(const char *const buff, size_t buff_len, char **info) {
   bool ok = false;

   (*info) = malloc(sizeof(char*) * buff_len);
   if (NULL != info) {
      ok = true;
      isnprintf((*info), buff_len, "%s%c", buff, '\0');
   } else {
      printf("EMERG: Allocating memory failed - %m\n");
   }

   return ok;
}

/* See iss_parser.h for description */
bool nasaParser(ftp_info_s ftp) {
   bool ok = true;

   if (  (!ftp.valid) || (ftp.data == NULL) ) {
      printf("NOTICE: No data available\n");
   } else {
      json_object *obj = json_tokener_parse(ftp.data);
      json_object_object_foreach(obj, key, val) {
         if (0 == strncmp(key, "date", sizeof("date"))) {
            istrncpy(apod.date, json_object_get_string(val), sizeof(apod.date));
         } else if (0 == strncmp(key, "explanation", sizeof("explanation"))) {
            ok = nasaStoreInfo(  json_object_get_string(val),
                                 strlen(json_object_get_string(val)) + 1U,
                                 &apod.explanation);
            printf("\tEXPLANATION: %s\n", apod.explanation);
         } else if (0 == strncmp(key, "hdurl", sizeof("hdurl"))) {
            ok = nasaStoreInfo(  json_object_get_string(val),
                                 strlen(json_object_get_string(val)) + 1U,
                                 &apod.hdurl);
            printf("\tHD URL: %s\n", apod.hdurl);
         } else if (0 == strncmp(key, "url", sizeof("url"))) {
            ok = nasaStoreInfo(  json_object_get_string(val),
                                 strlen(json_object_get_string(val)) + 1U,
                                 &apod.url);
            printf("\tURL: %s\n", apod.url);
         } else if (0 == strncmp(key, "title", sizeof("title"))) {
            ok = nasaStoreInfo(  json_object_get_string(val),
                                 strlen(json_object_get_string(val)) + 1U,
                                 &apod.title);
            printf("\tTITLE: %s\n", apod.title);
         } else if (0 == strncmp(key, "copyright", sizeof("copyright"))) {
            /* Unneeded information */
         } else if (0 == strncmp(key, "media_type", sizeof("media_type"))) {
            /* Unneeded information */
         } else if (0 == strncmp(key, "service_version", sizeof("service_version"))) {
            /* Unneeded information */
         } else {
            printf("Unknown key: %s\n", key);
            ok = false;
         }

         if (!ok) {
            break;
         }
      }
   }

   if (ok) {
      const char *const url = nasa.use_hd ? apod.hdurl:apod.url;
      ok = nasaGetPicture(url);
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
   bool ok = true;
   printf("Location = ");
   json_object_object_foreach(obj, key, val) {
      if (0 == strncmp(key, "latitude", sizeof("latitude"))) {
         const float lat = json_object_get_double(val);
         if ( (LATITUDE >= lat) && (TO_NEGATIVE(LATITUDE) <= lat) ) {
            iss.lat = lat;
            printf("LATITUDE(%f) ", iss.lat);
         } else {
            ok = false;
            printf("NOTICE: Invalid latitude value - (%f)\n", lat);
         }
      } else if (0 == strncmp(key, "longitude", sizeof("longitude"))) {
         const float lon = json_object_get_double(val);
         if ( (LONGITUDE >= lon) && (TO_NEGATIVE(LONGITUDE) <= lon) ) {
            iss.lon = lon;
            printf("LONGITUDE(%f) ", iss.lon);
         } else {
            ok = false;
            printf("NOTICE: Invalid longitude value - (%f)\n", lon);
         }
      } else {
         // Invalid item found in JSON file
         ok = false;
      }
   }

   return ok;
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
            char timestamp[100] = {0};
            if (-1 == timecompactUnixToDate(iss.timestamp, "%H:%M %a, %B %d", timestamp, sizeof(timestamp))) {
               ok = false;
               break;
            }
            printf("timestamp = %lu | %s\n", iss.timestamp, timestamp);
         } else if (0 == strncmp(key, "iss_position", sizeof("iss_position"))) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = issParserPosition(new_obj);
            printf("\n");
         } else {
            ok = false;
            break;
         }
      }
   }

   return ok;
}

/* See iss_parser.h for description */
char *nasaGetKey( void ) {
   return nasa.key;
}
