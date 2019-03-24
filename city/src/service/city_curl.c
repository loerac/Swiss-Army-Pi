#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "city_curl.h"
#include "city_types.h"
#include "city_custom.h"

static char url_str[512U] = {0};
static city_info city = { '\0' };

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
   size_t realsize = size * nmemb;
   city_info *tmp_city= (city_info *)userp;
   char *ptr = realloc(tmp_city->data, tmp_city->size + realsize + 1);

   if (NULL == ptr) {
      printf("EMERG: Re-allocating memory failed - error(%m)\n");
      realsize = 0;
   } else {
      tmp_city->data = ptr;
      memcpy(&(tmp_city->data[tmp_city->size]), contents, realsize);
      tmp_city->size += realsize;
      tmp_city->data[tmp_city->size] = 0;
   }

   return realsize;
}

static bool url_configuration(const url_sts *url) {
   bool ok = false;
   city_list_s *format_list = get_format();
   city_list_s *search_list = get_location();

   if (NULL != search_list) {
      const city_search_s *search = (city_search_s *)search_list->data;
      char format_str[27] = {0};
      while (NULL != format_list) {
         const city_format_s *format = (city_format_s *)format_list->data;
         (void)strncat(format_str, format->data, format->size);
         format_list = city_list_next(format_list);
      }
      (void)snprintf(url_str, sizeof(url_str), "%s%s%s&appid=%s%c", url->url, search->data, format_str, url->key, '\0');
      printf("url_str: %s\n", url_str);
      ok = true;
   }

   return ok;
}

bool weatherURL(const url_sts *url) {
   CURLcode res;
   CURL *curl_handle;
   bool ok = false;

   /*
   * Configure the URL with the main URL,
   * search location type, any format type,
   * and the API key.
   */
   url_configuration(url);

   curl_global_init(CURL_GLOBAL_ALL);
   curl_handle = curl_easy_init();

   curl_easy_setopt(curl_handle, CURLOPT_URL, url_str);
   curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
   curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&city);
   curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
   res = curl_easy_perform(curl_handle);

   if (res != CURLE_OK) {
      printf("ERR: curl_easy_perform() failed - error(%s)\n", curl_easy_strerror(res));
   } else {
      curl_easy_cleanup(curl_handle);
      ok = true;
   }

   curl_global_cleanup();

   return ok;
}


const city_info getCityInfo( void ) {
   return city;
}

void destroyCity( void ) {
   city.size = 0;
   if ( city.data[0] != '\0' ) {
      free(city.data);
   }
}
