#include <stdbool.h>

#include "city.h"
#include "city_file.h"
#include "city_curl.h"
#include "city_parse.h"
#include "city_types.h"

#define CITY_CUSTOM "data/city.json"
#define CITY_SCHEMA "data/city_schema.json"
#define URL_CUSTOM "data/url_feature.json"
#define URL_SCHEMA "data/url_schema.json"

static url_sts url = {
    .key = {'\0'},
    .s_format = {'\0'},
    .e_format = KELVIN,
    .lang = {'\0'},
    .city = {'\0'},
    .id = {'\0'},
    .lat = {'\0'},
    .lon = {'\0'},
    .zip = {'\0'},
};

static city_map map = {
    .coord = {
        .lon = 0.0,
        .lat = 0.0,
    },
    .weather = {
        .id = 0,
        .main = {'\0'},
        .desc = {'\0'},
        .icon = {'\0'},
    },
    .main = {
        .temp = 0.0,
        .pressure = 0.0,
        .humidity = 0.0,
        .high = 0.0,
        .low = 0.0,
        .sea_level = 0.0,
        .grnd_level = 0.0,
    },
    .wind = {
        .speed = 0.0,
        .deg = 0.0,
    },
    .cloud = {
        .all = 0,
    },
    .rain = {
        .hour_1 = 0.0,
        .hour_2 = 0.0,
    },
    .snow = {
        .hour_1 = 0.0,
        .hour_2 = 0.0,
    },
    .sys = {
        .id = 0,
        .type = 0,
        .message = {'\0'},
        .country = {'\0'},
        .sunrise = 0,
        .sunset = 0,
    },
    .misc = {
        .base = {'\0'},
        .visibility = 0.0,
        .dt = 0,
        .id = 0,
        .name = {'\0'},
        .cod = 0,
    },
};

city_init cityInit( void ) {
    bool failed = false;
    city_init status = CITY_OK;

    failed = fileProcess(CITY_CUSTOM, CITY_SCHEMA);
    if (failed) {
        status = CITY_FILE_PROCESS_NOK;
        goto exit;
    }

    failed = parseCustom(&url);
    if (failed) {
        status = CITY_PARSE_CUSTOM_NOK;
        printf("Failed to parse custom file\n");
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
        goto exit;
    }

exit:
    return failed;
}
