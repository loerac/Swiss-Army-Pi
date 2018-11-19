#include "city.h"
#include "city_file.h"
#include "city_curl.h"
#include "city_parse.h"

#define CITY_CUSTOM "data/city.json"
#define CITY_SCHEMA "data/city_schema.json"

static map_sts map = {
    .sun = {
        .rise = 0.0,
        .set = 0.0,
    },
    .wthr = {
        .temp = 0.0,
        .humidity = 0.0,
        .temp_max = 0.0,
        .temp_min = 0.0,
        .pressure = 0.0,
        .description = NULL,
    },
    .locn = {
        .id = {'\0'},
        .zipcode = {'\0'},
        .latitude = {'\0'},
        .longitude = {'\0'},
        .city = {'\0'},
        .country = {'\0'},
    }
};

bool cityInit( void ) {
    bool failed = true;

    failed = fileProcess(CITY_CUSTOM, CITY_SCHEMA);
    if (failed) {
        goto exit;
    }

    failed = parseCustom(&map);
    if (failed) {
        goto exit;
    }

    setCitySts(map);
    failed = curlCityInfo();
    if (failed) {
        goto exit;
    }

exit:
    return failed;
}

int main(int argc, char *argv[]) {
    bool failed = true;

    failed = cityInit();
    if (failed) {
        goto exit;
    }

exit:
    return failed;
}
