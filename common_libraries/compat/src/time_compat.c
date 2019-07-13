#include "time_compat.h"

#include <stdio.h>
#include <string.h>

/* See time_compat.h for description */
ssize_t unixTimestampConvert( const time_t unix_timestamp,
                              const char *const format,
                              char *timestamp,
                              const size_t ts_length) {
   ssize_t ts_new_length = -1;
   struct tm lt_result = {0};

   if (NULL == localtime_r(&unix_timestamp, &lt_result)) {
      printf("NOTICE: Getting localtime failed - %m\n");
   } else if (0 == strftime(timestamp, ts_length, format, &lt_result)) {
      printf("NOTICE: Formatting time with '%s' failed - %m\n", format);
   } else {
      ts_new_length = strlen(timestamp);
   }

   return ts_new_length;
}

