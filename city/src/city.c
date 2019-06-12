#include <string.h>
#include <stdbool.h>

#include "ftp.h"
#include "city.h"
#include "city_ftp.h"
#include "city_parse.h"
#include "city_types.h"
#include "city_custom.h"
#include "type_compat.h"
#include "net_api.h"

#define CITY_CUSTOM     "/SAP/custom/city/city.json"
#define CITY_OPERATION  "/SAP/conf/city/city_operation.json"

static ftp_info_s *ftp = NULL;
static city_operation_s oper = {0};
static city_map_s map = {0};
static char url_str[512U] = {0};

/**********************************************
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    True if the URL is configured, else false.
 * DESCRIPTION:
 *    Configures the URL from the city.json and
 *    the city_operation.json customization file.
 **********************************************/
static bool urlConfiguration(const city_operation_s *oper) {
   bool ok = false;
   slist_s *format_list = get_format();
   slist_s *search_list = get_location();

   if (NULL != search_list) {
      const city_search_s *search = (city_search_s *)search_list->data;
      char format_str[27] = {0};
      while (NULL != format_list) {
         const city_format_s *format = (city_format_s *)format_list->data;
         istrncat(format_str, format->data, format->size);
         format_list = slistNext(format_list);
      }
      isnprintf(url_str, sizeof(url_str), "%s%s%s&appid=%s%c", oper->url, search->data, format_str, oper->key, '\0');
      printf("URL STRING: %s\n", url_str);
      ok = ftpGet(&ftp, url_str);
   }

   return ok;
}

/**********************************************
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    CITY_OK on success.
 *    On failure, the status is set
 * DESCRIPTION:
 *    Configures the URL and MAP of the city.
 **********************************************/
city_init_e cityInit( void ) {
   city_init_e status = CITY_OK;

   if (netInit() && internetAvail()) {
      status = (city_city_custom(CITY_CUSTOM)) ? CITY_OK : CITY_CITY_CUSTOM;

      if (CITY_OK == status) {
         status = (city_url_custom(CITY_OPERATION, &oper)) ? CITY_OK : CITY_URL_CUSTOM;
      }

      if (CITY_OK == status) {
         status = (urlConfiguration(&oper)) ? CITY_OK : CITY_WEATHER_URL;
      }

      if (CITY_OK == status) {
         status = (jsonConfig(&map, &ftp)) ? CITY_OK : CITY_WEATHER_PARSE;
      }
   } else {
      status = CITY_OK_NO_INTERNET;
      printf("No internet available, exiting\n");
   }

   return status;
}

int main(int argc, char *argv[]) {
    bool failed = true;

    city_init_e status = cityInit();
    if (CITY_OK != status) {
        printf("Initializing the city failed - status(%d)\n", status);
        failed = false;
    }

    return failed;
}
