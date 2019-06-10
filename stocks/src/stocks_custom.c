#include "slist.h"
#include "stocks.h"
#include "type_compat.h"
#include "stocks_custom.h"
#include "stocks_common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

#define STOCK_LOOKUP    "/custom/stocks/stock_lookup.json"
#define STOCK_OPERATION "/custom/stocks/stock_operation.json"

static slist_s *stock_list = NULL;

typedef bool (*stock_callback)(json_object*);
static stocks_operation_s stock_opers = {0};

/**
 * INPUT:
 *    obj - The current location of the JSON file
 * OUTPUT:
 *    obj - Point to the next location of the JSON file
 * RETURN:
 *    True if the parsing stock time series successed,
 *    Else, false.
 * DESCRIPTION:
 *    Parses the stock time series
 **/
static bool parse_stock_time_series(json_object *obj) {
   bool ok = true;

   printf("STOCK TIME SERIES:\n");
   json_object_object_foreach(obj, key, val) {
      if (0 == strncmp(key, "function", sizeof("function"))) {
         time_series_function_e function = tsFunction_stoi(json_object_get_string(val), strlen(json_object_get_string(val)));
         ok = (MAX_TIME_SERIES_FUNCTION != function);
         printf("\tFUNCTION: = %s\n", json_object_get_string(val));
      }  else if (0 == strncmp(key, "interval", sizeof("interval"))) {
         time_series_interval_e interval = tsInterval_stoi(json_object_get_string(val), strlen(json_object_get_string(val)));
         ok = (MAX_TIME_SERIES_INTERVAL != interval);
         printf("\tINTERVAL: = %s\n", json_object_get_string(val));
      } else if (0 == strncmp(key, "outputsize", sizeof("outputsize"))) {
         time_series_opsize_e opsize = tsOpsize_stoi(json_object_get_string(val), strlen(json_object_get_string(val)));
         ok = (MAX_TIME_SERIES_OPSIZE != opsize);
         printf("\tOUTPUTSIZE: = %s\n", json_object_get_string(val));
      } else if ( (0 == strncmp(key, "symbol", sizeof("symbol"))) &&
                  (0 < strlen(json_object_get_string(val))) ) {
         printf("\tSYMBOL: = %s\n", json_object_get_string(val));
      } else {
         // Invalid key found in JSON file
         ok = false;
      }

      if (! ok) {
         printf("ERR: '%s' is an invalid stock exchange customization file\n", STOCK_LOOKUP);
         break;
      }
   }

   return ok;
}

/**
 * INPUT:
 *    obj - The current location of the JSON file
 * OUTPUT:
 *    obj - Point to the next location of the JSON file
 * RETURN:
 *    True if the parsing foreign exchange successed,
 *    Else, false.
 * DESCRIPTION:
 *    Parses the foreign exchange
 **/
static bool parse_foreign_exchange(json_object *obj) {
   bool ok = true;

   /* TODO: Create common converter for the foreign exchange * /
   printf("FOREIGN EXCHANGE:\n");
   json_object_object_foreach(obj, key, val) {
      if ( (0 == strncmp(key, "function", sizeof("function"))) &&
           (valid_call(json_object_get_string(val), sizeof(json_object_get_string(val)),
                       valid_foreign_exchange_calls, MAX_FOREIGN_EXCHANGE_CALL)) ) {
         printf("\tFUNCTION: = %s\n", json_object_get_string(val));
      }
      / * TODO: Check if currency is valid * /
      else if (0 == strncmp(key, "from_currency", sizeof("from_currency"))) {
         printf("\tFROM_CURRENCY: = %s\n", json_object_get_string(val));
      } else if (0 == strncmp(key, "to_currency", sizeof("to_currency"))) {
         printf("\tTO_CURRENCY: = %s\n", json_object_get_string(val));
      } else if ( (0 == strncmp(key, "interval", sizeof("interval"))) &&
                  (valid_call(json_object_get_string(val), sizeof(json_object_get_string(val)),
                              valid_interval_calls, MAX_INTERVAL_CALL)) ) {
         printf("\tINTERVAL: = %s\n", json_object_get_string(val));
      } else if ( (0 == strncmp(key, "outputsize", sizeof("outputsize"))) &&
                  (valid_call(json_object_get_string(val), sizeof(json_object_get_string(val)),
                              valid_outputsize_calls, MAX_OUTPUTSIZE_CALL)) ) {
         printf("\tOUTPUTSIZE: = %s\n", json_object_get_string(val));
      } else {
         // Invalid key found in JSON file
         ok = false;
      }

      if (! ok) {
         printf("ERR: '%s' is an invalid stock exchange customization file\n", STOCK_LOOKUP);
         break;
      }
   }
   */

   return ok;
}

/**
 * INPUT:
 *    obj - The current location of the JSON file
 *    key - Key of the start of the array
 *    cb  - Callback function for stock time series or
 *          foreign exchange
 * OUTPUT:
 *    obj - Point to the next location of the JSON file
 * RETURN:
 *    True if the parsing callback functions successed,
 *    Else, false.
 * DESCRIPTION:
 *    Iterate through the array
 **/
static bool parse_lookup_callback(json_object *obj, const char * const key, stock_callback cb) {
   bool ok = true;
   json_object *json_array = obj;
   if (NULL != key) {
      json_array = json_object_object_get(obj, key);
      const unsigned int array_len = json_object_array_length(json_array);
      json_object *json_value = NULL;

      for (size_t i = 0; ( (ok) && (i < array_len) ); i++) {
         json_value = json_object_array_get_idx(json_array, i);
         if (json_type_null != json_object_get_type(json_value)) {
            ok = cb(json_value);
         }
      }
   } else {
      ok = false;
      printf("NOTICE: Missing key\n");
   }

   return ok;
}

/* See stocks_custom.h for description */
bool stock_custom( void ) {
   bool ok = true;

   json_object *obj = json_object_from_file(STOCK_LOOKUP);
   if (NULL != obj) {
      json_object_object_foreach(obj, key, val) {
         if (0 == strncmp(key, "stock_time_series", sizeof("stock_time_series")) ) {
            ok = parse_lookup_callback(obj, key, parse_stock_time_series);
            if (!ok) { break; }
         } else if (0 == strncmp(key, "foreign_exchange", sizeof("foreign_exchange")) ) {
            ok = parse_lookup_callback(obj, key, parse_foreign_exchange);
            if (!ok) { break; }
         } else if ( (0 == strncmp(key, "sector_performance", sizeof("sector_performance"))) &&
                     (0 == strncmp(json_object_get_string(val), "SECTOR", sizeof("SECTOR"))) ) {
            printf("SECTOR:\n");
            printf("\tFUNCTION: %s\n", json_object_get_string(val));
         } else {
            printf("ERR: '%s' is an invalid stock exchange customization file\n", STOCK_LOOKUP);
            ok = false;
            break;
         }
      }

      /* JSON is the only datatype that is being used */
      //strncpy(stock_opers.datatype, "json", sizeof(stock_opers.datatype));
   } else {
      printf("NOTICE: Reading JSON '%s' failed - error(%m)\n", STOCK_LOOKUP);
      ok = false;
   }

   return ok;
}

/* See stocks_custom.h for description */
bool stock_api_custom( void ) {
   bool ok = true;

   json_object *obj = json_object_from_file(STOCK_OPERATION);
   if (NULL != obj) {
      json_object_object_foreach(obj, key, val) {
         if (0 == strncmp(key, "url", sizeof("url"))) {
            istrncpy(stock_opers.url, json_object_get_string(val), sizeof(stock_opers.url));
            printf("stock_opers.url = %s\n", stock_opers.url);
         } else if (0 == strncmp(key, "key", sizeof("key"))) {
            istrncpy(stock_opers.key, json_object_get_string(val), sizeof(stock_opers.key));
            printf("stock_opers.key = %s\n", stock_opers.key);
         } else if (0 == strncmp(key, "time_interval", sizeof("udpate_timer"))) {
            stock_opers.time_interval = json_object_get_int(val);
            printf("stock_opers.time_interval = %d\n", stock_opers.time_interval);
         } else {
            printf("ERR: '%s' is an invalid stock exchange customization file\n", STOCK_LOOKUP);
            ok = false;
            break;
         }
      }
   } else {
      printf("NOTICE: Reading JSON '%s' failed - error(%m)\n", STOCK_OPERATION);
      ok = false;
   }

   return ok;
}
