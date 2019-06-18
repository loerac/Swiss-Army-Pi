#ifndef _AIS_PARSER_H_
#define _AIS_PARSER_H_

#include "ftp.h"

#include <stdbool.h>
#include <linux/limits.h>

typedef struct nis_operation {
   int time_interval;
   char save_location[PATH_MAX + 1U];
} nasa_operation_s;

typedef struct iss_now {
   float lat;
   float lon;
   unsigned long timestamp;
   char *message;
} iss_now_s;

/**
 * RETURN:
 *    True if the NASA Operation file is present and valid
 *    Else, false
 *
 * DESCRIPTION:
 *    Parses the NASA Operation file
 **/
bool nasaConf( void );

/**
 * INPUT:
 *    ftp - holds the content of the retrieve JSON file
 *
 * RETURN:
 *    True if the JSON file is valid
 *    Else, false
 *
 * DESCRIPTION:
 *    Parses the International Space Station API
 **/
bool issParser(ftp_info_s ftp);

#endif /* _AIS_PARSER_H_ */

