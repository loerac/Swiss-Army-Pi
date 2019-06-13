#ifndef _CITY_FILE_H_
#define _CITY_FILE_H_

#include "slist.h"
#include "city_types.h"

#include <stdbool.h>

typedef struct city_search_s {
    size_t size;
    char data[MAX_FORMAT_LEN];
    city_search_e search;
} city_search_s;

typedef struct city_format_s {
    size_t size;
    char data[MAX_LOCATION_LEN];
    city_format_e format;
} city_format_s;

/**********************************************
 * INPUT:
 *    json
 *       city.json file that defines the
 *       city location
 * OUTPUT:
 *    NONE
 * RETURN:
 *    True if json file is present and valid,
 *    else false.
 * DESCRIPTION:
 *    See RETURN for description.
 **********************************************/
bool city_city_custom(const char *const json);

/**********************************************
 * INPUT:
 *    json
 *       city_operation.json file that defines the
 *       openweathermap.org URL and key
 * OUTPUT:
 *    url
 *       Update the URL configuration with
 *       the URL and key
 * RETURN:
 *    True if json file is present and valid,
 *    else false.
 * DESCRIPTION:
 *    See RETURN for description.
 **********************************************/
bool city_url_custom(const char *const json, city_operation_s *oper);

/**********************************************
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    Linked list of the search format
 * DESCRIPTION:
 *    See RETURN for description.
 **********************************************/
slist_s *get_format( void );

/**********************************************
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    Linked list of the search locations
 * DESCRIPTION:
 *    See RETURN for description.
 **********************************************/
slist_s *get_location( void );

#endif
