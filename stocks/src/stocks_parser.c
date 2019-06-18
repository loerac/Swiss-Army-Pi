#include "type_compat.h"
#include "stocks_parser.h"

#include <stdio.h>
#include <json-c/json.h>

static bool stocksParserMetadata(json_object *obj) {
   bool ok = true;
   json_object_object_foreach(obj, key, val) {
      // Ignore the leading numbering (i.e. '1. Information')
      if (strncmp(&key[2], "Information", sizeof("Information")) == 0) {
         printf("\tINFORMATION: %s\n", json_object_get_string(val));
      } else if (strncmp(&key[2], "Symbol", sizeof("Symbol")) == 0) {
         printf("\tSymbol: %s\n", json_object_get_string(val));
      } else if (strncmp(&key[2], "Last Refreshed", sizeof("Last Refreshed")) == 0) {
         printf("\tLast Refreshed: %s\n", json_object_get_string(val));
      } else if (strncmp(&key[2], "Interval", sizeof("Interval")) == 0) {
         printf("\tInterval: %s\n", json_object_get_string(val));
      } else if (strncmp(&key[2], "Output Size", sizeof("Output Size")) == 0) {
         printf("\tOutput Size: %s\n", json_object_get_string(val));
      } else if (strncmp(&key[2], "Time Zone", sizeof("Time Zone")) == 0) {
         printf("\tTime Zone: %s\n", json_object_get_string(val));
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

static bool stocksParserTimeSeries(json_object *obj) {
   return true;
}

bool stocksParser(ftp_info_s ftp) {
   bool ok = false;

   if (  (!ftp.valid) || (NULL == ftp.data) ) {
      printf("No data available\n");
   } else {
      json_object *obj = json_tokener_parse(ftp.data);
      json_object_object_foreach(obj, key, val) {
         if (0 == strncmp(key, "Meta Data", sizeof("Meta Data"))) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = stocksParserMetadata(new_obj);
         } else if (0 == strncmp(key, "Time Series", sizeof("Time Series") - 1)) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = stocksParserTimeSeries(new_obj);
         }
      }
   }

   return ok;
}

bool stocksParserAdjusted(ftp_info_s ftp) {
   bool ok = false;

   /*
   if (  (!ftp.valid) || (NULL == ftp.data) ) {
      printf("No data available\n");
   } else {
      json_object *obj = json_tokener_parse(ftp.data);
      json_object_object_foreach(obj, key, val) {
         if (strncmp(key, "coord", sizeof("coord")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonCoord(new_obj, map);
         }
      }
   }
   */

   return ok;
}

