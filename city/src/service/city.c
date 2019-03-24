#include <string.h>
#include <stdbool.h>

#include "city.h"
#include "city_curl.h"
#include "city_parse.h"
#include "city_types.h"
#include "city_custom.h"

#define CITY_CUSTOM  "/custom/city/city.json"
#define CITY_URL     "/custom/city/url.json"

static url_config_s url = {0};
static city_map_s map = {0};

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
   bool ok = true;
   city_init_e status = CITY_OK;

   ok = city_city_custom(CITY_CUSTOM);
   if (ok) {
      ok = city_url_custom(CITY_URL, &url);
   } else {
      status = CITY_CITY_CUSTOM;
   }

   if (ok) {
      ok = weatherURL(&url);
   } else {
      status = CITY_URL_CUSTOM;
   }

   if (ok) {
      ok = jsonConfig(&map);
   } else {
      status = CITY_WEATHER_URL;
   }

   if (!ok) {
      status = CITY_WEATHER_PARSE;
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
