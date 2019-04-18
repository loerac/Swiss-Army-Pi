#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/wireless.h>

#include "net_api.h"

int main(int argc, char *argv[]) {
   int retval = -1;

   if (netInit()) {
      retval = 1;
      if (internetAvail()) {
         retval = 0;
         signal_info_s *const sig_info = getInternetInfo();
         //printf("Internet is available - wireless name(%s)\n", iwname);
         printf("SSID------- %s\n", sig_info->ssid);
         printf("Level------ %d dBm\n", sig_info->level);

         if (sig_info->level <= -30) {
            sig_info->strength = SIGNAL_STRENGTH_STRONG;
            printf("Strong signal strength\n");
         } else if ((-30 < sig_info->level) && (sig_info->level <= -60)) {
            sig_info->strength = SIGNAL_STRENGTH_GOOD;
            printf("Good signal strength\n");
         } else if ((-60 < sig_info->level) && (sig_info->level <= -70)) {
            sig_info->strength = SIGNAL_STRENGTH_WEAK;
            printf("Reliable/weak signal strength\n");
         } else {
            sig_info->strength = SIGNAL_STRENGTH_BAD;
            printf("Unstable signal strength\n");
         }
      }
   }

   return retval;
}
