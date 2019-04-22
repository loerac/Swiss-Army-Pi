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
 * See stocks_custom.h for description
 **********************************************/
bool stock_custom( void ) {
   bool ok = true;

   json_object *obj = json_object_from_file(STOCK_EXCHANGE_CUSTOM);
   if (NULL != obj) {
      json_object_object_foreach(obj, key, val) {
         if (0 == strncmp(key, "function", sizeof("function"))) {
            (void)strncpy(stocks_api.function, json_object_get_string(val), sizeof(stocks_api.function));
            printf("stocks_api.function = %s\n", stocks_api.function);
         } else if (0 == strncmp(key, "symbol", sizeof("symbol"))) {
            (void)strncpy(stocks_api.symbol, json_object_get_string(val), sizeof(stocks_api.symbol));
            printf("stocks_api.symbol = %s\n", stocks_api.symbol);
         } else if (0 == strncmp(key, "interval", sizeof("interval"))) {
            (void)strncpy(stocks_api.interval, json_object_get_string(val), sizeof(stocks_api.interval));
            printf("stocks_api.interval = %s\n", stocks_api.interval);
         } else if (0 == strncmp(key, "outputsize", sizeof("outputsize"))) {
            (void)strncpy(stocks_api.outputsize, json_object_get_string(val), sizeof(stocks_api.outputsize));
            printf("stocks_api.outputsize = %s\n", stocks_api.outputsize);
         } else {
            printf("ERR: '%s' is an invalid stock exchange customization file\n", STOCK_EXCHANGE_CUSTOM);
            ok = false;
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
   return true;
}
