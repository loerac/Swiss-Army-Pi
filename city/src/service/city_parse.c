#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

#include "city.h"
#include "city_curl.h"
#include "city_parse.h"

/**********************************************
 * INPUT:
 *    size
 *       Size of the array
 * OUTPUT:
 *    military_time
 *       Time being stored in military time
 * RETURN:
 *    NONE
 * DESCRIPTION:
 *    Convert UNIX time to military time
 **********************************************/
static void timeToChar(time_t unix_time, char *military_time, size_t size) {
   struct tm *timeinfo;
   timeinfo = localtime(&unix_time);
   (void)strftime(military_time, size, "%R", timeinfo);
}

/**********************************************
 * INPUT:
 *    jb
 *       JSON object pointing to current location
 * OUTPUT:
 *    map
 *       Store city info
 * RETURN:
 *    True if valid, else false
 * DESCRIPTION:
 *    Parses the city coordinates
 **********************************************/
static bool jsonCoord(json_object *jb, city_map *map) {
   bool ok = true;
   printf("Coord:\n");
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "lon", sizeof("lon")) == 0) {
         map->coord.lon = (float)json_object_get_double(val);
         printf("\tlon: %f\n", map->coord.lon);
      } else if (strncmp(key, "lat", sizeof("lat")) == 0) {
         map->coord.lat = (float)json_object_get_double(val);
         printf("\tlat: %f\n", map->coord.lat);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

/**********************************************
 * INPUT:
 *    jb
 *       JSON object pointing to current location
 * OUTPUT:
 *    map
 *       Store city info
 * RETURN:
 *    True if valid, else false
 * DESCRIPTION:
 *    Parses the city main information
 **********************************************/
static bool jsonMain(json_object *jb, city_map *map) {
   bool ok = true;
   printf("Main:\n");
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "temp", sizeof("temp")) == 0) {
         map->main.temp = (float)json_object_get_double(val);
         printf("\ttemp: %f\n", map->main.temp);
      } else if (strncmp(key, "pressure", sizeof("pressure")) == 0) {
         map->main.pressure = (float)json_object_get_double(val);
         printf("\tpressure: %f\n", map->main.pressure);
      } else if (strncmp(key, "humidity", sizeof("humidity")) == 0) {
         map->main.humidity = (float)json_object_get_double(val);
         printf("\thumidity: %f\n", map->main.humidity);
      } else if (strncmp(key, "temp_min", sizeof("temp_min")) == 0) {
         map->main.low = (float)json_object_get_double(val);
         printf("\tlow: %f\n", map->main.low);
      } else if (strncmp(key, "temp_max", sizeof("temp_max")) == 0) {
         map->main.high = (float)json_object_get_double(val);
         printf("\thigh: %f\n", map->main.high);
      } else if (strncmp(key, "sea_level", sizeof("sea_level")) == 0) {
         map->main.sea_level = (float)json_object_get_double(val);
         printf("\tsea_level: %f\n", map->main.sea_level);
      } else if (strncmp(key, "grnd_level", sizeof("grnd_level")) == 0) {
         map->main.grnd_level = (float)json_object_get_double(val);
         printf("\tgrnd: %f\n", map->main.grnd_level);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

/**********************************************
 * INPUT:
 *    jb
 *       JSON object pointing to current location
 * OUTPUT:
 *    map
 *       Store city info
 * RETURN:
 *    True if valid, else false
 * DESCRIPTION:
 *    Parses the city weather information
 **********************************************/
static bool jsonWeather(json_object *jb, city_map *map, const char *const k) {
   bool ok = true;
   printf("Weather:\n");
   json_object *jarray = jb;
   if (k != NULL) {
      json_object_object_get(jb, k);
   }

   unsigned int i;
   int array_len = json_object_array_length(jarray);
   for (i = 0; i < array_len; i++) {
      json_object *jvalue = json_object_array_get_idx(jb, i);
      json_object_object_foreach(jvalue, key, val) {
         if (strncmp(key, "id", sizeof("id")) == 0) {
            map->weather[i].id = json_object_get_int(val);
            printf("\tid[%d]: %d\n", i + 1, map->weather[i].id);
         } else if (strncmp(key, "main", sizeof("main")) == 0) {
            strncpy(map->weather[i].main, json_object_get_string(val), sizeof(map->weather[i].main));
            printf("\tmain[%d]: %s\n", i + 1, map->weather[i].main);
         } else if (strncmp(key, "description", sizeof("descpription")) == 0) {
            strncpy(map->weather[i].desc, json_object_get_string(val), sizeof(map->weather[i].desc));
            printf("\tdesc[%d]: %s\n", i + 1, map->weather[i].desc);
         } else if (strncmp(key, "icon", sizeof("icon")) == 0) {
            snprintf(map->weather[i].icon, PATH_MAX + 1, "http://openweathermap.org/img/w/%s.png", json_object_get_string(val));
            printf("\ticon[%d]: %s\n", i + 1, map->weather[i].icon);
         } else {
            ok = false;
            printf("\tUnknown key: %s\n", key);
            break;
         }
      }

      if (!ok) {
         break;
      }
   }

   return ok;
}

/**********************************************
 * INPUT:
 *    jb
 *       JSON object pointing to current location
 * OUTPUT:
 *    map
 *       Store city info
 * RETURN:
 *    True if valid, else false
 * DESCRIPTION:
 *    Parses the city wind information
 **********************************************/
static bool jsonWind(json_object *jb, city_map *map) {
   bool ok = true;
   printf("Wind:\n");
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "speed", sizeof("speed")) == 0) {
         map->wind.speed = (float)json_object_get_double(val);
         printf("\tspeed: %f\n", map->wind.speed);
      } else if (strncmp(key, "deg", sizeof("deg")) == 0) {
         map->wind.deg = (float)json_object_get_double(val);
         printf("\tdeg: %f\n", map->wind.deg);
      } else if (strncmp(key, "gust", sizeof("gust")) == 0) {
         map->wind.gust = (float)json_object_get_double(val);
         printf("\tgust: %f\n", map->wind.gust);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

/**********************************************
 * INPUT:
 *    jb
 *       JSON object pointing to current location
 * OUTPUT:
 *    map
 *       Store city info
 * RETURN:
 *    True if valid, else false
 * DESCRIPTION:
 *    Parses the city cloud information
 **********************************************/
static bool jsonCloud(json_object *jb, city_map *map) {
   bool ok = true;
   printf("Cloud:\n");
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "all", sizeof("all")) == 0) {
         map->cloud.all = json_object_get_int(val);
         printf("\tcloud: %d\n", map->cloud.all);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

/**********************************************
 * INPUT:
 *    jb
 *       JSON object pointing to current location
 * OUTPUT:
 *    map
 *       Store city info
 * RETURN:
 *    True if valid, else false
 * DESCRIPTION:
 *    Parses the city rain information
 **********************************************/
static bool jsonRain(json_object *jb, city_map *map) {
   bool ok = true;
   printf("Rain:\n");
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "1hour", sizeof("1hour")) == 0) {
         map->rain.hour_1 = (float)json_object_get_double(val);
         printf("\thour_1: %f\n", map->rain.hour_1);
      } else if (strncmp(key, "2hour", sizeof("2hour")) == 0) {
         map->rain.hour_2 = (float)json_object_get_double(val);
         printf("\thour_1: %f\n", map->rain.hour_1);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

/**********************************************
 * INPUT:
 *    jb
 *       JSON object pointing to current location
 * OUTPUT:
 *    map
 *       Store city info
 * RETURN:
 *    True if valid, else false
 * DESCRIPTION:
 *    Parses the city snow information
 **********************************************/
static bool jsonSnow(json_object *jb, city_map *map) {
   bool ok = true;
   printf("Snow:\n");
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "1hour", sizeof("1hour")) == 0) {
         map->snow.hour_1 = (float)json_object_get_double(val);
         printf("\thour_1: %f\n", map->snow.hour_1);
      } else if (strncmp(key, "2hour", sizeof("2hour")) == 0) {
         map->snow.hour_2 = (float)json_object_get_double(val);
         printf("\thour_2: %f\n", map->snow.hour_2);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

/**********************************************
 * INPUT:
 *    jb
 *       JSON object pointing to current location
 * OUTPUT:
 *    map
 *       Store city info
 * RETURN:
 *    True if valid, else false
 * DESCRIPTION:
 *    Parses the city system information
 **********************************************/
static bool jsonSys(json_object *jb, city_map *map) {
   bool ok = true;
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "type", sizeof("type")) == 0) {
         map->sys.type = json_object_get_int(val);
         printf("\ttype: %d\n", map->sys.type);
      } else if (strncmp(key, "id", sizeof("id")) == 0) {
         map->sys.id= json_object_get_int(val);
         printf("\tid: %d\n", map->sys.id);
      } else if (strncmp(key, "message", sizeof("message")) == 0) {
         strncpy(map->sys.message, json_object_get_string(val), sizeof(map->sys.message));
         printf("\tmessge: %s\n", map->sys.message);
      } else if (strncmp(key, "country", sizeof("country")) == 0) {
         strncpy(map->sys.country, json_object_get_string(val), sizeof(map->sys.country));
         printf("\tcountry: %s\n", map->sys.country);
      } else if (strncmp(key, "sunrise", sizeof("sunrise")) == 0) {
         timeToChar((time_t)json_object_get_double(val), map->sys.sunrise, sizeof(map->sys.sunrise));
         printf("\tsunrise: %s\n", map->sys.sunrise);
      } else if (strncmp(key, "sunset", sizeof("sunset")) == 0) {
         timeToChar((time_t)json_object_get_double(val), map->sys.sunset, sizeof(map->sys.sunset));
         printf("\tsunset: %s\n", map->sys.sunset);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

/**********************************************
 * See city_parse.h for description.
 **********************************************/
bool jsonConfig(city_map *map) {
   bool ok = false;
   city_info info = getCityInfo();

   if (  (!info.valid) ||
         (info.data == NULL) ) {
      printf("No data available\n");
   } else {
      json_object *obj = json_tokener_parse(info.data);
      json_object_object_foreach(obj, key, val) {
         if (strncmp(key, "coord", sizeof("coord")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonCoord(new_obj, map);
         } else if (strncmp(key, "weather", sizeof("weather")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonWeather(new_obj, map, key);
         } else if (strncmp(key, "main", sizeof("main")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonMain(new_obj, map);
         } else if (strncmp(key, "wind", sizeof("wind")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonWind(new_obj, map);
         } else if (strncmp(key, "clouds", sizeof("clouds")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonCloud(new_obj, map);
         } else if (strncmp(key, "rain", sizeof("rain")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonRain(new_obj, map);
         } else if (strncmp(key, "snow", sizeof("snow")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonSnow(new_obj, map);
         } else if (strncmp(key, "sys", sizeof("sys")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonSys(new_obj, map);
         } else if (strncmp(key, "base", sizeof("base")) == 0) {
            strncpy(map->misc.base, json_object_get_string(val), sizeof(map->misc.base));
            printf("base: %s\n", map->misc.base);
         } else if (strncmp(key, "visibility", sizeof("visibility")) == 0) {
            map->misc.visibility = (float)json_object_get_double(val);
            printf("visibility: %f\n", map->misc.visibility);
         } else if (strncmp(key, "dt", sizeof("dt")) == 0) {
            map->misc.dt = json_object_get_int64(val);
            printf("dt: %ld\n", map->misc.dt);
         } else if (strncmp(key, "id", sizeof("id")) == 0) {
            map->misc.id = json_object_get_int64(val);
            printf("id: %ld\n", map->misc.id);
         } else if (strncmp(key, "name", sizeof("name")) == 0) {
            strncpy(map->misc.name, json_object_get_string(val), sizeof(map->misc.name));
            printf("name: %s\n", map->misc.name);
         } else if (strncmp(key, "cod", sizeof("cod")) == 0) {
            map->misc.cod = json_object_get_int64(val);
            printf("cod: %ld\n", map->misc.cod);
         }
      }
   }

   // City info is not needed any more,
   // memory can be freed.
   destroyCity();

   return ok;
}
