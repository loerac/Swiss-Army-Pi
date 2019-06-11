#ifndef _CITY_FTP_H_
#define _CITY_FTP_H_

#include <stdbool.h>
#include <curl/curl.h>

#include "city.h"

typedef struct city_info {
    size_t size;
    char *data;
    bool valid;
} city_info_s;

/**********************************************
 * INPUT:
 *    url
 *       Configuration for the URL and user key
 * OUTPUT:
 *    NONE
 * RETURN:
 *    True if data retrieve ok, else false.
 * DESCRIPTION:
 *    Retrieves the data from https://openweathermap.org
 **********************************************/
bool weatherURL(const url_config_s *url);

/**********************************************
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    Value of city_info.
 *    NULL if invalid.
 * DESCRIPTION:
 *    See RETURN for description.
 **********************************************/
const city_info_s getCityInfo( void );

/**********************************************
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    NONE
 * DESCRIPTION:
 *    Sets the city_info to NULL.
 **********************************************/
void destroyCity( void );

#endif /* _CITY_FTP_H_ */

