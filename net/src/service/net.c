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

#define MAX_MAC_SIZE    18U
#define MAX_SSID_LENGTH 33U

typedef struct signal_info_s {
   int level;
   int bit_rate;
   char mac[MAX_MAC_SIZE];
   char ssid[MAX_SSID_LENGTH];
} signal_info;

bool check_wireless(const char* ifname, char* protocol) {
   int sock = -1;
   struct iwreq pwrq;
   bool failed = true;
   memset(&pwrq, 0, sizeof(pwrq));
   (void)strncpy(pwrq.ifr_name, ifname, IFNAMSIZ);

   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      printf("Failed to create socket - error(%m)\n");
      goto exit;
   }

   if (ioctl(sock, SIOCGIWNAME, &pwrq) != -1) {
      if (protocol[0] != '\0') {
          (void)strncpy(protocol, pwrq.u.name, IFNAMSIZ);
      }
      (void)close(sock);
      goto exit;
   }

   (void)close(sock);
   failed = false;

exit:
   return failed;
}

bool getNetworkName(char *iwname) {
   bool failed = true;
   struct ifaddrs *ifaddr, *ifa;

   if (getifaddrs(&ifaddr) == -1) {
      printf("Failed to get interface address - error(%m)\n");
      goto exit;
   }

   for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
      char protocol[IFNAMSIZ]  = {0};
      if (  ifa->ifa_addr == NULL
         || ifa->ifa_addr->sa_family != AF_PACKET ) {
         continue;
      }

      if (check_wireless(ifa->ifa_name, protocol)) {
         (void)strncpy(iwname,ifa->ifa_name, strlen(ifa->ifa_name));
      }
   }

   freeifaddrs(ifaddr);
   failed = (iwname[0] != '\0') ? false:true;

exit:
   return failed;
}

bool getSignalInfo(signal_info *sig_info, char *iwname) {
   struct iwreq req;
   bool failed = true;
   struct iw_statistics * stats;
   (void)strncpy(req.ifr_name, iwname, sizeof(req.ifr_name));

   int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
   req.u.data.pointer = (struct iw_statistic *)malloc(sizeof(* stats));
   req.u.data.length = sizeof(* stats);
   req.u.data.flags = 1;

   if (ioctl(sockfd, SIOCGIWSTATS, &req) == -1) {
      fprintf(stderr, "Invalid interface\n");
      goto exit;
   } else if (((struct iw_statistics *)req.u.data.pointer)->qual.updated & IW_QUAL_DBM) {
      sig_info->level = ((struct iw_statistics *)req.u.data.pointer)->qual.level - 256;
   }

   char buffer[32];
   memset(buffer, 0, 32);
   req.u.essid.pointer = buffer;
   req.u.essid.length = 32;
   if (ioctl(sockfd, SIOCGIWESSID, &req) == -1) {
      fprintf(stderr, "Invalid SSID\n");
      goto exit;
   } else {
      memcpy(&sig_info->ssid, req.u.essid.pointer, req.u.essid.length);
      memset(&sig_info->ssid[req.u.essid.length], 0, 1);
   }

   int bit_rate = -1;
   if (ioctl(sockfd, SIOCGIWRATE, &req) == -1) {
      fprintf(stderr, "Invalid bite\n");
      goto exit;
   } else {
      memcpy(&bit_rate, &req.u.bitrate, sizeof(int));
      sig_info->bit_rate = bit_rate / 1000000;
   }

   struct ifreq req2;
   strcpy(req2.ifr_name, iwname);
   if (ioctl(sockfd, SIOCGIFHWADDR, &req2) == -1) {
      fprintf(stderr, "Invalid MAC address\n");
      goto exit;
   } else {
      (void)snprintf(sig_info->mac, 18, "%.2X", (unsigned char)req2.ifr_hwaddr.sa_data[0]);
      int i = 1;
      for (; i < 6; i++) {
         (void)snprintf(sig_info->mac+strlen(sig_info->mac), 18, ":%.2X", (unsigned char)req2.ifr_hwaddr.sa_data[i]);
      }
   }
   (void)close(sockfd);
   failed = false;

exit:
   return failed;
}

int main(int argc, char *argv[]) {
   char iwname[16] = { '\0' };
   if (getNetworkName(iwname)) {
      printf("No valid network name found\n");
      goto exit;
   }
   signal_info sig_info;
   if (getSignalInfo(&sig_info, iwname)) {
      printf("Failed\n");
      goto exit;
   }

   printf("SSID------- %s\n", sig_info.ssid);
   printf("MAC-------- %s\n", sig_info.mac);
   printf("Level------ %d\n", sig_info.level);
   printf("Bit Rate--- %d\n", sig_info.bit_rate);

exit:
   return 0;
}
