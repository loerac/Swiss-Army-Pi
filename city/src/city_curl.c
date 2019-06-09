#include "slist.h"
#include "city_curl.h"
#include "city_types.h"
#include "city_custom.h"
#include "type_compat.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

static char url_str[512U] = {0};
static city_info_s city = { '\0' };

/**********************************************
 * INPUT:
 *    contents
 *       Data retreived from cURL operation
 *    size
 *       Alwasy one (1)
 *    nmemb
 *       Size of the 'contents'
 * OUTPUT:
 *    userp
 *       Pointer to where the data is being written
 * RETURN:
 *    Total size of the 'contents'
 * DESCRIPTION:
 *    Call back function from cURL:
 *       https://curl.haxx.se/libcurl/c/getinmemory.html
 *
 *    Shows how the write callback function can
 *    be used to download data into a chunk of
 *    memory instead of storing it in a file.
 **********************************************/
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
   size_t realsize = size * nmemb;
   city_info_s *tmp_city= (city_info_s *)userp;
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

/**********************************************
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    True if the URL is configured, else false.
 * DESCRIPTION:
 *    Configures the URL from the city.json and
 *    the url.json customization file.
 **********************************************/
static bool url_configuration(const url_config_s *url) {
   bool ok = false;
   slist_s *format_list = get_format();
   slist_s *search_list = get_location();

   if (NULL != search_list) {
      const city_search_s *search = (city_search_s *)search_list->data;
      char format_str[27] = {0};
      while (NULL != format_list) {
         const city_format_s *format = (city_format_s *)format_list->data;
         (void)strncat(format_str, format->data, format->size);
         format_list = slistNext(format_list);
      }
      isnprintf(url_str, sizeof(url_str), "%s%s%s&appid=%s%c", url->url, search->data, format_str, url->key, '\0');
      printf("URL STRING: %s\n", url_str);
      ok = true;
   }

   return ok;
}

/**********************************************
 * See city_curl.h for description.
 **********************************************/
bool weatherURL(const url_config_s *url) {
   CURLcode res;
   CURL *curl_handle;
   bool ok = false;

   /*
   * Configure the URL with the main URL,
   * search location type, any format type,
   * and the API key.
   */
   if (url_configuration(url)) {
      curl_global_init(CURL_GLOBAL_ALL);
      curl_handle = curl_easy_init();

      curl_easy_setopt(curl_handle, CURLOPT_URL, url_str);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&city);
      curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
      res = curl_easy_perform(curl_handle);

      if (CURLE_OK != res) {
         printf("ERR: curl_easy_perform() failed - error(%s)\n", curl_easy_strerror(res));
      } else {
         curl_easy_cleanup(curl_handle);
         ok = true;
      }
      city.valid = ok;
      curl_global_cleanup();
   }

   return ok;
}


/**********************************************
 * See city_curl.h for description.
 **********************************************/
const city_info_s getCityInfo( void ) {
   return city;
}

/**********************************************
 * See city_curl.h for description.
 **********************************************/
void destroyCity( void ) {
   if ( city.data[0] != '\0' ) {
      free(city.data);
   }
   city.size = 0;
   city.valid = false;
}
