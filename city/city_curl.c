#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "city_curl.h"
#include "city_types.h"

#define WEATHER_URL      "https://api.openweathermap.org/data/2.5/weather?"
#define MAX_GETBY_METHOD 64U

static city_info city = {
    .size = 0,
    .data = "",
};

void destroyCity( void ) {
    city.size = 0;
    free(city.data);
}

size_t writeMemoryCb(void *contents, size_t size, size_t nmemb, void *userd) {
    size_t real_size = size * nmemb;
    city_info *c = (city_info*)userd;
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

bool findBy(char *gb, const url_sts *url) {
    if ((url->find_by & CITY_ID) == CITY_ID) {
        snprintf(gb, MAX_GETBY_METHOD, "id=%s", url->id);
    } else if ((url->find_by & CITY_ZIP) == CITY_ZIP) {
        snprintf(gb, MAX_GETBY_METHOD, "zip=%s", url->zip);
    } else if ((url->find_by & CITY_NAME) == CITY_NAME) {
        snprintf(gb, BUFSIZ, "q=%s,%s", url->city, url->country);
    } else if ((url->find_by & CITY_COORD) == CITY_COORD) {
        snprintf(gb, MAX_GETBY_METHOD, "lat=%s&log%s", url->lat, url->lon);
    }

    return url->find_by;
}

bool weatherURL(const url_sts *url) {
    CURLcode res;
    CURL *curl_handle;
    bool failed = true;
    uint32_t url_size = 0;
    char url_search_city[MAX_GETBY_METHOD];
    char *url_text = "";

    int find_by = findBy(url_search_city, url);
    if ( find_by == 0 ) {
        printf("No valid key given to search city: status(%d)\n", find_by);
        goto exit;
    }

    url_size = strlen(WEATHER_URL) + strlen(url->key) + MAX_KEY_SIZE + strlen(url_search_city) + strlen(url->s_format) + 1;
    url_text= malloc(url_size);
    snprintf(url_text, url_size, "%s%s&units=%s&appid=%s", WEATHER_URL, url_search_city, url->s_format, url->key);

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


const city_info getCityInfo( void ) {
    return city;
}
