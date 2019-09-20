#include "ftp.h"
#include "stocks.h"
#include "net_api.h"
#include "type_compat.h"
#include "stocks_parser.h"
#include "stocks_custom.h"

#include <stdio.h>

static ftp_info_s ftp = {0};
static char url_str[512U] = {0};

typedef enum stock_init_status {
   STOCK_OK = 0,
   STOCK_CUSTOM_INVALID,
   STOCK_URL_API_INVALID,
} stock_init_status_e;

bool urlConfiguration( void ) {
   bool ok = true;
   static slist_s *equity = NULL;
   const stocks_operation_s oper = getAPIOperation();

   if (NULL == equity) {
      equity = getStockEquity();
   }

   if (NULL != equity) {
      const time_series_s *ts = (time_series_s *)equity->data;

      if (TS_INTRADAY == ts->function) {
         if (MAX_TIME_SERIES_INTERVAL != ts->interval) {
            isnprintf(url_str, sizeof(url_str), "%sfunction=%s&symbol=%s&interval=%s&outputsize=%s&apikey=%s",
               getAPIURL(),
               tsFunction_itos(ts->function),
               ts->symbol,
               tsInterval_itos(ts->interval),
               tsOpsize_btos(ts->is_compact),
               oper.key);
            printf("URL STRING: %s\n", url_str);
         } else {
            ok = false;
            printf("Function INTRADAY requires the INTERVAL commmand\n");
         }
      } else {
         isnprintf(url_str, sizeof(url_str), "%sfunction=%s&symbol=%s&outputsize=%s&apikey=%s",
            getAPIURL(),
            tsFunction_itos(ts->function),
            ts->symbol,
            tsOpsize_btos(ts->is_compact),
            oper.key);
         printf("URL STRING: %s\n", url_str);
      }
      equity = slistNext(equity);
      ok = ftpGet(&ftp, url_str);
      if (NULL == equity) {
         ok = false;
      }
   } else {
      ok = false;
   }

   return ok;
}

int main(int argc, char *argv[]) {
   stock_init_status_e status = STOCK_OK;
   if (!stock_custom()) {
      status = STOCK_CUSTOM_INVALID;
   } else if (!stock_api_custom()) {
      status = STOCK_URL_API_INVALID;
   } else if (netInit() && internetAvail()) {
      while (urlConfiguration()) {
         stocksParser(ftp);
      }
   }

   return status;
}
