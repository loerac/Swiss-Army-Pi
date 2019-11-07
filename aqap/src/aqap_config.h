#ifndef _AQAP_CONFIG_H_
#define _AQAP_CONFIG_H_

#include "ftp.h"

#include <stdbool.h>

/**
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    True if able to parse AQAP operation file, else false
 * DESCRIPTION:
 *    Parses the AQAP operation file that has the aqicn key,
 *    and a list of cities to search.
 **/
bool aqap_config_init( void );

/**
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    Returns the aqicn API key
 * DESCRIPTION:
 *    See RETURN
 **/
char *aqap_get_key( void );

/**
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    Returns the aqicn check-in time in seconds.
 * DESCRIPTION:
 *    See RETURN
 **/
aqap_get_checkin( void );

#endif /* _AQAP_CONFIG_H_ */

