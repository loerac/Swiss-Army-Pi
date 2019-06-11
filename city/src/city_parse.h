#ifndef _CITY_PARSEH_
#define _CITY_PARSEH_

#include "ftp.h"
#include "city.h"
#include "city_types.h"

#include <stdbool.h>

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
bool jsonConfig(city_map_s *map, ftp_info_s *ftp);

#endif
