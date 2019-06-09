#include <stdio.h>

#include "stocks.h"
#include "stocks_custom.h"

typedef enum stock_init_status {
   STOCK_OK = 0,
   STOCK_CUSTOM_INVALID,
   STOCK_URL_API_INVALID,
} stock_init_status_e;

int main(int argc, char *argv[]) {
   /*
   * TODO: The start-up needs to be done
   * 1.) Read JSON stock exchange file
   *  a.) Store the file in a struct
   *  b.) The struct will need:
   *     - function: time series
   *     - symbol: name of equity
   *     - interval: time interval
   * 2.) Read JSON user API URL and key file
   *  a.) Store the API usage in a struct
   *  b.) The struct will need:
   *     - API URL
   *     - API key
   * 3.) Connect to the API and parse
   */
   stock_init_status_e status = STOCK_OK;
   if (!stock_custom()) {
      status = STOCK_CUSTOM_INVALID;
   } else if (!stock_api_custom()) {
      status = STOCK_URL_API_INVALID;
   }

   return status;
}
