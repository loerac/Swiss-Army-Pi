#ifndef _CITY_PARSEH_
#define _CITY_PARSEH_

#include <stdbool.h>

#include "city.h"
#include "city_types.h"

/**********************************************
 * INTPUT:
 *    NONE
 * OUTPUT:
 *    map
 *       Store data retrieved
 * RETURN:
 *    True if data was valid, else false.
 * DESCRIPTION:
 *    Parses the returned data from https://openweathermap.org/
 **********************************************/
bool jsonConfig(city_map *map);

#endif
