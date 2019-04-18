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

static bool internet_avail = false;
static signal_info_s *signal = {0};

/**********************************************
 * INPUT:
 *    ifname
 *       Interface name found
 * OUTPUT:
 *    NONE
 * RETURN:
 *    True on a valid wireless name, else false
 * DESCRIPTION:
 *    Check if the interface name is a value of
 *    the Wireless Extensions
 **********************************************/
static bool validWireless(const char *const ifname) {
   int sock = -1;
   struct iwreq pwrq;
   bool ok = false;
   (void)memset(&pwrq, 0, sizeof(pwrq));
   (void)strncpy(pwrq.ifr_name, ifname, IFNAMSIZ);

   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      printf("ERR: Failed to create socket - error(%m)\n");
   } else if (ioctl(sock, SIOCGIWNAME, &pwrq) != -1) {
      (void)close(sock);
      ok = true;
   }
   (void)close(sock);

   return ok;
}

/**********************************************
 * INPUT:
 *    NONE
 * OUTPUT:
 *    iwname
 *       Wirelss network name
 * RETURN:
 *    True on able to get a valid wirelss network,
 *    else false
 * DESCRIPTION:
 *    Check all the wirelss networks on the card
 *    and check if they're a valid network
 **********************************************/
static bool getNetworkName(char *iwname) {
   bool ok = false;
   struct ifaddrs *ifaddr, *ifa;

   if (getifaddrs(&ifaddr) == -1) {
      printf("ERR: Failed to get interface address - error(%m)\n");
   } else {
      for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
         if (  (ifa->ifa_addr == NULL) ||
               (ifa->ifa_addr->sa_family != AF_PACKET)) {
            continue;
         }

         if (validWireless(ifa->ifa_name)) {
            (void)strncpy(iwname,ifa->ifa_name, strlen(ifa->ifa_name));
            ok = true;
         }
      }
      freeifaddrs(ifaddr);
   }

   return ok;
}

/**********************************************
 * INPUT:
 *    iwname
 *       Wirelss network name
 * OUTPUT:
 *    sig_info
 *       Information about the Wireless network
 * RETURN:
 *    True on able to get information, else false
 * DESCRIPTION:
 *    Get the SSID and signal strength of the
 *    wireless network we're using.
 **********************************************/
static bool getSignalInfo(signal_info_s *const sig_info, const char *const iwname) {
   struct iwreq req;
   bool ok = true;
   struct iw_statistics *stats;
   static bool has_not_logged = true;
   (void)strncpy(req.ifr_name, iwname, sizeof(req.ifr_name));

   int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
   req.u.data.pointer = (struct iw_statistic *)malloc(sizeof(* stats));
   if (NULL == req.u.data.pointer) {
      printf("EMERG: Allocating memory failed - error(%m)\n");
      ok = false;
   }
   req.u.data.length = sizeof(* stats);
   req.u.data.flags = 1;

   /* Check if Wirless name has internet access */
   if (-1 == ioctl(sockfd, SIOCGIWSTATS, &req)) {
      if (has_not_logged) {
         printf("ERR: Invalid interface - error(%m)\n");
         has_not_logged = false;
      }
      ok = false;
   } else {
      internet_avail = true;

      /* Get signal stregth */
      if (((struct iw_statistics *)req.u.data.pointer)->qual.updated & IW_QUAL_DBM) {
         sig_info->level = ((struct iw_statistics *)req.u.data.pointer)->qual.level - 256;
      }
   }

   /* Get the SSID */
   if (ok) {
      char buffer[32];
      (void)memset(buffer, 0, 32);
      req.u.essid.pointer = buffer;
      req.u.essid.length = 32;
      if (ioctl(sockfd, SIOCGIWESSID, &req) == -1) {
         printf("ERR: Invalid SSID - error(%m)\n");
         ok = false;
      } else {
         (void)memcpy(&sig_info->ssid, req.u.essid.pointer, req.u.essid.length);
         (void)memset(&sig_info->ssid[req.u.essid.length], 0, 1);
      }
   }
   (void)close(sockfd);

   return ok;
}

/**********************************************
 * See net_api.h for description.
 **********************************************/
bool netInit( void ) {
   bool ok = false;
   char dev_name[MAX_DEV_LEN + 1U] = {0};

   if (getNetworkName(dev_name)) {
      if (getSignalInfo(signal, dev_name)) {
         ok = true;
      } else {
         printf("NOTICE: Failed to retrieve information on wireless device %s\n", dev_name);
      }
   } else {
      printf("NOTICE: No valid network name found\n");
   }

   return ok;
}

/**********************************************
 * See net_api.h for description.
 **********************************************/
signal_info_s *getInternetInfo( void ) {
   return signal;
}

/**********************************************
 * See net_api.h for description.
 **********************************************/
const bool internetAvail( void ) {
   return internet_avail;
}
