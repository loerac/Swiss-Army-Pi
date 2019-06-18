#include "net_api.h"
#include "ais_parser.h"

#include <stdio.h>

#define ISS_API "http://api.open-notify.org/iss-now.json"

int main(int argc, char *argv[]) {
   (void)nasaConf();

   if (netInit() && internetAvail()) {
      ftp_info_s ftp = {0};
      if (ftpGet(&ftp, ISS_API)) {
         (void)issParser(ftp);
      }
   }

   return 0;
}
