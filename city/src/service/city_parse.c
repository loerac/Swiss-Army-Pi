#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

#include "city.h"
#include "city_curl.h"
#include "city_parse.h"

bool jsonCoord(json_object *jb, city_map *m) {
   bool ok = true;
   printf("Coord:\n");
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "lon", sizeof("lon")) == 0) {
         m->coord.lon = (float)json_object_get_double(val);
         printf("\tlon: %f\n", m->coord.lon);
      } else if (strncmp(key, "lat", sizeof("lat")) == 0) {
         m->coord.lat = (float)json_object_get_double(val);
         printf("\tlat: %f\n", m->coord.lat);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

bool jsonMain(json_object *jb, city_map *m) {
   bool ok = true;
   printf("Main:\n");
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "temp", sizeof("temp")) == 0) {
         m->main.temp = (float)json_object_get_double(val);
         printf("\ttemp: %f\n", m->main.temp);
      } else if (strncmp(key, "pressure", sizeof("pressure")) == 0) {
         m->main.pressure = (float)json_object_get_double(val);
         printf("\tpressure: %f\n", m->main.pressure);
      } else if (strncmp(key, "humidity", sizeof("humidity")) == 0) {
         m->main.humidity = (float)json_object_get_double(val);
         printf("\thumidity: %f\n", m->main.humidity);
      } else if (strncmp(key, "temp_min", sizeof("temp_min")) == 0) {
         m->main.low = (float)json_object_get_double(val);
         printf("\tlow: %f\n", m->main.low);
      } else if (strncmp(key, "temp_max", sizeof("temp_max")) == 0) {
         m->main.high = (float)json_object_get_double(val);
         printf("\thigh: %f\n", m->main.high);
      } else if (strncmp(key, "sea_level", sizeof("sea_level")) == 0) {
         m->main.sea_level = (float)json_object_get_double(val);
         printf("\tsea_level: %f\n", m->main.sea_level);
      } else if (strncmp(key, "grnd_level", sizeof("grnd_level")) == 0) {
         m->main.grnd_level = (float)json_object_get_double(val);
         printf("\tgrnd: %f\n", m->main.grnd_level);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

bool jsonWeather(json_object *jb, city_map *m, const char *k) {
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
            m->weather[i].id = json_object_get_int(val);
            printf("\tid[%d]: %d\n", i + 1, m->weather[i].id);
         } else if (strncmp(key, "main", sizeof("main")) == 0) {
            strncpy(m->weather[i].main, json_object_get_string(val), sizeof(m->weather[i].main));
            printf("\tmain[%d]: %s\n", i + 1, m->weather[i].main);
         } else if (strncmp(key, "description", sizeof("descpription")) == 0) {
            strncpy(m->weather[i].desc, json_object_get_string(val), sizeof(m->weather[i].desc));
            printf("\tdesc[%d]: %s\n", i + 1, m->weather[i].desc);
         } else if (strncmp(key, "icon", sizeof("icon")) == 0) {
            snprintf(m->weather[i].icon, PATH_MAX + 1, "http://openweathermap.org/img/w/%s.png", json_object_get_string(val));
            printf("\ticon[%d]: %s\n", i + 1, m->weather[i].icon);
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

bool jsonWind(json_object *jb, city_map *m) {
   bool ok = true;
   printf("Wind:\n");
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "speed", sizeof("speed")) == 0) {
         m->wind.speed = (float)json_object_get_double(val);
         printf("\tspeed: %f\n", m->wind.speed);
      } else if (strncmp(key, "deg", sizeof("deg")) == 0) {
         m->wind.deg = (float)json_object_get_double(val);
         printf("\tdeg: %f\n", m->wind.deg);
      } else if (strncmp(key, "gust", sizeof("gust")) == 0) {
         m->wind.gust = (float)json_object_get_double(val);
         printf("\tgust: %f\n", m->wind.gust);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

bool jsonCloud(json_object *jb, city_map *m) {
   bool ok = true;
   printf("Cloud:\n");
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "all", sizeof("all")) == 0) {
         m->cloud.all = json_object_get_int(val);
         printf("\tcloud: %d\n", m->cloud.all);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

bool jsonRain(json_object *jb, city_map *m) {
   bool ok = true;
   printf("Rain:\n");
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "1hour", sizeof("1hour")) == 0) {
         m->rain.hour_1 = (float)json_object_get_double(val);
         printf("\thour_1: %f\n", m->rain.hour_1);
      } else if (strncmp(key, "2hour", sizeof("2hour")) == 0) {
         m->rain.hour_2 = (float)json_object_get_double(val);
         printf("\thour_1: %f\n", m->rain.hour_1);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

bool jsonSnow(json_object *jb, city_map *m) {
   bool ok = true;
   printf("Snow:\n");
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "1hour", sizeof("1hour")) == 0) {
         m->snow.hour_1 = (float)json_object_get_double(val);
         printf("\thour_1: %f\n", m->snow.hour_1);
      } else if (strncmp(key, "2hour", sizeof("2hour")) == 0) {
         m->snow.hour_2 = (float)json_object_get_double(val);
         printf("\thour_2: %f\n", m->snow.hour_2);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

void timeToChar(time_t unix_time, char *military_time, size_t size) {
   struct tm *timeinfo;
   timeinfo = localtime(&unix_time);
   (void)strftime(military_time, size, "%R", timeinfo);
}

bool jsonSys(json_object *jb, city_map *m) {
   bool ok = true;
   json_object_object_foreach(jb, key, val) {
      if (strncmp(key, "type", sizeof("type")) == 0) {
         m->sys.type = json_object_get_int(val);
         printf("\ttype: %d\n", m->sys.type);
      } else if (strncmp(key, "id", sizeof("id")) == 0) {
         m->sys.id= json_object_get_int(val);
         printf("\tid: %d\n", m->sys.id);
      } else if (strncmp(key, "message", sizeof("message")) == 0) {
         strncpy(m->sys.message, json_object_get_string(val), sizeof(m->sys.message));
         printf("\tmessge: %s\n", m->sys.message);
      } else if (strncmp(key, "country", sizeof("country")) == 0) {
         strncpy(m->sys.country, json_object_get_string(val), sizeof(m->sys.country));
         printf("\tcountry: %s\n", m->sys.country);
      } else if (strncmp(key, "sunrise", sizeof("sunrise")) == 0) {
         timeToChar((time_t)json_object_get_double(val), m->sys.sunrise, sizeof(m->sys.sunrise));
         printf("\tsunrise: %s\n", m->sys.sunrise);
      } else if (strncmp(key, "sunset", sizeof("sunset")) == 0) {
         timeToChar((time_t)json_object_get_double(val), m->sys.sunset, sizeof(m->sys.sunset));
         printf("\tsunset: %s\n", m->sys.sunset);
      } else {
         ok = false;
         printf("\tUnknown key: %s\n", key);
         break;
      }
   }

   return ok;
}

bool jsonConfig(city_map *m) {
   bool ok = false;
   char *city_data = getCityInfo().data;
   if (city_data == NULL) {
      printf("No data available\n");
   } else {
      json_object *obj = json_tokener_parse(city_data);
      json_object_object_foreach(obj, key, val) {
         if (strncmp(key, "coord", sizeof("coord")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonCoord(new_obj, m);
         } else if (strncmp(key, "weather", sizeof("weather")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonWeather(new_obj, m, key);
         } else if (strncmp(key, "main", sizeof("main")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonMain(new_obj, m);
         } else if (strncmp(key, "wind", sizeof("wind")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonWind(new_obj, m);
         } else if (strncmp(key, "clouds", sizeof("clouds")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonCloud(new_obj, m);
         } else if (strncmp(key, "sys", sizeof("sys")) == 0) {
            json_object *new_obj = json_object_object_get(obj, key);
            ok = jsonSys(new_obj, m);
         } else if (strncmp(key, "base", sizeof("base")) == 0) {
            strncpy(m->misc.base, json_object_get_string(val), sizeof(m->misc.base));
            printf("base: %s\n", m->misc.base);
         } else if (strncmp(key, "visibility", sizeof("visibility")) == 0) {
            m->misc.visibility = (float)json_object_get_double(val);
            printf("visibility: %f\n", m->misc.visibility);
         } else if (strncmp(key, "dt", sizeof("dt")) == 0) {
            m->misc.dt = json_object_get_int64(val);
            printf("dt: %ld\n", m->misc.dt);
         } else if (strncmp(key, "id", sizeof("id")) == 0) {
            m->misc.id = json_object_get_int64(val);
            printf("id: %ld\n", m->misc.id);
         } else if (strncmp(key, "name", sizeof("name")) == 0) {
            strncpy(m->misc.name, json_object_get_string(val), sizeof(m->misc.name));
            printf("name: %s\n", m->misc.name);
         } else if (strncmp(key, "cod", sizeof("cod")) == 0) {
            m->misc.cod = json_object_get_int64(val);
            printf("cod: %ld\n", m->misc.cod);
         }
      }
   }

   return ok;
}
