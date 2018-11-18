#include "city.h"
#include "city_file.h"

bool parse_custom(map_sts *m) {
    bool failed = true;
    json_object *obj = json_tokener_parse(get_text());
    json_object_object_foreach(obj, key, val) {
        if (!strcmp(key, "id")) {
            strncpy(m->locn.id , json_object_get_string(val), MAX_ID_SIZE);
        } else if (!strcmp(key, "zipcode")) {
            strncpy(m->locn.zipcode, json_object_get_string(val), MAX_ZIPCODE_SIZE);
        } else if (!strcmp(key, "name")) {
            strncpy(m->locn.city, json_object_get_string(val), MAX_CITY_NAME);
        } else if (!strcmp(key, "country")) {
            strncpy(m->locn.country, json_object_get_string(val), MAX_COUNTRY_CODE);
        } else if (!strcmp(key, "key")) {
            strncpy(m->url.key, json_object_get_string(val), MAX_KEY_SIZE + 1);
        }
    }
    failed = false;

    return failed;
}
