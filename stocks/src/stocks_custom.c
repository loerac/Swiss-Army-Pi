#include "slist.h"
#include "stocks.h"
#include "type_compat.h"
#include "stocks_custom.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

#define STOCK_LOOKUP       "/custom/stocks/stock_lookup.json"
#define STOCK_EXCHANGE_API "/custom/stocks/stock_exchange_url.json"

#define MAX_OUTPUTSIZE_CALL         2U
#define MAX_INTERVAL_CALL           5U
#define MAX_FOREIGN_EXCHANGE_CALL   5U
#define MAX_TIME_SERIES_CALL        8U

static slist_s *stock_list = NULL;

const char *const valid_outputsize_calls[MAX_OUTPUTSIZE_CALL] = {
   "compact",
   "full"
};

const char *const valid_interval_calls[MAX_INTERVAL_CALL] = {
   "1min",
   "5min",
   "15min",
   "30min",
   "60min"
};

const char *const valid_time_series_calls[MAX_TIME_SERIES_CALL] = {
   "TIME_SERIES_INTRADAY",
   "TIME_SERIES_DAILY",
   "TIME_SERIES_DAILY_ADJUSTED",
   "TIME_SERIES_WEEKLY",
   "TIME_SERIES_WEEKLY_ADJUSTED",
   "TIME_SERIES_MONTH",
   "TIME_SERIES_MONTH_ADJUSTED",
   "GLOBAL_QUOTE"
};

const char *const valid_foreign_exchange_calls[MAX_FOREIGN_EXCHANGE_CALL] = {
   "CURRENCY_EXCHANGE_RATE",
   "FX_INTRADAY",
   "FX_DAILY",
   "FX_WEEKLY",
   "FX_MONTHLY"
};

typedef bool (*stock_callback)(json_object*);
static stocks_api_parameters_s stocks_api = {0};

/**********************************************
 * INPUT:
 *    user_call - Call the user is using
 *    user_call_size - Size of 'user_call'
 *    calls - Valid calls the user can use
 *    calls_size - Size of the valid calls
 * OUTPUT:
 *    NONE
 * RETURN:
 *    True if the users call is valid, else false.
 * DESCRIPTION:
 *    Validates the users call for the API
 **********************************************/
static bool valid_call( const char *const user_call, const unsigned int user_call_size,
                        const char *const *const calls, const unsigned int calls_size) {
   bool found = false;
   unsigned int i = 0;

   while ( (!found) && (i < calls_size) ) {
      if (0 == strncmp(user_call, calls[i], user_call_size)) {
         found = true;
      }
      i++;
   }

   return found;
}

/**********************************************
 * INPUT:
 *    obj - The current location of the JSON file
 * OUTPUT:
 *    obj - Point to the next location of the JSON file
 * RETURN:
 *    True if the parsing stock time series successed,
 *    Else, false.
 * DESCRIPTION:
 *    Parses the stock time series
 **********************************************/
static bool parse_stock_time_series(json_object *obj) {
   bool ok = true;

   printf("STOCK TIME SERIES:\n");
   json_object_object_foreach(obj, key, val) {
      if ( (0 == strncmp(key, "function", sizeof("function"))) &&
           (valid_call(json_object_get_string(val), sizeof(json_object_get_string(val)),
                       valid_time_series_calls, MAX_TIME_SERIES_CALL)) ) {
         printf("\tFUNCTION: = %s\n", json_object_get_string(val));
      } else if ( (0 == strncmp(key, "symbol", sizeof("symbol"))) &&
                  (0 < strlen(json_object_get_string(val))) ) {
         printf("\tSYMBOL: = %s\n", json_object_get_string(val));
      } else if ( (0 == strncmp(key, "interval", sizeof("interval"))) &&
                  (valid_call(json_object_get_string(val), sizeof(json_object_get_string(val)),
                              valid_interval_calls, MAX_INTERVAL_CALL)) ) {
         printf("\tINTERVAL: = %s\n", json_object_get_string(val));
      } else if ( (0 == strncmp(key, "outputsize", sizeof("outputsize"))) &&
                  (valid_call(json_object_get_string(val), sizeof(json_object_get_string(val)),
                              valid_outputsize_calls, MAX_OUTPUTSIZE_CALL)) ) {
         printf("\tOUTPUTSIZE: = %s\n", json_object_get_string(val));
      } else {
         printf("ERR: '%s' is an invalid stock exchange customization file\n", STOCK_LOOKUP);
         ok = false;
         break;
      }
   }

   return ok;
}

/**********************************************
 * INPUT:
 *    obj - The current location of the JSON file
 * OUTPUT:
 *    obj - Point to the next location of the JSON file
 * RETURN:
 *    True if the parsing foreign exchange successed,
 *    Else, false.
 * DESCRIPTION:
 *    Parses the foreign exchange
 **********************************************/
static bool parse_foreign_exchange(json_object *obj) {
   bool ok = true;

   printf("FOREIGN EXCHANGE:\n");
   json_object_object_foreach(obj, key, val) {
      if ( (0 == strncmp(key, "function", sizeof("function"))) &&
           (valid_call(json_object_get_string(val), sizeof(json_object_get_string(val)),
                       valid_foreign_exchange_calls, MAX_FOREIGN_EXCHANGE_CALL)) ) {
         printf("\tFUNCTION: = %s\n", json_object_get_string(val));
      }
      /* TODO: Check if currency is valid */
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
         printf("ERR: '%s' is an invalid stock exchange customization file\n", STOCK_LOOKUP);
         ok = false;
         break;
      }
   }

   return ok;
}

/**********************************************
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
 **********************************************/
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

/**********************************************
 * See stocks_custom.h for description
 **********************************************/
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
      strncpy(stocks_api.datatype, "json", sizeof(stocks_api.datatype));
   } else {
      printf("NOTICE: Reading JSON '%s' failed - error(%m)\n", STOCK_LOOKUP);
      ok = false;
   }

   return ok;
}

/**********************************************
 * See stocks_custom.h for description
 **********************************************/
bool stock_api_custom( void ) {
   bool ok = true;

   json_object *obj = json_object_from_file(STOCK_EXCHANGE_API);
   if (NULL != obj) {
      json_object_object_foreach(obj, key, val) {
         if (0 == strncmp(key, "url", sizeof("url"))) {
            istrncpy(stocks_api.url, json_object_get_string(val), sizeof(stocks_api.url));
            printf("stocks_api.url = %s\n", stocks_api.url);
         } else if (0 == strncmp(key, "key", sizeof("key"))) {
            istrncpy(stocks_api.key, json_object_get_string(val), sizeof(stocks_api.key));
            printf("stocks_api.key = %s\n", stocks_api.key);
         } else {
            printf("ERR: '%s' is an invalid stock exchange customization file\n", STOCK_LOOKUP);
            ok = false;
            break;
         }
      }
   } else {
      printf("NOTICE: Reading JSON '%s' failed - error(%m)\n", STOCK_EXCHANGE_API);
      ok = false;
   }

   return ok;
}
