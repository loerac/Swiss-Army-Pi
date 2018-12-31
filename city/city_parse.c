#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

#include "city.h"
#include "city_curl.h"
#include "city_parse.h"

bool jsonCoord(json_object *jb, city_map *m) {
    bool failed = true;
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "lon", sizeof("lon")) == 0) {
            m->coord.lon = (float)json_object_get_double(val);
        } else if (strncmp(key, "lat", sizeof("lat")) == 0) {
            m->coord.lat = (float)json_object_get_double(val);
        } else {
            printf("Key not found: key(%s)\n", key);
            goto exit;
        }
    }
    failed = false;

exit:
    return failed;
}

bool jsonMain(json_object *jb, city_map *m) {
    bool failed = true;
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "temp", sizeof("temp")) == 0) {
            m->main.temp = (float)json_object_get_double(val);
        } else if (strncmp(key, "pressure", sizeof("pressure")) == 0) {
            m->main.pressure = (float)json_object_get_double(val);
        } else if (strncmp(key, "humidity", sizeof("humidity")) == 0) {
            m->main.humidity = (float)json_object_get_double(val);
        } else if (strncmp(key, "temp_min", sizeof("temp_min")) == 0) {
            m->main.high = (float)json_object_get_double(val);
        } else if (strncmp(key, "temp_max", sizeof("temp_max")) == 0) {
            m->main.low = (float)json_object_get_double(val);
        } else if (strncmp(key, "sea_level", sizeof("sea_level")) == 0) {
            m->main.sea_level = (float)json_object_get_double(val);
        } else if (strncmp(key, "grnd_level", sizeof("grnd_level")) == 0) {
            m->main.grnd_level = (float)json_object_get_double(val);
        } else {
            printf("Key not found: key(%s)\n", key);
            goto exit;
        }
    }
    failed = false;

exit:
    return failed;
}

bool jsonWeather(json_object *jb, city_map *m, const char *k) {
    bool failed = true;
    json_object *jarray = jb;
    if (k != NULL) {
        json_object_object_get(jb, k);
    }

    int i;
    int array_len = json_object_array_length(jarray);
    for (i = 0; i < array_len; i++) {
        json_object *jvalue = json_object_array_get_idx(jb, i);
        json_object_object_foreach(jvalue, key, val) {
            if (strncmp(key, "id", sizeof("id")) == 0) {
                m->weather[i].id = json_object_get_int(val);
            } else if (strncmp(key, "main", sizeof("main")) == 0) {
                strncpy(m->weather[i].main, json_object_get_string(val), MAX_WEATHER_PAR);
            } else if (strncmp(key, "description", sizeof("descpription")) == 0) {
                strncpy(m->weather[i].desc, json_object_get_string(val), MAX_WEATHER_DESC);
            } else if (strncmp(key, "icon", sizeof("icon")) == 0) {
                snprintf(m->weather[i].icon, PATH_MAX + 1, "http://openweathermap.org/img/w/%s.png", json_object_get_string(val));
            } else {
                printf("Key not found: val(%s)\n", key);
                goto exit;
            }
        }
    }
    failed = false;

exit:
    return failed;
}

bool jsonWind(json_object *jb, city_map *m) {
    bool failed = true;
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "speed", sizeof("speed")) == 0) {
            m->wind.speed = (float)json_object_get_double(val);
        } else if (strncmp(key, "deg", sizeof("deg")) == 0) {
            m->wind.deg = (float)json_object_get_double(val);
        } else if (strncmp(key, "gust", sizeof("gust")) == 0) {
            m->wind.gust = (float)json_object_get_double(val);
        } else {
            printf("Key not found: key(%s)\n", key);
            goto exit;
        }
    }
    failed = false;

exit:
    return failed;
}

bool jsonCloud(json_object *jb, city_map *m) {
    bool failed = true;
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "all", sizeof("all")) == 0) {
            m->cloud.all = json_object_get_int(val);
        } else {
            printf("Key not found: key(%s)\n", key);
            goto exit;
        }
    }
    failed = false;

exit:
    return failed;
}

bool jsonRain(json_object *jb, city_map *m) {
    bool failed = true;
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "1hour", sizeof("1hour")) == 0) {
            m->rain.hour_1 = (float)json_object_get_double(val);
        } else if (strncmp(key, "1hour", sizeof("1hour")) == 0) {
            m->rain.hour_2 = (float)json_object_get_double(val);
        } else {
            printf("Key not found: key(%s)\n", key);
            goto exit;
        }
    }
    failed = false;

exit:
    return failed;
}

bool jsonSnow(json_object *jb, city_map *m) {
    bool failed = true;
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "1hour", sizeof("1hour")) == 0) {
            m->snow.hour_1 = (float)json_object_get_double(val);
        } else if (strncmp(key, "1hour", sizeof("1hour")) == 0) {
            m->snow.hour_2 = (float)json_object_get_double(val);
        } else {
            printf("Key not found: key(%s)\n", key);
            goto exit;
        }
    }
    failed = false;

exit:
    return failed;
}

bool jsonSys(json_object *jb, city_map *m) {
    bool failed = true;
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "type", sizeof("type")) == 0) {
            m->sys.type = json_object_get_int(val);
        } else if (strncmp(key, "id", sizeof("id")) == 0) {
            m->sys.id= json_object_get_int(val);
        } else if (strncmp(key, "message", sizeof("message")) == 0) {
            strncpy(m->sys.message, json_object_get_string(val), MAX_MESSAGE_SIZE);
        } else if (strncmp(key, "country", sizeof("country")) == 0) {
            strncpy(m->sys.country, json_object_get_string(val), MAX_COUNTRY_CODE);
        } else if (strncmp(key, "sunrise", sizeof("sunrise")) == 0) {
            m->sys.sunrise = (time_t)json_object_get_double(val);
        } else if (strncmp(key, "sunset", sizeof("sunset")) == 0) {
            m->sys.sunset = (time_t)json_object_get_double(val);
        } else {
            printf("Key not found: key(%s)\n", key);
            goto exit;
        }
    }
    failed = false;

exit:
    return failed;
}

bool jsonConfig(city_map *m) {
    bool failed = true;
    json_object *obj = json_tokener_parse(getCityInfo().data);
    json_object_object_foreach(obj, key, val) {
        if (strncmp(key, "coord", sizeof("coord")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            failed = jsonCoord(new_obj, m);
        } else if (strncmp(key, "weather", sizeof("weather")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            failed = jsonWeather(new_obj, m, key);
        } else if (strncmp(key, "main", sizeof("main")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            failed = jsonMain(new_obj, m);
        } else if (strncmp(key, "wind", sizeof("wind")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            failed = jsonWind(new_obj, m);
        } else if (strncmp(key, "clouds", sizeof("clouds")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            failed = jsonCloud(new_obj, m);
        } else if (strncmp(key, "sys", sizeof("sys")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            failed = jsonSys(new_obj, m);
        } else if (strncmp(key, "base", sizeof("base")) == 0) {
            strncpy(m->misc.base, json_object_get_string(val), MAX_BASE_PAR);
        } else if (strncmp(key, "visibility", sizeof("visibility")) == 0) {
            m->misc.visibility = (float)json_object_get_double(val);
        } else if (strncmp(key, "dt", sizeof("dt")) == 0) {
            m->misc.dt = json_object_get_int64(val);
        } else if (strncmp(key, "id", sizeof("id")) == 0) {
            m->misc.id = json_object_get_int64(val);
        } else if (strncmp(key, "name", sizeof("name")) == 0) {
            strncpy(m->misc.name, json_object_get_string(val), MAX_CITY_NAME);
        } else if (strncmp(key, "cod", sizeof("cod")) == 0) {
            m->misc.cod = json_object_get_int64(val);
        } else {
            printf("Key not found: key(%s)\n", key);
            goto exit;
        }

        if ( failed ) {
            printf("Failed to parse JSON file\n");
            goto exit;
        }
    }
    failed = false;
    
exit:
    return failed;
}
