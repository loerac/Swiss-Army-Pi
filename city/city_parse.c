#include "city.h"
#include "city_file.h"

bool parseCustom(map_sts *m) {
    bool failed = true;
    const size_t city_size = getFileInfo().size; 
    const file_status city_status = getFileInfo().status;
    if (    (city_size == -1)
        &&  (city_status != FILE_OK)) {
        printf("No data found\n");
    } else {
        char city_data[city_size];
        memset(city_data, 0, city_size);
        strncpy(city_data, getFileInfo().text, city_size);
        json_object *obj = json_tokener_parse(city_data);
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
    }

    return failed;
}
