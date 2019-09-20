#ifndef _AIS_PARSER_H_
#define _AIS_PARSER_H_

#include "ftp.h"

#include <stdbool.h>
#include <linux/limits.h>

#define MAX_KEY_LENGTH  40U
#define MAX_DATE_LENGTH 10U

typedef struct nasa_operation {
   char save_location[PATH_MAX + 1U];
   char key[MAX_KEY_LENGTH + 1U];
   bool use_hd;
} nasa_operation_s;

typedef struct nasa_apod {
   char date[MAX_DATE_LENGTH + 1U]; // Format YYYY-MM-DD
   char *explanation;
   char *hdurl;
   char *url;
   char *title;
} nasa_apod_s;

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
 * RETURN:
 *    The API key to access NASA's API
 *
 * DESCRIPTION:
 *    See RETURN for description
 **/
char *nasaGetKey( void );

/**
 * INPUT:
 *    ftp - holds the content of the retrieve JSON file
 *
 * RETURN:
 *    True if the JSON file is valid
 *    Else, false
 *
 * DESCRIPTION:
 *    Parses the NASA Astronomy Picture Of the Day API
 **/
bool nasaParser(ftp_info_s ftp);

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

