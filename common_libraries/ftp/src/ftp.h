#ifndef _FTP_H_
#define _FTP_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct ftp_info {
   size_t size;
   bool valid;
   char *data;
} ftp_info_s;

/**
 * INPUT:
 *    url - URL that cURL is going to get the data from
 * OUTPUT:
 *    ftp - Stores the data of what was received
 * RETURN:
 *    True if data retrieve ok, else false.
 * DESCRIPTION:
 *    Retrieves the data from the given 'url'
 *
 *    **USER IS RESPONSIBLE ON FREEING MEMORY**
 **/
bool ftpGet(ftp_info_s **ftp, const char *const url);

// TODO: Implement a FTP Push
//bool ftpPush(ftp_info_s **ftp, const char *const url);

/**
 * DESCRIPTION:
 *    Destroy the resceived FTP data
 **/
void ftpDestroyInfo(ftp_info_s **ftp);

#endif /* _FTP_H_ */

