#include <stdbool.h>

#include "city.h"
#include "city_file.h"
#include "city_curl.h"
#include "city_parse.h"
#include "city_types.h"

#define CITY_CUSTOM "/custom/city/city.json"

static url_sts  url = { '\0' };
static city_map map = { '\0' };

city_init cityInit( void ) {
    bool failed = false;
    city_init status = CITY_OK;

    failed = city_config(CITY_CUSTOM);
    if (failed) {
        status = CITY_FILE_PROCESS_NOK;
        goto exit;
    }

    failed = weatherURL(&url);
    if (failed) {
        status = CITY_WEATHER_URL_NOK;
        goto exit;
    }

    failed = jsonConfig(&map);
    if (failed) {
        goto exit;
    }

exit:
    return status;
}

int main(int argc, char *argv[]) {
    bool failed = true;

    city_init status = cityInit();
    if (status != CITY_OK) {
        printf("Initializing the city failed - status(%d)\n", status);
        failed = false;
    }

    return failed;
}
