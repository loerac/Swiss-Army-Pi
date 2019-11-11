#ifndef _AQAP_CUSTOM_H_
#define _AQAP_CUSTOM_H_

#include "ftp.h"
#include "slist.h"

#include <stdbool.h>

/**
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    True if able to parse AQAP customization file, else false
 * DESCRIPTION:
 *    Parses the AQAP customization file
 **/
bool aqap_custom_init( void );

/**
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    Returns the list of cities to search.
 * DESCRIPTION:
 *    See RETURN
 **/
slist_s *aqap_custom_get_list( void );

#endif /* _AQAP_CUSTOM_H_ */

