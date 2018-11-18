#include "city_curl.h"

#define MAX_GETBY_METHOD 64U

typedef struct city_data_s {
    size_t size;
    char *data;
} city_data;

static city_data city = {
    .size = 0,
    .data = "",
};

static map_sts map;

void destroyData() {
    city.size = 0;
    free(city.data);
}

size_t writeMemoryCb(void *contents, size_t size, size_t nmemb, void *userd) {
    size_t real_size = size * nmemb;
    city_data *c = (city_data*)userd;
    char *ptr = malloc(c->size + real_size + 1);
    if (ptr == NULL) {
        printf("Not enough memory (realloc returned NULL)\n");
        goto exit;
    }

    c->data = ptr;
    memcpy(&(c->data[c->size]), contents, real_size);
    c->size += real_size;
    c->data[c->size] = 0;

exit:
    return real_size;
}

get_city_by getBy(char *gb) {
    get_city_by get_by = GET_FAILED;
    if (map.locn.id[0] != '\0') {
        get_by = GET_BY_ID;
        snprintf(gb, MAX_GETBY_METHOD, "id=%s", map.locn.id);
    } else if (map.locn.zipcode[0] != '\0') {
        get_by = GET_BY_ZIPCODE;
        snprintf(gb, MAX_GETBY_METHOD, "zip=%s", map.locn.zipcode);
    } else if (    (map.locn.city[0] != '\0')
               &&  (map.locn.country[0] != '\0')) {
        get_by = GET_BY_CITY;
        snprintf(gb, MAX_GETBY_METHOD, "q=%s,%s", map.locn.city, map.locn.country);
    } else if (    (map.locn.latitude[0] != '\0')
               &&  (map.locn.longitude[0] != '\0')) {
        get_by = GET_BY_LATLOT;
        snprintf(gb, MAX_GETBY_METHOD, "lat=%s&log%s", map.locn.latitude, map.locn.longitude);
    }
    return get_by;
}

bool curlCityInfo( void ) {
    CURLcode res;
    CURL *curl_handle;
    bool failed = true;
    uint32_t url_size = 0;
    char url_string[MAX_GETBY_METHOD];
    char *url_text = "";

    get_city_by get_by = getBy(url_string);
    if (get_by == GET_FAILED) {
        printf("No valid key given to search city: status(%d)\n", get_by);
        goto exit;
    }

    url_size = strlen(WEATHER_URL) + strlen(WEATHER_KEY) + MAX_KEY_SIZE + strlen(url_string) + 1;
    url_text= malloc(url_size);
    snprintf(url_text, url_size, "%s%s%s%s", WEATHER_URL, url_string, WEATHER_KEY, map.url.key);

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, url_text);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeMemoryCb);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&city);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    res = curl_easy_perform(curl_handle);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: error(%s)\n", curl_easy_strerror(res));
        goto exit;
    }

    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
    failed = false;

exit:
    return failed;
}

void setCitySts(map_sts m) {
    map = m;
}

const size_t getCitySize() {
    return city.size;
}

const char *getCityData() {
    return city.data;
}
