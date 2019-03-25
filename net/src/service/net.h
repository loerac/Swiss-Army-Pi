#ifndef _NET_H_
#define _NET_H_

#include "net_types.h"

#define MAX_SSID_LENGTH 33U

typedef struct signal_info {
   int level;
   char ssid[MAX_SSID_LENGTH];
   signal_strength_e strength;
} signal_info_s;

#endif /* _NET_H_ */
