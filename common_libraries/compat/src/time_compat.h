#ifndef _TIME_COMPAT_H_
#define _TIME_COMPAT_H_

#include <time.h>
#include <stdlib.h>

/**
 * INPUT:
 *    unix_ts - Unix timestamp
 *    format - Convert 'unix_timestamp' into specific format
 *    ts_length - Buffer length of 'timestamp'
 * OUTPUT:
 *    timestamp - Holds the converted Unix timestamp on success,
 *                Else NULL
 * RETURN:
 *    Size of the 'timestamp' buffer on success, else -1
 * DESCRIPTION:
 *    Convert an Unix timestamp to a specific format
 **/
ssize_t timecompactUnixToDate(const time_t unix_ts,
                              const char *const format,
                              char *timestamp,
                              const size_t ts_length);

/**
 * INPUT:
 *    format - Gets the timestamp in a specific format
 * RETURN:
 *    Returns the timestamp in a string
 * DESCRIPTION:
 *    See RETURN for description
 *
 *    ** USER IS RESPONSIBLE FOR FREEING MEMORY **
 **/
char *timecompactStringTimestamp(const char *const format);

#endif /* _TIME_COMPAT_H_ */
