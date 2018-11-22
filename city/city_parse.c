#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

#include "city.h"
#include "city_parse.h"

void jsonCoordURL(json_object *jb, url_sts *u) {
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "lon", sizeof("lon")) == 0) {
            strncpy(u->lon, json_object_get_string(val), MAX_LAT_LON);
        } else if (strncmp(key, "lat", sizeof("lat")) == 0) {
            strncpy(u->lat, json_object_get_string(val), MAX_LAT_LON);
        }
    }
}

bool parseCustom(url_sts *u) {
    bool failed = true;
    const size_t city_size = getCityFile().size;
    const file_status city_status = getCityFile().status;
    if (    (city_size == -1)
        &&  (city_status != FILE_OK)) {
        printf("Invalid file\n");
    } else {
        char city_data[city_size];
        memset(city_data, 0, city_size);
        strncpy(city_data, getCityFile().text, city_size);
        json_object *obj = json_tokener_parse(city_data);
        json_object_object_foreach(obj, key, val) {
            if (strncmp(key, "id", sizeof("id")) == 0) {
                strncpy(u->id , json_object_get_string(val), MAX_ID);
            } else if (strncmp(key, "zipcode", sizeof("zipcode")) == 0) {
                strncpy(u->zip, json_object_get_string(val), MAX_ZIPCODE);
            } else if (strncmp(key, "name", sizeof("name")) == 0) {
                strncpy(u->city, json_object_get_string(val), MAX_CITY_NAME);
            } else if (strncmp(key, "country", sizeof("country")) == 0) {
                strncpy(u->country, json_object_get_string(val), MAX_COUNTRY_CODE);
            } else if (strncmp(key, "key", sizeof("key")) == 0) {
                strncpy(u->key, json_object_get_string(val), MAX_KEY_SIZE + 1);
            } else if (strncmp(key, "language", sizeof("lang")) == 0) {
                strncpy(u->lang, json_object_get_string(val), MAX_LANG_SIZE);
            } else if (strncmp(key, "units", sizeof("units")) == 0) {
                u->u_format = json_object_get_int(val);
            } else if (strncmp(key, "url", sizeof("url")) == 0) {
                strncpy(u->u_string, json_object_get_string(val), MAX_UNIT_FORMAT);
            } else if (strncmp(key, "coord", sizeof("coord")) == 0) {
                jsonCoordURL(obj, u);
            }
        }
        failed = false;
    }

    return failed;
}

void jsonCoord(json_object *jb, city_map *m) {
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "lon", sizeof("lon")) == 0) {
            m->coord.lon = (float)json_object_get_double(val);
        } else if (strncmp(key, "lat", sizeof("lat")) == 0) {
            m->coord.lat = (float)json_object_get_double(val);
        }
    }
}

void jsonWeather(json_object *jb, city_map *m) {
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "id", sizeof("id")) == 0) {
            m->weather.id = json_object_get_int(val);
        } else if (strncmp(key, "main", sizeof("main")) == 0) {
            strncpy(m->weather.main, json_object_get_string(val), MAX_WEATHER_PAR);
        } else if (strncmp(key, "description", sizeof("descpription")) == 0) {
            strncpy(m->weather.desc, json_object_get_string(val), MAX_WEATHER_DESC);
        } else if (strncmp(key, "icon", sizeof("icon")) == 0) {
            strncpy(m->weather.icon, json_object_get_string(val), MAX_ICON_ID);
        }
    }
}

void jsonMain(json_object *jb, city_map *m) {
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
        }
    }
}

void jsonWind(json_object *jb, city_map *m) {
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "speed", sizeof("speed")) == 0) {
            m->wind.speed = (float)json_object_get_double(val);
        } else if (strncmp(key, "deg", sizeof("deg")) == 0) {
            m->wind.deg = (float)json_object_get_double(val);
        }
    }
}

void jsonCloud(json_object *jb, city_map *m) {
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "all", sizeof("all")) == 0) {
            m->cloud.all = json_object_get_int(val);
        }
    }
}

void jsonRain(json_object *jb, city_map *m) {
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "1hour", sizeof("1hour")) == 0) {
            m->rain.hour_1 = (float)json_object_get_double(val);
        } else if (strncmp(key, "1hour", sizeof("1hour")) == 0) {
            m->rain.hour_2 = (float)json_object_get_double(val);
        }
    }
}

void jsonSnow(json_object *jb, city_map *m) {
    json_object_object_foreach(jb, key, val) {
        if (strncmp(key, "1hour", sizeof("1hour")) == 0) {
            m->snow.hour_1 = (float)json_object_get_double(val);
        } else if (strncmp(key, "1hour", sizeof("1hour")) == 0) {
            m->snow.hour_2 = (float)json_object_get_double(val);
        }
    }
}

void jsonSys(json_object *jb, city_map *m) {
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
            m->sys.sunrise = (float)json_object_get_double(val);
        } else if (strncmp(key, "sunset", sizeof("sunset")) == 0) {
            m->sys.sunset = (float)json_object_get_double(val);
        }
    }
}

bool jsonConfig(city_map *m) {
    bool failed = true;
    //TODO: Get data from curl and place it here
    json_object *obj = json_tokener_parse(city_data);
    json_object_object_foreach(obj, key, val) {
        if (strncmp(key, "coord", sizeof("coord")) == 0) {
            jsonCoord(obj, m);
        } else if (strncmp(key, "weather", sizeof("weather")) == 0) {
            jsonWeather(obj, m);
        } else if (strncmp(key, "base", sizeof("base")) == 0) {
            strncpy(m->misc.base, json_object_get_string(val), MAX_BASE_PAR);
        } else if (strncmp(key, "main", sizeof("main")) == 0) {
            jsonMain(obj, m);
        } else if (strncmp(key, "visibility", sizeof("visibility")) == 0) {
            m->misc.visibility = (float)json_object_get_double(val);
        } else if (strncmp(key, "wind", sizeof("wind")) == 0) {
            jsonWind(obj, m);
        } else if (strncmp(key, "clouds", sizeof("clouds")) == 0) {
            jsonCloud(obj, m);
        } else if (strncmp(key, "dt", sizeof("dt")) == 0) {
            m->misc.dt = json_object_get_int64(val);
        } else if (strncmp(key, "sys", sizeof("sys")) == 0) {
            jsonSys(obj, m);
        } else if (strncmp(key, "id", sizeof("id")) == 0) {
            m->misc.id = json_object_get_int64(val);
        } else if (strncmp(key, "name", sizeof("name")) == 0) {
            strncpy(m->misc.name, json_object_get_string(val), MAX_CITY_NAME);
        } else if (strncmp(key, "cod", sizeof("cod")) == 0) {
            m->misc.cod = json_object_get_int64(val);
        }
    }
    failed = false;
    
    return failed;
}
