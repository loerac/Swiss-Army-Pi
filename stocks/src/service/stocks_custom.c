#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

#include "stocks.h"
#include "stocks_custom.h"

#define STOCK_EXCHANGE_CUSTOM "/custom/stocks/stock_exchange.json"
#define STOCK_EXCHANGE_API    "/custom/stocks/stock_exchange_url.json"

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
 * See stocks_custom.h for description
 **********************************************/
bool stock_custom( void ) {
   bool ok = true;

   json_object *obj = json_object_from_file(STOCK_EXCHANGE_CUSTOM);
   if (NULL != obj) {
      json_object_object_foreach(obj, key, val) {
         if (0 == strncmp(key, "function", sizeof("function"))) {
            if (valid_call(json_object_get_string(val), sizeof(json_object_get_string(val)),
                           valid_function_calls, MAX_FUNCTION_CALL_LEN)) {
               (void)strncpy(stocks_api.function, json_object_get_string(val), sizeof(stocks_api.function));
               printf("stocks_api.function = %s\n", stocks_api.function);
            } else {
               goto invalid_custom;
            }
         } else if (0 == strncmp(key, "symbol", sizeof("symbol"))) {
            if (0 < strlen(json_object_get_string(val))) {
               (void)strncpy(stocks_api.symbol, json_object_get_string(val), sizeof(stocks_api.symbol));
               printf("stocks_api.symbol = %s\n", stocks_api.symbol);
            } else {
               goto invalid_custom;
            }
         } else if (0 == strncmp(key, "interval", sizeof("interval"))) {
            if (valid_call(json_object_get_string(val), sizeof(json_object_get_string(val)),
                           valid_interval_calls, MAX_INTERVAL_CALL_LEN)) {
               (void)strncpy(stocks_api.interval, json_object_get_string(val), sizeof(stocks_api.interval));
               printf("stocks_api.interval = %s\n", stocks_api.interval);
            } else {
               goto invalid_custom;
            }
         } else if (0 == strncmp(key, "outputsize", sizeof("outputsize"))) {
            if (valid_call(json_object_get_string(val), sizeof(json_object_get_string(val)),
                           valid_outputsize_calls, MAX_OUTPUTSIZE_CALL_LEN)) {
               (void)strncpy(stocks_api.outputsize, json_object_get_string(val), sizeof(stocks_api.outputsize));
               printf("stocks_api.outputsize = %s\n", stocks_api.outputsize);
            } else {
               goto invalid_custom;
            }
         } else {
invalid_custom:
            printf("ERR: '%s' is an invalid stock exchange customization file\n", STOCK_EXCHANGE_CUSTOM);
            ok = false;
            break;
         }
      }

      /* JSON is the only datatype that is being used */
      strncpy(stocks_api.datatype, "json", sizeof(stocks_api.datatype));
   } else {
      printf("NOTICE: Reading JSON '%s' failed - error(%m)\n", STOCK_EXCHANGE_CUSTOM);
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
            (void)strncpy(stocks_api.url, json_object_get_string(val), sizeof(stocks_api.url));
            printf("stocks_api.url = %s\n", stocks_api.url);
         } else if (0 == strncmp(key, "key", sizeof("key"))) {
            (void)strncpy(stocks_api.key, json_object_get_string(val), sizeof(stocks_api.key));
            printf("stocks_api.key = %s\n", stocks_api.key);
         } else {
            printf("ERR: '%s' is an invalid stock exchange customization file\n", STOCK_EXCHANGE_CUSTOM);
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
