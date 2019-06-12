#include "ftp.h"
#include "type_compat.h"

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

/**
 * INPUT:
 *    contents - data retreived from cURL operation
 *    size - alwasy one (1)
 *    nmemb - size of 'contents'
 * OUTPUT:
 *    userp - pointer to where the data is being written
 * RETURN:
 *    Total size of 'contents'
 * DESCRIPTON:
 *    Writes the downloaded data into a chunck of memory
 *    that was retrieved from cURL
 *
 *    Callback function from cURL:
 *       https://curl.haxx.se/libcurl/c/getinmemory.html
 **/
static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
   size_t realsize = size * nmemb;
   ftp_info_s *info = (ftp_info_s *)userp;
   char *ptr = realloc(info->data, info->size + realsize + 1);

   if (NULL != ptr) {
      info->data = ptr;
      imemcpy( &(info->data[info->size]), contents, realsize);
      info->size += realsize;
      info->data[info->size] = '\0';
      info->valid = true;
   } else {
      printf("EMERG: Re-allocating memory failed - %m\n");
   }

   return realsize;
}

/* See ftp.h for description */
bool ftpGet(ftp_info_s **ftp, const char *const url) {
   CURL *curl_handle = NULL;
   CURLcode result = CURLE_FAILED_INIT;

   // Initialize cURL
   curl_global_init(CURL_GLOBAL_ALL);
   curl_handle = curl_easy_init();

   if (NULL != curl_handle) {
      if ('\0' != url[0]) {
         curl_easy_setopt(curl_handle, CURLOPT_URL, url);
         curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeCallback);
         curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&ftp);
         curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
         result = curl_easy_perform(curl_handle);

         if (CURLE_OK != result) {
            printf("ERR: curl_easy_perform(%s)\n", curl_easy_strerror(result));
         }
         curl_easy_cleanup(curl_handle);
         curl_global_cleanup();
      } else {
         printf("NOTICE: Missing URL\n");
      }
   } else {
      printf("NOTICE: Failed to initialize cURL\n");
   }

   return (CURLE_OK == result);
}

/* See ftp.h for description */
void ftpDestroyInfo(ftp_info_s **ftp) {
   if ('\0' != (*ftp)->data[0]) {
      free((*ftp)->data);
   }
   (*ftp)->data = NULL;
   (*ftp)->size = 0;
   (*ftp)->valid = false;
}
