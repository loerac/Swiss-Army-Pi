#include "net_api.h"
#include "ais_parser.h"
#include "type_compat.h"

#include <stdio.h>
#include <stdbool.h>

#define ISS_API "http://api.open-notify.org/iss-now.json"
#define NASA_APOD "https://api.nasa.gov/planetary/apod?api_key="

int main(int argc, char *argv[]) {
   if ( nasaConf() ) {
      char nasa_apod[85] = {0};
      isnprintf(nasa_apod, sizeof(nasa_apod), "%s%s", NASA_APOD, nasaGetKey());

      if (netInit() && internetAvail()) {
         ftp_info_s iss_ftp = {0};
         ftp_info_s nasa_ftp = {0};

         if (ftpGet(&iss_ftp, ISS_API)) {
            if (!issParser(iss_ftp)) {
               printf("Parsing ISS failed\n");
            }
         }

         if (ftpGet(&nasa_ftp, nasa_apod)) {
            if (!nasaParser(nasa_ftp)) {
               printf("Parsing NASA failed\n");
            }
         }
      }
   }

   return 0;
}
