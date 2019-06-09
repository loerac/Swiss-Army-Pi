#include <string.h>
#include <stdbool.h>

#include "city.h"
#include "city_curl.h"
#include "city_parse.h"
#include "city_types.h"
#include "city_custom.h"
#include "net_api.h"

#define CITY_CUSTOM  "/custom/city/city.json"
#define CITY_URL     "/custom/city/url.json"

static url_config_s  url = { '\0' };
static city_map_s    map = { '\0' };

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
         status = (city_url_custom(CITY_URL, &url)) ? CITY_OK : CITY_URL_CUSTOM;
      }

      if (CITY_OK == status) {
         status = (weatherURL(&url)) ? CITY_OK : CITY_WEATHER_URL;
      }

      if (CITY_OK == status) {
         status = (jsonConfig(&map)) ? CITY_OK : CITY_WEATHER_PARSE;
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
