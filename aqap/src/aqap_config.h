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
 *    True if able to parse AQAP configuration file, else false
 * DESCRIPTION:
 *    Parses the AQAP configuration file
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
 *    Returns the aqicn check-in time in seconds,
 *    Else -1 for error
 * DESCRIPTION:
 *    See RETURN
 **/
ssize_t aqap_get_checkin( void );

#endif /* _AQAP_CONFIG_H_ */

