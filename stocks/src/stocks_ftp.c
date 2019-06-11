#include "slist.h"
#include "stocks_ftp.h"
#include "type_compat.h"
#include "stocks_custom.h"
#include "stocks_common.h"

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

static char url_str[512U] = {0};
static stock_info_s info = {0};

/**
 * INPUT:
 *    contents - data retreived from cURL operation
 *    size - alwasy one (1)
 *    nmemb - size of 'contents'
 * OUTPUT:
 *    userp - pointer to where the data is being written
 * RETURN:
 *    Total size of 'contents'
 * DESCRIPTON:
 *    Writes the downloaded data into a chunck of memory
 *    that was retrieved from cURL
 *
 *    Callback function from cURL:
 *       https://curl.haxx.se/libcurl/c/getinmemory.html
 **/
static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
   size_t realsize = size * nmemb;
   stock_info_s *tmp = (stock_info_s *)userp;
   char *ptr = realloc(tmp->data, tmp->size + realsize + 1);

   if (NULL != ptr) {
      tmp->data = ptr;
      imemcpy( &(tmp->data[tmp->size]), contents, realsize);
      tmp->size += realsize;
      tmp->data[tmp->size] = '\0';
   } else {
      printf("EMERG: Re-allacting memory failed - %m\n");
   }

   return realsize;
}

/**
 * INPUT:
 *    url - Alpha Vantage URL information
 * RETURN:
 *    True if the URL is configure,
 *    Else false
 * DESCRIPTION:
 *    Configures the Alpha Vantage API URL
 **/
static bool urlConfiguration( void ) {
   bool ok = true;
   static slist_s *equity = NULL;
   const stocks_operation_s oper = getAPIOperation();

   if (NULL == equity) {
      equity = getStockEquity();
   }

   if (NULL != equity) {
      const time_series_s *ts = (time_series_s *)equity->data;

      if (TIME_SERIES_INTRADAY == ts->function) {
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
      if (NULL == equity) {
         ok = false;
      }
   } else {
      ok = false;
   }

   return ok;
}

bool stockEquityURL( void ) {
   CURLcode res;
   CURL *curl_handle;
   bool ok = true;

   // Initialize cURL
   curl_global_init(CURL_GLOBAL_ALL);

   /*
    * Configure the URL with the main API URL,
    * function type, symbol, outputsize, and key.
    */
   while (urlConfiguration()) {
      // TODO: Figure out how to do this, all stock equity
      curl_handle = curl_easy_init();
      curl_easy_setopt(curl_handle, CURLOPT_URL, url_str);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeCallback);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&info);
      curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
      res = curl_easy_perform(curl_handle);

      if (CURLE_OK != res) {
         ok = false;
         printf("ERR: curl_easy_perform(%s)\n", curl_easy_strerror(res));
      } else {
         printf("ts.data(%s)\n", info.data);
         destroyEquity();
         curl_easy_cleanup(curl_handle);
      }
      curl_global_cleanup();
   }

   return ok;
}

void destroyEquity( void ) {
   if ('\0' != info.data[0]) {
      free(info.data);
   }
   info.data = NULL;
   info.size = 0;
}
